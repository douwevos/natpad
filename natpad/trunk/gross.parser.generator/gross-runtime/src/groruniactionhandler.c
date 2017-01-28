/*
   File:    groruniactionhandler.c
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

#include "groruniactionhandler.h"

G_DEFINE_INTERFACE(GroRunIActionHandler, grorun_iaction_handler, G_TYPE_OBJECT);

static void grorun_iaction_handler_default_init (GroRunIActionHandlerInterface *iface) {
}

GObject *grorun_iaction_handler_run_action(GroRunIActionHandler *self, GroRunFork *fork, GroRunReduceLayout *reduce_layout, int action_id) {
	return GRORUN_IACTION_HANDLER_GET_INTERFACE(self)->runAction(self, fork, reduce_layout, action_id);
}
