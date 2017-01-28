/*
   File:    jagprefsjredialog.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 12, 2014
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

#ifndef JAGPREFSJREDIALOG_H_
#define JAGPREFSJREDIALOG_H_

#include "../jagprefsjrewo.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_PREFS_JRE_DIALOG              (jag_prefs_jre_dialog_get_type())
#define JAG_PREFS_JRE_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_prefs_jre_dialog_get_type(), JagPrefsJreDialog))
#define JAG_PREFS_JRE_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PREFS_JRE_DIALOG, JagPrefsJreDialogClass))
#define JAG_IS_PREFS_JRE_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PREFS_JRE_DIALOG))
#define JAG_IS_PREFS_JRE_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PREFS_JRE_DIALOG))
#define JAG_PREFS_JRE_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PREFS_JRE_DIALOG, JagPrefsJreDialogClass))


typedef struct _JagPrefsJreDialog               JagPrefsJreDialog;
typedef struct _JagPrefsJreDialogPrivate        JagPrefsJreDialogPrivate;
typedef struct _JagPrefsJreDialogClass          JagPrefsJreDialogClass;


struct _JagPrefsJreDialog {
	GObject parent;
};

struct _JagPrefsJreDialogClass {
	GObjectClass parent_class;
};

GType jag_prefs_jre_dialog_get_type();

JagPrefsJreDialog *jag_prefs_jre_dialog_new(VipService *vip_service, JagPrefsJreWo *e_jre_wo);

int jag_prefs_jre_dialog_run(JagPrefsJreDialog *dialog);

void jag_prefs_jre_dialog_close(JagPrefsJreDialog *dialog);

G_END_DECLS

#endif /* JAGPREFSJREDIALOG_H_ */
