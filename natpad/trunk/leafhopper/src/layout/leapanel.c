/*
   File:    leapanel.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    5-9-2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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

#include "leapanel.h"
#include "../leaframe.h"
#include "leaipanelowner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaPanel"
#include <logging/catlog.h>


struct _LeaPanelPrivate {
	CatWeakList *listeners;
	const CatStringWo *label;
	GtkWidget *child;
//	GtkWidget *last_focus;	// TODO: ensure handling the possible dispose of the widget
	gboolean is_active;
	gboolean hasFocus;
	LeaActionGroup *action_group;
	int order_index;
	struct _LeaIPanelOwner *panel_owner;
};


G_DEFINE_TYPE(LeaPanel, lea_panel, GTK_TYPE_BIN)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_widget_show(GtkWidget *widget);
static void l_widget_hide(GtkWidget *widget);
static void l_widget_destroy(GtkWidget *widget);
static void  l_widget_grab_focus(GtkWidget           *widget) {
	lea_panel_request_focus(LEA_PANEL(widget));
}


static void lea_panel_class_init(LeaPanelClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaPanelPrivate));

	clazz->focusActivated = NULL;
	clazz->focusSet = NULL;
	clazz->close = lea_panel_close_real;
	clazz->closing = NULL;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkWidgetClass *widget_class = (GtkWidgetClass*) clazz;
	widget_class->show = l_widget_show;
	widget_class->hide = l_widget_hide;
	widget_class->destroy = l_widget_destroy;
	widget_class->grab_focus = l_widget_grab_focus;
}

static void lea_panel_init(LeaPanel *node) {
}



void lea_panel_construct(LeaPanel *panel, LeaIPanelOwner *panel_owner, gpointer frame, const CatStringWo *c_name, GtkWidget *widget) {
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);
	cat_log_debug("panel=%p %s", panel, g_type_name_from_instance((GTypeInstance *) panel));
	priv->panel_owner = cat_ref_ptr(panel_owner);
	priv->label = c_name;
	priv->child = widget;
	priv->is_active = FALSE;
	priv->hasFocus = FALSE;
	priv->action_group = NULL;
	priv->listeners = cat_weak_list_new();
	priv->order_index = 0;
	lea_ipanel_owner_add_panel(panel_owner, panel);
	gtk_container_add(GTK_CONTAINER(panel), widget);

	gtk_widget_set_can_focus(GTK_WIDGET(panel), FALSE);
}

LeaPanel *lea_panel_new(LeaIPanelOwner *panel_owner, gpointer frame, const CatStringWo *c_name, GtkWidget *widget) {
	LeaPanel *result = g_object_new(LEA_TYPE_PANEL, NULL);
	cat_ref_anounce(result);
	lea_panel_construct(result, panel_owner, frame, c_name, widget);
	return result;
}




static void l_dispose(GObject *object) {
	cat_log_debug("dispose: %p rc=%d %s", object, object->ref_count, g_type_name_from_instance((GTypeInstance *) object));
	cat_log_indent_level++;
	LeaPanel *instance = LEA_PANEL(object);
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(instance);

	if (priv->child) {
		gtk_container_remove(GTK_CONTAINER(instance), priv->child);
		priv->child = NULL;
	}


	G_OBJECT_CLASS(lea_panel_parent_class)->dispose(object);
	if (priv->panel_owner) {
		lea_ipanel_owner_remove_panel(priv->panel_owner, instance);
	}
	cat_unref_ptr(priv->panel_owner);
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->label);
	cat_unref_ptr(priv->action_group);
	cat_log_indent_level--;
	cat_log_debug("disposed: %p rc=%d", object, object->ref_count);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalizing: %p %s", object, g_type_name_from_instance((GTypeInstance *) object));
	cat_log_indent_level++;
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_panel_parent_class)->finalize(object);
	cat_log_indent_level--;
	cat_log_detail("finalized: %p", object);
}


static void l_widget_destroy(GtkWidget *widget) {
	cat_log_detail("destroying: %p %s", widget, g_type_name_from_instance((GTypeInstance *) widget));
	GTK_WIDGET_CLASS(lea_panel_parent_class)->destroy(widget);
	cat_log_detail("destroyed: %p", widget);
}


gboolean lea_panel_get_has_focus(LeaPanel *panel) {
	return LEA_PANEL_GET_PRIVATE(panel)->hasFocus;
}

void lea_panel_set_has_focus(LeaPanel *panel, gboolean nval) {
	LEA_PANEL_GET_PRIVATE(panel)->hasFocus = nval;
	LeaPanelClass *panel_class = LEA_PANEL_GET_CLASS(panel);
	if (panel_class->focusSet) {
		panel_class->focusSet(panel, nval);
	}
}

LeaKeyContext *lea_panel_get_key_context(LeaPanel *panel) {
	if (panel==NULL) {
		return NULL;
	}
	LeaKeyContext *result = NULL;
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);
	if (priv->panel_owner) {
		result = lea_ipanel_owner_get_key_context(priv->panel_owner);
	}
	return result;
}

void lea_panel_set_order_index(LeaPanel *panel, int order_index) {
	LEA_PANEL_GET_PRIVATE(panel)->order_index = order_index;
}

int lea_panel_get_order_index(LeaPanel *panel) {
	return LEA_PANEL_GET_PRIVATE(panel)->order_index;
}


gboolean lea_panel_close(LeaPanel *panel) {
	return LEA_PANEL_GET_CLASS(panel)->close(panel);
}

gboolean lea_panel_close_real(LeaPanel *panel) {
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);
	GtkWidget *parent_widget = gtk_widget_get_parent(GTK_WIDGET(panel));
	if (parent_widget==NULL) {
		return FALSE;
	}
	if (priv->panel_owner) {
		if (!lea_ipanel_owner_panel_closing(priv->panel_owner, panel)) {
			return FALSE;
		}
	}

	cat_log_debug("Closing confirmed");

	LeaPanelClass *pan_class = LEA_PANEL_GET_CLASS(panel);
	if (pan_class->closing) {
		pan_class->closing(panel);
	}

	cat_ref_ptr(panel);

	GtkWidget *pan_wdgt = GTK_WIDGET(panel);
	gtk_container_remove(GTK_CONTAINER(parent_widget), pan_wdgt);

	cat_log_debug("panel-rc=%d", ((GObject *) panel)->ref_count);

	lea_ipanel_owner_remove_panel(priv->panel_owner, panel);
	cat_unref_ptr(priv->panel_owner);

	gtk_widget_unrealize(GTK_WIDGET(panel));

	cat_unref(panel);
	cat_log_debug("Returning TRUE");
	return TRUE;
}

LeaSurface *lea_panel_get_surface(LeaPanel *panel) {
	GtkWidget *par = gtk_widget_get_parent(GTK_WIDGET(panel));
	if (LEA_IS_SURFACE(par)) {
		return (LeaSurface *) par;
	}
	return NULL;
}

LeaIPanelOwner *lea_panel_get_panel_owner(LeaPanel *panel) {
	return LEA_PANEL_GET_PRIVATE(panel)->panel_owner;
}


const CatStringWo *lea_panel_get_label(const LeaPanel *panel) {
	return LEA_PANEL_GET_PRIVATE((LeaPanel *) panel)->label;
}

gboolean lea_panel_is_active(LeaPanel *panel) {
	if (panel == NULL) {
		return FALSE;
	}
	return LEA_PANEL_GET_PRIVATE(panel)->is_active;
}

void lea_panel_set_label(LeaPanel *panel, const CatStringWo *label) {
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);

	if (cat_string_wo_equal(priv->label, label)) {
		return;
	}
	cat_ref_swap(priv->label, label);
	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		LeaIPanelListener *listener = (LeaIPanelListener *) cat_iiterator_next(iter);
		LeaIPanelListenerInterface *iface = LEA_IPANEL_LISTENER_GET_INTERFACE(listener);
		if (iface->panel_label_set) {
			iface->panel_label_set(listener, panel, label);
		}
	}
	cat_unref_ptr(iter);
	LeaSurface *surface = lea_panel_get_surface(panel);
	if (surface) {
		LeaSurfaceTabBar *tab_bar = lea_surface_get_tab_bar(surface);
		gtk_widget_queue_draw(GTK_WIDGET(tab_bar));
	}
}


void lea_panel_request_focus(LeaPanel *panel) {
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);

	if (priv->child==NULL) {
		cat_log_warn("panel has no child");
		return;
	}
	GtkWidget *new_focus_child = priv->child;
	if (GTK_IS_CONTAINER(priv->child)) {
		new_focus_child = gtk_container_get_focus_child(GTK_CONTAINER(priv->child));
	}
	if (new_focus_child == NULL) {
		cat_log_debug("no focus-child");
		return;
	}
	cat_log_debug("Grabbing focus");
	gtk_widget_grab_focus(new_focus_child);
}


void lea_panel_focus_activated(LeaPanel *panel, gboolean is_active) {
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);

	LeaPanelClass *clazz = LEA_PANEL_GET_CLASS(panel);
	if (priv->is_active != is_active) {
		priv->is_active = is_active;
		if (clazz->focusActivated) {
			clazz->focusActivated(panel, is_active);
		}

		LeaSurface *surface = lea_panel_get_surface(panel);
		if (surface) {
			gtk_widget_queue_draw(GTK_WIDGET(surface));
		}
	}
}

LeaActionGroup *lea_panel_get_action_group(LeaPanel *panel) {
	return LEA_PANEL_GET_PRIVATE(panel)->action_group;
}

void lea_panel_set_action_group(LeaPanel *panel, LeaActionGroup *group) {
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);
	cat_ref_swap(priv->action_group, group);
}

gboolean lea_panel_equal(LeaPanel *panel_a, LeaPanel *panel_b) {
	if (panel_a == panel_b) {
		return TRUE;
	}
	return FALSE;
}

int lea_panel_hash(LeaPanel *panel) {
	return (int) (long) panel;
}



void lea_panel_add_listener(LeaPanel *panel, LeaIPanelListener *listener) {
	g_return_if_fail(listener!=NULL);
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
}

void lea_panel_remove_listener(LeaPanel *panel, LeaIPanelListener *listener) {
	LeaPanelPrivate *priv = LEA_PANEL_GET_PRIVATE(panel);
	cat_weak_list_remove(priv->listeners, (GObject *) listener);
}


static void l_widget_show(GtkWidget *widget) {
	if (GTK_WIDGET_CLASS(lea_panel_parent_class)->show) {
		(*GTK_WIDGET_CLASS(lea_panel_parent_class)->show) (widget);
	}
	cat_log_detail("showing:%p", widget);

	GtkWidget *childWidget = gtk_bin_get_child(GTK_BIN(widget));
	if (childWidget) {
		gtk_widget_show(childWidget);
	}

}

static void l_widget_hide(GtkWidget *widget) {
	if (GTK_WIDGET_CLASS(lea_panel_parent_class)->hide) {
		(*GTK_WIDGET_CLASS(lea_panel_parent_class)->hide) (widget);
	}

	cat_log_detail("hiding:%p", widget);

	GtkWidget *childWidget = gtk_bin_get_child(GTK_BIN(widget));
	cat_log_detail("childWidget:%p", widget);
	if (childWidget) {
		gtk_widget_hide(childWidget);
	}
}

