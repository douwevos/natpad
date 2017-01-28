/*
   File:    elkactioncloseall.h
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

#ifndef ELKACTIONCLOSEALL_H_
#define ELKACTIONCLOSEALL_H_

#include "../elkiservice.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_CLOSE_ALL              (elk_action_close_all_get_type())
#define ELK_ACTION_CLOSE_ALL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_close_all_get_type(), ElkActionCloseAll))
#define ELK_ACTION_CLOSE_ALL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_CLOSE_ALL, ElkActionCloseAllClass))
#define ELK_IS_ACTION_CLOSE_ALL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_CLOSE_ALL))
#define ELK_IS_ACTION_CLOSE_ALL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_CLOSE_ALL))
#define ELK_ACTION_CLOSE_ALL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_CLOSE_ALL, ElkActionCloseAllClass))
#define ELK_ACTION_CLOSE_ALL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ELK_TYPE_ACTION_CLOSE_ALL, ElkActionCloseAllPrivate))

typedef struct _ElkActionCloseAll               ElkActionCloseAll;
typedef struct _ElkActionCloseAllPrivate        ElkActionCloseAllPrivate;
typedef struct _ElkActionCloseAllClass          ElkActionCloseAllClass;


struct _ElkActionCloseAll {
	LeaAction parent;
	ElkActionCloseAllPrivate *priv;
};

struct _ElkActionCloseAllClass {
	LeaActionClass parent_class;
};


GType elk_action_close_all_get_type();

ElkActionCloseAll *elk_action_close_all_new(ElkIService *elk_iservice);

void elk_action_close_all_set_editor_list(ElkActionCloseAll *action, CatArrayWo *a_editor_list);

G_END_DECLS

#endif /* ELKACTIONCLOSEALL_H_ */
