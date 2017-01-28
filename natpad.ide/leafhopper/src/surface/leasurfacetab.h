/*
   File:    leasurfacetab.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 19, 2014
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

#ifndef LEASURFACETAB_H_
#define LEASURFACETAB_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_SURFACE_TAB              (lea_surface_tab_get_type())
#define LEA_SURFACE_TAB(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_surface_tab_get_type(), LeaSurfaceTab))
#define LEA_SURFACE_TAB_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SURFACE_TAB, LeaSurfaceTabClass))
#define LEA_IS_SURFACE_TAB(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SURFACE_TAB))
#define LEA_IS_SURFACE_TAB_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SURFACE_TAB))
#define LEA_SURFACE_TAB_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SURFACE_TAB, LeaSurfaceTabClass))


typedef struct _LeaSurfaceTab               LeaSurfaceTab;
typedef struct _LeaSurfaceTabPrivate        LeaSurfaceTabPrivate;
typedef struct _LeaSurfaceTabClass          LeaSurfaceTabClass;


struct _LeaSurfaceTab {
	GObject parent;
};

struct _LeaSurfaceTabClass {
	GObjectClass parent_class;
};


GType lea_surface_tab_get_type();

LeaSurfaceTab *lea_surface_tab_new(GtkWidget *widget, CatStringWo *a_label);

GtkWidget *lea_surface_tab_get_widget(LeaSurfaceTab *tab);

void lea_surface_tab_unset_widget(LeaSurfaceTab *tab);

const GtkAllocation lea_surface_tab_do_layout(LeaSurfaceTab *tab, int tab_height, int x_offset);

void lea_surface_tab_get_rectangle(LeaSurfaceTab *tab, GdkRectangle *rectangle);

gboolean lea_surface_tab_get_hover_info(LeaSurfaceTab *tab, int wmx, int wmy, gboolean *is_on_close);

GdkRectangle lea_surface_tab_draw(LeaSurfaceTab *tab, cairo_t *cr, GdkRectangle *view_rect, int x_view, gboolean is_hovering, gboolean is_on_close);

void lea_surface_tab_set_last_focus_child(LeaSurfaceTab *tab, GtkWidget *focus_child);
GtkWidget *lea_surface_tab_get_last_focus_child_ref(LeaSurfaceTab *tab);


G_END_DECLS

#endif /* LEASURFACETAB_H_ */
