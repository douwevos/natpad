/*
   File:    cowichangelistener.h
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 26, 2014
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

#ifndef COWICHANGELISTENER_H_
#define COWICHANGELISTENER_H_

#include <caterpillar.h>

#define COW_TYPE_ICHANGE_LISTENER                 (cow_ichange_listener_get_type())
#define COW_ICHANGE_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), COW_TYPE_ICHANGE_LISTENER, CowIChangeListener))
#define COW_IS_ICHANGE_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), COW_TYPE_ICHANGE_LISTENER))
#define COW_ICHANGE_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), COW_TYPE_ICHANGE_LISTENER, CowIChangeListenerInterface))


typedef struct _CowIChangeListener               CowIChangeListener; /* dummy object */
typedef struct _CowIChangeListenerInterface      CowIChangeListenerInterface;

struct _CowIChangeListenerInterface {
	GTypeInterface parent_iface;
	void(*configChanged)(CowIChangeListener *self, GObject *new_config);
};

GType cow_ichange_listener_get_type(void);

void cow_ichange_listener_config_changed(CowIChangeListener *self, GObject *new_config);

#endif /* COWICHANGELISTENER_H_ */
