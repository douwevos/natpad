/*
   File:    mooipropertieshandler.c
   Project: moose
   Author:  Douwe Vos
   Date:    Mar 24, 2014
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

#include "mooipropertieshandler.h"

G_DEFINE_INTERFACE(MooIPropertiesHandler, moo_iproperties_handler, G_TYPE_OBJECT);

static void moo_iproperties_handler_default_init (MooIPropertiesHandlerInterface *klass) {
}

void moo_iproperties_handler_commit(MooIPropertiesHandler *self, CatIIterator *iter, struct _MooNodeWo *from_node, struct _MooNodeWo *to_node) {
	MOO_IPROPERTIES_HANDLER_GET_INTERFACE(self)->commit(self, iter, from_node, to_node);
}


//long long moo_iproperties_handler_next(MooIPropertiesHandler *self) {
//	g_return_val_if_fail(MOO_IS_INODE_MATCHER(self), 0);
//	return MOO_INODE_MATCHER_GET_INTERFACE(self)->next(self);
//}
