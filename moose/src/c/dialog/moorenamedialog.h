/*
   File:    moorenamedialog.h
   Project: mooose-explorer
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

#ifndef MOORENAMEDIALOG_H_
#define MOORENAMEDIALOG_H_

#include <gtk/gtk.h>
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define MOO_TYPE_RENAME_DIALOG              (moo_rename_dialog_get_type())
#define MOO_RENAME_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_rename_dialog_get_type(), MooRenameDialog))
#define MOO_RENAME_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_RENAME_DIALOG, MooRenameDialogClass))
#define MOO_IS_RENAME_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_RENAME_DIALOG))
#define MOO_IS_RENAME_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_RENAME_DIALOG))
#define MOO_RENAME_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_RENAME_DIALOG, MooRenameDialogClass))

typedef struct _MooRenameDialog               MooRenameDialog;
typedef struct _MooRenameDialogPrivate        MooRenameDialogPrivate;
typedef struct _MooRenameDialogClass          MooRenameDialogClass;

struct _MooRenameDialog {
	GtkDialog parent;
};

struct _MooRenameDialogClass {
	GtkDialogClass parent_class;
};

GType moo_rename_dialog_get_type();

MooRenameDialog *moo_rename_dialog_new(LeaFrame *frame);

int moo_rename_dialog_run(MooRenameDialog *rename_dialog, CatStringWo *a_initial_name);

CatStringWo *moo_rename_dialog_get_new_name(MooRenameDialog *rename_dialog);

void moo_rename_dialog_set_message(MooRenameDialog *rename_dialog, CatStringWo *a_new_message);

G_END_DECLS

#endif /* MOORENAMEDIALOG_H_ */
