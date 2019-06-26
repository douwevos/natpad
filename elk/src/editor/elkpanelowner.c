/*
   File:    elkpanelowner.c
   Project: elk
   Author:  Douwe Vos
   Date:    Aug 29, 2015
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

#include "elkpanelowner.h"
#include "../elkservice.h"
#include "../dialog/elkdialogs.h"
#include "../action/elkgroupmain.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkPanelOwner"
#include <logging/catlog.h>

struct _ElkPanelOwnerPrivate {
	ElkGroupMain *group_main;
	ElkDialogs *dialogs;
	MooService *moo_servie;
	CowIEntryAccessor *dra_prefs_accessor;
	ElkPreferencesService *elk_pref_service;
};

static void l_cow_listener_iface_init(CowIChangeListenerInterface *iface);
static void l_lea_event_listener_iface_init(LeaIEventListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkPanelOwner, elk_panel_owner, DRA_TYPE_PANEL_OWNER,
		G_ADD_PRIVATE(ElkPanelOwner)
		G_IMPLEMENT_INTERFACE(COW_TYPE_ICHANGE_LISTENER, l_cow_listener_iface_init);
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IEVENT_LISTENER, l_lea_event_listener_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_add_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel);
static void l_remove_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel);

static void elk_panel_owner_class_init(ElkPanelOwnerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraPanelOwnerClass *dra_class = DRA_PANEL_OWNER_CLASS(clazz);
	dra_class->addPanel = l_add_panel;
	dra_class->removePanel = l_remove_panel;
}

static void elk_panel_owner_init(ElkPanelOwner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkPanelOwner *instance = ELK_PANEL_OWNER(object);
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(instance);
	LeaFrame *frame = dra_panel_owner_get_frame((DraPanelOwner *) object);
	if (frame && priv->group_main) {
		lea_frame_unmerge_action_group(frame, (LeaActionGroup *) priv->group_main);
		cat_unref_ptr(priv->group_main);
	}
	cat_unref_ptr(priv->elk_pref_service);
	cat_unref_ptr(priv->group_main);
	cat_unref_ptr(priv->dialogs);
	G_OBJECT_CLASS(elk_panel_owner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_panel_owner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkPanelOwner *elk_panel_owner_new(ElkIService *elk_service, MooService *moo_service, VipService *vip_service, LeaFrame *frame) {
	ElkPanelOwner *result = g_object_new(ELK_TYPE_PANEL_OWNER, NULL);
	cat_ref_anounce(result);
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(result);
	dra_panel_owner_construct((DraPanelOwner *) result, frame, moo_service_get_worm_service(moo_service));
	priv->group_main = elk_group_main_new(elk_service, vip_service, frame);
	lea_frame_merge_action_group(frame, (LeaActionGroup *) priv->group_main);
	priv->dialogs = elk_dialogs_new(frame);
	priv->moo_servie = cat_ref_ptr(moo_service);

	ElkPreferencesService *elk_pref_service = elk_service_get_preferences_service((ElkService *) elk_service);
	priv->elk_pref_service = cat_ref_ptr(elk_pref_service);
	priv->dra_prefs_accessor = elk_preferences_services_get_chameleon_prefs_accessor(elk_pref_service);

	DraSpellHelper *spell_helper = dra_panel_owner_get_spell_helper((DraPanelOwner *) result);
	elk_preferences_service_set_spell_helper(elk_pref_service, spell_helper);

	return result;
}

ElkDialogs *elk_panel_owner_get_dialogs(ElkPanelOwner *panel_owner) {
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(panel_owner);
	return priv->dialogs;
}

MooService *elk_panel_owner_get_moose_service(ElkPanelOwner *panel_owner) {
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(panel_owner);
	return priv->moo_servie;
}


typedef void (*ApplyPrefsChange)(ElkPanelOwner *panel_owner, DraPreferencesWo *e_prefs);

static void l_basic_prefs_operation(ElkPanelOwner *panel_owner, ApplyPrefsChange apply_changes) {
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(panel_owner);
	ElkPreferencesContainer *container = elk_preferences_service_get_container(priv->elk_pref_service);
	ElkPreferencesWo *e_prefs = elk_preferences_container_get_editable(container);
	e_prefs = elk_preferences_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);
	DraPreferencesWo *dra_prefs = (DraPreferencesWo *) cow_ientry_accessor_get(priv->dra_prefs_accessor, e_prefs);
	cat_log_error("dra_prefs=%p, e_prefs=%p", dra_prefs, e_prefs);
	DraPreferencesWo *e_dra_prefs = NULL;
	if (dra_prefs) {
		e_dra_prefs = dra_preferences_wo_clone(dra_prefs, CAT_CLONE_DEPTH_MAIN);
	} else {
		e_dra_prefs = dra_preferences_wo_new();
	}
	apply_changes(panel_owner, e_dra_prefs);
	cow_ientry_accessor_set(priv->dra_prefs_accessor, e_prefs, e_dra_prefs);
	elk_preferences_container_set(container, e_prefs);

	ElkPreferencesWo *new_prefs = elk_preferences_container_get_fixed(container);
	cat_log_error("new prefs:%O", new_prefs);
//	elk_preferences_container_set(container, new_prefs);
	elk_preferences_service_save(priv->elk_pref_service);
}

void l_toggle_word_wrap_apply(ElkPanelOwner *panel_owner, DraPreferencesWo *e_prefs) {
	gboolean wrap = cha_preferences_wo_get_wrap_lines((ChaPreferencesWo *) e_prefs);
	cha_preferences_wo_set_wrap_lines((ChaPreferencesWo *) e_prefs, !wrap);
}

void elk_panel_owner_toggle_word_wrap(ElkPanelOwner *panel_owner) {
	l_basic_prefs_operation(panel_owner, l_toggle_word_wrap_apply);
}

void l_toggle_show_whitespace_apply(ElkPanelOwner *panel_owner, DraPreferencesWo *e_prefs) {
	gboolean show_whitespace = cha_preferences_wo_get_show_whitespace((ChaPreferencesWo *) e_prefs);
	cha_preferences_wo_set_show_whitespace((ChaPreferencesWo *) e_prefs, !show_whitespace);
}

void elk_panel_owner_toggle_show_whitespace(ElkPanelOwner *panel_owner) {
	l_basic_prefs_operation(panel_owner, l_toggle_show_whitespace_apply);
}

void l_toggle_mark_occurrences_apply(ElkPanelOwner *panel_owner, DraPreferencesWo *e_prefs) {
	gboolean mark_occurrences = cha_preferences_wo_get_mark_occurrences((ChaPreferencesWo *) e_prefs);
	cha_preferences_wo_set_mark_occurrences((ChaPreferencesWo *) e_prefs, !mark_occurrences);
}

void elk_panel_owner_toggle_mark_occurrences(ElkPanelOwner *panel_owner) {
	l_basic_prefs_operation(panel_owner, l_toggle_mark_occurrences_apply);
}


static void l_notify_new_editor_list(ElkPanelOwner *panel_owner) {
	CatArrayWo *enlisted = dra_panel_owner_enlist_all((DraPanelOwner *) panel_owner, NULL);
	enlisted = cat_array_wo_anchor(enlisted, 0);
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(panel_owner);
	elk_group_main_set_editor_list(priv->group_main, enlisted);
	cat_unref_ptr(enlisted);
}

static void l_add_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel) {
	DRA_PANEL_OWNER_CLASS(elk_panel_owner_parent_class)->addPanel(panel_owner, panel);
	l_notify_new_editor_list((ElkPanelOwner *) panel_owner);
}

static void l_remove_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel) {
	DRA_PANEL_OWNER_CLASS(elk_panel_owner_parent_class)->removePanel(panel_owner, panel);
	l_notify_new_editor_list((ElkPanelOwner *) panel_owner);
}


/********************* begin LeaIEventListener implementation *********************/

static void l_layout_panel_focused_changed(LeaIEventListener *self, LeaPanelFocusEvent *focus_event) {
	cat_log_debug("focus changed");
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(ELK_PANEL_OWNER(self));

	DraEditorPanel *editor_panel = NULL;
	if (DRA_IS_EDITOR_PANEL(focus_event->new_panel)) {
		editor_panel = DRA_EDITOR_PANEL(focus_event->new_panel);
	}

	cat_log_debug("new_editor=%o", editor_panel);
	elk_group_main_set_editor_panel(priv->group_main, editor_panel);

}

static void l_lea_event(LeaIEventListener *self, LeaEvent *event) {
	if (LEA_IS_PANEL_FOCUS_EVENT(event)) {
		l_layout_panel_focused_changed(self, (LeaPanelFocusEvent *) event);
	}

	LeaIEventListenerInterface *p_iface = g_type_interface_peek_parent(LEA_IEVENT_LISTENER_GET_INTERFACE(self));
	p_iface->event(self, event);
}

static void l_lea_event_listener_iface_init(LeaIEventListenerInterface *iface) {
	iface->event = l_lea_event;
}

/********************* end LeaIEventListener implementation *********************/

/********************* begin CowIChangeListener implementation *********************/

static void l_config_changed(CowIChangeListener *self, GObject *new_config) {
	ElkPanelOwner *instance = ELK_PANEL_OWNER(self);
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(instance);

	DraPreferencesWo *dra_prefs = (DraPreferencesWo *) cow_ientry_accessor_get(priv->dra_prefs_accessor, new_config);
	cat_log_error("dra_prefs=%O", dra_prefs);
	dra_panel_owner_set_configuration((DraPanelOwner *) instance, dra_prefs);
}

static void l_cow_listener_iface_init(CowIChangeListenerInterface *iface) {
	iface->configChanged = l_config_changed;
}

/********************* end CowIChangeListener implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkPanelOwner *instance = ELK_PANEL_OWNER(self);
	ElkPanelOwnerPrivate *priv = elk_panel_owner_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p group_main=%p]", iname, self, priv->group_main);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
