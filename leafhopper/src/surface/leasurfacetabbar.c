/*
   File:    leasurfacetabbar.c
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

#include "leasurfacetabbar.h"
#include "leasurface.h"
#include "leaitabmodellistener.h"
#include "../leaframe.h"
#include "../layout/leapanel.h"
#include "drag/leagripmove.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaSurfaceTabBar"
#include <logging/catlog.h>

#define TAB_PAGE_SIZE 40

struct _LeaSurfaceTabBarPrivate {
	LeaSurfaceTabModel *tab_model;
	GdkWindow *event_window;

	gboolean tab_model_view_invalid;
	gboolean view_rect_invalid;
	int tab_model_view_offset;
	int tab_model_view_width;
	gboolean show_steppers;
	gboolean back_stepper_sensitive;
	gboolean back_stepper_prehighlight;
	gboolean back_stepper_pressed;
	gboolean forward_stepper_sensitive;
	gboolean forward_stepper_pressed;
	gboolean forward_stepper_prehighlight;
	LeaSurfaceTab *hover_tab;
	int hover_index;
	gboolean hover_is_on_close;
	guint32 grab_time;
	int grab_start_x, grab_start_y;
	gboolean grab_active;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_tab_model_listener_iface_init(LeaITabModelListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaSurfaceTabBar, lea_surface_tab_bar, GTK_TYPE_WIDGET,
		G_ADD_PRIVATE(LeaSurfaceTabBar)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_ITAB_MODEL_LISTENER, l_tab_model_listener_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr);
static void l_widget_realize(GtkWidget *widget);
static void l_widget_unrealize(GtkWidget *widget);
static void l_widget_map(GtkWidget *widget);
static void l_widget_unmap(GtkWidget *widget);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gboolean l_button_release(GtkWidget *gwidget, GdkEventButton *eev, gpointer data);
static gboolean l_button_press(GtkWidget *gwidget, GdkEventButton *eev, gpointer data);
static gboolean l_leave_notify(GtkWidget *gwidget, GdkEventCrossing *eev, gpointer data);
static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *eev, GObject *data);
static gboolean l_scroll_event(GtkWidget *widget, GdkEventScroll *event, GObject *data);


static void lea_surface_tab_bar_class_init(LeaSurfaceTabBarClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->draw = l_widget_draw;
	widget_class->realize = l_widget_realize;
	widget_class->unrealize = l_widget_unrealize;
	widget_class->map = l_widget_map;
	widget_class->unmap = l_widget_unmap;
	widget_class->size_allocate = l_widget_size_allocate;
}

static void lea_surface_tab_bar_init(LeaSurfaceTabBar *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(object);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);
	cat_unref_ptr(priv->hover_tab);
	cat_unref_ptr(priv->tab_model);
	G_OBJECT_CLASS(lea_surface_tab_bar_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_surface_tab_bar_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaSurfaceTabBar *lea_surface_tab_bar_new(LeaSurfaceTabModel *tab_model) {
	LeaSurfaceTabBar *result = g_object_new(LEA_TYPE_SURFACE_TAB_BAR, NULL);
	cat_ref_anounce(result);
	gtk_widget_set_has_window((GtkWidget *) result, FALSE);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(result);
	priv->tab_model = cat_ref_ptr(tab_model);

	priv->back_stepper_sensitive = FALSE;
	priv->back_stepper_pressed = FALSE;
	priv->back_stepper_prehighlight = FALSE;

	priv->forward_stepper_sensitive = FALSE;
	priv->forward_stepper_pressed = FALSE;
	priv->forward_stepper_prehighlight = FALSE;

	priv->tab_model_view_offset = 0;
	priv->tab_model_view_width = 0;
	priv->grab_time = 0;
	priv->grab_active = FALSE;

	priv->tab_model_view_invalid = TRUE;
	priv->hover_tab = NULL;
	priv->hover_is_on_close = FALSE;



	lea_surface_tab_model_add_listener(tab_model, LEA_ITAB_MODEL_LISTENER(result));


	gtk_widget_set_events((GtkWidget *) result, gtk_widget_get_events((GtkWidget *) result)
												 | GDK_BUTTON_PRESS_MASK
												 | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_POINTER_MOTION_MASK
	                                             | GDK_POINTER_MOTION_HINT_MASK
	                                             | GDK_SCROLL_MASK);

	g_signal_connect(result, "button-press-event", G_CALLBACK(l_button_press), G_OBJECT(result));
	g_signal_connect(result, "button-release-event", G_CALLBACK(l_button_release), G_OBJECT(result));
	g_signal_connect(result, "motion-notify-event", G_CALLBACK(l_mouse_motion), G_OBJECT(result));
	g_signal_connect(result, "leave-notify-event", G_CALLBACK(l_leave_notify), G_OBJECT(result));
	g_signal_connect(result, "scroll-event", G_CALLBACK(l_scroll_event), G_OBJECT(result));

	gtk_widget_set_can_focus(GTK_WIDGET(result), FALSE);
	gtk_widget_set_can_default(GTK_WIDGET(result), FALSE);

	return result;
}


static void l_setup_view_rect(LeaSurfaceTabBar *tab_bar, GdkRectangle *view_rect) {
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	GtkWidget *widget = GTK_WIDGET(tab_bar);

	GtkAllocation widget_alloc;
	gtk_widget_get_allocation(widget, &widget_alloc);
	view_rect->x = 0;
	view_rect->y = 0;
	view_rect->width = widget_alloc.width;
	view_rect->height = widget_alloc.height;
	if (priv->show_steppers) {
		view_rect->width = view_rect->width-view_rect->height*2;
		view_rect->x = view_rect->x+view_rect->height;
	}
	cat_log_debug("view_rect[%d,%d,%d,%d]", view_rect->x, view_rect->y, view_rect->width, view_rect->height);
}


static void l_validate(LeaSurfaceTabBar *tab_bar) {
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	cat_log_debug("validating: %p", tab_bar);
	int tab_height = gtk_widget_get_allocated_height(GTK_WIDGET(tab_bar));
	int tab_width = gtk_widget_get_allocated_width(GTK_WIDGET(tab_bar));
	if (priv->tab_model_view_invalid) {

		priv->tab_model_view_invalid = FALSE;
		int x_offset = 0;
		CatIIterator *iter = lea_surface_tab_model_iterator(priv->tab_model);
		if (iter) {
			while(cat_iiterator_has_next(iter)) {
				LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
				const GtkAllocation alloc = lea_surface_tab_do_layout(tab, tab_height, x_offset);
				x_offset += alloc.width;
			}
			cat_unref_ptr(iter);
		}
		priv->tab_model_view_width = x_offset;
		priv->view_rect_invalid = TRUE;
	}

	if (priv->view_rect_invalid) {
		priv->view_rect_invalid = FALSE;

		if (tab_width>=priv->tab_model_view_width) {
			priv->show_steppers = FALSE;
			priv->tab_model_view_offset = 0;
		} else {
			priv->show_steppers = TRUE;
			int tabs_view_width = tab_width - tab_height*2;
			if (priv->tab_model_view_offset+tabs_view_width>priv->tab_model_view_width) {
				priv->tab_model_view_offset = priv->tab_model_view_width-tabs_view_width;
			}
			if (priv->tab_model_view_offset<0) {
				priv->tab_model_view_offset = 0;
			}
		}

		cat_log_detail("end validation: %p, priv->show_steppers=%d, tab_model_view_width=%d", tab_bar, priv->show_steppers, priv->tab_model_view_width);
	}
}



gboolean lea_surface_tab_bar_get_tab_view_rect(LeaSurfaceTabBar *tab_bar, int panel_idx, GdkRectangle *field_rect) {
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	gboolean result = FALSE;
	l_validate(tab_bar);

	GdkRectangle view_rect;
	l_setup_view_rect(tab_bar, &view_rect);
	if (panel_idx==-1) {
		field_rect->y = view_rect.y;
		field_rect->height = view_rect.height;
		field_rect->x = view_rect.x;
		field_rect->width = view_rect.width;
		return TRUE;
	}

	LeaSurfaceTab *tab = lea_surface_tab_model_get_tab_at(priv->tab_model, panel_idx);
	if (tab==NULL) {
		return FALSE;
	}

	lea_surface_tab_get_rectangle(tab, field_rect);
	field_rect->x -= priv->tab_model_view_offset;

	field_rect->x += view_rect.x;
	field_rect->y += view_rect.y;

	result = gdk_rectangle_intersect(field_rect, &view_rect, field_rect);

	return result;
}


int lea_surface_tab_bar_get_panel_index_at(LeaSurfaceTabBar *tab_bar, int xpos, int ypos) {
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);

	GdkRectangle view_rect;
	l_setup_view_rect(tab_bar, &view_rect);
	if ((xpos<view_rect.x || xpos>view_rect.x+view_rect.width) ||
			(ypos<view_rect.y || ypos>view_rect.y+view_rect.height)) {
		return -2;
	}


	int tab_bar_index = 0;
	CatIIterator *iter = lea_surface_tab_model_iterator(priv->tab_model);

	int result = -1;
	gboolean is_on_close;
	xpos += priv->tab_model_view_offset;
	if (priv->show_steppers) {
		int tab_height = gtk_widget_get_allocated_height(GTK_WIDGET(tab_bar));
		xpos -= tab_height;
	}
	while(result<0 && cat_iiterator_has_next(iter)) {
		LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
		if (lea_surface_tab_get_hover_info(tab, xpos, ypos, &is_on_close)) {
			result = tab_bar_index;
		}
		tab_bar_index++;
	}
	cat_unref(iter);
	return result;
}


void lea_surface_tab_bar_invalidate_layout(LeaSurfaceTabBar *tab_bar) {
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	priv->tab_model_view_invalid = TRUE;
}


void lea_surface_tab_bar_ensure_index_visible(LeaSurfaceTabBar *tab_bar) {
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	LeaSurfaceTab *tab = lea_surface_tab_model_get_selected_tab(priv->tab_model);
	if (tab==NULL) {
		return;
	}
	l_validate(tab_bar);
	GdkRectangle view_rect;
	l_setup_view_rect(tab_bar, &view_rect);

	GdkRectangle field_rect;
	int tab_model_view_offset = priv->tab_model_view_offset;
	lea_surface_tab_get_rectangle(tab, &field_rect);

	cat_log_debug("tab_model_view_offset=%d", tab_model_view_offset);
	cat_log_debug("field_rect.x/width=%d/%d, view_rect.width=%d ", field_rect.x, field_rect.width, view_rect.width);
	if (field_rect.x+field_rect.width>tab_model_view_offset+view_rect.width) {
		tab_model_view_offset = field_rect.x+field_rect.width-view_rect.width;
	}
	if (field_rect.x<tab_model_view_offset) {
		tab_model_view_offset = field_rect.x;
	}
	if (tab_model_view_offset<0) {
		tab_model_view_offset = 0;
	}
	cat_log_debug("tab_model_view_offset=%d", tab_model_view_offset);
	if (priv->tab_model_view_offset!=tab_model_view_offset) {
		priv->tab_model_view_offset = tab_model_view_offset;
		gtk_widget_queue_draw((GtkWidget *) tab_bar);
	}

	priv->tab_model_view_invalid = TRUE;

}


static void l_draw_tab_border(cairo_t *cairo, int view_right, const GdkRectangle field_rect, gboolean is_selected_tab, gboolean surface_has_focus, gboolean focus_active) {
	double rect_left = field_rect.x;
	double rect_right = field_rect.x+field_rect.width;
	double rect_top = 0;
	double rect_bottom = field_rect.height;

	double bview_left = 0;
	double bview_right = view_right;

	if (surface_has_focus && is_selected_tab) {
		cairo_set_line_width(cairo,2);
		rect_left += 1;
		rect_right -= 2;
		rect_top = 1;
		rect_bottom = field_rect.height-1;
		if (focus_active) {
			cairo_set_source_rgb(cairo,0.50,0.125,0.0);
		} else {
			cairo_set_source_rgb(cairo,0.4,0.4,0.4);
		}
	} else {
		bview_left+=1;
		bview_right-=2;
		cairo_set_source_rgb(cairo, 0.4,0.4,0.4);
		rect_left += 1.5;
		rect_right -= 2.5;
		rect_top = 0.5;
		rect_bottom = field_rect.height-0.5;
		cairo_set_line_width(cairo, 1);
	}

	if (is_selected_tab) {

		cairo_move_to(cairo, rect_left, rect_bottom);

		int r=10;
		cairo_line_to(cairo, rect_left, rect_top+r);

		cairo_curve_to(cairo,rect_left, rect_top+r/2, rect_left+r/3,rect_top, rect_left+r,rect_top);

		cairo_line_to(cairo, rect_right-r, rect_top);
		cairo_curve_to(cairo,rect_right-r/3, rect_top, rect_right,rect_top+r/2, rect_right,rect_top+r);

		cairo_line_to(cairo, rect_right, rect_bottom);


		cairo_line_to(cairo, bview_right, rect_bottom);
		cairo_stroke(cairo);
		cairo_move_to(cairo, bview_left, rect_bottom);
		cairo_line_to(cairo, rect_left, rect_bottom);
		cairo_stroke(cairo);
	} else {
		cairo_move_to(cairo, rect_left, rect_bottom);
		cairo_line_to(cairo, rect_left, rect_top);
		cairo_line_to(cairo, rect_right, rect_top);
		cairo_line_to(cairo, rect_right, rect_bottom);
		cairo_stroke(cairo);
	}
}


static void l_paint_stepper(LeaSurfaceTabBar *tab_bar, cairo_t *cairo, gboolean forward_stepper, GdkRectangle *arrow_rect, gboolean clicked, gboolean prelighted, gboolean sensitive) {
	cat_log_debug("forward_stepper=%d, clicked=%d, prelighted=%d", forward_stepper, clicked, prelighted);
	GtkWidget *widget = (GtkWidget *) tab_bar;
	int arrow_x, arrow_y;
	int arrow_width, arrow_height;
	gfloat arrow_scaling = 0.5f;
	GtkStateType state_type = GTK_STATE_NORMAL;

	if (!sensitive) {
		state_type = GTK_STATE_INSENSITIVE;
	} else if (clicked) {
		state_type = GTK_STATE_ACTIVE;
	} else if (prelighted) {
		state_type = GTK_STATE_PRELIGHT;
	}

	arrow_width = arrow_rect->width * arrow_scaling;
	arrow_height = arrow_rect->height * arrow_scaling;

	arrow_x = arrow_rect->x + (arrow_rect->width - arrow_width) / 2;
	arrow_y = arrow_rect->y + (arrow_rect->height - arrow_height) / 2;

	GtkStyleContext *w_style_ctx = gtk_widget_get_style_context(widget);
	gtk_style_context_save(w_style_ctx);
	gtk_style_context_set_state(w_style_ctx, state_type);
	gtk_render_background(w_style_ctx, cairo, arrow_rect->x, arrow_rect->y, arrow_rect->width, arrow_rect->height);
	gtk_render_arrow(w_style_ctx, cairo, forward_stepper ? G_PI/2 : G_PI*3/2 , arrow_x, arrow_y, arrow_width);
	gtk_style_context_restore(w_style_ctx);
}

static void l_get_stepper_rect(LeaSurfaceTabBar *tab_bar, gboolean forward_stepper, GdkRectangle *view_rect, GdkRectangle *stepper_rect) {
	int tab_height = gtk_widget_get_allocated_height(GTK_WIDGET(tab_bar))-4;
	int tab_width = gtk_widget_get_allocated_width(GTK_WIDGET(tab_bar))-2;
	if (forward_stepper) {
		stepper_rect->x = tab_width-tab_height;
	} else {
		stepper_rect->x = 0;
	}
	stepper_rect->y = 1;
	stepper_rect->width = tab_height+2;
	stepper_rect->height = tab_height;
}


static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr) {
	LeaSurfaceTabBar *tab_bar = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);

	GtkAllocation widget_alloc;
	gtk_widget_get_allocation(widget, &widget_alloc);

	l_validate(tab_bar);


	cairo_save(cr);

	GdkRectangle view_rect;
	l_setup_view_rect(tab_bar, &view_rect);

	cairo_rectangle(cr, view_rect.x, view_rect.y, view_rect.width, view_rect.height);
	cairo_set_source_rgb(cr, 1,0,0);
	cairo_clip(cr);

	int view_right = view_rect.x+view_rect.width;
	int tab_model_view_offset = -view_rect.x+priv->tab_model_view_offset;


	GtkWidget *sw = gtk_widget_get_parent(widget);
	gboolean surface_has_focus = FALSE;
	gboolean focus_active = FALSE;
	if (LEA_IS_SURFACE(sw)) {
		LeaSurface *surface = LEA_SURFACE(sw);
		LeaFrame *frame = lea_surface_get_frame(surface);
		LeaSurface *focus_surface = lea_frame_get_focus_surface(frame);
		surface_has_focus = focus_surface==surface;
		focus_active = lea_frame_get_focus_active(frame);
	}

	CatIIterator *iter = lea_surface_tab_model_iterator(priv->tab_model);
	LeaSurfaceTab *selected_tab = lea_surface_tab_model_get_selected_tab(priv->tab_model);
	while(cat_iiterator_has_next(iter)) {
		LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
		gboolean is_selected_tab = selected_tab==tab;
		gboolean is_hovering = priv->hover_tab==tab;


		GdkRectangle field_rect = lea_surface_tab_draw(tab, cr, &view_rect, tab_model_view_offset, is_hovering, is_hovering && priv->hover_is_on_close);
		cat_log_debug("tab=%o, field_rect=[%d,%d,%d,%d]", tab, field_rect.x, field_rect.y, field_rect.width, field_rect.height);

		l_draw_tab_border(cr, view_right, field_rect, is_selected_tab, surface_has_focus, focus_active);
	}
	cat_unref_ptr(iter);

	cairo_restore(cr);

	if (priv->show_steppers) {
		view_rect.x = 0;
		view_rect.width = widget_alloc.width;
		int tab_width = widget_alloc.width;
		int tab_height = widget_alloc.height;

		int tabs_view_width = tab_width - tab_height*2;
		gboolean forward_stepper_sensitive = priv->tab_model_view_offset+tabs_view_width < priv->tab_model_view_width;
		gboolean backward_stepper_sensitive = priv->tab_model_view_offset>0;


		GdkRectangle stepper_rect;
		l_get_stepper_rect(tab_bar, FALSE, &view_rect, &stepper_rect);
		l_paint_stepper(tab_bar, cr, FALSE, &stepper_rect, priv->back_stepper_pressed, priv->back_stepper_prehighlight, backward_stepper_sensitive);

		l_get_stepper_rect(tab_bar, TRUE, &view_rect, &stepper_rect);
		l_paint_stepper(tab_bar, cr, TRUE,  &stepper_rect, priv->forward_stepper_pressed, priv->forward_stepper_prehighlight, forward_stepper_sensitive);



		double shift = 0;
		double size = 0;

		double red = 0.4;
		double green = 0.4;
		double blue = 0.4;

		if (surface_has_focus) {

			shift = 1;
			cairo_set_line_width(cr, 2);
			if (focus_active) {
				cairo_set_source_rgb(cr,0.50,0.125,0.0);
				red = 0.5;
				green = 0.125;
				blue = 0.0;
			} else {
				cairo_set_source_rgb(cr,0.4,0.4,0.4);
			}
		} else {
			size = 1;
			shift = 0.5;
			cairo_set_line_width(cr, 1);
			cairo_set_source_rgb(cr, 0.4,0.4,0.4);
		}

		cairo_move_to(cr, widget_alloc.width-size-tab_height-shift, tab_height-shift);
		cairo_line_to(cr, widget_alloc.width-size, tab_height-shift);
		cairo_stroke(cr);

		cairo_move_to(cr, tab_height, tab_height-shift);
		cairo_line_to(cr, size, tab_height-shift);
		cairo_stroke(cr);


		double d2 = tab_height/2;

		cairo_pattern_t *pat;
		if (backward_stepper_sensitive) {
			pat = cairo_pattern_create_linear(tab_height-shift, 0.0,  tab_height-shift+d2, 0.0);
			cairo_pattern_add_color_stop_rgba(pat, 1, red, green, blue, 0);
			cairo_pattern_add_color_stop_rgba(pat, 0, red, green, blue, 1);
			cairo_rectangle(cr, tab_height-shift, 0, d2, tab_height);
			cairo_set_source (cr, pat);
			cairo_fill (cr);
			cairo_pattern_destroy (pat);
		}

		if (forward_stepper_sensitive) {
			pat = cairo_pattern_create_linear(widget_alloc.width-tab_height+shift, 0.0,  widget_alloc.width-tab_height+shift-d2, 0.0);
			cairo_pattern_add_color_stop_rgba(pat, 1, red, green, blue, 0);
			cairo_pattern_add_color_stop_rgba(pat, 0, red, green, blue, 1);
			cairo_rectangle(cr, widget_alloc.width-tab_height+shift-d2, 0, d2, tab_height);
			cairo_set_source (cr, pat);
			cairo_fill (cr);
			cairo_pattern_destroy (pat);
		}

	}

	return TRUE;
}

static LeaSurface *l_get_surface(LeaSurfaceTabBar *tab_bar) {
	LeaSurface *result = NULL;
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(tab_bar));
	if (LEA_IS_SURFACE(widget)) {
		result = (LeaSurface *) widget;
	}
	return result;
}

static gboolean l_update_hover(LeaSurfaceTabBar *tab_bar, int wmx, int wmy) {
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	LeaSurfaceTab *new_hover_tab = NULL;
	gboolean is_on_close = FALSE;
	gboolean result = FALSE;

	GdkRectangle view_rect;
	GtkWidget *widget = GTK_WIDGET(tab_bar);
	view_rect.x = 0;
	view_rect.y = 0;
	view_rect.width = gtk_widget_get_allocated_width(widget);
	view_rect.height = gtk_widget_get_allocated_height(widget);

	if (priv->show_steppers) {
		GdkRectangle stepper_rect;
		l_get_stepper_rect(tab_bar, FALSE, &view_rect, &stepper_rect);
		if (wmx>=stepper_rect.x && wmx<=stepper_rect.x+stepper_rect.width &&
				wmy>=stepper_rect.y && wmy<=stepper_rect.y+stepper_rect.height) {
			priv->forward_stepper_prehighlight = FALSE;
			cat_unref_ptr(priv->hover_tab);
			if (!priv->back_stepper_prehighlight) {
				priv->back_stepper_prehighlight = TRUE;
				return TRUE;
			}
			return FALSE;
		}

		l_get_stepper_rect(tab_bar, TRUE, &view_rect, &stepper_rect);
		if (wmx>=stepper_rect.x && wmx<=stepper_rect.x+stepper_rect.width &&
				wmy>=stepper_rect.y && wmy<=stepper_rect.y+stepper_rect.height) {
			priv->back_stepper_prehighlight = FALSE;
			cat_unref_ptr(priv->hover_tab);
			if (!priv->forward_stepper_prehighlight) {
				priv->forward_stepper_prehighlight = TRUE;
				return TRUE;
			}
			return FALSE;
		}


		view_rect.width = view_rect.width-2*view_rect.height;
		view_rect.x = view_rect.x+view_rect.height;
	}
	if (priv->back_stepper_prehighlight) {
		priv->back_stepper_prehighlight = FALSE;
		result = TRUE;
	}
	if (priv->forward_stepper_prehighlight) {
		priv->forward_stepper_prehighlight = FALSE;
		result = TRUE;
	}

	CatIIterator *iter = lea_surface_tab_model_iterator(priv->tab_model);


	int new_hover_index = -1;

	cat_log_detail("view_rect=[%d, %d, %d, %d]", view_rect.x, view_rect.y, view_rect.width, view_rect.height);

	wmx -= view_rect.x;
	wmy -= view_rect.y;

	if ((wmx>=0 && wmx<view_rect.x+view_rect.width)
			&& (wmy>=0 && wmy<view_rect.y+view_rect.height)) {
		wmx+=priv->tab_model_view_offset;
		l_validate(tab_bar);
		int panel_idx = 0;
		while(cat_iiterator_has_next(iter)) {
			LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
			cat_log_detail("tab[%d]=%o", panel_idx, tab);
			gboolean is_hovering = lea_surface_tab_get_hover_info(tab, wmx, wmy, &is_on_close);
			if (is_hovering) {
				new_hover_tab = tab;
				new_hover_index = panel_idx;
				break;
			}
			panel_idx++;
		}
	}

	if (priv->hover_tab!=new_hover_tab) {
		cat_ref_swap(priv->hover_tab, new_hover_tab);
		priv->hover_index = new_hover_index;
		result = TRUE;

	}
	if (priv->hover_is_on_close != is_on_close) {
		priv->hover_is_on_close = is_on_close;
		result = TRUE;
	}
	cat_unref(iter);
	return result;
}


static gboolean l_button_press(GtkWidget *widget, GdkEventButton *eev, gpointer data) {
	if (!gtk_widget_get_realized(widget)) {
		return TRUE;
	}
	LeaSurfaceTabBar *tab_bar = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	l_validate(tab_bar);

	int wmx = (int) eev->x;
	int wmy = (int) eev->y;

	priv->grab_time = (guint32) eev->time;
	priv->grab_start_x = wmx;
	priv->grab_start_y = wmy;
	cat_log_trace("press:xy=%d, %d :: time=%d", wmx, wmy, priv->grab_time);

	if (priv->show_steppers) {
		l_update_hover(tab_bar, wmx, wmy);
		if (priv->back_stepper_prehighlight) {
			priv->back_stepper_pressed = TRUE;
			priv->tab_model_view_offset-=TAB_PAGE_SIZE;
			priv->view_rect_invalid = TRUE;
			gdk_window_invalidate_rect(gtk_widget_get_window(widget), NULL, TRUE);
		} else if (priv->forward_stepper_prehighlight) {
			priv->forward_stepper_pressed = TRUE;
			priv->tab_model_view_offset+=TAB_PAGE_SIZE;
			priv->view_rect_invalid = TRUE;
			gdk_window_invalidate_rect(gtk_widget_get_window(widget), NULL, TRUE);
		}

	}

	return FALSE;
}


static gboolean l_button_release(GtkWidget *widget, GdkEventButton *eev, gpointer data) {
	if (!gtk_widget_get_realized(widget)) {
		return TRUE;
	}
	LeaSurfaceTabBar *tab_bar = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	l_validate(tab_bar);

	int wmx = (int) eev->x;
	int wmy = (int) eev->y;

	gboolean did_change = l_update_hover(tab_bar, wmx, wmy);

	if (priv->hover_tab) {
		if (priv->hover_is_on_close) {
			LeaSurfaceTab *tab = lea_surface_tab_model_get_tab_at(priv->tab_model, priv->hover_index);
			GtkWidget *widget = lea_surface_tab_get_widget(tab);
			gboolean did_close = TRUE;
			cat_unref_ptr(priv->hover_tab);
			if (LEA_IS_PANEL(widget)) {
				did_close = lea_panel_close(LEA_PANEL(widget));
			} else  if (did_close) {
				lea_surface_tab_model_remove_tab_at(priv->tab_model, priv->hover_index);
			}
			priv->hover_index = -1;

		} else {
			lea_surface_tab_model_set_selected_tab(priv->tab_model, priv->hover_index);
			did_change = TRUE;
		}
	} else if (priv->show_steppers) {
		did_change = did_change || priv->back_stepper_pressed || priv->forward_stepper_pressed;
		priv->back_stepper_pressed = FALSE;
		priv->forward_stepper_pressed = FALSE;
	}
	if (did_change) {
		gdk_window_invalidate_rect(gtk_widget_get_window(GTK_WIDGET(tab_bar)), NULL, TRUE);
	}

	priv->grab_time = 0;
	return FALSE;
}

static gboolean l_leave_notify(GtkWidget *gwidget, GdkEventCrossing *eev, gpointer data) {
	if (!gtk_widget_get_realized(gwidget)) {
		return TRUE;
	}
	LeaSurfaceTabBar *tab_bar = LEA_SURFACE_TAB_BAR(data);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	if (priv->hover_tab) {
		cat_unref_ptr(priv->hover_tab);
		priv->hover_is_on_close = FALSE;
		gdk_window_invalidate_rect(gtk_widget_get_window(GTK_WIDGET(tab_bar)), NULL, TRUE);
	}
	return TRUE;

}

static gboolean l_scroll_event(GtkWidget *widget, GdkEventScroll *event, GObject *data) {
	LeaSurfaceTabBar *tab_bar = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	gboolean result = FALSE;

	if (priv->show_steppers) {
		if (event->direction == GDK_SCROLL_UP) {
			priv->tab_model_view_offset+=TAB_PAGE_SIZE;
			priv->view_rect_invalid = TRUE;
			gdk_window_invalidate_rect(gtk_widget_get_window(widget), NULL, TRUE);
			result = TRUE;
		} else if (event->direction == GDK_SCROLL_DOWN) {
			priv->tab_model_view_offset-=TAB_PAGE_SIZE;
			priv->view_rect_invalid = TRUE;
			gdk_window_invalidate_rect(gtk_widget_get_window(widget), NULL, TRUE);
			result = TRUE;
		}
	}
	return result;
}


static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *eev, GObject *data) {
	cat_log_detail("l_mouse_motion");
	if (!gtk_widget_get_realized(widget)) {
		cat_log_trace("widget not realized");
		return TRUE;
	}
	LeaSurfaceTabBar *tab_bar = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(tab_bar);
	l_validate(tab_bar);
	int wmx = (int) eev->x;
	int wmy = (int) eev->y;

	gboolean did_change = l_update_hover(tab_bar, wmx, wmy);
	cat_log_detail("_udpate_hover:did_change=%d, tab_bar->grab_time=%d, back_stepper_prehighlight=%d, wmx=%d, wmy=%d", did_change, priv->grab_time, priv->back_stepper_prehighlight, wmx, wmy);
	if (did_change) {
		gdk_window_invalidate_rect(gtk_widget_get_window(GTK_WIDGET(tab_bar)), NULL, TRUE);
	} else if (priv->grab_time>0){
		guint32 dtime = ((guint32) eev->time)-priv->grab_time;
		cat_log_debug("dtime=%d", dtime);
		if (dtime<700) {
			int deltax = priv->grab_start_x - wmx;
			int deltay = priv->grab_start_y - wmy;
			int delta = deltax*deltax+deltay*deltay;
			if (delta>5) {

				if (priv->hover_tab) {
					LeaSurface *surface = l_get_surface(tab_bar);

					if (surface) {
						LeaFrame *frame = lea_surface_get_frame(surface);
						LeaDragLayer *drag_layer = lea_frame_get_drag_layer(frame);
						cat_log_debug("SNAP hover field !!!, priv->hover_index=%d", priv->hover_index);
						LeaGripMove *grip = lea_grip_move_new(surface, priv->hover_index);
						lea_igrip_add_notify((LeaIGrip *) grip, frame);
						lea_drag_layer_set_active_grip(drag_layer, (LeaIGrip *) grip);
						cat_unref_ptr(grip);

					}
				}

			}
		}
	}
	return TRUE;
}



static void l_widget_realize(GtkWidget *widget) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);

	GtkAllocation allocation;
	gtk_widget_get_allocation (widget, &allocation);

	gtk_widget_set_realized (widget, TRUE);

	GdkWindowAttr attributes;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.wclass = GDK_INPUT_ONLY;
	attributes.event_mask = gtk_widget_get_events (widget);
	attributes.event_mask |= (GDK_BUTTON_PRESS_MASK |
                              GDK_BUTTON_RELEASE_MASK |
                              GDK_ENTER_NOTIFY_MASK |
                              GDK_LEAVE_NOTIFY_MASK);
	gint attributes_mask = GDK_WA_X | GDK_WA_Y;

	GdkWindow *window = gtk_widget_get_parent_window (widget);
	gtk_widget_set_window(widget, window);
	cat_ref_ptr(window);

	priv->event_window = gdk_window_new(window, &attributes, attributes_mask);
	gtk_widget_register_window(widget, priv->event_window);
}

static void l_widget_unrealize(GtkWidget *widget) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);

	if (priv->event_window) {
		gtk_widget_unregister_window(widget, priv->event_window);
		gdk_window_destroy(priv->event_window);
		priv->event_window = NULL;
	}

	GTK_WIDGET_CLASS(lea_surface_tab_bar_parent_class)->unrealize(widget);
}


static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);
	gtk_widget_set_allocation(widget, allocation);
	priv->view_rect_invalid = TRUE;

	if (gtk_widget_get_realized(widget)) {
		gdk_window_move_resize(priv->event_window, allocation->x, allocation->y, allocation->width, allocation->height);
	}
}


static void l_widget_map(GtkWidget *widget) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);

	GTK_WIDGET_CLASS(lea_surface_tab_bar_parent_class)->map(widget);

	if (priv->event_window) {
		gdk_window_show(priv->event_window);
	}
}

static void l_widget_unmap(GtkWidget *widget) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(widget);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);

	if (priv->event_window) {
		gdk_window_hide(priv->event_window);
	}

	GTK_WIDGET_CLASS(lea_surface_tab_bar_parent_class)->unmap(widget);
}


/********************* start LeaITabModelListener implementation *********************/

static void l_tab_model_list_changed(LeaITabModelListener *self) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(self);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);
	priv->tab_model_view_invalid = TRUE;
}

static void l_tab_model_tab_selected(LeaITabModelListener *self, int index, LeaSurfaceTab *tab) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(self);
	lea_surface_tab_bar_ensure_index_visible(instance);
}

static void l_tab_model_tab_added(LeaITabModelListener *self, LeaSurfaceTab *tab) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(self);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);
	priv->tab_model_view_invalid = TRUE;
	gtk_widget_queue_draw(GTK_WIDGET(instance));
}

static void l_tab_model_tab_removed(LeaITabModelListener *self, LeaSurfaceTab *tab) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(self);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);
	priv->tab_model_view_invalid = TRUE;
	gtk_widget_queue_draw(GTK_WIDGET(instance));
}


static void l_tab_model_listener_iface_init(LeaITabModelListenerInterface *iface) {
	iface->listChanged = l_tab_model_list_changed;
	iface->tabSelected = l_tab_model_tab_selected;
	iface->tabAdded = l_tab_model_tab_added;
	iface->tabRemoved = l_tab_model_tab_removed;
}

/********************* end LeaITabModelListener implementation *********************/



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaSurfaceTabBar *instance = LEA_SURFACE_TAB_BAR(self);
	LeaSurfaceTabBarPrivate *priv = lea_surface_tab_bar_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, model=%o]", iname, self, priv->tab_model);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
