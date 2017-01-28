/*
   File:    jagcompilerpanel.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 13, 2014
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

#include "jagcompilerpanel.h"
#include "../../moose/module/jagmodulecontentwo.h"
#include "../../moose/module/jagmodulesettingswo.h"
#include <moose.h>
#include <string.h>

#include "jaggladecompiler.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagCompilerPanel"
#include <logging/catlog.h>

struct _JagCompilerPanelPrivate {
	MooNodeWo *e_moo_node;
	JagModuleSettingsWo *e_jag_module_settings;
	JagPreferencesWo *a_jag_prefs;

	GtkWidget *w_cmb_selected_compiler;
	gulong s_cmb_selected_compiler;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagCompilerPanel, jag_compiler_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(JagCompilerPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);
static void l_refresh_compiler_list(JagCompilerPanel *compiler_panel);
static void l_selected_compiler_changed(GtkWidget *box, gpointer user_data);
static void l_post_new_jag_module_settings(JagCompilerPanel *panel, JagModuleSettingsWo *new_settings);

static void jag_compiler_panel_class_init(JagCompilerPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_compiler_panel_init(JagCompilerPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagCompilerPanel *instance = JAG_COMPILER_PANEL(object);
	JagCompilerPanelPrivate *priv = jag_compiler_panel_get_instance_private(instance);
	cat_unref_ptr(priv->e_moo_node);
	cat_unref_ptr(priv->e_jag_module_settings);
	cat_unref_ptr(priv->a_jag_prefs);
	G_OBJECT_CLASS(jag_compiler_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_compiler_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagCompilerPanel *jag_compiler_panel_new(JagPreferencesWo *a_jag_prefs) {
	JagCompilerPanel *result = g_object_new(JAG_TYPE_COMPILER_PANEL, NULL);
	cat_ref_anounce(result);
	JagCompilerPanelPrivate *priv = jag_compiler_panel_get_instance_private(result);
	cow_panel_construct((CowPanel *) result);

	priv->e_moo_node = NULL;
	priv->e_jag_module_settings = NULL;
	priv->a_jag_prefs = cat_ref_ptr(a_jag_prefs);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) jaggladecompiler_glade, (gsize) jaggladecompiler_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_cmb_selected_compiler = (GtkWidget *) gtk_builder_get_object(builder, "cmb_jdk_or_jre");

	GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
	gtk_combo_box_set_model(GTK_COMBO_BOX(priv->w_cmb_selected_compiler), GTK_TREE_MODEL(store));
	gtk_combo_box_set_entry_text_column(GTK_COMBO_BOX(priv->w_cmb_selected_compiler), 0);
	cat_unref (store);
	GtkCellRenderer *rend = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start( GTK_CELL_LAYOUT( priv->w_cmb_selected_compiler), rend, TRUE );

	priv->s_cmb_selected_compiler = g_signal_connect(priv->w_cmb_selected_compiler, "changed" , G_CALLBACK(l_selected_compiler_changed), result);
	l_refresh_compiler_list(result);

	return result;
}



static void l_refresh_compiler_list(JagCompilerPanel *compiler_panel) {
	JagCompilerPanelPrivate *priv = jag_compiler_panel_get_instance_private(compiler_panel);

	GtkTreeIter sibling;
	GtkListStore *list_store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_selected_compiler)));

	gtk_list_store_clear(list_store);


	CatArrayWo *a_jre_list = NULL;
	if (priv->a_jag_prefs) {
		a_jre_list = jag_preferences_wo_get_jre_list(priv->a_jag_prefs);
	}

	cat_log_debug("a_jre_list=%p", a_jre_list);

	if (a_jre_list) {
		CatIIterator *iterator = cat_array_wo_iterator(a_jre_list);
		while(cat_iiterator_has_next(iterator)) {
			JagPrefsJreWo *a_jre = (JagPrefsJreWo *) cat_iiterator_next(iterator);
			CatStringWo *jre_name = jag_prefs_jre_wo_get_name(a_jre);
			cat_log_debug("jre_name=%o", jre_name);
			gtk_list_store_append(list_store, &sibling);
			gtk_list_store_set(list_store, &sibling, 0, cat_string_wo_getchars(jre_name), 1, a_jre, -1);
		}
		cat_unref_ptr(iterator);
	}
}


static void l_selected_compiler_changed(GtkWidget *box, gpointer user_data) {
	JagCompilerPanel *instance = JAG_COMPILER_PANEL(user_data);
	JagCompilerPanelPrivate *priv = jag_compiler_panel_get_instance_private(instance);


	gchar *cmbtxt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(box));
	CatStringWo *a_new_jre_name = cat_string_wo_new_with(cmbtxt);

	if (priv->e_jag_module_settings) {
		CatStringWo *a_cur_name = jag_module_settings_wo_get_jre_name(priv->e_jag_module_settings);
		if (!cat_string_wo_equal(a_cur_name, a_new_jre_name)) {
			jag_module_settings_wo_set_jre_name(priv->e_jag_module_settings, a_new_jre_name);
			l_post_new_jag_module_settings(instance, priv->e_jag_module_settings);
		}
	}
}


static void l_list_set_selected_compiler(JagCompilerPanel *compiler_panel, CatStringWo *a_selected_name) {
	JagCompilerPanelPrivate *priv = jag_compiler_panel_get_instance_private(compiler_panel);

	if (a_selected_name==NULL) {
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_selected_compiler), NULL);
		return;
	}

	GtkTreeModel *tree_model = gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_selected_compiler));
	GtkTreeIter child;

	gboolean did_set_active = FALSE;
	gboolean got_child = gtk_tree_model_iter_children(tree_model, &child, NULL);
	while(got_child) {
		JagPrefsJreWo *a_lm_jre;
		gtk_tree_model_get(tree_model, &child, 1, &a_lm_jre, -1);
		CatStringWo *a_lm_jre_name = jag_prefs_jre_wo_get_name(a_lm_jre);
		if (cat_string_wo_equal(a_lm_jre_name, a_selected_name)) {
			gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_selected_compiler), &child);
			did_set_active = TRUE;
			break;
		}
		got_child = gtk_tree_model_iter_next(tree_model, &child);
	}

	if (!did_set_active) {
		GtkListStore *store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(priv->w_cmb_selected_compiler)));
		GtkTreeIter add_iter;
		gtk_list_store_append(store, &(add_iter));
		gtk_list_store_set(store, &(add_iter), 0, strdup(cat_string_wo_getchars(a_selected_name)), 1, NULL, -1);
		GtkTreePath *tree_path = gtk_tree_model_get_path(tree_model, &(add_iter));
		gtk_tree_model_row_changed(tree_model, tree_path, &(add_iter));
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(priv->w_cmb_selected_compiler), &(add_iter));
	}
}


static void l_post_new_jag_module_settings(JagCompilerPanel *panel, JagModuleSettingsWo *new_settings) {
	JagCompilerPanelPrivate *priv = jag_compiler_panel_get_instance_private(panel);
	cat_log_error("posting-new=%o", new_settings);
	if (priv->e_moo_node==NULL) {
		return;
	}
	JagModuleContentWo *module_content = (JagModuleContentWo *) moo_node_wo_get_content(priv->e_moo_node, jag_module_content_wo_key());
	if (module_content==NULL) {
		return;
	}
	JagModuleContentWo *e_module_content = jag_module_content_wo_ensure_editable(module_content);
	if (e_module_content!=module_content) {
		moo_node_wo_set_content(priv->e_moo_node, (MooIContent *) e_module_content);
	}
	cat_unref(e_module_content);

	gboolean did_change = jag_module_content_wo_set_module_settings(e_module_content, new_settings);
	cat_log_error("did-change=%d", did_change);
	cow_panel_set_configuration(COW_PANEL(panel), (GObject *) priv->e_moo_node);
}


static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	JagCompilerPanel *instance = JAG_COMPILER_PANEL(self);
	JagCompilerPanelPrivate *priv = jag_compiler_panel_get_instance_private(instance);
	MooNodeWo *e_moo_node = MOO_NODE_WO(config);
	cat_ref_swap(priv->e_moo_node, e_moo_node);
	if (e_moo_node) {

		JagModuleContentWo *module_content = (JagModuleContentWo *) moo_node_wo_get_content(e_moo_node, jag_module_content_wo_key());
		JagModuleSettingsWo *module_settings = module_content==NULL ? NULL : jag_module_content_wo_get_module_settings(module_content);

		cat_log_debug("node=%o, module_content=%o, module_settings=%o", e_moo_node, module_content, module_settings);

		if (jag_module_settings_wo_is_fixed(module_settings)) {
			JagModuleSettingsWo *e_module_settings = jag_module_settings_wo_ensure_writable(module_settings);
			module_settings = e_module_settings;
			cat_ref_swap(priv->e_jag_module_settings, module_settings);
			cat_unref_ptr(module_settings);
		} else {
			cat_ref_swap(priv->e_jag_module_settings, module_settings);
		}

	} else {
		cat_unref_ptr(priv->e_jag_module_settings);
	}


	gboolean has_config = priv->e_jag_module_settings!=NULL;
	if (has_config) {
		CatStringWo *a_selected_compiler = jag_module_settings_wo_get_jre_name(priv->e_jag_module_settings);
		g_signal_handler_block(priv->w_cmb_selected_compiler, priv->s_cmb_selected_compiler);

		CatStringWo *tmp = cat_string_wo_new_with(cat_string_wo_getchars(a_selected_compiler));
		tmp = cat_string_wo_anchor(tmp, 0);

		l_list_set_selected_compiler(instance, tmp);
		cat_unref_ptr(tmp);
		g_signal_handler_unblock(priv->w_cmb_selected_compiler, priv->s_cmb_selected_compiler);
	}
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

