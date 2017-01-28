/*
   File:    draprefstemplatespanel.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 25, 2014
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

#include "draprefstemplatespanel.h"
#include "../drapreferenceswo.h"

#include "dragladeprefstemplates.h"

#include <logging/catlogdefs.h>

#include "draprefstemplatedialog.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPrefsTemplatesPanel"
#include <logging/catlog.h>

struct _DraPrefsTemplatesPanelPrivate {
	CowIEntryAccessor *entry_accessor;
	CatWo *e_prefs;
	ChaPreferencesWo *e_dra_prefs;

	GtkWidget *w_tree_view;
	GtkWidget *w_but_add;
	GtkWidget *w_but_edit;
	GtkWidget *w_but_remove;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsTemplatesPanel, dra_prefs_templates_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(DraPrefsTemplatesPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void dra_prefs_templates_panel_class_init(DraPrefsTemplatesPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_templates_panel_init(DraPrefsTemplatesPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPrefsTemplatesPanel *instance = DRA_PREFS_TEMPLATES_PANEL(object);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(instance);
	cat_unref_ptr(priv->e_dra_prefs);
	G_OBJECT_CLASS(dra_prefs_templates_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_templates_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_list_name_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector);
static void l_list_path_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector);
static void l_on_add_template(GtkButton *button, gpointer user_data);
static void l_on_edit_template(GtkButton *button, gpointer user_data);
static void l_on_remove_template(GtkButton *button, gpointer user_data);
static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data);

DraPrefsTemplatesPanel *dra_prefs_templates_panel_new(CowIEntryAccessor *entry_accessor) {
	DraPrefsTemplatesPanel *result = g_object_new(DRA_TYPE_PREFS_TEMPLATES_PANEL, NULL);
	cat_ref_anounce(result);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(result);
	priv->entry_accessor = cat_ref_ptr(entry_accessor);
	priv->e_prefs = NULL;
	priv->e_dra_prefs = NULL;
	cow_panel_construct((CowPanel *) result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) dragladeprefstemplates_glade, (gsize) dragladeprefstemplates_glade_len, (GError **) (&gerror));
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
	g_signal_connect(priv->w_but_add, "clicked", G_CALLBACK(l_on_add_template), result);

	priv->w_but_edit = (GtkWidget *) gtk_builder_get_object(builder, "but_edit");
	g_signal_connect(priv->w_but_edit, "clicked", G_CALLBACK(l_on_edit_template), result);

	priv->w_but_remove = (GtkWidget *) gtk_builder_get_object(builder, "but_remove");
	g_signal_connect(priv->w_but_remove, "clicked", G_CALLBACK(l_on_remove_template), result);


	return result;
}


static void l_list_name_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	ChaPrefsTemplateWo *template = NULL;
	gtk_tree_model_get(model, iter, 0, &template, -1);
	CatStringWo *j_name = cha_prefs_template_wo_get_name(template);
	if (j_name) {
		g_object_set(renderer, "text", cat_string_wo_getchars(j_name), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	} else {
		g_object_set(renderer, "text", "<null>", "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	}
}

static void l_list_path_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	ChaPrefsTemplateWo *template = NULL;
	gtk_tree_model_get(model, iter, 0, &template, -1);
	CatStringWo *j_descr = cha_prefs_template_wo_get_description(template);
	if (j_descr) {
		g_object_set(renderer, "text", cat_string_wo_getchars(j_descr), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	} else {
		g_object_set(renderer, "text", "<null>", "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	}
}


static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data) {
	DraPrefsTemplatesPanel *instance = DRA_PREFS_TEMPLATES_PANEL(user_data);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(instance);

	GtkTreeModel *model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree_view);
	GtkTreeIter iter;
	gboolean has_selection = gtk_tree_selection_get_selected(tree_selection, &model, &iter);
	gtk_widget_set_sensitive(priv->w_but_edit, has_selection);
	gtk_widget_set_sensitive(priv->w_but_remove, has_selection);
}


static gboolean l_notify_modification(DraPrefsTemplatesPanel *panel) {
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(panel);
	cow_panel_set_configuration((CowPanel *) panel, (GObject *) priv->e_prefs);
	return TRUE;
}


static void l_on_add_template(GtkButton *button, gpointer user_data) {
	DraPrefsTemplatesPanel *instance = DRA_PREFS_TEMPLATES_PANEL(user_data);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(instance);
	ChaPrefsTemplateWo *e_template = cha_prefs_template_wo_new();
	DraPrefsTemplateDialog *template_dialog = dra_prefs_template_dialog_new(e_template);
	gboolean keep_open = TRUE;
	while(keep_open) {
		int result = dra_prefs_template_dialog_run(template_dialog);
		cat_log_debug("result=%d", result);
		if (result == GTK_RESPONSE_OK) {
			CatStringWo *new_template_name = cha_prefs_template_wo_get_name(e_template);
			cat_log_debug("new_template_name=%o", new_template_name);

			gboolean name_exists = FALSE;

			CatHashMapWo *e_template_map = cha_preferences_wo_get_editable_template_map(priv->e_dra_prefs);
			GObject *existing_template = cat_hash_map_wo_get(e_template_map, new_template_name);
			if (existing_template) {
				name_exists = TRUE;
			}

			if (name_exists) {
				GtkWidget *msg_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Name already exists. Cannot add template.");
				gtk_dialog_run (GTK_DIALOG (msg_dialog));
				gtk_widget_destroy(msg_dialog);
			} else {
				cat_hash_map_wo_put(e_template_map, (GObject *) new_template_name, (GObject *) e_template);
				keep_open = FALSE;
			}
		} else {
			keep_open = FALSE;
		}
	}

	dra_prefs_template_dialog_close(template_dialog);
	cat_unref_ptr(e_template);
	cat_unref_ptr(template_dialog);

	l_notify_modification(instance);
}


static void l_on_edit_template(GtkButton *button, gpointer user_data) {
	DraPrefsTemplatesPanel *instance = DRA_PREFS_TEMPLATES_PANEL(user_data);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(instance);

	GtkTreeSelection *tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->w_tree_view));
	GtkTreeModel *model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree_view);
	GtkTreeIter iter;
	gboolean has_selection = gtk_tree_selection_get_selected(tree_selection, &model, &iter);
	if (!has_selection) {
		return;
	}
	ChaPrefsTemplateWo *template = NULL;
	gtk_tree_model_get(model, &iter, 0, &template, -1);

	cat_log_debug("priv->e_ast_prefs=%o", priv->e_dra_prefs);
	CatHashMapWo *e_template_map = cha_preferences_wo_get_editable_template_map(priv->e_dra_prefs);
	template = (ChaPrefsTemplateWo *) cat_hash_map_wo_get(e_template_map, cha_prefs_template_wo_get_name(template));

	if (template==NULL) {
		return;
	}

	ChaPrefsTemplateWo *e_template = cha_prefs_template_wo_clone(template, CAT_CLONE_DEPTH_FULL);
	cat_log_debug("e_template=%o", e_template);
	DraPrefsTemplateDialog *template_dialog = dra_prefs_template_dialog_new(e_template);
	gboolean keep_open = TRUE;
	while(keep_open) {
		int result = dra_prefs_template_dialog_run(template_dialog);
		cat_log_debug("result=%d", result);
		if (result == GTK_RESPONSE_OK) {
			CatStringWo *new_template_name = cha_prefs_template_wo_get_name(e_template);
			cat_log_debug("new_template_name=%o", new_template_name);
			if (!cat_string_wo_equal(new_template_name, cha_prefs_template_wo_get_name(template))) {
				cat_log_debug("old name differs");
				if (cat_hash_map_wo_get(e_template_map, new_template_name)!=NULL) {
					cat_log_debug("new name in use");
					GtkWidget *msg_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Name already in use by another template. Cannot store modified template.");
					gtk_dialog_run (GTK_DIALOG (msg_dialog));
					gtk_widget_destroy(msg_dialog);
					continue;
				}
				cat_log_debug("removing old template");
				GObject *old = cat_hash_map_wo_remove(e_template_map, (GObject *) cha_prefs_template_wo_get_name(template));
				cat_log_debug("unreffing old:%o", old);
				cat_unref_ptr(old);
			}
			cat_log_debug("storing new:%o", new_template_name);
			cat_hash_map_wo_put(e_template_map, (GObject *) new_template_name, (GObject *) e_template);
			keep_open = FALSE;
		} else {
			keep_open = FALSE;
		}
	}

	cat_log_debug("closing dialog");

	dra_prefs_template_dialog_close(template_dialog);
	cat_unref_ptr(e_template);
	cat_unref_ptr(template_dialog);

	cat_log_debug("notifying new config");

	l_notify_modification(instance);
	cat_log_debug("done");
}

static void l_on_remove_template(GtkButton *button, gpointer user_data) {
	DraPrefsTemplatesPanel *instance = DRA_PREFS_TEMPLATES_PANEL(user_data);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(instance);


	GtkWidget *msg_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Are you sure you want to remove the selected template ?");
	int response = gtk_dialog_run (GTK_DIALOG (msg_dialog));
	gtk_widget_destroy(msg_dialog);

	if (response == GTK_RESPONSE_OK) {
		GtkTreeSelection *tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->w_tree_view));
		GtkTreeModel *model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree_view);
		GtkTreeIter iter;
		gboolean has_selection = gtk_tree_selection_get_selected(tree_selection, &model, &iter);
		if (!has_selection) {
			return;
		}
		ChaPrefsTemplateWo *template = NULL;
		gtk_tree_model_get(model, &iter, 0, &template, -1);
		CatHashMapWo *e_template_map = cha_preferences_wo_get_editable_template_map(priv->e_dra_prefs);
		GObject *old = cat_hash_map_wo_remove(e_template_map, (GObject *) cha_prefs_template_wo_get_name(template));
		cat_unref_ptr(old);

		l_notify_modification(instance);
	}
}



static void l_refresh_form(DraPrefsTemplatesPanel *panel) {
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(panel);

	GtkListStore *list = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(priv->w_tree_view)));
	gtk_list_store_clear(list);
	cat_log_debug("priv->e_jag_prefs=%o", priv->e_dra_prefs);
	if (priv->e_dra_prefs) {
		CatHashMapWo *template_map = cha_preferences_wo_get_template_map(priv->e_dra_prefs);

		CatArrayWo *e_keys = cat_hash_map_wo_enlist_keys(template_map, NULL);
		cat_array_wo_sort(e_keys, (GCompareFunc) cat_string_wo_compare);
		CatIIterator *key_iter = cat_array_wo_iterator(e_keys);
		GtkTreeIter iter;
		while(cat_iiterator_has_next(key_iter)) {
			CatStringWo *name = (CatStringWo *) cat_iiterator_next(key_iter);
			ChaPrefsTemplateWo *template = (ChaPrefsTemplateWo *) cat_hash_map_wo_get(template_map, name);
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, template, -1);
		}

		cat_unref_ptr(key_iter);
	}
}



static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	DraPrefsTemplatesPanel *instance = DRA_PREFS_TEMPLATES_PANEL(self);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(instance);

	cat_log_debug("config=%o", config);
	CatWo *e_prefs = CAT_WO(config);
	if (e_prefs!=NULL) {
		e_prefs = cat_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);
	}
	cat_unref_ptr(priv->e_prefs);
	cat_unref_ptr(priv->e_dra_prefs);
	priv->e_prefs = e_prefs;

	if (e_prefs!=NULL) {

		cat_log_debug("e_prefs=%p", e_prefs);
		DraPreferencesWo *ast_prefs = (DraPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, e_prefs);
		DraPreferencesWo *e_dra_prefs = NULL;
		if (ast_prefs == NULL) {
			e_dra_prefs = dra_preferences_wo_new();
		} else {
			e_dra_prefs = dra_preferences_wo_clone(ast_prefs, CAT_CLONE_DEPTH_MAIN);
		}
		cow_ientry_accessor_set(priv->entry_accessor, e_prefs, e_dra_prefs);
		cat_log_debug("e_ast_prefs=%p", e_dra_prefs);
		priv->e_dra_prefs = (ChaPreferencesWo *) e_dra_prefs;
	}

	l_refresh_form(instance);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPrefsTemplatesPanel *instance = DRA_PREFS_TEMPLATES_PANEL(self);
	DraPrefsTemplatesPanelPrivate *priv = dra_prefs_templates_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p e_ast_prefs=%p]", iname, self, priv->e_dra_prefs);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

