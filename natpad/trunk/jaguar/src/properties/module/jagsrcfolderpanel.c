/*
   File:    jagsrcfolderpanel.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 31, 2014
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

#include "jagsrcfolderpanel.h"
#include "../../moose/module/jagmodulecontentwo.h"
#include "../../moose/module/jagmodulesettingswo.h"
#include <moose.h>

#include "jaggladesourcefolders.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagSrcFolderPanel"
#include <logging/catlog.h>

struct _JagSrcFolderPanelPrivate {
	MooNodeWo *e_moo_node;
	JagModuleSettingsWo *jag_module_settings;

	GtkWidget *w_but_add;
	GtkWidget *w_but_edit;
	GtkWidget *w_but_remove;

	GtkWidget *w_source_folder_tree;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagSrcFolderPanel, jag_src_folder_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(JagSrcFolderPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void jag_src_folder_panel_class_init(JagSrcFolderPanelClass *clazz) {

	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_src_folder_panel_init(JagSrcFolderPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagSrcFolderPanel *instance = JAG_SRC_FOLDER_PANEL(object);
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(instance);
	cat_unref_ptr(priv->e_moo_node);
	cat_unref_ptr(priv->jag_module_settings);
	G_OBJECT_CLASS(jag_src_folder_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_src_folder_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_text_cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	void *data;
	gtk_tree_model_get(model, iter, 0, &data, -1);
	CatStringWo *a_pattern = (CatStringWo *) data;
	g_object_set(renderer, "text", cat_string_wo_getchars(a_pattern), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
}

static void l_clicked_add(GtkButton *button, gpointer user_data);
static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data);


JagSrcFolderPanel *jag_src_folder_panel_new() {
	JagSrcFolderPanel *result = g_object_new(JAG_TYPE_SRC_FOLDER_PANEL, NULL);
	cat_ref_anounce(result);
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(result);
	cow_panel_construct((CowPanel *) result);
	priv->e_moo_node = NULL;
	priv->jag_module_settings = NULL;

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) jaggladesourcefolders_glade, (gsize) jaggladesourcefolders_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_but_add = (GtkWidget *) gtk_builder_get_object(builder, "but_add");
	g_signal_connect(priv->w_but_add, "clicked", G_CALLBACK(l_clicked_add), result);

	priv->w_but_edit = (GtkWidget *) gtk_builder_get_object(builder, "but_edit");
	priv->w_but_remove = (GtkWidget *) gtk_builder_get_object(builder, "but_remove");
	priv->w_source_folder_tree = (GtkWidget *) gtk_builder_get_object(builder, "treeview_src_folders");

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_renderer_set_padding(renderer, 0, 0);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(priv->w_source_folder_tree), 0, "Folder-name", renderer, (GtkTreeCellDataFunc) l_text_cell_data_func, result, NULL);


	GtkTreeSelection *tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->w_source_folder_tree));
	gtk_tree_selection_set_mode(tree_selection, GTK_SELECTION_SINGLE);

	g_signal_connect(G_OBJECT(tree_selection), "changed", G_CALLBACK(l_selection_changed), result);

	return result;
}


static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data) {
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(JAG_SRC_FOLDER_PANEL(user_data));

	GtkTreeModel *model = gtk_tree_view_get_model((GtkTreeView *) priv->w_source_folder_tree);
	GtkTreeIter iter;
	if (gtk_tree_selection_get_selected(tree_selection, &model, &iter)) {
		gtk_widget_set_sensitive(priv->w_but_remove, TRUE);
	} else {
		gtk_widget_set_sensitive(priv->w_but_remove, FALSE);
	}
}


static void l_post_new_jag_module_settings(JagSrcFolderPanel *panel, JagModuleSettingsWo *new_settings) {
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(panel);
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


static gboolean l_find_existing_source_folder(JagModuleSettingsWo *settings, CatArrayWo *e_source_fldr_entries) {
	gboolean found = FALSE;
	CatArrayWo *a_src_fldr_list = jag_module_settings_wo_get_source_folders(settings);
	if (a_src_fldr_list) {
		CatIIterator *iter = cat_array_wo_iterator(a_src_fldr_list);
		while(!found && cat_iiterator_has_next(iter)) {
			CatArrayWo *e_old_entries = (CatArrayWo *) cat_iiterator_next(iter);
			if (cat_array_wo_size(e_old_entries)==cat_array_wo_size(e_source_fldr_entries)) {
				int idx;
				found = TRUE;
				for(idx=cat_array_wo_size(e_old_entries)-1; idx>=0 ; idx--) {
					CatStringWo *a_old_entry = (CatStringWo *) cat_array_wo_get(e_old_entries, idx);
					CatStringWo *a_new_entry = (CatStringWo *) cat_array_wo_get(e_source_fldr_entries, idx);
					if (!cat_string_wo_equal(a_old_entry, a_new_entry)) {
						found = FALSE;
						break;
					}
				}
			}
		}
		cat_unref_ptr(iter);
	}
	return found;
}

static void l_clicked_add(GtkButton *button, gpointer user_data) {
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(JAG_SRC_FOLDER_PANEL(user_data));
	if (priv->jag_module_settings==NULL) {
		return;
	}

	LeaBasicInputDialog *dialog = lea_basic_input_dialog_new(GTK_WIDGET(user_data), "Folder _name", "Please type the name of a folder inside the project");
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_OK) {
		const gchar *txt_new_class_name = lea_basic_input_dialog_get_entry_text(dialog);
		if (txt_new_class_name) {
			cat_log_debug("txt_new_class_name=%s", txt_new_class_name);
			const CatStringWo *a_new_source_folder_name = cat_string_wo_new_data(txt_new_class_name);
			CatArrayWo *e_split = cat_string_wo_split(a_new_source_folder_name, '/', FALSE);
			cat_unref_ptr(a_new_source_folder_name);


			if (!l_find_existing_source_folder(priv->jag_module_settings, e_split)) {
				CatArrayWo *a_src_fldr_list = jag_module_settings_wo_get_source_folders(priv->jag_module_settings);
				CatArrayWo *e_src_fldr_list = NULL;
				if (a_src_fldr_list==NULL) {
					e_src_fldr_list = cat_array_wo_new();
				} else  {
					e_src_fldr_list = cat_array_wo_clone(a_src_fldr_list, CAT_CLONE_DEPTH_MAIN);
				}
				cat_array_wo_append(e_src_fldr_list, (GObject *) e_split);
				a_src_fldr_list = cat_array_wo_anchor(e_src_fldr_list, 0);
				jag_module_settings_wo_set_source_folders(priv->jag_module_settings, a_src_fldr_list);
				cat_unref_ptr(a_src_fldr_list);

				l_post_new_jag_module_settings(JAG_SRC_FOLDER_PANEL(user_data), priv->jag_module_settings);
			}
			cat_unref_ptr(e_split);

		} else {
			cat_log_warn("txt_new_class_name is NULL");
		}
	}
	gtk_widget_destroy(GTK_WIDGET(dialog));

}

static void l_refresh_source_folder_list(JagSrcFolderPanel *panel) {
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(panel);
	if (priv->jag_module_settings) {
		CatArrayWo *a_source_folders = jag_module_settings_wo_get_source_folders(priv->jag_module_settings);
		CatIIterator *iterator = a_source_folders==NULL ? NULL : cat_array_wo_iterator(a_source_folders);

		cat_log_debug("source_folders=%o, iterator=%o", a_source_folders, iterator);

//
		void *data;
		GtkTreeIter iter, sibling;
		GtkTreeModel *tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(priv->w_source_folder_tree));
		GtkListStore *list_store = GTK_LIST_STORE(tree_model);
		gboolean valid_iter = gtk_tree_model_get_iter_first(tree_model, &iter);
		while(iterator && cat_iiterator_has_next(iterator)) {
			CatArrayWo *e_source_folder = (CatArrayWo *) cat_iiterator_next(iterator);

			CatStringWo *e_nodename = cat_string_wo_new();
			int eidx = 0;
			CatIIterator *sfiter = cat_array_wo_iterator(e_source_folder);
			while(cat_iiterator_has_next(sfiter)) {
				CatStringWo *a_e = (CatStringWo *) cat_iiterator_next(sfiter);
				if (eidx>0) {
					cat_string_wo_append_char(e_nodename, '/');
				}
				cat_string_wo_append(e_nodename, a_e);
				eidx++;
			}
			cat_unref_ptr(sfiter);

			CatStringWo *a_pattern = cat_string_wo_anchor(e_nodename, 0);
			cat_log_debug("pattern=%o", a_pattern);


			gboolean inserted = FALSE;
			while(!inserted) {
				if (valid_iter) {
					gtk_tree_model_get(tree_model, &iter, 0, &data, -1);
					CatStringWo *a_lm_src_folder = (CatStringWo *) data;
					if (cat_string_wo_equal(a_lm_src_folder, a_pattern)) {
						valid_iter = gtk_tree_model_iter_next(tree_model, &iter);
						inserted = TRUE;
					} else {
						int idx = cat_array_wo_find_index_ext(a_source_folders, a_lm_src_folder, -1, (GCompareFunc) cat_string_wo_compare);
						if (idx>=0) {
							gtk_list_store_insert_before(list_store, &sibling, &iter);
							gtk_list_store_set(list_store, &sibling, 0, a_pattern, -1);
							inserted = TRUE;
						} else {
							valid_iter = gtk_list_store_remove(list_store, &iter);
						}
					}
				} else {
					gtk_list_store_append(list_store, &sibling);
					gtk_list_store_set(list_store, &sibling, 0, a_pattern, -1);
					inserted = TRUE;
				}
			}
		}

		while(valid_iter) {
			valid_iter = gtk_list_store_remove(list_store, (GtkTreeIter *) (&iter));
		}
		cat_unref_ptr(iterator);
		cat_log_detail("end of refresh pattern list");
	}
}



static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	JagSrcFolderPanel *instance = JAG_SRC_FOLDER_PANEL(self);
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(instance);
	MooNodeWo *e_moo_node = MOO_NODE_WO(config);
	cat_ref_swap(priv->e_moo_node, e_moo_node);
	if (e_moo_node) {

		JagModuleContentWo *module_content = (JagModuleContentWo *) moo_node_wo_get_content(e_moo_node, jag_module_content_wo_key());
		JagModuleSettingsWo *module_settings = module_content==NULL ? NULL : jag_module_content_wo_get_module_settings(module_content);

		cat_log_debug("node=%o, module_content=%o, module_settings=%o", e_moo_node, module_content, module_settings);

		if (jag_module_settings_wo_is_fixed(module_settings)) {
			JagModuleSettingsWo *e_module_settings = jag_module_settings_wo_ensure_writable(module_settings);
			module_settings = e_module_settings;
			cat_ref_swap(priv->jag_module_settings, module_settings);
			cat_unref_ptr(module_settings);
		} else {
			cat_ref_swap(priv->jag_module_settings, module_settings);
		}

	} else {
		cat_unref_ptr(priv->jag_module_settings);
	}
	l_refresh_source_folder_list(instance);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagSrcFolderPanel *instance = JAG_SRC_FOLDER_PANEL(self);
	JagSrcFolderPanelPrivate *priv = jag_src_folder_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p] jag_module_settings=%p", iname, self, priv->jag_module_settings);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

