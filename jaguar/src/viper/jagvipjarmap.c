/*
   File:    jagvipjarmap.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 27, 2013
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
#include "jagvipjarmap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagVipJarMap"
#include <logging/catlog.h>

struct _JagVipJarMapPrivate {
	VipIFile *main_file;
	CatStringWo *a_name;
	CatArrayWo /* <VipIResource *> */ *a_vip_children;
};

static void l_resource_iface_init(VipIResourceInterface *iface);
static void l_map_iface_init(VipIMapInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagVipJarMap, jag_vip_jar_map, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagVipJarMap)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IRESOURCE, l_resource_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IMAP, l_map_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_vip_jar_map_class_init(JagVipJarMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_vip_jar_map_init(JagVipJarMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagVipJarMap *instance = JAG_VIP_JAR_MAP(object);
	JagVipJarMapPrivate *priv = jag_vip_jar_map_get_instance_private(instance);
	cat_unref_ptr(priv->main_file);
	cat_unref_ptr(priv->a_name);
	cat_unref_ptr(priv->a_vip_children);
	G_OBJECT_CLASS(jag_vip_jar_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_vip_jar_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagVipJarMap *jag_vip_jar_map_new(VipIFile *mainFile, CatStringWo *a_name, CatArrayWo /* <VipIResource *> */ *a_vipchildren) {
	JagVipJarMap *result = g_object_new(JAG_TYPE_VIP_JAR_MAP, NULL);
	cat_ref_anounce(result);
	JagVipJarMapPrivate *priv = jag_vip_jar_map_get_instance_private(result);
	priv->main_file = cat_ref_ptr(mainFile);
	priv->a_name = cat_ref_ptr(a_name);
	priv->a_vip_children = cat_ref_ptr(a_vipchildren);
	return result;
}

/********************* start VipIResource implementation *********************/

static CatStringWo *l_resource_get_name(VipIResource *self) {
	JagVipJarMap *instance = JAG_VIP_JAR_MAP(self);
	JagVipJarMapPrivate *priv = jag_vip_jar_map_get_instance_private(instance);
	return priv->a_name;
}

static long long l_resource_last_modified(VipIResource *self) {
//	JagVipJarMapPrivate *priv = JAG_VIP_JAR_MAP_GET_PRIVATE(self);
//	return vip_fs_get_last_modified(priv->fs);
	return 0;
}

static void l_resource_iface_init(VipIResourceInterface *iface) {
	iface->getName = l_resource_get_name;
	iface->lastModified = l_resource_last_modified;
}

/********************* end VipIResource implementation *********************/

/********************* start VipIMap implementation *********************/

static CatArrayWo *l_map_enlist(VipIMap *self) {
	cat_log_debug("enlisting:%o", self);
	JagVipJarMap *instance = JAG_VIP_JAR_MAP(self);
	JagVipJarMapPrivate *priv = jag_vip_jar_map_get_instance_private(instance);
	CatArrayWo *e_result = cat_array_wo_new();
	cat_array_wo_append_all(e_result, priv->a_vip_children);
	return e_result;
}

static void l_map_iface_init(VipIMapInterface *iface) {
	iface->enlist = l_map_enlist;
}

/********************* end VipIMap implementation *********************/
