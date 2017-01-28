/*
   File:    vipmapperregistry.h
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


#ifndef VIPMAPPERREGISTRY_H_
#define VIPMAPPERREGISTRY_H_

#include "vipimapper.h"
#include "vipiscanwork.h"
#include "../vipnode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_MAPPER_REGISTRY              (vip_mapper_registry_get_type())
#define VIP_MAPPER_REGISTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_mapper_registry_get_type(), VipMapperRegistry))
#define VIP_MAPPER_REGISTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_MAPPER_REGISTRY, VipMapperRegistryClass))
#define VIP_IS_MAPPER_REGISTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_MAPPER_REGISTRY))
#define VIP_IS_MAPPER_REGISTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_MAPPER_REGISTRY))
#define VIP_MAPPER_REGISTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_MAPPER_REGISTRY, VipMapperRegistryClass))
#define VIP_MAPPER_REGISTRY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_MAPPER_REGISTRY, VipMapperRegistryPrivate))


typedef struct _VipMapperRegistry               VipMapperRegistry;
typedef struct _VipMapperRegistryPrivate        VipMapperRegistryPrivate;
typedef struct _VipMapperRegistryClass          VipMapperRegistryClass;


struct _VipMapperRegistry {
	GObject parent;
	VipMapperRegistryPrivate *priv;
};

struct _VipMapperRegistryClass {
	GObjectClass parent_class;
};


GType vip_mapper_registry_get_type();

VipMapperRegistry *vip_mapper_registry_new(VipIMapper *default_mapper);

void vip_mapper_registry_add_mapper(VipMapperRegistry *registry, VipIMapper *mapper);

VipIScanWork *vip_mapper_registry_create_work_for_node(VipMapperRegistry *registry, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent);


G_END_DECLS



#endif /* VIPMAPPERREGISTRY_H_ */
