/*
   File:    vipilistener.h
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


#ifndef VIPILISTENER_H_
#define VIPILISTENER_H_

#include <caterpillar.h>

#define VIP_TYPE_ILISTENER                 (vip_ilistener_get_type())
#define VIP_ILISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_ILISTENER, VipIListener))
#define VIP_IS_ILISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_ILISTENER))
#define VIP_ILISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_ILISTENER, VipIListenerInterface))


typedef struct _VipIListener               VipIListener; /* dummy object */
typedef struct _VipIListenerInterface      VipIListenerInterface;

struct _VipSnapshot;

struct _VipIListenerInterface {
	GTypeInterface parent_iface;
	void (*snapshotSet)(VipIListener *self, struct _VipSnapshot *snapshot);
};

GType vip_ilistener_get_type(void);

void vip_ilistener_snapshot_set(VipIListener *self, struct _VipSnapshot *snapshot);

#endif /* VIPILISTENER_H_ */
