/*
   File:    elkipreferenceslistener.c
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

#include "elkipreferenceslistener.h"

G_DEFINE_INTERFACE(ElkIPreferencesListener, elk_ipreferences_listener, G_TYPE_OBJECT);

static void elk_ipreferences_listener_default_init (ElkIPreferencesListenerInterface *iface) {
	iface->preferencesChanged = NULL;
}

void elk_ipreferences_listener_preferences_changed(ElkIPreferencesListener *self, ElkPreferencesWo *new_prefs) {
	g_return_if_fail(ELK_IS_IPREFERENCES_LISTENER(self));
	ELK_IPREFERENCES_LISTENER_GET_INTERFACE(self)->preferencesChanged(self, new_prefs);
}
