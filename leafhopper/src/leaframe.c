/*
   File:    leaframe2.c
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

#include "leaframe.h"
#include "surface/leaidrawable.h"
#include "surface/lealayer.h"
#include "surface/leasurfacehandlerprivate.h"
#include "surface/drag/leaigrip.h"
#include "key/leakeyhandler.h"
#include "action/leaactiongroupref.h"
#include "menu/leamenuaction.h"
#include "layout/leaipanelowner.h"
#include "layout/leapanelfocusevent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaFrame"
#include <logging/catlog.h>

struct _LeaFramePrivate {
	LeaLayer *bottom_layer;
	LeaDragLayer *top_layer;
	CatArrayWo *a_children;
	LeaSurfaceHandler *service_handler;
	LeaIGrip *current_grip;
	GdkWindow *window;

	gulong event_signal;
	LeaKeyHandler *key_handler;
	LeaActionGroupRef *root_action_group;
	LeaMenuAction *root_menu_action;
	GtkWidget *root_menu_widget;
	LeaStatusBar *status_bar;

	gboolean focus_active;
	LeaPanel *focus_panel;

	CatWeakList *listeners;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaFrame, lea_frame, GTK_TYPE_CONTAINER,
		G_ADD_PRIVATE(LeaFrame)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static GType l_container_child_type(GtkContainer *container);
static void l_container_add(GtkContainer *container, GtkWidget *child);
static void l_container_remove(GtkContainer *container, GtkWidget *child);
static void l_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);
static GtkWidgetPath *l_container_path_for_child(GtkContainer *container, GtkWidget *child);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void l_widget_realize(GtkWidget *widget);
static void l_widget_unrealize(GtkWidget *widget);
static void l_widget_map(GtkWidget *widget);
static void l_widget_unmap(GtkWidget *widget);
static void l_widget_destroy(GtkWidget *widget);
static gboolean l_widget_button_release_event(GtkWidget *widget, GdkEventButton *event);
static gboolean l_widget_key_press_event(GtkWidget *widget, GdkEventKey *event);
static gboolean l_widget_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);


static void lea_frame_class_init(LeaFrameClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkContainerClass *container_class = GTK_CONTAINER_CLASS(clazz);
	container_class->child_type = l_container_child_type;
	container_class->add = l_container_add;
	container_class->remove = l_container_remove;
	container_class->forall = l_container_forall;
	container_class->get_path_for_child = l_container_path_for_child;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->realize = l_widget_realize;
	widget_class->unrealize = l_widget_unrealize;
	widget_class->map = l_widget_map;
	widget_class->unmap = l_widget_unmap;
	widget_class->destroy = l_widget_destroy;
	widget_class->size_allocate = l_widget_size_allocate;

	widget_class->button_release_event = l_widget_button_release_event;
	widget_class->key_press_event = l_widget_key_press_event;
}

static void lea_frame_init(LeaFrame *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaFrame *instance = LEA_FRAME(object);
	LeaFramePrivate *priv = lea_frame_get_instance_private(instance);
	cat_unref_ptr(priv->bottom_layer);
	cat_unref_ptr(priv->top_layer);
	cat_unref_ptr(priv->current_grip);
	cat_unref_ptr(priv->focus_panel);
	cat_unref_ptr(priv->key_handler);
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->root_action_group);
	cat_unref_ptr(priv->root_menu_action);
	cat_unref_ptr(priv->status_bar);
	cat_unref_ptr(priv->service_handler);
	cat_unref_ptr(priv->a_children);
	G_OBJECT_CLASS(lea_frame_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_frame_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *eev, gpointer data);
static gboolean l_button_press(GtkWidget *widget, GdkEventButton *eev, gpointer data);
static gboolean l_leave_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer data);
static void l_gdk_event_handler(GdkEvent *event, gpointer data);


LeaFrame *lea_frame_new() {
	LeaFrame *result = g_object_new(LEA_TYPE_FRAME, NULL);
	cat_ref_anounce(result);
	LeaFramePrivate *priv = lea_frame_get_instance_private(result);
	priv->a_children = cat_array_wo_anchor(cat_array_wo_new(), 0);
	priv->bottom_layer = NULL;
	priv->service_handler = lea_surface_handler_new();
	priv->listeners = cat_weak_list_new();
	priv->current_grip = NULL;
	priv->window = NULL;
	priv->event_signal = 0;

	priv->focus_active = TRUE;
	priv->focus_panel = NULL;

	priv->key_handler = lea_key_handler_new();
	priv->top_layer = lea_drag_layer_new();
	priv->status_bar = lea_status_bar_new();
	priv->root_action_group = lea_action_group_ref_new(NULL);
	priv->root_menu_action = lea_menu_action_new((LeaAction *) priv->root_action_group, LEA_MENU_TYPE_BAR);
	priv->root_menu_widget = GTK_WIDGET(lea_menu_action_get_menu_shell(priv->root_menu_action));

	gtk_widget_set_parent(GTK_WIDGET(priv->top_layer), GTK_WIDGET(result));
	gtk_widget_show(GTK_WIDGET(priv->top_layer));
	gtk_widget_set_parent(GTK_WIDGET(priv->status_bar), GTK_WIDGET(result));
	gtk_widget_show(GTK_WIDGET(priv->status_bar));
	gtk_widget_set_parent(priv->root_menu_widget, GTK_WIDGET(result));
	cat_ref_ptr(GTK_WIDGET(priv->root_menu_widget));
	gtk_widget_show(GTK_WIDGET(priv->root_menu_widget));

	GtkWidget *widget = GTK_WIDGET(result);
	gtk_widget_set_events(widget, gtk_widget_get_events(widget)
												 | GDK_BUTTON_PRESS_MASK
												 | GDK_BUTTON_RELEASE_MASK
												 | GDK_LEAVE_NOTIFY_MASK
	                                             | GDK_POINTER_MOTION_MASK
	                                             | GDK_POINTER_MOTION_HINT_MASK);



	g_signal_connect(widget, "motion-notify-event", G_CALLBACK(l_mouse_motion), G_OBJECT(result));
	g_signal_connect(widget, "button-press-event", G_CALLBACK(l_button_press), G_OBJECT(result));
	g_signal_connect(widget, "leave-notify-event", G_CALLBACK(l_leave_notify), G_OBJECT(result));



	cat_log_debug("setting event-handler");
	gdk_event_handler_set((GdkEventFunc) l_gdk_event_handler, result, NULL);

	return result;
}

LeaSurfaceHandler *lea_frame_get_surface_handler(LeaFrame *frame) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	return priv->service_handler;
}

LeaDragLayer *lea_frame_get_drag_layer(LeaFrame *frame) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	return priv->top_layer;
}


void lea_frame_merge_action_group(LeaFrame *frame, LeaActionGroup *action_group) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	if (priv->root_action_group==NULL) {
		return;
	}
	cat_log_debug("merging group: action_group=%o", action_group);
	lea_action_group_ref_merge(priv->root_action_group, action_group);
	lea_frame_update_menu(frame);
	lea_frame_update_key_bindings(frame);
}

void lea_frame_unmerge_action_group(LeaFrame *frame, LeaActionGroup *action_group) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	if (priv->root_action_group==NULL) {
		return;
	}
	cat_log_debug("unmerging group: action_group=%o", action_group);
	lea_action_group_ref_unmerge(priv->root_action_group, action_group);
	lea_frame_update_menu(frame);
	lea_frame_update_key_bindings(frame);
}

void lea_frame_update_menu(LeaFrame *frame){
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	if (priv->root_menu_action) {
		lea_menu_action_update(priv->root_menu_action, NULL);
	}
}

void lea_frame_update_key_bindings(LeaFrame *frame) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	lea_key_handler_update(priv->key_handler, (LeaActionGroup *) priv->root_action_group);
}


LeaStatusBar *lea_frame_get_status_bar(LeaFrame *frame) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	return priv->status_bar;
}


LeaSurface *lea_frame_get_focus_surface(LeaFrame *frame) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	LeaSurface *result = NULL;
	if (priv->focus_panel) {
		GtkWidget *par_w = gtk_widget_get_parent(GTK_WIDGET(priv->focus_panel));
		if (LEA_IS_SURFACE(par_w)) {
			result = LEA_SURFACE(par_w);
		}
	}
	return result;
}

gboolean lea_frame_get_focus_active(LeaFrame *frame) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	return priv->focus_active;

}


void lea_frame_add_event_listener(LeaFrame *frame, LeaIEventListener *listener) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
}

void lea_frame_remove_event_listener(LeaFrame *frame, LeaIEventListener *listener) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	cat_weak_list_remove(priv->listeners, (GObject *) listener);
}

void lea_frame_dispatch_event(LeaFrame *frame, LeaEvent *event) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		LeaIEventListener *listener = (LeaIEventListener *) cat_iiterator_next(iter);
		lea_ievent_listener_event(listener, event);
	}
	cat_unref_ptr(iter);
}


static void l_gdk_event_handler(GdkEvent *event, gpointer data) {
	cat_log_trace("event->type=%d", event->type);
	switch(event->type) {
		case GDK_BUTTON_RELEASE :
		case GDK_BUTTON_PRESS :
		case GDK_MOTION_NOTIFY : {
			LeaFrame *frame = LEA_FRAME(data);
			LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
			lea_drag_layer_rewrite_event(priv->top_layer, event);
		} break;
		case GDK_EXPOSE : {
			LeaFrame *frame = LEA_FRAME(data);
			LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
			if (priv->top_layer) {
				if (lea_layer_has_drawables((LeaLayer *) priv->top_layer)) {
					gtk_main_do_event(event);
					GdkEventExpose *evt_exp = (GdkEventExpose *) event;
					GdkWindow *frame_window = gtk_widget_get_window((GtkWidget *) frame);
					if (evt_exp->window!=frame_window) {
						cairo_t *cr = gdk_cairo_create(frame_window);
						GtkAllocation alloc;
						gtk_widget_get_allocation((GtkWidget *) priv->top_layer, &alloc);
						cairo_translate(cr, alloc.x, alloc.y);
						gtk_widget_draw((GtkWidget *) priv->top_layer, cr);
						cairo_destroy(cr);
					}
					return;
				}
			}
		} break;
		default : break;
	}
	gtk_main_do_event(event);
}



static GType l_container_child_type(GtkContainer *container) {
	return GTK_TYPE_WIDGET;
}

static void l_container_add(GtkContainer *container, GtkWidget *child) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(container));
	CatArrayWo *e_children = cat_array_wo_create_editable(priv->a_children);
	cat_array_wo_append(e_children, (GObject *) child);
	e_children = cat_array_wo_anchor(e_children, 0);
	cat_ref_swap(priv->a_children, e_children);

	gtk_widget_set_parent(child, GTK_WIDGET(container));
	cat_unref_ptr(e_children);

	gtk_widget_set_child_visible(child, TRUE);
}

static void l_container_remove(GtkContainer *container, GtkWidget *child) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(container));
	if (priv->root_menu_widget==child) {
		priv->root_menu_widget = NULL;
		gtk_widget_unparent(child);
		gtk_widget_queue_resize(GTK_WIDGET(container));
	} else if ((GtkWidget*) priv->status_bar==child) {
		priv->status_bar = NULL;
		gtk_widget_unparent(child);
		cat_unref_ptr(child);
		gtk_widget_queue_resize(GTK_WIDGET(container));
	} else if ((GtkWidget*) priv->top_layer==child) {
		priv->top_layer = NULL;
		gtk_widget_unparent(child);
		cat_unref_ptr(child);
		gtk_widget_queue_resize(GTK_WIDGET(container));
	} else if (priv->a_children) {
		CatArrayWo *e_children = cat_array_wo_create_editable(priv->a_children);
		int index = cat_array_wo_find_index(e_children, child, -1);
		if (index>=0) {
			cat_ref(child);
			cat_array_wo_remove(e_children, index, NULL);
			e_children = cat_array_wo_anchor(e_children, 0);
			cat_ref_swap(priv->a_children, e_children);

			gboolean was_visible = gtk_widget_get_visible(child);
			gtk_widget_unparent(child);
			if (was_visible) {
				gtk_widget_queue_resize(GTK_WIDGET(container));
			}
			cat_unref(child);
		}

		cat_unref_ptr(e_children);
	}
}

static void l_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(container));
	if (priv->a_children) {
		CatIIterator *iter = cat_array_wo_iterator(priv->a_children);
		while(cat_iiterator_has_next(iter)) {
			GtkWidget *widget = (GtkWidget *) cat_iiterator_next(iter);
			(* callback)(widget, callback_data);

			cat_log_debug("child-rc=%p - %d", G_OBJECT(widget), G_OBJECT(widget)->ref_count);

		}
		cat_unref_ptr(iter);
	}

	if (priv->top_layer) {
		(* callback)(GTK_WIDGET(priv->top_layer), callback_data);
	}
	if (priv->status_bar) {
		(* callback)(GTK_WIDGET(priv->status_bar), callback_data);
	}
	if (priv->root_menu_widget) {
		(*callback)(priv->root_menu_widget, callback_data);
	}
}


static GtkWidgetPath *l_container_path_for_child(GtkContainer *container, GtkWidget *child) {
	GtkWidgetPath *path = gtk_widget_get_path(GTK_WIDGET(container));
	path = gtk_widget_path_copy(path);
	gtk_widget_path_append_for_widget (path, child);
	return path;
}




static void l_widget_realize(GtkWidget *widget) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(widget));

	gtk_widget_set_realized (widget, TRUE);

	GtkAllocation allocation;
	gtk_widget_get_allocation (widget, &allocation);

	GdkWindowAttr attributes = { 0 };
	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual(widget);
	attributes.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;
	GdkWindowAttributesType attributes_mask = (GDK_WA_X | GDK_WA_Y) | GDK_WA_VISUAL;

	priv->window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
	gtk_widget_set_window (widget, priv->window);
	gtk_widget_register_window (widget, priv->window);
}

static void l_widget_unrealize(GtkWidget *widget) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(widget));

    gtk_container_forall(GTK_CONTAINER (widget), (GtkCallback) gtk_widget_unrealize, NULL);

    if (priv->window) {
		gtk_widget_unregister_window (widget, priv->window);
		gdk_window_destroy (priv->window);
		priv->window = NULL;
    }
    gtk_selection_remove_all (widget);
    gtk_widget_set_realized(widget, FALSE);

}

static void l_widget_map(GtkWidget *widget) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(widget));
	GTK_WIDGET_CLASS(lea_frame_parent_class)->map(widget);
	GtkWidget *toplevel = gtk_widget_get_toplevel(widget);
	priv->event_signal = g_signal_connect(toplevel, "event", G_CALLBACK(l_widget_event), widget);


}

static void l_widget_unmap(GtkWidget *widget) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(widget));
	GtkWidget *toplevel = gtk_widget_get_toplevel(widget);
	g_signal_handler_disconnect(toplevel, priv->event_signal);
	priv->event_signal = 0;
	GTK_WIDGET_CLASS(lea_frame_parent_class)->unmap(widget);
}


static void l_widget_destroy(GtkWidget *widget) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(widget));
	if (priv->top_layer) {
		gtk_widget_unparent((GtkWidget *) priv->top_layer);
		gtk_widget_destroy((GtkWidget *) priv->top_layer);
	}
	if (priv->status_bar) {
		gtk_widget_unparent((GtkWidget *) priv->status_bar);
		gtk_widget_destroy((GtkWidget *) priv->status_bar);
	}
	if (priv->root_menu_widget) {
		gtk_widget_unparent((GtkWidget *) priv->root_menu_widget);
		gtk_widget_destroy((GtkWidget *) priv->root_menu_widget);
		cat_unref_ptr(priv->root_menu_widget);

		cat_unref_ptr(priv->root_menu_action);
	}
	GTK_WIDGET_CLASS(lea_frame_parent_class)->destroy(widget);
}


static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(widget));
	cat_log_debug("allocation:%d,%d,%d,%d, widget=%p", allocation->x, allocation->y, allocation->width, allocation->height, widget);
	gtk_widget_set_allocation(widget, allocation);


	int min_menu_height = 0;
	int nat_menu_height = 0;
	if (priv->root_menu_widget) {
		gtk_widget_get_preferred_height((GtkWidget *) priv->root_menu_widget, &min_menu_height, &nat_menu_height);
	}


	int min_sb_height = 0;
	int nat_sb_height = 0;
	if (priv->status_bar) {
		gtk_widget_get_preferred_height((GtkWidget *) priv->status_bar, &min_sb_height, &nat_sb_height);
	}


	GtkAllocation view_allocation = *allocation;
	view_allocation.x = 0;
	view_allocation.y = 0;
	view_allocation.height -= nat_menu_height+nat_sb_height;
	view_allocation.y += nat_menu_height;

	lea_surface_handler_allocate(priv->service_handler, &view_allocation);

	if (priv->top_layer) {
		gtk_widget_size_allocate(GTK_WIDGET(priv->top_layer), &view_allocation);
	}


	if (priv->root_menu_widget) {
		GtkAllocation menu_allocation = *allocation;
		menu_allocation.x = 0;
		menu_allocation.y = 0;
		menu_allocation.height = nat_menu_height;
		gtk_widget_size_allocate(GTK_WIDGET(priv->root_menu_widget), &menu_allocation);
	}

	if (priv->status_bar) {
		GtkAllocation sb_allocation = *allocation;
		sb_allocation.x = 0;
		sb_allocation.y = allocation->height-nat_sb_height;
		sb_allocation.height = nat_sb_height;
		gtk_widget_size_allocate(GTK_WIDGET(priv->status_bar), &sb_allocation);
	}


	if (gtk_widget_get_realized(widget)) {
	      gdk_window_move_resize(priv->window, allocation->x, allocation->y, allocation->width, allocation->height);
	}
	cat_log_debug("allocation-done:%d,%d,%d,%d, widget=%p", allocation->x, allocation->y, allocation->width, allocation->height, widget);

}


static void l_set_grip(LeaFrame *frame, LeaIGrip *grip) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	if (priv->current_grip==grip) {
		return;
	}

	lea_igrip_is_identical(grip, priv->current_grip);

	if (priv->current_grip) {
		lea_layer_remove_drawable(LEA_LAYER(priv->top_layer), LEA_IDRAWABLE(priv->current_grip));
		lea_igrip_remove_notify(priv->current_grip);
	}
	cat_ref_swap(priv->current_grip, grip);
	if (priv->current_grip) {
		lea_igrip_add_notify(grip, frame);
		lea_layer_add_drawable(LEA_LAYER(priv->top_layer), LEA_IDRAWABLE(grip));
	}
}


static gboolean l_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(data));

	gboolean result = FALSE;
	GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(data));
	if (event->window == window) {

		int mouse_x = (int) event->x;
		int mouse_y = (int) event->y;

		LeaIGrip *grip = lea_surface_handler_calculate_locator(priv->service_handler, mouse_x, mouse_y);
		l_set_grip((LeaFrame *) widget, grip);
		result = grip!=NULL;
		cat_log_debug("mouse_x=%d, mouse_y=%d, result=%d", mouse_x, mouse_y, result);
		cat_unref_ptr(grip);
	} else {
		l_set_grip((LeaFrame *) widget, NULL);
	}
	return result;
}

static gboolean l_widget_button_release_event(GtkWidget *widget, GdkEventButton *event) {
	gboolean result = FALSE;
	return result;
}



static gboolean l_widget_key_press_event(GtkWidget *widget, GdkEventKey *event) {
	cat_log_detail("start");
	LeaFrame *frame = LEA_FRAME(widget);
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	gboolean handled = FALSE;

	LeaKey *key = lea_key_from_gkd_event(event, TRUE);
	cat_log_debug("key:%o", key);

	if (!handled && priv->key_handler) {

		LeaPanel *focus_panel = priv->focus_panel;
		cat_log_debug("focus_panel=%o", focus_panel);
		LeaKeyContext *active_key_context = lea_panel_get_key_context(focus_panel);
		cat_log_debug("active_key_context=%p", active_key_context);
		handled = lea_key_handler_run(priv->key_handler, key, active_key_context);
	}

	if (!handled) {
		handled = gtk_bindings_activate_event(G_OBJECT(widget), event);
	}

	return handled;
}


static gboolean l_button_press(GtkWidget *widget, GdkEventButton *eev, gpointer data) {
	cat_log_detail("press:x:%f, y:%f root:x:%f, y:%f, widget=%p, data=%p", eev->x, eev->y, eev->x_root, eev->y_root, widget, data);
	LeaFramePrivate *priv = lea_frame_get_instance_private(LEA_FRAME(data));

	int mouse_x = (int) eev->x;
	int mouse_y = (int) eev->y;

	LeaIGrip *grip = lea_surface_handler_calculate_locator(priv->service_handler, mouse_x, mouse_y);
	if (grip) {
		l_set_grip(LEA_FRAME(data), NULL);
		lea_igrip_add_notify(grip, LEA_FRAME(data));
		lea_drag_layer_set_active_grip(priv->top_layer, grip);
		cat_unref_ptr(grip);
	}
	return TRUE;
}

static gboolean l_leave_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer data) {
	l_set_grip(LEA_FRAME(data), NULL);
	return FALSE;
}


static void l_set_focus_active(LeaFrame *frame, gboolean active) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	if (priv->focus_active==active) {
		return;
	}
	priv->focus_active = active;

	if (priv->focus_panel) {
		lea_panel_focus_activated(priv->focus_panel, active);
	}
}


static LeaPanel *l_find_panel_with_widget(GtkWidget *widget) {
	cat_log_debug("widget=%p: widget_class=%s", widget, g_type_name_from_instance((GTypeInstance *) widget));

	LeaPanel *result = NULL;
	while(result==NULL && widget) {
		if (LEA_IS_PANEL(widget)) {
			result = LEA_PANEL(widget);
		} else if (LEA_IS_SURFACE_BOX(widget)) {
			widget = NULL;
		} else {
			widget = gtk_widget_get_parent(widget);
		}
	}
	return result;
}

void lea_frame_focus_widget_set(LeaFrame *frame, GtkWidget *focus_widget) {
	LeaFramePrivate *priv = lea_frame_get_instance_private(frame);
	LeaPanel *new_focus_panel = l_find_panel_with_widget(focus_widget);
	if (new_focus_panel==priv->focus_panel) {
		return;
	}
	cat_log_debug("lea_frame_focus_widget_set:new_focus_panel=%o", new_focus_panel);

	LeaPanel *old_panel = priv->focus_panel;
	LeaIPanelOwner *old_owner = old_panel ? lea_panel_get_panel_owner(old_panel) : NULL;

	LeaIPanelOwner *new_owner = new_focus_panel ? lea_panel_get_panel_owner(new_focus_panel) : NULL;

	cat_ref_swap(priv->focus_panel, new_focus_panel);

	LeaPanelFocusEvent *focus_event = lea_panel_focus_event_new(focus_widget, old_owner, old_panel, new_owner, new_focus_panel);

	if (old_panel) {
		lea_panel_set_has_focus(old_panel, FALSE);
	}
	if (new_focus_panel) {
		lea_panel_focus_activated(new_focus_panel, priv->focus_active);
		lea_panel_set_has_focus(new_focus_panel, TRUE);
	}

	if (old_owner!=new_owner) {
		if (new_owner) {
			GtkWidget *widget = lea_ipanel_owner_get_status_bar_content(new_owner);
			lea_status_bar_set_context_widget(priv->status_bar, widget);
		} else {
			lea_status_bar_set_context_widget(priv->status_bar, NULL);
		}
	}

	lea_frame_dispatch_event(frame, (LeaEvent *) focus_event);

	cat_unref_ptr(focus_event);
	gtk_widget_queue_draw(GTK_WIDGET(frame));
}

static gboolean l_widget_event(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
	if (event->type == GDK_FOCUS_CHANGE) {
		LeaFrame *frame = LEA_FRAME(user_data);
		GdkEventFocus *focus_event = (GdkEventFocus *) event;
		l_set_focus_active(frame, focus_event->in!=0);
	}
	return FALSE;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaFrame *instance = LEA_FRAME(self);
	LeaFramePrivate *priv = lea_frame_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p children=%o, current_grip=%o]", iname, self, priv->a_children, priv->current_grip);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

