/*
   File:    moorenamedialog.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Jul 30, 2012
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

#include "moorenamedialog.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooRenameDialog"
#include <logging/catlog.h>

struct _MooRenameDialogPrivate {
	GtkWidget *w_txt_name;
	GtkWidget *w_lab_message;
	GtkWidget *w_but_ok;

	CatStringWo *a_initial_name;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooRenameDialog, moo_rename_dialog, GTK_TYPE_DIALOG)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_rename_dialog_class_init(MooRenameDialogClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_rename_dialog_init(MooRenameDialog *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooRenameDialog *instance = MOO_RENAME_DIALOG(object);
	MooRenameDialogPrivate *priv = moo_rename_dialog_get_instance_private(instance);
	cat_unref_ptr(priv->a_initial_name);
	G_OBJECT_CLASS(moo_rename_dialog_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_rename_dialog_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooRenameDialog *moo_rename_dialog_new(LeaFrame *frame) {
	MooRenameDialog *result = g_object_new(MOO_TYPE_RENAME_DIALOG, NULL);
	cat_ref_anounce(result);
	MooRenameDialogPrivate *priv = moo_rename_dialog_get_instance_private(result);
	gtk_window_set_transient_for(GTK_WINDOW(result), GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(frame))));
	gtk_window_set_modal(GTK_WINDOW(result), TRUE);
	gtk_widget_set_size_request(GTK_WIDGET(result), 600, 150);

	GtkBox *vboxHandle = (GtkBox *) gtk_bin_get_child(GTK_BIN(result));

	GtkWidget *msg = gtk_label_new_with_mnemonic("Rename");
	gtk_box_pack_start(vboxHandle, msg, TRUE, TRUE, 2);

	GtkWidget *grid = gtk_grid_new();
//	GtkWidget *table = gtk_table_new(2,3,FALSE);
//	gtk_box_pack_start(vboxHandle, table, TRUE, TRUE, 2);
	gtk_box_pack_start(vboxHandle, grid, TRUE, TRUE, 2);

	GtkWidget *w_label = gtk_label_new_with_mnemonic("_New name:");

	GValue gvalue = { 0.f, };
	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.0f );
	g_object_set_property( G_OBJECT(w_label), "xalign", &gvalue );
	g_value_unset( &gvalue );

	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.5f );
	g_object_set_property( G_OBJECT(w_label), "yalign", &gvalue );
	g_value_unset( &gvalue );


//	gtk_table_attach((GtkTable *) table, w_label,0,1,0,1, GTK_FILL, GTK_FILL, 6, 0);
	gtk_grid_attach((GtkGrid *) grid, w_label, 0,0, 1,1);

	GtkWidget *entry = gtk_entry_new();
	priv->w_txt_name = entry;
	gtk_widget_set_can_default(entry, FALSE);

	gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);

//	gtk_table_attach((GtkTable *) table, entry,1,3,0,1, GTK_FILL|GTK_EXPAND, GTK_FILL, 0,0);
	gtk_grid_attach((GtkGrid *) grid, entry, 1,0, 2,1);
	gtk_widget_set_hexpand(entry, TRUE);

	GtkWidget *w_message = gtk_label_new_with_mnemonic("");
	priv->w_lab_message = w_message;


	GtkCssProvider *emergency_provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(emergency_provider, ".redmsg { color: rgba(255, 0.0, 0.0, 255); }", -1, NULL);
	gtk_style_context_add_provider(gtk_widget_get_style_context(w_message), GTK_STYLE_PROVIDER(emergency_provider),
	                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.0f );
	g_object_set_property( G_OBJECT(w_message), "xalign", &gvalue );
	g_value_unset( &gvalue );

	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.5f );
	g_object_set_property( G_OBJECT(w_message), "yalign", &gvalue );
	g_value_unset( &gvalue );

//	gtk_table_attach((GtkTable *) table, w_message,0,3,1,2, GTK_FILL, GTK_FILL, 6, 0);
	gtk_grid_attach((GtkGrid *) grid, w_message, 0,1, 3,1);

	gtk_dialog_add_button((GtkDialog *) result, "Cancel", GTK_RESPONSE_CANCEL);
	priv->w_but_ok = gtk_dialog_add_button((GtkDialog *) result, "OK", GTK_RESPONSE_OK);
	gtk_widget_set_can_default(priv->w_but_ok, TRUE);
	gtk_widget_grab_default(priv->w_but_ok);
	gtk_dialog_set_default_response((GtkDialog *) result, 1);

	return result;
}

int moo_rename_dialog_run(MooRenameDialog *rename_dialog, CatStringWo *a_initial_name) {
	MooRenameDialogPrivate *priv = moo_rename_dialog_get_instance_private(rename_dialog);
	cat_ref_swap(priv->a_initial_name, a_initial_name);
	gtk_entry_set_text(GTK_ENTRY(priv->w_txt_name), cat_string_wo_getchars(a_initial_name));
	gtk_widget_show_all(GTK_WIDGET(rename_dialog));
//	gtk_widget_set_sensitive(priv->w_but_ok, FALSE);
	return gtk_dialog_run(GTK_DIALOG(rename_dialog));
}


CatStringWo *moo_rename_dialog_get_new_name(MooRenameDialog *rename_dialog) {
	MooRenameDialogPrivate *priv = moo_rename_dialog_get_instance_private(rename_dialog);
	const gchar *txt_as_chars = gtk_entry_get_text(GTK_ENTRY(priv->w_txt_name));
	return cat_string_wo_new_with(txt_as_chars);
}

void moo_rename_dialog_set_message(MooRenameDialog *rename_dialog, CatStringWo *a_new_message) {
	MooRenameDialogPrivate *priv = moo_rename_dialog_get_instance_private(rename_dialog);
	if (a_new_message) {
		gtk_label_set_text(GTK_LABEL(priv->w_lab_message), cat_string_wo_getchars(a_new_message));
		cat_unref_ptr(a_new_message);
	} else {
		gtk_label_set_text(GTK_LABEL(priv->w_lab_message), "");
	}
}

