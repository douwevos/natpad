/*
   File:    elkactionsave.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 12, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "elkactionsave.h"
#include <caterpillar.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionSave"
#include <logging/catlog.h>

struct _ElkActionSavePrivate {
	DraEditorPanel *editor_panel;
};

G_DEFINE_TYPE_WITH_CODE(ElkActionSave, elk_action_save, LEA_TYPE_ACTION, {
		G_ADD_PRIVATE(ElkActionSave)
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void elk_action_save_class_init(ElkActionSaveClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_save_init(ElkActionSave *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionSave *instance = ELK_ACTION_SAVE(object);
	ElkActionSavePrivate *priv = elk_action_save_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(elk_action_save_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_action_save_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionSave *elk_action_save_new() {
	ElkActionSave *result = g_object_new(ELK_TYPE_ACTION_SAVE, NULL);
	cat_ref_anounce(result);
	ElkActionSavePrivate *priv = elk_action_save_get_instance_private(result);

	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("elk.editor.save"), cat_string_wo_new_with("_Save"), cat_string_wo_new_with("document-save"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+S")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}


static void l_action_run(LeaAction *self) {
	ElkActionSave *action = ELK_ACTION_SAVE(self);
	ElkActionSavePrivate *priv = elk_action_save_get_instance_private(action);
	cat_log_debug("priv->editor=%p", priv->editor_panel);
	if (priv->editor_panel) {
		DRA_EDITOR_PANEL_GET_CLASS(priv->editor_panel)->save(priv->editor_panel, FALSE, NULL);
	}
}

void elk_action_save_set_editor_panel(ElkActionSave *action, DraEditorPanel *editor_panel) {
	ElkActionSavePrivate *priv = elk_action_save_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	lea_action_set_sensitive_self((LeaAction *) action, editor_panel!=NULL);
}
