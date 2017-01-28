/*
   File:    grorunitokenfactory.c
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

#include "grorunitokenfactory.h"

G_DEFINE_INTERFACE(GroRunITokenFactory, grorun_itoken_factory, G_TYPE_OBJECT);

static void grorun_itoken_factory_default_init (GroRunITokenFactoryInterface *iface) {
}

GroRunIToken *grorun_itoken_factory_create_token(GroRunITokenFactory *self, GroRunLeftRight indexes, GroRunSymbol *symbol, GObject *value, GroRunLocation *location, CatStringWo *symbol_description) {
	return GRORUN_ITOKEN_FACTORY_GET_INTERFACE(self)->createToken(self, indexes, symbol, value, location, symbol_description);
}
