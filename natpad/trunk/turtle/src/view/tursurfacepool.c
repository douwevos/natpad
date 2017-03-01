/*
   File:    tursurfacepool.c
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 14, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "tursurfacepool.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "TurSurfacePool"
#include <logging/catlog.h>

struct _TurSurfacePoolPrivate {
	CatArrayWo *pool;
	int version;
	int width, height;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TurSurfacePool, tur_surface_pool, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TurSurfacePool)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void tur_surface_pool_class_init(TurSurfacePoolClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void tur_surface_pool_init(TurSurfacePool *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	TurSurfacePool *instance = TUR_SURFACE_POOL(object);
//	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(instance);
	G_OBJECT_CLASS(tur_surface_pool_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(tur_surface_pool_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TurSurfacePool *tur_surface_pool_new() {
	TurSurfacePool *result = g_object_new(TUR_TYPE_SURFACE_POOL, NULL);
	cat_ref_anounce(result);
	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(result);
	priv->version = 0;
	priv->pool = cat_array_wo_new();
	return result;
}

void tur_surface_pool_set_image_dimension(TurSurfacePool *pool, int width, int height) {
	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(pool);
	if (priv->width!=width || priv->height!=height) {
		priv->width = width;
		priv->height = height;
		cat_array_wo_clear(priv->pool);
		priv->version++;
	}
}

void tur_surface_pool_set_image_height(TurSurfacePool *pool, int height) {
	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(pool);
	if (priv->height!=height) {
		priv->height = height;
		cat_array_wo_clear(priv->pool);
		priv->version++;
	}
}

void tur_surface_pool_set_image_width(TurSurfacePool *pool, int width) {
	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(pool);
	if (priv->width!=width) {
		priv->width = width;
		cat_array_wo_clear(priv->pool);
		priv->version++;
	}
}

TurSurfaceWrapper *tur_surface_pool_obtain(TurSurfacePool *pool, GdkWindow *window) {
	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(pool);
	TurSurfaceWrapper *result = NULL;
	if (cat_array_wo_size(priv->pool)>0) {
		cat_array_wo_remove_last(priv->pool, (GObject **) &result);
	} else {
		cairo_surface_t *cairo_surface = NULL;
		if (window==NULL) {
			cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, priv->width, priv->height);
		} else {
			cairo_surface = gdk_window_create_similar_surface(window, CAIRO_CONTENT_COLOR, priv->width, priv->height);
		}
		result = tur_surface_wrapper_new(cairo_surface, priv->version);
		cat_log_debug("new surface created");
	}
	return result;
}

void tur_surface_pool_release(TurSurfacePool *pool, TurSurfaceWrapper *surface) {
	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(pool);
	if (tur_surface_wrapper_get_version(surface)==priv->version) {
		cat_array_wo_append(priv->pool, (GObject *) surface);
	}
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TurSurfacePool *instance = TUR_SURFACE_POOL(self);
	TurSurfacePoolPrivate *priv = tur_surface_pool_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
