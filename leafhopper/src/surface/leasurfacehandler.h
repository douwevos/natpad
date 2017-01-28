/*
   File:    leasurfacehandler.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 17, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef LEASURFACEHANDLER_H_
#define LEASURFACEHANDLER_H_

#include "leasurface.h"
#include "leasurfacebox.h"
#include "drag/leaigrip.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_SURFACE_HANDLER              (lea_surface_handler_get_type())
#define LEA_SURFACE_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_surface_handler_get_type(), LeaSurfaceHandler))
#define LEA_SURFACE_HANDLER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SURFACE_HANDLER, LeaSurfaceHandlerClass))
#define LEA_IS_SURFACE_HANDLER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SURFACE_HANDLER))
#define LEA_IS_SURFACE_HANDLER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SURFACE_HANDLER))
#define LEA_SURFACE_HANDLER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SURFACE_HANDLER, LeaSurfaceHandlerClass))


typedef struct _LeaSurfaceHandler               LeaSurfaceHandler;
typedef struct _LeaSurfaceHandlerPrivate        LeaSurfaceHandlerPrivate;
typedef struct _LeaSurfaceHandlerClass          LeaSurfaceHandlerClass;


struct _LeaSurfaceHandler {
	GObject parent;
};

struct _LeaSurfaceHandlerClass {
	GObjectClass parent_class;
};

typedef int (*LeaMatchFunc) (const LeaIRegion *region, gconstpointer data);


GType lea_surface_handler_get_type();

LeaSurfaceHandler *lea_surface_handler_new();

LeaSurfaceBox *lea_surface_handler_get_root(LeaSurfaceHandler *surface_handler);

void lea_surface_handler_cleanup(LeaSurfaceHandler *surface_handler);

LeaIRegion *lea_surface_handler_find_parent_region(LeaSurfaceHandler *surface_handler, LeaIRegion *region);

LeaSurface *lea_surface_handler_find_matching_surface(LeaSurfaceHandler *surface_handler, LeaMatchFunc match_func, gpointer match_data);

LeaSurface *lea_surface_handler_create_surface(LeaSurfaceHandler *surface_handler);

void lea_surface_handler_allocate(LeaSurfaceHandler *surface_handler, GtkAllocation *allocation);

void lea_surface_handler_reallocate(LeaSurfaceHandler *surface_handler);


void lea_surface_handler_dump(LeaSurfaceHandler *surface_handler);


G_END_DECLS

#endif /* LEASURFACEHANDLER_H_ */
