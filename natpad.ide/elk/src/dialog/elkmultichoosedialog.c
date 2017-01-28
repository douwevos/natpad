/*
   File:    elkmultichoosedialog.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 13, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#include "elkmultichoosedialog.h"
#include <caterpillar.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkMultiChooseDialog"
#include <logging/catlog.h>

#define RESPONSE_SELECT_ALL 20
#define RESPONSE_SELECT_NONE 30


struct _ElkMultiChooseDialogPrivate {
	CatArrayWo *a_editor_list;
	CatArrayWo *e_selected_list;
	gboolean *item_selected;
	GtkWidget *tree_view;

};

G_DEFINE_TYPE (ElkMultiChooseDialog, elk_multi_choose_dialog, GTK_TYPE_DIALOG)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void elk_multi_choose_dialog_class_init(ElkMultiChooseDialogClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkMultiChooseDialogPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

//	GtkDialogClass *sub_clazz = GTK_DIALOG_CLASS(clazz);
}

static void elk_multi_choose_dialog_init(ElkMultiChooseDialog *instance) {
	ElkMultiChooseDialogPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_MULTI_CHOOSE_DIALOG, ElkMultiChooseDialogPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkMultiChooseDialog *instance = ELK_MULTI_CHOOSE_DIALOG(object);
	ElkMultiChooseDialogPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_editor_list);
	cat_unref_ptr(priv->e_selected_list);
	cat_free_ptr(priv->item_selected);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}



static void l_text_cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, ElkMultiChooseDialog *selector) {
	ElkMultiChooseDialogPrivate *priv = ELK_MULTI_CHOOSE_DIALOG_GET_PRIVATE(selector);

	void *data;
	gtk_tree_model_get(model, iter, 0, &data, -1);

	DraEditorPanel *editor = DRA_EDITOR_PANEL(data);
	cat_log_on_debug({
//		char *chrs = cat_string_wo_getchars(source->name);
//		cat_log_debug("l_text_cell_data_func: source =%lx, name=%s\n", (gulong) source, chrs);
	});
	if (GTK_IS_CELL_RENDERER_TEXT(renderer)) {
		const CatStringWo *name = dra_editor_panel_get_name(editor);
		g_object_set(renderer, "text", cat_string_wo_getchars(name), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
	} else if (GTK_IS_CELL_RENDERER_TOGGLE(renderer)) {
		int idx = cat_array_wo_find_index(priv->a_editor_list, editor, -1);
		if (idx>=0) {
			gtk_cell_renderer_toggle_set_active(GTK_CELL_RENDERER_TOGGLE(renderer), priv->item_selected[idx]);
//			g_object_set(renderer, "active", selector->item_selected[idx], NULL);
		}
	}
}

static void l_row_toggled(GtkCellRendererToggle *cell_renderer, gchar *path, gpointer user_data) {
	cat_log_detail("path=%s\n", path);
	ElkMultiChooseDialog *multi_choose_dialog = (ElkMultiChooseDialog *) user_data;
	ElkMultiChooseDialogPrivate *priv = ELK_MULTI_CHOOSE_DIALOG_GET_PRIVATE(multi_choose_dialog);
	GtkTreeModel *tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(priv->tree_view));
	GtkTreeIter tree_iter;
	if (gtk_tree_model_get_iter_from_string(tree_model, &tree_iter, path)) {
		void *data;
		gtk_tree_model_get(tree_model, &tree_iter, 0, &data, -1);

		int idx = cat_array_wo_find_index(priv->a_editor_list, data, -1);
		if (idx>=0) {
			priv->item_selected[idx] = !priv->item_selected[idx];
			cat_log_detail("toggling:%s\n", path);
			return;
		}
	}
}

static void l_select_all(ElkMultiChooseDialog* multi_choose_dialog, gboolean activate) {
	ElkMultiChooseDialogPrivate *priv = ELK_MULTI_CHOOSE_DIALOG_GET_PRIVATE(multi_choose_dialog);
	GtkTreeModel *tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(priv->tree_view));
	GtkListStore *list_model = GTK_LIST_STORE(tree_model);
	GtkTreeIter tree_iter;

	int idx;
	int item_count = cat_array_wo_size(priv->a_editor_list);
	for(idx=0; idx<item_count; idx++) {
		priv->item_selected[idx] = activate;
	}
	gboolean keep_running = gtk_tree_model_get_iter_first(tree_model, &tree_iter);
	while(keep_running) {
		void *data;
		gtk_tree_model_get(tree_model, &tree_iter, 0, &data, -1);
		gtk_list_store_set(list_model, &tree_iter, 0, data, -1);
		keep_running = gtk_tree_model_iter_next(tree_model, &tree_iter);
	}
}


ElkMultiChooseDialog *elk_multi_choose_dialog_new(GtkWidget *top_window, CatArrayWo *a_editor_list) {
	ElkMultiChooseDialog *result = g_object_new(ELK_TYPE_MULTI_CHOOSE_DIALOG, NULL);
	cat_ref_anounce(result);
	ElkMultiChooseDialogPrivate *priv = result->priv;
	priv->a_editor_list = cat_ref_ptr(a_editor_list);
	int item_count = cat_array_wo_size(a_editor_list);
	priv->item_selected = g_new(gboolean, item_count);
	priv->e_selected_list = cat_array_wo_new();


	GtkDialog *dialog = GTK_DIALOG(result);
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(top_window));
	gtk_window_set_modal(GTK_WINDOW(result), TRUE);

	GtkWidget *msg = gtk_label_new_with_mnemonic("Select the documents you wish to save.");
	GtkBox *dialog_box = GTK_BOX(gtk_bin_get_child(GTK_BIN(dialog)));
	gtk_box_pack_start(dialog_box, msg, FALSE, TRUE, 2);

	GtkListStore *list_model = gtk_list_store_new(1, G_TYPE_POINTER);

	GtkTreeIter tree_iter;
	int idx;
	for(idx=0; idx<item_count; idx++) {
		priv->item_selected[idx] = TRUE;
		gtk_list_store_append(list_model, &tree_iter);
		gtk_list_store_set(list_model, &tree_iter, 0, cat_array_wo_get(a_editor_list, idx), -1);
	}

	GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_model));
	priv->tree_view = tree_view;
	GtkCellRenderer *renderer_toggle = gtk_cell_renderer_toggle_new();
	int xpad, ypad;
	gtk_cell_renderer_get_padding(renderer_toggle, &xpad, &ypad);
	gtk_cell_renderer_set_padding(renderer_toggle, xpad, 0);
	gtk_cell_renderer_set_sensitive(renderer_toggle, TRUE);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(tree_view), 0, "Name", renderer_toggle, (GtkTreeCellDataFunc) l_text_cell_data_func, result, NULL);
	GtkTreeViewColumn *column = gtk_tree_view_get_column(GTK_TREE_VIEW(tree_view),0);
	gtk_tree_view_column_set_expand(column, FALSE);
	g_signal_connect(renderer_toggle, "toggled", G_CALLBACK(l_row_toggled), result);

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_renderer_get_padding(renderer, &xpad, &ypad);
	gtk_cell_renderer_set_padding(renderer, xpad, 0);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(tree_view), 1, "Name", renderer, (GtkTreeCellDataFunc) l_text_cell_data_func, result, NULL);
	column = gtk_tree_view_get_column(GTK_TREE_VIEW(tree_view),1);
	gtk_tree_view_column_set_expand(column, TRUE);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW (tree_view), FALSE);





	GtkWidget *frame = gtk_frame_new("Documents");
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);

	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), tree_view);

	gtk_container_add(GTK_CONTAINER(frame), scrolled_window);


	gtk_box_pack_start(dialog_box, frame, TRUE, TRUE, 2);


	gtk_dialog_add_button(GTK_DIALOG(result), "Select _All", RESPONSE_SELECT_ALL);
	gtk_dialog_add_button(GTK_DIALOG(result), "Select _None", RESPONSE_SELECT_NONE);
	gtk_dialog_add_button(GTK_DIALOG(result), "_Cancel", GTK_RESPONSE_CANCEL);
	gtk_dialog_add_button(GTK_DIALOG(result), "_OK", GTK_RESPONSE_OK);

	gtk_window_set_default_size(GTK_WINDOW(dialog), 250, 325);

	gtk_widget_show_all(GTK_WIDGET(dialog_box));

	return result;
}







int elk_multi_choose_dialog_run(ElkMultiChooseDialog* multi_choose_dialog) {
	ElkMultiChooseDialogPrivate *priv = ELK_MULTI_CHOOSE_DIALOG_GET_PRIVATE(multi_choose_dialog);
	gboolean cont = TRUE;
	int response;
	cat_array_wo_clear(priv->e_selected_list);
	while(cont) {
		response = gtk_dialog_run(GTK_DIALOG(multi_choose_dialog));
		switch(response) {
			case GTK_RESPONSE_OK :
			case GTK_RESPONSE_CANCEL : {
				cont = FALSE;
			} break;
			case RESPONSE_SELECT_ALL : {
				l_select_all(multi_choose_dialog, TRUE);
			} break;
			case RESPONSE_SELECT_NONE : {
				l_select_all(multi_choose_dialog, FALSE);
			} break;
			default : {
				cont = FALSE;
			} break;
		}
	}
	if (response==GTK_RESPONSE_OK) {
		int idx=0;
		int item_count = cat_array_wo_size(priv->a_editor_list);
		for(idx=0; idx<item_count; idx++) {
			if (priv->item_selected[idx]) {
				GObject *editor = cat_array_wo_get(priv->a_editor_list, idx);
				cat_array_wo_append(priv->e_selected_list, editor);
			}
		}
	}
	return response;
}


CatArrayWo *elk_multi_choose_dialog_get_selected(ElkMultiChooseDialog* multi_choose_dialog) {
	ElkMultiChooseDialogPrivate *priv = ELK_MULTI_CHOOSE_DIALOG_GET_PRIVATE(multi_choose_dialog);
	return priv->e_selected_list;
}
