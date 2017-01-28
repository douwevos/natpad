/*
   File:    chasurfacepool.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 13, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chasurfacepool.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSurfacePool"
#include <logging/catlog.h>

struct _ChaSurfacePoolPrivate {
	CatArrayWo *pool;
	int width;
	int height;
	short version;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSurfacePool, cha_surface_pool, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaSurfacePool)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_surface_pool_class_init(ChaSurfacePoolClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_surface_pool_init(ChaSurfacePool *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaSurfacePool *instance = CHA_SURFACE_POOL(object);
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(instance);
	cat_unref_ptr(priv->pool);
	G_OBJECT_CLASS(cha_surface_pool_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_surface_pool_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaSurfacePool *cha_surface_pool_new() {
	ChaSurfacePool *result = g_object_new(CHA_TYPE_SURFACE_POOL, NULL);
	cat_ref_anounce(result);
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(result);
	priv->pool = cat_array_wo_new();
	priv->width = 1;
	priv->height = 1;
	priv->version = 0;
	return result;
}


ChaCairoSurfaceWrapper *cha_surface_pool_obtain(ChaSurfacePool *pool, GdkWindow *window) {
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(pool);
	ChaCairoSurfaceWrapper *result = NULL;
	if (cat_array_wo_size(priv->pool)>0) {
		cat_array_wo_remove_last(priv->pool, (GObject **) &result);
	} else {
		cairo_surface_t *cairo_surface = NULL;
		if (window==NULL) {
			cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, priv->width, priv->height);
		} else {
			cairo_surface = gdk_window_create_similar_surface(window, CAIRO_CONTENT_COLOR, priv->width, priv->height);
		}
		result = cha_cairo_surface_wrapper_new(cairo_surface, priv->version);
		cat_log_debug("new surface created");
	}
	return result;
}

void cha_surface_pool_release(ChaSurfacePool *pool, ChaCairoSurfaceWrapper *surface) {
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(pool);
	if (cha_cairo_surface_wrapper_get_version(surface)==priv->version) {
		cat_array_wo_append(priv->pool, (GObject *) surface);
	}
}

void cha_surface_pool_set_image_dimension(ChaSurfacePool *pool, int width, int height) {
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(pool);
	if (priv->width==width && priv->height==height) {
		return;
	}
	priv->width = width;
	priv->height = height;
	priv->version++;
	cat_array_wo_clear(priv->pool);
}

void cha_surface_pool_set_image_height(ChaSurfacePool *pool, int height) {
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(pool);
	if (priv->height==height) {
		return;
	}
	priv->height = height;
	priv->version++;
	cat_array_wo_clear(priv->pool);
}

void cha_surface_pool_set_image_width(ChaSurfacePool *pool, int width) {
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(pool);
	if (priv->width==width) {
		return;
	}
	priv->width = width;
	priv->version++;
	cat_array_wo_clear(priv->pool);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaSurfacePool *instance = CHA_SURFACE_POOL(self);
	ChaSurfacePoolPrivate *priv = cha_surface_pool_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p width=%d, height=%d]", iname, self, priv->width, priv->height);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
