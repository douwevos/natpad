/*
   File:    moopanel.c
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

#include "moopanel.h"
#include "moopanelowner.h"
#include "mooexplorer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooPanel"
#include <logging/catlog.h>

struct _MooPanelPrivate {
	GtkWidget *w_scrolled_window;
	MooExplorer *explorer;
	MooService *moo_service;

	VipSnapshot *vip_snaphot;
	CatArrayWo *selection;
};

static void l_layout_model_listener_iface_init(MooILayoutModelListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooPanel, moo_panel, LEA_TYPE_PANEL, {
		G_ADD_PRIVATE(MooPanel)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ILAYOUT_MODEL_LISTENER, l_layout_model_listener_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_panel_class_init(MooPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_panel_init(MooPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooPanel *instance = MOO_PANEL(object);
	MooPanelPrivate *priv = moo_panel_get_instance_private(instance);
	G_OBJECT_CLASS(moo_panel_parent_class)->dispose(object);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->explorer);
	cat_unref_ptr(priv->selection);
	cat_unref_ptr(priv->vip_snaphot);
	cat_unref_ptr(priv->w_scrolled_window);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_explorer_destroy(GtkWidget *widget, gpointer data) {
	MooPanel *instance = MOO_PANEL(data);
	MooPanelPrivate *priv = moo_panel_get_instance_private(instance);
	cat_log_detail("unreffing explorer:%o", priv->explorer);
	cat_unref_ptr(priv->explorer);
}

static void l_initialize_widget(MooPanel *panel, MooPanelOwner *panel_owner, MooExplorer *explorer) {
	MooPanelPrivate *priv = moo_panel_get_instance_private(panel);
	MooNodeRenderRegistry *registry = moo_panel_owner_get_render_registry(panel_owner);
	if (explorer) {
		priv->explorer = cat_ref_ptr(explorer);
	} else {
		priv->explorer = moo_explorer_new(priv->moo_service, registry);
	}

	LeaScrolledWindow *scrolled_window = lea_scrolled_window_new((GtkWidget *) priv->explorer);
//	lea_scrolled_window_set_use_x11_scroll(scrolled_window, TRUE);

	priv->w_scrolled_window = (GtkWidget *) cat_ref_ptr(scrolled_window);

//	priv->w_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
//	gtk_scrolled_window_add_with_viewport(GTK_CONTAINER(priv->w_scrolled_window), GTK_WIDGET(priv->explorer));

	gtk_widget_show((GtkWidget *) priv->explorer);

	g_signal_connect(priv->explorer, "destroy", G_CALLBACK(l_explorer_destroy), panel);
}

MooPanel *moo_panel_new(LeaIPanelOwner *panel_owner, LeaFrame *frame, MooService *moo_service, MooExplorer *explorer) {
	MooPanel *result = g_object_new(MOO_TYPE_PANEL, NULL);
	cat_ref_anounce(result);
	MooPanelPrivate *priv = moo_panel_get_instance_private(result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->selection = NULL;
	priv->vip_snaphot = NULL;
	MooPanelOwner *moo_panel_owner = MOO_PANEL_OWNER(panel_owner);
	l_initialize_widget(result, moo_panel_owner, explorer);
	lea_panel_construct((LeaPanel *) result, panel_owner, frame, cat_string_wo_new_with("Moose explorer"), priv->w_scrolled_window);
	gtk_widget_grab_focus(GTK_WIDGET(priv->explorer));

	MooLayoutModel *layout_model = moo_explorer_get_layout_model(explorer);
	moo_layout_model_add_listener(layout_model, (MooILayoutModelListener *) result);
	return result;
}

CatArrayWo *moo_panel_get_selection(MooPanel *panel) {
	MooPanelPrivate *priv = moo_panel_get_instance_private(panel);
	return priv->selection;
}

VipSnapshot *moo_panel_get_vip_snapshot(MooPanel *panel) {
	MooPanelPrivate *priv = moo_panel_get_instance_private(panel);
	return priv->vip_snaphot;
}


/********************* begin MooILayoutModelListener implementation *********************/

static void l_layout_model_selection_set(MooILayoutModelListener *self, CatArrayWo *new_selection) {
	MooPanel *instance = MOO_PANEL(self);
	MooPanelPrivate *priv = moo_panel_get_instance_private(instance);
	cat_ref_swap(priv->selection, new_selection);
	if (priv->moo_service==NULL) {
		return;
	}
	VipService *vip_service = moo_service_get_viper_service(priv->moo_service);
	if (vip_service==NULL) {
		return;
	}
	VipSnapshot *snaphot = vip_service_get_snapshot(vip_service);
	cat_ref_sink_swap(priv->vip_snaphot, snaphot);
	cat_unref_ptr(snaphot);

	cat_log_debug("selection set:new_selection=%o", new_selection);
	MooPanelOwner *moo_panel_owner = (MooPanelOwner *) lea_panel_get_panel_owner((LeaPanel *) self);
	moo_panel_owner_update_selection(moo_panel_owner);
}

static void l_layout_model_listener_iface_init(MooILayoutModelListenerInterface *iface) {
	iface->selectionSet = l_layout_model_selection_set;
}

/********************* end MooILayoutModelListener implementation *********************/
