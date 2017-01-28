/*
   File:    draeditorpanel.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Mar 22, 2015
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

#include "context/dracontexteditor.h"
#include "spell/draspellrequestfactory.h"
#include "occurrences/draoccurrencesrequest.h"
#include "draeditorpanel.h"
#include "drapanelowner.h"
#include "dratagoverviewbar.h"
#include "dralineinfowo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraEditorPanel"
#include <logging/catlog.h>

struct _DraEditorPanelPrivate {
	DraEditor *editor;
	DraTagOverviewBar *tag_overview_bar;
	gboolean is_saved_in_tab_name;
	int tab_name_last_revision_version;
	int focus_order_index;
	gboolean focus_active;
	gboolean focus_set;
	gboolean focus_active_and_set;
	DraOccurrencesRequest *occurrences_request;
	CatStringWo *line_info_key;
};

static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraEditorPanel, dra_editor_panel, LEA_TYPE_PANEL,
		G_ADD_PRIVATE(DraEditorPanel)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, l_document_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_is_saved(const DraEditorPanel *self);
static DraContextEditor *l_create_context_editor(DraEditorPanel *editor_panel);
static void l_panel_focus_activated(LeaPanel *panel, gboolean focus_active);
static void l_panel_focus_set(LeaPanel *panel, gboolean focus_set);
static gboolean l_panel_close(LeaPanel *panel);
static void l_reconfigure(DraEditorPanel *editor_panel, DraPreferencesWo *a_prefs);


static void dra_editor_panel_class_init(DraEditorPanelClass *clazz) {
	clazz->reconfigure = l_reconfigure;
	clazz->initEditor = NULL;
	clazz->createContextEditor = l_create_context_editor;

	clazz->doSaveRequest = NULL;
	clazz->isSaved = l_is_saved;
	clazz->getName = NULL;
	clazz->save = NULL;
	clazz->revert = NULL;

	LeaPanelClass *panel_class = LEA_PANEL_CLASS(clazz);
	panel_class->focusActivated = l_panel_focus_activated;
	panel_class->focusSet = l_panel_focus_set;
	panel_class->close = l_panel_close;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_editor_panel_init(DraEditorPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraEditorPanel *instance = DRA_EDITOR_PANEL(object);
//	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(instance);
//	cat_unref_ptr(priv->editor);
	G_OBJECT_CLASS(dra_editor_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_editor_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_key_release_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data);

void dra_editor_panel_construct(DraEditorPanel *editor_panel, LeaIPanelOwner *panel_owner, ChaDocument *document, DraIConnectorRequestFactory *request_factory) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);
	DraEditorPanelClass *editor_pan_class = DRA_EDITOR_PANEL_GET_CLASS(editor_panel);
	priv->focus_order_index = 0;


//	CatStringWo *a_slot_key_spell = cat_string_wo_new("spell-checker");
//	a_slot_key_spell = cat_string_wo_anchor(a_slot_key_spell, 0);
//	cat_log_debug("yep:a_slot_key_spell=%O", a_slot_key_spell);
//	DraIConnectorRequestFactory *request_connectory_factory = (DraIConnectorRequestFactory *) dra_spell_request_factory_new(a_slot_key_spell);

	DraPanelOwner *dra_panel_owner = DRA_PANEL_OWNER(panel_owner);
	DraConnectorMap *connector_map = dra_panel_owner_get_connector_map(dra_panel_owner);

	WorService *wor_service = dra_panel_owner_get_wor_service(dra_panel_owner);

	if (editor_pan_class->initEditor) {
		priv->editor = editor_pan_class->initEditor(document, connector_map, request_factory);
	} else {
		priv->editor = dra_editor_new(document, connector_map, request_factory, wor_service);
	}

	ChaSearchService *search_service = dra_panel_owner_get_search_service(dra_panel_owner);
	cha_editor_set_search_service((ChaEditor *) priv->editor, search_service);

	DraContextEditor *context_editor = editor_pan_class->createContextEditor(editor_panel);
	dra_editor_set_context_editor(priv->editor, context_editor);
	cat_unref_ptr(context_editor);

	g_signal_connect(priv->editor, "key-release-event", G_CALLBACK(l_key_release_event), editor_panel);


	priv->is_saved_in_tab_name = FALSE;
	priv->tab_name_last_revision_version = -1;
	priv->occurrences_request = NULL;
	GtkScrolledWindow *scrolled_window = (GtkScrolledWindow *) gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
#ifdef	gtk_scrolled_window_set_overlay_scrolling
	gtk_scrolled_window_set_overlay_scrolling(scrolled_window, FALSE);
#endif
	gtk_container_add((GtkContainer *) scrolled_window, (GtkWidget *) priv->editor);
	gtk_widget_set_can_focus((GtkWidget *) scrolled_window, FALSE);


	GtkBox *box = (GtkBox *) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	gtk_box_pack_start(box, (GtkWidget *) scrolled_window, TRUE, TRUE, 0);

	priv->line_info_key = NULL;
	if (request_factory) {
		DraIConnectorRequestFactoryInterface *req_fact_iface = DRA_ICONNECTOR_REQUEST_FACTORY_GET_INTERFACE(request_factory);
		priv->line_info_key = req_fact_iface->getSlotKey(request_factory);
		cat_ref_ptr(priv->line_info_key);
	}

	priv->tag_overview_bar = dra_tag_overview_bar_new();
	gtk_box_pack_start(box, (GtkWidget *) priv->tag_overview_bar, FALSE, TRUE, 0);

	lea_panel_construct((LeaPanel *) editor_panel, panel_owner, NULL, cat_string_wo_new_with("Chameleon"), (GtkWidget *) box);

	gtk_widget_show_all((GtkWidget *) editor_panel);

	cha_document_add_listener(document, (ChaIDocumentListener *) editor_panel);

//	gtk_widget_grab_focus(priv->editor);
}


DraEditorPanel *dra_editor_panel_new(LeaIPanelOwner *panel_owner, ChaDocument *document, DraIConnectorRequestFactory *request_factory) {
	DraEditorPanel *result = g_object_new(DRA_TYPE_EDITOR_PANEL, NULL);
	cat_ref_anounce(result);
	dra_editor_panel_construct(result, panel_owner, document, request_factory);
	return result;
}


DraEditor *dra_editor_panel_get_editor(DraEditorPanel *editor_panel) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);
	return priv->editor;
}

const CatStringWo *dra_editor_panel_get_name(const DraEditorPanel *editor_panel) {
	DraEditorPanelClass *pan_class = DRA_EDITOR_PANEL_GET_CLASS(editor_panel);
	if (pan_class->getName) {
		return pan_class->getName(editor_panel);
	}
	return NULL;
}


void dra_editor_panel_reconfigure(DraEditorPanel *editor_panel, DraPreferencesWo *a_prefs) {
	DraEditorPanelClass *pan_class = DRA_EDITOR_PANEL_GET_CLASS(editor_panel);
	if (pan_class->reconfigure) {
		pan_class->reconfigure(editor_panel, a_prefs);
	}
}

int dra_editor_panel_get_focus_order_index(const DraEditorPanel *editor_panel) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private((DraEditorPanel *) editor_panel);
	return priv->focus_order_index;
}

void dra_editor_panel_set_focus_order_index(DraEditorPanel *editor_panel, int focus_order_index) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);
	priv->focus_order_index = focus_order_index;
}

static gboolean l_is_saved(const DraEditorPanel *self) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(DRA_EDITOR_PANEL(self));
	ChaDocument *document = cha_editor_get_document((ChaEditor *) priv->editor);
	return cha_document_is_saved_revision(document);
}

static DraContextEditor *l_create_context_editor(DraEditorPanel *editor_panel) {
	return dra_context_editor_new(editor_panel);
}

static gboolean l_key_release_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data) {
	gboolean ctrlPressed = eev->state & GDK_CONTROL_MASK;

	if (ctrlPressed!=0 && (eev->keyval=GDK_KEY_Control_L) && eev->is_modifier!=0) {
		cat_log_detail("Release ctrlPressed=%d cont_l=%d, keyval=%d, keycode=%d, eev->state=%d, eev->ismod=%d!!!!",ctrlPressed, GDK_KEY_Control_L, eev->keyval, eev->hardware_keycode, eev->state, eev->is_modifier);
		DraEditorPanel *editor_panel = DRA_EDITOR_PANEL(data);
		DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);
		cat_log_detail("editor_panel=%p", editor_panel);
		DraPanelOwner *panel_owner = (DraPanelOwner *) lea_panel_get_panel_owner((LeaPanel *) editor_panel);
		priv->focus_order_index = dra_panel_owner_stop_browsing(panel_owner);
	}
	return FALSE;
}

static void l_panel_test_for_reactivation(DraEditorPanel *editor_panel) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);
	gboolean focus_active_and_set = priv->focus_active && priv->focus_set;
	if (priv->focus_active_and_set != focus_active_and_set) {
		priv->focus_active_and_set = focus_active_and_set;
		DraEditorPanelClass *editor_panel_class = DRA_EDITOR_PANEL_GET_CLASS(editor_panel);
		editor_panel_class->reactivated(editor_panel, focus_active_and_set);
	}
}

static void l_panel_focus_set(LeaPanel *panel, gboolean focus_set) {
	DraEditorPanel *editor_panel = DRA_EDITOR_PANEL(panel);
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);
	priv->focus_set = focus_set;

	LeaPanelClass *panel_class = LEA_PANEL_CLASS(dra_editor_panel_parent_class);
	if (panel_class->focusSet) {
		panel_class->focusSet(panel, focus_set);
	}

	l_panel_test_for_reactivation(editor_panel);
}

static void l_panel_focus_activated(LeaPanel *panel, gboolean focus_active) {
	DraEditorPanel *editor_panel = DRA_EDITOR_PANEL(panel);
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);

	dra_editor_set_focus_active(priv->editor, focus_active);
	DraPanelOwner *panel_owner = (DraPanelOwner *) lea_panel_get_panel_owner(panel);
	if (focus_active) {
		dra_panel_owner_refresh_clipboard(panel_owner);
	}
	priv->focus_active = focus_active;
	LeaPanelClass *panel_class = LEA_PANEL_CLASS(dra_editor_panel_parent_class);
	if (panel_class->focusActivated) {
		panel_class->focusActivated(panel, focus_active);
	}

	l_panel_test_for_reactivation(editor_panel);
}

static gboolean l_panel_close(LeaPanel *panel) {
	DraEditorPanel *editor_panel = DRA_EDITOR_PANEL(panel);
	DraPanelOwner *panel_owner = (DraPanelOwner *) lea_panel_get_panel_owner(panel);
	gboolean req = dra_panel_owner_get_request_before_close(panel_owner);
	if (req) {
		DraEditorPanelClass *editor_panel_class = DRA_EDITOR_PANEL_GET_CLASS(editor_panel);
		if (!editor_panel_class->isSaved(editor_panel)) {
			gboolean do_save = TRUE;
			if (editor_panel_class->doSaveRequest) {
				int response = editor_panel_class->doSaveRequest(editor_panel);
				if (response==GTK_RESPONSE_NO) {
					do_save = FALSE;
				} else if (response==GTK_RESPONSE_YES) {
					do_save = TRUE;
				} else {
					return FALSE;
				}
			}

			if (do_save) {
				ChaAsyncBasic *async = cha_async_basic_new();
				editor_panel_class->save(editor_panel, FALSE, (ChaIOAsync *) async);
				gboolean save_result = cha_async_basic_wait(async, 15000);
				if (!save_result) {
					return FALSE;
				}
			}
		}
	}

	return LEA_PANEL_CLASS(dra_editor_panel_parent_class)->close(panel);
}

static void l_reconfigure(DraEditorPanel *editor_panel, DraPreferencesWo *a_prefs) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(editor_panel);
	cha_editor_set_preferences((ChaEditor *) priv->editor, (ChaPreferencesWo *) a_prefs);
}


/********************* start ChaIDocumentListener implementation *********************/

static void l_on_new_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(DRA_EDITOR_PANEL(self));

	/* change panel name if version changed and state changed */
	int plver = cha_revision_wo_get_page_list_version(a_new_revision);
	cat_log_debug("plver=%d, priv->tab_name_last_revision_version=%d", plver, priv->tab_name_last_revision_version);
	if (priv->tab_name_last_revision_version!=plver) {
		gboolean force = priv->tab_name_last_revision_version<0;
		priv->tab_name_last_revision_version = plver;
		ChaDocument *document = cha_editor_get_document((ChaEditor *) priv->editor);
		gboolean is_saved = cha_document_is_saved_revision(document);
		cat_log_debug("is_saved=%d, is_saved_in_tab_name=%d, force=%d", is_saved, priv->is_saved_in_tab_name, force);
		if ((is_saved!=priv->is_saved_in_tab_name) || force) {
			priv->is_saved_in_tab_name = is_saved;
			const CatStringWo *name = dra_editor_panel_get_name(DRA_EDITOR_PANEL(self));
			if (!is_saved) {
				CatStringWo *e_usname = cat_string_wo_new_with("*");
				cat_string_wo_append(e_usname, name);
				name = cat_string_wo_anchor(e_usname, 0);
			} else {
				cat_ref_ptr(name);
			}
			lea_panel_set_label((LeaPanel *) self, name);
			cat_unref_ptr(name);
		}
	}

	DraPanelOwner *panel_owner = DRA_PANEL_OWNER(lea_panel_get_panel_owner((LeaPanel *) self));
	WorService *wor_service = dra_panel_owner_get_wor_service(panel_owner);
	DraOccurrencesRequest *occ_req = dra_occurrences_request_new(priv->editor, a_new_revision);
	wor_request_set_time_out((WorRequest *) occ_req, cat_date_current_time()+750);
	wor_service_post_request(wor_service, (WorRequest *) occ_req);
	cat_unref_ptr(occ_req);

}

static void l_on_new_saved_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(DRA_EDITOR_PANEL(self));
	priv->tab_name_last_revision_version = -1;
	l_on_new_revision(self, a_new_revision);

}


static void l_on_enrichment_slot_notify(ChaIDocumentListener *self, ChaRevisionWo *a_revision, GObject *slot_key, gpointer user_data) {
	DraEditorPanelPrivate *priv = dra_editor_panel_get_instance_private(DRA_EDITOR_PANEL(self));
	cat_log_debug("priv->line_info_key=%O, slot_key=%O", priv->line_info_key, slot_key);
	if ((GObject *) priv->line_info_key!=slot_key) {
		return;
	}
	ChaDocument *doc = cha_editor_get_document((ChaEditor *) priv->editor);
	if (cha_document_is_big_file_mode(doc)) {
		return;
	}

	CatIntArrayWo *int_array = cat_int_array_wo_new();
	int slot_index = cha_revision_wo_get_slot_index(a_revision, slot_key, -1);
	ChaLineIterator *line_iter = cha_line_iterator_new(a_revision, 0);
	int abs_line_index = 0;
	while(cat_iiterator_has_next((CatIIterator *) line_iter)) {
		ChaLineLocationWo *line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) line_iter);
		ChaLineWo *line = cha_revision_wo_line_at_location(a_revision, line_location);
		DraLineInfoWo *a_line_info = (DraLineInfoWo *) cha_line_wo_get_slot_content_ref(line, slot_index, slot_key);
		if (a_line_info) {
			CatArrayWo *tags = dra_line_info_wo_get_line_tags(a_line_info);
			if (tags && cat_array_wo_size(tags)>0) {
				cat_int_array_wo_append(int_array, abs_line_index);
				cat_log_trace("adding tagged-line=%d", abs_line_index);
			}
		}

		abs_line_index++;
	}
	cat_unref_ptr(line_iter);

	dra_tag_overview_bar_set_tagged_lines(priv->tag_overview_bar, int_array, abs_line_index);
	cat_unref_ptr(int_array);
}


static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	iface->onNewRevision = l_on_new_revision;
	iface->onNewSavedRevision = l_on_new_saved_revision;
	iface->onEnrichmentSlotNotify = l_on_enrichment_slot_notify;
}

/********************* end ChaIDocumentListener implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
