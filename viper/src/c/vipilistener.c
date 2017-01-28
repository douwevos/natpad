/*
   File:    vipilistener.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 11, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "vipilistener.h"

G_DEFINE_INTERFACE(VipIListener, vip_ilistener, G_TYPE_OBJECT);

static void vip_ilistener_default_init (VipIListenerInterface *klass) {
}


void vip_ilistener_snapshot_set(VipIListener *self, struct _VipSnapshot *snapshot) {
	g_return_if_fail(VIP_IS_ILISTENER(self));
	VIP_ILISTENER_GET_INTERFACE(self)->snapshotSet(self, snapshot);
}
