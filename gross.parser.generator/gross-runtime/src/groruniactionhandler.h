/*
   File:    groruniactionhandler.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef GRORUNIACTIONHANDLER_H_
#define GRORUNIACTIONHANDLER_H_

#include "grorunfork.h"
#include "grorunsymbol.h"
#include "grorunreducelayout.h"
#include <caterpillar.h>

#define GRORUN_TYPE_IACTION_HANDLER                 (grorun_iaction_handler_get_type())
#define GRORUN_IACTION_HANDLER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRORUN_TYPE_IACTION_HANDLER, GroRunIActionHandler))
#define GRORUN_IS_IACTION_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRORUN_TYPE_IACTION_HANDLER))
#define GRORUN_IACTION_HANDLER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRORUN_TYPE_IACTION_HANDLER, GroRunIActionHandlerInterface))


typedef struct _GroRunIActionHandler               GroRunIActionHandler; /* dummy object */
typedef struct _GroRunIActionHandlerInterface      GroRunIActionHandlerInterface;


struct _GroRunIActionHandlerInterface {
	GTypeInterface parent_iface;
	GObject *(*runAction)(GroRunIActionHandler *self, GroRunFork *fork, GroRunReduceLayout *reduce_layout, int action_id);
};

GType grorun_iaction_handler_get_type(void);

GObject *grorun_iaction_handler_run_action(GroRunIActionHandler *self, GroRunFork *fork, GroRunReduceLayout *reduce_layout, int action_id);

#endif /* GRORUNIACTIONHANDLER_H_ */
