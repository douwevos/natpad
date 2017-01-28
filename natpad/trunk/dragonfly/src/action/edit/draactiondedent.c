/*
   File:    draactiondedent.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 13, 2015
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

#include "draactiondedent.h"
#include "../../context/dracontexteditor.h"
#include "../../uow/drauowindent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionDedent"
#include <logging/catlog.h>

struct _DraActionDedentPrivate {
	DraEditorPanel *editor_panel;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionDedent, dra_action_dedent, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionDedent)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_dedent_class_init(DraActionDedentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_dedent_init(DraActionDedent *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionDedent *instance = DRA_ACTION_DEDENT(object);
	DraActionDedentPrivate *priv = dra_action_dedent_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_dedent_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_dedent_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionDedent *dra_action_dedent_new() {
	DraActionDedent *result = g_object_new(DRA_TYPE_ACTION_DEDENT, NULL);
	cat_ref_anounce(result);
	DraActionDedentPrivate *priv = dra_action_dedent_get_instance_private(result);
	priv->editor_panel = NULL;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.dedent"), cat_string_wo_new_with("Remove Indent"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Shift+LeftTab")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}

static void l_update_state(DraActionDedent *action) {
	DraActionDedentPrivate *priv = dra_action_dedent_get_instance_private(action);
	lea_action_set_sensitive_self(LEA_ACTION(action), priv->editor_panel!=NULL);
}

void dra_action_dedent_set_editor_panel(DraActionDedent *action, DraEditorPanel *editor_panel) {
	DraActionDedentPrivate *priv = dra_action_dedent_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update_state(action);
}

static void l_action_run(LeaAction *self) {
	DraActionDedentPrivate *priv = dra_action_dedent_get_instance_private(DRA_ACTION_DEDENT(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		DraUowIndent *uow_indent = dra_uow_indent_new(FALSE);
		cha_editor_run((ChaEditor*) editor, (ChaUow *) uow_indent);
		cat_unref_ptr(uow_indent);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionDedent *instance = DRA_ACTION_DEDENT(self);
	DraActionDedentPrivate *priv = dra_action_dedent_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

