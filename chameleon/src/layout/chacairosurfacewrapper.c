/*
   File:    chacairosurfacewrapper.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 28, 2015
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


#include "chacairosurfacewrapper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaCairoSurfaceWrapper"
#include <logging/catlog.h>

struct _ChaCairoSurfaceWrapperPrivate {
	cairo_surface_t *surface;
	cairo_t *cairo;
	short version;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaCairoSurfaceWrapper, cha_cairo_surface_wrapper, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaCairoSurfaceWrapper)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_cairo_surface_wrapper_class_init(ChaCairoSurfaceWrapperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_cairo_surface_wrapper_init(ChaCairoSurfaceWrapper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaCairoSurfaceWrapper *instance = CHA_CAIRO_SURFACE_WRAPPER(object);
	ChaCairoSurfaceWrapperPrivate *priv = cha_cairo_surface_wrapper_get_instance_private(instance);
	if (priv->cairo) {
		cairo_destroy(priv->cairo);
		priv->cairo = NULL;
	}
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
	}
	G_OBJECT_CLASS(cha_cairo_surface_wrapper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_cairo_surface_wrapper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaCairoSurfaceWrapper *cha_cairo_surface_wrapper_new(cairo_surface_t *surface, short version) {
	ChaCairoSurfaceWrapper *result = g_object_new(CHA_TYPE_CAIRO_SURFACE_WRAPPER, NULL);
	cat_ref_anounce(result);
	ChaCairoSurfaceWrapperPrivate *priv = cha_cairo_surface_wrapper_get_instance_private(result);
	priv->surface = surface;
	priv->cairo = cairo_create(surface);
	priv->version = version;
	return result;
}

ChaCairoSurfaceWrapper *cha_cairo_surface_wrapper_new_cairo(cairo_t *cairo) {
	ChaCairoSurfaceWrapper *result = g_object_new(CHA_TYPE_CAIRO_SURFACE_WRAPPER, NULL);
	cat_ref_anounce(result);
	ChaCairoSurfaceWrapperPrivate *priv = cha_cairo_surface_wrapper_get_instance_private(result);
	priv->surface = NULL;
	priv->cairo = cairo;
	return result;
}

short cha_cairo_surface_wrapper_get_version(ChaCairoSurfaceWrapper *surface_wrapper) {
	ChaCairoSurfaceWrapperPrivate *priv = cha_cairo_surface_wrapper_get_instance_private(surface_wrapper);
	return priv->version;
}


cairo_surface_t *cha_cairo_surface_wrapper_get(ChaCairoSurfaceWrapper *surface_wrapper) {
	if (surface_wrapper==NULL) {
		return NULL;
	}
	ChaCairoSurfaceWrapperPrivate *priv = cha_cairo_surface_wrapper_get_instance_private(surface_wrapper);
	return priv->surface;
}

cairo_t *cha_cairo_surface_wrapper_get_context(ChaCairoSurfaceWrapper *surface_wrapper) {
	if (surface_wrapper==NULL) {
		return NULL;
	}
	ChaCairoSurfaceWrapperPrivate *priv = cha_cairo_surface_wrapper_get_instance_private(surface_wrapper);
	return priv->cairo;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaCairoSurfaceWrapper *instance = CHA_CAIRO_SURFACE_WRAPPER(self);
	ChaCairoSurfaceWrapperPrivate *priv = cha_cairo_surface_wrapper_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p surface=%p]", iname, self, priv->surface);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
