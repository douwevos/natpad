/*
   File:    catweakreference.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 25, 2013
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

#include "catweakreference.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatWeakReference"
#include "../logging/catlog.h"

struct _CatWeakReferencePrivate {
	GWeakRef ref;
};

G_DEFINE_TYPE_WITH_PRIVATE(CatWeakReference, cat_weak_reference, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_weak_reference_class_init(CatWeakReferenceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_weak_reference_init(CatWeakReference *instance) {
}


static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatWeakReference *instance = CAT_WEAK_REFERENCE(object);
	CatWeakReferencePrivate *priv = cat_weak_reference_get_instance_private(instance);
	g_weak_ref_clear(&priv->ref);

	G_OBJECT_CLASS(cat_weak_reference_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_weak_reference_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatWeakReference *cat_weak_reference_new(GObject *value) {
	CatWeakReference *result = g_object_new(CAT_TYPE_WEAK_REFERENCE, NULL);
	cat_ref_anounce(result);
	CatWeakReferencePrivate *priv = cat_weak_reference_get_instance_private(result);
	g_weak_ref_init(&priv->ref, value);
	return result;
}

GObject *cat_weak_reference_get(CatWeakReference *reference) {
	CatWeakReferencePrivate *priv = cat_weak_reference_get_instance_private(reference);
	GObject *result = g_weak_ref_get(&priv->ref);
	return cat_fake_ref_ptr(result);
}

void cat_weak_reference_set(CatWeakReference *reference, GObject *new_value) {
	CatWeakReferencePrivate *priv = cat_weak_reference_get_instance_private(reference);
	g_weak_ref_set(&priv->ref, (gpointer) new_value);
}










