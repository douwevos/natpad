/*
   File:    drapanelowner.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jun 20, 2015
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

#include "drapanelowner.h"
#include "draeditor.h"
#include "drastatusbarcontent.h"
#include "action/dragroupmain.h"
#include <cow.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPanelOwner"
#include <logging/catlog.h>

struct _DraPanelOwnerPrivate {
	LeaKeyContext *key_context;
	CatArrayWo *e_panel_list;
	DraGroupMain *group_main;
	LeaFrame *frame;

	/* next fields are used to create an ordered editor list. The order of the list is determined by the last time the editors
	 * received the focus. The field last_focus_order_id is increased every time one of the editor receives a focus and this
	 * editor its field focus_order_id is set with the value of last_focus_order_id. This allows us to create the ordered list
	 * used by the AstActionNextEditor and AstActionPrevioudEditor. When running these actions the field focus_order_id is not
	 * updated until the user releases the Ctrl-key (TODO: if the user assigns a different modifier key to switching then the
	 * mechanism doesnt work yet). Running these two actions is called browsing and while browsing the field browsing_index is
	 * set with a value other then -1. So whenever the field browsing_index is not -1 and an editor received the focus the
	 * editor-panel-owner knows the user is browsing and therefore does not update the field focus_order_id.
	 */
	int last_focus_order_id;
	int browsing_index;

	gboolean request_before_close;

	DraStatusBarContent *w_status_bar_content;

	ChaSearchService *search_service;
	WorService *wor_service;
	DraMacroManager *macro_manager;
	DraConnectorMap *connector_map;

	DraPreferencesWo *a_preferences;
};

static void l_panel_owner_iface_init(LeaIPanelOwnerInterface *iface);
static void l_lea_event_listener_iface_init(LeaIEventListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPanelOwner, dra_panel_owner, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraPanelOwner)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IPANEL_OWNER, l_panel_owner_iface_init);
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IEVENT_LISTENER, l_lea_event_listener_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_add_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel);
static void l_remove_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel);


static void dra_panel_owner_class_init(DraPanelOwnerClass *clazz) {
	clazz->addPanel = l_add_panel;
	clazz->removePanel = l_remove_panel;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_panel_owner_init(DraPanelOwner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPanelOwner *instance = DRA_PANEL_OWNER(object);
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(instance);
	if (priv->frame && priv->group_main) {
		lea_frame_unmerge_action_group(priv->frame, (LeaActionGroup *) priv->group_main);
		cat_unref_ptr(priv->group_main);
	}

	cat_unref_ptr(priv->key_context);
	cat_unref_ptr(priv->a_preferences);
	cat_unref_ptr(priv->e_panel_list);
	cat_unref_ptr(priv->group_main);
	cat_unref_ptr(priv->search_service);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->w_status_bar_content);
	cat_unref_ptr(priv->macro_manager);
	cat_unref_ptr(priv->connector_map);
	G_OBJECT_CLASS(dra_panel_owner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_panel_owner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void dra_panel_owner_construct(DraPanelOwner *panel_owner, LeaFrame *frame, WorService *wor_service) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	priv->frame = cat_ref_ptr(frame);
	priv->key_context = lea_key_context_new((CatStringWo *) cat_string_wo_new_with("dragonfly-key-context"));
	priv->e_panel_list = cat_array_wo_new();
	priv->macro_manager = dra_macro_manager_new();
	priv->group_main = dra_group_main_new(priv->key_context, panel_owner);
	priv->browsing_index = -1;
	priv->a_preferences = NULL;
	priv->last_focus_order_id = 0;
	priv->request_before_close = TRUE;
	priv->w_status_bar_content = dra_status_bar_content_new();
	priv->search_service = cha_search_service_new();
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->connector_map = dra_connector_map_new(wor_service);
	cat_ref_sink_ptr(priv->w_status_bar_content);
	lea_frame_merge_action_group(frame, (LeaActionGroup *) priv->group_main);
	lea_frame_add_event_listener(frame, LEA_IEVENT_LISTENER(panel_owner));
}


DraPanelOwner *dra_panel_owner_new(LeaFrame *frame, WorService *wor_service) {
	DraPanelOwner *result = g_object_new(DRA_TYPE_PANEL_OWNER, NULL);
	cat_ref_anounce(result);
	dra_panel_owner_construct(result, frame, wor_service);
	return result;
}

void dra_panel_owner_set_configuration(DraPanelOwner *panel_owner, DraPreferencesWo *a_prefs) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	cat_ref_swap(priv->a_preferences, a_prefs);
	dra_connector_map_reconfigure(priv->connector_map, a_prefs);
	CatIIterator *iterator = cat_array_wo_iterator(priv->e_panel_list);
	while(cat_iiterator_has_next(iterator)) {
		DraEditorPanel *editor = DRA_EDITOR_PANEL(cat_iiterator_next(iterator));
		cat_log_trace("setting configuration on editor:%p", editor);
		dra_editor_panel_reconfigure(editor, a_prefs);
	}
	cat_unref(iterator);
}


CatArrayWo *dra_panel_owner_enlist_all(const DraPanelOwner *panel_owner, CatArrayWo *e_enlist_in) {
	const DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private((DraPanelOwner *) panel_owner);
	if (e_enlist_in==NULL) {
		e_enlist_in = cat_array_wo_new();
	}
	cat_array_wo_append_all(e_enlist_in, priv->e_panel_list);
	return e_enlist_in;
}

DraConnectorMap *dra_panel_owner_get_connector_map(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	return priv->connector_map;
}


gboolean dra_panel_owner_close_editors(DraPanelOwner *panel_owner, CatArrayWo *editors_to_close, gboolean close_unsaved) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	priv->request_before_close = FALSE;
	gboolean result = TRUE;
	CatIIterator *iter = cat_array_wo_iterator(editors_to_close);
	while(cat_iiterator_has_next(iter)) {
		GObject *editor = (GObject *) cat_iiterator_next(iter);
		cat_log_detail("close editor=%o", editor);
		if (DRA_IS_EDITOR_PANEL(editor)) {
			DraEditorPanel *editor_panel = (DraEditorPanel *) editor;
			DraEditorPanelClass *dra_class = DRA_EDITOR_PANEL_GET_CLASS(editor_panel);
			if (!close_unsaved) {
				if (!dra_class->isSaved(editor_panel)) {
					cat_log_detail("not saved=%p", editor);
					result = FALSE;
					continue;
				}
			}
			if (!LEA_PANEL_GET_CLASS(editor_panel)->close((LeaPanel *) editor_panel)) {
				cat_log_detail("could not close=%p", editor);
				result = FALSE;
			}
		}
	}
	cat_unref_ptr(iter);

	cat_log_detail("finished");

	priv->request_before_close = TRUE;
	return result;
}


void dra_panel_owner_save_editors(DraPanelOwner *panel_owner, CatArrayWo *editor_panel_list) {
	if (editor_panel_list) {
		CatIIterator *iter = cat_array_wo_iterator(editor_panel_list);
		while(cat_iiterator_has_next(iter)) {
			DraEditorPanel *editor_panel = (DraEditorPanel *) cat_iiterator_next(iter);
			DraEditor *editor = dra_editor_panel_get_editor(editor_panel);
			ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
			if (!cha_document_is_saved_revision(document)) {
				DRA_EDITOR_PANEL_GET_CLASS(editor_panel)->save(editor_panel, FALSE, NULL);
			}
		}
		cat_unref_ptr(iter);
	}
}

LeaFrame *dra_panel_owner_get_frame(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	return priv->frame;
}

gboolean dra_panel_owner_get_request_before_close(const DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private((DraPanelOwner *) panel_owner);
	return priv->request_before_close;
}


static void l_activate_editor(DraEditorPanel *editor) {
	LeaPanel *panel = LEA_PANEL(editor);
	LeaSurface *owner = lea_panel_get_surface(panel);
	lea_surface_set_selected_tab(owner, (GtkWidget *) panel);
	lea_panel_request_focus(panel);
}


static int l_compare_ordered(DraEditorPanel *editor_panel_a, DraEditorPanel *editor_panel_b) {
	if (editor_panel_a==editor_panel_b) {
		return 0;
	}
	int idx_a = dra_editor_panel_get_focus_order_index(editor_panel_a);
	int idx_b = dra_editor_panel_get_focus_order_index(editor_panel_b);
	return idx_a==idx_b ? 0 : (idx_a<idx_b ? 1 : -1);
}


static CatArrayWo *l_create_browse_list(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	if (priv->e_panel_list==NULL || cat_array_wo_size(priv->e_panel_list)==0) {
		return NULL;
	}
	CatArrayWo *result = cat_array_wo_new();
	cat_array_wo_append_all(result, priv->e_panel_list);
	cat_array_wo_sort(result, (GCompareFunc) l_compare_ordered);

	cat_log_on_debug({
		int idx = 0;
		CatIIterator *iter = cat_array_wo_iterator(result);
		while(cat_iiterator_has_next(iter)) {
			DraEditorPanel *editor = (DraEditorPanel *) cat_iiterator_next(iter);
			cat_log_debug("panel[%d] = %d", idx++, dra_editor_panel_get_focus_order_index(editor));
		}
		cat_unref_ptr(iter);
	});

	return result;
}


void dra_panel_owner_browse_forward(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	CatArrayWo *browse_list = l_create_browse_list(panel_owner);
	if (browse_list==NULL) {
		return;
	}


	if (priv->browsing_index==-1) {
		priv->browsing_index = 0;
	}
	priv->browsing_index++;
	int count = cat_array_wo_size(browse_list);
	if (priv->browsing_index>=count) {
		priv->browsing_index = 0;
	}

	DraEditorPanel *editor = (DraEditorPanel *) cat_array_wo_get(browse_list, priv->browsing_index);
	cat_log_debug("select [%d] = %p", priv->browsing_index, editor);
	l_activate_editor(editor);
}


void dra_panel_owner_browse_backward(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	CatArrayWo *browse_list = l_create_browse_list(panel_owner);
	if (browse_list==NULL) {
		return;
	}
	if (priv->browsing_index<=0) {
		int count = cat_array_wo_size(browse_list);
		priv->browsing_index = count - 1;
	} else {
		priv->browsing_index--;
	}
	DraEditorPanel *editor = (DraEditorPanel *) cat_array_wo_get(browse_list, priv->browsing_index);
	cat_log_debug("select [%d] = %p", priv->browsing_index, editor);
	l_activate_editor(editor);
}


int dra_panel_owner_stop_browsing(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	priv->browsing_index = -1;
	cat_log_debug("stop_browsing: %d", priv->last_focus_order_id);
	return priv->last_focus_order_id++;
}

ChaSearchService *dra_panel_owner_get_search_service(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	return priv->search_service;
}

DraMacroManager *dra_panel_owner_get_macro_manager(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	return priv->macro_manager;
}


WorService *dra_panel_owner_get_wor_service(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	return priv->wor_service;
}

void dra_panel_owner_refresh_clipboard(DraPanelOwner *panel_owner) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	dra_group_main_refresh_clipboard(priv->group_main);
}


static void l_add_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	DraEditorPanel *editor = DRA_EDITOR_PANEL(panel);
	cat_array_wo_append(priv->e_panel_list, (GObject *) editor);
	dra_editor_panel_reconfigure(editor, priv->a_preferences);
}

static void l_remove_panel(DraPanelOwner *panel_owner, DraEditorPanel *panel) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	DraEditorPanel *editor = DRA_EDITOR_PANEL(panel);
	cat_ref_ptr(editor);
	int index = cat_array_wo_find_index(priv->e_panel_list, editor, -1);
	if (index>=0) {
		cat_array_wo_remove(priv->e_panel_list, index, NULL);
	}
	cat_unref_ptr(editor);
}



void dra_panel_owner_setup_context_menu(DraPanelOwner *panel_owner, LeaActionGroup *ctx_menu_grp, DraEditor *editor, ChaCursorWo *cursor) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(panel_owner);
	dra_group_main_context_menu(priv->group_main, ctx_menu_grp);
}

/********************* start LeaIPanelOwner implementation *********************/

static void l_panel_owner_add_panel(LeaIPanelOwner *self, LeaPanel *panel) {
	if (DRA_IS_EDITOR_PANEL(panel)) {
		DraPanelOwner *panel_owner = DRA_PANEL_OWNER(self);
		DRA_PANEL_OWNER_GET_CLASS(panel_owner)->addPanel(panel_owner, (DraEditorPanel *) panel);
	} else {
		cat_log_error("panel not of type DraEditorPanel: panel=%p, %s", panel, g_type_name_from_instance((GTypeInstance *) panel));
	}
}

static void l_panel_owner_remove_panel(LeaIPanelOwner *self, LeaPanel *panel) {
	if (DRA_IS_EDITOR_PANEL(panel)) {
		DraPanelOwner *panel_owner = DRA_PANEL_OWNER(self);
		DRA_PANEL_OWNER_GET_CLASS(panel_owner)->removePanel(panel_owner, (DraEditorPanel *) panel);
	} else {
		cat_log_error("panel not of type DraEditorPanel: panel=%p, %s", panel, g_type_name_from_instance((GTypeInstance *) panel));
	}
}

static LeaKeyContext *l_panel_owner_get_key_context(LeaIPanelOwner *self) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(DRA_PANEL_OWNER(self));
	return priv->key_context;
}

static gboolean l_panel_owner_panel_closing(LeaIPanelOwner *self, LeaPanel *panel) {
	cat_log_debug("closing return TRUE");
	return TRUE;
}



static GtkWidget *l_panel_owner_get_status_bar_content(LeaIPanelOwner *self) {
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(DRA_PANEL_OWNER(self));
	return (GtkWidget *) priv->w_status_bar_content;
}


static void l_panel_owner_iface_init(LeaIPanelOwnerInterface *iface) {
	iface->addPanel = l_panel_owner_add_panel;
	iface->removePanel = l_panel_owner_remove_panel;
	iface->getKeyContext = l_panel_owner_get_key_context;
	iface->panel_closing = l_panel_owner_panel_closing;
	iface->getStatusBarContent = l_panel_owner_get_status_bar_content;
}

/********************* end LeaIPanelOwner implementation *********************/


/********************* begin LeaIEventListener implementation *********************/

static void l_layout_panel_focused_changed(LeaIEventListener *self, LeaPanelFocusEvent *focus_event) {
	cat_log_debug("focus changed");
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(DRA_PANEL_OWNER(self));
	gboolean new_is_editor = DRA_IS_EDITOR_PANEL(focus_event->new_panel);

	DraEditorPanel *new_editor = NULL;
	if (new_is_editor) {
		new_editor = DRA_EDITOR_PANEL(focus_event->new_panel);
		if (priv->browsing_index == -1) {
			if (priv->browsing_index == -1) {
				dra_editor_panel_set_focus_order_index(new_editor, priv->last_focus_order_id++);
			}
		}

	}

	if (lea_action_set_visible_self((LeaAction *) priv->group_main, new_is_editor) && priv->frame) {
		lea_frame_update_menu(priv->frame);
	}

	cat_log_debug("new_editor=%o", new_editor);
	dra_group_main_set_editor_panel(priv->group_main, new_editor);

	dra_status_bar_content_set_active_editor_panel(priv->w_status_bar_content, new_editor);

	if (new_editor) {
		dra_macro_manager_set_editor(priv->macro_manager, dra_editor_panel_get_editor(new_editor));
	} else {
		dra_macro_manager_set_editor(priv->macro_manager, NULL);
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



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPanelOwner *instance = DRA_PANEL_OWNER(self);
	DraPanelOwnerPrivate *priv = dra_panel_owner_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p browsing_index=%d]", iname, self, priv->browsing_index);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

