/*
   File:    mooipropertieshandler.h
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

#ifndef MOOIPROPERTIESHANDLER_H_
#define MOOIPROPERTIESHANDLER_H_


#include <caterpillar.h>

#define MOO_TYPE_IPROPERTIES_HANDLER                 (moo_iproperties_handler_get_type())
#define MOO_IPROPERTIES_HANDLER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_IPROPERTIES_HANDLER, MooIPropertiesHandler))
#define MOO_IS_IPROPERTIES_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_IPROPERTIES_HANDLER))
#define MOO_IPROPERTIES_HANDLER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_IPROPERTIES_HANDLER, MooIPropertiesHandlerInterface))


typedef struct _MooIPropertiesHandler               MooIPropertiesHandler; /* dummy object */
typedef struct _MooIPropertiesHandlerInterface      MooIPropertiesHandlerInterface;


struct _MooNodeWo;

struct _MooIPropertiesHandlerInterface {
	GTypeInterface parent_iface;
	void (*commit)(MooIPropertiesHandler *self, CatIIterator *iter, struct _MooNodeWo *from_node, struct _MooNodeWo *to_node);
};

GType moo_iproperties_handler_get_type(void);


void moo_iproperties_handler_commit(MooIPropertiesHandler *self, CatIIterator *iter, struct _MooNodeWo *from_node, struct _MooNodeWo *to_node);

#define MOO_IPROPERTIES_HANDLER_NEXT(iter, from_node, to_node) { \
	if (cat_iiterator_has_next(iter)) { \
		MooIPropertiesHandler *handler = cat_iiterator_next(iter); \
		cat_log_debug("next-handler:%o", handler); \
		moo_iproperties_handler_commit(handler, iter, from_node, to_node); \
	} \
}


#endif /* MOOIPROPERTIESHANDLER_H_ */
