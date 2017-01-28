/*
   File:    jagnamepackage.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 21, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "jagnamepackage.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagNamePackage"
#include <logging/catlog.h>

struct _JagNamePackagePrivate {
	CatStringWo *a_package_name;
	CatHashMapWo *e_type_map;
	CatRWLock *type_map_lock;
};

G_DEFINE_TYPE (JagNamePackage, jag_name_package, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_name_package_class_init(JagNamePackageClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagNamePackagePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_name_package_init(JagNamePackage *instance) {
	JagNamePackagePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_NAME_PACKAGE, JagNamePackagePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagNamePackage *instance = JAG_NAME_PACKAGE(object);
	JagNamePackagePrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_package_name);
	cat_unref_ptr(priv->e_type_map);
	cat_unref_ptr(priv->type_map_lock);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagNamePackage *jag_name_package_new(CatStringWo *a_package_name) {
	JagNamePackage *result = g_object_new(JAG_TYPE_NAME_PACKAGE, NULL);
	cat_ref_anounce(result);
	JagNamePackagePrivate *priv = result->priv;
	priv->a_package_name = cat_ref_ptr(a_package_name);
	priv->e_type_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	priv->type_map_lock = cat_rw_lock_new();

	return result;
}



CatStringWo *jag_name_package_get_name(JagNamePackage *name_package) {
	return JAG_NAME_PACKAGE_GET_PRIVATE(name_package)->a_package_name;
}


JagNameType *jag_name_package_get_or_create_type_ref(JagNamePackage *name_package, CatStringWo *a_type_name) {
	JagNamePackagePrivate *priv = JAG_NAME_PACKAGE_GET_PRIVATE(name_package);

	cat_rw_lock_write_lock(priv->type_map_lock);
	JagNameType *result = (JagNameType *) cat_hash_map_wo_get(priv->e_type_map, a_type_name);
	if (result == NULL) {
		result = jag_name_type_new(jag_name_package_get_name(name_package), a_type_name);
		cat_hash_map_wo_put(priv->e_type_map, (GObject *) a_type_name, (GObject *) result);
	} else {
		cat_ref_ptr(result);
	}
	cat_rw_lock_write_unlock(priv->type_map_lock);
	return result;
}


