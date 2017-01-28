/*
   File:    elkdialogs.h
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 4, 2015
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

#ifndef DIALOG_ELKDIALOGS_H_
#define DIALOG_ELKDIALOGS_H_

#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_DIALOGS              (elk_dialogs_get_type())
#define ELK_DIALOGS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_dialogs_get_type(), ElkDialogs))
#define ELK_DIALOGS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_DIALOGS, ElkDialogsClass))
#define ELK_IS_DIALOGS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_DIALOGS))
#define ELK_IS_DIALOGS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_DIALOGS))
#define ELK_DIALOGS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_DIALOGS, ElkDialogsClass))


typedef struct _ElkDialogs               ElkDialogs;
typedef struct _ElkDialogsPrivate        ElkDialogsPrivate;
typedef struct _ElkDialogsClass          ElkDialogsClass;


struct _ElkDialogs {
	GObject parent;
};

struct _ElkDialogsClass {
	GObjectClass parent_class;
};


GType elk_dialogs_get_type();

ElkDialogs *elk_dialogs_new(LeaFrame *frame);

CatStringWo *elk_dialogs_save_file_selector(ElkDialogs *dialogs);

GtkResponseType elk_dialogs_save_before_close(ElkDialogs *dialogs);

G_END_DECLS

#endif /* DIALOG_ELKDIALOGS_H_ */
