/*
   File:    leaieventlistener.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 25, 2014
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

#ifndef LEAIEVENTLISTENER_H_
#define LEAIEVENTLISTENER_H_

#include "leaevent.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_IEVENT_LISTENER                 (lea_ievent_listener_get_type())
#define LEA_IEVENT_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IEVENT_LISTENER, LeaIEventListener))
#define LEA_IS_IEVENT_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IEVENT_LISTENER))
#define LEA_IEVENT_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IEVENT_LISTENER, LeaIEventListenerInterface))

typedef struct _LeaIEventListener               LeaIEventListener; /* dummy object */
typedef struct _LeaIEventListenerInterface      LeaIEventListenerInterface;

struct _MooNodeWo;

struct _LeaIEventListenerInterface {
	GTypeInterface parent_iface;

	void (*event)(LeaIEventListener *self, LeaEvent *event);
};

GType lea_ievent_listener_get_type(void);

void lea_ievent_listener_event(LeaIEventListener *self, LeaEvent *event);

G_END_DECLS



#endif /* LEAIEVENTLISTENER_H_ */
