/*
   File:    jagilinklistener.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#include "jagilinklistener.h"

G_DEFINE_INTERFACE(JagILinkListener, jag_ilink_listener, G_TYPE_OBJECT);

static void jag_ilink_listener_default_init (JagILinkListenerInterface *klass) {
}

void jag_ilink_listener_notify(JagILinkListener *self, struct _JagLink *link, GObject *notify_data) {
	g_return_if_fail(JAG_IS_ILINK_LISTENER(self));
	JAG_ILINK_LISTENER_GET_INTERFACE(self)->notify(self, link, notify_data);
}
