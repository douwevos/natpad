/*
   File:    leasurface.c
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

#include "leasurface.h"
#include "leasurfacetabbar.h"
#include "leairegion.h"
#include "leaitabmodellistener.h"
#include "../layout/leapanel.h"
#include "../leaframeprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaSurface"
#include <logging/catlog.h>

struct _LeaSurfacePrivate {
	GtkAllocation child_allocation;
	GtkAllocation tab_allocation;
	LeaSurfaceTabModel *tab_model;
	LeaSurfaceTabBar *tab_bar;
	gboolean hold;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_region_iface_init(LeaIRegionInterface *iface);
static void l_tab_model_listener_iface_init(LeaITabModelListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaSurface, lea_surface, GTK_TYPE_CONTAINER,
		G_ADD_PRIVATE(LeaSurface)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IREGION, l_region_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_ITAB_MODEL_LISTENER, l_tab_model_listener_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static GType l_container_child_type(GtkContainer *container);
static void l_container_add(GtkContainer *container, GtkWidget *child);
static void l_container_remove(GtkContainer *container, GtkWidget *child);
static void l_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);
static void l_container_set_focus_child(GtkContainer *container, GtkWidget *child);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr);
static void l_widget_destroy(GtkWidget *widget);

static void lea_surface_class_init(LeaSurfaceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkContainerClass *container_class = GTK_CONTAINER_CLASS(clazz);
	container_class->child_type = l_container_child_type;
	container_class->add = l_container_add;
	container_class->remove = l_container_remove;
	container_class->forall = l_container_forall;
	container_class->set_focus_child = l_container_set_focus_child;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->size_allocate = l_widget_size_allocate;
	widget_class->draw = l_widget_draw;
	widget_class->destroy = l_widget_destroy;
}

static void lea_surface_init(LeaSurface *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaSurface *instance = LEA_SURFACE(object);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(instance);
	G_OBJECT_CLASS(lea_surface_parent_class)->dispose(object);
	cat_unref_ptr(priv->tab_bar);
	cat_unref_ptr(priv->tab_model);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_surface_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaSurface *lea_surface_new() {
	LeaSurface *result = g_object_new(LEA_TYPE_SURFACE, NULL);
	cat_ref_anounce(result);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(result);
	priv->hold = FALSE;
	gtk_widget_set_has_window(GTK_WIDGET(result), FALSE);
	priv->tab_model = lea_surface_tab_model_new();
	lea_surface_tab_model_add_listener(priv->tab_model, LEA_ITAB_MODEL_LISTENER(result));
	priv->tab_bar = lea_surface_tab_bar_new(priv->tab_model);
	gtk_widget_set_parent(GTK_WIDGET(priv->tab_bar), GTK_WIDGET(result));

	gtk_widget_set_can_focus(GTK_WIDGET(result), FALSE);
	return result;
}

LeaSurfaceTabModel *lea_surface_get_tab_model(LeaSurface *surface) {
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	return priv->tab_model;
}

LeaSurfaceTabBar *lea_surface_get_tab_bar(LeaSurface *surface) {
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	return priv->tab_bar;
}

gboolean lea_surface_has_tabs(LeaSurface *surface) {
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	return lea_surface_tab_model_count(priv->tab_model)>0;
}


struct _LeaFrame *lea_surface_get_frame(LeaSurface *surface) {
	struct _LeaFrame *result = NULL;
	GtkWidget *parent = gtk_widget_get_parent(GTK_WIDGET(surface));
	if (LEA_IS_FRAME(parent)) {
		result = (LeaFrame *) parent;
	}
	return result;
}

void lea_surface_set_selected_tab(LeaSurface *surface, GtkWidget *widget_on_tab) {
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	int index = lea_surface_tab_model_find_tab_owning_widget(priv->tab_model, widget_on_tab);
	if (index>=0) {
		lea_surface_tab_model_set_selected_tab(priv->tab_model, index);
	}
}

void lea_surface_set_hold(LeaSurface *surface, gboolean hold) {
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	priv->hold = TRUE;
}


gboolean lea_surface_get_hold(LeaSurface *surface) {
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	return priv->hold;

}



static GtkWidget *l_get_visible_widget(LeaSurfacePrivate *priv) {
	GtkWidget *widget = NULL;
	LeaSurfaceTab *tab = lea_surface_tab_model_get_selected_tab(priv->tab_model);
	if (tab) {
		widget = lea_surface_tab_get_widget(tab);
	}
	return widget;
}



static void l_container_set_focus_child(GtkContainer *container, GtkWidget *child) {
	cat_log_debug("focus-child");
	LeaSurface *surface = LEA_SURFACE(container);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);

	GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(container));
	if (toplevel && gtk_widget_is_toplevel(toplevel)) {
		GtkWidget *focus_widget = gtk_window_get_focus(GTK_WINDOW(toplevel));
		GtkWidget *par_child_widget = focus_widget;
		while(par_child_widget) {
			GtkWidget *par_widget = gtk_widget_get_parent(par_child_widget);
			if (par_widget == (GtkWidget *) container) {
				int tab_idx = lea_surface_tab_model_find_tab_owning_widget(priv->tab_model, par_child_widget);
				if (tab_idx>=0) {
					LeaSurfaceTab *tab = lea_surface_tab_model_get_tab_at(priv->tab_model, tab_idx);
					lea_surface_tab_set_last_focus_child(tab, focus_widget);
				}
				break;
			}
			par_child_widget = par_widget;
		}
	}


	LeaFrame *frame = lea_surface_get_frame(surface);

	lea_frame_focus_widget_set(frame, child);

	GTK_CONTAINER_CLASS(lea_surface_parent_class)->set_focus_child(container, child);

	// TODO: test whether this is really necessary
	GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(surface));
	if (window) {
		gdk_window_invalidate_rect(window, NULL, TRUE);
	}


}


static GType l_container_child_type(GtkContainer *container) {
	return GTK_TYPE_WIDGET;
}

static void l_container_add(GtkContainer *container, GtkWidget *child) {
	LeaSurface *surface = LEA_SURFACE(container);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);

	CatStringWo *a_label = cat_string_wo_new();
	cat_string_wo_format(a_label, "%o", child);
	a_label = cat_string_wo_anchor(a_label, 0);
	LeaSurfaceTab *tab = lea_surface_tab_new(child, a_label);
	lea_surface_tab_model_add_tab(priv->tab_model, tab);
	cat_unref_ptr(tab);
	cat_unref_ptr(a_label);
}

static void l_container_remove(GtkContainer *container, GtkWidget *child) {
	LeaSurface *surface = LEA_SURFACE(container);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	int tab_index = lea_surface_tab_model_find_tab_owning_widget(priv->tab_model, child);
	cat_log_debug("tab_index=%d", tab_index);
	if (tab_index>=0) {
		lea_surface_tab_model_remove_tab_at(priv->tab_model, tab_index);
	}
}

static void l_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data) {
	LeaSurface *surface = LEA_SURFACE(container);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	if (priv->tab_model) {
		CatIIterator *iter = lea_surface_tab_model_iterator(priv->tab_model);
		if (iter) {
			while(cat_iiterator_has_next(iter)) {
				LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
				GtkWidget *widget = lea_surface_tab_get_widget(tab);
				if (widget) {
					(* callback) (widget, callback_data);
				}
			}
			cat_unref_ptr(iter);
		}
	}
	if (priv->tab_bar) {
		(* callback) (GTK_WIDGET(priv->tab_bar), callback_data);
	}
}


#define LEA_TAB_INSETS 4

static int l_calculate_tab_bar_height(LeaSurface *surface) {
	PangoContext *pango_context = gtk_widget_get_pango_context(GTK_WIDGET(surface));
	PangoFontDescription *font_desc = pango_context_get_font_description(pango_context);
	PangoFontMetrics *pango_font_metrics = pango_context_get_metrics(pango_context, font_desc, gtk_get_default_language());
	int font_size = (pango_font_metrics_get_ascent(pango_font_metrics) + pango_font_metrics_get_descent(pango_font_metrics))/PANGO_SCALE;

	int result = font_size+LEA_TAB_INSETS+LEA_TAB_INSETS;
	cat_log_debug("calculated height=%d", result);
	return result;
}


static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	gtk_widget_set_allocation(widget, allocation);
	LeaSurface *surface = LEA_SURFACE(widget);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	GtkWidget *visible_widget = l_get_visible_widget(priv);
	cat_log_debug("calling size_allocate on item:%o, allocation[%d,%d,%d,%d]", visible_widget, allocation->x, allocation->y, allocation->width, allocation->height);


	int tab_height = l_calculate_tab_bar_height(surface);
	cat_log_debug("tab_height:%d", tab_height);

	priv->child_allocation = *allocation;
	priv->child_allocation.x += 5;
	priv->child_allocation.y += 5+tab_height;
	priv->child_allocation.width -= 10;
	priv->child_allocation.height -= 10+tab_height;

	priv->tab_allocation = priv->child_allocation;
	priv->tab_allocation.y -= tab_height;
	priv->tab_allocation.height = tab_height;
	priv->tab_allocation.x -= 2;
	priv->tab_allocation.width += 4;

	if (visible_widget) {
		if ((priv->child_allocation.width>0) && (priv->child_allocation.height>0)) {
			gtk_widget_size_allocate(visible_widget, &(priv->child_allocation));
			gtk_widget_show_all(visible_widget);
		} else {
			gtk_widget_hide(visible_widget);
		}
	}

	if (priv->tab_bar) {
		if ((priv->tab_allocation.width>0) && (priv->tab_allocation.height>0)) {
			gtk_widget_size_allocate((GtkWidget *) priv->tab_bar, &(priv->tab_allocation));
			gtk_widget_show((GtkWidget *) priv->tab_bar);
		} else {
			gtk_widget_hide((GtkWidget *) priv->tab_bar);
		}
	}
}



static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr) {
	LeaSurface *surface = LEA_SURFACE(widget);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	GtkWidget *visible_widget = l_get_visible_widget(priv);
	cat_log_debug("calling draw on item:%o", visible_widget);
	if (visible_widget) {
		gtk_container_propagate_draw(GTK_CONTAINER(widget), visible_widget, cr);
	}

	if (priv->tab_bar) {
		gtk_container_propagate_draw(GTK_CONTAINER(widget), GTK_WIDGET(priv->tab_bar), cr);
	}

	if (priv->tab_model && lea_surface_tab_model_count(priv->tab_model)!=0) {
		GtkAllocation alloc;
		gtk_widget_get_allocation(widget, &alloc);
		double left = priv->child_allocation.x - alloc.x;
		double right = left + priv->child_allocation.width;
		double top = priv->child_allocation.y - alloc.y;
		double bottom = top + priv->child_allocation.height;


		LeaFrame *frame = lea_surface_get_frame(surface);
		gboolean is_focus_surface = lea_frame_get_focus_surface(frame)==surface;

		if (is_focus_surface) {

			left-=1;
			right+=1;
			top-=1;
			bottom+=1;
			cairo_set_line_width(cr, 2);
			if (lea_frame_get_focus_active(frame)) {
				cairo_set_source_rgb(cr,0.50,0.125,0.0);
			} else {
				cairo_set_source_rgb(cr,0.4,0.4,0.4);
			}
		} else {
			left-=0.5;
			right+=0.5;
			top-=1;
			bottom+=0.5;
			cairo_set_line_width(cr, 1);
			cairo_set_source_rgb(cr, 0.4,0.4,0.4);
		}
		cairo_move_to(cr, left, top);
		cairo_line_to(cr, left, bottom);
		cairo_line_to(cr, right, bottom);
		cairo_line_to(cr, right, top);
		cairo_stroke(cr);


	}

	return FALSE;
}

static void l_widget_destroy(GtkWidget *widget) {
	cat_log_detail("destroying:%o", widget);
	LeaSurface *surface = LEA_SURFACE(widget);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	if (priv->tab_bar) {
		cat_ref_ptr(priv->tab_bar);
		gtk_widget_unparent((GtkWidget *) priv->tab_bar);
		gtk_widget_destroy((GtkWidget *) priv->tab_bar);
		cat_unref_ptr(priv->tab_bar);
	}
	if (priv->tab_model) {
		lea_surface_tab_model_destroy(priv->tab_model);
	}


	GTK_WIDGET_CLASS(lea_surface_parent_class)->destroy(widget);
}


/********************* start LeaITabModelListener implementation *********************/

static void l_tab_model_list_changed(LeaITabModelListener *self) {
}

static void l_tab_model_tab_removed(LeaITabModelListener *self, LeaSurfaceTab *tab) {
	GtkWidget *widget = lea_surface_tab_get_widget(tab);
	gtk_widget_unparent(widget);
	LeaSurface *surface = LEA_SURFACE(self);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	if (lea_surface_tab_model_count(priv->tab_model)==0) {
		GtkWidget *par = gtk_widget_get_parent(GTK_WIDGET(self));
		gtk_widget_queue_draw(par);
	}
}

static void l_tab_model_tab_added(LeaITabModelListener *self, LeaSurfaceTab *tab) {
	GtkWidget *widget = lea_surface_tab_get_widget(tab);
	gtk_widget_set_parent(widget, GTK_WIDGET(self));
	LeaSurface *surface = LEA_SURFACE(self);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
	if (lea_surface_tab_model_count(priv->tab_model)==1) {
		GtkWidget *par = gtk_widget_get_parent(GTK_WIDGET(self));
		gtk_widget_queue_draw(par);
	}
}

static gboolean  l_grab_first_focusable(GtkWidget *widget) {
	if (gtk_widget_get_can_focus(widget)) {
		cat_log_debug("grabbing=%o", widget);
		gtk_widget_grab_focus(widget);
		return TRUE;
	}
	if (GTK_IS_CONTAINER(widget)) {
		GList *children = gtk_container_get_children((GtkContainer *) widget);
		while(children) {
			if (l_grab_first_focusable((GTK_WIDGET(children->data)))) {
				cat_log_debug("children->data=%o", children->data);
				g_list_free(children);
				return TRUE;
			}
			children = children->next;
		}
		g_list_free(children);
	}
	return FALSE;
}

static void l_tab_model_tab_selected(LeaITabModelListener *self, int index, LeaSurfaceTab *tab) {
	if (tab) {
		LeaSurface *surface = LEA_SURFACE(self);
		LeaSurfacePrivate *priv = lea_surface_get_instance_private(surface);
		GtkWidget *widget = lea_surface_tab_get_widget(tab);
		if ((priv->child_allocation.width>0) && (priv->child_allocation.height>0)) {
			gtk_widget_size_allocate(widget, &(priv->child_allocation));
			gtk_widget_show_all(widget);


			GtkWidget *last_focus_child_ref = lea_surface_tab_get_last_focus_child_ref(tab);
			if (last_focus_child_ref) {
				gtk_widget_grab_focus(last_focus_child_ref);
				cat_unref_ptr(last_focus_child_ref);
			} else {
				GtkWidget *focus_child = NULL;
				if (GTK_IS_CONTAINER(widget)) {
					focus_child = gtk_container_get_focus_child(GTK_CONTAINER(widget));
				}
				cat_log_debug("focus_child=%o", focus_child);
				if (focus_child) {
					gtk_widget_grab_focus(focus_child);
				} else {
					l_grab_first_focusable(widget);
				}
			}

			if (LEA_IS_PANEL(widget)) {
				LeaPanel *panel = LEA_PANEL(widget);
				LeaFrame *frame = lea_surface_get_frame(surface);
				lea_panel_focus_activated(panel, lea_frame_get_focus_active(frame));
			}

		} else {
			gtk_widget_hide(widget);
		}
	}
}


static void l_tab_model_listener_iface_init(LeaITabModelListenerInterface *iface) {
	iface->listChanged = l_tab_model_list_changed;
	iface->tabAdded = l_tab_model_tab_added;
	iface->tabRemoved = l_tab_model_tab_removed;
	iface->tabSelected = l_tab_model_tab_selected;
}

/********************* end LeaITabModelListener implementation *********************/


/********************* start LeaIRegion implementation *********************/

static void l_region_size_allocate(LeaIRegion *self, GtkAllocation *allocation) {
	gtk_widget_size_allocate(GTK_WIDGET(self), allocation);
}

static const GtkAllocation l_region_get_allocation(LeaIRegion *self) {
	GtkAllocation allocation;
	gtk_widget_get_allocation(GTK_WIDGET(self), &allocation);
	return allocation;
}


static void l_region_iface_init(LeaIRegionInterface *iface) {
	iface->sizeAllocate = l_region_size_allocate;
	iface->getAllocation = l_region_get_allocation;
}

/********************* end LeaIRegion implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaSurface *instance = LEA_SURFACE(self);
	LeaSurfacePrivate *priv = lea_surface_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p priv->tab_model=%o, child-alloc[%d,%d,%d,%d]]", iname, self, priv->tab_model, priv->child_allocation.x, priv->child_allocation.y, priv->child_allocation.width, priv->child_allocation.height);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

