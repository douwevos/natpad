/*
   File:    catistringable.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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

#include "catistringable.h"

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatIStringable"
#include "logging/catlog.h"


G_DEFINE_INTERFACE(CatIStringable, cat_istringable, G_TYPE_OBJECT);

static void l_print(CatIStringable *self, struct _CatStringWo *append_to) {
	cat_log_error("CatIStringable->print not implemented for: %p:%s", self, g_type_name_from_instance((GTypeInstance *) self));
}


static void cat_istringable_default_init(CatIStringableInterface *iface) {
	iface->print = l_print;
}

void cat_istringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	g_return_if_fail(CAT_IS_ISTRINGABLE(self));
	CAT_ISTRINGABLE_GET_INTERFACE(self)->print(self, append_to);
}
