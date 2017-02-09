/*
   File:    leadraglayer.c
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

#include "lealayerprivate.h"
#include "leadraglayer.h"
#include <gdk/gdk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDragLayer"
#include <logging/catlog.h>

struct _LeaDragLayerPrivate {
	GdkWindow *window;
	LeaIGrip *drag_grip;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaDragLayer, lea_drag_layer, LEA_TYPE_LAYER,
		G_ADD_PRIVATE(LeaDragLayer)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_widget_realize(GtkWidget *widget);
static void l_widget_unrealize(GtkWidget *widget);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);

static void lea_drag_layer_class_init(LeaDragLayerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->realize = l_widget_realize;
	widget_class->unrealize = l_widget_unrealize;
	widget_class->size_allocate = l_widget_size_allocate;
}

static void lea_drag_layer_init(LeaDragLayer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaDragLayer *instance = LEA_DRAG_LAYER(object);
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(instance);
	cat_unref_ptr(priv->drag_grip);
	G_OBJECT_CLASS(lea_drag_layer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_drag_layer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *eev, gpointer data);
static gboolean l_button_release(GtkWidget *widget, GdkEventButton *eev, gpointer data);


LeaDragLayer *lea_drag_layer_new() {
	LeaDragLayer *result = g_object_new(LEA_TYPE_DRAG_LAYER, NULL);
	cat_ref_anounce(result);
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(result);
	lea_layer_construct((LeaLayer *) result);
//	gtk_widget_set_has_window(GTK_WIDGET(result), TRUE);
	priv->window = NULL;
	priv->drag_grip = NULL;

	GtkWidget *widget = GTK_WIDGET(result);
	gtk_widget_set_events(widget, gtk_widget_get_events(widget)
			 | GDK_BUTTON_RELEASE_MASK
			 | GDK_BUTTON_PRESS_MASK
			 | GDK_POINTER_MOTION_MASK
			 | GDK_POINTER_MOTION_HINT_MASK);

	g_signal_connect(result, "button-release-event", G_CALLBACK(l_button_release), G_OBJECT(result));
	g_signal_connect(widget, "motion-notify-event", G_CALLBACK(l_mouse_motion), G_OBJECT(result));

	return result;
}

void lea_drag_layer_set_active_grip(LeaDragLayer *drag_layer, LeaIGrip *active_grip) {
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(drag_layer);
	if (priv->drag_grip == active_grip) {
		return;
	}

	if (priv->drag_grip) {
		lea_igrip_remove_notify(priv->drag_grip);
	}

	cat_ref_swap(priv->drag_grip, active_grip);
	cat_log_debug("active_grip=%o", active_grip);
	if (active_grip) {
		lea_layer_add_drawable(LEA_LAYER(drag_layer), LEA_IDRAWABLE(active_grip));
		gdk_window_show(priv->window);
	} else {
		gdk_window_hide(priv->window);
	}
}

gboolean lea_drag_layer_is_active(LeaDragLayer *drag_layer) {
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(drag_layer);
	return priv->drag_grip != NULL;
}

static void l_rewrite_events_translate(LeaDragLayer *drag_layer, GdkWindow *old_window, GdkWindow *new_window, gdouble *x, gdouble *y) {
	cat_log_debug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 rewrting");
	gint old_origin_x, old_origin_y;
	gint new_origin_x, new_origin_y;

	gdk_window_get_origin(old_window, &old_origin_x, &old_origin_y);
	gdk_window_get_origin(new_window, &new_origin_x, &new_origin_y);

	GtkAllocation alloc;
	gtk_widget_get_allocation(GTK_WIDGET(drag_layer), &alloc);

	*x += (old_origin_x - new_origin_x) + alloc.x;
	*y += (old_origin_y - new_origin_y) + alloc.y;
}


void lea_drag_layer_rewrite_event(LeaDragLayer *drag_layer, GdkEvent *event) {
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(drag_layer);
	if (priv->drag_grip==NULL) {
		return;
	}
	switch(event->type) {
		case GDK_MOTION_NOTIFY : {
			GdkEventMotion *motion_event = (GdkEventMotion *) event;
			if (motion_event->window == priv->window) {
				return;
			}
			l_rewrite_events_translate(drag_layer, motion_event->window, priv->window, &(motion_event->x), &(motion_event->y));
			cat_ref_swap(motion_event->window, priv->window);
		} break;
		case GDK_BUTTON_RELEASE :
		case GDK_BUTTON_PRESS : {
			GdkEventButton *button_event = (GdkEventButton *) event;
			if (button_event->window == priv->window) {
				return;
			}
			l_rewrite_events_translate(drag_layer, button_event->window, priv->window, &(button_event->x), &(button_event->y));
			cat_ref_swap(button_event->window, priv->window);
		} break;
		default :
			break;
	}
}

static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *eev, gpointer data) {
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(LEA_DRAG_LAYER(data));
	gboolean result = FALSE;
	if (priv->drag_grip) {
		lea_igrip_motion_event(priv->drag_grip, eev);
		result = TRUE;
	}
	return result;
}

static gboolean l_button_release(GtkWidget *widget, GdkEventButton *eev, gpointer data) {
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(LEA_DRAG_LAYER(data));
	gboolean result = FALSE;
	if (priv->drag_grip) {
		LeaLayer *layer = LEA_LAYER(data);
		lea_igrip_release_event(priv->drag_grip, eev);
		lea_layer_remove_drawable(layer, LEA_IDRAWABLE(priv->drag_grip));
		lea_drag_layer_set_active_grip((LeaDragLayer *) layer, NULL);
		result = TRUE;
	}
	return result;
}



static void l_widget_realize(GtkWidget *widget) {
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(LEA_DRAG_LAYER(widget));
	cat_log_debug("realizing");
	gtk_widget_set_realized(widget, TRUE);

	GdkWindowAttr attributes = {0};
	attributes.window_type = GDK_WINDOW_TEMP;
	GtkAllocation walloc;
	gtk_widget_get_allocation(widget, &walloc);
	attributes.x = walloc.x;
	attributes.y = walloc.y;
	attributes.width = walloc.width;
	attributes.height = walloc.height;
	attributes.wclass = GDK_INPUT_ONLY ;
	attributes.event_mask =  GDK_BUTTON_PRESS_MASK |
							GDK_BUTTON_RELEASE_MASK |
							GDK_ENTER_NOTIFY_MASK |
							GDK_POINTER_MOTION_MASK |
							GDK_LEAVE_NOTIFY_MASK;

	gint attributes_mask = GDK_WA_X | GDK_WA_Y;

	GdkScreen *screen = gtk_widget_get_screen(widget);
	GdkWindow *parent_window = gtk_widget_get_parent_window(widget);
	priv->window = gdk_window_new(parent_window, &attributes, attributes_mask);
	gtk_widget_set_window(widget, parent_window);
	cat_ref_ptr(parent_window);
	gtk_widget_register_window(widget, priv->window);
}



static void l_widget_unrealize(GtkWidget *widget) {
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(LEA_DRAG_LAYER(widget));
	cat_log_debug("unrealize");

	GdkWindow *actual_parent_window = gtk_widget_get_window(widget);
	if (actual_parent_window) {
		gtk_widget_set_window(widget, NULL);
		cat_unref_ptr(actual_parent_window);
	}

	if (priv->window) {
		cat_ref_ptr(priv->window);
		gtk_widget_unregister_window (widget, priv->window);
		gdk_window_destroy(priv->window);
		cat_unref(priv->window);
		priv->window = NULL;
	}
	gtk_widget_set_realized(widget, FALSE);
}

static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	GTK_WIDGET_CLASS(lea_drag_layer_parent_class)->size_allocate(widget, allocation);
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(LEA_DRAG_LAYER(widget));
	if (priv->window) {
		gdk_window_move_resize(priv->window,
									allocation->x, allocation->y,
									allocation->width, allocation->height);
	}
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaDragLayer *instance = LEA_DRAG_LAYER(self);
	LeaDragLayerPrivate *priv = lea_drag_layer_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p grip=%o]", iname, self, priv->drag_grip);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
