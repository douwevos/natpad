/*
   File:    cowichangelistener.c
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

#include "cowichangelistener.h"

G_DEFINE_INTERFACE(CowIChangeListener, cow_ichange_listener, G_TYPE_OBJECT);

static void cow_ichange_listener_default_init (CowIChangeListenerInterface *iface) {
	iface->configChanged = NULL;
}

void cow_ichange_listener_config_changed(CowIChangeListener *self, GObject *new_config) {
	g_return_if_fail(COW_IS_ICHANGE_LISTENER(self));
	COW_ICHANGE_LISTENER_GET_INTERFACE(self)->configChanged(self, new_config);
}
