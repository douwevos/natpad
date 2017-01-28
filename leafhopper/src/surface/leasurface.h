/*
   File:    leasurface.h
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

#ifndef LEASURFACE_H_
#define LEASURFACE_H_

#include "leasurfacetabmodel.h"
#include "leasurfacetabbar.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_SURFACE              (lea_surface_get_type())
#define LEA_SURFACE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_surface_get_type(), LeaSurface))
#define LEA_SURFACE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SURFACE, LeaSurfaceClass))
#define LEA_IS_SURFACE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SURFACE))
#define LEA_IS_SURFACE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SURFACE))
#define LEA_SURFACE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SURFACE, LeaSurfaceClass))


typedef struct _LeaSurface               LeaSurface;
typedef struct _LeaSurfacePrivate        LeaSurfacePrivate;
typedef struct _LeaSurfaceClass          LeaSurfaceClass;

struct _LeaFrame;

struct _LeaSurface {
	GtkContainer parent;
};

struct _LeaSurfaceClass {
	GtkContainerClass parent_class;
};


GType lea_surface_get_type();

LeaSurface *lea_surface_new();

LeaSurfaceTabModel *lea_surface_get_tab_model(LeaSurface *surface);

LeaSurfaceTabBar *lea_surface_get_tab_bar(LeaSurface *surface);

gboolean lea_surface_has_tabs(LeaSurface *surface);

void lea_surface_set_selected_tab(LeaSurface *surface, GtkWidget *widget_on_tab);

void lea_surface_set_hold(LeaSurface *surface, gboolean hold);
gboolean lea_surface_get_hold(LeaSurface *surface);


struct _LeaFrame *lea_surface_get_frame(LeaSurface *surface);

G_END_DECLS

#endif /* LEASURFACE_H_ */
