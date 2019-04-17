/*
   File:    cowdialog.c
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 27, 2014
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

#include "cowdialog.h"
#include "cowtreeentry.h"
#include "cowpanelmodelprivate.h"
#include "../cowichangelistener.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CowDialog"
#include <logging/catlog.h>

#include "cowgladedialog.h"

struct _CowDialogPrivate {
	CowPanelModel *panel_model;
	CowContainer *container;
	GObject *marked_configuration;
	CowContainer *edit_container;

	CatArrayWo *panel_list;

	GtkWidget *w_but_cancel;
	GtkWidget *w_but_apply;
	GtkWidget *w_but_ok;
	GtkContainer *w_panel_container;
	GtkWidget *w_tree;

	GtkWidget *w_lab_entry;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_change_listener_iface_init(CowIChangeListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CowDialog, cow_dialog, GTK_TYPE_DIALOG,
		G_ADD_PRIVATE(CowDialog)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_ICHANGE_LISTENER, l_change_listener_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_remove_all_panels(CowDialog *dialog);
static void l_destroy(GtkWidget *widget);

static void cow_dialog_class_init(CowDialogClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;


	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->destroy = l_destroy;
}

static void cow_dialog_init(CowDialog *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CowDialog *instance = COW_DIALOG(object);
	CowDialogPrivate *priv = cow_dialog_get_instance_private(instance);
	l_remove_all_panels(instance);
	cat_unref_ptr(priv->panel_model);
	cat_unref_ptr(priv->container);
	cat_unref_ptr(priv->edit_container);
	cat_unref_ptr(priv->panel_list);
	cat_unref_ptr(priv->marked_configuration);
	G_OBJECT_CLASS(cow_dialog_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cow_dialog_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_text_cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, CowPanelDescription *con_dialog);
static void l_tree_selection_changed(GtkTreeSelection *tree_selection, gpointer config_dialog_as_pointer);
static void l_on_cancel_pressed(void *cause, gpointer user_data);

CowDialog *cow_dialog_new(CowPanelModel *panel_model, CowContainer *container, CowDialogType dialog_type) {
	CowDialog *result = g_object_new(COW_TYPE_DIALOG, NULL);
	cat_ref_anounce(result);
	CowDialogPrivate *priv = cow_dialog_get_instance_private(result);
	cat_log_debug("container = %O", container);
	priv->panel_model = cat_ref_ptr(panel_model);
	priv->container = cat_ref_ptr(container);
	priv->edit_container = NULL;
	priv->panel_list = NULL;
	priv->w_but_apply = NULL;
	priv->w_but_ok = NULL;
	priv->w_but_cancel = NULL;

	gtk_window_set_modal(GTK_WINDOW(result), TRUE);
	gtk_widget_set_size_request(GTK_WIDGET(result), 900, 500);

	GtkWidget *vbox = gtk_dialog_get_content_area((GtkDialog *) result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) cowgladedialog_glade, (gsize) cowgladedialog_glade_len, (GError **) (&gerror));

	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_paned");
	gtk_widget_unparent(frame);
	gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 0);

	if (dialog_type!=COW_DIALOG_OK_ONLY) {
		priv->w_but_cancel = gtk_button_new_with_mnemonic("_Cancel");
		gtk_dialog_add_action_widget((GtkDialog *) result, priv->w_but_cancel, GTK_RESPONSE_CANCEL);
		g_signal_connect(result, "close", G_CALLBACK(l_on_cancel_pressed), result);
	}


	if (dialog_type==COW_DIALOG_OK_CANCEL_AND_APPLY) {
		priv->w_but_apply = gtk_button_new_with_mnemonic("_Apply");
		gtk_dialog_add_action_widget((GtkDialog *) result, priv->w_but_apply, GTK_RESPONSE_APPLY);
	}

	priv->w_but_ok = gtk_button_new_with_mnemonic("_Ok");
	gtk_dialog_add_action_widget((GtkDialog *) result, priv->w_but_ok, GTK_RESPONSE_OK);


//	priv->w_but_add_file = (GtkButton *) gtk_builder_get_object(builder, "but_add_file");
//	g_signal_connect(priv->w_but_add_file, "clicked", G_CALLBACK(l_clicked_add_file), result);

	priv->w_panel_container = (GtkContainer *) gtk_builder_get_object(builder, "main_container");

	priv->w_lab_entry = (GtkWidget *) gtk_builder_get_object(builder, "entry_label");

	priv->w_tree = (GtkWidget *) gtk_builder_get_object(builder, "main_treeview");

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	int x,y;
	gtk_cell_renderer_get_padding(renderer, &x,&y);
	gtk_cell_renderer_set_padding(renderer, x, 0);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(priv->w_tree), 0, "Name",renderer, (GtkTreeCellDataFunc) l_text_cell_data_func, result, NULL);

	GtkTreeSelection *selection  = gtk_tree_view_get_selection((GtkTreeView *) priv->w_tree);
	g_signal_connect(selection, "changed", G_CALLBACK(l_tree_selection_changed), result);




	return result;
}

CowContainer *cow_dialog_get_edit_container(CowDialog *dialog) {
	CowDialogPrivate *priv = cow_dialog_get_instance_private(dialog);
	return priv->edit_container;
}


static void l_config_changed(CowIChangeListener *self, GObject *new_config);

static void l_attach_panel_factories(CowDialog *dialog) {
	CowDialogPrivate *priv = cow_dialog_get_instance_private(dialog);
	CatIIterator *iterator = cow_panel_model_factory_iterator(priv->panel_model);
	while(cat_iiterator_has_next(iterator)) {
		CowIPanelFactory *iclazz = (CowIPanelFactory *) cat_iiterator_next(iterator);
		COW_IPANEL_FACTORY_GET_INTERFACE(iclazz)->attach(iclazz, dialog, priv->edit_container);
	}
	cat_unref_ptr(iterator);
}

int cow_dialog_show(CowDialog *cow_dialog) {
	CowDialogPrivate *priv = cow_dialog_get_instance_private(cow_dialog);
	GObject *config = cow_container_get_configuration(priv->container);
	cat_log_debug("config=%o", config);
	cat_ref_swap(priv->marked_configuration, config);
	cat_unref_ptr(priv->edit_container);
	priv->edit_container = cow_container_create_editor(priv->container);
	cow_container_add_change_listener(priv->edit_container, (CowIChangeListener *) cow_dialog);
	l_config_changed((CowIChangeListener *) cow_dialog, cow_container_get_configuration(priv->edit_container));

	l_attach_panel_factories(cow_dialog);

	gtk_widget_show_all(GTK_WIDGET(cow_dialog));
	return gtk_dialog_run(GTK_DIALOG(cow_dialog));
}




GtkTreeIter cow_dialog_add_entry(CowDialog *dialog, GtkTreeIter *tree_iter, CowIPanelFactory *panel_factory, CowPanelDescription *panel_description) {
	CowDialogPrivate *priv = cow_dialog_get_instance_private(dialog);
	GtkTreeIter child_iter;

	CatStringWo *entry_name = cow_panel_description_get_entry_name(panel_description);

	GtkTreeModel *tree_model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree);

	gboolean child_found = FALSE;
	CowTreeEntry *cow_tree_entry = NULL;
	if (gtk_tree_model_iter_children(tree_model, &child_iter, tree_iter)) {
		while(TRUE) {
		    gtk_tree_model_get(tree_model, &child_iter, 0, &cow_tree_entry,  -1);
		    if (cat_string_wo_equal(cow_tree_entry_get_name(cow_tree_entry), entry_name)) {
		    	child_found = TRUE;
		    	break;
		    }
			if (!gtk_tree_model_iter_next(tree_model, &(child_iter))) {
				break;
			}
		}
	}

	if (!child_found) {
		gtk_tree_store_append((GtkTreeStore *) tree_model, &child_iter, tree_iter);
		cow_tree_entry = cow_tree_entry_new(entry_name);
		gtk_tree_store_set((GtkTreeStore *) tree_model, &child_iter, 0, cow_tree_entry, -1);
	}

	if (panel_factory) {
		cow_tree_entry_add_description(cow_tree_entry, panel_factory, panel_description);
	}
	return child_iter;
}


static int l_find_description_by_name(CatArrayWo *panel_descriptions, CatStringWo *name, int from_index) {
	int found_index = -1;
	int count = cat_array_wo_size(panel_descriptions);
	for(; from_index<count; from_index++) {
		CowPanelDescription *description = (CowPanelDescription *) cat_array_wo_get(panel_descriptions, from_index);
		CatStringWo *descr_name = cow_panel_description_get_entry_name(description);
		if (cat_string_wo_equal(descr_name, name)) {
			found_index = from_index;
			break;
		}
	}
	return found_index;
}

void cow_dialog_update_entries(CowDialog *dialog, GtkTreeIter *tree_iter, CowIPanelFactory *panel_factory, CatArrayWo *panel_descriptions) {
	CowDialogPrivate *priv = cow_dialog_get_instance_private(dialog);
	GtkTreeModel *tree_model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree);

	GtkTreeIter child_iter;
	gboolean child_iter_valid = gtk_tree_model_iter_children(tree_model, &child_iter, tree_iter);
	CowTreeEntry *cow_tree_entry = NULL;
	if (child_iter_valid) {
		gtk_tree_model_get(tree_model, &child_iter, 0, &cow_tree_entry,  -1);
	}


	CatIIterator *new_iter = cat_array_wo_iterator(panel_descriptions);
	int out_index = 0;
	while(cat_iiterator_has_next(new_iter)) {
		CowPanelDescription *description = (CowPanelDescription *) cat_iiterator_next(new_iter);
		CatStringWo *descr_name = cow_panel_description_get_entry_name(description);
		gboolean did_add = FALSE;

		while(!did_add) {
			if (child_iter_valid) {
				/* the child_iter is valid. Lets look if the entry of it has the same name as the descr_name */
				CatStringWo *entry_name = cow_tree_entry_get_name(cow_tree_entry);
				if (cat_string_wo_equal(entry_name, descr_name)) {
					/* the iter holds the name  of the description. We just replace the content */
					cow_tree_entry_add_description(cow_tree_entry, panel_factory, description);
					child_iter_valid = gtk_tree_model_iter_next(tree_model, &(child_iter));
					if (child_iter_valid) {
						gtk_tree_model_get(tree_model, &child_iter, 0, &cow_tree_entry,  -1);
					}
					did_add = TRUE;
				} else {
					/* lets check if the entry_name exists in the panel_descriptions */
					int idx = l_find_description_by_name(panel_descriptions, entry_name, out_index);
					if (idx<=0) {
						/* the entry-name does not exist anymore in the new panel-descriptions. Lets remove the entry*/
						child_iter_valid = gtk_tree_store_remove(GTK_TREE_STORE(tree_model), &(child_iter));
						// TODO unref row-entry ???
						if (child_iter_valid) {
							gtk_tree_model_get(tree_model, &child_iter, 0, &cow_tree_entry,  -1);
						}
					} else {
						/* the entry-name does exist but is not equal as the description-name. Maybe the order was changed */
						GtkTreeIter child_inserted;
						gtk_tree_store_insert_before(GTK_TREE_STORE(tree_model), &child_inserted, tree_iter, &child_iter);
						cow_tree_entry = cow_tree_entry_new(descr_name);
						gtk_tree_store_set((GtkTreeStore *) tree_model, &child_inserted, 0, cow_tree_entry, -1);

						if (panel_factory) {
							cow_tree_entry_add_description(cow_tree_entry, panel_factory, description);
						}
						did_add = TRUE;

					}
				}
			} else {
				gtk_tree_store_append((GtkTreeStore *) tree_model, &child_iter, tree_iter);
				cow_tree_entry = cow_tree_entry_new(descr_name);
				gtk_tree_store_set((GtkTreeStore *) tree_model, &child_iter, 0, cow_tree_entry, -1);

				if (panel_factory) {
					cow_tree_entry_add_description(cow_tree_entry, panel_factory, description);
				}
				did_add = TRUE;
			}
		}
		out_index++;
	}
	cat_unref_ptr(new_iter);

	while(child_iter_valid) {
		child_iter_valid = gtk_tree_store_remove(GTK_TREE_STORE(tree_model), &(child_iter));
	}

}


static void l_remove_all_panels(CowDialog *dialog) {
	CowDialogPrivate *priv = cow_dialog_get_instance_private(dialog);
	CatArrayWo *panel_list = priv->panel_list;
	priv->panel_list = NULL;

	if (panel_list) {
		CatIIterator *iter = cat_array_wo_iterator(panel_list);
		while(cat_iiterator_has_next(iter)) {
			CowPanel *panel = (CowPanel *) cat_iiterator_next(iter);
			cat_log_debug("closing panel of type:%p - %s", panel, g_type_name_from_instance((GTypeInstance *) panel));
			COW_PANEL_GET_CLASS(panel)->removeNotify(panel);
		}
		cat_unref_ptr(iter);

		GList *children = gtk_container_get_children(priv->w_panel_container);
		while(children) {
			GList *next_child = children->next;
			gtk_container_remove(priv->w_panel_container, (GtkWidget *) children->data);
			children = next_child;
		}
	}
	cat_unref_ptr(panel_list);
}


static void l_tree_selection_changed(GtkTreeSelection *tree_selection, gpointer dialog_as_pointer) {
	CowDialog *dialog = COW_DIALOG(dialog_as_pointer);
	CowDialogPrivate *priv = cow_dialog_get_instance_private(dialog);
	GtkTreeIter iter;
	GtkTreeModel *model;

	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(tree_selection), &model, &iter)) {

		void *data = NULL;
	    gtk_tree_model_get(model, &iter, 0, &data,  -1);
	    CowTreeEntry *cow_tree_entry = COW_TREE_ENTRY(data);

	    CatArrayWo *new_panel_list = NULL;

	    CatStringWo *new_label = NULL;

	    if (cow_tree_entry) {
	    	int factory_count = cow_tree_entry_count(cow_tree_entry);
	    	new_label = cow_tree_entry_get_name(cow_tree_entry);
	    	int idx;
	    	new_panel_list = cat_array_wo_new();
	    	for(idx=0; idx<factory_count; idx++) {
	    		CowIPanelFactory *factory = cow_tree_entry_factory_at(cow_tree_entry, idx);
	    		CowPanelDescription *description = cow_tree_entry_description_at(cow_tree_entry, idx);

	    		CowPanel *panel = cow_ipanel_factory_create(factory, dialog, description);
	    		if (panel) {
	    			cat_array_wo_append(new_panel_list, (GObject *) panel);
	    			cat_unref(panel);
	    		}
	    	}
	    }

	    cat_log_debug("new_panel_list=%p, dialog=%p", new_panel_list, dialog);
	    if (new_panel_list) {
    		l_remove_all_panels(dialog);

    		cat_unref_ptr(priv->panel_list);
	    	priv->panel_list = cat_ref_ptr(new_panel_list);

	    	if (new_panel_list) {
				CatIIterator *iter = cat_array_wo_iterator(new_panel_list);
				gboolean is_first = TRUE;
				while(cat_iiterator_has_next(iter)) {
					CowPanel *panel = (CowPanel *) cat_iiterator_next(iter);
					cat_ref_ptr(panel);
					if (is_first) {
						is_first = FALSE;
					} else {
						GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
						gtk_container_add(priv->w_panel_container, sep);
					}
					if (GTK_IS_BOX(priv->w_panel_container)) {
						gtk_box_pack_start((GtkBox *) priv->w_panel_container, (GtkWidget *) panel, TRUE, TRUE, 0);
					} else {
						gtk_container_add(priv->w_panel_container, (GtkWidget *) panel);
					}
					cat_log_debug("opening panel of type:%p - %s", panel, g_type_name_from_instance((GTypeInstance *) panel));
					COW_PANEL_GET_CLASS(panel)->addNotify(panel, dialog);
				}
				cat_unref_ptr(iter);
	    	}
	    	gtk_widget_show_all((GtkWidget *) (priv->w_panel_container));
	    	if (new_label) {
	    		gtk_label_set_text((GtkLabel *) (priv->w_lab_entry), cat_string_wo_getchars(new_label));
	    	}
	    	cat_unref_ptr(new_panel_list);
	    }

	}
}

static void l_text_cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, CowPanelDescription *con_dialog) {
	void *data;
	gtk_tree_model_get(model, iter, 0, &data, -1);
	CowTreeEntry *tree_entry = (CowTreeEntry *) data;
	CatStringWo *entry_name = cow_tree_entry_get_name(tree_entry);
	const char *label_chrs = cat_string_wo_getchars(entry_name);
	g_object_set(renderer, "text", label_chrs, "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
}



static void l_on_cancel_pressed(void *cause, gpointer user_data) {
	gtk_dialog_response(GTK_DIALOG(user_data), GTK_RESPONSE_CANCEL);
}

static void l_clean(GtkTreeModel *tree_model, GtkTreeIter *child_iter) {
	gboolean child_iter_valid = TRUE;
	while(child_iter_valid) {
		void *data = NULL;
	    gtk_tree_model_get(tree_model, child_iter, 0, &data,  -1);
	    CowTreeEntry *cow_tree_entry = COW_TREE_ENTRY(data);

	    GtkTreeIter sub_child_iter;

		gboolean sub_child_iter_valid = gtk_tree_model_iter_children(tree_model, &sub_child_iter, child_iter);
		if (sub_child_iter_valid) {
			l_clean(tree_model, &sub_child_iter);
		}

	    if (cow_tree_entry) {
			cat_unref_ptr(cow_tree_entry);
			child_iter_valid = gtk_tree_model_iter_next(tree_model, child_iter);
	    }

	}
}

static void l_destroy(GtkWidget *widget) {
	CowDialog *dialog = COW_DIALOG(widget);
	CowDialogPrivate *priv = cow_dialog_get_instance_private(dialog);
	GtkTreeModel *tree_model = gtk_tree_view_get_model((GtkTreeView *) priv->w_tree);

	GtkTreeIter child_iter;
	gboolean child_iter_valid = gtk_tree_model_iter_children(tree_model, &child_iter, NULL);
	if(child_iter_valid) {
		l_clean(tree_model, &child_iter);
	}

	GtkWidgetClass *w_class = GTK_WIDGET_CLASS(cow_dialog_parent_class);
	if (w_class->destroy) {
		w_class->destroy(widget);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CowDialog *instance = COW_DIALOG(self);
	CowDialogPrivate *priv = cow_dialog_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p] marked_config=%p", iname, self, priv->marked_configuration);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* start CowIChangeListener implementation *********************/

static void l_config_changed(CowIChangeListener *self, GObject *new_config) {
	CowDialog *instance = COW_DIALOG(self);
	CowDialogPrivate *priv = cow_dialog_get_instance_private(instance);
	if (priv->w_but_apply) {
		gboolean is_same_as_current = cow_container_is_same_config(priv->edit_container, cow_container_get_configuration(priv->edit_container), cow_container_get_configuration(priv->container));
		gtk_widget_set_sensitive(GTK_WIDGET(priv->w_but_apply), !is_same_as_current);
	}
//	gboolean is_same_as_marked = cow_container_is_same_config(priv->edit_container, priv->marked_configuration, new_config);
//	gtk_widget_set_sensitive(GTK_WIDGET(priv->w_but_apply), !is_same_as_marked);

}

static void l_change_listener_iface_init(CowIChangeListenerInterface *iface) {
	iface->configChanged = l_config_changed;
}

/********************* end CowIChangeListener implementation *********************/
