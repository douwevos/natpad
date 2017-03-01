/*
   File:    tursurfacewrapper.h
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

#ifndef VIEW_TURSURFACEWRAPPER_H_
#define VIEW_TURSURFACEWRAPPER_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define TUR_TYPE_SURFACE_WRAPPER              (tur_surface_wrapper_get_type())
#define TUR_SURFACE_WRAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tur_surface_wrapper_get_type(), TurSurfaceWrapper))
#define TUR_SURFACE_WRAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TUR_TYPE_SURFACE_WRAPPER, TurSurfaceWrapperClass))
#define TUR_IS_SURFACE_WRAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TUR_TYPE_SURFACE_WRAPPER))
#define TUR_IS_SURFACE_WRAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TUR_TYPE_SURFACE_WRAPPER))
#define TUR_SURFACE_WRAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TUR_TYPE_SURFACE_WRAPPER, TurSurfaceWrapperClass))


typedef struct _TurSurfaceWrapper               TurSurfaceWrapper;
typedef struct _TurSurfaceWrapperPrivate        TurSurfaceWrapperPrivate;
typedef struct _TurSurfaceWrapperClass          TurSurfaceWrapperClass;


struct _TurSurfaceWrapper {
	GObject parent;
};

struct _TurSurfaceWrapperClass {
	GObjectClass parent_class;
};


GType tur_surface_wrapper_get_type();

TurSurfaceWrapper *tur_surface_wrapper_new(cairo_surface_t *surface, short version);
TurSurfaceWrapper *tur_surface_wrapper_new_cairo(cairo_t *cairo);

cairo_surface_t *tur_surface_wrapper_get(TurSurfaceWrapper *surface_wrapper);
short tur_surface_wrapper_get_version(TurSurfaceWrapper *surface_wrapper);

cairo_t *tur_surface_wrapper_get_context(TurSurfaceWrapper *surface_wrapper);

G_END_DECLS

#endif /* VIEW_TURSURFACEWRAPPER_H_ */
