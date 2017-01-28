/*
   File:    jagprefscompilerpanel.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 28, 2014
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

#include "jagprefscompilerpanel.h"
#include "jagprefsjredialog.h"
#include "../jagpreferenceswo.h"
#include "../jagprefsjrewo.h"
#include "../jagprefsaccessor.h"
#include <elk.h>

#include "jaggladeprefscompiler.h"


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPrefsCompilerPanel"
#include <logging/catlog.h>

struct _JagPrefsCompilerPanelPrivate {
	CatWo *e_prefs;
	JagPreferencesWo *e_jag_prefs;

	CowIEntryAccessor *entry_accessor;

	VipService *vip_service;
	GtkWidget *w_tree_view;
	GtkWidget *w_but_add;
	GtkWidget *w_but_edit;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPrefsCompilerPanel, jag_prefs_compiler_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(JagPrefsCompilerPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void jag_prefs_compiler_panel_class_init(JagPrefsCompilerPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_prefs_compiler_panel_init(JagPrefsCompilerPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPrefsCompilerPanel *instance = JAG_PREFS_COMPILER_PANEL(object);
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->e_jag_prefs);
	cat_unref_ptr(priv->entry_accessor);
	G_OBJECT_CLASS(jag_prefs_compiler_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_prefs_compiler_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_list_name_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector);
static void l_list_path_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector);
static void l_on_add_jre(GtkButton *button, gpointer user_data);
static void l_on_edit_jre(GtkButton *button, gpointer user_data);
static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data);


JagPrefsCompilerPanel *jag_prefs_compiler_panel_new(VipService *vip_service) {
	JagPrefsCompilerPanel *result = g_object_new(JAG_TYPE_PREFS_COMPILER_PANEL, NULL);
	cat_ref_anounce(result);
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->e_prefs = NULL;
	priv->e_jag_prefs = NULL;
	priv->entry_accessor = (CowIEntryAccessor *) jag_prefs_accessor_new();
	cow_panel_construct((CowPanel *) result);


	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) jaggladeprefscompiler_glade, (gsize) jaggladeprefscompiler_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_tree_view = (GtkWidget *) gtk_builder_get_object(builder, "tv_list");

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_renderer_set_padding(renderer, 0, 0);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(priv->w_tree_view), 0, "Name",renderer, (GtkTreeCellDataFunc) l_list_name_col_func, result, NULL);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(priv->w_tree_view), 1, "Path",renderer, (GtkTreeCellDataFunc) l_list_path_col_func, result, NULL);
	GtkTreeSelection *tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->w_tree_view));
	gtk_tree_selection_set_mode(tree_selection, GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(tree_selection), "changed", G_CALLBACK(l_selection_changed), result);



	priv->w_but_add = (GtkWidget *) gtk_builder_get_object(builder, "but_add");
	g_signal_connect(priv->w_but_add, "clicked", G_CALLBACK(l_on_add_jre), result);

	priv->w_but_edit = (GtkWidget *) gtk_builder_get_object(builder, "but_edit");
	g_signal_connect(priv->w_but_edit, "clicked", G_CALLBACK(l_on_edit_jre), result);
//
//	priv->w_but_remove = (GtkWidget *) gtk_builder_get_object(builder, "but_remove");
//	g_signal_connect(priv->w_but_remove, "clicked", G_CALLBACK(l_on_remove_jre), result);

	return result;
}




static gboolean l_notify_modification(JagPrefsCompilerPanel *panel) {
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(panel);
	cow_panel_set_configuration((CowPanel *) panel, (GObject *) priv->e_prefs);
	return TRUE;
}



static void l_list_name_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	JagPrefsJreWo *jre = NULL;
	gtk_tree_model_get(model, iter, 0, &jre, -1);
	CatStringWo *j_name = jag_prefs_jre_wo_get_name(jre);
	if (j_name) {
		g_object_set(renderer, "text", cat_string_wo_getchars(j_name), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	} else {
		g_object_set(renderer, "text", "<null>", "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	}
}

static void l_list_path_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	JagPrefsJreWo *jre = NULL;
	gtk_tree_model_get(model, iter, 0, &jre, -1);
	CatStringWo *j_path = jag_prefs_jre_wo_get_path(jre);
	if (j_path) {
		g_object_set(renderer, "text", cat_string_wo_getchars(j_path), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	} else {
		g_object_set(renderer, "text", "<null>", "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	}
}


static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data) {
	JagPrefsCompilerPanel *instance = JAG_PREFS_COMPILER_PANEL(user_data);
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(instance);

	GtkTreeModel *model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree_view);
	GtkTreeIter iter;
	gboolean has_selection = gtk_tree_selection_get_selected(tree_selection, &model, &iter);
	gtk_widget_set_sensitive(priv->w_but_edit, has_selection);
//	gtk_widget_set_sensitive(priv->w_but_remove, has_selection);
}


static void l_on_add_jre(GtkButton *button, gpointer user_data) {
	JagPrefsCompilerPanel *instance = JAG_PREFS_COMPILER_PANEL(user_data);
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(instance);
	JagPrefsJreWo *e_prefs_jre_wo = jag_prefs_jre_wo_new();
	JagPrefsJreDialog *jre_dialog = jag_prefs_jre_dialog_new(priv->vip_service, e_prefs_jre_wo);
	gboolean keep_open = TRUE;
	while(keep_open) {
		int result = jag_prefs_jre_dialog_run(jre_dialog);
		cat_log_debug("result=%d", result);
		if (result == GTK_RESPONSE_OK) {
			CatStringWo *new_jre_name = jag_prefs_jre_wo_get_name(e_prefs_jre_wo);

			gboolean name_exists = FALSE;
			CatArrayWo *e_jre_list = jag_preferences_wo_get_editable_jre_list(priv->e_jag_prefs);
			CatIIterator *array_iter = cat_array_wo_iterator(e_jre_list);
			while(cat_iiterator_has_next(array_iter)) {
				JagPrefsJreWo *prefs_jre = (JagPrefsJreWo *) cat_iiterator_next(array_iter);
				CatStringWo *ex_jre_name = jag_prefs_jre_wo_get_name(prefs_jre);
				if (cat_string_wo_equal(new_jre_name, ex_jre_name)) {
					name_exists = TRUE;
					break;
				}
			}
			cat_unref_ptr(array_iter);

			if (name_exists) {
				GtkWidget *msg_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Name already exists. Cannot add JRE settings.");
				gtk_dialog_run (GTK_DIALOG (msg_dialog));
				gtk_widget_destroy(msg_dialog);
			} else {
				cat_array_wo_append(e_jre_list, (GObject *) e_prefs_jre_wo);
				keep_open = FALSE;
			}
		} else {
			keep_open = FALSE;
		}
	}

	jag_prefs_jre_dialog_close(jre_dialog);
	cat_unref_ptr(e_prefs_jre_wo);
	cat_unref_ptr(jre_dialog);


	l_notify_modification(instance);
}


static void l_on_edit_jre(GtkButton *button, gpointer user_data) {
	JagPrefsCompilerPanel *instance = JAG_PREFS_COMPILER_PANEL(user_data);
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(instance);

	GtkTreeSelection *tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->w_tree_view));
	GtkTreeModel *model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree_view);
	GtkTreeIter iter;
	gboolean has_selection = gtk_tree_selection_get_selected(tree_selection, &model, &iter);
	if (!has_selection) {
		return;
	}
	JagPrefsJreWo *jre = NULL;
	gtk_tree_model_get(model, &iter, 0, &jre, -1);

	cat_log_debug("priv->e_jag_prefs=%o", priv->e_jag_prefs);
	CatArrayWo *e_jre_list = jag_preferences_wo_get_editable_jre_list(priv->e_jag_prefs);
	int jre_index = cat_array_wo_find_index(e_jre_list, jre, -1);
	cat_log_debug("jre_index=%d, jre=%o", jre_index, jre);
	cat_log_debug("e_jre_list=%o", e_jre_list);
	if (jre_index<0) {
		return;
	}

	JagPrefsJreWo *e_prefs_jre_wo = jag_prefs_jre_wo_clone(jre, CAT_CLONE_DEPTH_FULL);
	cat_log_debug("e_prefs_jre_wo=%o", e_prefs_jre_wo);
	JagPrefsJreDialog *jre_dialog = jag_prefs_jre_dialog_new(priv->vip_service, e_prefs_jre_wo);
	gboolean keep_open = TRUE;
	while(keep_open) {
		int result = jag_prefs_jre_dialog_run(jre_dialog);
		cat_log_debug("result=%d", result);
		if (result == GTK_RESPONSE_OK) {
			CatStringWo *new_jre_name = jag_prefs_jre_wo_get_name(e_prefs_jre_wo);

			gboolean name_exists = FALSE;
			CatIIterator *array_iter = cat_array_wo_iterator(e_jre_list);
			while(cat_iiterator_has_next(array_iter)) {
				JagPrefsJreWo *prefs_jre = (JagPrefsJreWo *) cat_iiterator_next(array_iter);
				if (prefs_jre == jre) {
					continue;
				}

				CatStringWo *ex_jre_name = jag_prefs_jre_wo_get_name(prefs_jre);
				if (cat_string_wo_equal(new_jre_name, ex_jre_name)) {
					name_exists = TRUE;
					break;
				}
			}
			cat_unref_ptr(array_iter);

			if (name_exists) {
				GtkWidget *msg_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Name already exists. Cannot add JRE settings.");
				gtk_dialog_run (GTK_DIALOG (msg_dialog));
				gtk_widget_destroy(msg_dialog);
			} else {
				cat_array_wo_set(e_jre_list, (GObject *) e_prefs_jre_wo, jre_index, NULL);
				cat_log_debug("set at %d: e_jre_list=%o", jre_index, e_jre_list);
				keep_open = FALSE;
			}
		} else {
			keep_open = FALSE;
		}
	}

	jag_prefs_jre_dialog_close(jre_dialog);
	cat_unref_ptr(e_prefs_jre_wo);
	cat_unref_ptr(jre_dialog);

	l_notify_modification(instance);
}

static void l_refresh_config(JagPrefsCompilerPanel *panel) {
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(panel);

	GtkListStore *list = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(priv->w_tree_view)));
	gtk_list_store_clear(list);
	cat_log_debug("priv->e_jag_prefs=%o", priv->e_jag_prefs);
	if (priv->e_jag_prefs) {
		CatArrayWo *jre_list = jag_preferences_wo_get_jre_list(priv->e_jag_prefs);
		cat_log_debug("jre_list=%o", jre_list);
		CatIIterator *array_iter = cat_array_wo_iterator(jre_list);
		GtkTreeIter iter;
		while(cat_iiterator_has_next(array_iter)) {
			JagPrefsJreWo *prefs_jre = (JagPrefsJreWo *) cat_iiterator_next(array_iter);
			cat_log_debug("prefs_jre=%o", prefs_jre);
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, prefs_jre, -1);
		}
		cat_unref_ptr(array_iter);
	}
}

static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	JagPrefsCompilerPanel *instance = JAG_PREFS_COMPILER_PANEL(self);
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(instance);


	cat_log_debug("config=%o", config);
	CatWo *e_prefs = CAT_WO(config);
	if (e_prefs!=NULL) {
		e_prefs = cat_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);
	}
	cat_unref_ptr(priv->e_prefs);
	cat_unref_ptr(priv->e_jag_prefs);
	priv->e_prefs = e_prefs;

	if (e_prefs!=NULL) {

		cat_log_debug("e_prefs=%p", e_prefs);
		JagPreferencesWo *jag_prefs = (JagPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, e_prefs);
		JagPreferencesWo *e_jag_prefs = NULL;
		if (jag_prefs == NULL) {
			e_jag_prefs = jag_preferences_wo_new();
		} else {
			e_jag_prefs = jag_preferences_wo_clone(jag_prefs, CAT_CLONE_DEPTH_MAIN);
		}
		cow_ientry_accessor_set(priv->entry_accessor, e_prefs, e_jag_prefs);
		cat_log_debug("e_jag_prefs=%p", e_jag_prefs);
		priv->e_jag_prefs = e_jag_prefs;
	}

	l_refresh_config(instance);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPrefsCompilerPanel *instance = JAG_PREFS_COMPILER_PANEL(self);
	JagPrefsCompilerPanelPrivate *priv = jag_prefs_compiler_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p e_jag_prefs=%p]", iname, self, priv->e_jag_prefs);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
