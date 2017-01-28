/*
   File:    leabasicinputdialog.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jun 20, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#include "leabasicinputdialog.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaBasicInputDialog"
#include <logging/catlog.h>

G_DEFINE_TYPE (LeaBasicInputDialog, lea_basic_input_dialog, GTK_TYPE_DIALOG)

static void lea_basic_input_dialog_class_init(LeaBasicInputDialogClass *clazz) {
}

static void lea_basic_input_dialog_init(LeaBasicInputDialog *instance) {
}


static void _activate_ok_button(GdkEventKey *eev, gpointer button_as_object) {
	GtkWidget *ok_button = GTK_WIDGET(button_as_object);
	gtk_button_clicked(GTK_BUTTON(ok_button));
}

LeaBasicInputDialog *lea_basic_input_dialog_new(GtkWidget *owner, const gchar *label, const gchar *message) {
	LeaBasicInputDialog *result = g_object_new(LEA_TYPE_BASIC_INPUT_DIALOG, NULL);
	cat_ref_anounce(result);
	GtkDialog *dialog = GTK_DIALOG(result);
	gtk_window_set_modal(GTK_WINDOW(result), TRUE);

	GtkWidget *pp = owner;
	while(pp) {
		if (GTK_IS_WINDOW(pp)) {
			break;
		}
		pp = gtk_widget_get_parent(pp);
	}

	if (pp) {
		gtk_window_set_transient_for(GTK_WINDOW(result), (GtkWindow *) pp);
	}

	GtkBox *content_area = GTK_BOX(gtk_dialog_get_content_area(dialog));
	cat_log_debug("content_area=%p", content_area);


	GtkWidget *msg = gtk_label_new_with_mnemonic(message);
	gtk_box_pack_start(content_area, msg, TRUE, TRUE, 2);

	GtkWidget *hboxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(content_area, hboxHandle, TRUE, TRUE, 2);


	if (label) {
		GtkWidget *w_label = gtk_label_new_with_mnemonic(label);
		gtk_box_pack_start(GTK_BOX(hboxHandle), w_label, FALSE, TRUE, 2);
	}

	GtkWidget *entry = gtk_entry_new();
	result->entry = entry;
	gtk_box_pack_start(GTK_BOX(hboxHandle), entry, TRUE, TRUE, 2);


	gtk_dialog_add_button(GTK_DIALOG(result), "_Cancel", GTK_RESPONSE_CANCEL);
	GtkWidget *ok_button = gtk_dialog_add_button(GTK_DIALOG(result), "_OK", GTK_RESPONSE_OK);
	g_signal_connect(entry, "activate", (GCallback) _activate_ok_button, ok_button);


	gtk_widget_show_all(GTK_WIDGET(content_area));
	return result;
}

const gchar *lea_basic_input_dialog_get_entry_text(LeaBasicInputDialog *dialog) {
	return gtk_entry_get_text(GTK_ENTRY(dialog->entry));
}
