/*
   File:    draactionopendeclaration.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 8, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "draactionopendeclaration.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "DraActionOpenDeclaration"
#include <logging/catlog.h>

struct _DraActionOpenDeclarationPrivate {
	DraEditorPanel *editor_panel;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionOpenDeclaration, dra_action_open_declaration, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionOpenDeclaration)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_open_declaration_class_init(DraActionOpenDeclarationClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_open_declaration_init(DraActionOpenDeclaration *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionOpenDeclaration *instance = DRA_ACTION_OPEN_DECLARATION(object);
	DraActionOpenDeclarationPrivate *priv = dra_action_open_declaration_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_open_declaration_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_open_declaration_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionOpenDeclaration *dra_action_open_declaration_new() {
	DraActionOpenDeclaration *result = g_object_new(DRA_TYPE_ACTION_OPEN_DECLARATION, NULL);
	cat_ref_anounce(result);
	DraActionOpenDeclarationPrivate *priv = dra_action_open_declaration_get_instance_private(result);
	priv->editor_panel = NULL;
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("dragonfly.editor.open.declaration"), cat_string_wo_new_with("_Open Declaration"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("F3")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}

static void l_action_run(LeaAction *self) {
	DraActionOpenDeclarationPrivate *priv = dra_action_open_declaration_get_instance_private(DRA_ACTION_OPEN_DECLARATION(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		DraContextEditor *context_editor = dra_editor_get_context_editor(editor);
		DraContextEditorClass *ctx_ed_class = DRA_CONTEXT_EDITOR_GET_CLASS(context_editor);
		ctx_ed_class->openDeclaration(context_editor);
	}
}

static void l_update(DraActionOpenDeclaration *action) {
	DraActionOpenDeclarationPrivate *priv = dra_action_open_declaration_get_instance_private(action);
	cat_log_debug("updating, editor=%p", priv->editor_panel);
	gboolean is_sens = FALSE;
	if (priv->editor_panel!=NULL) {
		DraContextEditor *context_editor = dra_editor_get_context_editor(dra_editor_panel_get_editor(priv->editor_panel));
		DraContextEditorClass *ctx_ed_class = DRA_CONTEXT_EDITOR_GET_CLASS(context_editor);
		is_sens = ctx_ed_class->openDeclaration!=NULL;

	}
	lea_action_set_sensitive_self(LEA_ACTION(action), is_sens);
}
void dra_action_open_declaration_set_editor_panel(DraActionOpenDeclaration *action, DraEditorPanel *editor_panel) {
	DraActionOpenDeclarationPrivate *priv = dra_action_open_declaration_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update(action);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionOpenDeclaration *instance = DRA_ACTION_OPEN_DECLARATION(self);
	DraActionOpenDeclarationPrivate *priv = dra_action_open_declaration_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

