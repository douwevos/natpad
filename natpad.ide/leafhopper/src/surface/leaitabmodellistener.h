/*
   File:    leaitabmodellistener.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 19, 2014
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

#ifndef LEAITABMODELLISTENER_H_
#define LEAITABMODELLISTENER_H_

#include "leasurfacetab.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_ITAB_MODEL_LISTENER                 (lea_itab_model_listener_get_type())
#define LEA_ITAB_MODEL_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_ITAB_MODEL_LISTENER, LeaITabModelListener))
#define LEA_IS_ITAB_MODEL_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_ITAB_MODEL_LISTENER))
#define LEA_ITAB_MODEL_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_ITAB_MODEL_LISTENER, LeaITabModelListenerInterface))

typedef struct _LeaITabModelListener               LeaITabModelListener; /* dummy object */
typedef struct _LeaITabModelListenerInterface      LeaITabModelListenerInterface;

struct _MooNodeWo;

struct _LeaITabModelListenerInterface {
	GTypeInterface parent_iface;

	void (*tabAdded)(LeaITabModelListener *self, LeaSurfaceTab *tab);
	void (*tabRemoved)(LeaITabModelListener *self, LeaSurfaceTab *tab);
	void (*listChanged)(LeaITabModelListener *self);
	void (*tabSelected)(LeaITabModelListener *self, int index, LeaSurfaceTab *tab);
};

GType lea_itab_model_listener_get_type(void);

void lea_itab_model_listener_tab_added(LeaITabModelListener *self, LeaSurfaceTab *tab);
void lea_itab_model_listener_tab_removed(LeaITabModelListener *self, LeaSurfaceTab *tab);
void lea_itab_model_listener_list_changed(LeaITabModelListener *self);
void lea_itab_model_listener_tab_selected(LeaITabModelListener *self, int index, LeaSurfaceTab *tab);

G_END_DECLS

#endif /* LEAITABMODELLISTENER_H_ */
