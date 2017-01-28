/*
   File:    dratagpopup.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Nov 30, 2016
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

#include "dratagpopup.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraTagPopup"
#include <logging/catlog.h>

struct _DraTagPopupPrivate {
	GtkWindow *popup_window;
	GtkWidget *scrolled_window;
	GtkTextView *text_view;
	int xpos, ypos;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraTagPopup, dra_tag_popup, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraTagPopup)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_tag_popup_class_init(DraTagPopupClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_tag_popup_init(DraTagPopup *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraTagPopup *instance = DRA_TAG_POPUP(object);
	DraTagPopupPrivate *priv = dra_tag_popup_get_instance_private(instance);
	if (priv->popup_window) {
		GtkWindow *popup_window = priv->popup_window;
		priv->popup_window = NULL;
		gtk_widget_destroy(GTK_WIDGET(popup_window));
	}
	G_OBJECT_CLASS(dra_tag_popup_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_tag_popup_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer data);

DraTagPopup *dra_tag_popup_new(ChaEditor *editor, int xpos, int ypos) {
	DraTagPopup *result = g_object_new(DRA_TYPE_TAG_POPUP, NULL);
	cat_ref_anounce(result);
	DraTagPopupPrivate *priv = dra_tag_popup_get_instance_private(result);


	int editor_x = 0;
	int editor_y = 0;

	GtkWidget *top_widget = gtk_widget_get_toplevel(GTK_WIDGET(editor));
	gdk_window_get_origin(gtk_widget_get_window(top_widget), &editor_x, &editor_y);

	GtkAllocation alloc;
	GtkWidget *s_widget = gtk_widget_get_parent(GTK_WIDGET(editor));
	gtk_widget_get_allocation(s_widget, &alloc);
	editor_x += alloc.x;
	editor_y += alloc.y;


	priv->xpos = xpos+editor_x;
	priv->ypos = ypos+editor_y;
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

	priv->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(priv->scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(priv->popup_window), (GtkWidget *) priv->scrolled_window);
//

	priv->text_view = (GtkTextView *) gtk_text_view_new();

	gtk_container_add(GTK_CONTAINER(priv->scrolled_window), GTK_WIDGET(priv->text_view));


	g_signal_connect(priv->popup_window, "size_allocate", G_CALLBACK(l_size_allocate), result);
//	g_signal_connect(priv->popup_window, "event", G_CALLBACK(l_new_event), result);
//	g_signal_connect(priv->scrolled_window, "event", G_CALLBACK(l_new_event), result);
//	g_signal_connect(priv->auto_complete_panel, "event", G_CALLBACK(l_new_event), result);
//
//	priv->s_focus_out = g_signal_connect(priv->editor, "focus-out-event", G_CALLBACK(l_focus_out_event), result);
//
//	GtkAdjustment *hadj = gtk_scrollable_get_hadjustment((GtkScrollable *) priv->editor);
//	priv->s_hadj = g_signal_connect(hadj, "value-changed", G_CALLBACK(l_adjustment_value_changed), result);
//
//	GtkAdjustment *vadj = gtk_scrollable_get_vadjustment((GtkScrollable *) priv->editor);
//	priv->s_vadj = g_signal_connect(vadj, "value-changed", G_CALLBACK(l_adjustment_value_changed), result);
//
//
//	g_signal_connect(priv->popup_window, "destroy", G_CALLBACK(l_destroy), result);
//
//
//	ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
//	cha_document_add_listener(document, CHA_IDOCUMENT_LISTENER(result));


	return result;
}


void dra_tag_popup_show(DraTagPopup *popup, CatStringWo *text) {
	DraTagPopupPrivate *priv = dra_tag_popup_get_instance_private(popup);

	GtkTextBuffer *buf = gtk_text_view_get_buffer(priv->text_view);
	gtk_text_buffer_set_text(buf, cat_string_wo_getchars(text), cat_string_wo_length(text));

	gtk_window_set_focus_on_map(GTK_WINDOW(priv->popup_window), FALSE);
	gtk_widget_show_all(GTK_WIDGET(priv->popup_window));


	GtkRequisition nat_req;
	gtk_widget_get_preferred_size(GTK_WIDGET(priv->text_view), &nat_req, NULL);
//	gtk_widget_get_preferred_size(GTK_WIDGET(priv->popup_window), &nat_req, NULL);



	cat_log_debug("nat.size = %d, %d, hw=%d, vw=%d", nat_req.width, nat_req.height);

//	gtk_widget_set_size_request((GtkWidget *) priv->scrolled_window, hw, nat_req.height);
	gtk_widget_set_size_request((GtkWidget *) priv->popup_window, nat_req.width+4, nat_req.height+4);


}



static void l_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer data) {
	DraTagPopup *ac_popup = DRA_TAG_POPUP(data);
	DraTagPopupPrivate *priv = dra_tag_popup_get_instance_private(ac_popup);
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



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
