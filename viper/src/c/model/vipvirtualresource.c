/*
   File:    vipvirtualresource.c
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

#include "vipvirtualresource.h"
#include "vipiresource.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipVirtualResource"
#include <logging/catlog.h>

struct _VipVirtualResourcePrivate {
	CatStringWo *name;
};

static void l_resource_iface_init(VipIResourceInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipVirtualResource, vip_virtual_resource, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(VipVirtualResource)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IRESOURCE, l_resource_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_virtual_resource_class_init(VipVirtualResourceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_virtual_resource_init(VipVirtualResource *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipVirtualResource *instance = VIP_VIRTUAL_RESOURCE(object);
	VipVirtualResourcePrivate *priv = vip_virtual_resource_get_instance_private(instance);
	cat_unref_ptr(priv->name);
	G_OBJECT_CLASS(vip_virtual_resource_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_virtual_resource_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

VipVirtualResource *vip_virtual_resource_new(CatStringWo *name) {
	VipVirtualResource *result = g_object_new(VIP_TYPE_VIRTUAL_RESOURCE, NULL);
	cat_ref_anounce(result);
	VipVirtualResourcePrivate *priv = vip_virtual_resource_get_instance_private(result);
	priv->name = cat_ref_ptr(name);
	return result;
}


/********************* start VipIResource implementation *********************/

static CatStringWo *l_resource_get_name(VipIResource *self) {
	VipVirtualResource *instance = VIP_VIRTUAL_RESOURCE(self);
	VipVirtualResourcePrivate *priv = vip_virtual_resource_get_instance_private(instance);
	return priv->name;
}

static long long l_resource_last_modified(VipIResource *self) {
	return 0;
}

static void l_resource_iface_init(VipIResourceInterface *iface) {
	iface->getName = l_resource_get_name;
	iface->lastModified = l_resource_last_modified;
}

/********************* end VipIResource implementation *********************/
