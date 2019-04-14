/*
   File:    dragroupmain.c
   Project: dragonfly
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

#include "draactionclipboard.h"
#include "dragroupmain.h"
#include "dragroupedit.h"
#include "dragroupnavigate.h"
#include "dragroupsearch.h"
#include "dragroupmacro.h"
#include "draactionprint.h"
#include "draactionprintpreview.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraGroupMain"
#include <logging/catlog.h>

struct _DraGroupMainPrivate {
	DraEditorPanel *focused_editor_panel;
	DraGroupEdit *group_edit;
	DraGroupNavigate *group_navigate;
	DraGroupSearch *group_search;
	DraGroupMacro *group_macro;
	gboolean clipboard_has_text;
	ChaLineEnd line_ends;
	gboolean line_ends_are_mixed;

	DraActionPrint *action_print;
	DraActionPrintPreview *action_print_preview;
};

static void l_document_view_listener_iface_init(ChaIDocumentViewListenerInterface *iface);
static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraGroupMain, dra_group_main, LEA_TYPE_ACTION_GROUP,
		G_ADD_PRIVATE(DraGroupMain)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_VIEW_LISTENER, l_document_view_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, l_document_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_group_main_class_init(DraGroupMainClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_group_main_init(DraGroupMain *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraGroupMain *instance = DRA_GROUP_MAIN(object);
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(instance);
	if (priv->focused_editor_panel) {
		dra_group_main_set_editor_panel(instance, NULL);
	}
	cat_unref_ptr(priv->group_edit);
	cat_unref_ptr(priv->group_navigate);
	cat_unref_ptr(priv->group_search);
	cat_unref_ptr(priv->group_macro);
	cat_unref_ptr(priv->action_print);
	cat_unref_ptr(priv->action_print_preview);
	cat_unref_ptr(priv->focused_editor_panel);
	G_OBJECT_CLASS(dra_group_main_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_group_main_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraGroupMain *dra_group_main_new(LeaKeyContext *key_context, DraPanelOwner *panel_owner) {
	DraGroupMain *result = g_object_new(DRA_TYPE_GROUP_MAIN, NULL);
	cat_ref_anounce(result);
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(result);
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("dra.group.main"), cat_string_wo_new_with("DragonFly Main Group"));
	priv->focused_editor_panel = NULL;

	LeaActionGroup *grp_file = lea_action_group_create_sub((LeaActionGroup *) result, cat_ref_ptr(lea_prov_file_group_name()), cat_string_wo_new_with("_File"));

	LeaActionGroup *grp_print = lea_action_group_create_sub((LeaActionGroup *) grp_file, cat_string_wo_new_with("draongfly.print.group"), NULL);
	lea_action_set_order((LeaAction *) grp_print, 100);

	priv->action_print = dra_action_print_new();
	lea_action_set_order((LeaAction *) priv->action_print, 100);
	lea_action_group_add(grp_print, (LeaAction *) priv->action_print);

	priv->action_print_preview = dra_action_print_preview_new();
	lea_action_set_order((LeaAction *) priv->action_print_preview, 200);
	lea_action_group_add(grp_print, (LeaAction *) priv->action_print_preview);


	cat_unref(grp_print);
	cat_unref(grp_file);


	priv->group_edit = dra_group_edit_new(key_context, result);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_edit);

	priv->group_navigate = dra_group_navigate_new(key_context, panel_owner);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_navigate);

	priv->group_search = dra_group_search_new(key_context, result);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_search);

	priv->group_macro = dra_group_macro_new(key_context, panel_owner);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_macro);


	return result;
}


void dra_group_main_context_menu(DraGroupMain *group_main, LeaActionGroup *ctx_menu_grp) {
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(group_main);
	lea_action_group_add(ctx_menu_grp, (LeaAction *) priv->group_edit);
}


static void l_set_has_selection(DraGroupMain *group_main, gboolean has_selection) {
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(group_main);
	dra_group_edit_set_has_selection(priv->group_edit, has_selection);
}

void dra_group_main_set_editor_panel(DraGroupMain *group_main, DraEditorPanel *editor_panel) {
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(group_main);
	if (priv->focused_editor_panel == editor_panel) {
		return;
	}
	if (priv->focused_editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->focused_editor_panel);
		ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
		cha_document_view_remove_listener(document_view, (ChaIDocumentViewListener *) group_main);
		ChaDocument *document = cha_document_view_get_document(document_view);
		cha_document_remove_listener(document, (ChaIDocumentListener *) group_main);

//		ast_view_remove_view_listener(priv->focused_editor_panel->ast_view, AST_IVIEW_LISTENER(group_main));
//		AstDocument *document = priv->focused_editor_panel->ast_document;
//		if (document) {
//			ast_document_remove_listener(document, AST_IDOCUMENT_LISTENER(group_main));
//		}
	}
	int history_size = 0;
	int history_index = 0;
	gboolean has_selection = FALSE;
//	gboolean has_marked_lines = FALSE;
	ChaLineEnd line_ends = CHA_LINE_END_LF;
	gboolean line_ends_are_mixed = TRUE;
	if (editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(editor_panel);
		ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
		cha_document_view_add_listener(document_view, (ChaIDocumentViewListener *) group_main);
		ChaDocument *document = cha_document_view_get_document(document_view);
		cha_document_add_listener(document, (ChaIDocumentListener *) group_main);
		cha_document_get_history_info(document, &history_index, &history_size);
		has_selection = cha_document_view_get_selection(document_view)!=NULL;

		ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);
		line_ends = cha_revision_wo_get_line_ends(a_rev);
		line_ends_are_mixed = cha_revision_wo_get_line_ends_are_mixed(a_rev);
		priv->line_ends = line_ends;
		priv->line_ends_are_mixed = line_ends_are_mixed;
		cat_unref_ptr(a_rev);

//		AstDocument *document = editor_panel->ast_document;
//		if (document) {
//			ast_document_add_listener(document, AST_IDOCUMENT_LISTENER(group_main));
//			ast_document_history_info(document, &size, &index, NULL, NULL);
//			has_marked_lines = ast_document_has_marked_lines(document);
//		}
	}
	cat_ref_swap(priv->focused_editor_panel, editor_panel);

	dra_action_print_set_editor_panel(priv->action_print, editor_panel);
	dra_action_print_preview_set_editor_panel(priv->action_print_preview, editor_panel);

	dra_group_edit_set_editor_panel(priv->group_edit, editor_panel);
	dra_group_edit_set_history_info(priv->group_edit, history_index, history_size);
	dra_group_edit_set_line_ends(priv->group_edit, line_ends, line_ends_are_mixed);

	dra_group_search_set_editor_panel(priv->group_search, editor_panel);
	dra_group_macro_set_editor_panel(priv->group_macro, editor_panel);

	l_set_has_selection(group_main, has_selection);
}


static void l_has_clipboard_cb(GtkClipboard *clipboard, const gchar *text, gpointer data) {
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(DRA_GROUP_MAIN(data));
	gboolean has_clipboard = text!=NULL;
	if (priv->clipboard_has_text!=has_clipboard) {
		priv->clipboard_has_text = has_clipboard;
		dra_group_edit_set_has_clipboard(priv->group_edit, has_clipboard);
	}
}

void dra_clipboard_refresh_has_text(gpointer clipboard_handler) {
	cat_log_debug("refresh of clipboard text detection");
	gtk_clipboard_request_text(gtk_clipboard_get(GDK_NONE), l_has_clipboard_cb, clipboard_handler);
}


void dra_group_main_refresh_clipboard(DraGroupMain *group_main) {
	dra_clipboard_refresh_has_text(group_main);
}

/********************* start ChaIDocumentViewListener implementation *********************/

static void l_selection_changed(ChaIDocumentViewListener *self, ChaSelection *selection) {
	l_set_has_selection(DRA_GROUP_MAIN(self), selection!=NULL);
}


static void l_document_view_listener_iface_init(ChaIDocumentViewListenerInterface *iface) {
	iface->selectionChanged = l_selection_changed;
}

/********************* end ChaIDocumentViewListener implementation *********************/

/********************* start ChaIDocumentListener implementation *********************/

static void l_on_history_modified(ChaIDocumentListener *self, int history_index, int history_length) {
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(DRA_GROUP_MAIN(self));
	dra_group_edit_set_history_info(priv->group_edit, history_index, history_length);
}

void l_on_new_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(DRA_GROUP_MAIN(self));

	ChaLineEnd line_ends = cha_revision_wo_get_line_ends(a_new_revision);
	gboolean line_ends_are_mixed = cha_revision_wo_get_line_ends_are_mixed(a_new_revision);
	if (priv->line_ends==line_ends && priv->line_ends_are_mixed==line_ends_are_mixed) {
		return;
	}

	priv->line_ends = line_ends;
	priv->line_ends_are_mixed = line_ends_are_mixed;
	dra_group_edit_set_line_ends(priv->group_edit, line_ends, line_ends_are_mixed);
}


static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	iface->onHistoryModified = l_on_history_modified;
	iface->onNewRevision = l_on_new_revision;
}

/********************* end ChaIDocumentListener implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraGroupMain *instance = DRA_GROUP_MAIN(self);
	DraGroupMainPrivate *priv = dra_group_main_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p focused_editor_panel=%o]", iname, self, priv->focused_editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

