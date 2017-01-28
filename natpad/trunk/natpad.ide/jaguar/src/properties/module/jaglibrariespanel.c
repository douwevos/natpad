/*
   File:    jaglibrariespanel.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 5, 2014
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

#include "jaglibrariespanel.h"
#include "../../moose/module/jagmodulecontentwo.h"
#include "../../moose/module/jagmodulesettingswo.h"
#include <moose.h>

#include "jaggladelibraries.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagLibrariesPanel"
#include <logging/catlog.h>

struct _JagLibrariesPanelPrivate {
	MooNodeWo *e_moo_node;
	JagModuleSettingsWo *jag_module_settings;

	GtkButton *w_but_add_file;
	GtkButton *w_but_add_folder;
	GtkButton *w_but_remove;
	GtkButton *w_but_edit;
	GtkTreeView *w_trv_jars;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagLibrariesPanel, jag_libraries_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(JagLibrariesPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);
static void l_clicked_add_file(GtkButton *button, gpointer user_data);
static void l_clicked_remove(GtkButton *button, gpointer user_data);
static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data);


static void jag_libraries_panel_class_init(JagLibrariesPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_libraries_panel_init(JagLibrariesPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagLibrariesPanel *instance = JAG_LIBRARIES_PANEL(object);
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(instance);
	cat_unref_ptr(priv->e_moo_node);
	cat_unref_ptr(priv->jag_module_settings);

	G_OBJECT_CLASS(jag_libraries_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_libraries_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_text_cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	void *data;
	gtk_tree_model_get(model, iter, 0, &data, -1);
	CatStringWo *pattern = (CatStringWo *) data;
	g_object_set(renderer, "text", cat_string_wo_getchars(pattern), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
}


JagLibrariesPanel *jag_libraries_panel_new() {
	JagLibrariesPanel *result = g_object_new(JAG_TYPE_LIBRARIES_PANEL, NULL);
	cat_ref_anounce(result);
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(result);
	cow_panel_construct((CowPanel *) result);
	priv->e_moo_node = NULL;
	priv->jag_module_settings = NULL;

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) jaggladelibraries_glade, (gsize) jaggladelibraries_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_but_add_file = (GtkButton *) gtk_builder_get_object(builder, "but_add_file");
	g_signal_connect(priv->w_but_add_file, "clicked", G_CALLBACK(l_clicked_add_file), result);

	priv->w_but_add_folder = (GtkButton *) gtk_builder_get_object(builder, "but_add_folder");

	priv->w_but_remove = (GtkButton *) gtk_builder_get_object(builder, "but_remove");
	gtk_widget_set_sensitive((GtkWidget *) priv->w_but_remove, FALSE);
	g_signal_connect(priv->w_but_remove, "clicked", G_CALLBACK(l_clicked_remove), result);

	priv->w_but_edit = (GtkButton *) gtk_builder_get_object(builder, "but_edit");
	gtk_widget_set_sensitive((GtkWidget *) priv->w_but_edit, FALSE);

	priv->w_trv_jars = (GtkTreeView *) gtk_builder_get_object(builder, "treeview_libraries");
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_renderer_set_padding(renderer, 0, 0);
	gtk_tree_view_insert_column_with_data_func(priv->w_trv_jars, 0, "Folder-name", renderer, (GtkTreeCellDataFunc) l_text_cell_data_func, result, NULL);
	GtkTreeSelection *tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->w_trv_jars));
	gtk_tree_selection_set_mode(tree_selection, GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(tree_selection), "changed", G_CALLBACK(l_selection_changed), result);

	return result;
}


static void l_selection_changed(GtkTreeSelection *tree_selection, gpointer user_data) {
	JagLibrariesPanel *panel = JAG_LIBRARIES_PANEL(user_data);
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(panel);

	GtkTreeModel *model = gtk_tree_view_get_model((GtkTreeView *) priv->w_trv_jars);
	GtkTreeIter iter;
	if (gtk_tree_selection_get_selected(tree_selection, &model, &iter)) {
		gtk_widget_set_sensitive((GtkWidget *) priv->w_but_remove, TRUE);
		gtk_widget_set_sensitive((GtkWidget *) priv->w_but_edit, TRUE);
	} else {
		gtk_widget_set_sensitive((GtkWidget *) priv->w_but_remove, FALSE);
		gtk_widget_set_sensitive((GtkWidget *) priv->w_but_edit, FALSE);
	}
}


static void l_post_new_jag_module_settings(JagLibrariesPanel *panel, JagModuleSettingsWo *new_settings) {
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(panel);
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



static gboolean l_find_existing_library(JagModuleSettingsWo *settings, CatStringWo *library_2_search) {
	gboolean found = FALSE;
	CatArrayWo *a_libraries = jag_module_settings_wo_get_libraries(settings);
	if (a_libraries) {
		CatIIterator *iter = cat_array_wo_iterator(a_libraries);
		while(!found && cat_iiterator_has_next(iter)) {
			CatStringWo *in_list_library = (CatStringWo *) cat_iiterator_next(iter);
			if (cat_string_wo_equal(in_list_library, library_2_search)) {
				found = TRUE;
				break;
			}
		}
		cat_unref_ptr(iter);
	}
	return found;
}


static void l_clicked_remove(GtkButton *button, gpointer user_data) {
	JagLibrariesPanel *panel = JAG_LIBRARIES_PANEL(user_data);
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(panel);
	if (priv->jag_module_settings==NULL) {
		return;
	}
	GtkTreeSelection *tree_selection = gtk_tree_view_get_selection((GtkTreeView *) priv->w_trv_jars);
	GtkTreeModel *model;
	GtkTreeIter iter;

	if (gtk_tree_selection_get_selected(tree_selection, &model, &iter)) {
		CatStringWo *name = NULL;
		gtk_tree_model_get(model, &iter, 0, &name, -1);
		if (name) {
			CatArrayWo *a_libraries_list = jag_module_settings_wo_get_libraries(priv->jag_module_settings);
			if (a_libraries_list==NULL) {
				return;
			}

			int idx;
			for(idx=cat_array_wo_size(a_libraries_list)-1; idx>=0; idx--) {
				CatStringWo *ex_library = (CatStringWo *) cat_array_wo_get(a_libraries_list, idx);
				if (cat_string_wo_equal(ex_library, name)) {
					CatArrayWo *e_libraries_list = cat_array_wo_clone(a_libraries_list, CAT_CLONE_DEPTH_MAIN);
					cat_array_wo_remove(e_libraries_list, idx, NULL);

					JagModuleSettingsWo *e_jag_module_settings = priv->jag_module_settings;
					if (jag_module_settings_wo_is_fixed(e_jag_module_settings)) {
						e_jag_module_settings = jag_module_settings_wo_ensure_writable(e_jag_module_settings);
					}
					a_libraries_list = cat_array_wo_anchor(e_libraries_list, 0);
					jag_module_settings_wo_set_libraries(e_jag_module_settings, a_libraries_list);
					cat_unref_ptr(a_libraries_list);
					l_post_new_jag_module_settings(panel, e_jag_module_settings);
					break;
				}
			}
		}
	}
}


static void l_clicked_add_file(GtkButton *button, gpointer user_data) {
	JagLibrariesPanel *panel = JAG_LIBRARIES_PANEL(user_data);
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(panel);
	if (priv->jag_module_settings==NULL) {
		return;
	}

	GtkWindow *top_window = (GtkWindow *) gtk_widget_get_toplevel(GTK_WIDGET(panel));
	GtkWidget *dialog = gtk_file_chooser_dialog_new((const gchar *) "Select path of JAR/ZIP file", top_window,
					(GtkFileChooserAction) GTK_FILE_CHOOSER_ACTION_OPEN,
					"_Cancel", GTK_RESPONSE_CANCEL,
//					GTK_STOCK_OK, GTK_RESPONSE_OK,
					NULL);

	GtkFileFilter *myfilter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(myfilter, "*.jar");
	gtk_file_filter_add_pattern(myfilter, "*.zip");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), myfilter);

//	GtkWidget *w_but_ok =
			gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", GTK_RESPONSE_OK);
//	gtk_widget_set_sensitive(w_but_ok, FALSE);
//	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "/home/superman/work/");
	gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog), FALSE);

//	g_signal_connect(GTK_FILE_CHOOSER(dialog), "selection-changed", G_CALLBACK(_on_choose_selection_changed), user_data);
	gtk_widget_show_all(dialog);

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	if (response==GTK_RESPONSE_OK) {
		gchar *fname = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));
		if (fname!=NULL) {
			CatStringWo *library_2_add = cat_string_wo_new_with(fname);

			if (!l_find_existing_library(priv->jag_module_settings, library_2_add)) {
				CatArrayWo *a_library_list = jag_module_settings_wo_get_libraries(priv->jag_module_settings);
				CatArrayWo *e_library_list = NULL;
				if (a_library_list == NULL) {
					e_library_list = cat_array_wo_new();
				} else {
					e_library_list = cat_array_wo_clone(a_library_list, CAT_CLONE_DEPTH_MAIN);
				}
				cat_array_wo_append(e_library_list, (GObject *) library_2_add);
				a_library_list = cat_array_wo_anchor(e_library_list, 0);
				JagModuleSettingsWo *e_jag_module_settings = priv->jag_module_settings;
				if (jag_module_settings_wo_is_fixed(e_jag_module_settings)) {
					e_jag_module_settings = jag_module_settings_wo_ensure_writable(e_jag_module_settings);
				}
				jag_module_settings_wo_set_libraries(e_jag_module_settings, a_library_list);
				cat_unref_ptr(a_library_list);

				l_post_new_jag_module_settings(panel, e_jag_module_settings);
			}
			cat_unref_ptr(library_2_add);
		}
	}
	gtk_widget_destroy(dialog);
}



static void l_refresh_libraries_list(JagLibrariesPanel *libraries_panel) {
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(libraries_panel);
	if (priv->jag_module_settings) {
		CatArrayWo *a_libraries = jag_module_settings_wo_get_libraries(priv->jag_module_settings);
		CatIIterator *iterator = a_libraries==NULL ? NULL : cat_array_wo_iterator(a_libraries);

		cat_log_debug("libraries=%o, iterator=%o", a_libraries, iterator);
//
		void *data;
		GtkTreeIter iter, sibling;
		GtkTreeModel *tree_model = gtk_tree_view_get_model(priv->w_trv_jars);
		GtkListStore *list_store = GTK_LIST_STORE(tree_model);
		gboolean valid_iter = gtk_tree_model_get_iter_first(tree_model, &iter);
		while(iterator && cat_iiterator_has_next(iterator)) {
			CatStringWo *library = (CatStringWo *) cat_iiterator_next(iterator);
			cat_log_debug("library=%o", library);


			gboolean inserted = FALSE;
			while(!inserted) {
				if (valid_iter) {
					gtk_tree_model_get(tree_model, &iter, 0, &data, -1);
					CatStringWo *lm_library = (CatStringWo *) data;
					cat_log_debug("lm_library=%o", lm_library);
					if (cat_string_wo_equal(lm_library, library)) {
						valid_iter = gtk_tree_model_iter_next(tree_model, &iter);
						inserted = TRUE;
						cat_log_debug("in-list-already");
					} else {
						int idx = cat_array_wo_find_index_ext(a_libraries, lm_library, -1, (GCompareFunc) cat_string_wo_compare);
						cat_log_debug("inserting:%d", idx);
						if (idx>=0) {
							gtk_list_store_insert_before(list_store, &sibling, &iter);
							gtk_list_store_set(list_store, &sibling, 0, library, -1);
							inserted = TRUE;
						} else {
							valid_iter = gtk_list_store_remove(list_store, &iter);
						}
					}
				} else {
					cat_log_debug("appending");
					gtk_list_store_append(list_store, &sibling);
					gtk_list_store_set(list_store, &sibling, 0, library, -1);
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
	JagLibrariesPanel *instance = JAG_LIBRARIES_PANEL(self);
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(instance);
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
	l_refresh_libraries_list(instance);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagLibrariesPanel *instance = JAG_LIBRARIES_PANEL(self);
	JagLibrariesPanelPrivate *priv = jag_libraries_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p] jag_module_settings=%p", iname, self, priv->jag_module_settings);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







