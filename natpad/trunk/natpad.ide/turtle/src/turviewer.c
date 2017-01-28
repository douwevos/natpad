/*
   File:    turviewer.c
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 20, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "turviewer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TurViewer"
#include <logging/catlog.h>

enum {
  PROP_0,
  PROP_HADJUSTMENT,
  PROP_VADJUSTMENT,
  PROP_HSCROLL_POLICY,
  PROP_VSCROLL_POLICY,
};


struct _TurViewerPrivate {
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;
	int hscroll_policy : 1;
	int vscroll_policy : 1;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TurViewer, tur_viewer, GTK_TYPE_WIDGET,
		G_ADD_PRIVATE(TurViewer)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);

static void l_widget_realize(GtkWidget *widget);
static void l_widget_unrealize(GtkWidget *widget);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void l_set_vadjustment_values(TurViewer *viewer);
static void l_set_hadjustment_values(TurViewer *viewer);

static void tur_viewer_class_init(TurViewerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
	object_class->set_property = l_set_property;
	object_class->get_property = l_get_property;

	/* GtkScrollable interface */
	g_object_class_override_property (object_class, PROP_HADJUSTMENT,    "hadjustment");
	g_object_class_override_property (object_class, PROP_VADJUSTMENT,    "vadjustment");
	g_object_class_override_property (object_class, PROP_HSCROLL_POLICY, "hscroll-policy");
	g_object_class_override_property (object_class, PROP_VSCROLL_POLICY, "vscroll-policy");

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->realize = l_widget_realize;
	widget_class->unrealize = l_widget_unrealize;
	widget_class->size_allocate = l_widget_size_allocate;
//	widget_class->draw = l_widget_draw;
//	widget_class->focus = l_widget_focus;
}

static void tur_viewer_init(TurViewer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	TurViewer *instance = TUR_VIEWER(object);
//	TurViewerPrivate *priv = tur_viewer_get_instance_private(instance);
	G_OBJECT_CLASS(tur_viewer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(tur_viewer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TurViewer *tur_viewer_new() {
	TurViewer *result = g_object_new(TUR_TYPE_VIEWER, NULL);
	cat_ref_anounce(result);
	TurViewerPrivate *priv = tur_viewer_get_instance_private(result);
	return result;
}






static void l_widget_realize(GtkWidget *widget) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(TUR_VIEWER(widget));

	gtk_widget_set_realized(widget, TRUE);

	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);

	GdkWindowAttr attributes;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual (widget);
	attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK
			| GDK_EXPOSURE_MASK
			| gtk_widget_get_events(widget);

	gint attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

	GdkWindow *window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
	gtk_widget_set_window(widget, window);
	gtk_widget_register_window(widget, window);

//	priv->text_window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);

//	gdk_window_set_invalidate_handler(window, l_window_invalidate_handler);


//	priv->pango_context = gtk_widget_create_pango_context(widget);
//	if (priv->document_view==NULL) {
//		ChaEditorClass *editor_class = JOR_OUTLINE_VIEWER_GET_CLASS(widget);
//		if (editor_class->initializeDocumentView) {
//			priv->document_view = editor_class->initializeDocumentView((TurViewer *) widget, priv->document, priv->pango_context, widget);
//		} else {
//			priv->document_view = cha_document_view_new(priv->document, priv->pango_context, widget);
//		}
//		if (priv->a_preferences) {
//			cha_document_view_set_preferences(priv->document_view, priv->a_preferences);
//		}
//		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, priv->vadjustment);
//	}
//
//	gdk_window_set_cursor(gtk_widget_get_window(widget), gdk_cursor_new(GDK_XTERM));
//	gtk_im_context_set_client_window(priv->im_context, window);
}

static void l_widget_unrealize(GtkWidget *widget) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(TUR_VIEWER(widget));
//	gtk_im_context_set_client_window(priv->im_context, NULL);
	GTK_WIDGET_CLASS(tur_viewer_parent_class)->unrealize (widget);
}

static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(TUR_VIEWER(widget));

	GtkAllocation cur_alloc;
	gtk_widget_get_allocation(widget, &cur_alloc);
	if (cur_alloc.x==allocation->x
			&& cur_alloc.y==allocation->y
			&& cur_alloc.width==allocation->width
						&& cur_alloc.height==allocation->height
						) {
		cat_log_warn("breaking !!!");
		return;
	}
	gtk_widget_set_allocation(widget, allocation);

	if (gtk_widget_get_realized(widget)) {
		gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x, allocation->y, allocation->width, allocation->height);
	}

//	cha_document_view_set_view_size(priv->document_view, allocation->width, allocation->height);

	l_set_hadjustment_values((TurViewer *) widget);
	l_set_vadjustment_values((TurViewer *) widget);

//	cha_document_view_invalidate_lines(priv->document_view);
}


static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr) {
	TurViewer *editor = JOR_OUTLINE_VIEWER(widget);
	TurViewerPrivate *priv = tur_viewer_get_instance_private(TUR_VIEWER(widget));




//	cha_document_view_draw(priv->document_view, cr);
	cairo_set_source_rgb(cr,1,0,0);
	cairo_move_to(cr, 50,30);
	cairo_line_to(cr, 150,300);
	cairo_stroke(cr);
	return FALSE;
}

static gboolean l_widget_focus(GtkWidget *widget, GtkDirectionType direction) {
	gtk_widget_grab_focus(widget);
	return TRUE;
}


static void l_set_hadjustment_values(TurViewer *viewer) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(viewer);
	GtkAllocation allocation;
	gtk_widget_get_allocation(GTK_WIDGET(viewer), &allocation);
	int view_width = allocation.width;

	gdouble old_value = gtk_adjustment_get_value(priv->hadjustment);
	gdouble new_upper = view_width * 2;
//  gdouble new_upper = MAX (screen_width, priv->width);
	g_object_set(priv->hadjustment, "lower", 0.0, "upper", new_upper,
			"page-size", (gdouble) view_width,
			"step-increment", view_width * 0.1, "page-increment", view_width * 0.9,
			NULL);

	gdouble new_value = CLAMP(old_value, 0, new_upper - view_width);
	if (new_value != old_value) {
		gtk_adjustment_set_value (priv->hadjustment, new_value);
	}
}



static void l_set_vadjustment_values(TurViewer *viewer) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(viewer);
	GtkAllocation allocation;
	gtk_widget_get_allocation(GTK_WIDGET(viewer), &allocation);
	int view_height = allocation.height;

	gdouble old_value = gtk_adjustment_get_value(priv->vadjustment);
	gdouble new_upper = gtk_adjustment_get_upper(priv->vadjustment);
//	if (priv->document_view) {
//		long long int layout_height = cha_document_view_get_layout_height(priv->document_view);
//		new_upper = MAX(layout_height, view_height);
//	}
	cat_log_trace("view_height=%d, new_upper=%d", view_height, (int) new_upper);
	g_object_set(priv->vadjustment, "lower", 0.0, "upper", new_upper,
			"page-size", (gdouble) view_height,
			"step-increment", view_height * 0.1, "page-increment", view_height * 0.9,
			NULL);

	gdouble new_value = CLAMP(old_value, 0, new_upper - view_height);
	cat_log_trace("new_value=%d", (int) new_value);
	if (new_value != old_value) {
		gtk_adjustment_set_value (priv->vadjustment, new_value);
	}
}


static void l_hadjustment_value_changed(GtkAdjustment *adjustment, TurViewer *viewer) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(viewer);
	glong aval = (glong) gtk_adjustment_get_value(adjustment);
//	glong old = cha_document_view_set_view_x(priv->document_view, aval);
//    glong dx = old - aval;
//	if (priv->depreacated_scrolling) {
//		l_deprecated_scroll(editor, dx, 0);
//	} else {
//		GdkWindow *window = gtk_widget_get_window((GtkWidget *) editor);
//		cha_document_view_invalidate_lines(priv->document_view);
//		gdk_window_scroll(window, dx, 0);
//		gdk_window_process_updates(window, TRUE);
//	}
}


static void l_vadjustment_value_changed(GtkAdjustment *adjustment, TurViewer *viewer) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(viewer);
	glong aval = (glong) gtk_adjustment_get_value(adjustment);

//	glong old = cha_document_view_set_view_y(priv->document_view, aval);
//    glong dy = old - aval;
//    cat_log_debug("scroll-v:%d, %d", (int) dy, aval);
//
//    if (priv->depreacated_scrolling) {
//    	l_deprecated_scroll(editor, 0, dy);
//    } else {
//		GdkWindow *window = gtk_widget_get_window((GtkWidget *) editor);
//		cha_document_view_set_in_scroll(priv->document_view, TRUE);
//		if (!cha_document_view_check_cache_boundaries(priv->document_view)) {
//			cha_document_view_invalidate_lines(priv->document_view);
//		}
//		gdk_window_scroll(window, 0, dy);
//		gdk_window_process_updates(window, TRUE);
//		cha_document_view_set_in_scroll(priv->document_view, FALSE);
//    }
}


static void l_set_hadjustment(TurViewer *viewer, GtkAdjustment *adjustment) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(viewer);

	if (adjustment && priv->hadjustment == adjustment) {
	    return;
	}

	if (priv->hadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(priv->hadjustment, l_hadjustment_value_changed, viewer);
		g_object_unref(priv->hadjustment);
	}

	if (adjustment == NULL) {
	    adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(l_hadjustment_value_changed), viewer);
	priv->hadjustment = cat_ref_sink_ptr(adjustment);
//	if (priv->document_view) {
//		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, priv->vadjustment);
//	}
	l_set_hadjustment_values(viewer);
	g_object_notify(G_OBJECT(viewer), "hadjustment");
}



static void l_set_vadjustment(TurViewer *viewer, GtkAdjustment *adjustment) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(viewer);

	if (adjustment && priv->vadjustment == adjustment) {
	    return;
	}

	if (priv->vadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(priv->vadjustment, l_vadjustment_value_changed, viewer);
		g_object_unref(priv->vadjustment);
	}

	if (adjustment == NULL) {
	    adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(l_vadjustment_value_changed), viewer);
	priv->vadjustment = cat_ref_sink_ptr(adjustment);
//	if (priv->document_view) {
//		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, priv->vadjustment);
//	}
	l_set_vadjustment_values(viewer);
	g_object_notify(G_OBJECT(viewer), "vadjustment");
}



static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	TurViewer *viewer = TUR_VIEWER(object);
	TurViewerPrivate *priv = tur_viewer_get_instance_private(viewer);

	switch (prop_id) {
		case PROP_HADJUSTMENT:
			l_set_hadjustment(viewer, g_value_get_object(value));
			break;
		case PROP_VADJUSTMENT:
			l_set_vadjustment(viewer, g_value_get_object(value));
			break;
		case PROP_HSCROLL_POLICY:
			if (priv->hscroll_policy != g_value_get_enum(value)) {
				priv->hscroll_policy = g_value_get_enum(value);
				gtk_widget_queue_resize(GTK_WIDGET(viewer));
				g_object_notify_by_pspec(object, pspec);
			} break;
    	case PROP_VSCROLL_POLICY:
    		if (priv->vscroll_policy != g_value_get_enum(value)) {
    			priv->vscroll_policy = g_value_get_enum(value);
    			gtk_widget_queue_resize(GTK_WIDGET(viewer));
    			g_object_notify_by_pspec (object, pspec);
    		} break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}


static void l_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	TurViewerPrivate *priv = tur_viewer_get_instance_private(TUR_VIEWER(object));

	switch(prop_id) {
		case PROP_HADJUSTMENT:
			g_value_set_object(value, priv->hadjustment);
			break;
		case PROP_VADJUSTMENT:
			g_value_set_object(value, priv->vadjustment);
			break;
		case PROP_HSCROLL_POLICY:
			g_value_set_enum(value, priv->hscroll_policy);
			break;
		case PROP_VSCROLL_POLICY:
			g_value_set_enum(value, priv->vscroll_policy);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TurViewer *instance = TUR_VIEWER(self);
	TurViewerPrivate *priv = tur_viewer_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

