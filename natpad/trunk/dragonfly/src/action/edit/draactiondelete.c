/*
   File:    draactiondelete.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Aug 30, 2015
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

#include "draactiondelete.h"

#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionDelete"
#include <logging/catlog.h>

struct _DraActionDeletePrivate {
	DraEditorPanel *editor_panel;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionDelete, dra_action_delete, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionDelete)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_delete_class_init(DraActionDeleteClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_delete_init(DraActionDelete *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionDelete *instance = LEA_ACTION_DELETE(object);
	DraActionDeletePrivate *priv = dra_action_delete_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_delete_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_delete_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionDelete *dra_action_delete_new(LeaKeyContext *key_context) {
	DraActionDelete *result = g_object_new(LEA_TYPE_ACTION_DELETE, NULL);
	cat_ref_anounce(result);
	DraActionDeletePrivate *priv = dra_action_delete_get_instance_private(result);
	priv->editor_panel = NULL;
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("dragonfly.editor.delete"), cat_string_wo_new_with("_Delete"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_new(lea_key_new(GDK_KEY_Delete, 0)));
	lea_action_set_key_context(LEA_ACTION(result), key_context);
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}


static void l_action_run(LeaAction *self) {
	DraActionDeletePrivate *priv = dra_action_delete_get_instance_private(LEA_ACTION_DELETE(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		gboolean ctrlPressed = FALSE;
		ChaUow *uow = (ChaUow *) cha_uow_remove_text_new(TRUE, ctrlPressed);
		cha_editor_run((ChaEditor *) editor, (ChaUow *) uow);
		cat_unref_ptr(uow);
	}
}


static void l_update(DraActionDelete *action) {
	DraActionDeletePrivate *priv = dra_action_delete_get_instance_private(action);
	cat_log_debug("updating, editor=%p", priv->editor_panel);
	lea_action_set_sensitive_self(LEA_ACTION(action), (priv->editor_panel!=NULL));
}

void dra_action_delete_set_editor_panel(DraActionDelete *action, DraEditorPanel *editor_panel) {
	DraActionDeletePrivate *priv = dra_action_delete_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update(action);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionDelete *instance = LEA_ACTION_DELETE(self);
	DraActionDeletePrivate *priv = dra_action_delete_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
