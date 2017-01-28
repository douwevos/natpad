/*
   File:    leascrolledwindow.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Apr 17, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

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


#include "leascrolledwindow.h"
#include <math.h>

#include <math.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdk.h>


#ifdef LEA_X11_SUPPORT
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaScrolledWindow"
#include <logging/catlog.h>

G_DEFINE_TYPE(LeaScrolledWindow, lea_scrolled_window, GTK_TYPE_BIN)


enum {
  PROP_0,
  PROP_HADJUSTMENT,
  PROP_VADJUSTMENT,
  PROP_HSCROLLBAR_POLICY,
  PROP_VSCROLLBAR_POLICY,
  PROP_LAST
};


static void l_set_vadjustment(LeaScrolledWindow *scrolled_window, GtkAdjustment *adjustment);
static void l_set_hadjustment(LeaScrolledWindow *scrolled_window, GtkAdjustment *adjustment);
static void l_set_vadjustment_values(LeaScrolledWindow *scrolled_window, gboolean *value_changed, GtkAllocation *view_allocation);
static void l_set_hadjustment_values(LeaScrolledWindow *scrolled_window, gboolean *value_changed, GtkAllocation *view_allocation);
static void l_adjustment_value_changed(GtkAdjustment *adjustment, gpointer data);
static void l_child_size_allocate(LeaScrolledWindow *scrolled_window, GtkAllocation *child_allocation);


static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void l_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);


static void l_container_add(GtkContainer *container, GtkWidget *child);
static void l_container_remove(GtkContainer *container, GtkWidget *child);
static void l_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);

static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void l_widget_realize(GtkWidget *widget);
static void l_widget_unrealize(GtkWidget *widget);

static void l_widget_adjust_size_request(GtkWidget *widget, GtkOrientation orientation, gint *minimum_size, gint *natural_size);
static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cario);

static gint l_widget_scroll_event(GtkWidget *widget, GdkEventScroll *event);
static void l_widget_show(GtkWidget *widget);
static void l_widget_hide(GtkWidget *widget);



static void _finalize(GObject *object) {
	cat_log_detail("finalize");
	G_OBJECT_CLASS (lea_scrolled_window_parent_class)->finalize(object);
	cat_log_detail("finalize:end");

}

static void _dispose(GObject *object) {
	cat_log_detail("dispose: %p", object);
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(object);

	if (scrolled_window->vscrollbar) {
		gtk_widget_destroy(scrolled_window->vscrollbar);
		scrolled_window->vscrollbar = NULL;
	}
	if (scrolled_window->hscrollbar) {
		gtk_widget_destroy(scrolled_window->hscrollbar);
		scrolled_window->hscrollbar = NULL;
	}
	if (scrolled_window->vadjustment) {
		g_signal_handlers_disconnect_by_func(scrolled_window->vadjustment, G_CALLBACK(l_adjustment_value_changed), scrolled_window);
		cat_unref_ptr(scrolled_window->vadjustment);
	}

	if (scrolled_window->hadjustment) {
		g_signal_handlers_disconnect_by_func(scrolled_window->hadjustment, G_CALLBACK(l_adjustment_value_changed), scrolled_window);
		cat_unref_ptr(scrolled_window->hadjustment);
	}

	cat_log_detail("scrollbars destroyed");

	G_OBJECT_CLASS(lea_scrolled_window_parent_class)->dispose(object);
	cat_log_detail("child drestoryed......");
	cat_log_detail("disposed: %p", object);
}


static void lea_scrolled_window_class_init(LeaScrolledWindowClass *clazz) {
	cat_log_on_debug({
		gboolean is_bin_class = GTK_IS_BIN_CLASS(lea_scrolled_window_parent_class);
		gboolean is_container_class = GTK_IS_CONTAINER_CLASS(lea_scrolled_window_parent_class);
		cat_log_debug("parent name::::::is_bin=%d, is_container=%d", is_bin_class, is_container_class);
	});

	GtkContainerClass *container_class = (GtkContainerClass*) clazz;
	container_class->add = l_container_add;
	container_class->remove = l_container_remove;
	container_class->forall = l_container_forall;

	GtkWidgetClass *widget_class = (GtkWidgetClass*) clazz;
	widget_class->size_allocate = l_widget_size_allocate;
	widget_class->realize = l_widget_realize;
	widget_class->unrealize = l_widget_unrealize;
	widget_class->adjust_size_request = l_widget_adjust_size_request;
	widget_class->draw = l_widget_draw;
	widget_class->scroll_event = l_widget_scroll_event;
	widget_class->show = l_widget_show;
	widget_class->hide = l_widget_hide;

	GObjectClass *obj_class = G_OBJECT_CLASS(clazz);
	obj_class->set_property = l_set_property;
	obj_class->get_property = l_get_property;
	obj_class->finalize = _finalize;
	obj_class->dispose = _dispose;


	g_object_class_install_property (obj_class, PROP_HADJUSTMENT, g_param_spec_object ("hadjustment",
	                                                        ("Horizontal Adjustment"), ("The GtkAdjustment for the horizontal position"),
	                                                        GTK_TYPE_ADJUSTMENT, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
	g_object_class_install_property (obj_class, PROP_VADJUSTMENT, g_param_spec_object ("vadjustment",
	                                                        ("Vertical Adjustment"), ("The GtkAdjustment for the vertical position"),
	                                                        GTK_TYPE_ADJUSTMENT, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));


	g_object_class_install_property (obj_class, PROP_HSCROLLBAR_POLICY, g_param_spec_enum ("hscrollbar_policy",
	                                                      ("Horizontal Scrollbar Policy"), ("When the horizontal scrollbar is displayed"),
	                                                      GTK_TYPE_POLICY_TYPE, GTK_POLICY_ALWAYS, G_PARAM_READWRITE));
	g_object_class_install_property(obj_class, PROP_VSCROLLBAR_POLICY, g_param_spec_enum ("vscrollbar_policy",
	                                                      ("Vertical Scrollbar Policy"), ("When the vertical scrollbar is displayed"),
	                                                      GTK_TYPE_POLICY_TYPE, GTK_POLICY_ALWAYS, G_PARAM_READWRITE));

}

static void lea_scrolled_window_init(LeaScrolledWindow *instance) {
	gtk_widget_set_redraw_on_allocate(GTK_WIDGET(instance), FALSE);
}



LeaScrolledWindow *lea_scrolled_window_new(GtkWidget *child) {
	LeaScrolledWindow *result;


	result = g_object_new(LEA_TYPE_SCROLLED_WINDOW, NULL);
	cat_ref_anounce(result);

	gtk_widget_set_has_window(GTK_WIDGET(result), FALSE);

	result->view_window = NULL;
	result->in_scrolling = 0;
	result->view_x = 0;
	result->view_y = 0;
	result->hscrollbar_policy = GTK_POLICY_AUTOMATIC;
	result->vscrollbar_policy = GTK_POLICY_ALWAYS;
	result->hscrollbar_visible = FALSE;
	result->vscrollbar_visible = FALSE;

#ifdef LEA_X11_SUPPORT
  	result->use_x11_scroll = FALSE;
	result->xgc_created = FALSE;
#endif

	result->hadjustment = NULL;
	result->vadjustment = NULL;
	l_set_vadjustment(result, NULL);
	l_set_hadjustment(result, NULL);



	result->vscrollbar = gtk_scrollbar_new(GTK_ORIENTATION_VERTICAL, GTK_ADJUSTMENT(result->vadjustment));
	gtk_widget_set_parent(result->vscrollbar, GTK_WIDGET(result));

	result->hscrollbar = gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(result->hadjustment));
	gtk_widget_set_parent(result->hscrollbar, GTK_WIDGET(result));

	gtk_container_add(GTK_CONTAINER(result), child);
	return result;
}


cat_log_on_debug(
static void _dump_adjustment(GtkAdjustment *adjustment) {
	cat_log_debug("adjustment:%x:[lower=%d, upper=%d, page_size=%d]", (unsigned int) adjustment, (int) gtk_adjustment_get_lower(adjustment), (int) gtk_adjustment_get_upper(adjustment), (int) gtk_adjustment_get_page_size(adjustment));
}
)

void lea_scrolled_window_set_view_size(LeaScrolledWindow *scrolled_window, gint view_width, gint view_height) {
	cat_log_debug("_set_view_size:start:view_width=%d, view_height=%d", view_width, view_height);
	gboolean did_change = FALSE;
//	cat_log_debug("lea_scrolled_window_set_view_size:view_width=%d, view_height=%d\n", view_width, view_height);
	if (scrolled_window->view_height != view_height) {
		scrolled_window->view_height = view_height;

		gtk_adjustment_set_upper(GTK_ADJUSTMENT(scrolled_window->vadjustment), view_height);
		did_change = TRUE;
	}
	if (scrolled_window->view_width!=view_width) {
		scrolled_window->view_width = view_width;
		gtk_adjustment_set_upper(GTK_ADJUSTMENT(scrolled_window->hadjustment), view_width);
		did_change = TRUE;
	}

	if (did_change && (scrolled_window->hscrollbar_policy==GTK_POLICY_AUTOMATIC || scrolled_window->vscrollbar_policy==GTK_POLICY_AUTOMATIC)) {
		gtk_widget_queue_resize(GTK_WIDGET(scrolled_window));
	}

	cat_log_on_debug({
		_dump_adjustment(GTK_ADJUSTMENT(scrolled_window->vadjustment));
	});
	cat_log_debug("_set_view_size:end");
}


void lea_scrolled_window_set_view_location(LeaScrolledWindow *scrolled_window, gint view_x, gint view_y) {
	cat_log_debug("_set_view_location:view_x=%d, view_y=%d", view_x, view_y);

	gtk_adjustment_set_value(GTK_ADJUSTMENT(scrolled_window->vadjustment), view_y);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(scrolled_window->hadjustment), view_x);
}

void lea_scrolled_window_get_view_location(LeaScrolledWindow *scrolled_window, gint *view_x, gint *view_y) {
	*view_x = scrolled_window->view_x;
	*view_y = scrolled_window->view_y;
}

void lea_scrolled_window_set_use_x11_scroll(LeaScrolledWindow *scrolled_window, gboolean x11_scroll_enabled) {
#ifdef LEA_X11_SUPPORT
	scrolled_window->use_x11_scroll = x11_scroll_enabled;
#endif
}



void lea_scrolled_window_set_policy(LeaScrolledWindow *scrolled_window, GtkPolicyType hscrollbar_policy, GtkPolicyType vscrollbar_policy) {
	cat_log_debug("_set_policy:start");
	GObject *object = G_OBJECT(scrolled_window);
	g_return_if_fail (LEA_IS_SCROLLED_WINDOW(scrolled_window));

	if ((scrolled_window->hscrollbar_policy != hscrollbar_policy) || (scrolled_window->vscrollbar_policy != vscrollbar_policy)) {
		scrolled_window->hscrollbar_policy = hscrollbar_policy;
		scrolled_window->vscrollbar_policy = vscrollbar_policy;

		gtk_widget_queue_resize(GTK_WIDGET(scrolled_window));

		g_object_freeze_notify(object);
		g_object_notify(object, "hscrollbar_policy");
		g_object_notify(object, "vscrollbar_policy");
		g_object_thaw_notify(object);
    }
	cat_log_debug("_set_policy:end");
}



static void lea_scrolled_window_get_view_allocation(LeaScrolledWindow *scrolled_window, GtkAllocation *view_allocation) {
	cat_log_detail("_get_view_allocation:start");
	gint border_width = gtk_container_get_border_width(GTK_CONTAINER(scrolled_window));
	view_allocation->x = 0;
	view_allocation->y = 0;


	GtkBin *bin = GTK_BIN(scrolled_window);

	GtkWidget *binChild = gtk_bin_get_child(bin);
	if (binChild) {
		view_allocation->height = gtk_widget_get_allocated_height(binChild)-border_width*2;
		view_allocation->width = gtk_widget_get_allocated_width(binChild)-border_width*2;
		GtkRequisition dummy_requisition;
		if (scrolled_window->vscrollbar_visible) {
			GtkRequisition vscrollbar_requisition;
			gtk_widget_get_preferred_size(scrolled_window->vscrollbar, &dummy_requisition, &vscrollbar_requisition);
			view_allocation->width -= -vscrollbar_requisition.width;
		}

		if (scrolled_window->hscrollbar_visible) {
			GtkRequisition hscrollbar_requisition;
			gtk_widget_get_preferred_size(scrolled_window->hscrollbar, &dummy_requisition, &hscrollbar_requisition);
			view_allocation->height -= -hscrollbar_requisition.height;
		}
		view_allocation->width = MAX(1, view_allocation->width);
		view_allocation->height = MAX(1, view_allocation->height);
	} else {
		view_allocation->height = 1;
		view_allocation->width = 1;
	}
	cat_log_debug("_get_view_allocation:end");
}

static void l_set_vadjustment(LeaScrolledWindow *scrolled_window, GtkAdjustment *adjustment) {
	cat_log_debug("l_set_vadjustment:start");
	if (adjustment && adjustment == GTK_ADJUSTMENT(scrolled_window->vadjustment)) {
		return;
	}

	if (!adjustment) {
		adjustment = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 20.0, 1.0, 2.0, 3.0));
	}

	cat_unref_ptr(scrolled_window->vadjustment);
	scrolled_window->vadjustment = adjustment;
	cat_ref(adjustment);
	cat_ref_sink_ptr(adjustment);	// TODO : seems like a ref not being unreffed
	gboolean value_changed;

	GtkAllocation view_allocation;
	lea_scrolled_window_get_view_allocation(scrolled_window, &view_allocation);

	l_set_vadjustment_values(scrolled_window, &value_changed, &view_allocation);

	g_signal_connect(adjustment, "value_changed", G_CALLBACK (l_adjustment_value_changed), scrolled_window);


	if (value_changed) {
	} else {
		l_adjustment_value_changed (adjustment, scrolled_window);
	}
	cat_log_debug("l_set_vadjustment:end");
}


static void l_set_hadjustment(LeaScrolledWindow *scrolled_window, GtkAdjustment *adjustment) {
	cat_log_debug("l_set_hadjustment:start");
	if (adjustment && adjustment == GTK_ADJUSTMENT(scrolled_window->hadjustment)) {
		return;
	}

	if (!adjustment) {
		adjustment = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 20.0, 1.0, 2.0, 3.0));
	}

	cat_unref_ptr(scrolled_window->hadjustment);
	scrolled_window->hadjustment = adjustment;
	cat_ref(adjustment);
	cat_ref_sink_ptr(adjustment);	// TODO : seems like a ref not being unreffed

	gboolean value_changed;

	GtkAllocation view_allocation;
	lea_scrolled_window_get_view_allocation(scrolled_window, &view_allocation);

	l_set_hadjustment_values(scrolled_window, &value_changed, &view_allocation);

	g_signal_connect(adjustment, "value_changed", G_CALLBACK (l_adjustment_value_changed), scrolled_window);


	if (value_changed) {
	} else {
		l_adjustment_value_changed (adjustment, scrolled_window);
	}
	cat_log_debug("l_set_hadjustment:end");
}



static void l_adjustment_value_changed(GtkAdjustment *adjustment, gpointer data) {
	cat_log_debug("l_adjustment_value_changed:start");
	LeaScrolledWindow *scrolled_window;
	GtkBin *bin;

	g_return_if_fail (GTK_IS_ADJUSTMENT(adjustment));
	g_return_if_fail (LEA_IS_SCROLLED_WINDOW(data));

	scrolled_window = LEA_SCROLLED_WINDOW(data);

#ifdef LEA_X11_SUPPORT
  	if (!scrolled_window->use_x11_scroll) {
#endif
	scrolled_window->in_scrolling++;

#ifdef LEA_X11_SUPPORT
  	}
#endif

	bin = GTK_BIN (data);

	GtkWidget *binChild = gtk_bin_get_child(bin);
	if (binChild && gtk_widget_get_visible(binChild)  && gtk_widget_get_realized(GTK_WIDGET(scrolled_window))  && gtk_widget_get_realized(binChild)) {

		gint old_x = scrolled_window->view_x;
		gint old_y = scrolled_window->view_y;

		GtkAdjustment *hadjustment = GTK_ADJUSTMENT(scrolled_window->hadjustment);
		GtkAdjustment *vadjustment = GTK_ADJUSTMENT(scrolled_window->vadjustment);
		cat_log_on_debug({
			_dump_adjustment(GTK_ADJUSTMENT(scrolled_window->vadjustment));
		});
		gint new_x, new_y;

		new_x = gtk_adjustment_get_value(hadjustment);
		new_y = gtk_adjustment_get_value(vadjustment);

		cat_log_detail("l_adjustment_value_changed: new_x=%d, new_y=%d, old_x=%d, old_y=%d", new_x, new_y, old_x, old_y);

		gint border_width = gtk_container_get_border_width(GTK_CONTAINER(scrolled_window));
		int height = gtk_widget_get_allocated_height(binChild)-border_width*2;
		int width = gtk_widget_get_allocated_width(binChild)-border_width*2;

		gint dx = new_x - old_x;
		gint dy = new_y - old_y;

		gboolean no_scroll = (dx > width || -dx > width || dy > height || -dy > height || (dx != 0 && dy != 0));

		if (scrolled_window->in_scrolling>1) {
			no_scroll = TRUE;
		}

		cat_log_trace("dx=%d, dy=%d, no_scroll=%d, width=%d, height=%d", dx, dy, no_scroll, width, height);


		if (no_scroll || dx!=0) {
			scrolled_window->view_x = new_x;
			scrolled_window->view_y = new_y;
			GdkRectangle invalid_rect;
			invalid_rect.x = 0;
			invalid_rect.y = 0;
			invalid_rect.width = width;
			invalid_rect.height = height;
			gdk_window_invalidate_rect(scrolled_window->bin_window, &invalid_rect, TRUE);
			cat_log_detail("invalidating rect[%d,%d,%d,%d]", invalid_rect.x, invalid_rect.y, invalid_rect.width, invalid_rect.height);
		} else {
			GdkWindow *window = scrolled_window->bin_window;
			scrolled_window->view_x = new_x;
			scrolled_window->view_y = new_y;



#ifdef LEA_X11_SUPPORT
			gboolean did_x11_scroll = FALSE;
			if (scrolled_window->use_x11_scroll) {
				if (GDK_IS_X11_WINDOW(window)) {
					cat_log_debug("scrolling fast !!!");




					GdkX11Window *x11window = GDK_X11_WINDOW(window);
					Window xid = gdk_x11_window_get_xid(x11window);

					GdkDisplay *display = gdk_window_get_display(window);
					Display *xdisplay = gdk_x11_display_get_xdisplay(display);


					if (!scrolled_window->xgc_created) {
						scrolled_window->xgc_created = TRUE;
						XGCValues values;
						values.graphics_exposures = True;
						values.subwindow_mode = IncludeInferiors;
						GC xgc = XCreateGC(xdisplay, GDK_WINDOW_XID(window), GCSubwindowMode | GCGraphicsExposures, &values);
						scrolled_window->xgc = xgc;

					}


					XCopyArea(xdisplay, xid, xid, scrolled_window->xgc, 0,0, width, height, -dx, -dy);
					XFlush(xdisplay);

					gdk_window_flush(window);
					gdk_window_process_updates(scrolled_window->bin_window, FALSE);

					did_x11_scroll = TRUE;
				}
			}
			if (!did_x11_scroll) {
#endif


G_GNUC_BEGIN_IGNORE_DEPRECATIONS
				gdk_window_scroll(window, dx, dy);

				gdk_window_flush(window);
G_GNUC_END_IGNORE_DEPRECATIONS
				
#ifdef LEA_X11_SUPPORT
			}
#endif

			GdkRectangle invalid_rect;
			if (dy!=0) {
				invalid_rect.x = 0;
				invalid_rect.width = width;
				if (dy<0) {
					invalid_rect.y = 0;
					invalid_rect.height = -dy;
				} else {
					invalid_rect.y = height - dy;
					invalid_rect.height = dy+1;
				}
			} else {
				invalid_rect.y = 0;
				invalid_rect.height = height;
				if (dx<0) {
					invalid_rect.x = 0;
					invalid_rect.width = -dx;
				} else {
					invalid_rect.x = width - dx;
					invalid_rect.width = dx;
				}
			}


#ifdef G_OS_UNIX
			gdk_window_invalidate_rect(scrolled_window->bin_window, &invalid_rect, TRUE);
#endif
			cat_log_detail("invalidating rect[%d,%d,%d,%d]", invalid_rect.x, invalid_rect.y, invalid_rect.width, invalid_rect.height);
		}

#ifdef G_OS_UNIX
		gdk_window_process_updates(scrolled_window->bin_window, FALSE);
#endif
	}
	scrolled_window->in_scrolling = 0;
	cat_log_debug("l_adjustment_value_changed:end");
}



static void l_container_add(GtkContainer *container, GtkWidget *child) {
	cat_log_debug("l_container_add:start");
	GtkBin *bin;

	bin = GTK_BIN(container);
	g_return_if_fail(gtk_bin_get_child(bin) == NULL);

	GTK_CONTAINER_CLASS (lea_scrolled_window_parent_class)->add(container, child);

	cat_log_debug("l_container_add:end");
}

static void l_container_remove(GtkContainer *container, GtkWidget    *child) {
	cat_log_debug("l_container_remove:start");
	g_return_if_fail (LEA_IS_SCROLLED_WINDOW (container));
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(container);
	if (child==scrolled_window->vscrollbar || child==scrolled_window->hscrollbar) {
		return;
	}

	g_return_if_fail (child != NULL);
	g_return_if_fail (gtk_bin_get_child(GTK_BIN(container)) == child);

	/* chain parent class handler to remove child */
	GTK_CONTAINER_CLASS (lea_scrolled_window_parent_class)->remove (container, child);
	cat_log_debug("l_container_remove:end");
}



static void l_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data) {
	cat_log_debug("for-all enter");
	g_return_if_fail (LEA_IS_SCROLLED_WINDOW (container));
	g_return_if_fail (callback != NULL);

	GTK_CONTAINER_CLASS(lea_scrolled_window_parent_class)->forall(container, include_internals, callback, callback_data);
	if (include_internals) {
		LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW (container);

		if (scrolled_window->vscrollbar) {
			callback(scrolled_window->vscrollbar, callback_data);
		}
		if (scrolled_window->hscrollbar) {
			callback(scrolled_window->hscrollbar, callback_data);
		}
	}
	cat_log_debug("for-all exit");
}


static void l_widget_realize(GtkWidget *widget) {
	cat_log_debug("l_widget_realize:start");
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);

	GtkBin *bin = GTK_BIN (widget);

	gint border_width = gtk_container_get_border_width(GTK_CONTAINER(widget));
	if (gtk_widget_get_window(widget)) {
		printf("scrolled-window : HAS A WINDOW !!!!!!!\n");
		return;
	}



	GdkWindowAttr attributes;
	gint attributes_mask;
	gint event_mask;


	gtk_widget_set_realized(widget, TRUE);
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	attributes.x = allocation.x + border_width;
	attributes.y = allocation.y + border_width;
	attributes.width = allocation.width - border_width * 2;
	if (attributes.width<1) {
		attributes.width = 1;
	}
	attributes.height = allocation.height - border_width * 2;
	if (attributes.height<1) {
		attributes.height = 1;
	}
	attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;



	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual (widget);

	event_mask = gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;
	/* We select on button_press_mask so that button 4-5 scrolls are trapped.
	 */
	attributes.event_mask = event_mask
			| GDK_SCROLL_MASK
//			| GDK_SMOOTH_SCROLL_MASK
			| GDK_BUTTON_PRESS_MASK;


	GdkWindow *par_window = gtk_widget_get_parent_window(widget);
	scrolled_window->parent_window = par_window;

	gtk_widget_set_window(widget, par_window);
	gtk_widget_realize(scrolled_window->vscrollbar);
	gtk_widget_realize(scrolled_window->hscrollbar);
	GtkAllocation alloc = { 0 };
	gtk_widget_size_allocate(scrolled_window->vscrollbar, &alloc);
	gtk_widget_size_allocate(scrolled_window->hscrollbar, &alloc);


	GdkWindow *viewWindow = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
	scrolled_window->view_window = viewWindow;
	gdk_window_set_user_data (viewWindow, scrolled_window);

	GtkWidget *binChild = gtk_bin_get_child(bin);
	if (binChild) {
		gtk_widget_set_parent_window(binChild, viewWindow);
		gtk_widget_realize(binChild);
		scrolled_window->bin_window = gtk_widget_get_window(binChild);
		gdk_window_show(scrolled_window->bin_window);
	}

	gdk_window_show(viewWindow);

	if (binChild) {
		GtkAllocation child_allocation;
		l_child_size_allocate(scrolled_window, &child_allocation);
	}

	cat_log_debug("l_widget_realize:end");
}

static void l_widget_unrealize(GtkWidget *widget) {
	cat_log_debug("l_widget_unrealize:start");
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);
	GtkBin *bin = GTK_BIN (widget);

	GtkWidget *binChild = gtk_bin_get_child(bin);
	if (binChild) {
		gtk_widget_unrealize(binChild);
		scrolled_window->bin_window = NULL;
	}


	gdk_window_set_user_data(scrolled_window->view_window, NULL);
	gdk_window_destroy(scrolled_window->view_window);
	gtk_widget_set_window(widget, NULL);

	gtk_widget_unrealize(scrolled_window->vscrollbar);
	gtk_widget_unrealize(scrolled_window->hscrollbar);

	gtk_widget_set_realized(widget, FALSE);
	cat_log_debug("l_widget_unrealize:end");
}



static void l_widget_adjust_size_request(GtkWidget *widget, GtkOrientation orientation, gint *minimum_size, gint *natural_size) {
	cat_log_debug("_widget_size_request:start");
	GtkBin *bin;
	GtkRequisition requisition;
	GtkRequisition child_requisition;


	bin = GTK_BIN (widget);
	int border_width = gtk_container_get_border_width(GTK_CONTAINER (widget));

	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	GtkStateFlags gtkStateFlags = gtk_widget_get_state_flags(widget);
	cat_log_detail("gtkStateFlags=%d, context=%p", gtkStateFlags, context);
	GtkBorder border;
	gtk_style_context_get_border(context, gtkStateFlags, &border);

	requisition.width = border.left+border.right + border_width*2;
	requisition.height = border.top+border.bottom + border_width*2;

	GtkWidget *binChild = gtk_bin_get_child(bin);

	GtkRequisition dummy_requisition;
	if (binChild && gtk_widget_get_visible(binChild)) {
		gtk_widget_get_preferred_size(binChild, &dummy_requisition, &child_requisition);
		requisition.width += child_requisition.width;
		requisition.height += child_requisition.height;
	}

	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);
	GtkRequisition scrollbar_requisition;
	gtk_widget_get_preferred_size(scrolled_window->vscrollbar, &dummy_requisition, &scrollbar_requisition);
	requisition.width += scrollbar_requisition.width;

	gtk_widget_get_preferred_size(scrolled_window->hscrollbar, &dummy_requisition, &scrollbar_requisition);
	requisition.height += scrollbar_requisition.height;

	cat_log_detail("requisition[%d,%d]", requisition.width, requisition.height);
	switch(orientation) {
		case GTK_ORIENTATION_HORIZONTAL : {
			if (minimum_size) {
				*minimum_size = requisition.width;
			}
			if (natural_size) {
				*natural_size = requisition.width;
			}
		} break;
		case GTK_ORIENTATION_VERTICAL : {
			if (minimum_size) {
				*minimum_size = requisition.height;
			}
			if (natural_size) {
				*natural_size = requisition.height;
			}
		} break;
	}
}


static void l_widget_show(GtkWidget *widget) {
	if (GTK_WIDGET_CLASS(lea_scrolled_window_parent_class)->show) {
		(*GTK_WIDGET_CLASS(lea_scrolled_window_parent_class)->show) (widget);
	}

	cat_log_detail("showing:%p", widget);

	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);
	if (scrolled_window->view_window) {
		gdk_window_show(scrolled_window->view_window);
	}

	GtkWidget *childWidget = gtk_bin_get_child(GTK_BIN(widget));
	if (childWidget) {
		gtk_widget_show(childWidget);
	}

}

static void l_widget_hide(GtkWidget *widget) {
	if (GTK_WIDGET_CLASS(lea_scrolled_window_parent_class)->hide) {
		(*GTK_WIDGET_CLASS(lea_scrolled_window_parent_class)->hide) (widget);
	}

	cat_log_detail("hiding:%p", widget);

	GtkWidget *childWidget = gtk_bin_get_child(GTK_BIN(widget));
	if (childWidget) {
		gtk_widget_hide(childWidget);
	}

	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);
	if (scrolled_window->view_window) {
		gdk_window_hide(scrolled_window->view_window);
	}

}


static void _reclamp_adjustment(GtkAdjustment *adjustment, gboolean *value_changed) {
	gboolean result = FALSE;
	gdouble value = gtk_adjustment_get_value(adjustment);
	gdouble newValue = CLAMP(value, 0, gtk_adjustment_get_upper(adjustment) - gtk_adjustment_get_page_size(adjustment));
	if (value != newValue) {
		result = TRUE;
		gtk_adjustment_set_value(adjustment, newValue);
    }
	cat_log_detail("_reclamp_adjustment:adustment=%x, value=%d", (unsigned int) adjustment, (int) value);

	if (value_changed) {
		*value_changed = result;
	}
}


static void l_set_hadjustment_values(LeaScrolledWindow *scrolled_window, gboolean *value_changed, GtkAllocation *view_allocation) {
	GtkBin *bin = GTK_BIN (scrolled_window);
	GtkAdjustment *hadjustment = GTK_ADJUSTMENT(scrolled_window->hadjustment);
	gdouble old_page_size;
	gdouble old_upper;
	gdouble old_value;


	old_page_size = gtk_adjustment_get_page_size(hadjustment);
	old_upper = gtk_adjustment_get_upper(hadjustment);
	old_value = gtk_adjustment_get_value(hadjustment);
	gtk_adjustment_set_page_size(hadjustment, view_allocation->width);
	gtk_adjustment_set_step_increment(hadjustment, view_allocation->width * 0.1);
	gtk_adjustment_set_page_increment(hadjustment, view_allocation->width * 0.9);
	gtk_adjustment_set_lower(hadjustment, 0);

	GtkWidget *binChild = gtk_bin_get_child(bin);

	if (binChild && gtk_widget_get_visible(binChild)) {
		GtkRequisition child_requisition;
		GtkRequisition dummy_requisition;
		gtk_widget_get_preferred_size(binChild, &dummy_requisition, &child_requisition);
		gtk_adjustment_set_upper(hadjustment, MAX(scrolled_window->view_width, view_allocation->width));
	} else {
		gtk_adjustment_set_upper(hadjustment, view_allocation->width);
	}

	if (gtk_widget_get_direction(GTK_WIDGET(scrolled_window)) == GTK_TEXT_DIR_RTL) {
		gdouble dist = old_upper - (old_value + old_page_size);
		gtk_adjustment_set_value(hadjustment, gtk_adjustment_get_upper(hadjustment) - dist - gtk_adjustment_get_page_size(hadjustment));
		_reclamp_adjustment(hadjustment, value_changed);
		*value_changed = (old_value != gtk_adjustment_get_value(hadjustment));
	} else {
		_reclamp_adjustment(hadjustment, value_changed);
	}
}


static void l_set_vadjustment_values(LeaScrolledWindow *scrolled_window, gboolean *value_changed, GtkAllocation *view_allocation) {
	GtkAdjustment *vadjustment = GTK_ADJUSTMENT(scrolled_window->vadjustment);
	GtkBin *bin = GTK_BIN(scrolled_window);
	gtk_adjustment_set_page_size(vadjustment, view_allocation->height);
	gtk_adjustment_set_step_increment(vadjustment, view_allocation->height * 0.1);
	gtk_adjustment_set_page_increment(vadjustment, view_allocation->height * 0.9);
	gtk_adjustment_set_lower(vadjustment, 0);

	GtkWidget *binChild = gtk_bin_get_child(bin);

	if (binChild && gtk_widget_get_visible(binChild)) {
		GtkRequisition child_requisition;
		GtkRequisition dummy_requisition;
		gtk_widget_get_preferred_size(binChild, &dummy_requisition, &child_requisition);
		gtk_adjustment_set_upper(vadjustment, MAX(scrolled_window->view_height, view_allocation->height));
	} else {
		gtk_adjustment_set_upper(vadjustment, view_allocation->height);
	}

	_reclamp_adjustment(vadjustment, value_changed);
}

static void l_child_size_allocate(LeaScrolledWindow *scrolled_window, GtkAllocation *child_allocation) {
	cat_log_debug("l_child_size_allocate:start: ");
	GtkBin *bin = GTK_BIN(scrolled_window);
	GtkWidget *widget = GTK_WIDGET(scrolled_window);

	GtkWidget *binChild = gtk_bin_get_child(bin);
	if (binChild) {
		gint border_width = gtk_container_get_border_width(GTK_CONTAINER(widget));

		GtkAllocation subAlloc;
		gtk_widget_get_allocation(widget, &subAlloc);

		int sb_x = subAlloc.x - border_width;
		int sb_y = subAlloc.y - border_width;

		int sb_height = 0;
		int sb_width = 0;


		GtkRequisition min_req, nat_req;
		gtk_widget_get_preferred_size(scrolled_window->hscrollbar, &min_req, &nat_req);
		cat_log_debug("min=%d,%d, nat=%d,%d", min_req.width, min_req.height, nat_req.width, nat_req.height);


		if (scrolled_window->hscrollbar_visible) {
			if (!gtk_widget_get_visible(scrolled_window->hscrollbar)) {
				gtk_widget_show(scrolled_window->hscrollbar);
				cat_log_detail("hscrollbar :: showing");
			}

			int min_height = 0;
			int nat_height = 0;
			gtk_widget_get_preferred_height(scrolled_window->hscrollbar, &min_height, &nat_height);

			sb_height = MAX(sb_height, min_height);
			sb_height = MAX(sb_height, nat_height);
		}

		if (scrolled_window->vscrollbar_visible) {
			if (!gtk_widget_get_visible(scrolled_window->vscrollbar)) {
				gtk_widget_show(scrolled_window->vscrollbar);
			}

			int min_width = 0;
			int nat_width = 0;
			gtk_widget_get_preferred_width(scrolled_window->vscrollbar, &min_width, &nat_width);
			cat_log_debug("min_width=%d, nat_width=%d", min_width, nat_width);
			sb_width = MAX(sb_width, min_width);
			sb_width = MAX(sb_width, nat_width);
		}


		int vis_width = subAlloc.width - border_width * 2 - sb_width;
		int vis_height = subAlloc.height - border_width * 2 - sb_height;

		vis_width = MAX(1, vis_width);
		vis_height = MAX(1, vis_height);

		cat_log_debug("vis[%d,%d], sb[%d, %d]", vis_width, vis_height, sb_width, sb_height);


		if (scrolled_window->vscrollbar_visible) {
			GtkAllocation scrollbar_allocation;
			scrollbar_allocation.x = sb_x+vis_width;
			scrollbar_allocation.y = sb_y+0;
			scrollbar_allocation.width = sb_width;
			scrollbar_allocation.height = vis_height;
			gtk_widget_size_allocate(scrolled_window->vscrollbar, &scrollbar_allocation);
		} else {
			gtk_widget_hide(scrolled_window->vscrollbar);
		}

		if (scrolled_window->hscrollbar_visible) {
			GtkAllocation scrollbar_allocation;
			scrollbar_allocation.x = sb_x+0;
			scrollbar_allocation.y = sb_y+vis_height;
			scrollbar_allocation.width = vis_width;
			scrollbar_allocation.height = sb_height;
			gtk_widget_size_allocate(scrolled_window->hscrollbar, &scrollbar_allocation);
			cat_log_detail("hscrollbar :: allocation[%d,%d,%d,%d]",scrollbar_allocation.x, scrollbar_allocation.y, scrollbar_allocation.width, scrollbar_allocation.height);
		} else {
			gtk_widget_hide(scrolled_window->hscrollbar);
		}

		child_allocation->x = 0;
		child_allocation->y = 0;
		child_allocation->width = vis_width;
		child_allocation->height = vis_height;

		gtk_widget_size_allocate(binChild, child_allocation);

		cat_log_detail("binChild :: allocation[%d,%d,%d,%d]",child_allocation->x, child_allocation->y, child_allocation->width, child_allocation->height);
	} else {
		child_allocation->x = 0;
		child_allocation->y = 0;
		child_allocation->width = 1;
		child_allocation->height = 1;

		gtk_widget_hide(scrolled_window->hscrollbar);
		gtk_widget_hide(scrolled_window->vscrollbar);

		cat_log_debug("child not yet visible");
	}
	cat_log_debug("l_child_size_allocate:end");
}

static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocationIn) {
	cat_log_debug("l_widget_size_allocate:start:%p allocation=[%d,%d,%d,%d]", widget, allocationIn->x, allocationIn->y, allocationIn->width, allocationIn->height);
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);


	GtkAllocation *allocation = g_new(GtkAllocation, 1);
	gtk_widget_get_allocation(widget, allocation);
	if ((allocationIn->x==allocation->x) &&
			(allocationIn->y==allocation->y) &&
			(allocationIn->width==allocation->width) &&
			(allocationIn->height==allocation->height)) {
		return;
	}

	*allocation = *allocationIn;

	int height = gtk_widget_get_allocated_height(widget);
	if (height!=allocation->height) {
		cat_log_debug("height=%d", height);
		height++;
	}


	gint border_width = gtk_container_get_border_width(GTK_CONTAINER(widget));
	gboolean hadjustment_value_changed, vadjustment_value_changed;


	cat_log_detail("allocation[%d,%d,%d,%d]", allocation->x, allocation->y, allocation->width, allocation->height);


	GTK_WIDGET_CLASS(lea_scrolled_window_parent_class)->size_allocate(widget, allocation);

	int vis_width = allocation->width - border_width * 2;
	int vis_height = allocation->height - border_width * 2;

	cat_log_detail("vis_[%d,%d]", vis_width, vis_height);


	GtkRequisition vscrollbar_requisition, hscrollbar_requisition;
	GtkRequisition dummy_requisition;
	gtk_widget_get_preferred_size(scrolled_window->vscrollbar, &dummy_requisition, &vscrollbar_requisition);
	gtk_widget_get_preferred_size(scrolled_window->hscrollbar, &dummy_requisition, &hscrollbar_requisition);

	int unclear_count = 0;
	switch(scrolled_window->vscrollbar_policy) {
		case GTK_POLICY_ALWAYS : {
			scrolled_window->vscrollbar_visible = TRUE;
		} break;
		case GTK_POLICY_NEVER : {
			scrolled_window->vscrollbar_visible = FALSE;
		} break;
		default : {
			if (scrolled_window->view_height<vis_height) {
				scrolled_window->vscrollbar_visible = FALSE;
			} else if (scrolled_window->view_height<(vis_height-hscrollbar_requisition.height)) {
				scrolled_window->vscrollbar_visible = TRUE;
			} else {
				unclear_count++;
			}
		} break;
	}

	switch(scrolled_window->hscrollbar_policy) {
		case GTK_POLICY_ALWAYS : {
			scrolled_window->hscrollbar_visible = TRUE;
		} break;
		case GTK_POLICY_NEVER : {
			scrolled_window->hscrollbar_visible = FALSE;
		} break;
		default : {
			if (scrolled_window->view_width<vis_width) {
				scrolled_window->hscrollbar_visible = FALSE;
			} else if (scrolled_window->view_width<(vis_width-hscrollbar_requisition.width)) {
				scrolled_window->hscrollbar_visible = TRUE;
			} else {
				if (unclear_count==0) {
					scrolled_window->hscrollbar_visible = scrolled_window->vscrollbar_visible;
				} else {
					scrolled_window->vscrollbar_visible = TRUE;
					scrolled_window->hscrollbar_visible = TRUE;
					unclear_count++;
				}
			}
		} break;
	}


	if (unclear_count==1) {
		scrolled_window->vscrollbar_visible = scrolled_window->hscrollbar_visible;
	}

	cat_log_debug("vscrollbar_visible=%d, hscrollbar_visible=%d", scrolled_window->vscrollbar_visible, scrolled_window->hscrollbar_visible);

	GtkAllocation child_allocation;
	child_allocation = *allocationIn;
	l_child_size_allocate(scrolled_window, &child_allocation);

	cat_log_debug("child_allocation=[%d,%d,%d,%d], view_window=%o", child_allocation.x, child_allocation.y, child_allocation.width, child_allocation.height, scrolled_window->view_window);


	if (gtk_widget_get_realized(widget) && scrolled_window->view_window) {
		gdk_window_move_resize(scrolled_window->view_window,
							  allocation->x + border_width, allocation->y + border_width,
							  child_allocation.width, child_allocation.height);

		cat_log_debug("view_window :: =[%d,%d,%d,%d]", allocation->x + border_width, allocation->y + border_width, child_allocation.width, child_allocation.height);

	} else {
		cat_log_debug("not yet realized");
	}



	l_set_hadjustment_values (scrolled_window, &hadjustment_value_changed, &child_allocation);
	l_set_vadjustment_values (scrolled_window, &vadjustment_value_changed, &child_allocation);

	GtkAdjustment *hadjustment = GTK_ADJUSTMENT(scrolled_window->hadjustment);
	GtkAdjustment *vadjustment = GTK_ADJUSTMENT(scrolled_window->vadjustment);

	cat_log_debug("l_widget_size_allocate:end:");
	cat_log_on_debug({
		_dump_adjustment(vadjustment);
	});
	g_free(allocation);
}



static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cairo) {
	cat_log_debug("l_widget_draw:start");

	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);
	GtkBin *bin;
	bin = GTK_BIN (widget);

	cairo_set_source_rgb(cairo, 0, 0, 1);

	if (gtk_widget_is_drawable(widget)) {
		GtkWidget *childWidget = gtk_bin_get_child(bin);

		if (gtk_cairo_should_draw_window(cairo, scrolled_window->parent_window)) {

			GtkWidget *vwidget = scrolled_window->vscrollbar;
			if (vwidget) {
				cairo_save(cairo);
				gtk_container_propagate_draw(GTK_CONTAINER(widget), vwidget, cairo);
				cairo_restore(cairo);
			}

			GtkWidget *hwidget = scrolled_window->hscrollbar;
			if (hwidget) {
				cairo_save(cairo);
				gtk_container_propagate_draw(GTK_CONTAINER(widget), hwidget, cairo);
				cairo_restore(cairo);
			}
		}


		if (gtk_widget_is_drawable(childWidget) && gtk_widget_is_visible(childWidget)) {
		      cairo_save(cairo);

		      gtk_cairo_transform_to_window(cairo, childWidget, gtk_widget_get_window(childWidget));

		      gtk_container_propagate_draw(GTK_CONTAINER(widget), childWidget, cairo);

		      cairo_restore(cairo);
		}

	} else {
		cat_log_debug("l_widget_draw:not drawable");
	}

	cat_log_debug("l_widget_draw:end");
	return FALSE;
}



/**
 * THIS FUNCTION IS COPIED FROM GTKRANGE.C
 *
 * _gtk_range_get_wheel_delta:
 * @range: a #GtkRange
 * @direction: A #GdkScrollDirection
 *
 * Returns a good step value for the mouse wheel.
 *
 * Return value: A good step value for the mouse wheel.
 *
 * Since: 2.4
 **/



gdouble
l_copied_gtk_range_get_wheel_delta (GtkRange       *range,
                            GdkEventScroll *event)
{
  GtkAdjustment *adjustment = gtk_range_get_adjustment(range);
  gdouble dx, dy;
  gdouble delta;
  gdouble page_size;
  gdouble page_increment;
  gdouble scroll_unit;

  page_size = gtk_adjustment_get_page_size (adjustment);
  page_increment = gtk_adjustment_get_page_increment (adjustment);

  if (GTK_IS_SCROLLBAR (range))
    scroll_unit = pow (page_size, 2.0 / 3.0);
  else
    scroll_unit = page_increment;

  if (gdk_event_get_scroll_deltas ((GdkEvent *) event, &dx, &dy))
    {
      if (dx != 0 &&
          gtk_orientable_get_orientation (GTK_ORIENTABLE (range)) == GTK_ORIENTATION_HORIZONTAL)
        delta = dx * scroll_unit;
      else
        delta = dy * scroll_unit;
    }
  else
    {
      if (event->direction == GDK_SCROLL_UP ||
          event->direction == GDK_SCROLL_LEFT)
        delta = - scroll_unit;
      else
        delta = scroll_unit;
    }

  if (gtk_range_get_inverted(range)) {
    delta = - delta;
  }

  return delta;
}


static gint l_widget_scroll_event(GtkWidget *widget, GdkEventScroll *event) {
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(widget);
	GtkWidget *range;

	cat_log_debug("wdgt->window=%p, bin_window=%p, view_window=%p", gtk_widget_get_window(widget), scrolled_window->bin_window, scrolled_window->view_window);
	cat_log_debug("scroll event:window=%p, direction=%d", event->window, event->direction);
	g_return_val_if_fail (LEA_IS_SCROLLED_WINDOW(widget), FALSE);
	g_return_val_if_fail (event != NULL, FALSE);

	if (event->direction == GDK_SCROLL_SMOOTH) {
		return FALSE;
	}



	if (event->direction == GDK_SCROLL_UP || event->direction == GDK_SCROLL_DOWN) {
		range = scrolled_window->vscrollbar;
	} else {
		range = scrolled_window->hscrollbar;
	}

	if (range && gtk_widget_get_visible(range)) {
		GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(range));
		gdouble delta, new_value;

		delta = l_copied_gtk_range_get_wheel_delta(GTK_RANGE (range), event);
		new_value = CLAMP (gtk_adjustment_get_value(adj) + delta, gtk_adjustment_get_lower(adj), gtk_adjustment_get_upper(adj)- gtk_adjustment_get_page_size(adj));
		gtk_adjustment_set_value(adj, new_value);
		cat_log_debug("scroll event returns true");
		return TRUE;
	}
	return FALSE;
}


static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW (object);

	switch (prop_id) {
		case PROP_HADJUSTMENT:
			break;
		case PROP_VADJUSTMENT:
			break;
		case PROP_HSCROLLBAR_POLICY:
			lea_scrolled_window_set_policy(scrolled_window, g_value_get_enum(value), scrolled_window->vscrollbar_policy);
			break;
		case PROP_VSCROLLBAR_POLICY:
			lea_scrolled_window_set_policy(scrolled_window, scrolled_window->hscrollbar_policy, g_value_get_enum(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void l_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	LeaScrolledWindow *scrolled_window = LEA_SCROLLED_WINDOW(object);

	switch (prop_id) {
		case PROP_HADJUSTMENT:
			g_value_set_object(value, G_OBJECT(scrolled_window->hadjustment));
			break;
		case PROP_VADJUSTMENT:
			g_value_set_object(value, G_OBJECT(scrolled_window->vadjustment));
			break;
		case PROP_HSCROLLBAR_POLICY:
			g_value_set_enum(value, scrolled_window->hscrollbar_policy);
			break;
		case PROP_VSCROLLBAR_POLICY:
			g_value_set_enum(value, scrolled_window->vscrollbar_policy);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

