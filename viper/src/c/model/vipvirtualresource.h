/*
   File:    vipvirtualresource.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 20, 2013
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

#ifndef VIPVIRTUALRESOURCE_H_
#define VIPVIRTUALRESOURCE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_VIRTUAL_RESOURCE              (vip_virtual_resource_get_type())
#define VIP_VIRTUAL_RESOURCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_virtual_resource_get_type(), VipVirtualResource))
#define VIP_VIRTUAL_RESOURCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_VIRTUAL_RESOURCE, VipVirtualResourceClass))
#define VIP_IS_VIRTUAL_RESOURCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_VIRTUAL_RESOURCE))
#define VIP_IS_VIRTUAL_RESOURCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_VIRTUAL_RESOURCE))
#define VIP_VIRTUAL_RESOURCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_VIRTUAL_RESOURCE, VipVirtualResourceClass))
#define VIP_VIRTUAL_RESOURCE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_VIRTUAL_RESOURCE, VipVirtualResourcePrivate))


typedef struct _VipVirtualResource               VipVirtualResource;
typedef struct _VipVirtualResourcePrivate        VipVirtualResourcePrivate;
typedef struct _VipVirtualResourceClass          VipVirtualResourceClass;


struct _VipVirtualResource {
	GObject parent;
	VipVirtualResourcePrivate *priv;
};

struct _VipVirtualResourceClass {
	GObjectClass parent_class;
};


GType vip_virtual_resource_get_type();

VipVirtualResource *vip_virtual_resource_new(CatStringWo *name);

G_END_DECLS

#endif /* VIPVIRTUALRESOURCE_H_ */
