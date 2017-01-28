/*
   File:    terprefsclazzpanel.c
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 27, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "terprefsclazzpanel.h"
#include "../terpreferenceswo.h"
#include "../terprefsmapper.h"
#include <elk.h>
#include <string.h>

#include "tergladeprefsclazz.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerPrefsClazzPanel"
#include <logging/catlog.h>

struct _TerPrefsClazzPanelPrivate {
	CatStringWo *clazz_name;
	VipPath *shared_grammars_path;
	TerPreferencesWo *e_ter_prefs;
	TerPrefsClazzWo *e_ter_clazz;
	CatArrayWo *grammar_list;

	GtkWidget *w_cmb_grammar_file;
	gulong s_cmb_grammar_file;

	GtkWidget *w_but_open_grammar_file;
	GtkWidget *w_txt_other_characters;
	gulong s_txt_other_characters;
	GtkWidget *w_tv_patterns;
	GtkWidget *w_but_clazz_remove;

	GtkWidget *w_txt_pattern_name;
	GtkWidget *w_but_pattern_add;
	GtkWidget *w_but_pattern_remove;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerPrefsClazzPanel, ter_prefs_clazz_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(TerPrefsClazzPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);
static void l_refresh_grammar_list(TerPrefsClazzPanel *panel);
static void l_grammar_changed(GtkWidget *cmb_grammar , gpointer user_data);
static void l_other_chars_changed(GtkWidget *widget , gpointer user_data);
static void l_clicked_pattern_set_or_add(GtkButton *button, gpointer user_data);
static void l_pattern_cell_data_cb(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector);

static void ter_prefs_clazz_panel_class_init(TerPrefsClazzPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_prefs_clazz_panel_init(TerPrefsClazzPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerPrefsClazzPanel *instance = TER_PREFS_CLAZZ_PANEL(object);
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(instance);
	cat_unref_ptr(priv->e_ter_prefs);
	G_OBJECT_CLASS(ter_prefs_clazz_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_prefs_clazz_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerPrefsClazzPanel *ter_prefs_clazz_panel_new(TerPrefsClazzWo *prefs_clazz, VipPath *shared_grammars_path) {
	TerPrefsClazzPanel *result = g_object_new(TER_TYPE_PREFS_CLAZZ_PANEL, NULL);
	cat_ref_anounce(result);
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(result);
	priv->e_ter_prefs = NULL;
	priv->shared_grammars_path = cat_ref_ptr(shared_grammars_path);
	priv->clazz_name = ter_prefs_clazz_wo_get_name(prefs_clazz);
	cow_panel_construct((CowPanel *) result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) tergladeprefsclazz_glade, (gsize) tergladeprefsclazz_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_cmb_grammar_file = (GtkWidget *) gtk_builder_get_object(builder, "cmb_grammar_file");
	GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_OBJECT);
	gtk_combo_box_set_model(GTK_COMBO_BOX(priv->w_cmb_grammar_file), GTK_TREE_MODEL(store));
	cat_unref (store);
	GtkCellRenderer *rend = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start( GTK_CELL_LAYOUT( priv->w_cmb_grammar_file), rend, TRUE );
	priv->s_cmb_grammar_file = g_signal_connect(priv->w_cmb_grammar_file, "changed" , G_CALLBACK(l_grammar_changed), result);


	priv->w_but_open_grammar_file = (GtkWidget *) gtk_builder_get_object(builder, "but_open_grammar_file");

	priv->w_txt_other_characters = (GtkWidget *) gtk_builder_get_object(builder, "txt_other_characters");
	priv->s_txt_other_characters = g_signal_connect(priv->w_txt_other_characters, "changed" , G_CALLBACK(l_other_chars_changed), result);


	GtkWidget *w_sw_patterns = (GtkWidget *) gtk_builder_get_object(builder, "sw_patterns");


//	priv->w_tv_patterns = (GtkWidget *) gtk_builder_get_object(builder, "tv_patterns");
//	gtk_tree_view_set_model(priv->w_tv_patterns, (GtkTreeModel *) list_model);
	GtkListStore *list_model = gtk_list_store_new(1, G_TYPE_POINTER);
	priv->w_tv_patterns = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_model));
	gtk_container_remove(GTK_CONTAINER(w_sw_patterns), gtk_bin_get_child(GTK_BIN(w_sw_patterns)));
	gtk_container_add(GTK_CONTAINER(w_sw_patterns), priv->w_tv_patterns);


	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_renderer_set_padding(renderer, 0, 0);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(priv->w_tv_patterns), 0, "Name",renderer, (GtkTreeCellDataFunc) l_pattern_cell_data_cb, priv, NULL);



	cat_log_debug("priv->w_tv_patterns=%p", priv->w_tv_patterns);

	priv->w_txt_pattern_name = (GtkWidget *) gtk_builder_get_object(builder, "txt_pattern_name");
	priv->w_but_pattern_add = (GtkWidget *) gtk_builder_get_object(builder, "but_pattern_add");
	g_signal_connect(priv->w_but_pattern_add, "clicked", G_CALLBACK(l_clicked_pattern_set_or_add), result);

	priv->w_but_pattern_remove = (GtkWidget *) gtk_builder_get_object(builder, "w_but_pattern_remove");


	priv->w_but_clazz_remove = (GtkWidget *) gtk_builder_get_object(builder, "but_clazz_remove");



	l_refresh_grammar_list(result);

	return result;
}


static void l_pattern_cell_data_cb(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	void *data;
	gtk_tree_model_get(model, iter, 0, &data, -1);
	CatStringWo *pattern = (CatStringWo *) data;
	g_object_set(renderer, "text", cat_string_wo_getchars(pattern), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
}


static void l_notify_modification(TerPrefsClazzPanel *panel) {
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(panel);
	CatWo *e_prefs = (CatWo *) cow_panel_get_configuration((CowPanel *) panel);
	CatWo *e_replacement = cat_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);
	TerPreferencesWo *anchored_prefs = ter_preferences_wo_anchor(priv->e_ter_prefs, 1);
	elk_preferences_wo_set_sub((ElkPreferencesWo *) e_prefs, CAT_S(ter_s_textpad), (GObject *) anchored_prefs);
	TerPreferencesWo *new_editable_prefs = ter_preferences_wo_create_editable(anchored_prefs);
	cat_unref_ptr(anchored_prefs);
	priv->e_ter_prefs = new_editable_prefs;
	cow_panel_set_configuration((CowPanel *) panel, (GObject *) e_replacement);
	cat_unref_ptr(e_replacement);
}

static void l_clicked_pattern_set_or_add(GtkButton *button, gpointer user_data) {
	TerPrefsClazzPanel *instance = TER_PREFS_CLAZZ_PANEL(user_data);
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(instance);
	GtkTreeModel *tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(priv->w_tv_patterns));


	const gchar *text = gtk_entry_get_text(GTK_ENTRY(priv->w_txt_pattern_name));
	if (text==NULL || strlen(text)==0) {
		return;
	}
	CatStringWo *new_pattern = cat_string_wo_new_with(text);


	CatArrayWo *pattern_list = ter_prefs_clazz_wo_get_pattern_list(priv->e_ter_clazz);

	if (pattern_list) {
		int idx = cat_array_wo_find_index_ext(pattern_list, new_pattern, -1, (GEqualFunc) cat_string_wo_equal);
		if (idx>=0) {
			cat_log_warn("pattern already added");
			return;
		}
	}


	gboolean did_add = FALSE;
	GtkTreeSelection *tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->w_tv_patterns));
	if (tree_selection!=NULL) {
		GtkTreeIter iter;
		if (gtk_tree_selection_get_selected(tree_selection, NULL, &iter)) {
			void *data;
			gtk_tree_model_get(tree_model, &iter, 0, &data, -1);
			CatStringWo *lm_pattern = (CatStringWo *) data;
			int index = -1;
			if (pattern_list) {
				index = cat_array_wo_find_index_ext(pattern_list, lm_pattern, -1, (GEqualFunc) cat_string_wo_equal);
			}
			if (index>=0) {
				ter_prefs_clazz_wo_set_pattern_at(priv->e_ter_clazz, new_pattern, index);
				did_add = TRUE;
			}

		}
	}

	if (!did_add) {
		ter_prefs_clazz_wo_add_pattern(priv->e_ter_clazz, new_pattern);
	}

	l_notify_modification(instance);

	cat_unref(new_pattern);
	cat_log_detail("end of set_or_add");
}




static void l_other_chars_changed(GtkWidget *widget , gpointer user_data) {
	TerPrefsClazzPanel *instance = TER_PREFS_CLAZZ_PANEL(user_data);
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(instance);

	const gchar *extra_chars = gtk_entry_get_text(GTK_ENTRY(widget));
	CatStringWo *new_extra_chars = cat_string_wo_new_with(extra_chars);

	ter_prefs_clazz_wo_set_extra_characters(priv->e_ter_clazz, new_extra_chars);
	cat_unref_ptr(new_extra_chars);
	l_notify_modification(instance);
}


static void l_grammar_changed(GtkWidget *cmb_grammar , gpointer user_data) {
	TerPrefsClazzPanel *instance = TER_PREFS_CLAZZ_PANEL(user_data);
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(instance);

	gchar *cmbtxt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(cmb_grammar));
	CatStringWo *new_grammar_location = cat_string_wo_new_with(cmbtxt);

	ter_prefs_clazz_wo_set_grammar_file(priv->e_ter_clazz, new_grammar_location);
	l_notify_modification(instance);
}

static CatArrayWo *l_enlist_textpad_grammars(TerPrefsClazzPanel *panel) {
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(panel);
	CatArrayWo *result = cat_array_wo_new();

#ifdef G_OS_UNIX
	const CatStringWo *extension = cat_string_wo_new_data(".syn");

	CatStringWo *a_full_path = vip_path_to_string(priv->shared_grammars_path);
	const gchar *full_path_chars = cat_string_wo_getchars(a_full_path);

	// TODO use viper
	DIR *dir = opendir(full_path_chars);
	cat_log_debug("dir=%s", full_path_chars);
	if (dir!=NULL) {
		struct dirent *pent;
		while ((pent=readdir(dir))) {
			CatStringWo *fname = cat_string_wo_new_with(pent->d_name);
			if (cat_string_wo_endswith(fname, extension)) {
				cat_log_debug("adding:fname=%o", fname);
				cat_string_wo_limit(fname, 0, cat_string_wo_length(fname) - 4);
				cat_array_wo_append(result, (GObject *) fname);
			}
			cat_unref(fname);
		}
		cat_log_debug("end of dir");
		closedir(dir);
	} else {
		cat_log_warn("WARNING: no directory called:%s", full_path_chars);
	}
	cat_unref_ptr(a_full_path);
#endif
	return result;
}


static void l_refresh_grammar_list(TerPrefsClazzPanel *panel) {
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(panel);

	CatArrayWo *grammar_list = l_enlist_textpad_grammars(panel);
	cat_array_wo_sort(grammar_list, (GCompareFunc) cat_string_wo_compare);

	GtkListStore *store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_grammar_file)));
	gtk_list_store_clear(store);

	GtkTreeIter iter;
	CatIIterator *iterator = cat_array_wo_iterator(grammar_list);
	while(cat_iiterator_has_next(iterator)) {
		CatStringWo *item = (CatStringWo *) cat_iiterator_next(iterator);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 0, cat_string_wo_getchars(item), 1, item, -1);
//		if (grammar_file_name!=NULL && g_string_equal(grammar_file_name, fname)) {
//			gtk_combo_box_set_active_iter(GTK_COMBO_BOX(this->combo_textpad_syntax_file), &iter);
//		}
	}
	cat_unref_ptr(iterator);

	cat_ref_swap(priv->grammar_list, grammar_list);
	cat_unref_ptr(grammar_list);
}



static void l_list_set_active_grammar(TerPrefsClazzPanel *panel, CatStringWo *to_activate) {
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(panel);

	if (to_activate==NULL) {
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_grammar_file), NULL);
		return;
	}

	GtkTreeModel *tree_model = gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_grammar_file));
	GtkTreeIter child;

	gboolean did_set_active = FALSE;
	gboolean got_child = gtk_tree_model_iter_children(tree_model, &child, NULL);
	while(got_child) {
		CatStringWo *gram_name;
		gtk_tree_model_get(tree_model, &child, 1, &gram_name, -1);
		if (cat_string_wo_equal(gram_name, to_activate)) {
			gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_grammar_file), &child);
			did_set_active = TRUE;
			break;
		}
		got_child = gtk_tree_model_iter_next(tree_model, &child);
	}

	if (!did_set_active) {
		GtkListStore *store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_grammar_file)));
		GtkTreeIter add_iter;
		gtk_list_store_append(store, &(add_iter));
		gtk_list_store_set(store, &(add_iter), 0, cat_string_wo_getchars(to_activate), 1, to_activate, -1);
		GtkTreePath *tree_path = gtk_tree_model_get_path(tree_model, &(add_iter));
		gtk_tree_model_row_changed(tree_model, tree_path, &(add_iter));
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_grammar_file), &(add_iter));
	}
}





static void l_refresh_pattern_list(TerPrefsClazzPanel *panel) {
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(panel);

	GtkTreeIter iter, sibling;
	GtkTreeModel *tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(priv->w_tv_patterns));
	GtkListStore *list_store = GTK_LIST_STORE(tree_model);
	gboolean valid_iter = gtk_tree_model_get_iter_first(tree_model, &iter);

	CatArrayWo *pattern_list = priv->e_ter_clazz==NULL ? NULL : ter_prefs_clazz_wo_get_pattern_list(priv->e_ter_clazz);
	cat_log_debug("pattern_list=%o", pattern_list);


	if (pattern_list) {
		CatIIterator *iterator = cat_array_wo_iterator(pattern_list);
		void *data;
		while(cat_iiterator_has_next(iterator)) {
			CatStringWo *pattern = (CatStringWo *) cat_iiterator_next(iterator);
			cat_log_debug("pattern=%o", pattern);
			gboolean inserted = FALSE;
			while(!inserted) {
				if (valid_iter) {
					gtk_tree_model_get(tree_model, &iter, 0, &data, -1);
					CatStringWo *lm_pattern = (CatStringWo *) data;
					if (cat_string_wo_equal(lm_pattern, pattern)) {
						valid_iter = gtk_tree_model_iter_next(tree_model, &iter);
						inserted = TRUE;
					} else {
						int idx = cat_array_wo_find_index_ext(pattern_list, lm_pattern, -1, (GCompareFunc) cat_string_wo_equal);
						if (idx>=0) {
							gtk_list_store_insert_before(list_store, &sibling, &iter);
							gtk_list_store_set(list_store, &sibling, 0, pattern, -1);
							inserted = TRUE;
						} else {
							valid_iter = gtk_list_store_remove(list_store, &iter);
						}
					}
				} else {
					gtk_list_store_append(list_store, &sibling);
					gtk_list_store_set(list_store, &sibling, 0, pattern, -1);
					inserted = TRUE;
				}
			}
		}
		cat_unref_ptr(iterator);
	}

	while(valid_iter) {
		valid_iter = gtk_list_store_remove(list_store, &iter);
	}
	cat_log_detail("end of refresh pattern list");
}


static void l_refresh_form(TerPrefsClazzPanel *panel) {
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(panel);


	l_refresh_pattern_list(panel);

	cat_log_debug("priv->e_jag_prefs=%o", priv->e_ter_prefs);
	if (priv->e_ter_clazz) {

//		g_signal_handler_block(priv->w_cmb_grammar_file, priv->s_cmb_grammar_file);
		l_list_set_active_grammar(panel, ter_prefs_clazz_wo_get_grammar_file(priv->e_ter_clazz));
//		g_signal_handler_unblock(priv->w_cmb_grammar_file, priv->s_cmb_grammar_file);


		g_signal_handler_block(priv->w_txt_other_characters, priv->s_txt_other_characters);
		CatStringWo *extra_chars = ter_prefs_clazz_wo_get_extra_characters(priv->e_ter_clazz);
		gtk_entry_set_text(GTK_ENTRY(priv->w_txt_other_characters), cat_string_wo_getchars(extra_chars));
		g_signal_handler_unblock(priv->w_txt_other_characters, priv->s_txt_other_characters);

	} else {

	}
}


static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	TerPrefsClazzPanel *instance = TER_PREFS_CLAZZ_PANEL(self);
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(instance);

	ElkPreferencesWo *e_prefs = ELK_PREFERENCES_WO(config);
	cat_log_debug("e_prefs=%o", e_prefs);
	if (e_prefs!=NULL && elk_preferences_wo_is_anchored(e_prefs)) {
		return;
	}

	if (e_prefs!=NULL) {

		cat_log_debug("e_prefs=%p", e_prefs);
		TerPreferencesWo *ter_prefs = (TerPreferencesWo *) elk_preferences_wo_get_sub(e_prefs, CAT_S(ter_s_textpad));
		TerPreferencesWo *e_ter_prefs = ter_prefs;
		cat_log_debug("e_ter_prefs=%p", e_ter_prefs);
		if (e_ter_prefs == NULL) {
			e_ter_prefs = ter_preferences_wo_new();
			elk_preferences_wo_set_sub(e_prefs, CAT_S(ter_s_textpad), (GObject *) e_ter_prefs);
			cat_unref(e_ter_prefs);
		} else if (ter_preferences_wo_is_anchored(ter_prefs)) {
			e_ter_prefs = ter_preferences_wo_create_editable(ter_prefs);
			elk_preferences_wo_set_sub(e_prefs, CAT_S(ter_s_textpad), (GObject *) e_ter_prefs);
			cat_unref(e_ter_prefs);
		}

		cat_log_debug("e_ter_prefs=%p", e_ter_prefs);
		cat_ref_swap(priv->e_ter_prefs, e_ter_prefs);


		TerPrefsClazzWo *ter_clazz = ter_preferences_wo_get_clazz(priv->e_ter_prefs, priv->clazz_name);
		if (ter_clazz==NULL) {
			ter_clazz = ter_prefs_clazz_wo_new(priv->clazz_name);
			ter_preferences_wo_set_clazz(priv->e_ter_prefs, ter_clazz);
			cat_unref(ter_clazz);
		} else if (ter_prefs_clazz_wo_is_anchored(ter_clazz)) {
			ter_clazz = ter_prefs_clazz_wo_create_editable(ter_clazz);
			ter_preferences_wo_set_clazz(priv->e_ter_prefs, ter_clazz);
			cat_unref(ter_clazz);
		}

		cat_ref_swap(priv->e_ter_clazz, ter_clazz);


	} else {
		cat_unref_ptr(priv->e_ter_prefs);
	}

	l_refresh_form(instance);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	TerPrefsClazzPanel *instance = TER_PREFS_CLAZZ_PANEL(self);
	TerPrefsClazzPanelPrivate *priv = ter_prefs_clazz_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p name=%o, e_ter_clazz=%o]", iname, self, priv->clazz_name, priv->e_ter_clazz);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
