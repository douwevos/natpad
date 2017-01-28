/*
   File:    mooilayoutmodellistener.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Dec 25, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef MOOILAYOUTMODELLISTENER_H_
#define MOOILAYOUTMODELLISTENER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_ILAYOUT_MODEL_LISTENER                 (moo_ilayout_model_listener_get_type())
#define MOO_ILAYOUT_MODEL_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_ILAYOUT_MODEL_LISTENER, MooILayoutModelListener))
#define MOO_IS_ILAYOUT_MODEL_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_ILAYOUT_MODEL_LISTENER))
#define MOO_ILAYOUT_MODEL_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_ILAYOUT_MODEL_LISTENER, MooILayoutModelListenerInterface))


typedef struct _MooILayoutModelListener               MooILayoutModelListener; /* dummy object */
typedef struct _MooILayoutModelListenerInterface      MooILayoutModelListenerInterface;


struct _MooILayoutModelListenerInterface {
	GTypeInterface parent_iface;
	void (*selectionSet)(MooILayoutModelListener *self, CatArrayWo *new_selection);
};

GType moo_ilayout_model_listener_get_type(void);

G_END_DECLS

#endif /* MOOILAYOUTMODELLISTENER_H_ */
