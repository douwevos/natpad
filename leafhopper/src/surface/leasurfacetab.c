/*
   File:    leasurfacetab.c
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

#include "leasurfacetab.h"
#include "../layout/leapanel.h"
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaSurfaceTab"
#include <logging/catlog.h>

#define LEA_TAB_INSETS 4
#define LEA_TAB_PAD 3
#define LEA_TAB_MAX_WIDTH 250

struct _LeaSurfaceTabPrivate {
	GtkWidget *widget;
	gulong destroy_signal_id;
	const CatStringWo *a_label;
	GtkAllocation allocation;
	gboolean pango_layout_dirty;
	PangoLayout *pango_layout;
	int label_width;
	CatWeakReference *last_focus_child_ref;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_panel_listener_iface_init(LeaIPanelListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaSurfaceTab, lea_surface_tab, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaSurfaceTab)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IPANEL_LISTENER, l_panel_listener_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_surface_tab_class_init(LeaSurfaceTabClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_surface_tab_init(LeaSurfaceTab *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaSurfaceTab *instance = LEA_SURFACE_TAB(object);
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(instance);
	cat_unref_ptr(priv->a_label);
	if (priv->widget) {
		g_signal_handler_disconnect(priv->widget, priv->destroy_signal_id);
	}

	cat_unref_ptr(priv->widget);
	cat_unref_ptr(priv->last_focus_child_ref);
	cat_unref_ptr(priv->pango_layout);

	G_OBJECT_CLASS(lea_surface_tab_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_surface_tab_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_widget_destroy(GtkWidget *object, gpointer user_data) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(LEA_SURFACE_TAB(user_data));
	cat_unref_ptr(priv->widget);
}


LeaSurfaceTab *lea_surface_tab_new(GtkWidget *widget, CatStringWo *a_label) {
	LeaSurfaceTab *result = g_object_new(LEA_TYPE_SURFACE_TAB, NULL);
	cat_ref_anounce(result);
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(result);
	priv->widget = cat_ref_ptr(widget);
	priv->a_label = cat_ref_ptr(a_label);
	priv->pango_layout = NULL;
	priv->last_focus_child_ref = cat_weak_reference_new(NULL);
	priv->destroy_signal_id = g_signal_connect(priv->widget, "destroy", G_CALLBACK(l_widget_destroy), result);
	if (LEA_IS_PANEL(widget)) {
		lea_panel_add_listener(LEA_PANEL(widget), (LeaIPanelListener *) result);
	}
	return result;
}

GtkWidget *lea_surface_tab_get_widget(LeaSurfaceTab *tab) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	return priv->widget;
}

void lea_surface_tab_unset_widget(LeaSurfaceTab *tab) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	cat_unref_ptr(priv->widget);
}


void lea_surface_tab_set_last_focus_child(LeaSurfaceTab *tab, GtkWidget *focus_child) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	cat_weak_reference_set(priv->last_focus_child_ref, (GObject *) focus_child);
}

GtkWidget *lea_surface_tab_get_last_focus_child_ref(LeaSurfaceTab *tab) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	return (GtkWidget *) cat_weak_reference_get(priv->last_focus_child_ref);
}


static const CatStringWo *l_get_label(LeaSurfaceTabPrivate *priv) {
	const CatStringWo *result = NULL;
	if (LEA_IS_PANEL(priv->widget)) {
		result = lea_panel_get_label((LeaPanel *) priv->widget);
		cat_ref_swap(priv->a_label, (CatStringWo *) result);
	} else {
		result = priv->a_label;
	}
	return result;
}

static PangoLayout *l_get_pango_layout(LeaSurfaceTabPrivate *priv, int tab_height) {
	if (priv->pango_layout == NULL) {
		GtkWidget *surface_widget = gtk_widget_get_parent(priv->widget);
		priv->pango_layout = pango_layout_new(gtk_widget_get_pango_context(surface_widget));
		priv->pango_layout_dirty = TRUE;
	}
	if (priv->pango_layout_dirty) {
		const CatStringWo *label = l_get_label(priv);
		pango_layout_set_text(priv->pango_layout, cat_string_wo_getchars(label), cat_string_wo_length(label));
		priv->pango_layout_dirty = FALSE;

		PangoRectangle extents;
		pango_layout_set_width(priv->pango_layout, -1);
		pango_layout_get_extents(priv->pango_layout, NULL, &extents);
		priv->label_width = extents.width/PANGO_SCALE;
		if (priv->label_width>LEA_TAB_MAX_WIDTH) {
			priv->label_width = LEA_TAB_MAX_WIDTH;
		}

	}
	return priv->pango_layout;
}

const GtkAllocation lea_surface_tab_do_layout(LeaSurfaceTab *tab, int tab_height, int x_offset) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	l_get_pango_layout(priv, tab_height);
	priv->allocation.x = x_offset;
	priv->allocation.y = 0;
	priv->allocation.width = priv->label_width + LEA_TAB_PAD + tab_height + LEA_TAB_INSETS*2;
	priv->allocation.height = tab_height;
	return priv->allocation;
}

gboolean lea_surface_tab_get_hover_info(LeaSurfaceTab *tab, int wmx, int wmy, gboolean *is_on_close) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	if (wmx<priv->allocation.x || wmx>=priv->allocation.x+priv->allocation.width) {
		return FALSE;
	}

	*is_on_close = FALSE;

	double tab_height = priv->allocation.height-LEA_TAB_INSETS*2;
	double close_center_left = priv->allocation.x+LEA_TAB_INSETS+priv->label_width+LEA_TAB_PAD;
	double close_center_top = priv->allocation.y+LEA_TAB_INSETS;

	if ((wmx>=close_center_left && wmx<close_center_left+tab_height)
			&& (wmy>=close_center_top && wmy<close_center_top+tab_height)) {
		*is_on_close = TRUE;
	}

	return TRUE;
}


void lea_surface_tab_get_rectangle(LeaSurfaceTab *tab, GdkRectangle *rectangle) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	rectangle->x = priv->allocation.x;
	rectangle->y = priv->allocation.y;
	rectangle->width = priv->allocation.width;
	rectangle->height = priv->allocation.height;
}


static void l_cairo_rounded_rect(cairo_t *cr, double x, double y, double width, double height) {
	double aspect        = 1.0,     /* aspect ratio */
	       corner_radius = height / 3.0;   /* and corner curvature radius */


	double radius = corner_radius / aspect;
	double degrees = M_PI / 180.0;

	cairo_new_sub_path (cr);
	cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path (cr);

}

GdkRectangle lea_surface_tab_draw(LeaSurfaceTab *tab, cairo_t *cr, GdkRectangle *view_rect, int x_view, gboolean is_hovering, gboolean is_on_close) {
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(tab);
	cairo_set_source_rgb(cr, 0.1, 0.4, 0.1);

	GdkRectangle field_rect;
	field_rect.x = priv->allocation.x-x_view;
	field_rect.y = priv->allocation.y;
	field_rect.width = priv->allocation.width;
	field_rect.height = priv->allocation.height;

	if (is_hovering) {
		cairo_set_source_rgb(cr, 0.f, 0.f, .7f);
	} else {
		cairo_set_source_rgb(cr, 0.f, 0.f, 0.f);
	}

	PangoLayout *layout = l_get_pango_layout(priv, view_rect->height);


	if (priv->label_width>0) {
		cat_log_debug("label_width=%d", priv->label_width);
		PangoLayout *use_layout = layout;
		pango_layout_set_width(use_layout, priv->label_width*PANGO_SCALE);
		pango_layout_set_wrap(use_layout, PANGO_WRAP_CHAR);
		pango_layout_set_ellipsize(use_layout, PANGO_ELLIPSIZE_END);
		pango_layout_set_height(use_layout, 1);

		cairo_move_to(cr, field_rect.x+LEA_TAB_INSETS, field_rect.y+LEA_TAB_INSETS);
		pango_cairo_show_layout(cr, use_layout);
	}

	double tab_height = priv->allocation.height-LEA_TAB_INSETS*2;

	double close_center_left = field_rect.x+LEA_TAB_INSETS+priv->label_width+LEA_TAB_PAD;
	double close_center_top = field_rect.y+LEA_TAB_INSETS;

	cairo_set_line_width(cr, 2);
	if (is_on_close) {
		cairo_set_source_rgb(cr, 1.f, 0.f, 0.f);
		l_cairo_rounded_rect(cr, close_center_left, close_center_top, tab_height, tab_height);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 1.f, 1.f, 1.f);

	} else {
		cairo_set_source_rgb(cr, 0.f, 0.f, 0.f);
	}

	cairo_move_to(cr, close_center_left+tab_height*0.3, close_center_top+tab_height*0.3);
	cairo_line_to(cr, close_center_left+tab_height*0.7, close_center_top+tab_height*0.7);
	cairo_stroke(cr);

	cairo_move_to(cr, close_center_left+tab_height*0.3, close_center_top+tab_height*0.7);
	cairo_line_to(cr, close_center_left+tab_height*0.7, close_center_top+tab_height*0.3);
	cairo_stroke(cr);

	return field_rect;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaSurfaceTab *instance = LEA_SURFACE_TAB(self);
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p label=%o, alloc=%d,%d,%d,%d]", iname, self, priv->a_label, priv->allocation.x, priv->allocation.y, priv->allocation.width, priv->allocation.height);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* start LeaIPanelListener implementation *********************/

static void l_panel_label_set(LeaIPanelListener *self, gpointer panel, const CatStringWo *a_new_label) {
	LeaSurfaceTab *instance = LEA_SURFACE_TAB(self);
	LeaSurfaceTabPrivate *priv = lea_surface_tab_get_instance_private(instance);
	priv->pango_layout_dirty = TRUE;
	GtkWidget *par = gtk_widget_get_parent(priv->widget);
	if (LEA_IS_SURFACE(par)) {
		LeaSurfaceTabBar *bar = lea_surface_get_tab_bar((LeaSurface *) par);
		lea_surface_tab_bar_invalidate_layout(bar);
		gtk_widget_queue_draw(GTK_WIDGET(bar));
	}
}


static void l_panel_listener_iface_init(LeaIPanelListenerInterface *iface) {
	iface->panel_label_set = l_panel_label_set;
}

/********************* end LeaIPanelListener implementation *********************/

