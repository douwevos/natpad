/*
   File:    elkcompletionpanel.c
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 18, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "elkcompletionpanel.h"
#include <gdk/gdkkeysyms.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkCompletionPanel"
#include <logging/catlog.h>

enum {
	PROP_0,
	PROP_HADJUSTMENT,
	PROP_VADJUSTMENT,
	PROP_HSCROLL_POLICY,
	PROP_VSCROLL_POLICY
};

static void l_scrollable_iface_init(GtkScrollableInterface *iface) {
}

G_DEFINE_TYPE_WITH_CODE(ElkCompletionPanel, elk_completion_panel, GTK_TYPE_DRAWING_AREA, {
		G_IMPLEMENT_INTERFACE(GTK_TYPE_SCROLLABLE, l_scrollable_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void _get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

static gboolean _widget_draw(GtkWidget *widget, cairo_t *cairo);
static gboolean _widget_focus_out_event(GtkWidget *widget, GdkEventFocus *event);
static void _widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gboolean _widget_scroll_event(GtkWidget *widget, GdkEventScroll *event);
static void _widget_realize(GtkWidget *widget);


static gboolean _handle_key_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer panel_as_obj);
static gboolean _button_press_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer _obj);
static gboolean _button_release_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer _obj);

static gboolean l_button_release_event(GtkWidget *widget, GdkEventButton *event);


static void _set_hadjustment(ElkCompletionPanel *panel, GtkAdjustment *adjustment);
static void _set_hadjustment_values(ElkCompletionPanel *panel);

static void _set_vadjustment(ElkCompletionPanel *panel, GtkAdjustment *adjustment);
static void _set_vadjustment_values(ElkCompletionPanel *panel);

static void _adjustment_value_changed(GtkAdjustment *adjustment, ElkCompletionPanel *panel);


static void elk_completion_panel_class_init(ElkCompletionPanelClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->set_property = _set_property;
	object_class->get_property = _get_property;
//	GtkDrawingArea *sub_clazz = GTK_DRAWING_AREA_CLAZZ(clazz);

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->draw = _widget_draw;
//	widget_class->focus = _widget_focus;
	widget_class->focus_out_event = _widget_focus_out_event;
	widget_class->size_allocate = _widget_size_allocate;
	widget_class->scroll_event = _widget_scroll_event;
	widget_class->realize = _widget_realize;


	widget_class->button_release_event = l_button_release_event;

	g_object_class_override_property(object_class, PROP_HADJUSTMENT,    "hadjustment");
	g_object_class_override_property(object_class, PROP_VADJUSTMENT,    "vadjustment");
	g_object_class_override_property(object_class, PROP_HSCROLL_POLICY, "hscroll-policy");
	g_object_class_override_property(object_class, PROP_VSCROLL_POLICY, "vscroll-policy");

}

static void elk_completion_panel_init(ElkCompletionPanel *panel) {
	panel->view_height = 0;
	panel->view_width = 0;
	panel->hadjustment = NULL;
	panel->vadjustment = NULL;
	panel->xoffset = 0;
	panel->yoffset = 0;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkCompletionPanel *instance = ELK_COMPLETION_PANEL(object);
	cat_unref_ptr(instance->a_entries);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("end-dispose:%p", object);
}



static int l_get_entry_height(GtkWidget *widget) {
	PangoContext *pango_context = gtk_widget_get_pango_context(widget);
	PangoFontMetrics *font_metrics = pango_context_get_metrics(pango_context, NULL, NULL);
	int font_height = (pango_font_metrics_get_ascent(font_metrics)+pango_font_metrics_get_descent(font_metrics))/PANGO_SCALE;
	pango_font_metrics_unref(font_metrics);
	return font_height+2;
}


static void _calulate_dimensions(ElkCompletionPanel *completion_panel, int *twidth, int *theight) {
	GtkWidget *widget = GTK_WIDGET(completion_panel);
	PangoContext *pango_context = gtk_widget_get_pango_context(widget);
	PangoLayout *pango_layout = pango_layout_new(pango_context);

	int max_width = 10;
	CatIIterator *iterator = cat_array_wo_iterator(completion_panel->a_entries);
	while(cat_iiterator_has_next(iterator)) {
		ElkCompletionEntry *entry = (ElkCompletionEntry *) cat_iiterator_next(iterator);
		int width = elk_completion_entry_get_width(entry, pango_layout);
		if (width>max_width) {
			max_width = width;
		}
	}
	cat_unref_ptr(iterator);

	*twidth = max_width+4;

	int entry_height = l_get_entry_height(widget);

	*theight = entry_height*cat_array_wo_size(completion_panel->a_entries);
}



ElkCompletionPanel *elk_completion_panel_new(LeaPanel *editor_panel, CatArrayWo *entries) {
	ElkCompletionPanel *result = g_object_new(ELK_TYPE_COMPLETION_PANEL, NULL);
	cat_ref_anounce(result);
	result->a_entries = cat_ref_ptr(entries);
	result->selected_entry_idx = 0;
	result->editor_panel = editor_panel;

	int twidth, theight;

	_calulate_dimensions(result, &twidth, &theight);

	result->view_width = twidth;
	result->view_height = theight;


	cat_log_debug("dimensions[%d, %d]", twidth, theight);
	gtk_widget_set_size_request(GTK_WIDGET(result), twidth, theight);


	gtk_widget_set_can_focus(GTK_WIDGET(result), TRUE);
	gtk_widget_set_can_default(GTK_WIDGET(result), TRUE);


	g_signal_connect(result, "key-press-event", G_CALLBACK(_handle_key_event), result);
	g_signal_connect(result, "button-press-event", G_CALLBACK(_button_press_event), result);
	g_signal_connect(result, "button-release-event", G_CALLBACK(_button_release_event), result);


	gtk_widget_add_events((GtkWidget *) result, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

	return result;
}




static gboolean _widget_draw(GtkWidget *widget, cairo_t *cairo) {
	ElkCompletionPanel *panel = ELK_COMPLETION_PANEL(widget);

	GtkAllocation alloc;
	gtk_widget_get_allocation(widget, &alloc);

	GdkRGBA gdkRgba;


	GtkStyleContext *style_context = gtk_widget_get_style_context(gtk_widget_get_parent(widget));
	gtk_style_context_get(style_context, GTK_STATE_NORMAL,"background-color", &gdkRgba, NULL);

	cairo_save(cairo);


	cairo_set_source_rgb(cairo, 1.0, 1.0, 0.7);
	cairo_rectangle(cairo, 0,0, alloc.width, alloc.height);
	cairo_fill(cairo);

	cairo_translate(cairo, -panel->xoffset, -panel->yoffset);

//	color.red = 0xFFFF*0.0;
//	color.green = 0xFFFF*0.0;
//	color.blue = 0xFFFF*0.2;
//	color.pixel = 0;
//
//	gdk_gc_set_foreground(gc, &color);
//	if (colormap) {
//		gdk_rgb_find_color(colormap, &color);
//	}
//
//	gdk_gc_set_foreground(gc, &color);


	PangoContext *pango_context = gtk_widget_get_pango_context(widget);
	PangoLayout *pango_layout = pango_layout_new(pango_context);

	cairo_set_source_rgb(cairo, 0, 0, 0.2);
	pango_cairo_update_layout(cairo, pango_layout);

//	cairo_set_source_rgb(cairo, 1, 1, 0.2);

	int entry_height = l_get_entry_height(widget);

	int count = cat_array_wo_size(panel->a_entries);
	int idx;
	for(idx=0; idx<count; idx++) {
		ElkCompletionEntry *completion_entry = (ElkCompletionEntry *) cat_array_wo_get(panel->a_entries, idx);
		int ypos = idx*entry_height;
		gboolean selected = panel->selected_entry_idx==idx;
		elk_completion_entry_paint(widget, completion_entry, cairo, pango_layout, ypos, entry_height, selected);
	}

	cairo_set_source_rgb(cairo, 1, 1, 0.2);

	cairo_restore(cairo);
	return TRUE;
}


static gboolean _widget_focus_out_event(GtkWidget *widget, GdkEventFocus *event) {
//	GtkWidget *top = gtk_widget_get_toplevel(widget);
//	gtk_widget_destroy(top);
	return TRUE;
}

static void _widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
//	gtk_widget_set_allocation(widget, allocation);
//
//	if (gtk_widget_get_realized(widget)) {
//		gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x+1, allocation->y+1, allocation->width, allocation->height);
//	}
//	GtkWidgetClass *wclass = GTK_WIDGET_CLASS(parent_class);
	GTK_WIDGET_CLASS(parent_class)->size_allocate(widget, allocation);

	ElkCompletionPanel *completion_panel = ELK_COMPLETION_PANEL(widget);
	_set_hadjustment_values(completion_panel);
	_set_vadjustment_values(completion_panel);

}



static gboolean _widget_scroll_event(GtkWidget *widget, GdkEventScroll *event) {

	g_return_val_if_fail (ELK_IS_COMPLETION_PANEL(widget), FALSE);
	g_return_val_if_fail (event != NULL, FALSE);

	ElkCompletionPanel *completion_panel = ELK_COMPLETION_PANEL(widget);

	GtkAdjustment *adjustment = NULL;
//	GtkWidget *range;
	if (event->direction == GDK_SCROLL_UP || event->direction == GDK_SCROLL_DOWN) {
		adjustment = completion_panel->vadjustment;
	} else {
		adjustment = completion_panel->hadjustment;
	}

	if (adjustment) {
		gdouble delta, new_value;

		delta = 3*l_get_entry_height(widget) * 3;
		if (event->direction == GDK_SCROLL_UP) {
			delta = -delta;
		}

		new_value = CLAMP (gtk_adjustment_get_value(adjustment) + delta, gtk_adjustment_get_lower(adjustment), gtk_adjustment_get_upper(adjustment)- gtk_adjustment_get_page_size(adjustment));
		gtk_adjustment_set_value(adjustment, new_value);
		return TRUE;
	}
	return FALSE;

}


static void _widget_realize(GtkWidget *widget) {
	GTK_WIDGET_CLASS(parent_class)->realize(widget);
	GdkWindow *window = gtk_widget_get_window(widget);
	/* We select on button_press_mask so that button 4-5 scrolls (mouse-wheel) are trapped */
	gdk_window_set_events(window, gdk_window_get_events(window) | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
}



static gboolean _handle_key_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer panel_as_obj) {
	ElkCompletionPanel *completion_panel = ELK_COMPLETION_PANEL(panel_as_obj);
	int key_val = eev->keyval;

	cat_log_debug("keyval:%x", key_val);

	switch(key_val) {
		case GDK_KEY_Escape : {
			cat_log_debug("destroying popup");
			GtkWidget *top = gtk_widget_get_toplevel(gwidget);
			gtk_widget_destroy(top);
			return TRUE;
		}
		case GDK_KEY_Up : {
			if (cat_array_wo_size(completion_panel->a_entries)>0) {
				int new_index = completion_panel->selected_entry_idx-1;
				if (new_index<0) {
					new_index = cat_array_wo_size(completion_panel->a_entries)-1;
				}

				elk_completion_panel_set_selected_index(completion_panel, new_index);
			}
			return TRUE;
		}
		case GDK_KEY_Down : {
			if (cat_array_wo_size(completion_panel->a_entries)>0) {
				int new_index = completion_panel->selected_entry_idx+1;
				new_index = new_index%cat_array_wo_size(completion_panel->a_entries);
				elk_completion_panel_set_selected_index(completion_panel, new_index);
			}
			return TRUE;
		}
		case GDK_KEY_Page_Up : {
			if (cat_array_wo_size(completion_panel->a_entries)>0) {
				int view_height = gtk_widget_get_allocated_height(gwidget);
				int entry_height = l_get_entry_height(gwidget);

				int new_index = completion_panel->selected_entry_idx-(view_height/entry_height);
				if (new_index<0) {
					new_index = 0;
				}
				elk_completion_panel_set_selected_index(completion_panel, new_index);
			}
			return TRUE;
		}

		case GDK_KEY_Page_Down : {
			if (cat_array_wo_size(completion_panel->a_entries)>0) {
				int view_height = gtk_widget_get_allocated_height(gwidget);
				int entry_height = l_get_entry_height(gwidget);

				int new_index = completion_panel->selected_entry_idx+(view_height/entry_height);
				if (new_index>=cat_array_wo_size(completion_panel->a_entries)) {
					new_index = cat_array_wo_size(completion_panel->a_entries)-1;
				}
				elk_completion_panel_set_selected_index(completion_panel, new_index);
			}
			return TRUE;
		}

		case GDK_KEY_KP_Enter :
		case GDK_KEY_Return : {
			GtkWidget *top = gtk_widget_get_toplevel(gwidget);
			ElkCompletionEntry *entry = (ElkCompletionEntry *) cat_array_wo_get(completion_panel->a_entries, completion_panel->selected_entry_idx);
			if (entry) {
				elk_completion_entry_run(entry, completion_panel->editor_panel);
			}
			gtk_widget_destroy(top);
			return TRUE;
		} break;
	}

	return FALSE;
}


static gboolean _button_press_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer _obj) {
	ElkCompletionPanel *panel = ELK_COMPLETION_PANEL(_obj);

	int ypos = (int) (eev->y+panel->yoffset);
	int index = ypos/l_get_entry_height(gwidget);
	if (index>=0 && index<cat_array_wo_size(panel->a_entries)) {
		elk_completion_panel_set_selected_index(panel, index);
//		panel->selected_entry_idx = index;
//		gdk_window_invalidate_rect(gtk_widget_get_window(GTK_WIDGET(panel)), NULL, TRUE);
	}
	return TRUE;
}

static gboolean _button_release_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer _obj) {
	ElkCompletionPanel *panel = ELK_COMPLETION_PANEL(_obj);

	int ypos = (int) (eev->y+panel->yoffset);
	int index = ypos/l_get_entry_height(gwidget);
	if (index>=0 && index<cat_array_wo_size(panel->a_entries)) {
		GtkWidget *top = gtk_widget_get_toplevel(gwidget);
		if (index==panel->selected_entry_idx) {
			ElkCompletionEntry *entry = (ElkCompletionEntry *) cat_array_wo_get(panel->a_entries, panel->selected_entry_idx);
			if (entry) {
				elk_completion_entry_run(entry, panel->editor_panel);
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


static gboolean l_button_release_event(GtkWidget *widget, GdkEventButton *event) {
	return _button_release_event(widget, event, widget);
}



static void _set_hadjustment(ElkCompletionPanel *panel, GtkAdjustment *adjustment) {
	if (adjustment && panel->hadjustment == adjustment) {
		return;
	}

	if (panel->hadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(panel->hadjustment, _adjustment_value_changed, panel);
		g_object_unref(panel->hadjustment);
	}

	if (adjustment == NULL) {
		adjustment = gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(_adjustment_value_changed), panel);
	panel->hadjustment = cat_ref_sink_ptr(adjustment);
	_set_hadjustment_values(panel);

	g_object_notify(G_OBJECT(panel), "hadjustment");
}


static void _set_hadjustment_values(ElkCompletionPanel *panel) {
	int width = gtk_widget_get_allocated_width(GTK_WIDGET(panel));
	gdouble old_value = gtk_adjustment_get_value (panel->hadjustment);
//	gdouble new_upper = (gdouble) width;
	//  new_upper = MAX (screen_width, priv->width);
	gdouble new_upper = (gdouble) (width > panel->view_width ? width : panel->view_width);

	g_object_set(panel->hadjustment,
			"lower", 0.0,
			"upper", new_upper,
			"page-size", (gdouble) width,
			"step-increment", width * 0.1,
			"page-increment", width * 0.9,
			NULL);

	gdouble new_value = CLAMP (old_value, 0, new_upper - width);
	if (new_value != old_value) {
		gtk_adjustment_set_value(panel->hadjustment, new_value);
	}
}





static void _set_vadjustment(ElkCompletionPanel *panel, GtkAdjustment *adjustment) {
	if (adjustment && panel->vadjustment == adjustment) {
		return;
	}

	if (panel->vadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(panel->vadjustment, _adjustment_value_changed, panel);
		g_object_unref(panel->vadjustment);
	}

	if (adjustment == NULL) {
		adjustment = gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(_adjustment_value_changed), panel);
	panel->vadjustment = cat_ref_sink_ptr(adjustment);
	_set_vadjustment_values(panel);

	g_object_notify(G_OBJECT(panel), "vadjustment");
}


static void _set_vadjustment_values(ElkCompletionPanel *panel) {
	int height = gtk_widget_get_allocated_height(GTK_WIDGET(panel));
	gdouble old_value = gtk_adjustment_get_value (panel->vadjustment);
	gdouble new_upper = (gdouble) (height > panel->view_height ? height : panel->view_height);
	//  new_upper = MAX (screen_width, priv->width);

	cat_log_debug("height=%d, new_upper=%lf, view_height=%d", height, new_upper, panel->view_height);

	g_object_set(panel->vadjustment,
			"lower", 0.0,
			"upper", new_upper,
			"page-size", (gdouble) height,
			"step-increment", height * 0.1,
			"page-increment", height * 0.9,
			NULL);

	gdouble new_value = CLAMP (old_value, 0, new_upper-height);
	if (new_value != old_value) {
		gtk_adjustment_set_value(panel->vadjustment, new_value);
	}
}





static void _adjustment_value_changed(GtkAdjustment *adjustment, ElkCompletionPanel *panel) {
	cat_log_debug("adjustment=%p", adjustment);
	if (adjustment == panel->hadjustment) {
		int dx = panel->xoffset;
		panel->xoffset = gtk_adjustment_get_value(adjustment);
		dx = dx - panel->xoffset;
		cat_log_debug("dx=%d", dx);
		if (gtk_widget_get_realized(GTK_WIDGET(panel))) {
			GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(panel));
		    gdk_window_invalidate_rect(window, NULL, FALSE);
//		    gdk_window_scroll(window, dx, 0);
		}
	} else if (adjustment == panel->vadjustment) {
		int dy = panel->yoffset;
		panel->yoffset = gtk_adjustment_get_value(adjustment);
		dy = dy - panel->yoffset;
		cat_log_debug("dy=%d", dy);
		if (gtk_widget_get_realized(GTK_WIDGET(panel))) {
			GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(panel));
		    gdk_window_invalidate_rect(window, NULL, FALSE);
//		    gdk_window_scroll(window, 0, dy);
		}
	}
}


static void _set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	ElkCompletionPanel *panel = ELK_COMPLETION_PANEL(object);

	switch (prop_id) {
		case PROP_HADJUSTMENT : {
			_set_hadjustment(panel, g_value_get_object(value));
		} break;
		case PROP_VADJUSTMENT : {
			_set_vadjustment(panel, g_value_get_object(value));
		} break;
		case PROP_HSCROLL_POLICY : {
			panel->hscroll_policy = g_value_get_enum (value);
			gtk_widget_queue_resize(GTK_WIDGET(panel));
		} break;
		case PROP_VSCROLL_POLICY : {
			panel->vscroll_policy = g_value_get_enum (value);
			gtk_widget_queue_resize(GTK_WIDGET(panel));
		} break;
		default : {
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		} break;
	}
}


static void _get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	ElkCompletionPanel *panel = ELK_COMPLETION_PANEL(object);

	switch (prop_id) {
		case PROP_HADJUSTMENT : {
			g_value_set_object (value, panel->hadjustment);
		} break;
		case PROP_VADJUSTMENT : {
			g_value_set_object (value, panel->vadjustment);
		} break;
		case PROP_HSCROLL_POLICY : {
			g_value_set_enum(value, panel->hscroll_policy);
		} break;
		case PROP_VSCROLL_POLICY : {
			g_value_set_enum(value, panel->vscroll_policy);
		} break;
		default: {
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		} break;
	}
}


void elk_completion_panel_set_selected_index(ElkCompletionPanel *panel, int index) {
	if (panel->selected_entry_idx == index) {
		return;
	}
	if (index<0 || index>= cat_array_wo_size(panel->a_entries)) {
		return;
	}

	panel->selected_entry_idx = index;
	int entry_height = l_get_entry_height(GTK_WIDGET(panel));
	int offset = index*entry_height;
	if (offset<panel->yoffset) {
		gtk_adjustment_set_value(panel->vadjustment, offset);
	}

	offset+=entry_height;

	int view_height = gtk_widget_get_allocated_height(GTK_WIDGET(panel));

	if (offset>=panel->yoffset+view_height) {
//		panel->yoffset = offset-view_height;
		gtk_adjustment_set_value(panel->vadjustment, offset-view_height);
	}
	GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(panel));
	gdk_window_invalidate_rect(window, NULL, FALSE);
}

