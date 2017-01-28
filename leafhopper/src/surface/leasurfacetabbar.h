/*
   File:    leasurfacetabbar.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 18, 2014
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

#ifndef LEASURFACETABBAR_H_
#define LEASURFACETABBAR_H_

#include "leasurfacetabmodel.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_SURFACE_TAB_BAR              (lea_surface_tab_bar_get_type())
#define LEA_SURFACE_TAB_BAR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_surface_tab_bar_get_type(), LeaSurfaceTabBar))
#define LEA_SURFACE_TAB_BAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SURFACE_TAB_BAR, LeaSurfaceTabBarClass))
#define LEA_IS_SURFACE_TAB_BAR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SURFACE_TAB_BAR))
#define LEA_IS_SURFACE_TAB_BAR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SURFACE_TAB_BAR))
#define LEA_SURFACE_TAB_BAR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SURFACE_TAB_BAR, LeaSurfaceTabBarClass))


typedef struct _LeaSurfaceTabBar               LeaSurfaceTabBar;
typedef struct _LeaSurfaceTabBarPrivate        LeaSurfaceTabBarPrivate;
typedef struct _LeaSurfaceTabBarClass          LeaSurfaceTabBarClass;


struct _LeaSurfaceTabBar {
	GtkWidget parent;
};

struct _LeaSurfaceTabBarClass {
	GtkWidgetClass parent_class;
};


GType lea_surface_tab_bar_get_type();

LeaSurfaceTabBar *lea_surface_tab_bar_new(LeaSurfaceTabModel *tab_model);

gboolean lea_surface_tab_bar_get_tab_view_rect(LeaSurfaceTabBar *tab_bar, int panel_idx, GdkRectangle *field_rect);

int lea_surface_tab_bar_get_panel_index_at(LeaSurfaceTabBar *tab_bar, int xpos, int ypos);

void lea_surface_tab_bar_invalidate_layout(LeaSurfaceTabBar *tab_bar);

G_END_DECLS

#endif /* LEASURFACETABBAR_H_ */
