/*
   File:    leaipanellistener.h
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 26, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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


#ifndef LEAIPANELLISTENER_H_
#define LEAIPANELLISTENER_H_

#include <caterpillar.h>
#include <glib-object.h>


#define LEA_TYPE_IPANEL_LISTENER                 (lea_ipanel_listener_get_type())
#define LEA_IPANEL_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IPANEL_LISTENER, LeaIPanelListener))
#define LEA_IS_IPANEL_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IPANEL_LISTENER))
#define LEA_IPANEL_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IPANEL_LISTENER, LeaIPanelListenerInterface))


typedef struct _LeaIPanelListener               LeaIPanelListener; /* dummy object */
typedef struct _LeaIPanelListenerInterface      LeaIPanelListenerInterface;

struct _LeaIPanelListenerInterface {
	GTypeInterface parent_iface;

	void (*panel_label_set)(LeaIPanelListener *self, gpointer panel, const CatStringWo *a_new_label);
};

GType lea_ipanel_listener_get_type(void);

void lea_ipanel_label_set(LeaIPanelListener *self, gpointer panel, const CatStringWo *a_new_label);


#endif /* LEAIPANELLISTENER_H_ */
