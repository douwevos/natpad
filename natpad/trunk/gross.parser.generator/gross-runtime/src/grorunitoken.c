/*
   File:    grorunitoken.c
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

#include "grorunitoken.h"

G_DEFINE_INTERFACE(GroRunIToken, grorun_itoken, G_TYPE_OBJECT);

static void grorun_itoken_default_init (GroRunITokenInterface *iface) {
}

GroRunSymbol *grorun_itoken_get_symbol(GroRunIToken *self) {
	return GRORUN_ITOKEN_GET_INTERFACE(self)->getSymbol(self);
}

GObject *grorun_itoken_get_value(GroRunIToken *self) {
	return GRORUN_ITOKEN_GET_INTERFACE(self)->getValue(self);
}

GroRunLeftRight grorun_itoken_get_indexes(GroRunIToken *self) {
	return GRORUN_ITOKEN_GET_INTERFACE(self)->getIndexes(self);
}

GroRunLocation *grorun_itoken_get_location(GroRunIToken *self) {
	return GRORUN_ITOKEN_GET_INTERFACE(self)->getLocation(self);
}
