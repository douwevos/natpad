/*
   File:    jagnamemap.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 20, 2012
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

#include "jagnamemap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagNameMap"
#include <logging/catlog.h>

struct _JagNameMapPrivate {
	CatHashMapWo *e_map;
	CatRWLock *map_lock;
};

G_DEFINE_TYPE (JagNameMap, jag_name_map, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_name_map_class_init(JagNameMapClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagNameMapPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_name_map_init(JagNameMap *instance) {
	JagNameMapPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_NAME_MAP, JagNameMapPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagNameMap *instance = JAG_NAME_MAP(object);
	JagNameMapPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_map);
	cat_unref_ptr(priv->map_lock);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagNameMap *jag_name_map_new() {
	JagNameMap *result = g_object_new(JAG_TYPE_NAME_MAP, NULL);
	cat_ref_anounce(result);
	JagNameMapPrivate *priv = result->priv;
	priv->e_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	priv->map_lock = cat_rw_lock_new();
	return result;
}



JagNamePackage *jag_name_map_get_or_create_package_ref(JagNameMap *name_map, CatStringWo *a_package_name) {
	JagNameMapPrivate *priv = JAG_NAME_MAP_GET_PRIVATE(name_map);
	if (a_package_name==NULL) {
		a_package_name = CAT_S(cat_s_empty_string);
	}
	cat_rw_lock_write_lock(priv->map_lock);
	JagNamePackage *result = (JagNamePackage *) cat_hash_map_wo_get(priv->e_map, a_package_name);
	if (result == NULL) {
		result = jag_name_package_new(a_package_name);
		cat_hash_map_wo_put(priv->e_map, (GObject *) a_package_name, (GObject *) result);
	} else {
		cat_ref_ptr(result);
	}
	cat_rw_lock_write_unlock(priv->map_lock);
	return result;
}


CatArrayWo *jag_name_map_enlist_packages(JagNameMap *name_map) {
	JagNameMapPrivate *priv = JAG_NAME_MAP_GET_PRIVATE(name_map);
	cat_rw_lock_read_lock(priv->map_lock);
	CatArrayWo *e_result = cat_hash_map_wo_enlist_values(priv->e_map, NULL);
	cat_rw_lock_write_unlock(priv->map_lock);
	return e_result;
}
