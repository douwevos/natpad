/*
   File:    cowdialog.h
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

#ifndef COWDIALOG_H_
#define COWDIALOG_H_

#include "cowpanelmodel.h"
#include "cowpaneldescription.h"
#include "../cowcontainer.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define COW_TYPE_DIALOG              (cow_dialog_get_type())
#define COW_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cow_dialog_get_type(), CowDialog))
#define COW_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), COW_TYPE_DIALOG, CowDialogClass))
#define COW_IS_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), COW_TYPE_DIALOG))
#define COW_IS_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), COW_TYPE_DIALOG))
#define COW_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), COW_TYPE_DIALOG, CowDialogClass))

enum _CowDialogType {
	COW_DIALOG_OK_ONLY,
	COW_DIALOG_OK_AND_CANCEL_ONLY,
	COW_DIALOG_OK_CANCEL_AND_APPLY,
};

typedef enum _CowDialogType             CowDialogType;


typedef struct _CowDialog               CowDialog;
typedef struct _CowDialogPrivate        CowDialogPrivate;
typedef struct _CowDialogClass          CowDialogClass;


struct _CowDialog {
	GtkDialog parent;
};

struct _CowDialogClass {
	GtkDialogClass parent_class;
};


GType cow_dialog_get_type();

CowDialog *cow_dialog_new(CowPanelModel *panel_model, CowContainer *container, CowDialogType dialog_type);

int cow_dialog_show(CowDialog *cow_dialog);

GtkTreeIter cow_dialog_add_entry(CowDialog *dialog, GtkTreeIter *tree_iter, CowIPanelFactory *panel_factory, CowPanelDescription *panel_description);
void cow_dialog_update_entries(CowDialog *dialog, GtkTreeIter *tree_iter, CowIPanelFactory *panel_factory, CatArrayWo *panel_descriptions);


CowContainer *cow_dialog_get_edit_container(CowDialog *dialog);

G_END_DECLS

#endif /* COWDIALOG_H_ */
