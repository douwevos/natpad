/*
   File:    tursurfacewrapper.c
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

#include "tursurfacewrapper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "TurSurfaceWrapper"
#include <logging/catlog.h>

struct _TurSurfaceWrapperPrivate {
	cairo_surface_t *surface;
	cairo_t *cairo;
	short version;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TurSurfaceWrapper, tur_surface_wrapper, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TurSurfaceWrapper)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void tur_surface_wrapper_class_init(TurSurfaceWrapperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void tur_surface_wrapper_init(TurSurfaceWrapper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TurSurfaceWrapper *instance = TUR_SURFACE_WRAPPER(object);
	TurSurfaceWrapperPrivate *priv = tur_surface_wrapper_get_instance_private(instance);
	if (priv->cairo) {
		cairo_destroy(priv->cairo);
		priv->cairo = NULL;
	}
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
	}
	G_OBJECT_CLASS(tur_surface_wrapper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(tur_surface_wrapper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TurSurfaceWrapper *tur_surface_wrapper_new(cairo_surface_t *surface, short version) {
	TurSurfaceWrapper *result = g_object_new(TUR_TYPE_SURFACE_WRAPPER, NULL);
	cat_ref_anounce(result);
	TurSurfaceWrapperPrivate *priv = tur_surface_wrapper_get_instance_private(result);
	priv->surface = surface;
	priv->cairo = cairo_create(surface);
	priv->version = version;
	return result;
}

TurSurfaceWrapper *tur_surface_wrapper_new_cairo(cairo_t *cairo) {
	TurSurfaceWrapper *result = g_object_new(TUR_TYPE_SURFACE_WRAPPER, NULL);
	cat_ref_anounce(result);
	TurSurfaceWrapperPrivate *priv = tur_surface_wrapper_get_instance_private(result);
	priv->surface = NULL;
	priv->cairo = cairo;
	return result;
}

cairo_surface_t *tur_surface_wrapper_get(TurSurfaceWrapper *surface_wrapper) {
	if (surface_wrapper==NULL) {
		return NULL;
	}
	TurSurfaceWrapperPrivate *priv = tur_surface_wrapper_get_instance_private(surface_wrapper);
	return priv->surface;

}

short tur_surface_wrapper_get_version(TurSurfaceWrapper *surface_wrapper) {
	TurSurfaceWrapperPrivate *priv = tur_surface_wrapper_get_instance_private(surface_wrapper);
	return priv->version;
}

cairo_t *tur_surface_wrapper_get_context(TurSurfaceWrapper *surface_wrapper) {
	if (surface_wrapper==NULL) {
		return NULL;
	}
	TurSurfaceWrapperPrivate *priv = tur_surface_wrapper_get_instance_private(surface_wrapper);
	return priv->cairo;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TurSurfaceWrapper *instance = TUR_SURFACE_WRAPPER(self);
	TurSurfaceWrapperPrivate *priv = tur_surface_wrapper_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p surface=%p]", iname, self, priv->surface);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

