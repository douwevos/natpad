/*
   File:    leaieventlistener.c
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

#include "leaieventlistener.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaIEventListener"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(LeaIEventListener, lea_ievent_listener, G_TYPE_OBJECT);

static void lea_ievent_listener_default_init (LeaIEventListenerInterface *iface) {
	iface->event = NULL;
}

void lea_ievent_listener_event(LeaIEventListener *self, LeaEvent *event) {
	g_return_if_fail(LEA_IS_IEVENT_LISTENER(self));
	LEA_IEVENT_LISTENER_GET_INTERFACE(self)->event(self, event);
}

