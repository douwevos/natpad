/*
   File:    chaeditor.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 23, 2015
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

#include "chaeditor.h"
#include "layout/chablinker.h"
#include "uow/chauow.h"
#include "uow/chauowselect.h"
#include "uow/chauowinserttext.h"
#include "uow/chauowreplacetext.h"
#include "uow/chauowinserttemplate.h"
#include "uow/chauowlinesmove.h"
#include "uow/chauowlinesremove.h"
#include "uow/chauowmovecursor.h"
#include "uow/chauowremovetext.h"
#include "uow/chauowclipboard.h"
#include "uow/chauowlinemarker.h"
#include "uow/chauowundoredo.h"

#include "search/chasearchservice.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaEditor"
#include <logging/catlog.h>

enum {
  PROP_0,
  PROP_HADJUSTMENT,
  PROP_VADJUSTMENT,
  PROP_HSCROLL_POLICY,
  PROP_VSCROLL_POLICY,
};

struct _ChaEditorPrivate {
	ChaDocument *document;
	ChaPreferencesWo *a_preferences;
	ChaDocumentView *document_view;
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;

	gboolean mouse_down;

	int hscroll_policy : 1;
	int vscroll_policy : 1;

	GdkWindow *text_window;
	GdkCursor *cursor;

	PangoContext *pango_context;
	PangoFontDescription *pango_font_desc;

	gboolean depreacated_scrolling;

	ChaSearchService *search_service;

//	int font_height;

	ChaBlinker *blinker;

	CatWeakList *uow_listeners;


	GtkIMContext *im_context;
	gboolean need_im_reset;

	gboolean need_marker_out;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaEditor, cha_editor, GTK_TYPE_WIDGET,
		G_ADD_PRIVATE(ChaEditor)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(GTK_TYPE_SCROLLABLE, NULL)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void l_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void l_widget_realize(GtkWidget *widget);
static void l_widget_unrealize(GtkWidget *widget);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr);
static gboolean l_widget_focus(GtkWidget *widget, GtkDirectionType direction);
static void l_editor_toggle_word_wrap(ChaEditor *editor);
static void l_editor_toggle_show_whitespace(ChaEditor *editor);
static void l_editor_toggle_mark_occurrences(ChaEditor *editor);

static void cha_editor_class_init(ChaEditorClass *clazz) {
	clazz->initializeDocumentView = NULL;
	clazz->markerClicked = NULL;
	clazz->markerOver = NULL;
	clazz->markerOut = NULL;
	clazz->toggleWordWrap = l_editor_toggle_word_wrap;
	clazz->toggleShowWhitespace = l_editor_toggle_show_whitespace;
	clazz->toggleMarkOccurrences = l_editor_toggle_mark_occurrences;

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
	widget_class->draw = l_widget_draw;
	widget_class->focus = l_widget_focus;
}

static void cha_editor_init(ChaEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaEditor *instance = CHA_EDITOR(object);
	ChaEditorPrivate *priv = cha_editor_get_instance_private(instance);
	cat_unref_ptr(priv->document);
	cat_unref_ptr(priv->document_view);
	cat_unref_ptr(priv->hadjustment);
	cat_unref_ptr(priv->vadjustment);
	cat_unref_ptr(priv->a_preferences);
	cat_unref_ptr(priv->blinker);
	cat_unref_ptr(priv->pango_context);
	cat_unref_ptr(priv->search_service);
	if (priv->pango_font_desc) {
		pango_font_description_free(priv->pango_font_desc);
		priv->pango_font_desc = NULL;
	}
	cat_unref_ptr(priv->uow_listeners);
	G_OBJECT_CLASS(cha_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_key_press_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data);
static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data);
static gboolean l_button_release_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data);
static gboolean l_motion_event_cb(GtkWidget *gwidget, GdkEventMotion *eev, gpointer data);
static gboolean l_focus_in_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data);
static gboolean l_focus_out_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data);
static gboolean l_windows_state_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data);
static void l_im_context_commit_cb(GtkIMContext *context, const gchar *str, ChaEditor *editor);
static void l_im_preedit_changed_cb(GtkIMContext *context, ChaEditor *editor);
static gboolean l_im_retrieve_surrounding_cb(GtkIMContext *context, ChaEditor *editor);
//static gboolean l_im_delete_surrounding_cb(GtkIMContext *context, gint offset, gint n_chars, ChaEditor *editor);


void cha_editor_construct(ChaEditor *editor, ChaDocument *document, ChaEditorFlags flags) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	cat_log_debug("construct:editor=%p", editor);
	priv->document = cat_ref_ptr(document);
	priv->document_view = NULL;
	priv->hadjustment = NULL;
	priv->vadjustment = NULL;
	priv->pango_font_desc = NULL;
	priv->pango_context = NULL;
	priv->mouse_down = FALSE;
	priv->cursor = NULL;
	priv->depreacated_scrolling = FALSE;
	priv->uow_listeners = cat_weak_list_new();
	priv->search_service = cha_search_service_new();
	priv->im_context = gtk_im_multicontext_new();
	priv->need_im_reset = FALSE;
	priv->need_marker_out = FALSE;

	g_signal_connect(priv->im_context, "commit", G_CALLBACK(l_im_context_commit_cb), editor);
	g_signal_connect(priv->im_context, "preedit-changed", G_CALLBACK(l_im_preedit_changed_cb), editor);
	g_signal_connect (priv->im_context, "retrieve-surrounding", G_CALLBACK(l_im_retrieve_surrounding_cb), editor);
//	g_signal_connect (priv->im_context, "delete-surrounding", G_CALLBACK(l_im_delete_surrounding_cb), editor);


	GtkWidget *widget = GTK_WIDGET(editor);
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

	if ((flags & CHA_EDITOR_FLAG_DEFAULT_KEY_HANDLING)!=0) {
		g_signal_connect(editor, "key-press-event", G_CALLBACK(l_key_press_event), editor);
	}
	if ((flags & CHA_EDITOR_FLAG_DEFAULT_MOUSE_HANDLING)!=0) {
		g_signal_connect(editor, "button-press-event", G_CALLBACK(l_button_press_event_cb), editor);
		g_signal_connect(editor, "button-release-event", G_CALLBACK(l_button_release_event_cb), editor);
		g_signal_connect(editor, "motion-notify-event", G_CALLBACK(l_motion_event_cb), editor);
	}

	g_signal_connect(editor, "focus-in-event", G_CALLBACK(l_focus_in_event_cb), editor);
	g_signal_connect(editor, "focus-out-event", G_CALLBACK(l_focus_out_event_cb), editor);

	g_signal_connect(editor, "window-state-event", G_CALLBACK(l_windows_state_event_cb), editor);


	gtk_widget_set_app_paintable(widget, TRUE);
	gtk_widget_set_can_focus(widget, TRUE);
	gtk_widget_show(widget);
//	gtk_widget_grab_focus(widget);

	if ((flags & CHA_EDITOR_FLAG_NO_DEFAULT_BLINKER)==0) {
		priv->blinker = cha_blinker_new();
		cha_blinker_set_active_editor(priv->blinker, editor);
	} else {
		priv->blinker = NULL;
	}

}

ChaEditor *cha_editor_new(ChaDocument *document) {
	ChaEditor *result = g_object_new(CHA_TYPE_EDITOR, NULL);
	cat_ref_anounce(result);
	cha_editor_construct(result, document, CHA_EDITOR_FLAG_DEFAULT_KEY_HANDLING | CHA_EDITOR_FLAG_DEFAULT_MOUSE_HANDLING);
	return result;
}

ChaDocument *cha_editor_get_document(ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	return priv->document;
}

ChaDocumentView *cha_editor_get_document_view(ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	return priv->document_view;
}

void cha_editor_inform_activity(ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	cha_blinker_inform_activity(priv->blinker);
	cha_document_view_invalidate_lines(priv->document_view);
}

gboolean cha_editor_set_cursor_blink_up(ChaEditor *editor, gboolean blink_up) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	gboolean result = FALSE;
	if (priv->document_view) {
		result = cha_document_view_set_cursor_blink_up(priv->document_view, blink_up);
	}
	return result;
}



static gboolean l_windows_state_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(data));
	cha_document_view_set_has_focus(priv->document_view, gtk_widget_has_visible_focus(widget));
	return FALSE;
}


static gboolean l_focus_in_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(data));
	if (priv->document_view) {
		cha_document_view_set_has_focus(priv->document_view, TRUE);
	}

	if (priv->im_context) {
		gtk_im_context_focus_in(priv->im_context);
	}

	return FALSE;
}


static gboolean l_focus_out_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(data));
	if (priv->document_view) {
		cha_document_view_set_has_focus(priv->document_view, FALSE);
	}
	if (priv->im_context) {
		gtk_im_context_focus_out(priv->im_context);
	}

	return FALSE;
}


void cha_editor_set_preferences(ChaEditor *editor, ChaPreferencesWo *a_prefs) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	cat_log_debug("a_prefs=%O", a_prefs);
	cat_ref_swap(priv->a_preferences, a_prefs);
	if (a_prefs) {
		priv->depreacated_scrolling = cha_preferences_wo_get_deprecated_scrolling(a_prefs);
	}
	if (priv->document_view) {
		cha_document_view_set_preferences(priv->document_view, a_prefs);
	}
	if (priv->blinker) {
		if (a_prefs) {
			cha_blinker_set_blinker_active(priv->blinker, cha_preferences_wo_get_blink_cursor(a_prefs));
		}
	}
}

ChaPreferencesWo *cha_editor_get_preferences(const ChaEditor *editor) {
	const ChaEditorPrivate *priv = cha_editor_get_instance_private((ChaEditor *) editor);
	return priv->a_preferences;
}

void cha_editor_toggle_word_wrap(ChaEditor *editor) {
	CHA_EDITOR_GET_CLASS(editor)->toggleWordWrap(editor);
}

void cha_editor_toggle_show_whitespace(ChaEditor *editor) {
	CHA_EDITOR_GET_CLASS(editor)->toggleShowWhitespace(editor);
}

void cha_editor_toggle_mark_occurrences(ChaEditor *editor) {
	CHA_EDITOR_GET_CLASS(editor)->toggleMarkOccurrences(editor);
}


void cha_editor_set_search_service(ChaEditor *editor, struct _ChaSearchService *search_service) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	cat_ref_swap(priv->search_service, search_service);
}

struct _ChaSearchService *cha_editor_get_search_service(ChaEditor *editor) {
	const ChaEditorPrivate *priv = cha_editor_get_instance_private((ChaEditor *) editor);
	return priv->search_service;
}


static void l_set_hadjustment_values(ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	GtkAllocation allocation;
	gtk_widget_get_allocation(GTK_WIDGET(editor), &allocation);
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



static void l_set_vadjustment_values(ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	GtkAllocation allocation;
	gtk_widget_get_allocation(GTK_WIDGET(editor), &allocation);
	int view_height = allocation.height;

	gdouble old_value = gtk_adjustment_get_value(priv->vadjustment);
	gdouble new_upper = gtk_adjustment_get_upper(priv->vadjustment);
	if (priv->document_view) {
		long long int layout_height = cha_document_view_get_layout_height(priv->document_view);
		new_upper = MAX(layout_height, view_height);
	}
	cat_log_trace("view_height=%d, new_upper=%d", view_height, (int) new_upper);
//  gdouble new_upper = MAX (screen_width, priv->width);
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


static void l_deprecated_scroll(ChaEditor *editor, int dx, int dy) {
	GtkWidget *widget = (GtkWidget *) editor;
	GdkWindow *window = gtk_widget_get_window(widget);
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);

	const ChaDocumentViewContext vc = cha_document_view_get_context(priv->document_view);

	int height = gtk_widget_get_allocated_height(widget);
	int width = gtk_widget_get_allocated_width(widget);

	cairo_region_t *region = gdk_window_get_clip_region(window);
	GdkDrawingContext *drawing_context = gdk_window_begin_draw_frame(window, region);
	cairo_t *cr = gdk_drawing_context_get_cairo_context(drawing_context);

	/* clipping restricts the intermediate surface's size, so it's a good idea to use it. */
	GdkRectangle area;
	area.x = 0;
	area.y = 0;
	area.width = width;
	area.height = height;
	gdk_cairo_rectangle (cr, &area);

	cairo_clip (cr);
	cat_log_on_debug({
		cairo_rectangle_list_t *rect_list = cairo_copy_clip_rectangle_list(cr);
		if (rect_list) {
			cat_log_debug("rects=%d", rect_list->num_rectangles);
			int ridx;
			for(ridx=0; ridx<rect_list->num_rectangles; ridx++) {
				cairo_rectangle_t *rect = rect_list->rectangles+ridx;
				cat_log_debug("rect[%d]=%d,%d,%d,%d", ridx, (int) rect->x, (int) rect->height, (int) rect->width, (int) rect->height);
			}
		} else {
			cat_log_debug("no rectangles");
		}
	});


	/* Now push a group to change the target */
	cairo_push_group (cr);
	gdk_cairo_set_source_window(cr, window, dx, dy);

//	cairo_set_source_surface(cr, cairo_get_target(cr), -dx, -dy);
//	cairo_set_source_surface (cr, surface, dx, dy);
	cairo_paint (cr);
	/* Now copy the intermediate target back */
	cairo_pop_group_to_source (cr);
	cairo_paint (cr);
	gdk_window_end_draw_frame(window, drawing_context);

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	gdk_window_flush(window);
	G_GNUC_END_IGNORE_DEPRECATIONS


    if (!cha_document_view_check_cache_boundaries(priv->document_view)) {
    	cha_document_view_invalidate_lines(priv->document_view);
    }

	GdkRectangle invalid_rect;
	if (dy!=0) {
		invalid_rect.x = 0;
		invalid_rect.width = width;
		if (dy>0) {
			invalid_rect.y = 0;
			invalid_rect.height = dy+vc.line_height;
		} else {
			dy = dy-vc.line_height;
			invalid_rect.y = height + dy;
			invalid_rect.height = -dy+1;
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

	cat_log_debug("invalid_rect.y=%d, height=%d", invalid_rect.y, invalid_rect.height);
	gdk_window_invalidate_rect(window, &invalid_rect, TRUE);

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	gdk_window_process_updates(window, TRUE);
	G_GNUC_END_IGNORE_DEPRECATIONS

	while ((gtk_events_pending()) != 0) {
		gtk_main_iteration();
	}
}

static void l_hadjustment_value_changed(GtkAdjustment *adjustment, ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	glong aval = (glong) gtk_adjustment_get_value(adjustment);
	glong old = cha_document_view_set_view_x(priv->document_view, aval);
    glong dx = old - aval;
	if (priv->depreacated_scrolling) {
		l_deprecated_scroll(editor, dx, 0);
	} else {
		GdkWindow *window = gtk_widget_get_window((GtkWidget *) editor);
		cha_document_view_invalidate_lines(priv->document_view);
		gdk_window_scroll(window, dx, 0);
	}
}


static void l_vadjustment_value_changed(GtkAdjustment *adjustment, ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	glong aval = (glong) gtk_adjustment_get_value(adjustment);

//	int s = (int) (aval % priv->font_height);
//	if (s>priv->font_height/2) {
//		s -= priv->font_height;
//	}
//	aval = aval - s;

	glong old = cha_document_view_set_view_y(priv->document_view, aval);
    glong dy = old - aval;
    cat_log_debug("scroll-v:%d, %d", (int) dy, aval);

    if (priv->depreacated_scrolling) {
    	l_deprecated_scroll(editor, 0, dy);
    } else {
		GdkWindow *window = gtk_widget_get_window((GtkWidget *) editor);
		cha_document_view_set_in_scroll(priv->document_view, TRUE);
		if (!cha_document_view_check_cache_boundaries(priv->document_view)) {
			cha_document_view_invalidate_lines(priv->document_view);
		}
		gdk_window_scroll(window, 0, dy);
		cha_document_view_set_in_scroll(priv->document_view, FALSE);
    }
}


static void l_set_hadjustment(ChaEditor *editor, GtkAdjustment *adjustment) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);

	if (adjustment && priv->hadjustment == adjustment) {
	    return;
	}

	if (priv->hadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(priv->hadjustment, l_hadjustment_value_changed, editor);
		g_object_unref(priv->hadjustment);
	}

	if (adjustment == NULL) {
	    adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(l_hadjustment_value_changed), editor);
	priv->hadjustment = cat_ref_sink_ptr(adjustment);
	if (priv->document_view) {
		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, priv->vadjustment);
	}
	l_set_hadjustment_values(editor);
	g_object_notify(G_OBJECT(editor), "hadjustment");
}


static void l_set_vadjustment(ChaEditor *editor, GtkAdjustment *adjustment) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);

	if (adjustment && priv->vadjustment == adjustment) {
	    return;
	}

	if (priv->vadjustment != NULL) {
		g_signal_handlers_disconnect_by_func(priv->vadjustment, l_vadjustment_value_changed, editor);
		g_object_unref(priv->vadjustment);
	}

	if (adjustment == NULL) {
	    adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}

	g_signal_connect(adjustment, "value-changed", G_CALLBACK(l_vadjustment_value_changed), editor);
	priv->vadjustment = cat_ref_sink_ptr(adjustment);
	if (priv->document_view) {
		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, priv->vadjustment);
	}
	l_set_vadjustment_values(editor);
	g_object_notify(G_OBJECT(editor), "vadjustment");
}



static void l_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	ChaEditor *editor = CHA_EDITOR(object);
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);

	switch (prop_id) {
		case PROP_HADJUSTMENT:
			l_set_hadjustment(editor, g_value_get_object(value));
			break;
		case PROP_VADJUSTMENT:
			l_set_vadjustment(editor, g_value_get_object(value));
			break;
		case PROP_HSCROLL_POLICY:
			if (priv->hscroll_policy != g_value_get_enum(value)) {
				priv->hscroll_policy = g_value_get_enum(value);
				gtk_widget_queue_resize(GTK_WIDGET(editor));
				g_object_notify_by_pspec(object, pspec);
			} break;
    	case PROP_VSCROLL_POLICY:
    		if (priv->vscroll_policy != g_value_get_enum(value)) {
    			priv->vscroll_policy = g_value_get_enum(value);
    			gtk_widget_queue_resize(GTK_WIDGET(editor));
    			g_object_notify_by_pspec (object, pspec);
    		} break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}


static void l_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(object));

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

gboolean cha_editor_im_filter_keypress(ChaEditor *editor, GdkEventKey *eev) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	gboolean result = gtk_im_context_filter_keypress(priv->im_context, eev);
	if (result) {
		priv->need_im_reset = TRUE;
	}
	return result;
}

void cha_editor_im_reset_context(ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	if (priv->need_im_reset) {
		priv->need_im_reset = FALSE;
		gtk_im_context_reset(priv->im_context);
	}
}



static void l_im_context_commit_cb(GtkIMContext *context, const gchar *str, ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	cat_log_debug("commit:%s", str);
	ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
	ChaEditMode mode = cha_document_view_get_edit_mode(document_view);
	ChaUow *uow = NULL;
	const CatStringWo *predit = cha_document_view_get_preedit(priv->document_view);
	cat_log_debug("predit=%O", predit);
	// replace not supported when text is received via IME
	if (mode == CHA_EDIT_MODE_OVERWRITE && predit==NULL) {
		uow = (ChaUow *) cha_uow_replace_text_new(cat_string_wo_new_with(str));
	} else {
		uow = (ChaUow *) cha_uow_insert_text_new(cat_string_wo_new_with(str));
	}
	cha_editor_run((ChaEditor *) editor, uow);
	cat_unref(uow);
}


static void l_im_preedit_changed_cb(GtkIMContext *context, ChaEditor *editor) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
    gchar *preedit_string;
    gint cursor_pos;
    PangoAttrList *preedit_attrs = NULL;
    gtk_im_context_get_preedit_string(priv->im_context, &preedit_string, &preedit_attrs, &cursor_pos);

    const CatStringWo *predit_text = preedit_string==NULL ? NULL : cat_string_wo_new_data(preedit_string);

    cat_log_debug("predit_text=%O", predit_text);

    cha_document_view_set_preedit(priv->document_view, predit_text, preedit_attrs, cursor_pos);
}

static gboolean l_im_retrieve_surrounding_cb(GtkIMContext *context, ChaEditor *editor){
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	ChaRevisionWo *revision = cha_document_get_current_revision_ref(priv->document);
	ChaCursorWo *cursor = cha_revision_wo_get_cursor(revision);
	ChaLineLocationWo *ll = cha_cursor_wo_get_line_location(cursor);
	ChaPageWo *a_page = cha_revision_wo_page_at(revision, cha_line_location_wo_get_page_index(ll));
	cha_page_wo_hold_lines(a_page);

	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(a_page, cha_line_location_wo_get_page_line_index(ll), FALSE);
	gtk_im_context_set_surrounding(context, utf8_text.text, utf8_text.text_len, cha_cursor_wo_get_x_cursor_bytes(cursor));
	cha_utf8_text_cleanup(&utf8_text);
	cha_page_wo_release_lines(a_page);
	cat_unref_ptr(revision);
	return TRUE;
}

//static gboolean l_im_delete_surrounding_cb(GtkIMContext *context, gint offset, gint n_chars, ChaEditor *editor) {
//	return FALSE;
//}


static gboolean l_key_press_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data) {
	ChaEditor *editor = CHA_EDITOR(data);
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	int key_val = eev->keyval;

	gboolean shift_pressed = eev->state & GDK_SHIFT_MASK;
	gboolean ctrl_pressed = eev->state & GDK_CONTROL_MASK;
	gboolean mod1_pressed = eev->state & GDK_MOD1_MASK;
	gboolean mod5_pressed = eev->state & GDK_MOD5_MASK;
	gboolean alt_pressed = (mod1_pressed || mod5_pressed) ? TRUE : FALSE;

	cat_log_debug("key=%s", eev->string);

	ChaUow *uow = NULL;

	switch (key_val) {
		case GDK_KEY_Right: {
			uow = (ChaUow *) cha_uow_move_cursor_new(ctrl_pressed ? CHA_MOVE_CURSOR_NEXT_COLUMN_WORD : CHA_MOVE_CURSOR_NEXT_COLUMN, shift_pressed, alt_pressed);
		} break;
		case GDK_KEY_Left: {
			uow = (ChaUow *) cha_uow_move_cursor_new(ctrl_pressed ? CHA_MOVE_CURSOR_PREV_COLUMN_WORD : CHA_MOVE_CURSOR_PREV_COLUMN, shift_pressed, alt_pressed);
		} break;
		case GDK_KEY_Up: {
			if (alt_pressed) {
				uow = (ChaUow *) cha_uow_lines_move_new(FALSE);
			} else {
				uow = (ChaUow *) cha_uow_move_cursor_new(CHA_MOVE_CURSOR_UP, shift_pressed, alt_pressed);
			}
		} break;
		case GDK_KEY_Down: {
			if (alt_pressed) {
				uow = (ChaUow *) cha_uow_lines_move_new(TRUE);
			} else {
				uow = (ChaUow *) cha_uow_move_cursor_new(CHA_MOVE_CURSOR_DOWN, shift_pressed, alt_pressed);
			}
		} break;
		case GDK_KEY_Page_Down: {
			uow = (ChaUow *) cha_uow_move_cursor_new(CHA_MOVE_CURSOR_PAGE_DOWN, shift_pressed, alt_pressed);
		} break;
		case GDK_KEY_Page_Up: {
			uow = (ChaUow *) cha_uow_move_cursor_new(CHA_MOVE_CURSOR_PAGE_UP, shift_pressed, alt_pressed);
		} break;
		case GDK_KEY_Home: {
			uow = (ChaUow *) cha_uow_move_cursor_new(CHA_MOVE_CURSOR_BEGIN_OF_LINE, shift_pressed, alt_pressed);
		} break;
		case GDK_KEY_End: {
			uow = (ChaUow *) cha_uow_move_cursor_new(CHA_MOVE_CURSOR_END_OF_LINE, shift_pressed, alt_pressed);
		} break;
		case GDK_KEY_KP_Enter :
		case GDK_KEY_Return: {
			uow = (ChaUow *) cha_uow_insert_text_new(cat_string_wo_new_with("\n"));
		} break;
		case GDK_KEY_BackSpace: {
			uow = (ChaUow *) cha_uow_remove_text_new(FALSE, ctrl_pressed);
		} break;
		case GDK_KEY_Delete: {
			uow = (ChaUow *) cha_uow_remove_text_new(TRUE, ctrl_pressed);
		} break;
		case GDK_KEY_F4 : {
//			uow = (ChaUow *) cha_uow_insert_template_new(NULL);
			uow = (ChaUow *) cha_uow_insert_text_new(cat_string_wo_new_with("owh no test test\\\\\\\\nnot this time joepie"));
		} break;

	}

	if (uow==NULL && !ctrl_pressed && !mod1_pressed) {
		if (eev->string[0] != 0) {
			ChaEditMode edit_mode = cha_document_view_get_edit_mode(priv->document_view);
			if (edit_mode==CHA_EDIT_MODE_INSERT) {
				uow = (ChaUow *) cha_uow_insert_text_new(cat_string_wo_new_with(eev->string));
			} else {
				uow = (ChaUow *) cha_uow_replace_text_new(cat_string_wo_new_with(eev->string));
			}
		}
	} else if (ctrl_pressed) {
		switch(key_val) {
			case GDK_KEY_a : {
				uow = (ChaUow *) cha_uow_select_new(CHA_UOW_SELECT_ALL);
			} break;
			case GDK_KEY_c : {
				uow = (ChaUow *) cha_uow_clipboard_new(CHA_UOW_CLIPBOARD_COPY, FALSE);
			} break;
			case GDK_KEY_d : {
				uow = (ChaUow *) cha_uow_lines_remove_new();
			} break;
			case GDK_KEY_f : {
				ChaSearchService *search_service = cha_editor_get_search_service(editor);
				GtkWidget *owner_window = gtk_widget_get_parent((GtkWidget *) editor);
				while(owner_window) {
					if (GTK_IS_WINDOW(owner_window)) {
						break;
					}
					owner_window = gtk_widget_get_parent(owner_window);
				}
				cha_search_service_show_dialog(search_service, (GtkWindow *) owner_window, editor);
//
//				ChaSearchDialog *dialog = cha_search_dialog_new();
//				cha_search_dialog_set_editor(dialog, CHA_EDITOR(data));
//				cha_search_dialog_show(dialog);
			} break;
			case GDK_KEY_x : {
				uow = (ChaUow *) cha_uow_clipboard_new(CHA_UOW_CLIPBOARD_CUT, FALSE);
			} break;
			case GDK_KEY_z : {
				uow = (ChaUow *) cha_uow_undo_redo_new(FALSE);
			} break;
			case GDK_KEY_v : {
				uow = (ChaUow *) cha_uow_clipboard_new(CHA_UOW_CLIPBOARD_PASTE, FALSE);
			} break;
			case GDK_KEY_F2 : {
				if (shift_pressed) {
					uow = (ChaUow *) cha_uow_line_marker_new(CHA_UOW_MARK_SET);
				} else {
					uow = (ChaUow *) cha_uow_line_marker_new(CHA_UOW_MARK_TOGGLE);
				}
			} break;
			case GDK_KEY_F3 : {
				if (shift_pressed) {
					uow = (ChaUow *) cha_uow_line_marker_new(CHA_UOW_MARK_SET_ALL);
				} else {
					uow = (ChaUow *) cha_uow_line_marker_new(CHA_UOW_MARK_UNSET_ALL);
				}
			} break;
		}
	}

	cat_log_debug("uow=%o, %d", uow, (int) key_val);

	if (uow!=NULL) {
		cha_uow_run(uow, CHA_EDITOR(data), priv->document_view);
//		cha_document_view_invalidate_lines(priv->document_view);
		cat_unref(uow);
		return TRUE;
	}

	return FALSE;
}

void cha_editor_run(ChaEditor *editor, ChaUow *uow) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	if (priv->document==NULL) {
		return;
	}
	cat_log_debug("uow_class_name=%s", g_type_name_from_instance((GTypeInstance *) uow));

	cha_uow_run(uow, editor, priv->document_view);

	CatIIterator *iterator = cat_weak_list_iterator(priv->uow_listeners);
	while(cat_iiterator_has_next(iterator)) {
		ChaIUowListener *uow_listener = (ChaIUowListener *) cat_iiterator_next(iterator);
		cha_iuow_listener_uow_event(uow_listener, uow, priv->document_view, priv->document);
	}
	cat_unref_ptr(iterator);
	while ((gtk_events_pending()) != 0) {
		gtk_main_iteration();
	}

}

void cha_editor_notify_uow_ran(ChaEditor *editor, ChaUow *uow) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);

	CatIIterator *iterator = cat_weak_list_iterator(priv->uow_listeners);
	while(cat_iiterator_has_next(iterator)) {
		ChaIUowListener *uow_listener = (ChaIUowListener *) cat_iiterator_next(iterator);
		cha_iuow_listener_uow_event(uow_listener, uow, priv->document_view, priv->document);
	}
	cat_unref_ptr(iterator);
}

void cha_editor_add_uow_listener(ChaEditor *editor, ChaIUowListener *listener) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	cat_weak_list_append_once(priv->uow_listeners, (GObject *) listener);
}

void cha_editor_remove_uow_listener(ChaEditor *editor, ChaIUowListener *listener) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	cat_weak_list_remove(priv->uow_listeners, (GObject *) listener);
}


static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(data));
	gboolean result = FALSE;
	gint xmouse = eev->x;
	gint ymouse = eev->y;
	gboolean extendMark = eev->state & GDK_SHIFT_MASK;
	gboolean ctrl_pressed = eev->state & GDK_CONTROL_MASK;

	gtk_widget_grab_focus(gwidget);

	long long y_marker_view = 0;
	ChaLineLocationWo *y_marker = cha_document_view_cursor_at_marker(priv->document_view, xmouse, ymouse, &y_marker_view);
	if (y_marker!=NULL) {
		gdk_window_set_cursor(gtk_widget_get_window(gwidget), NULL);

		ChaEditorClass *editor_class = CHA_EDITOR_GET_CLASS(CHA_EDITOR(data));
		if (editor_class->markerClicked) {
			editor_class->markerClicked((ChaEditor *) data, y_marker, y_marker_view);
		}
		cat_unref_ptr(y_marker);
		return TRUE;
	}



	if (eev->type==GDK_3BUTTON_PRESS) {
		ChaUow *uow = (ChaUow *) cha_uow_select_new(CHA_UOW_SELECT_LINE);
		cha_uow_run(uow, CHA_EDITOR(data), priv->document_view);
		cat_unref_ptr(uow);
		result = TRUE;
		cha_document_view_invalidate_lines(priv->document_view);
	} else if (eev->type==GDK_2BUTTON_PRESS) {
		ChaUow *uow = (ChaUow *) cha_uow_select_new(CHA_UOW_SELECT_WORD);
		cha_uow_run(uow, CHA_EDITOR(data), priv->document_view);
		cat_unref_ptr(uow);
		result = TRUE;
		cha_document_view_invalidate_lines(priv->document_view);
	} else {
		result = TRUE;
		cat_log_debug("xmouse=%d, ymouse=%d",xmouse, ymouse);
		ChaDocument *document = cha_document_view_get_document(priv->document_view);
		ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
		ChaCursorWo *cursor = cha_document_view_cursor_at_xy(priv->document_view, xmouse, ymouse);
		if (cursor!=NULL) {

			if (extendMark) {
				ChaCursorWo *old_cursor = cha_revision_wo_get_cursor(e_revision);
				ChaSelection *selection = cha_document_view_get_or_create_selection(priv->document_view, e_revision, old_cursor, ctrl_pressed);
				cha_selection_set_end(selection, cursor);
			} else {
				ChaSelection *selection = cha_document_view_get_selection(priv->document_view);
				if (selection) {
					cha_document_view_clear_selection(priv->document_view, e_revision);
					cha_document_view_notify_selection_changed(priv->document_view);
				}
			}
			cha_revision_wo_set_cursor(e_revision, cursor);
			cat_unref_ptr(cursor);
		}

		cursor = cha_revision_wo_get_cursor(e_revision);
		cha_document_view_mark_layout_x_cursor(priv->document_view, e_revision, cursor);

		cha_document_anchor_document(document);

		priv->mouse_down = TRUE;
		cha_document_view_invalidate_lines(priv->document_view);
	}
	if (priv->blinker) {
		cha_blinker_inform_activity(priv->blinker);
	}

	return result;
}



static gboolean l_button_release_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(data));

//	gtk_widget_grab_focus(ast_editor->drawing_area);
	priv->mouse_down = FALSE;
	return TRUE;
}


static gboolean l_motion_event_cb(GtkWidget *gwidget, GdkEventMotion *eev, gpointer data) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(data));
	gint xmouse = eev->x;
	gint ymouse = eev->y;

	long long y_marker_view;
	ChaLineLocationWo *y_marker = cha_document_view_cursor_at_marker(priv->document_view, xmouse, ymouse, &y_marker_view);
	if (y_marker) {
		gdk_window_set_cursor(gtk_widget_get_window(gwidget), NULL);

		ChaEditorClass *editor_class = CHA_EDITOR_GET_CLASS(CHA_EDITOR(data));
		if (editor_class->markerOver) {
			editor_class->markerOver((ChaEditor *) data, y_marker, y_marker_view);
		}
		priv->need_marker_out = TRUE;
		cat_unref_ptr(y_marker);
		return TRUE;
	}

	if (priv->need_marker_out) {
		priv->need_marker_out = FALSE;
		ChaEditorClass *editor_class = CHA_EDITOR_GET_CLASS(CHA_EDITOR(data));
		if (editor_class->markerOut) {
			editor_class->markerOut((ChaEditor *) data);
		}
	}

	gdk_window_set_cursor(gtk_widget_get_window(gwidget), priv->cursor);


	if (priv->mouse_down) {

		gboolean ctrl_pressed = eev->state & GDK_CONTROL_MASK;
//		gboolean mod1_pressed = eev->state & GDK_MOD1_MASK;
//		gboolean mod5_pressed = eev->state & GDK_MOD5_MASK;


		cat_log_debug("xmouse=%d, ymouse=%d",xmouse, ymouse);
		ChaDocument *document = cha_document_view_get_document(priv->document_view);
		ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
		ChaCursorWo *cursor = cha_document_view_cursor_at_xy(priv->document_view, xmouse, ymouse);
		ChaSelection *selection = cha_document_view_get_or_create_selection(priv->document_view, e_revision, cursor, ctrl_pressed);
		cha_selection_set_end(selection, cursor);
		cat_log_debug("cursor=%o", cursor);
		if (cursor!=NULL) {
			cha_revision_wo_set_cursor(e_revision, cursor);
			cat_unref_ptr(cursor);
		}

		cursor = cha_revision_wo_get_cursor(e_revision);
		cha_document_view_mark_layout_x_cursor(priv->document_view, e_revision, cursor);

		cha_document_anchor_document(document);
		cha_document_view_notify_selection_changed(priv->document_view);
		cha_document_view_move_view_to_focus(priv->document_view, FALSE);

		cha_document_view_invalidate_lines(priv->document_view);

		if (priv->blinker) {
			cha_blinker_inform_activity(priv->blinker);
		}
		return TRUE;
	}
	return FALSE;
}


static void l_widget_realize(GtkWidget *widget) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(widget));

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

	priv->pango_context = gtk_widget_create_pango_context(widget);

	if (priv->document_view==NULL) {
		ChaEditorClass *editor_class = CHA_EDITOR_GET_CLASS(widget);
		if (editor_class->initializeDocumentView) {
			priv->document_view = editor_class->initializeDocumentView((ChaEditor *) widget, priv->document, priv->pango_context, widget);
		} else {
			priv->document_view = cha_document_view_new(priv->document, priv->pango_context, widget);
		}
		if (priv->a_preferences) {
			cha_document_view_set_preferences(priv->document_view, priv->a_preferences);
		}
		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, priv->vadjustment);
	}

	if (priv->cursor==NULL) {
		priv->cursor = gdk_cursor_new_for_display(gdk_window_get_display(window), GDK_XTERM);
	}

	gdk_window_set_cursor(gtk_widget_get_window(widget), priv->cursor);


	gtk_im_context_set_client_window(priv->im_context, window);
}

static void l_widget_unrealize(GtkWidget *widget) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(widget));
	gtk_im_context_set_client_window(priv->im_context, NULL);
	GTK_WIDGET_CLASS(cha_editor_parent_class)->unrealize (widget);
}

static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(CHA_EDITOR(widget));

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

	cha_document_view_set_view_size(priv->document_view, allocation->width, allocation->height);

	l_set_hadjustment_values((ChaEditor *) widget);
	l_set_vadjustment_values((ChaEditor *) widget);

	cha_document_view_invalidate_lines(priv->document_view);
}


static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr) {
	ChaEditor *editor = CHA_EDITOR(widget);
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);

	cha_document_view_draw(priv->document_view, cr);
	return FALSE;
}

static gboolean l_widget_focus(GtkWidget *widget, GtkDirectionType direction) {
	gtk_widget_grab_focus(widget);
	return TRUE;
}

typedef void (*ApplyTogglePreferencessFlag)(ChaEditor *editor, ChaPreferencesWo *e_prefs);

static void l_toggle_preferences_flag(ChaEditor *editor, ApplyTogglePreferencessFlag apply_cb) {
	ChaEditorPrivate *priv = cha_editor_get_instance_private(editor);
	gboolean wrap_lines = cha_preferences_wo_get_wrap_lines(priv->a_preferences);
	ChaPreferencesWo *e_prefs = cha_preferences_wo_create_editable(priv->a_preferences);
	cha_preferences_wo_set_wrap_lines(e_prefs, !wrap_lines);

	apply_cb(editor, e_prefs);

	int version = cat_wo_get_version((CatWo *) priv->a_preferences);
	ChaPreferencesWo *a_new_prefs = cha_preferences_wo_anchor(e_prefs, version+1);
	cha_editor_set_preferences(editor, a_new_prefs);
	cat_unref_ptr(a_new_prefs);
}


static void l_apply_toggle_word_wrap(ChaEditor *editor, ChaPreferencesWo *e_prefs) {
	gboolean wrap_lines = cha_preferences_wo_get_wrap_lines(e_prefs);
	cha_preferences_wo_set_wrap_lines(e_prefs, !wrap_lines);
}

static void l_editor_toggle_word_wrap(ChaEditor *editor) {
	l_toggle_preferences_flag(editor, l_apply_toggle_word_wrap);
}

static void l_apply_toggle_show_whitespace(ChaEditor *editor, ChaPreferencesWo *e_prefs) {
	gboolean show_whitespace = cha_preferences_wo_get_show_whitespace(e_prefs);
	cha_preferences_wo_set_show_whitespace(e_prefs, !show_whitespace);
}

static void l_editor_toggle_show_whitespace(ChaEditor *editor) {
	l_toggle_preferences_flag(editor, l_apply_toggle_show_whitespace);
}

static void l_apply_toggle_mark_occurrences(ChaEditor *editor, ChaPreferencesWo *e_prefs) {
	gboolean mark_occurrences = cha_preferences_wo_get_mark_occurrences(e_prefs);
	cha_preferences_wo_set_mark_occurrences(e_prefs, !mark_occurrences);
}

static void l_editor_toggle_mark_occurrences(ChaEditor *editor) {
	l_toggle_preferences_flag(editor, l_apply_toggle_mark_occurrences);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaEditor *instance = CHA_EDITOR(self);
	ChaEditorPrivate *priv = cha_editor_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, document=%p]", iname, self, priv->document);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
