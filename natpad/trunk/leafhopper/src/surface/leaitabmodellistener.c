/*
   File:    leaitabmodellistener.c
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

#include "leaitabmodellistener.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaITabModelListener"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(LeaITabModelListener, lea_itab_model_listener, G_TYPE_OBJECT);

static void lea_itab_model_listener_default_init (LeaITabModelListenerInterface *iface) {
	iface->tabAdded = NULL;
	iface->tabRemoved = NULL;
	iface->listChanged = NULL;
	iface->tabSelected = NULL;
}

void lea_itab_model_listener_tab_added(LeaITabModelListener *self, LeaSurfaceTab *tab) {
	g_return_if_fail(LEA_IS_ITAB_MODEL_LISTENER(self));
	LeaITabModelListenerInterface *iface = LEA_ITAB_MODEL_LISTENER_GET_INTERFACE(self);
	if (iface->tabAdded) {
		iface->tabAdded(self, tab);
	}
}

void lea_itab_model_listener_tab_removed(LeaITabModelListener *self, LeaSurfaceTab *tab) {
	g_return_if_fail(LEA_IS_ITAB_MODEL_LISTENER(self));
	LeaITabModelListenerInterface *iface = LEA_ITAB_MODEL_LISTENER_GET_INTERFACE(self);
	if (iface->tabRemoved) {
		iface->tabRemoved(self, tab);
	}
}

void lea_itab_model_listener_list_changed(LeaITabModelListener *self) {
	g_return_if_fail(LEA_IS_ITAB_MODEL_LISTENER(self));
	LeaITabModelListenerInterface *iface = LEA_ITAB_MODEL_LISTENER_GET_INTERFACE(self);
	if (iface->listChanged) {
		iface->listChanged(self);
	}
}

void lea_itab_model_listener_tab_selected(LeaITabModelListener *self, int index, LeaSurfaceTab *tab) {
	g_return_if_fail(LEA_IS_ITAB_MODEL_LISTENER(self));
	LeaITabModelListenerInterface *iface = LEA_ITAB_MODEL_LISTENER_GET_INTERFACE(self);
	if (iface->tabSelected) {
		iface->tabSelected(self, index, tab);
	}
}
