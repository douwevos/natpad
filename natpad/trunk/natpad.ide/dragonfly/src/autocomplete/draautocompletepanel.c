/*
   File:    draautocompletepanel.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 17, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "draautocompletepanel.h"
#include "draiaccontextlistener.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraAutoCompletePanel"
#include <logging/catlog.h>

enum {
	PROP_0,
	PROP_HADJUSTMENT,
	PROP_VADJUSTMENT,
	PROP_HSCROLL_POLICY,
	PROP_VSCROLL_POLICY
};


struct _DraAutoCompletePanelPrivate {
	DraAcContext *ac_context;
	int selected_entry_idx;
	DraEditor *editor;

	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;

	int hscroll_policy : 1;
	int vscroll_policy : 1;

	int view_width;
	int view_height;
};

static void l_context_listener_iface_init(DraIAcContextListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_scrollable_iface_init(GtkScrollableInterface *iface) {
}

G_DEFINE_TYPE_WITH_CODE(DraAutoCompletePanel, dra_auto_complete_panel, GTK_TYPE_DRAWING_AREA,
		G_ADD_PRIVATE(DraAutoCompletePanel)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_IAC_CONTEXT_LISTENER, l_context_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(GTK_TYPE_SCROLLABLE, l_scrollable_iface_init);
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void l_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

static gboolean l_draw(GtkWidget *widget, cairo_t *cairo);

static void dra_auto_complete_panel_class_init(DraAutoCompletePanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
	object_class->set_property = l_set_property;
	object_class->get_property = l_get_property;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->draw = l_draw;

	g_object_class_override_property(object_class, PROP_HADJUSTMENT,    "hadjustment");
	g_object_class_override_property(object_class, PROP_VADJUSTMENT,    "vadjustment");
	g_object_class_override_property(object_class, PROP_HSCROLL_POLICY, "hscroll-policy");
	g_object_class_override_property(object_class, PROP_VSCROLL_POLICY, "vscroll-policy");

}

static void dra_auto_complete_panel_init(DraAutoCompletePanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraAutoCompletePanel *instance = DRA_AUTO_COMPLETE_PANEL(object);
//	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(instance);
	G_OBJECT_CLASS(dra_auto_complete_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_auto_complete_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_get_layout_dimensions(DraAutoCompletePanel *panel, int *twidth, int *theight);
static int l_get_entry_height(GtkWidget *widget);
static gboolean l_handle_key_event_cb(GtkWidget *gwidget, GdkEventKey *eev, gpointer data);
static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer _obj);
static gboolean l_button_release_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer _obj);
static void l_widget_size_allocate_cb(GtkWidget *widget, GdkRectangle *allocation, gpointer data);

DraAutoCompletePanel *dra_auto_complete_panel_new(DraEditor *editor, DraAcContext *ac_context) {
	DraAutoCompletePanel *result = g_object_new(DRA_TYPE_AUTO_COMPLETE_PANEL, NULL);
	cat_ref_anounce(result);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(result);
	priv->ac_context = cat_ref_ptr(ac_context);
	priv->selected_entry_idx = 0;
	priv->editor = editor;

	int twidth, theight;

	l_get_layout_dimensions(result, &twidth, &theight);

	priv->view_width = twidth;
	priv->view_height = theight;


	cat_log_debug("dimensions[%d, %d]", twidth, theight);
	gtk_widget_set_size_request(GTK_WIDGET(result), twidth, theight);


	gtk_widget_set_can_focus(GTK_WIDGET(result), TRUE);
	gtk_widget_set_can_default(GTK_WIDGET(result), TRUE);

	g_signal_connect(result, "key-press-event", G_CALLBACK(l_handle_key_event_cb), result);
	g_signal_connect(result, "button-press-event", G_CALLBACK(l_button_press_event_cb), result);
	g_signal_connect(result, "button-release-event", G_CALLBACK(l_button_release_event_cb), result);
	g_signal_connect(result, "size-allocate", G_CALLBACK(l_widget_size_allocate_cb), result);


	gtk_widget_add_events((GtkWidget *) result, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

	GtkWidget *widget = (GtkWidget *) result;
	gtk_widget_set_events(widget, gtk_widget_get_events(widget)
	                                             | GDK_BUTTON_PRESS_MASK
	                                             | GDK_BUTTON2_MASK
	                                             | GDK_BUTTON3_MASK
	                                             | GDK_BUTTON4_MASK
	                                             | GDK_BUTTON5_MASK
	                                             | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_POINTER_MOTION_MASK
	                                             | GDK_POINTER_MOTION_HINT_MASK
	                                             | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_KEY_PRESS_MASK
	                                             | GDK_SCROLL_MASK
	                                             );

	dra_ac_context_add_listener(ac_context, DRA_IAC_CONTEXT_LISTENER(result));
	return result;
}

void dra_auto_complete_panel_set_selected_index(DraAutoCompletePanel *panel, int index) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	if (priv->selected_entry_idx == index) {
		return;
	}
	if (index<0 || index>= dra_ac_context_count(priv->ac_context)) {
		return;
	}

	priv->selected_entry_idx = index;
	int entry_height = l_get_entry_height(GTK_WIDGET(panel));
	int offset = index*entry_height;
	int view_height = gtk_widget_get_allocated_height(GTK_WIDGET(panel));

	int nyoffset = (int) gtk_adjustment_get_value(priv->vadjustment);

	if (offset+entry_height>nyoffset+view_height) {
		nyoffset = offset+entry_height-view_height;
	}

	if (offset<nyoffset) {
		nyoffset = offset;
	}

	gtk_adjustment_set_value(priv->vadjustment, nyoffset);

	GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(panel));
	gdk_window_invalidate_rect(window, NULL, FALSE);
}

gboolean dra_auto_complete_panel_new_revision(DraAutoCompletePanel *panel, ChaRevisionWo *a_new_revision) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	gboolean result = TRUE;
	result = dra_ac_context_set_next_revision(priv->ac_context, a_new_revision);
	return result;
}


static int l_get_entry_height(GtkWidget *widget) {
	PangoContext *pango_context = gtk_widget_get_pango_context(widget);
	PangoFontMetrics *font_metrics = pango_context_get_metrics(pango_context, NULL, NULL);
	int font_height = (pango_font_metrics_get_ascent(font_metrics)+pango_font_metrics_get_descent(font_metrics))/PANGO_SCALE;
	pango_font_metrics_unref(font_metrics);
	return font_height+2;
}

static void l_get_layout_dimensions(DraAutoCompletePanel *panel, int *twidth, int *theight) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	GtkWidget *widget = GTK_WIDGET(panel);
	PangoContext *pango_context = gtk_widget_get_pango_context(widget);
	PangoLayout *pango_layout = pango_layout_new(pango_context);

	int max_width = 10;
	CatIIterator *iterator = dra_ac_context_iterator(priv->ac_context);
	while(cat_iiterator_has_next(iterator)) {
		DraAcEntry *entry = (DraAcEntry *) cat_iiterator_next(iterator);
		int width = dra_ac_entry_get_width(entry, pango_layout);
		if (width>max_width) {
			max_width = width;
		}
	}
	cat_unref_ptr(iterator);

	*twidth = max_width+4;

	int entry_height = l_get_entry_height(widget);

	*theight = entry_height*dra_ac_context_count(priv->ac_context);
}



static gboolean l_draw(GtkWidget *widget, cairo_t *cairo) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(DRA_AUTO_COMPLETE_PANEL(widget));

	int w_height = gtk_widget_get_allocated_height(widget);

	gdouble view_y = gtk_adjustment_get_value(priv->vadjustment);
	gdouble view_x = gtk_adjustment_get_value(priv->hadjustment);

	cairo_save(cairo);

	GtkAllocation alloc;
	gtk_widget_get_allocation(widget, &alloc);
	cairo_set_source_rgba(cairo, 1.0, 1.0, 0.7, 0.7);
	cairo_rectangle(cairo, 0,0, alloc.width, alloc.height);
	cairo_fill(cairo);

	cairo_translate(cairo, -view_x, -view_y);

	cat_log_debug("w_height=%d, view_y=%f, view_x=%f", w_height, view_y, view_x);


	PangoContext *pango_context = gtk_widget_get_pango_context(widget);
	PangoLayout *pango_layout = pango_layout_new(pango_context);

	cairo_set_source_rgb(cairo, 0, 0, 0.2);
	pango_cairo_update_layout(cairo, pango_layout);

	int entry_height = l_get_entry_height(widget);

	int count = dra_ac_context_count(priv->ac_context);
	int idx;
	for(idx=0; idx<count; idx++) {
		DraAcEntry *completion_entry = dra_ac_context_get_at(priv->ac_context, idx);
		int ypos = idx*entry_height;
		if (ypos+entry_height<view_y) {
			continue;
		}
		if (ypos>view_y+w_height) {
			break;
		}
		gboolean selected = priv->selected_entry_idx==idx;
		dra_ac_entry_paint(completion_entry, widget, cairo, pango_layout, ypos, entry_height, selected);
	}



	cairo_restore(cairo);
	return TRUE;
}



static gboolean l_handle_key_event_cb(GtkWidget *gwidget, GdkEventKey *eev, gpointer data) {
	DraAutoCompletePanel *panel = DRA_AUTO_COMPLETE_PANEL(data);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	int key_val = eev->keyval;

	cat_log_debug("keyval:%x", key_val);

	int entry_count = dra_ac_context_count(priv->ac_context);

	switch(key_val) {
		case GDK_KEY_Escape : {
			cat_log_debug("destroying popup");
			GtkWidget *top = gtk_widget_get_toplevel(gwidget);
			gtk_widget_destroy(top);
			return TRUE;
		}
		case GDK_KEY_Up : {
			if (entry_count>0) {
				int new_index = priv->selected_entry_idx-1;
				if (new_index<0) {
					new_index = entry_count-1;
				}

				dra_auto_complete_panel_set_selected_index(panel, new_index);
			}
			return TRUE;
		}
		case GDK_KEY_Down : {
			if (entry_count>0) {
				int new_index = priv->selected_entry_idx+1;
				new_index = new_index%entry_count;
				dra_auto_complete_panel_set_selected_index(panel, new_index);
			}
			return TRUE;
		}
		case GDK_KEY_Page_Up : {
			if (entry_count>0) {
				int view_height = gtk_widget_get_allocated_height(gwidget);
				int entry_height = l_get_entry_height(gwidget);

				int new_index = priv->selected_entry_idx-(view_height/entry_height);
				if (new_index<0) {
					new_index = 0;
				}
				dra_auto_complete_panel_set_selected_index(panel, new_index);
			}
			return TRUE;
		}

		case GDK_KEY_Page_Down : {
			if (entry_count>0) {
				int view_height = gtk_widget_get_allocated_height(gwidget);
				int entry_height = l_get_entry_height(gwidget);

				int new_index = priv->selected_entry_idx+(view_height/entry_height);
				if (new_index>=entry_count) {
					new_index = entry_count-1;
				}
				dra_auto_complete_panel_set_selected_index(panel, new_index);
			}
			return TRUE;
		}

		case GDK_KEY_KP_Enter :
		case GDK_KEY_Return : {
			GtkWidget *top = gtk_widget_get_toplevel(gwidget);
			DraAcEntry *entry = dra_ac_context_get_at(priv->ac_context, priv->selected_entry_idx);
			if (entry) {
				dra_ac_entry_run(entry, priv->ac_context);
			}
			gtk_widget_destroy(top);
			return TRUE;
		} break;
	}

	return FALSE;
}

static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data) {
	DraAutoCompletePanel *panel = DRA_AUTO_COMPLETE_PANEL(data);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);

	gdouble yoffset = gtk_adjustment_get_value(priv->vadjustment);

	int ypos = (int) (eev->y+yoffset);
	int index = ypos/l_get_entry_height(gwidget);
	if (index>=0 && index<dra_ac_context_count(priv->ac_context)) {
		dra_auto_complete_panel_set_selected_index(panel, index);
//		panel->selected_entry_idx = index;
//		gdk_window_invalidate_rect(gtk_widget_get_window(GTK_WIDGET(panel)), NULL, TRUE);
	}
	return TRUE;
}

static gboolean l_button_release_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data) {
	DraAutoCompletePanel *panel = DRA_AUTO_COMPLETE_PANEL(data);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);

	gdouble yoffset = gtk_adjustment_get_value(priv->vadjustment);
	int ypos = (int) (eev->y+yoffset);
	int index = ypos/l_get_entry_height(gwidget);
	if (index>=0 && index<dra_ac_context_count(priv->ac_context)) {
		GtkWidget *top = gtk_widget_get_toplevel(gwidget);
		if (index==priv->selected_entry_idx) {
			DraAcEntry *entry = dra_ac_context_get_at(priv->ac_context, priv->selected_entry_idx);
			if (entry) {
				dra_ac_entry_run(entry, priv->ac_context);
			}
			gtk_widget_destroy(top);
		} else {
			cat_log_debug("allowing focus");
			gtk_widget_set_can_focus((gwidget), TRUE);
			gtk_widget_set_can_default((gwidget), TRUE);
			gtk_widget_set_can_focus(GTK_WIDGET(top), TRUE);
			gtk_widget_set_can_default(GTK_WIDGET(top), TRUE);

			gtk_widget_grab_focus(gwidget);

		}
	}
	return TRUE;
}


static void l_adjustment_value_changed(GtkAdjustment *adjustment, DraAutoCompletePanel *panel) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	cat_log_debug("adjustment=%p", adjustment);
	if (adjustment == priv->hadjustment) {
//		int dx = priv->xoffset;
//		priv->xoffset = gtk_adjustment_get_value(adjustment);
//		dx = dx - priv->xoffset;
//		cat_log_debug("dx=%d", dx);
		if (gtk_widget_get_realized(GTK_WIDGET(panel))) {
			GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(panel));
		    gdk_window_invalidate_rect(window, NULL, FALSE);
//		    gdk_window_scroll(window, dx, 0);
		}
	} else if (adjustment == priv->vadjustment) {
//		int dy = priv->yoffset;
//		priv->yoffset = gtk_adjustment_get_value(adjustment);
//		dy = dy - priv->yoffset;
//		cat_log_debug("dy=%d", dy);
		if (gtk_widget_get_realized(GTK_WIDGET(panel))) {
			GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(panel));
		    gdk_window_invalidate_rect(window, NULL, FALSE);
//		    gdk_window_scroll(window, 0, dy);
		}
	}
}



static void l_set_vadjustment_values(DraAutoCompletePanel *panel) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	int height = gtk_widget_get_allocated_height(GTK_WIDGET(panel));
	gdouble old_value = gtk_adjustment_get_value (priv->vadjustment);
	gdouble new_upper = (gdouble) (height > priv->view_height ? height : priv->view_height);

	cat_log_debug("height=%d, new_upper=%lf, view_height=%d", height, new_upper, priv->view_height);

	g_object_set(priv->vadjustment,
			"lower", 0.0,
			"upper", new_upper,
			"page-size", (gdouble) height,
			"step-increment", height * 0.1,
			"page-increment", height * 0.9,
			NULL);

	gdouble new_value = CLAMP (old_value, 0, new_upper-height);
	if (new_value != old_value) {
		gtk_adjustment_set_value(priv->vadjustment, new_value);
	}
}



static void l_set_vadjustment(DraAutoCompletePanel *panel, GtkAdjustment *adjustment) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	if (adjustment && priv->vadjustment == adjustment) {
		return;
	}

	if (priv->vadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(priv->vadjustment, l_adjustment_value_changed, panel);
		g_object_unref(priv->vadjustment);
	}

	if (adjustment == NULL) {
		adjustment = gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(l_adjustment_value_changed), panel);
	priv->vadjustment = cat_ref_sink_ptr(adjustment);
	l_set_vadjustment_values(panel);

	g_object_notify(G_OBJECT(panel), "vadjustment");
}






static void l_set_hadjustment_values(DraAutoCompletePanel *panel) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	int width = gtk_widget_get_allocated_width(GTK_WIDGET(panel));
	gdouble old_value = gtk_adjustment_get_value (priv->hadjustment);
	gdouble new_upper = (gdouble) (width > priv->view_width ? width : priv->view_width);

	g_object_set(priv->hadjustment,
			"lower", 0.0,
			"upper", new_upper,
			"page-size", (gdouble) width,
			"step-increment", width * 0.1,
			"page-increment", width * 0.9,
			NULL);

	gdouble new_value = CLAMP (old_value, 0, new_upper - width);
	if (new_value != old_value) {
		gtk_adjustment_set_value(priv->hadjustment, new_value);
	}
}


static void l_set_hadjustment(DraAutoCompletePanel *panel, GtkAdjustment *adjustment) {
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);
	if (adjustment && priv->hadjustment == adjustment) {
		return;
	}

	if (priv->hadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(priv->hadjustment, l_adjustment_value_changed, panel);
		g_object_unref(priv->hadjustment);
	}

	if (adjustment == NULL) {
		adjustment = gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(l_adjustment_value_changed), panel);
	priv->hadjustment = cat_ref_sink_ptr(adjustment);
	l_set_hadjustment_values(panel);

	g_object_notify(G_OBJECT(panel), "hadjustment");
}


static void l_widget_size_allocate_cb(GtkWidget *widget, GdkRectangle *allocation, gpointer data) {
	DraAutoCompletePanel *completion_panel = DRA_AUTO_COMPLETE_PANEL(widget);
	l_set_hadjustment_values(completion_panel);
	l_set_vadjustment_values(completion_panel);
}


static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	DraAutoCompletePanel *panel = DRA_AUTO_COMPLETE_PANEL(object);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);

	switch (prop_id) {
		case PROP_HADJUSTMENT : {
			l_set_hadjustment(panel, g_value_get_object(value));
		} break;
		case PROP_VADJUSTMENT : {
			l_set_vadjustment(panel, g_value_get_object(value));
		} break;
		case PROP_HSCROLL_POLICY : {
			priv->hscroll_policy = g_value_get_enum (value);
			gtk_widget_queue_resize(GTK_WIDGET(panel));
		} break;
		case PROP_VSCROLL_POLICY : {
			priv->vscroll_policy = g_value_get_enum (value);
			gtk_widget_queue_resize(GTK_WIDGET(panel));
		} break;
		default : {
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		} break;
	}
}


static void l_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	DraAutoCompletePanel *panel = DRA_AUTO_COMPLETE_PANEL(object);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);

	switch (prop_id) {
		case PROP_HADJUSTMENT : {
			g_value_set_object (value, priv->hadjustment);
		} break;
		case PROP_VADJUSTMENT : {
			g_value_set_object (value, priv->vadjustment);
		} break;
		case PROP_HSCROLL_POLICY : {
			g_value_set_enum(value, priv->hscroll_policy);
		} break;
		case PROP_VSCROLL_POLICY : {
			g_value_set_enum(value, priv->vscroll_policy);
		} break;
		default: {
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		} break;
	}
}



static void l_entry_list_changed(DraIAcContextListener *self) {
	DraAutoCompletePanel *panel = DRA_AUTO_COMPLETE_PANEL(self);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(panel);

	int twidth, theight;

	l_get_layout_dimensions(panel, &twidth, &theight);

	if (priv->view_width!=twidth || priv->view_height!=theight) {
		priv->view_width = twidth;
		priv->view_height = theight;

		cat_log_debug("dimensions[%d, %d]", twidth, theight);
		gtk_widget_set_size_request(GTK_WIDGET(panel), twidth, theight);
	}
	GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(panel));
    gdk_window_invalidate_rect(window, NULL, FALSE);
}




static void l_context_listener_iface_init(DraIAcContextListenerInterface *iface) {
	iface->entryListChanged = l_entry_list_changed;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraAutoCompletePanel *instance = DRA_AUTO_COMPLETE_PANEL(self);
	DraAutoCompletePanelPrivate *priv = dra_auto_complete_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p selected_entry_idx=%d]", iname, self, priv->selected_entry_idx);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
