/*
   File:    elkactionclose.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 13, 2012
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

#include "elkactionclose.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionClose"
#include <logging/catlog.h>

struct _ElkActionClosePrivate {
	DraEditorPanel *editor_panel;
};

G_DEFINE_TYPE_WITH_PRIVATE(ElkActionClose, elk_action_close, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *action);

static void elk_action_close_class_init(ElkActionCloseClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_close_init(ElkActionClose *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionClose *instance = ELK_ACTION_CLOSE(object);
	ElkActionClosePrivate *priv = elk_action_close_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(elk_action_close_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_action_close_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionClose *elk_action_close_new() {
	ElkActionClose *result = g_object_new(ELK_TYPE_ACTION_CLOSE, NULL);
	cat_ref_anounce(result);
	ElkActionClosePrivate *priv = elk_action_close_get_instance_private(result);
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("elk.close"), cat_string_wo_new_with("_Close"), cat_string_wo_new_with("window-close"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+W")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}

static void l_action_run(LeaAction *action) {
	ElkActionClosePrivate *priv = elk_action_close_get_instance_private((ElkActionClose *) action);
	if (priv->editor_panel) {
		lea_panel_close(LEA_PANEL(priv->editor_panel));
	}
}

void elk_action_close_set_editor_panel(ElkActionClose *action, DraEditorPanel *editor_panel) {
	ElkActionClosePrivate *priv = elk_action_close_get_instance_private((ElkActionClose *) action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	lea_action_set_sensitive_self((LeaAction *) action, editor_panel!=NULL);
}
