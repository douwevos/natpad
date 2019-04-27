/*
   File:    elkactionclose.h
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

#ifndef ELKACTIONCLOSE_H_
#define ELKACTIONCLOSE_H_

#include <dragonfly.h>
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_CLOSE              (elk_action_close_get_type())
#define ELK_ACTION_CLOSE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_close_get_type(), ElkActionClose))
#define ELK_ACTION_CLOSE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_CLOSE, ElkActionCloseClass))
#define ELK_IS_ACTION_CLOSE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_CLOSE))
#define ELK_IS_ACTION_CLOSE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_CLOSE))
#define ELK_ACTION_CLOSE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_CLOSE, ElkActionCloseClass))

typedef struct _ElkActionClose               ElkActionClose;
typedef struct _ElkActionClosePrivate        ElkActionClosePrivate;
typedef struct _ElkActionCloseClass          ElkActionCloseClass;


struct _ElkActionClose {
	LeaAction parent;
};

struct _ElkActionCloseClass {
	LeaActionClass parent_class;
};


GType elk_action_close_get_type();

ElkActionClose *elk_action_close_new();

void elk_action_close_set_editor_panel(ElkActionClose *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ELKACTIONCLOSE_H_ */
