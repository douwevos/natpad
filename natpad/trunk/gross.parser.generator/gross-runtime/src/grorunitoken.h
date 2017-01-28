/*
   File:    grorunitoken.h
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

#ifndef GRORUNITOKEN_H_
#define GRORUNITOKEN_H_

#include "grorunsymbol.h"
#include "grorunlocation.h"
#include <caterpillar.h>

#define GRORUN_TYPE_ITOKEN                 (grorun_itoken_get_type())
#define GRORUN_ITOKEN(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRORUN_TYPE_ITOKEN, GroRunIToken))
#define GRORUN_IS_ITOKEN(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRORUN_TYPE_ITOKEN))
#define GRORUN_ITOKEN_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRORUN_TYPE_ITOKEN, GroRunITokenInterface))

typedef struct _GroRunLeftRight            GroRunLeftRight;
struct _GroRunLeftRight {
	int left, right;
};

typedef struct _GroRunIToken               GroRunIToken; /* dummy object */
typedef struct _GroRunITokenInterface      GroRunITokenInterface;


struct _GroRunITokenInterface {
	GTypeInterface parent_iface;
	GroRunSymbol *(*getSymbol)(GroRunIToken *self);
	GObject *(*getValue)(GroRunIToken *self);
	GroRunLeftRight (*getIndexes)(GroRunIToken *self);
	GroRunLocation *(*getLocation)(GroRunIToken *self);
};

GType grorun_itoken_get_type(void);

GroRunSymbol *grorun_itoken_get_symbol(GroRunIToken *self);
GObject *grorun_itoken_get_value(GroRunIToken *self);
GroRunLeftRight grorun_itoken_get_indexes(GroRunIToken *self);
GroRunLocation *grorun_itoken_get_location(GroRunIToken *self);

#endif /* GRORUNITOKEN_H_ */
