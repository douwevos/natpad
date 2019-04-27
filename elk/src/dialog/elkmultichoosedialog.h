/*
   File:    elkmultichoosedialog.h
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

#ifndef ELKMULTICHOOSEDIALOG_H_
#define ELKMULTICHOOSEDIALOG_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_MULTI_CHOOSE_DIALOG              (elk_multi_choose_dialog_get_type())
#define ELK_MULTI_CHOOSE_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_multi_choose_dialog_get_type(), ElkMultiChooseDialog))
#define ELK_MULTI_CHOOSE_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_MULTI_CHOOSE_DIALOG, ElkMultiChooseDialogClass))
#define ELK_IS_MULTI_CHOOSE_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_MULTI_CHOOSE_DIALOG))
#define ELK_IS_MULTI_CHOOSE_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_MULTI_CHOOSE_DIALOG))
#define ELK_MULTI_CHOOSE_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_MULTI_CHOOSE_DIALOG, ElkMultiChooseDialogClass))

typedef struct _ElkMultiChooseDialog               ElkMultiChooseDialog;
typedef struct _ElkMultiChooseDialogPrivate        ElkMultiChooseDialogPrivate;
typedef struct _ElkMultiChooseDialogClass          ElkMultiChooseDialogClass;


struct _ElkMultiChooseDialog {
	GtkDialog parent;
};

struct _ElkMultiChooseDialogClass {
	GtkDialogClass parent_class;
};


GType elk_multi_choose_dialog_get_type();

ElkMultiChooseDialog *elk_multi_choose_dialog_new(GtkWidget *top_window, CatArrayWo *e_editor_list);

int elk_multi_choose_dialog_run(ElkMultiChooseDialog* multi_choose_dialog);

CatArrayWo *elk_multi_choose_dialog_get_selected(ElkMultiChooseDialog* multi_choose_dialog);

G_END_DECLS

#endif /* ELKMULTICHOOSEDIALOG_H_ */
