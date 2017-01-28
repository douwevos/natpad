/*
   File:    vipmapperregistry.c
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



#include "vipmapperregistry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipMapperRegistry"
#include <logging/catlog.h>

struct _VipMapperRegistryPrivate {
	CatArrayWo *mappers;
	VipIMapper *default_mapper;
};

G_DEFINE_TYPE (VipMapperRegistry, vip_mapper_registry, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_mapper_registry_class_init(VipMapperRegistryClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipMapperRegistryPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_mapper_registry_init(VipMapperRegistry *instance) {
	VipMapperRegistryPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_MAPPER_REGISTRY, VipMapperRegistryPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipMapperRegistry *instance = VIP_MAPPER_REGISTRY(object);
	VipMapperRegistryPrivate *priv = instance->priv;
	cat_unref_ptr(priv->default_mapper);
	cat_unref_ptr(priv->mappers);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipMapperRegistry *vip_mapper_registry_new(VipIMapper *default_mapper) {
	VipMapperRegistry *result = g_object_new(VIP_TYPE_MAPPER_REGISTRY, NULL);
	cat_ref_anounce(result);
	VipMapperRegistryPrivate *priv = result->priv;
	priv->default_mapper = cat_ref_ptr(default_mapper);
	priv->mappers = cat_array_wo_new();
	return result;
}



void vip_mapper_registry_add_mapper(VipMapperRegistry *registry, VipIMapper *mapper) {
	VipMapperRegistryPrivate *priv = VIP_MAPPER_REGISTRY_GET_PRIVATE(registry);
	cat_array_wo_append(priv->mappers, (GObject *) mapper);
}



VipIScanWork *vip_mapper_registry_create_work_for_node(VipMapperRegistry *registry, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent) {
	VipMapperRegistryPrivate *priv = VIP_MAPPER_REGISTRY_GET_PRIVATE(registry);
	VipIScanWork *result = NULL;
	CatIIterator *iter = cat_array_wo_iterator(priv->mappers);
	while((result==NULL) && cat_iiterator_has_next(iter)) {
		VipIMapper *mapper = (VipIMapper *) cat_iiterator_next(iter);
		result = vip_imapper_create_work_for_node(mapper, node, recursive_from_parent, validated_by_parent);
	}
	cat_unref_ptr(iter);
	if (result == NULL) {
		result = vip_imapper_create_work_for_node(priv->default_mapper, node, recursive_from_parent, validated_by_parent);
	}
	return result;
}

