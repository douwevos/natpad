/*
   File:    elkprefstemplatedialog.h
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 25, 2014
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

#ifndef ELKPREFSTEMPLATEDIALOG_H_
#define ELKPREFSTEMPLATEDIALOG_H_

#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_PREFS_TEMPLATE_DIALOG              (dra_prefs_template_dialog_get_type())
#define DRA_PREFS_TEMPLATE_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_prefs_template_dialog_get_type(), DraPrefsTemplateDialog))
#define DRA_PREFS_TEMPLATE_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PREFS_TEMPLATE_DIALOG, DraPrefsTemplateDialogClass))
#define DRA_IS_PREFS_TEMPLATE_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PREFS_TEMPLATE_DIALOG))
#define DRA_IS_PREFS_TEMPLATE_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PREFS_TEMPLATE_DIALOG))
#define DRA_PREFS_TEMPLATE_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PREFS_TEMPLATE_DIALOG, DraPrefsTemplateDialogClass))


typedef struct _DraPrefsTemplateDialog               DraPrefsTemplateDialog;
typedef struct _DraPrefsTemplateDialogPrivate        DraPrefsTemplateDialogPrivate;
typedef struct _DraPrefsTemplateDialogClass          DraPrefsTemplateDialogClass;


struct _DraPrefsTemplateDialog {
	GObject parent;
};

struct _DraPrefsTemplateDialogClass {
	GObjectClass parent_class;
};


GType dra_prefs_template_dialog_get_type();

DraPrefsTemplateDialog *dra_prefs_template_dialog_new(ChaPrefsTemplateWo *e_template);

int dra_prefs_template_dialog_run(DraPrefsTemplateDialog *dialog);

void dra_prefs_template_dialog_close(DraPrefsTemplateDialog *dialog);


G_END_DECLS

#endif /* ELKPREFSTEMPLATEDIALOG_H_ */
