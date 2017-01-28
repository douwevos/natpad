/*
   File:    jagnametype.c
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

#include "jagnametype.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagNameType"
#include <logging/catlog.h>

struct _JagNameTypePrivate {
	CatStringWo *a_package_name;
	CatStringWo *a_name;
	CatRWLock *lock;
	GObject *implementation;
	int implementation_version;

};

G_DEFINE_TYPE (JagNameType, jag_name_type, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_name_type_class_init(JagNameTypeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagNameTypePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_name_type_init(JagNameType *instance) {
	JagNameTypePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_NAME_TYPE, JagNameTypePrivate);
	instance->priv = priv;
}


static void l_weak_notify_implementation(gpointer data, GObject *where_the_object_was) {
	JagNameTypePrivate *priv = JAG_NAME_TYPE_GET_PRIVATE(data);
	priv->implementation = NULL;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagNameType *instance = JAG_NAME_TYPE(object);
	JagNameTypePrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_package_name);
	cat_unref_ptr(priv->a_name);
	GObject *impl = priv->implementation;
	priv->implementation = NULL;
	if (impl) {
		g_object_weak_unref(impl, (GWeakNotify) l_weak_notify_implementation, instance);
	}
	cat_unref_ptr(priv->implementation);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagNameType *jag_name_type_new(CatStringWo *a_package_name, CatStringWo *a_name) {
	JagNameType *result = g_object_new(JAG_TYPE_NAME_TYPE, NULL);
	cat_ref_anounce(result);
	JagNameTypePrivate *priv = result->priv;
	priv->a_package_name = cat_ref_ptr(a_package_name);
	priv->a_name = cat_ref_ptr(a_name);
	priv->lock = cat_rw_lock_new();
	priv->implementation = NULL;
	priv->implementation_version = 0;
	return result;
}

CatStringWo *jag_name_type_get_name(JagNameType *name_type) {
	JagNameTypePrivate *priv = JAG_NAME_TYPE_GET_PRIVATE(name_type);
	return priv->a_name;
}


void jag_name_type_set_implementation(JagNameType *name_type, GObject *clazz) {
	JagNameTypePrivate *priv = JAG_NAME_TYPE_GET_PRIVATE(name_type);
	GObject *old_implementation = NULL;
	cat_rw_lock_write_lock(priv->lock);
	old_implementation = priv->implementation;
	priv->implementation = clazz;
	if (clazz) {
		g_object_weak_ref(clazz, (GWeakNotify) l_weak_notify_implementation, name_type);
		priv->implementation_version++;
	}
	cat_rw_lock_write_unlock(priv->lock);
	if (old_implementation) {
		g_object_weak_unref(old_implementation, (GWeakNotify) l_weak_notify_implementation, name_type);
	}
}





GObject *jag_name_type_get_implementation_ref(JagNameType *name_type, int *version) {
	GObject *result = NULL;
	JagNameTypePrivate *priv = JAG_NAME_TYPE_GET_PRIVATE(name_type);
	cat_rw_lock_read_lock(priv->lock);
	result = cat_ref_ptr(priv->implementation);
	if (version) {
		*version = priv->implementation_version;
	}
	cat_rw_lock_read_unlock(priv->lock);
	return result;
}
