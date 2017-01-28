/*
   File:    elkactionrevert.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jan 12, 2013
   Web:     http://www.natpad.net/
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

#include "elkactionrevert.h"
#include "../editor/elkeditorpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionRevert"
#include <logging/catlog.h>

struct _ElkActionRevertPrivate {
	DraEditorPanel *editor_panel;
};

G_DEFINE_TYPE (ElkActionRevert, elk_action_revert, LEA_TYPE_ACTION)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void elk_action_revert_class_init(ElkActionRevertClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkActionRevertPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_revert_init(ElkActionRevert *instance) {
	ElkActionRevertPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_ACTION_REVERT, ElkActionRevertPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionRevert *instance = ELK_ACTION_REVERT(object);
	ElkActionRevertPrivate *priv = instance->priv;
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionRevert *elk_action_revert_new() {
	ElkActionRevert *result = g_object_new(ELK_TYPE_ACTION_REVERT, NULL);
	cat_ref_anounce(result);
	ElkActionRevertPrivate *priv = result->priv;
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("elk.editor.revert"), cat_string_wo_new_with("Rever_t"), cat_string_wo_new_with("document-revert"));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}


static void l_action_run(LeaAction *self) {
	ElkActionRevertPrivate *priv = ELK_ACTION_REVERT_GET_PRIVATE(self);
	cat_log_debug("priv->editor=%p", priv->editor_panel);
	if (priv->editor_panel) {
		DRA_EDITOR_PANEL_GET_CLASS(priv->editor_panel)->revert(priv->editor_panel);
//		ELK_EDITOR_PANEL_CLASS(priv->editor)->revert(priv->editor);
	}
}

void elk_action_revert_set_editor_panel(ElkActionRevert *action, DraEditorPanel *editor_panel) {
	ElkActionRevertPrivate *priv = ELK_ACTION_REVERT_GET_PRIVATE(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	gboolean can_revert = FALSE;
	if (editor_panel) {
		ElkDocumentBin *document_bin = elk_editor_panel_get_document_bin((ElkEditorPanel *) editor_panel);
		can_revert = elk_document_bin_has_file(document_bin);

//		if (MON_IS_IFILE(editor->file_ref)) {	// TODO new files don't have a file_ref, but once they get saved they will receive one but the sensitivity wont be updated.
//			can_revert = TRUE;
//		}
	}
	lea_action_set_sensitive_self((LeaAction *) action, can_revert);
}
