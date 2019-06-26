/*
   File:    leatoggleimage.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jun 21, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#include "leatoggleimage.h"
#include "../action/leaiattachable.h"
#include "../action/leaaction.h"

#define SIZE   20

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "LeaToggleImage"
#include <logging/catlog.h>

struct _LeaToggleImagePrivate {
	LeaAction *action;
	cairo_surface_t *surface;
	cairo_surface_t *surface_scaled;
	int w_width, w_height;
	gboolean hovering;
	gboolean pressed;
	gboolean is_toggled;
};

static void l_attachable_iface_init(LeaIAttachableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaToggleImage, lea_toggle_image, GTK_TYPE_WIDGET,  // @suppress("Unused static function")
	G_ADD_PRIVATE(LeaToggleImage)
	G_IMPLEMENT_INTERFACE(LEA_TYPE_IATTACHABLE, l_attachable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_widget_realize(GtkWidget *widget);
//static void l_widget_unrealize(GtkWidget *widget);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr);
static void l_widget_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width);
static void l_widget_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height);
static void l_widget_get_preferred_height_for_width(GtkWidget *widget, gint width, gint *minimum_height, gint *natural_height);
static void l_widget_get_preferred_width_for_height(GtkWidget *widget, gint height, gint *minimum_width, gint *natural_width);
static gboolean l_button_release(GtkWidget *widget, GdkEventButton *eev, gpointer data);
static gboolean l_button_press(GtkWidget *widget, GdkEventButton *eev, gpointer data);
static gboolean l_leave_notify(GtkWidget *widget, GdkEventCrossing *eev, gpointer data);
static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *eev, GObject *data);


static void lea_toggle_image_class_init(LeaToggleImageClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkWidgetClass *wdgt_class = GTK_WIDGET_CLASS(clazz);
	wdgt_class->realize = l_widget_realize;
//	wdgt_class->unrealize = l_widget_unrealize;
	wdgt_class->size_allocate = l_widget_size_allocate;
	wdgt_class->draw = l_widget_draw;
	wdgt_class->get_preferred_width = l_widget_get_preferred_width;
	wdgt_class->get_preferred_height = l_widget_get_preferred_height;
	wdgt_class->get_preferred_height_for_width = l_widget_get_preferred_height_for_width;
	wdgt_class->get_preferred_width_for_height = l_widget_get_preferred_width_for_height;

}

static void lea_toggle_image_init(LeaToggleImage *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaToggleImage *instance = LEA_TOGGLE_IMAGE(object);
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(instance);
	if (priv->action) {
		lea_action_detach(priv->action, (LeaIAttachable *) instance);
		cat_unref_ptr(priv->action);
	}
	G_OBJECT_CLASS(lea_toggle_image_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_toggle_image_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

LeaToggleImage *lea_toggle_image_new(LeaAction *action, const char *image_file_name) {
	LeaToggleImage *result = g_object_new(LEA_TYPE_TOGGLE_IMAGE, NULL);
	cat_ref_anounce(result);
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(result);
	priv->action = cat_ref_ptr(action);
	priv->surface = cairo_image_surface_create_from_png(image_file_name /*"/home/dvos/natpad-workspace/natpad/natpad/dist/icons/line_wrap_96.png"*/);

	gtk_widget_set_events((GtkWidget *) result, gtk_widget_get_events((GtkWidget *) result)
												 | GDK_BUTTON_PRESS_MASK
												 | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_POINTER_MOTION_MASK
	                                             | GDK_POINTER_MOTION_HINT_MASK);

	g_signal_connect(result, "button-press-event", G_CALLBACK(l_button_press), G_OBJECT(result));
	g_signal_connect(result, "button-release-event", G_CALLBACK(l_button_release), G_OBJECT(result));
	g_signal_connect(result, "motion-notify-event", G_CALLBACK(l_mouse_motion), G_OBJECT(result));
	g_signal_connect(result, "leave-notify-event", G_CALLBACK(l_leave_notify), G_OBJECT(result));


	lea_action_attach(action, (LeaIAttachable *) result);

	cat_log_error("priv->surface=%p", priv->surface);
	return result;
}

static gboolean l_button_release(GtkWidget *widget, GdkEventButton *eev, gpointer data) {
	if (!gtk_widget_get_realized(widget)) {
		return TRUE;
	}
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
	priv->pressed = FALSE;
//	priv->is_toggled = !priv->is_toggled;
	lea_action_set_toggled(priv->action, !priv->is_toggled);
	LEA_ACTION_GET_CLASS(priv->action)->action_run(priv->action);
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean l_button_press(GtkWidget *widget, GdkEventButton *eev, gpointer data) {
	if (!gtk_widget_get_realized(widget)) {
		return TRUE;
	}
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
	priv->pressed = TRUE;
	gtk_widget_queue_draw(widget);
	return TRUE;
}


static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *eev, GObject *data) {
	cat_log_detail("l_mouse_motion");
	if (!gtk_widget_get_realized(widget)) {
		cat_log_trace("widget not realized");
		return TRUE;
	}
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
	int wmx = (int) eev->x;
	int wmy = (int) eev->y;
	priv->hovering = TRUE;
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean l_leave_notify(GtkWidget *widget, GdkEventCrossing *eev, gpointer data) {
	if (!gtk_widget_get_realized(widget)) {
		return TRUE;
	}
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
	priv->hovering = FALSE;
	priv->pressed = FALSE;
	gtk_widget_queue_draw(widget);
	return TRUE;
}


static void l_widget_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width) {
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
//	int width = cairo_image_surface_get_width(priv->surface);
	int width = SIZE;
	cat_log_error("width=%d", width);
	*minimum_width = SIZE;
	*natural_width = width;
}

static void l_widget_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height) {
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
//	int height = cairo_image_surface_get_height(priv->surface);
	int height = SIZE;
	cat_log_error("height=%d", height);
	*minimum_height = SIZE;
	*natural_height = height;
}

static void l_widget_get_preferred_height_for_width(GtkWidget *widget, gint width, gint *minimum_height, gint *natural_height) {
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
	int height = cairo_image_surface_get_height(priv->surface);
	cat_log_error("height=%d", height);
	*minimum_height = width;
	*natural_height = width;
}

static void l_widget_get_preferred_width_for_height(GtkWidget *widget, gint height, gint *minimum_width, gint *natural_width) {
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
	int width = cairo_image_surface_get_width(priv->surface);
	cat_log_error("width=%d", width);
	*minimum_width = height;
	*natural_width = height;
}




static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	gtk_widget_set_allocation (widget, allocation);

	if (gtk_widget_get_realized(widget)) {

		if (gtk_widget_get_has_window (widget)) {
			gdk_window_move_resize (gtk_widget_get_window (widget),
					allocation->x,
					allocation->y,
					allocation->width,
					allocation->height);
		}
	}
}


static void l_widget_realize(GtkWidget *widget) {
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	gtk_widget_set_realized(widget, TRUE);

	GdkWindowAttr attributes;
	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.event_mask = gtk_widget_get_events (widget)
	                      | GDK_BUTTON_MOTION_MASK
	                      | GDK_BUTTON_PRESS_MASK
	                      | GDK_BUTTON_RELEASE_MASK
	                      | GDK_EXPOSURE_MASK
	                      | GDK_ENTER_NOTIFY_MASK
	                      | GDK_LEAVE_NOTIFY_MASK;
	attributes.visual = gtk_widget_get_visual (widget);
	attributes.wclass = GDK_INPUT_OUTPUT;
	gint attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

	GdkWindow *window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
	gtk_widget_set_window(widget, window);
	gtk_widget_register_window(widget, window);
}

//static void l_widget_unrealize(GtkWidget *widget) {
//
//}

static cairo_surface_t *l_scale_surface(cairo_surface_t *surface, int width, int height) {
	cairo_format_t format = cairo_image_surface_get_format(surface);
	cairo_surface_t *result = cairo_surface_create_similar_image(surface, format, width, height);

	int src_width = cairo_image_surface_get_width(surface);
	int src_height = cairo_image_surface_get_height(surface);

	double w_scale = (double) width/src_width;
	double h_scale = (double) height/src_height;


	double scale;
	if (w_scale<h_scale) {
		scale = w_scale;
	} else {
		scale = h_scale;
	}
	cat_log_error("w_scale=%f, h_scale=%f, scale=%f", w_scale, h_scale, scale);

	cairo_t *cr = cairo_create(result);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_scale(cr, scale, scale);
	cairo_set_source_surface(cr, surface, 0,0);
	cairo_paint(cr);

	cairo_destroy(cr);
	return result;
}

static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr) {
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(widget));
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	if (allocation.width!=priv->w_height || allocation.height!=priv->w_height || priv->surface_scaled == NULL) {
		if (priv->surface_scaled) {
			cairo_surface_destroy(priv->surface_scaled);
			priv->surface_scaled = NULL;
		}
		priv->surface_scaled = l_scale_surface(priv->surface, allocation.width, allocation.height);
		priv->w_height = allocation.width;
		priv->w_height = allocation.height;
	}
	int w = allocation.width-1;
	int h = allocation.height-1;


	if (priv->pressed || priv->is_toggled) {
		cairo_set_source_rgb(cr, 0.62,0.62,0.62);
		cairo_rectangle(cr, 0,0, w, h);
		cairo_fill(cr);
	}

	cairo_set_source_surface(cr, priv->surface_scaled,0,0);
	cairo_paint(cr);

	if (priv->hovering) {
		cairo_set_line_width(cr, 1);
		cairo_set_source_rgb(cr, 1,1,1);
		cairo_rectangle(cr, 0.5,0.5, w, h);
		cairo_stroke(cr);
		cairo_set_source_rgb(cr, 0.3,0.3,0.3);
		cairo_rectangle(cr, 1.5,1.5, w-2, h-2);
		cairo_stroke(cr);
	}
	return FALSE;
}





static void l_attachable_sensitivity_set(LeaIAttachable *self, gboolean flag) {
	gtk_widget_set_sensitive(GTK_WIDGET(self), flag);
}

static void l_attachable_toggled(LeaIAttachable *self, gboolean toggled) {
	LeaToggleImagePrivate *priv = lea_toggle_image_get_instance_private(LEA_TOGGLE_IMAGE(self));
	priv->is_toggled = toggled;
	if (gtk_widget_get_realized((GtkWidget *) self)) {
		gtk_widget_queue_draw((GtkWidget *) self);
	}
}


static void l_attachable_iface_init(LeaIAttachableInterface *iface) {
	iface->sensitivitySet = l_attachable_sensitivity_set;
	iface->toggled = l_attachable_toggled;
//	iface->toggled =
}
