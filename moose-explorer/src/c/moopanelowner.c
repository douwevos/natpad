/*
   File:    moopanelowner.c
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


#include "moopanelowner.h"
#include "moopanel.h"
#include "action/moogroupmain.h"
#include <leafhopper.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooPanelOwner"
#include <logging/catlog.h>

struct _MooPanelOwnerPrivate {
	MooNodeRenderRegistry *render_registry;
	MooGroupMain *group_main;
	LeaMenuAction *menu_act_add_project;
	MooPanel *active_panel;
	CatWeakReference *frame_ref;
	MooClipboard *moo_clipboard;
	LeaKeyContext *key_context;
};

static void l_panel_owner_iface_init(LeaIPanelOwnerInterface *iface);
static void l_lea_event_listener_iface_init(LeaIEventListenerInterface *iface);


G_DEFINE_TYPE_WITH_CODE(MooPanelOwner, moo_panel_owner, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IPANEL_OWNER, l_panel_owner_iface_init);
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IEVENT_LISTENER, l_lea_event_listener_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
//static void l_setup_menu(MooPanelOwnerPrivate *priv, LeaMenuBar *menu_bar);

static void moo_panel_owner_class_init(MooPanelOwnerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooPanelOwnerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_panel_owner_init(MooPanelOwner *instance) {
	MooPanelOwnerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_PANEL_OWNER, MooPanelOwnerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooPanelOwner *instance = MOO_PANEL_OWNER(object);
	MooPanelOwnerPrivate *priv = instance->priv;

	cat_log_debug("priv->group_main=%o", priv->group_main);
	if (priv->group_main && priv->frame_ref) {
		LeaFrame *frame = (LeaFrame *) cat_weak_reference_get(priv->frame_ref);
		cat_log_debug("frame=%o", frame);
		if (frame) {
			lea_frame_unmerge_action_group(frame, (LeaActionGroup *) priv->group_main);
			cat_unref_ptr(frame);
		}
	}

	cat_unref_ptr(priv->render_registry);
	cat_unref_ptr(priv->group_main);
	cat_unref_ptr(priv->frame_ref);
	cat_unref_ptr(priv->key_context);
	cat_unref_ptr(priv->moo_clipboard);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooPanelOwner *moo_panel_owner_new(MooService *moo_service, LeaFrame *frame) {
	MooPanelOwner *result = g_object_new(MOO_TYPE_PANEL_OWNER, NULL);
	cat_ref_anounce(result);
	MooPanelOwnerPrivate *priv = result->priv;
	priv->key_context = lea_key_context_new(cat_string_wo_new_with("moose-kc"));
	priv->moo_clipboard = moo_clipboard_new(moo_service, frame);
	priv->render_registry = moo_node_render_registry_new();
	priv->group_main = moo_group_main_new(moo_service, frame, priv->moo_clipboard, priv->key_context);
	priv->active_panel = NULL;
	priv->frame_ref = cat_weak_reference_new((GObject *) frame);

	lea_frame_merge_action_group(frame, (LeaActionGroup *) priv->group_main);

	lea_frame_add_event_listener(frame, LEA_IEVENT_LISTENER(result));
	return result;
}


MooNodeRenderRegistry *moo_panel_owner_get_render_registry(MooPanelOwner *panel_owner) {
	MooPanelOwnerPrivate *priv = MOO_PANEL_OWNER_GET_PRIVATE(panel_owner);
	return priv->render_registry;
}


void moo_panel_owner_update_selection(MooPanelOwner *panel_owner) {
	MooPanelOwnerPrivate *priv = MOO_PANEL_OWNER_GET_PRIVATE(panel_owner);
	CatArrayWo *selection = NULL;
	VipSnapshot *vip_snapshot = NULL;
	if (priv->active_panel) {
		selection = moo_panel_get_selection(priv->active_panel);
		vip_snapshot = moo_panel_get_vip_snapshot(priv->active_panel);
	}
	cat_log_debug("selection=%o, vip-snapshot=%o", selection, vip_snapshot);
	moo_group_main_set_selection(priv->group_main, vip_snapshot, selection);
}


/********************* start LeaIPanelOwner implementation *********************/


static void l_ipanel_owner_add_panel(LeaIPanelOwner *self, LeaPanel *panel) {
}

static void l_ipanel_owner_remove_panel(LeaIPanelOwner *self, LeaPanel *panel) {
}


static gboolean l_ipanel_owner_panel_closing(LeaIPanelOwner *self, LeaPanel *panel) {
	return TRUE;
}

static LeaKeyContext *l_panel_owner_get_key_context(LeaIPanelOwner *self) {
	return MOO_PANEL_OWNER_GET_PRIVATE(self)->key_context;

}


static void l_panel_owner_iface_init(LeaIPanelOwnerInterface *iface) {
	iface->addPanel = l_ipanel_owner_add_panel;
	iface->removePanel = l_ipanel_owner_remove_panel;
	iface->panel_closing = l_ipanel_owner_panel_closing;
	iface->getKeyContext = l_panel_owner_get_key_context;
}

/********************* end LeaIPanelOwner implementation *********************/



/********************* begin LeaIEventListener implementation *********************/

static void l_layout_panel_focused_changed(LeaIEventListener *self, LeaPanelFocusEvent *focus_event) {
	cat_log_debug("focus changed");
	MooPanelOwnerPrivate *priv = MOO_PANEL_OWNER_GET_PRIVATE(self);

	gboolean is_moose_panel = MOO_IS_PANEL(focus_event->new_panel);

	if (is_moose_panel) {
		MooPanel *new_panel = MOO_PANEL(focus_event->new_panel);
		cat_ref_swap(priv->active_panel, new_panel);
	} else {
		cat_unref_ptr(priv->active_panel);
	}

	moo_panel_owner_update_selection(MOO_PANEL_OWNER(self));


	if (moo_group_main_set_has_focus(priv->group_main, is_moose_panel)) {
		LeaFrame *frame = (LeaFrame *) cat_weak_reference_get(priv->frame_ref);
		if (frame) {
			lea_frame_update_menu(frame);
			cat_unref_ptr(frame);
		}
	}
}

static void l_lea_event(LeaIEventListener *self, LeaEvent *event) {
	if (LEA_IS_PANEL_FOCUS_EVENT(event)) {
		l_layout_panel_focused_changed(self, (LeaPanelFocusEvent *) event);
	}
}

static void l_lea_event_listener_iface_init(LeaIEventListenerInterface *iface) {
	iface->event = l_lea_event;
}

/********************* end LeaIEventListener implementation *********************/
