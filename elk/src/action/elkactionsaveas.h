/*
   File:    elkactionsaveas.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jan 11, 2013
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

#ifndef ELKACTIONSAVEAS_H_
#define ELKACTIONSAVEAS_H_

#include <dragonfly.h>
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_SAVE_AS              (elk_action_save_as_get_type())
#define ELK_ACTION_SAVE_AS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_save_as_get_type(), ElkActionSaveAs))
#define ELK_ACTION_SAVE_AS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_SAVE_AS, ElkActionSaveAsClass))
#define ELK_IS_ACTION_SAVE_AS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_SAVE_AS))
#define ELK_IS_ACTION_SAVE_AS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_SAVE_AS))
#define ELK_ACTION_SAVE_AS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_SAVE_AS, ElkActionSaveAsClass))

typedef struct _ElkActionSaveAs               ElkActionSaveAs;
typedef struct _ElkActionSaveAsPrivate        ElkActionSaveAsPrivate;
typedef struct _ElkActionSaveAsClass          ElkActionSaveAsClass;


struct _ElkActionSaveAs {
	LeaAction parent;
};

struct _ElkActionSaveAsClass {
	LeaActionClass parent_class;
};


GType elk_action_save_as_get_type();

ElkActionSaveAs *elk_action_save_as_new();

void elk_action_save_as_set_editor_panel(ElkActionSaveAs *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ELKACTIONSAVEAS_H_ */
