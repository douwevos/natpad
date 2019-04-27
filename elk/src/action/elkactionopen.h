/*
   File:    elkactionopen.h
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 8, 2012
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

#ifndef ELKACTIONOPEN_H_
#define ELKACTIONOPEN_H_

#include "../elkiservice.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_OPEN              (elk_action_open_get_type())
#define ELK_ACTION_OPEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_open_get_type(), ElkActionOpen))
#define ELK_ACTION_OPEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_OPEN, ElkActionOpenClass))
#define ELK_IS_ACTION_OPEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_OPEN))
#define ELK_IS_ACTION_OPEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_OPEN))
#define ELK_ACTION_OPEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_OPEN, ElkActionOpenClass))

typedef struct _ElkActionOpen               ElkActionOpen;
typedef struct _ElkActionOpenPrivate        ElkActionOpenPrivate;
typedef struct _ElkActionOpenClass          ElkActionOpenClass;


struct _ElkActionOpen {
	LeaAction parent;
};

struct _ElkActionOpenClass {
	LeaActionClass parent_class;
};


GType elk_action_open_get_type();

ElkActionOpen *elk_action_open_new(ElkIService *service);

G_END_DECLS

#endif /* ELKACTIONOPEN_H_ */
