/*
   File:    draprefsspellingpanel.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jan 1, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "draprefsspellingpanel.h"
#include "../drapreferenceswo.h"

#include "dragladeprefsspelling.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "DraPrefsSpellingPanel"
#include <logging/catlog.h>

struct _DraPrefsSpellingPanelPrivate {
	CowIEntryAccessor *entry_accessor;
	DraSpellHelper *spell_helper;
	CatWo *e_prefs;
	DraPrefsSpellingWo *e_prefs_spelling;

	CatArrayWo *language_list;

	GtkWidget *w_chk_enabled;
	gulong s_chk_enabled;
	GtkWidget *w_cmb_dict;
	gulong s_cmb_dict;
	GtkWidget *w_spn_max_nr_props;
	gulong s_spn_max_nr_props;

	GtkWidget *w_grid_spelling;


};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsSpellingPanel, dra_prefs_spelling_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(DraPrefsSpellingPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);


static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void dra_prefs_spelling_panel_class_init(DraPrefsSpellingPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_spelling_panel_init(DraPrefsSpellingPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPrefsSpellingPanel *instance = DRA_PREFS_SPELLING_PANEL(object);
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(instance);
	cat_unref_ptr(priv->entry_accessor);
	G_OBJECT_CLASS(dra_prefs_spelling_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_spelling_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_enabled_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static void l_dict_changed(GtkWidget *cmb_dict , gpointer user_data);
static void l_refresh_language_list(DraPrefsSpellingPanel *panel);
static void l_max_nr_props_changed(GtkSpinButton *spin_button, gpointer user_data);


DraPrefsSpellingPanel *dra_prefs_spelling_panel_new(CowIEntryAccessor *entry_accessor, DraSpellHelper *spell_helper) {
	DraPrefsSpellingPanel *result = g_object_new(DRA_TYPE_PREFS_SPELLING_PANEL, NULL);
	cat_ref_anounce(result);
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(result);
	priv->entry_accessor = cat_ref_ptr(entry_accessor);
	priv->spell_helper = cat_ref_ptr(spell_helper);
	priv->language_list = NULL;
	priv->e_prefs_spelling = NULL;
	priv->e_prefs = NULL;
	cow_panel_construct((CowPanel *) result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) dragladeprefsspelling_glade, (gsize) dragladeprefsspelling_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_chk_enabled = (GtkWidget *) gtk_builder_get_object(builder, "chkSpellEnable");
	priv->s_chk_enabled  = g_signal_connect(priv->w_chk_enabled, "toggled", G_CALLBACK(l_enabled_toggled), result);



	priv->w_cmb_dict = (GtkWidget *) gtk_builder_get_object(builder, "cbDict");
	GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_OBJECT);
	gtk_combo_box_set_model(GTK_COMBO_BOX(priv->w_cmb_dict), GTK_TREE_MODEL(store));
	cat_unref (store);
	GtkCellRenderer *rend = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start( GTK_CELL_LAYOUT( priv->w_cmb_dict), rend, TRUE );
	priv->s_cmb_dict = g_signal_connect(priv->w_cmb_dict, "changed" , G_CALLBACK(l_dict_changed), result);

	priv->w_grid_spelling = (GtkWidget *) gtk_builder_get_object(builder, "grid3");

	priv->w_spn_max_nr_props = (GtkWidget *) gtk_builder_get_object(builder, "spnMaxNrProps");
	priv->s_spn_max_nr_props = g_signal_connect(priv->w_spn_max_nr_props, "value-changed" , G_CALLBACK(l_max_nr_props_changed), result);



	l_refresh_language_list(result);

	return result;
}

static gboolean l_notify_modification(DraPrefsSpellingPanel *panel) {
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);
	cow_panel_set_configuration((CowPanel *) panel, (GObject *) priv->e_prefs);
	return TRUE;
}

static void l_dict_changed(GtkWidget *cmb_dict , gpointer user_data) {
	DraPrefsSpellingPanel *panel = DRA_PREFS_SPELLING_PANEL(user_data);
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);

	gchar *cmbtxt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(cmb_dict));
	CatStringWo *new_dict = cat_string_wo_new_with(cmbtxt);

	dra_prefs_spelling_wo_set_dictionary_name(priv->e_prefs_spelling, new_dict);
	cat_unref_ptr(new_dict);

	l_notify_modification(panel);
}

static gboolean l_enabled_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsSpellingPanel *panel = DRA_PREFS_SPELLING_PANEL(user_data);
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);
	gboolean is_spelling_enabled = gtk_toggle_button_get_active(toggle_button);
	dra_prefs_spelling_wo_set_enabled(priv->e_prefs_spelling, is_spelling_enabled);

	gtk_widget_set_sensitive(priv->w_grid_spelling, is_spelling_enabled);

	return l_notify_modification(panel);
}

static void l_max_nr_props_changed(GtkSpinButton *spin_button, gpointer user_data) {
	DraPrefsSpellingPanel *panel = DRA_PREFS_SPELLING_PANEL(user_data);
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);
	int val = gtk_spin_button_get_value_as_int(priv->w_spn_max_nr_props);
	cat_log_error("valu = %d", val);
	dra_prefs_spelling_wo_set_max_suggestions(priv->e_prefs_spelling, val);
	l_notify_modification(panel);
}

static CatArrayWo *l_enlist_languages(DraPrefsSpellingPanel *panel) {
	CatArrayWo *result = cat_array_wo_new();
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);
	if (priv->spell_helper) {
		cat_unref_ptr(result);
		result = dra_spell_helper_enlist_languages(priv->spell_helper);
	}
	return result;
}


static void l_refresh_language_list(DraPrefsSpellingPanel *panel) {
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);

	CatArrayWo *language_list = l_enlist_languages(panel);
	cat_array_wo_sort(language_list, (GCompareFunc) cat_string_wo_compare);

	GtkListStore *store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_dict)));
	gtk_list_store_clear(store);

	GtkTreeIter iter;
	CatIIterator *iterator = cat_array_wo_iterator(language_list);
	while(cat_iiterator_has_next(iterator)) {
		CatStringWo *item = (CatStringWo *) cat_iiterator_next(iterator);
		cat_log_error("item=%O", item);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 0, cat_string_wo_getchars(item), 1, item, -1);
//		if (grammar_file_name!=NULL && g_string_equal(grammar_file_name, fname)) {
//			gtk_combo_box_set_active_iter(GTK_COMBO_BOX(this->combo_textpad_syntax_file), &iter);
//		}
	}
	cat_unref_ptr(iterator);

	cat_ref_swap(priv->language_list, language_list);
	cat_unref_ptr(language_list);
}



static void l_list_set_active_dictionary(DraPrefsSpellingPanel *panel, CatStringWo *to_activate) {
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);

	if (to_activate==NULL) {
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_dict), NULL);
		return;
	}

	GtkTreeModel *tree_model = gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_dict));
	GtkTreeIter child;

	gboolean did_set_active = FALSE;
	gboolean got_child = gtk_tree_model_iter_children(tree_model, &child, NULL);
	while(got_child) {
		CatStringWo *gram_name;
		gtk_tree_model_get(tree_model, &child, 1, &gram_name, -1);
		if (cat_string_wo_equal(gram_name, to_activate)) {
			gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_dict), &child);
			did_set_active = TRUE;
			break;
		}
		got_child = gtk_tree_model_iter_next(tree_model, &child);
	}

	if (!did_set_active) {
		GtkListStore *store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_dict)));
		GtkTreeIter add_iter;
		gtk_list_store_append(store, &(add_iter));
		gtk_list_store_set(store, &(add_iter), 0, cat_string_wo_getchars(to_activate), 1, to_activate, -1);
		GtkTreePath *tree_path = gtk_tree_model_get_path(tree_model, &(add_iter));
		gtk_tree_model_row_changed(tree_model, tree_path, &(add_iter));
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_dict), &(add_iter));
	}
}


static void l_refresh_form(DraPrefsSpellingPanel *panel) {
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(panel);

	g_signal_handler_block(priv->w_cmb_dict, priv->s_cmb_dict);
	g_signal_handler_block(priv->w_chk_enabled, priv->s_chk_enabled);
	g_signal_handler_block(priv->w_spn_max_nr_props, priv->s_spn_max_nr_props);

	gboolean has_prefs = priv->e_prefs_spelling!=NULL;

	gtk_widget_set_sensitive(priv->w_cmb_dict, has_prefs);
	gtk_widget_set_sensitive(priv->w_chk_enabled, has_prefs);
	gtk_widget_set_sensitive(priv->w_spn_max_nr_props, has_prefs);

	cat_log_error("has_prefs=%s", has_prefs ? "true" : "false");

	if (has_prefs) {
		gboolean is_spelling_enabled = dra_prefs_spelling_wo_is_enabled(priv->e_prefs_spelling);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_chk_enabled), is_spelling_enabled);
		l_list_set_active_dictionary(panel, dra_prefs_spelling_wo_get_dictionary_name(priv->e_prefs_spelling));
		cat_log_error("prefs.dict-name=%O", dra_prefs_spelling_wo_get_dictionary_name(priv->e_prefs_spelling));
		gtk_widget_set_sensitive(priv->w_grid_spelling, is_spelling_enabled);
		gtk_spin_button_set_value(priv->w_spn_max_nr_props, dra_prefs_spelling_wo_get_max_suggestions(priv->e_prefs_spelling));
	} else {
		gtk_widget_set_sensitive(priv->w_grid_spelling, FALSE);
	}

	g_signal_handler_unblock(priv->w_spn_max_nr_props, priv->s_spn_max_nr_props);
	g_signal_handler_unblock(priv->w_cmb_dict, priv->s_cmb_dict);
	g_signal_handler_unblock(priv->w_chk_enabled, priv->s_chk_enabled);
}



static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	DraPrefsSpellingPanel *instance = DRA_PREFS_SPELLING_PANEL(self);
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(instance);

	cat_log_debug("config=%o", config);
	CatWo *e_prefs = CAT_WO(config);
	if (e_prefs!=NULL) {
		e_prefs = cat_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);
	}
	cat_unref_ptr(priv->e_prefs);
	priv->e_prefs_spelling = NULL;
	priv->e_prefs = e_prefs;

	if (e_prefs!=NULL) {

		cat_log_debug("e_prefs=%o", e_prefs);
		DraPreferencesWo *ast_prefs = (DraPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, e_prefs);
		DraPreferencesWo *e_dra_prefs = NULL;
		if (ast_prefs == NULL) {
			e_dra_prefs = dra_preferences_wo_new();
		} else {
			e_dra_prefs = dra_preferences_wo_clone(ast_prefs, CAT_CLONE_DEPTH_MAIN);
		}
		cow_ientry_accessor_set(priv->entry_accessor, e_prefs, e_dra_prefs);
		cat_log_debug("e_dra_prefs=%p", e_dra_prefs);
		priv->e_prefs_spelling = dra_preferences_wo_get_editable_spelling(e_dra_prefs);
		cat_unref_ptr(e_dra_prefs);
	}

	cat_log_debug("e_prefs=%o", e_prefs);


	l_refresh_form(instance);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPrefsSpellingPanel *instance = DRA_PREFS_SPELLING_PANEL(self);
	DraPrefsSpellingPanelPrivate *priv = dra_prefs_spelling_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p e_prefs_spelling=%p]", iname, self, priv->e_prefs_spelling);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

