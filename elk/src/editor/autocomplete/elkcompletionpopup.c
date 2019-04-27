/*
   File:    elkcompletionpopup.c
   Project: astarte
   Author:  Douwe Vos
   Date:    Sep 17, 2010
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


#include "elkcompletionpopup.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkCompletionPopup"
#include <logging/catlog.h>

G_DEFINE_TYPE(ElkCompletionPopup, elk_completion_popup, G_TYPE_OBJECT) // @suppress("Unused static function")

static void l_dispose(GObject *object);

//static gboolean _widget_draw(GtkWidget *widget, cairo_t *cairo);

static void elk_completion_popup_class_init(ElkCompletionPopupClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void elk_completion_popup_init(ElkCompletionPopup *obj) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkCompletionPopup *instance = ELK_COMPLETION_POPUP(object);
	cat_unref_ptr(instance->completion_panel);
	G_OBJECT_CLASS(elk_completion_popup_parent_class)->dispose(object);
	cat_log_detail("end-dispose:%p", object);
}


static gboolean l_new_event(GtkWidget *gwidget, GdkEvent *eev, gpointer panel_as_obj) {
	if (eev->type==GDK_BUTTON_PRESS) {
		cat_log_debug("press");
		ElkCompletionPopup *completion_popup = ELK_COMPLETION_POPUP(panel_as_obj);
		completion_popup->has_grab = TRUE;
		GtkWidget *popup_wdgt = GTK_WIDGET(completion_popup->popup_window);
#ifndef OSWINDOW
		gtk_window_set_accept_focus(completion_popup->popup_window, TRUE);
		gdk_window_focus(gtk_widget_get_window(popup_wdgt), GDK_CURRENT_TIME);
		gtk_window_activate_focus(GTK_WINDOW(popup_wdgt));
#else
		gtk_widget_set_can_focus(GTK_WIDGET(completion_popup->popup_window), TRUE);
		gtk_window_set_accept_focus(completion_popup->popup_window, TRUE);
		gdk_window_focus(gtk_widget_get_window(popup_wdgt), GDK_CURRENT_TIME);
#endif
//		return TRUE;
	}
	return FALSE;
}


static void l_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data) {
	ElkCompletionPopup *completion_popup = ELK_COMPLETION_POPUP(user_data);
	cat_log_debug("allocation=[%d,%d,%d,%d]", allocation->x, allocation->y, allocation->width, allocation->height);
	allocation->x = completion_popup->xpos;
	allocation->y = completion_popup->ypos;
	gtk_widget_set_allocation(widget, allocation);

	if (gtk_widget_get_realized(widget)) {
		gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x+1, allocation->y+1, allocation->width, allocation->height);
	}

	if (completion_popup->scrolled_window) {
		GtkAllocation swalloc;
		swalloc.x = 1;
		swalloc.y = 1;
		swalloc.width = allocation->width-2;
		swalloc.height = allocation->height-2;

		gtk_widget_size_allocate(completion_popup->scrolled_window, &swalloc);
	}
}

static void l_focus_out_event(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
	cat_log_debug("focus-out-event");
//	gtk_widget_destroy(widget);
}


ElkCompletionPopup *elk_completion_popup_new(GtkWidget *widget, ElkCompletionPanel *completion_panel) {
	ElkCompletionPopup *result = g_object_new(ELK_TYPE_COMPLETION_POPUP, NULL);
	cat_ref_anounce(result);
	result->count = 0;

	result->has_grab = FALSE;
#ifdef G_OS_WIN32
//	result->popup_window = gtk_window_new(GTK_WINDOW_POPUP);
	result->popup_window = (GtkWindow *) gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint(result->popup_window, GDK_WINDOW_TYPE_HINT_COMBO);
#else
	result->popup_window = (GtkWindow *) gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_type_hint(result->popup_window, GDK_WINDOW_TYPE_HINT_DOCK); // LINUX
#endif

////	gtk_window_set_type_hint(window, GTK_WINDOW_TOPLEVEL);
////	gtk_window_set_has_frame(window, FALSE);
	gtk_window_set_decorated(result->popup_window, FALSE);
	gtk_window_set_accept_focus(result->popup_window, FALSE);
	gtk_widget_set_can_focus(GTK_WIDGET(result->popup_window), FALSE);

	GtkWidget *toplevel = gtk_widget_get_toplevel(widget);

	gtk_window_set_transient_for(result->popup_window, GTK_WINDOW(toplevel));
	gtk_window_group_add_window(gtk_window_get_group(GTK_WINDOW(toplevel)), result->popup_window);
	result->widget = widget;
	result->scrolled_window = NULL;
	result->completion_panel = cat_ref_ptr(completion_panel);

	gtk_widget_add_events((GtkWidget *) result->popup_window, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
//
//	//	gtk_widget_set_parent(GTK_WIDGET(result->popup_window), widget);
//	GtkStyleContext *style_context = gtk_widget_get_style_context(widget);

//	gtk_widget_set_style(GTK_WIDGET(result->popup_window), gtk_widget_get_style(widget));
////	gtk_widget_style_attach(GTK_WIDGET(result->popup_window));
//
	result->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(result->scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add(GTK_CONTAINER(result->popup_window), result->scrolled_window);
////	gtk_widget_show_all(result->scrolled_window);
//
	gtk_container_add(GTK_CONTAINER(result->scrolled_window), GTK_WIDGET(result->completion_panel));
////	gtk_scrolled_window_add_with_viewport()_with_viewport(GTK_SCROLLED_WINDOW(result->scrolled_window), GTK_WIDGET(result->completion_panel));

//	GtkViewport *viewport = GTK_VIEWPORT(gtk_bin_get_child(GTK_BIN(result->scrolled_window)));
//	gtk_viewport_set_shadow_type(viewport, GTK_SHADOW_NONE);
//	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(result->scrolled_window), GTK_SHADOW_NONE);


//	gtk_widget_show(GTK_WIDGET(result->completion_panel));
//	gtk_widget_show_all(GTK_WIDGET(result->popup_window));

//	gtk_window_set_accept_focus(result->popup_window, FALSE);
//	gtk_widget_set_can_focus(GTK_WIDGET(result->popup_window), FALSE);


	g_signal_connect(result->popup_window, "size_allocate", G_CALLBACK(l_size_allocate), result);
	g_signal_connect(result->popup_window, "event", G_CALLBACK(l_new_event), result);
	g_signal_connect(result->scrolled_window, "event", G_CALLBACK(l_new_event), result);
	g_signal_connect(result->completion_panel, "event", G_CALLBACK(l_new_event), result);

	g_signal_connect(result->popup_window, "focus-out-event", G_CALLBACK(l_focus_out_event), result);

	return result;
}


//static gboolean _widget_draw(GtkWidget *widget, cairo_t *cairo) {
//	gboolean result;
//	result = GTK_WIDGET_CLASS(parent_class)->draw(widget, cairo);
//	if (gtk_widget_is_drawable(widget)) {
//		cairo_set_source_rgb(cairo, 0,0,0.2);
//
////		GdkGC *gc = widget->style->dark_gc[0];
//		int width = gtk_widget_get_allocated_width(widget);
//		int height = gtk_widget_get_allocated_height(widget);
//		cairo_rectangle(cairo, 0, 0, width, height);
//		cairo_stroke(cairo);
//	}
//	return result;
//}


void elk_completion_popup_show(ElkCompletionPopup *popup) {
	gtk_window_set_focus_on_map(GTK_WINDOW(popup->popup_window), FALSE);
	gtk_widget_show_all(GTK_WIDGET(popup->popup_window));
}


