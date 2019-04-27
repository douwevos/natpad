/*
   File:    leabasicinputdialog.h
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

#ifndef LEABASICINPUTDIALOG_H_
#define LEABASICINPUTDIALOG_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_BASIC_INPUT_DIALOG            (lea_basic_input_dialog_get_type())
#define LEA_BASIC_INPUT_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_basic_input_dialog_get_type(), LeaBasicInputDialog))
#define LEA_BASIC_INPUT_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_BASIC_INPUT_DIALOG, LeaBasicInputDialogClass))
#define LEA_IS_BASIC_INPUT_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_BASIC_INPUT_DIALOG))
#define LEA_IS_BASIC_INPUT_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_BASIC_INPUT_DIALOG))
#define LEA_BASIC_INPUT_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_BASIC_INPUT_DIALOG, LeaBasicInputDialogClass))

typedef struct _LeaBasicInputDialog	   		LeaBasicInputDialog;
typedef struct _LeaBasicInputDialogClass	  	LeaBasicInputDialogClass;

struct _LeaBasicInputDialog {
	GtkDialog parent;
	GtkWidget *entry;
};

struct _LeaBasicInputDialogClass {
	GtkDialogClass parent_class;
};


GType lea_basic_input_dialog_get_type();

LeaBasicInputDialog *lea_basic_input_dialog_new(GtkWidget *owner, const gchar *label, const gchar *message);

const gchar *lea_basic_input_dialog_get_entry_text(LeaBasicInputDialog *dialog);

G_END_DECLS

#endif /* LEABASICINPUTDIALOG_H_ */
