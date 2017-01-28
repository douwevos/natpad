/*
   File:    elkipreferenceslistener.h
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 23, 2014
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

#ifndef ELKIPREFERENCESLISTENER_H_
#define ELKIPREFERENCESLISTENER_H_

#include <caterpillar.h>
#include "elkpreferenceswo.h"

#define ELK_TYPE_IPREFERENCES_LISTENER                 (elk_ipreferences_listener_get_type())
#define ELK_IPREFERENCES_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), ELK_TYPE_IPREFERENCES_LISTENER, ElkIPreferencesListener))
#define ELK_IS_IPREFERENCES_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), ELK_TYPE_IPREFERENCES_LISTENER))
#define ELK_IPREFERENCES_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), ELK_TYPE_IPREFERENCES_LISTENER, ElkIPreferencesListenerInterface))


typedef struct _ElkIPreferencesListener               ElkIPreferencesListener; /* dummy object */
typedef struct _ElkIPreferencesListenerInterface      ElkIPreferencesListenerInterface;

struct _ElkIPreferencesListenerInterface {
	GTypeInterface parent_iface;
	void(*preferencesChanged)(ElkIPreferencesListener *self, ElkPreferencesWo *new_prefs);
};

GType elk_ipreferences_listener_get_type(void);

void elk_ipreferences_listener_preferences_changed(ElkIPreferencesListener *self, ElkPreferencesWo *new_prefs);


#endif /* ELKIPREFERENCESLISTENER_H_ */
