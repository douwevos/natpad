/*
   File:    mooexplorer.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 10, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#include "mooexplorer.h"
#include "moolayoutmodelprivate.h"
#include <leafhopper.h>
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooExplorer"
#include <logging/catlog.h>

struct _MooExplorerPrivate {
	VipSnapshot *vip_snapshot;
	MooService *moo_service;
	MooLayoutModel *layout_model;
	GtkCssProvider *css_provider;
};


static void l_transaction_listener_iface_init(MooITransactionListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooExplorer, moo_explorer, GTK_TYPE_DRAWING_AREA, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ITRANSACTION_LISTENER, l_transaction_listener_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_draw(GtkWidget *widget, cairo_t *cr, gpointer view_as_ptr);
static gboolean l_button_press_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer user_data);



static void moo_explorer_class_init(MooExplorerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooExplorerPrivate));

	clazz->activateNode = NULL;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_explorer_init(MooExplorer *instance) {
	MooExplorerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_EXPLORER, MooExplorerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooExplorer *instance = MOO_EXPLORER(object);
	MooExplorerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->layout_model);
	cat_unref_ptr(priv->vip_snapshot);
	cat_unref_ptr(priv->css_provider);
	cat_log_detail("dispose:%p", parent_class);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void moo_explorer_construct(MooExplorer *moo_explorer, MooService *moo_service, MooNodeRenderRegistry *render_registry) {
	MooExplorerPrivate *priv = MOO_EXPLORER_GET_PRIVATE(moo_explorer);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->layout_model = moo_layout_model_new(render_registry);
	priv->vip_snapshot = NULL;
	priv->css_provider = gtk_css_provider_new();
	MooNodeWo *root_node = moo_service_get_root_node_ref(moo_service);
	moo_layout_model_set_root_node(priv->layout_model, root_node);
	cat_unref_ptr(root_node);

	GtkWidget *main_widget = GTK_WIDGET(moo_explorer);

	GtkStyleContext *context = gtk_widget_get_style_context(main_widget);
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(priv->css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	gtk_css_provider_load_from_data(priv->css_provider, "MooExplorer { font: Sans 10; }", -1, NULL);


	gtk_widget_set_can_focus(main_widget, TRUE);
	gtk_widget_set_app_paintable(main_widget, TRUE);

	gtk_widget_set_events (main_widget, gtk_widget_get_events (main_widget)
	                                             | GDK_BUTTON_PRESS_MASK
	                                             | GDK_BUTTON2_MASK
	                                             | GDK_BUTTON3_MASK
	                                             | GDK_BUTTON4_MASK
	                                             | GDK_BUTTON5_MASK
	                                             | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_POINTER_MOTION_MASK
	                                             | GDK_POINTER_MOTION_HINT_MASK
	                                             | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_SCROLL_MASK
	                                             );

//	gtk_widget_set_events(main_widget, gtk_widget_get_events(main_widget)
//	                                             | GDK_POINTER_MOTION_MASK
//	                                             | GDK_POINTER_MOTION_HINT_MASK
//	                                             | GDK_BUTTON_PRESS_MASK
//	                                             | GDK_BUTTON_RELEASE_MASK);

	g_signal_connect(moo_explorer, "draw", G_CALLBACK(l_draw), moo_explorer);
	g_signal_connect(moo_explorer, "button-press-event", G_CALLBACK(l_button_press_event), moo_explorer);


	MooTransactionDispatcher *dispatcher = moo_service_get_transaction_dispatcher(moo_service);
	moo_transaction_dispatcher_add_listener(dispatcher, (MooITransactionListener *) moo_explorer);
}


MooExplorer *moo_explorer_new(MooService *moo_service, MooNodeRenderRegistry *render_registry) {
	MooExplorer *result = g_object_new(MOO_TYPE_EXPLORER, NULL);
	cat_ref_anounce(result);
	moo_explorer_construct(result, moo_service, render_registry);
	return result;
}

MooLayoutModel *moo_explorer_get_layout_model(MooExplorer *moo_explorer) {
	return MOO_EXPLORER_GET_PRIVATE(moo_explorer)->layout_model;
}


static gboolean l_draw(GtkWidget *widget, cairo_t *cairo, gpointer view_as_ptr) {
	MooExplorer *view = MOO_EXPLORER(view_as_ptr);
	MooExplorerPrivate *priv = MOO_EXPLORER_GET_PRIVATE(view);

	cairo_save(cairo);

	cat_log_on_detail({
		cairo_rectangle_list_t *list_t =  cairo_copy_clip_rectangle_list (cairo);
		cat_log_detail("list_t=%p", list_t);
		int idx;
		for(idx=list_t->num_rectangles-1; idx>=0; idx--) {
			cairo_rectangle_t rect = list_t->rectangles[idx];
			cat_log_detail("r[%d]=[%d,%d - %d,%d]",idx, (int) rect.x, (int) rect.y, (int) rect.width, (int) rect.height);
		}
	});

	double clip_x1, clip_y1, clip_x2, clip_y2;
	cairo_clip_extents(cairo, &clip_x1, &clip_y1, &clip_x2, &clip_y2);

	GtkWidget *s_window = gtk_widget_get_parent(widget);
	GtkAllocation allocation;
	if (LEA_IS_SCROLLED_WINDOW(s_window)) {
		gtk_widget_get_allocation(s_window, &allocation);
		LeaScrolledWindow *sw = LEA_SCROLLED_WINDOW(s_window);
		allocation.x = sw->view_x;
		allocation.y = sw->view_y;

		cairo_translate(cairo, -allocation.x, -allocation.y);
	} else {
		GdkWindow *view_window = gtk_viewport_get_bin_window(GTK_VIEWPORT(s_window));
		gdk_window_get_geometry(view_window, &(allocation.x), &(allocation.y), &(allocation.width), &(allocation.height));
		int x = -allocation.x;
		int y = -allocation.y;
		gtk_widget_get_allocation(s_window, &allocation);
		allocation.x=x;
		allocation.y=y;
	}
	cat_log_trace("allocation=%d,%d - %d,%d", allocation.x, allocation.y, allocation.width, allocation.height);


//	PangoContext *pango_context = gtk_widget_create_pango_context(widget);
//	PangoFontMetrics *font_metrics = pango_context_get_metrics(pango_context, NULL, NULL);
//	double font_height = ((double) (pango_font_metrics_get_ascent(font_metrics) + pango_font_metrics_get_descent(font_metrics)))/PANGO_SCALE;
//	int ifont_height = ceil(font_height);
//
//
//	cairo_set_source_rgb(cairo, 1.0, 0.0, 1.0);
//	cairo_rectangle(cairo, 0, 20, 100, 50);
//	cairo_fill(cairo);
	PangoContext *pango_context = gtk_widget_create_pango_context(widget);


	moo_layout_model_validate(priv->layout_model, cairo, pango_context);
	moo_layout_model_paint(priv->layout_model, pango_context, cairo);

	cairo_restore(cairo);

	return TRUE;
}



static void l_validate_and_paint(MooExplorer *explorer) {
	MooExplorerPrivate *priv = MOO_EXPLORER_GET_PRIVATE(explorer);

	cairo_t *cairo = gdk_cairo_create(gtk_widget_get_window((GtkWidget *) explorer));
	PangoContext *pango_context = gtk_widget_create_pango_context((GtkWidget *) explorer);

	moo_layout_model_validate(priv->layout_model, cairo, pango_context);


	int cur_width, cur_height, view_width, view_height;
	gtk_widget_get_size_request((GtkWidget *) explorer, &cur_width, &cur_height);
	moo_layout_model_get_view_size(priv->layout_model, &view_width, &view_height);
	cat_log_debug("current-size=[%d,%d], view-size=[%d, %d]", cur_width, cur_height, view_width, view_height);

	GtkWidget *s_window = gtk_widget_get_parent((GtkWidget *) explorer);
	if (LEA_IS_SCROLLED_WINDOW(s_window)) {
		LeaScrolledWindow *sw = LEA_SCROLLED_WINDOW(s_window);
		lea_scrolled_window_set_view_size(sw, view_width, view_height);
		gdk_window_invalidate_rect(gtk_widget_get_window((GtkWidget *) explorer), NULL, FALSE);
	} else if ((cur_width!=view_width)
			|| (cur_height!=view_height)) {
//		gtk_widget_set_size_request((GtkWidget *) explorer, view_width, view_height);
	}


//	l_draw((GtkWidget *) explorer, cairo, explorer);

	cairo_destroy(cairo);
}




static void l_get_view_location(MooExplorer *explorer, int *view_x, int *view_y) {
	GtkWidget *widget = GTK_WIDGET(explorer);
	GtkWidget *s_window = gtk_widget_get_parent(widget);
	if (LEA_IS_SCROLLED_WINDOW(s_window)) {
		LeaScrolledWindow *sw = LEA_SCROLLED_WINDOW(s_window);
		*view_x = sw->view_x;
		*view_y = sw->view_y;
	} else {
		GtkAllocation allocation;
		gtk_widget_get_allocation(widget, &allocation);
		*view_x = allocation.x;
		*view_y = allocation.y;
	}
}

static void l_get_mouse_location(MooExplorer *explorer, int *wmx, int *wmy) {
	int tx;
	int ty;

	GtkWidget *widget = GTK_WIDGET(explorer);
	GdkDeviceManager *device_manager = gdk_display_get_device_manager(gtk_widget_get_display(widget));
	GdkDevice *pointer = gdk_device_manager_get_client_pointer(device_manager);
	gdk_window_get_device_position(gtk_widget_get_window(widget), pointer, &tx, &ty, NULL);
	int view_x, view_y;
	l_get_view_location(explorer, &view_x, &view_y);
	*wmx = tx+view_x;
	*wmy = ty+view_y;
	cat_log_debug("wmx:%d, y:%d ", *wmx, *wmy);
}


static gboolean l_button_press_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer user_data) {
	MooExplorer *explorer = MOO_EXPLORER(gwidget);
	MooExplorerPrivate *priv = MOO_EXPLORER_GET_PRIVATE(explorer);
	gtk_widget_grab_focus(gwidget);
//	int modifiers = eev->state;

//	cat_log_debug("modifiers=%d, modifiersEx=", modifiers, modifiersEx);

	gboolean shiftPressed = eev->state & GDK_SHIFT_MASK;
	gboolean ctrlPressed = eev->state & GDK_CONTROL_MASK;

	int xmouse, ymouse;
	l_get_mouse_location(explorer, &xmouse, &ymouse);

	MooNodeLayout *nodeAt = moo_layout_model_node_at(priv->layout_model, xmouse, ymouse);
	gboolean tryToggleFirst = FALSE;
	if (nodeAt!=NULL) {
		MooNodeLayoutZone zone = moo_node_layout_get_zone(nodeAt, xmouse, ymouse);
		if (zone==MOO_NODE_LAYOUT_ZONE_EXPANDER) {
			tryToggleFirst = TRUE;
		}
		if (eev->type==GDK_2BUTTON_PRESS) {
			MooExplorerClass *me_class = MOO_EXPLORER_GET_CLASS(explorer);
			if (me_class->activateNode) {
				me_class->activateNode(explorer, nodeAt);
			}
		} else {
			if (moo_layout_model_select(priv->layout_model, nodeAt, tryToggleFirst, ctrlPressed, shiftPressed)) {
				l_validate_and_paint(explorer);
			}
		}
	}


	return TRUE;
}

/********************* begin MooITransactionListener implementation *********************/


typedef struct _NodeSetInfo NodeSetInfo;

struct _NodeSetInfo {
	MooExplorer *explorer;
	MooNodeWo *root_node;
	VipSnapshot *vip_snapshot;
};

static gboolean l_set_new_root_node(gpointer user_data) {
	NodeSetInfo *node_set_info = (NodeSetInfo *) user_data;
	MooExplorerPrivate *priv = MOO_EXPLORER_GET_PRIVATE(node_set_info->explorer);

	moo_layout_model_set_root_node(priv->layout_model, node_set_info->root_node);
	cat_ref_swap(priv->vip_snapshot, node_set_info->vip_snapshot);

	cairo_t *cairo = gdk_cairo_create(gtk_widget_get_window((GtkWidget *) node_set_info->explorer));

	PangoContext *pango_context = gtk_widget_create_pango_context((GtkWidget *) node_set_info->explorer);

	moo_layout_model_validate(priv->layout_model, cairo, pango_context);


	int cur_width, cur_height, view_width, view_height;
	gtk_widget_get_size_request((GtkWidget *) node_set_info->explorer, &cur_width, &cur_height);
	moo_layout_model_get_view_size(priv->layout_model, &view_width, &view_height);
	cat_log_debug("current-size=[%d,%d], view-size=[%d, %d]", cur_width, cur_height, view_width, view_height);

	GtkWidget *s_window = gtk_widget_get_parent((GtkWidget *) node_set_info->explorer);
	if (LEA_IS_SCROLLED_WINDOW(s_window)) {
		LeaScrolledWindow *sw = LEA_SCROLLED_WINDOW(s_window);
		lea_scrolled_window_set_view_size(sw, view_width, view_height);
	} else 	if ((cur_width!=view_width)
			|| (cur_height!=view_height)) {
//		gtk_widget_set_size_request((GtkWidget *) node_set_info->explorer, view_width, view_height);
	}





	l_draw((GtkWidget *) node_set_info->explorer, cairo, node_set_info->explorer);

	cairo_destroy(cairo);

//	cat_unref_ptr(node_set_info->explorer);
//	cat_unref_ptr(node_set_info->root_node);
//	cat_unref_ptr(node_set_info->vip_snapshot);
//	g_free(node_set_info);
	return FALSE;
}

static void l_snrn_destroy(gpointer data) {
	NodeSetInfo *node_set_info = (NodeSetInfo *) data;
	cat_unref_ptr(node_set_info->explorer);
	cat_unref_ptr(node_set_info->root_node);
	cat_unref_ptr(node_set_info->vip_snapshot);
	g_free(node_set_info);
}



static void l_transaction_commited(MooITransactionListener *self, struct _MooTransaction *transaction, struct _MooNodeWo *new_root) {
	cat_log_debug("transaction commited:new_root=%o", new_root);
	MooExplorerPrivate *priv = MOO_EXPLORER_GET_PRIVATE(self);
	VipService *vip_service = moo_service_get_viper_service(priv->moo_service);
	NodeSetInfo *node_set_info = g_new(NodeSetInfo, 1);
	node_set_info->explorer = cat_ref_ptr(self);
	node_set_info->root_node = cat_ref_ptr(new_root);
	node_set_info->vip_snapshot = vip_service_get_snapshot(vip_service);
	g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, (GSourceFunc) l_set_new_root_node, node_set_info, (GDestroyNotify) l_snrn_destroy);
}


static void l_transaction_listener_iface_init(MooITransactionListenerInterface *iface) {
	iface->transactionCommited = l_transaction_commited;
}


/********************* end MooITransactionListener implementation *********************/
