/*
   File:    grorunitokenfactory.h
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

#ifndef GRORUNITOKENFACTORY_H_
#define GRORUNITOKENFACTORY_H_

#include "grorunitoken.h"
#include "grorunlocation.h"
#include <caterpillar.h>

#define GRORUN_TYPE_ITOKEN_FACTORY                 (grorun_itoken_factory_get_type())
#define GRORUN_ITOKEN_FACTORY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRORUN_TYPE_ITOKEN_FACTORY, GroRunITokenFactory))
#define GRORUN_IS_ITOKEN_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRORUN_TYPE_ITOKEN_FACTORY))
#define GRORUN_ITOKEN_FACTORY_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRORUN_TYPE_ITOKEN_FACTORY, GroRunITokenFactoryInterface))


typedef struct _GroRunITokenFactory               GroRunITokenFactory; /* dummy object */
typedef struct _GroRunITokenFactoryInterface      GroRunITokenFactoryInterface;


struct _GroRunITokenFactoryInterface {
	GTypeInterface parent_iface;
	GroRunIToken *(*createToken)(GroRunITokenFactory *self, GroRunLeftRight indexes, GroRunSymbol *symbol, GObject *value, GroRunLocation *location, CatStringWo *symbol_description);
};

GType grorun_itoken_factory_get_type(void);

GroRunIToken *grorun_itoken_factory_create_token(GroRunITokenFactory *self, GroRunLeftRight indexes, GroRunSymbol *symbol, GObject *value, GroRunLocation *location, CatStringWo *symbol_description);

#endif /* GRORUNITOKENFACTORY_H_ */
