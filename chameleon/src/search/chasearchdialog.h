/*
   File:    chasearchdialog.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 8, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef SEARCH_CHASEARCHDIALOG_H_
#define SEARCH_CHASEARCHDIALOG_H_

#include "../chaeditor.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_SEARCH_DIALOG              (cha_search_dialog_get_type())
#define CHA_SEARCH_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_search_dialog_get_type(), ChaSearchDialog))
#define CHA_SEARCH_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SEARCH_DIALOG, ChaSearchDialogClass))
#define CHA_IS_SEARCH_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SEARCH_DIALOG))
#define CHA_IS_SEARCH_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SEARCH_DIALOG))
#define CHA_SEARCH_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SEARCH_DIALOG, ChaSearchDialogClass))


typedef struct _ChaSearchDialog               ChaSearchDialog;
typedef struct _ChaSearchDialogPrivate        ChaSearchDialogPrivate;
typedef struct _ChaSearchDialogClass          ChaSearchDialogClass;


struct _ChaSearchDialog {
	GObject parent;
};

struct _ChaSearchDialogClass {
	GObjectClass parent_class;
};


GType cha_search_dialog_get_type();

struct _ChaSearchService;

ChaSearchDialog *cha_search_dialog_new(struct _ChaSearchService *search_service);

void cha_search_dialog_set_editor(ChaSearchDialog *search_dialog, ChaEditor *editor);

void cha_search_dialog_show(ChaSearchDialog *dialog, GtkWindow *owner);


G_END_DECLS

#endif /* SEARCH_CHASEARCHDIALOG_H_ */
