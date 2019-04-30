/*
   File:    draautocompletepopup.c
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

#include "draautocompletepopup.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraAutoCompletePopup"
#include <logging/catlog.h>

struct _DraAutoCompletePopupPrivate {
	DraEditor *editor;
	GtkWidget *scrolled_window;
	GtkWindow *popup_window;
	DraAutoCompletePanel *auto_complete_panel;

	gulong s_focus_out;
	gulong s_vadj;
	gulong s_hadj;

	int count;
	int xpos, ypos;

	gboolean has_grab;
};

static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraAutoCompletePopup, dra_auto_complete_popup, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraAutoCompletePopup)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, l_document_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_auto_complete_popup_class_init(DraAutoCompletePopupClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_auto_complete_popup_init(DraAutoCompletePopup *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraAutoCompletePopup *instance = DRA_AUTO_COMPLETE_POPUP(object);
//	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(instance);
	G_OBJECT_CLASS(dra_auto_complete_popup_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_auto_complete_popup_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_new_event(GtkWidget *gwidget, GdkEvent *eev, gpointer data);
static void l_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer data);
static void l_focus_out_event(GtkWidget *widget, GdkEvent *event, gpointer data);
static void l_destroy(GtkWidget *object, gpointer data) {
	DraAutoCompletePopup *ac_popup = DRA_AUTO_COMPLETE_POPUP(data);
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(ac_popup);
	if (priv->s_focus_out!=-1) {
		g_signal_handler_disconnect(priv->editor, priv->s_focus_out);
		priv->s_focus_out = -1;
	}

	if (priv->s_vadj!=-1) {
		GtkAdjustment *vadj = gtk_scrollable_get_vadjustment((GtkScrollable *) priv->editor);
		g_signal_handler_disconnect(vadj, priv->s_vadj);
		priv->s_vadj = -1;
	}

	if (priv->s_hadj!=-1) {
		GtkAdjustment *hadj = gtk_scrollable_get_hadjustment((GtkScrollable *) priv->editor);
		g_signal_handler_disconnect(hadj, priv->s_hadj);
		priv->s_hadj = -1;
	}

	priv->popup_window = NULL;
	priv->auto_complete_panel = NULL;
}

static void l_adjustment_value_changed(GtkAdjustment *adjustment, gpointer data) {
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(DRA_AUTO_COMPLETE_POPUP(data));
	if (priv->auto_complete_panel) {
		if (priv->popup_window) {
			gtk_widget_destroy((GtkWidget *) priv->popup_window);
		}
	}
}


DraAutoCompletePopup *dra_auto_complete_popup_new(DraEditor *editor, DraAutoCompletePanel *auto_complete_panel) {
	DraAutoCompletePopup *result = g_object_new(DRA_TYPE_AUTO_COMPLETE_POPUP, NULL);
	cat_ref_anounce(result);
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(result);
	priv->count = 0;
	priv->s_focus_out = -1;
	priv->s_vadj = -1;
	priv->s_hadj = -1;

	priv->has_grab = FALSE;
#ifdef G_OS_WIN32
//	priv->popup_window = gtk_window_new(GTK_WINDOW_POPUP);
	priv->popup_window = (GtkWindow *) gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint(priv->popup_window, GDK_WINDOW_TYPE_HINT_COMBO);
#else
	priv->popup_window = (GtkWindow *) gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_type_hint(priv->popup_window, GDK_WINDOW_TYPE_HINT_DOCK); // LINUX
#endif


////	gtk_window_set_type_hint(window, GTK_WINDOW_TOPLEVEL);
////	gtk_window_set_has_frame(window, FALSE);
	gtk_window_set_decorated(priv->popup_window, FALSE);
	gtk_window_set_accept_focus(priv->popup_window, FALSE);
	gtk_widget_set_can_focus(GTK_WIDGET(priv->popup_window), FALSE);

	GtkWidget *toplevel = gtk_widget_get_toplevel((GtkWidget *) editor);

	gtk_window_set_transient_for(priv->popup_window, GTK_WINDOW(toplevel));
	gtk_window_group_add_window(gtk_window_get_group(GTK_WINDOW(toplevel)), priv->popup_window);
	priv->editor = cat_ref_ptr(editor);
	priv->scrolled_window = NULL;
	priv->auto_complete_panel = cat_ref_ptr(auto_complete_panel);

	gtk_widget_add_events((GtkWidget *) priv->popup_window, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

	priv->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(priv->scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add(GTK_CONTAINER(priv->popup_window), priv->scrolled_window);
//
	gtk_container_add(GTK_CONTAINER(priv->scrolled_window), GTK_WIDGET(priv->auto_complete_panel));


	g_signal_connect(priv->popup_window, "size_allocate", G_CALLBACK(l_size_allocate), result);
	g_signal_connect(priv->popup_window, "event", G_CALLBACK(l_new_event), result);
	g_signal_connect(priv->scrolled_window, "event", G_CALLBACK(l_new_event), result);
	g_signal_connect(priv->auto_complete_panel, "event", G_CALLBACK(l_new_event), result);

	priv->s_focus_out = g_signal_connect(priv->editor, "focus-out-event", G_CALLBACK(l_focus_out_event), result);

	GtkAdjustment *hadj = gtk_scrollable_get_hadjustment((GtkScrollable *) priv->editor);
	priv->s_hadj = g_signal_connect(hadj, "value-changed", G_CALLBACK(l_adjustment_value_changed), result);

	GtkAdjustment *vadj = gtk_scrollable_get_vadjustment((GtkScrollable *) priv->editor);
	priv->s_vadj = g_signal_connect(vadj, "value-changed", G_CALLBACK(l_adjustment_value_changed), result);

	g_signal_connect(priv->popup_window, "destroy", G_CALLBACK(l_destroy), result);

	ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
	cha_document_add_listener(document, CHA_IDOCUMENT_LISTENER(result));

	return result;
}

GtkWindow *dra_auto_complete_popup_get_window(DraAutoCompletePopup *auto_complete_popup) {
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(auto_complete_popup);
	return priv->popup_window;
}

void dra_auto_complete_popup_set_bounds(DraAutoCompletePopup *auto_complete_popup, GtkAllocation *alloc) {
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(auto_complete_popup);
	priv->xpos = alloc->x;
	priv->ypos = alloc->y;
	gtk_widget_set_size_request(GTK_WIDGET(priv->popup_window), alloc->width, alloc->height);
	gtk_widget_size_allocate(GTK_WIDGET(priv->popup_window), alloc);

}

void dra_auto_complete_popup_forward_event(DraAutoCompletePopup *auto_complete_popup, GdkEvent *eev) {
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(auto_complete_popup);
	if (priv->popup_window) {
		GdkEvent *repost_event = gdk_event_copy((GdkEvent *) eev);
		GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(priv->popup_window));
		repost_event->key.window = gtk_widget_get_window(toplevel);
		cat_ref(gtk_widget_get_window(toplevel));
		cat_log_debug("reposting event");
		gtk_widget_event(GTK_WIDGET(priv->auto_complete_panel), repost_event);
		gdk_event_free(repost_event);
	}
}

static gboolean l_new_event(GtkWidget *gwidget, GdkEvent *eev, gpointer data) {
	if (eev->type==GDK_BUTTON_PRESS) {
		cat_log_debug("press");
		DraAutoCompletePopup *ac_popup = DRA_AUTO_COMPLETE_POPUP(data);
		DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(ac_popup);
		priv->has_grab = TRUE;
		GtkWidget *popup_wdgt = GTK_WIDGET(priv->popup_window);
#ifndef OSWINDOW
		gtk_window_set_accept_focus(priv->popup_window, TRUE);
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


static void l_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer data) {
	DraAutoCompletePopup *ac_popup = DRA_AUTO_COMPLETE_POPUP(data);
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(ac_popup);
	cat_log_debug("allocation=[%d,%d,%d,%d]", allocation->x, allocation->y, allocation->width, allocation->height);
	allocation->x = priv->xpos;
	allocation->y = priv->ypos;
	gtk_widget_set_allocation(widget, allocation);

	if (gtk_widget_get_realized(widget)) {
		gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x+1, allocation->y+1, allocation->width, allocation->height);
	}
//	GTK_WIDGET_CLASS(parent_class)->size_allocate(widget, allocation);


	if (priv->scrolled_window) {
		GtkAllocation swalloc;
		swalloc.x = 1;
		swalloc.y = 1;
		swalloc.width = allocation->width-2;
		swalloc.height = allocation->height-2;

		gtk_widget_size_allocate(priv->scrolled_window, &swalloc);
	}
}

static void l_focus_out_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(DRA_AUTO_COMPLETE_POPUP(data));
	cat_log_on_debug({
		GdkEventFocus *evf = (GdkEventFocus *) event;
		cat_log_debug("focus-out-event: window=%p, in=%d, widget-window=%p, editor-window=%p", evf->window, evf->in, gtk_widget_get_window(widget), gtk_widget_get_window((GtkWidget *) priv->editor));
	});
	if (priv->popup_window) {
		gtk_widget_destroy((GtkWidget *) priv->popup_window);
	}
}


void dra_auto_complete_popup_show(DraAutoCompletePopup *auto_complete_popup) {
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(auto_complete_popup);
	gtk_window_set_focus_on_map(GTK_WINDOW(priv->popup_window), FALSE);
	gtk_widget_show_all(GTK_WIDGET(priv->popup_window));
}

/********************* start ChaIDocumentListener implementation *********************/

static void l_on_new_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(DRA_AUTO_COMPLETE_POPUP(self));
	if (priv->auto_complete_panel) {
		if (!dra_auto_complete_panel_new_revision(priv->auto_complete_panel, a_new_revision)) {
			if (priv->popup_window) {
				gtk_widget_destroy((GtkWidget *) priv->popup_window);
			}
		}
	}
}

static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	iface->onNewRevision = l_on_new_revision;
}

/********************* end ChaIDocumentListener implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraAutoCompletePopup *instance = DRA_AUTO_COMPLETE_POPUP(self);
	DraAutoCompletePopupPrivate *priv = dra_auto_complete_popup_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p popup_window=%p]", iname, self, priv->popup_window);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
