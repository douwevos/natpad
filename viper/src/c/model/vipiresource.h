/*
   File:    vipiresource.h
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


#ifndef VIPIRESOURCE_H_
#define VIPIRESOURCE_H_

#include "../vippath.h"
#include <caterpillar.h>

#define VIP_TYPE_IRESOURCE                 (vip_iresource_get_type())
#define VIP_IRESOURCE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_IRESOURCE, VipIResource))
#define VIP_IS_IRESOURCE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_IRESOURCE))
#define VIP_IRESOURCE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_IRESOURCE, VipIResourceInterface))


typedef struct _VipIResource               VipIResource; /* dummy object */
typedef struct _VipIResourceInterface      VipIResourceInterface;


struct _VipResource;

struct _VipIResourceInterface {
	GTypeInterface parent_iface;
	CatStringWo *(*getName)(VipIResource *self);
	VipPath *(*path)(VipIResource *self);
	long long (*lastModified)(VipIResource *self);
	VipIResource *(*rename)(VipIResource *self, CatStringWo *new_name);
	gboolean (*canRename)(VipIResource *self);
	gboolean (*delete)(VipIResource *self);
	gboolean (*canDelete)(VipIResource *self);
	gboolean (*equal)(VipIResource *res_a, VipIResource *res_b);
};

GType vip_iresource_get_type(void);


CatStringWo *vip_iresource_get_name(VipIResource *self);

long long vip_iresource_last_modified(VipIResource *self);

VipPath *vip_iresource_path(VipIResource *self);

VipIResource *vip_iresource_rename(VipIResource *self, CatStringWo *new_name);

gboolean vip_iresource_can_rename(VipIResource *self);

gboolean vip_iresource_delete(VipIResource *self);

gboolean vip_iresource_can_delete(VipIResource *self);

gboolean vip_iresource_equal(VipIResource *res_a, VipIResource *res_b);


#endif /* VIPIRESOURCE_H_ */
