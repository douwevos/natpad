/*
   File:    chacairosurfacewrapper.h
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

#ifndef CHACAIROSURFACEWRAPPER_H_
#define CHACAIROSURFACEWRAPPER_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_CAIRO_SURFACE_WRAPPER              (cha_cairo_surface_wrapper_get_type())
#define CHA_CAIRO_SURFACE_WRAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_cairo_surface_wrapper_get_type(), ChaCairoSurfaceWrapper))
#define CHA_CAIRO_SURFACE_WRAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_CAIRO_SURFACE_WRAPPER, ChaCairoSurfaceWrapperClass))
#define CHA_IS_CAIRO_SURFACE_WRAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_CAIRO_SURFACE_WRAPPER))
#define CHA_IS_CAIRO_SURFACE_WRAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_CAIRO_SURFACE_WRAPPER))
#define CHA_CAIRO_SURFACE_WRAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_CAIRO_SURFACE_WRAPPER, ChaCairoSurfaceWrapperClass))


typedef struct _ChaCairoSurfaceWrapper               ChaCairoSurfaceWrapper;
typedef struct _ChaCairoSurfaceWrapperPrivate        ChaCairoSurfaceWrapperPrivate;
typedef struct _ChaCairoSurfaceWrapperClass          ChaCairoSurfaceWrapperClass;


struct _ChaCairoSurfaceWrapper {
	GObject parent;
};

struct _ChaCairoSurfaceWrapperClass {
	GObjectClass parent_class;
};


GType cha_cairo_surface_wrapper_get_type();

ChaCairoSurfaceWrapper *cha_cairo_surface_wrapper_new(cairo_surface_t *surface, short version);
ChaCairoSurfaceWrapper *cha_cairo_surface_wrapper_new_cairo(cairo_t *cairo);

cairo_surface_t *cha_cairo_surface_wrapper_get(ChaCairoSurfaceWrapper *surface_wrapper);
short cha_cairo_surface_wrapper_get_version(ChaCairoSurfaceWrapper *surface_wrapper);

cairo_t *cha_cairo_surface_wrapper_get_context(ChaCairoSurfaceWrapper *surface_wrapper);


G_END_DECLS

#endif /* CHACAIROSURFACEWRAPPER_H_ */
