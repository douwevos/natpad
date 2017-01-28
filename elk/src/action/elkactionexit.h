/*
   File:    elkactionexit.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 17, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef ELKACTIONEXIT_H_
#define ELKACTIONEXIT_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include "../elkiservice.h"

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_EXIT            (elk_action_exit_get_type())
#define ELK_ACTION_EXIT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_exit_get_type(), ElkActionExit))
#define ELK_ACTION_EXIT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_EXIT, ElkActionExitClass))
#define ELK_IS_ACTION_EXIT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_EXIT))
#define ELK_IS_ACTION_EXIT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_EXIT))
#define ELK_ACTION_EXIT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_EXIT, ElkActionExitClass))


typedef struct _ElkActionExit	   		ElkActionExit;
typedef struct _ElkActionExitClass	  	ElkActionExitClass;


struct _ElkActionExit {
	LeaAction parent;
	ElkIService *service;
};

struct _ElkActionExitClass {
	LeaActionClass parent_class;
};


GType elk_action_exit_get_type();

ElkActionExit *elk_action_exit_new(ElkIService *service);

G_END_DECLS

#endif /* ELKACTIONEXIT_H_ */
