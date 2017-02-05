/*
   File:    draactionfindprevious.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 16, 2015
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

#include "draactionfindprevious.h"
#include "../../draeditor.h"
#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionFindPrevious"
#include <logging/catlog.h>

struct _DraActionFindPreviousPrivate {
	DraEditorPanel *editor_panel;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionFindPrevious, dra_action_find_previous, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionFindPrevious)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *action);

static void dra_action_find_previous_class_init(DraActionFindPreviousClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_find_previous_init(DraActionFindPrevious *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionFindPrevious *instance = DRA_ACTION_FIND_PREVIOUS(object);
	DraActionFindPreviousPrivate *priv = dra_action_find_previous_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_find_previous_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_find_previous_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionFindPrevious *dra_action_find_previous_new() {
	DraActionFindPrevious *result = g_object_new(DRA_TYPE_ACTION_FIND_PREVIOUS, NULL);
	cat_ref_anounce(result);
	DraActionFindPreviousPrivate *priv = dra_action_find_previous_get_instance_private(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.editor.find.previous"), cat_string_wo_new_with("Find Previous"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+Shift+K")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}


static void l_update_state(DraActionFindPrevious *action) {
	DraActionFindPreviousPrivate *priv = dra_action_find_previous_get_instance_private(action);
	lea_action_set_sensitive_self(LEA_ACTION(action), priv->editor_panel!=NULL);
}

void dra_action_find_previous_set_editor_panel(DraActionFindPrevious *action, DraEditorPanel *editor_panel) {
	DraActionFindPreviousPrivate *priv = dra_action_find_previous_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update_state(action);
}


static void l_action_run(LeaAction *action) {
	DraActionFindPreviousPrivate *priv = dra_action_find_previous_get_instance_private((DraActionFindPrevious *) action);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaSearchService *search_service = cha_editor_get_search_service((ChaEditor *) editor);
		ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
		ChaSelection *selection = cha_document_view_get_selection(document_view);

		ChaSearchQueryWo *query = cha_search_service_get_query(search_service);
		if (query) {
			query = cha_search_query_wo_clone(query, CAT_CLONE_DEPTH_MAIN);
		}

		if (selection) {
			ChaSelectionClass *selection_class = CHA_SELECTION_GET_CLASS(selection);
			CatStringWo *sel_text = selection_class->getAsText(selection, document_view, FALSE);
			if (query==NULL) {
				query = cha_search_query_wo_new();
			}
			sel_text = cat_string_wo_anchor(sel_text, 0);
			cha_search_query_wo_set_text(query, sel_text);
			cat_unref_ptr(sel_text);

		}

		if (query) {
			cha_search_service_set_editor(search_service, (ChaEditor *) editor);
			cha_search_service_set_query(search_service, query);
			cha_search_service_find(search_service,TRUE, TRUE);
		}
	}
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionFindPrevious *instance = DRA_ACTION_FIND_PREVIOUS(self);
	DraActionFindPreviousPrivate *priv = dra_action_find_previous_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
