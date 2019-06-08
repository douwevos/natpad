/*
   File:    draeditor.c
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

#include "draeditor.h"
#include "draeditorpanel.h"
#include "drapanelowner.h"
#include "dralineinfowo.h"
#include "dralinetagwo.h"
#include "uow/drauowindent.h"
#include "autocomplete/draaccontentrequest.h"
#include "autocomplete/draautocompletepopup.h"
#include "autocomplete/draaccontext.h"
#include "autocomplete/draiaccontentprovider.h"
#include "autocomplete/draaccontentrequest.h"
#include "autocomplete/draactemplateentry.h"
#include "highlighting/dratagpopup.h"

#include "action/macro/draactionreplay.h"
#include "action/draactionspellsuggest.h"

#include "dradocumentview.h"
#include "document/draconnectormap.h"



#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraEditor"
#include <logging/catlog.h>

struct _DraEditorPrivate {
	DraConnectorMap *connector_map;
	DraContextEditor *context_editor;

	DraAutoCompletePopup *auto_complete_popup;
	gboolean do_not_kill_assist;

	CatStringWo *occurrence_text;

	ChaLineLocationWo *tag_line_location;
	long long tag_y_marker_view;
	ChaLineLocationWo *tag_current_line_location;
	DraTagPopup *tag_popup;
	int tag_timeout_tag;

	CatStringWo *slot_key;

	LeaMenuAction *context_menu;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraEditor, dra_editor, CHA_TYPE_EDITOR,
		G_ADD_PRIVATE(DraEditor)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static ChaDocumentView *l_initialize_document_view(ChaEditor *editor, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget);
static void l_marker_clicked(ChaEditor *editor, ChaLineLocationWo *location, long long y_marker_view);
static void l_marker_over(ChaEditor *editor, ChaLineLocationWo *location, long long y_marker_view);
static void l_marker_out(ChaEditor *editor);
static void l_show_context_menu(DraEditor *editor, ChaCursorWo *cursor, int xpos, int ypos, DraLineTagWo *spell_tag, GdkEvent *event);


static void dra_editor_class_init(DraEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaEditorClass *cha_clazz = CHA_EDITOR_CLASS(clazz);
	cha_clazz->initializeDocumentView = l_initialize_document_view;
	cha_clazz->markerClicked = l_marker_clicked;
	cha_clazz->markerOver = l_marker_over;
	cha_clazz->markerOut = l_marker_out;

	clazz->showContextMenu = l_show_context_menu;
}

static void dra_editor_init(DraEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraEditor *instance = DRA_EDITOR(object);
	DraEditorPrivate *priv = dra_editor_get_instance_private(instance);
	if (priv->connector_map && priv->slot_key) {
		ChaDocument *document = cha_editor_get_document((ChaEditor *) instance);
		dra_connector_map_disconnect_document(priv->connector_map, document, priv->slot_key);

	}
	cat_unref_ptr(priv->slot_key);
	cat_unref_ptr(priv->connector_map);
	cat_unref_ptr(priv->context_editor);
	cat_unref_ptr(priv->occurrence_text);
	cat_unref_ptr(priv->tag_line_location);
	cat_unref_ptr(priv->tag_current_line_location);
	cat_unref_ptr(priv->tag_popup);
	if (priv->context_menu) {
		GtkMenuShell *menu = lea_menu_action_get_menu_shell(priv->context_menu);
		gtk_widget_destroy(GTK_WIDGET(menu));
	}
	cat_unref_ptr(priv->context_menu);

	G_OBJECT_CLASS(dra_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_key_press_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data);
static gboolean l_key_release_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data);
static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data);
static gboolean l_focus_out_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data);



DraEditor *dra_editor_new(ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *connector_factory, WorService *wor_service) {
	DraEditor *result = g_object_new(DRA_TYPE_EDITOR, NULL);
	cat_ref_anounce(result);
	DraEditorPrivate *priv = dra_editor_get_instance_private(result);
	priv->connector_map = cat_ref_ptr(connector_map);
	priv->context_editor = NULL;
	priv->auto_complete_popup = NULL;
	priv->do_not_kill_assist = FALSE;
	priv->occurrence_text = NULL;
	priv->tag_line_location = NULL;
	priv->tag_current_line_location = NULL;
	priv->tag_popup = NULL;
	priv->tag_timeout_tag = 0;

	priv->slot_key = NULL;
	priv->context_menu = NULL;

	if (connector_factory) {
		CatStringWo *a_slot_key = DRA_ICONNECTOR_REQUEST_FACTORY_GET_INTERFACE(connector_factory)->getSlotKey(connector_factory);
		dra_connector_map_connect_document(priv->connector_map, document, a_slot_key, connector_factory);
		priv->slot_key = cat_ref_ptr(a_slot_key);
	}


	g_signal_connect(result, "key-press-event", G_CALLBACK(l_key_press_event), result);
	g_signal_connect(result, "key-release-event", G_CALLBACK(l_key_release_event), result);

	g_signal_connect(result, "button-press-event", G_CALLBACK(l_button_press_event_cb), result);
	g_signal_connect(result, "focus-out-event", G_CALLBACK(l_focus_out_event_cb), result);

	cha_editor_construct((ChaEditor *) result, document, CHA_EDITOR_FLAG_DEFAULT_MOUSE_HANDLING);

	return result;
}


void dra_editor_set_focus_active(DraEditor *editor, gboolean focus_active) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	if (priv->auto_complete_popup) {
		cat_log_debug("destroying popup");
		GtkWindow *popup_window = dra_auto_complete_popup_get_window(priv->auto_complete_popup);
	//		gtk_widget_get_window((GtkWidget *) popup_window);
		priv->auto_complete_popup = NULL;
		gtk_widget_destroy(GTK_WIDGET(popup_window));
	}
}

void dra_editor_set_occurrence_text(DraEditor *editor, CatStringWo *text) {
	if (text==NULL) {
		return;
	}
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	if (!cat_string_wo_equal(text, priv->occurrence_text)) {
		cat_ref_swap(priv->occurrence_text, text);
		ChaSearchQueryWo *query = cha_search_query_wo_new();
		cha_search_query_wo_set_match_case(query, TRUE);
		cha_search_query_wo_set_match_words(query, TRUE);
		cha_search_query_wo_set_text(query, text);

		DraDocumentView *document_view = (DraDocumentView *) cha_editor_get_document_view((ChaEditor *) editor);

		ChaOccurrencesResultSet *occurrences_rset = cha_occurrences_result_set_new(query, NULL);

		dra_document_view_set_occurrences_rset(document_view, occurrences_rset);
		cat_unref_ptr(occurrences_rset);
		cat_unref_ptr(query);
		cha_document_view_invalidate_lines((ChaDocumentView *) document_view);
	}
}


void dra_editor_set_context_editor(DraEditor *editor, DraContextEditor *context_editor) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	cat_ref_swap(priv->context_editor, context_editor);
}

DraContextEditor *dra_editor_get_context_editor(DraEditor *editor) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	return priv->context_editor;
}


void dra_editor_goto_line(DraEditor *editor) {
//	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	LeaBasicInputDialog *dialog = lea_basic_input_dialog_new((GtkWidget *) editor, "Line number", "Enter line number");
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_OK) {
		const gchar *txt_line_number = lea_basic_input_dialog_get_entry_text(dialog);
		long long line_nr = atoll(txt_line_number);
		gtk_widget_destroy(GTK_WIDGET(dialog));
		if (line_nr>=1) {
			ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
			ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
			ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
			ChaLineLocationWo *nll = cha_revision_wo_calculate_line_location(e_revision, line_nr-1);
			if (nll) {
				ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
				cha_cursor_wo_set_line_location(e_cursor, nll);
				cat_unref(nll);
				cha_cursor_wo_set_x_cursor_bytes(e_cursor, 0);
			}
			cha_document_anchor_document(document);
			if (nll) {
				cha_document_view_move_view_to_focus(document_view, FALSE);
				cha_document_view_invalidate_lines(document_view);
			}
		}
//		g_free(txt_line_number);
		return;
	}
	gtk_widget_destroy(GTK_WIDGET(dialog));
}


static ChaDocumentView *l_initialize_document_view(ChaEditor *editor, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget) {
	DraEditor *instance = DRA_EDITOR(editor);
	DraEditorPrivate *priv = dra_editor_get_instance_private(instance);
	CatStringWo *a_line_info_key = priv->slot_key;
	return (ChaDocumentView *) dra_document_view_new(document, pango_context, widget, a_line_info_key);
}


static void l_open_tag_popup(ChaEditor *editor, ChaLineLocationWo *location, long long y_marker_view) {
	DraEditorPrivate *priv = dra_editor_get_instance_private((DraEditor *) editor);
	CatStringWo *a_line_info_key = priv->slot_key;
	CatStringWo *text = cat_string_wo_new();
	cat_log_debug("a_line=%O", a_line_info_key);
	if (a_line_info_key) {
		ChaDocument *document = cha_editor_get_document(editor);
		ChaRevisionWo *a_revision = cha_document_get_current_revision_ref(document);
		ChaPageWo *page = cha_revision_wo_page_at(a_revision, cha_line_location_wo_get_page_index(location));
		cha_page_wo_hold_lines(page);
		ChaLineWo *a_line = cha_page_wo_line_at(page, cha_line_location_wo_get_page_line_index(location));

		cat_log_debug("a_line=%O", a_line);

		DraLineInfoWo *line_info = (DraLineInfoWo *)
				cha_line_wo_get_slot_content_ref(a_line, -1, (GObject *) a_line_info_key);
		if (line_info) {
			CatArrayWo *line_tags = dra_line_info_wo_get_line_tags(line_info);
			cat_log_debug("line_tags=%O", line_tags);
			if (line_tags) {
				gboolean is_first = TRUE;
				CatIIterator *ltag_iter = cat_array_wo_iterator(line_tags);
				while(cat_iiterator_has_next(ltag_iter)) {
					DraLineTagWo *line_tag = (DraLineTagWo *) cat_iiterator_next(ltag_iter);
					cat_log_debug("line_tag=%O", line_tag);
					CatStringWo *txt = dra_line_tag_wo_get_text(line_tag);

					if (txt) {
						if (is_first) {
							is_first = FALSE;
						} else {
							cat_string_wo_append_chars(text, "\n");
						}
						cat_string_wo_append(text, txt);
					}
				}
				cat_unref_ptr(ltag_iter);
			}
		}
		cha_page_wo_release_lines(page);
		cat_unref_ptr(line_info);
		cat_unref_ptr(a_line)
		cat_unref_ptr(a_revision)
	}

	cat_log_debug("text=%O", text);


	if (cat_string_wo_length(text)>0) {
		cat_unref_ptr(priv->tag_popup);

		ChaDocumentView *document_view = cha_editor_get_document_view(editor);
		long long doc_view_y = cha_document_view_get_view_y(document_view);
		y_marker_view -= doc_view_y;
		const ChaDocumentViewContext ctx = cha_document_view_get_context(document_view);

		priv->tag_popup = dra_tag_popup_new(editor, ctx.line_height + ctx.line_nr_view_width, (int) y_marker_view);
		dra_tag_popup_show(priv->tag_popup, text);
	}
	cat_unref_ptr(text);

}

static void l_marker_clicked(ChaEditor *editor, ChaLineLocationWo *location, long long y_marker_view) {
	l_open_tag_popup(editor, location, y_marker_view);
}

static gint l_timeout(gpointer data) {
	DraEditor *instance = DRA_EDITOR(data);
	DraEditorPrivate *priv = dra_editor_get_instance_private(instance);

	if (priv->tag_current_line_location && priv->tag_line_location && cha_line_location_wo_equal(priv->tag_current_line_location, priv->tag_line_location)) {
		l_open_tag_popup((ChaEditor *) instance, priv->tag_line_location, priv->tag_y_marker_view);
	}
	priv->tag_timeout_tag = 0;
	return G_SOURCE_REMOVE;
}

static void l_marker_over(ChaEditor *editor, ChaLineLocationWo *location, long long y_marker_view) {
	DraEditor *instance = DRA_EDITOR(editor);
	DraEditorPrivate *priv = dra_editor_get_instance_private(instance);
	if (priv->tag_current_line_location && cha_line_location_wo_equal(priv->tag_current_line_location, location)) {
		return;
	}
	cat_ref_swap(priv->tag_current_line_location, location);
	priv->tag_y_marker_view = y_marker_view;

	if (priv->tag_current_line_location!=priv->tag_line_location) {
		cat_unref_ptr(priv->tag_popup);
		if (priv->tag_timeout_tag) {
			g_source_remove(priv->tag_timeout_tag);
			priv->tag_timeout_tag = 0;
		}
		cat_ref_swap(priv->tag_line_location, location);
		priv->tag_timeout_tag = gdk_threads_add_timeout_seconds_full(0, 1, (GSourceFunc) l_timeout, g_object_ref(editor), g_object_unref);
	}


}

static void l_marker_out(ChaEditor *editor) {
	cat_log_debug("marker out");
	DraEditor *instance = DRA_EDITOR(editor);
	DraEditorPrivate *priv = dra_editor_get_instance_private(instance);

	cat_unref_ptr(priv->tag_popup);
	if (priv->tag_timeout_tag) {
		g_source_remove(priv->tag_timeout_tag);
		priv->tag_timeout_tag = 0;
	}
	cat_unref_ptr(priv->tag_current_line_location);
	cat_unref_ptr(priv->tag_line_location);
}


static gboolean l_tab(ChaEditor *editor, gboolean left_shift) {
	gboolean result = FALSE;
	ChaDocument *document = cha_editor_get_document(editor);
	ChaRevisionWo *a_revision = cha_document_get_current_revision_ref(document);
	if (cha_revision_wo_get_form(a_revision)!=NULL) {
		ChaUowNextFormField *uow = cha_uow_next_form_field_new();
		cha_editor_run(editor, (ChaUow *) uow);
		cat_unref_ptr(uow);
		result = TRUE;
	}
	cat_unref_ptr(a_revision);

	DraEditorPrivate *priv = dra_editor_get_instance_private(DRA_EDITOR(editor));
	if (!result && priv->context_editor) {
		result = dra_context_editor_indent(priv->context_editor, !left_shift);
		if (result) {
			DraUowIndent *uow = dra_uow_indent_new(!left_shift);
			cha_editor_notify_uow_ran(editor, (ChaUow *) uow);
			cat_unref_ptr(uow);
		}
	}

	cat_log_debug("result=%d", result);

	return result;
}

void dra_editor_show_auto_complete_popup(DraEditor *editor, DraAcContext *ac_context);
DraAcContext *dra_editor_create_auto_complete_context(DraEditor *editor);

static void l_selection_done(GtkMenu *menui, gpointer data) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(DRA_EDITOR(data));
	if (priv->context_menu) {
		GtkMenuShell *menu = lea_menu_action_get_menu_shell(priv->context_menu);
		gtk_widget_destroy((GtkWidget *) menu);
		cat_unref_ptr(priv->context_menu);
	}
}

static void l_show_context_menu(DraEditor *editor, ChaCursorWo *cursor, int xpos, int ypos, DraLineTagWo *spell_tag, GdkEvent *event) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);

	if (priv->context_menu) {
		GtkMenuShell *menu = lea_menu_action_get_menu_shell(priv->context_menu);
		gtk_widget_destroy((GtkWidget *) menu);
		cat_unref_ptr(priv->context_menu);
	}

	if (priv->context_menu == NULL) {

		DraEditorPanel *editor_panel = dra_editor_get_panel(editor);
		DraPanelOwner *panel_owner = (DraPanelOwner *) lea_panel_get_panel_owner((LeaPanel *) editor_panel);


		LeaActionGroup *main_group = lea_action_group_new(cat_string_wo_new_with("blah"), cat_string_wo_new_with("blah"));
		LeaActionGroup *jam_group = lea_action_group_new(cat_string_wo_new_with("jam"), cat_string_wo_new_with("jam"));

		lea_action_group_add(main_group, (LeaAction *) jam_group);

		LeaMenuAction *lma = lea_menu_action_new((LeaAction *) main_group, LEA_MENU_TYPE_SUB);

		dra_panel_owner_setup_context_menu(panel_owner, jam_group, editor, cursor);


	//	LeaActionGroup *st_group = lea_action_group_new(cat_string_wo_new_with("st"), cat_string_wo_new_with("st"));
	//	lea_action_group_add(jam_group, st_group);


		if (spell_tag) {
			DraConnectorMap *connector_map = dra_panel_owner_get_connector_map(panel_owner);
			DraSpellHelper *spell_helper = dra_connector_map_get_spell_helper(connector_map);
			CatStringWo *misspelled_word = (CatStringWo *) dra_line_tag_wo_get_extra_data(spell_tag);
			CatArrayWo *corrections = dra_spell_helper_enlist_corrections(spell_helper, misspelled_word);
			if (corrections) {
				CatIIterator *iter = cat_array_wo_iterator(corrections);
				while(cat_iiterator_has_next(iter)) {
					CatStringWo *sug = (CatStringWo *) cat_iiterator_next(iter);
//					cat_log_debug("adding action for sug : %O", sug);
					DraActionSpellSuggest *act = dra_action_spell_suggest_new(editor_panel, sug, spell_tag);
					lea_action_group_add(jam_group, (LeaAction *) act);
					cat_unref_ptr(act);
				}
				cat_unref_ptr(iter);
				cat_unref_ptr(corrections);
			}
		}


		lea_menu_action_update(lma, NULL);
		cat_unref_ptr(main_group);
		cat_unref_ptr(jam_group);

		priv->context_menu = lma;
	}

	GtkMenuShell *menu = lea_menu_action_get_menu_shell(priv->context_menu);

	g_signal_connect(menu, "selection-done", G_CALLBACK(l_selection_done), editor);


	gtk_widget_show_all((GtkWidget *) menu);
	gtk_menu_popup_at_pointer(GTK_MENU(menu), event);
}

static gboolean l_key_press_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data) {
	DraEditor *editor = (DraEditor *) data;
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	int key_val = eev->keyval;


	if (cha_editor_im_filter_keypress((ChaEditor *) editor, eev)) {
		cat_log_debug("was IM filtered");
		return TRUE;
	}

	if (key_val == GDK_KEY_Return || key_val == GDK_KEY_KP_Enter || key_val == GDK_KEY_ISO_Enter ||
			key_val == GDK_KEY_Escape) {
		cha_editor_im_reset_context((ChaEditor *) editor);
	}

	gboolean shift_pressed = eev->state & GDK_SHIFT_MASK;
	gboolean ctrl_pressed = eev->state & GDK_CONTROL_MASK;
	gboolean mod1_pressed = eev->state & GDK_MOD1_MASK;
	gboolean mod5_pressed = eev->state & GDK_MOD5_MASK;
	gboolean alt_pressed = (mod1_pressed || mod5_pressed) ? TRUE : FALSE;


	if (priv->auto_complete_popup) {
		switch(key_val) {
			case GDK_KEY_KP_Enter :
			case GDK_KEY_Return :
			case GDK_KEY_Escape :
			case GDK_KEY_Up :
			case GDK_KEY_Down :
			case GDK_KEY_Page_Up :
			case GDK_KEY_Page_Down :
			{
				dra_auto_complete_popup_forward_event(priv->auto_complete_popup, (GdkEvent *) eev);
				return TRUE;
			}
		}
	}

	cat_log_debug("key=%s, key_val=%d", eev->string, key_val);

	ChaUow *uow = NULL;

	switch (key_val) {
		case GDK_KEY_Insert : {
			ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) data);
			ChaEditMode mode = cha_document_view_get_edit_mode(document_view);
			switch(mode) {
				case CHA_EDIT_MODE_INSERT : {
					mode = CHA_EDIT_MODE_INSERT_CONTEXT;
				} break;
				case CHA_EDIT_MODE_INSERT_CONTEXT : {
					mode = CHA_EDIT_MODE_OVERWRITE;
				} break;
				case CHA_EDIT_MODE_OVERWRITE : {
					mode = CHA_EDIT_MODE_INSERT;
				} break;
			}
			cha_document_view_set_edit_mode(document_view, mode);
			cha_editor_inform_activity((ChaEditor *) editor);
		}
		break;
		case GDK_KEY_ISO_Left_Tab:
			if (l_tab(CHA_EDITOR(data), TRUE)) {
				return TRUE;
			}
			break;
		case GDK_KEY_Tab:
			if (l_tab(CHA_EDITOR(data), FALSE)) {
				return TRUE;
			}
			break;
		case GDK_KEY_Escape: {
			uow = (ChaUow *) cha_uow_form_clear_new();
		} break;
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
			uow = (ChaUow *) cha_uow_insert_template_new(NULL);
		} break;

	}

	if (uow==NULL && !ctrl_pressed && !mod1_pressed) {



		GdkDisplay *display = gdk_window_get_display(eev->window);
		GdkModifierType no_text_input_mask = gdk_keymap_get_modifier_mask(gdk_keymap_get_for_display(display),
	                                      GDK_MODIFIER_INTENT_NO_TEXT_INPUT);

		if (eev->type == GDK_KEY_PRESS && (eev->state & no_text_input_mask) == 0) {
			gunichar ch = gdk_keyval_to_unicode(eev->keyval);
			if (ch != 0 && (!g_unichar_iscntrl (ch) || ch=='\t' )) {
				gchar buf[10];
				int len = g_unichar_to_utf8 (ch, buf);
				buf[len] = '\0';

				cat_log_debug("ptxt = %d %d", buf[0], buf[1]);

				ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) data);
				ChaEditMode mode = cha_document_view_get_edit_mode(document_view);
				cat_log_debug("mode=%d", mode);
				if (mode == CHA_EDIT_MODE_OVERWRITE) {
					uow = (ChaUow *) cha_uow_replace_text_new(cat_string_wo_new_with(buf));
				} else {
					uow = (ChaUow *) cha_uow_insert_text_new(cat_string_wo_new_with(buf));
				}

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
				ChaSearchService *search_service = cha_editor_get_search_service(CHA_EDITOR(data));
				GtkWidget *owner_window = gtk_widget_get_parent((GtkWidget *) editor);
				while(owner_window) {
					if (GTK_IS_WINDOW(owner_window)) {
						break;
					}
					owner_window = gtk_widget_get_parent(owner_window);
				}
				cha_search_service_show_dialog(search_service, (GtkWindow *) owner_window, CHA_EDITOR(data));
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

			case GDK_KEY_space : {
				DraAcContext *ac_context = dra_editor_create_auto_complete_context(editor);
				if (ac_context) {
					dra_editor_show_auto_complete_popup(editor, ac_context);
					cat_unref_ptr(ac_context);
				}
			} break;
		}
	}

	cat_log_debug("uow=%o, %d", uow, (int) key_val);

	if (uow!=NULL) {
		cha_editor_im_reset_context((ChaEditor *) editor);
		cha_editor_run((ChaEditor *) data, uow);
		cat_unref(uow);
		return TRUE;
	}

	return FALSE;
}

static gboolean l_key_release_event(GtkWidget *gwidget, GdkEventKey *eev, gpointer data) {
	DraEditor *editor = (DraEditor *) data;
	if (cha_editor_im_filter_keypress((ChaEditor *) editor, eev)) {
		cat_log_debug("was IM filtered");
		return TRUE;
	}
	return FALSE;
}


static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data) {
	DraEditor *editor = DRA_EDITOR(data);
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	if (priv->auto_complete_popup) {
		GtkWindow *popup_window = dra_auto_complete_popup_get_window(priv->auto_complete_popup);
		priv->auto_complete_popup = NULL;
		gtk_widget_destroy(GTK_WIDGET(popup_window));
	}


	if (eev->button==3) {
		DraEditorClass *editor_class = DRA_EDITOR_GET_CLASS(editor);

		int xmouse = eev->x;
		int ymouse = eev->y;

		ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
		ChaDocument *document = cha_document_view_get_document(document_view);
		ChaCursorWo *cursor = cha_document_view_cursor_at_xy(document_view, xmouse, ymouse);
		DraLineTagWo *spell_tag = NULL;
		if (cursor) {
			ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);

			ChaLineWo *line_wo = cha_revision_wo_line_at_location(a_rev, cha_cursor_wo_get_line_location(cursor));
			DraLineInfoWo *line_info = (DraLineInfoWo *)
						cha_line_wo_get_slot_content_ref(line_wo, -1, (GObject *) priv->slot_key);
			if (line_info) {
				CatArrayWo *line_tags = dra_line_info_wo_get_line_tags(line_info);
				if (line_tags) {
					int xcursor = cha_cursor_wo_get_x_cursor_bytes(cursor);
					CatIIterator *iter = cat_array_wo_iterator(line_tags);
					while(cat_iiterator_has_next(iter)) {
						DraLineTagWo *tag = (DraLineTagWo *) cat_iiterator_next(iter);
						if (dra_line_tag_wo_get_tag_type(tag)==DRA_TAG_TYPE_SPELL_ERROR) {
							int start;
							int end;
							dra_line_tag_wo_get_start_and_end_index(tag, &start, &end);
							if (xcursor>=start && xcursor<end) {
								spell_tag = cat_ref_ptr(tag);
								break;
							}
						}
					}
					cat_unref_ptr(iter);
				}
				cat_unref_ptr(line_info);
			}
			cat_unref_ptr(line_wo);
			cat_unref_ptr(a_rev);
		}

		editor_class->showContextMenu(editor, cursor, (int) eev->x, (int) eev->y, spell_tag, (GdkEvent *) eev);
		cat_unref_ptr(spell_tag);
		cat_unref_ptr(cursor);
		return TRUE;
	}

	return FALSE;
}


static gboolean l_focus_out_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
	DraEditor *editor = DRA_EDITOR(data);
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	if (priv->tag_popup) {
		cat_unref_ptr(priv->tag_popup);
		cat_unref_ptr(priv->tag_current_line_location);
	}
	return FALSE;
}



struct _DraEditorPanel *dra_editor_get_panel(DraEditor *editor) {
	GtkWidget *w = (GtkWidget *) editor;
	while(w) {
		if (DRA_IS_EDITOR_PANEL(w)) {
			return (DraEditorPanel *) w;
		}
		w = gtk_widget_get_parent(w);
	}
	return NULL;
}

DraAcContext *dra_editor_create_auto_complete_context(DraEditor *editor) {
	DraAcContext *result = dra_ac_context_new((ChaEditor *) editor);
	DraEditorPanel *editor_panel = dra_editor_get_panel(editor);
	DraPanelOwner *panel_owner = (DraPanelOwner *) lea_panel_get_panel_owner((LeaPanel *) editor_panel);
	WorService *wor_service = dra_panel_owner_get_wor_service(panel_owner);

	DraContextEditor *context_editor = dra_editor_get_context_editor(editor);
	if (DRA_IS_IAC_CONTENT_PROVIDER(context_editor)) {
		DraAcContentRequest *content_request = dra_ac_content_request_new((DraIAcContentProvider *) context_editor, result);
		wor_service_post_request(wor_service, (WorRequest *) content_request);
	}

	ChaPreferencesWo *a_prefs= cha_editor_get_preferences((ChaEditor *) editor);
	CatHashMapWo *tmap = cha_preferences_wo_get_template_map(a_prefs);
	CatArrayWo *templates = cat_hash_map_wo_enlist_values(tmap, NULL);
	CatIIterator *iter = cat_array_wo_iterator(templates);
	while(cat_iiterator_has_next(iter)) {
		ChaPrefsTemplateWo *a_template = (ChaPrefsTemplateWo *) cat_iiterator_next(iter);
		CatStringWo *template = cha_prefs_template_wo_get_name(a_template);
		DraAcEntry *entry = (DraAcEntry *) dra_ac_template_entry_new(template, template);
		dra_ac_add_entry(result, entry);
		cat_unref_ptr(entry);
	}

	dra_ac_context_wait_for_providers_to_finish(result);
	return result;

}

static gboolean l_enable_kill(DraEditor *editor);

static void l_popup_destroyed(GtkWidget *widget, gpointer data) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(DRA_EDITOR(data));
	if (priv->auto_complete_popup) {
		priv->auto_complete_popup = NULL;
	}

}

void dra_editor_show_auto_complete_popup(DraEditor *editor, DraAcContext *ac_context) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	if (priv->auto_complete_popup) {
		/* content flip-ing can be done here */
		return;
	}
	priv->do_not_kill_assist = TRUE;

	cat_log_debug("show_auto_completion_popup");

	DraAutoCompletePanel *auto_complete_panel = dra_auto_complete_panel_new(editor, ac_context);
	DraAutoCompletePopup *completion_popup = dra_auto_complete_popup_new(editor, auto_complete_panel);
	priv->auto_complete_popup = completion_popup;

	GtkWindow *popup_window = dra_auto_complete_popup_get_window(completion_popup);
	g_signal_connect(popup_window, "destroy", (GCallback) l_popup_destroyed, editor);


	GtkWidget *editor_widget = GTK_WIDGET(editor);
	int editor_x, editor_y;
	GdkWindow *widget_window = gtk_widget_get_window(editor_widget);
	GdkDisplay *gdk_display = gdk_window_get_display(widget_window);
	GdkMonitor *monitor = gdk_display_get_primary_monitor(gdk_display);
	GdkRectangle geom;
	gdk_monitor_get_geometry(monitor, &geom);
	int screen_width = geom.width;
	int screen_height = geom.height;


	GtkWidget *top_widget = gtk_widget_get_toplevel(GTK_WIDGET(editor));
	gdk_window_get_origin(gtk_widget_get_window(top_widget), &editor_x, &editor_y);

	GtkAllocation alloc;
	GtkWidget *s_widget = gtk_widget_get_parent(GTK_WIDGET(editor));
	gtk_widget_get_allocation(s_widget, &alloc);
	editor_x += alloc.x;
	editor_y += alloc.y;


	cat_log_debug("editor[%d, %d]", editor_x, editor_y);

	ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);

	int line_height = 0;
	int cursor_x = 0;
	int cursor_y = 0;
	cha_document_view_get_cursor_screen_location(document_view, &cursor_x, &cursor_y, &line_height);

	cat_log_debug("cursor_y=%d, line_height=%d", cursor_y, line_height);

	alloc.width = 400;
	alloc.height = 280;

	alloc.y = editor_y+cursor_y+line_height;
	alloc.x = editor_x+cursor_x;


	if (alloc.y+alloc.height>screen_height) {
		/* the popup would be to low, lets try to put the popup above than */
		alloc.y = editor_y+cursor_y-alloc.height;
		if (alloc.y<0) {
			/* The popup on top would be to far up. Lets put it back below and reduce the height */
			alloc.y = editor_y+cursor_y+line_height;
			alloc.height = screen_height-alloc.y;
			if (alloc.height<line_height) {
				/* this is awful */
				cat_log_warn("the popup just does not fit");
				alloc.y = 0;
				alloc.height = screen_height;
			}
		}
	}
	if (alloc.x+alloc.width>screen_width) {
		alloc.x = screen_width-alloc.width;
	}
	if (alloc.x<0) {
		alloc.x = 0;
	}

	dra_auto_complete_popup_set_bounds(completion_popup, &alloc);
	cat_log_debug("alloc[%d, %d, %d, %d]", alloc.x, alloc.y, alloc.width, alloc.height);


	dra_auto_complete_popup_show(completion_popup);

#ifdef G_OS_WIN32
		while ((gtk_events_pending()) != 0) {
			gtk_main_iteration();
		}
#endif


	g_idle_add((GSourceFunc) l_enable_kill, editor);
//	priv->do_not_kill_assist = FALSE;
	cat_log_trace("end");

}

static gboolean l_enable_kill(DraEditor *editor) {
	DraEditorPrivate *priv = dra_editor_get_instance_private(editor);
	cat_log_debug("re-enabling kill");
	priv->do_not_kill_assist = FALSE;
//	ElkCompletionPopup *completion_popup = editor->completion_popup;
//	gtk_window_move (GTK_WINDOW (completion_popup->popup_window), completion_popup->xpos, completion_popup->ypos);
	return FALSE;
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
