/*
   File:    mooaddprojectdialog.h
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 24, 2013
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef MOOADDPROJECTDIALOG_H_
#define MOOADDPROJECTDIALOG_H_

#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MOO_TYPE_ADD_PROJECT_DIALOG              (moo_add_project_dialog_get_type())
#define MOO_ADD_PROJECT_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_add_project_dialog_get_type(), MooAddProjectDialog))
#define MOO_ADD_PROJECT_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ADD_PROJECT_DIALOG, MooAddProjectDialogClass))
#define MOO_IS_ADD_PROJECT_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ADD_PROJECT_DIALOG))
#define MOO_IS_ADD_PROJECT_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ADD_PROJECT_DIALOG))
#define MOO_ADD_PROJECT_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ADD_PROJECT_DIALOG, MooAddProjectDialogClass))

typedef struct _MooAddProjectDialog               MooAddProjectDialog;
typedef struct _MooAddProjectDialogPrivate        MooAddProjectDialogPrivate;
typedef struct _MooAddProjectDialogClass          MooAddProjectDialogClass;

struct _MooAddProjectDialog {
	GtkDialog parent;
};

struct _MooAddProjectDialogClass {
	GtkDialogClass parent_class;
};

GType moo_add_project_dialog_get_type();

MooAddProjectDialog *moo_add_project_dialog_new();

int moo_add_project_dialog_run(MooAddProjectDialog *add_project_dialog);

CatStringWo *moo_add_project_dialog_get_module_name(MooAddProjectDialog *add_project_dialog);
CatStringWo *moo_add_project_dialog_get_base_path(MooAddProjectDialog *add_project_dialog);

G_END_DECLS

#endif /* MOOADDPROJECTDIALOG_H_ */
