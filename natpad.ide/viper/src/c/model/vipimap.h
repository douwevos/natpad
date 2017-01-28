/*
   File:    vipimap.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
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


#ifndef VIPIMAP_H_
#define VIPIMAP_H_

#include "vipifile.h"
#include "vipiresource.h"
#include <caterpillar.h>

#define VIP_TYPE_IMAP                 (vip_imap_get_type())
#define VIP_IMAP(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_IMAP, VipIMap))
#define VIP_IS_IMAP(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_IMAP))
#define VIP_IMAP_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_IMAP, VipIMapInterface))


typedef struct _VipIMap               VipIMap; /* dummy object */
typedef struct _VipIMapInterface      VipIMapInterface;

struct _VipIMapInterface {
	VipIResourceInterface parent_iface;
	CatArrayWo *(*enlist)(VipIMap *self);		// <VipIResource>

	VipIMap *(*createMap)(VipIMap *self, CatStringWo *new_map_name);
	VipIFile *(*createFile)(VipIMap *self, CatStringWo *new_file_name);

	gboolean (*canBeCopiedOrMovedTo)(VipIMap *self);
	void (*copyTo)(VipIMap *self, CatArrayWo /*<VipIResource *>*/ *toCopy);

};

GType vip_imap_get_type(void);


CatArrayWo *vip_imap_enlist(VipIMap *self);

VipIFile *vip_imap_create_file(VipIMap *self, CatStringWo *new_file_name);


#endif /* VIPIMAP_H_ */
