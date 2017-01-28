/*
   File:    draactionmarkcopy.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Sep 2, 2015
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

#include "draactionmarkcopy.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionMarkCopy"
#include <logging/catlog.h>

struct _DraActionMarkCopyPrivate {
	DraEditorPanel *editor_panel;
	gpointer clipboard_handler;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionMarkCopy, dra_action_mark_copy, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionMarkCopy)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_mark_copy_class_init(DraActionMarkCopyClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_mark_copy_init(DraActionMarkCopy *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionMarkCopy *instance = DRA_ACTION_MARK_COPY(object);
	DraActionMarkCopyPrivate *priv = dra_action_mark_copy_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_mark_copy_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_mark_copy_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionMarkCopy *dra_action_mark_copy_new(gpointer clipboard_handler) {
	DraActionMarkCopy *result = g_object_new(DRA_TYPE_ACTION_MARK_COPY, NULL);
	cat_ref_anounce(result);
	DraActionMarkCopyPrivate *priv = dra_action_mark_copy_get_instance_private(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.editor.mark.copy"), cat_string_wo_new_with("_Copy marked lines"), NULL);
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	priv->clipboard_handler = clipboard_handler;
	return result;
}

static void l_update(DraActionMarkCopy *action) {
	DraActionMarkCopyPrivate *priv = dra_action_mark_copy_get_instance_private(action);
	lea_action_set_sensitive_self(LEA_ACTION(action), priv->editor_panel!=NULL);
}

void dra_action_mark_copy_set_editor(DraActionMarkCopy *action, DraEditorPanel *editor_panel) {
	DraActionMarkCopyPrivate *priv = dra_action_mark_copy_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update(action);
}

static void l_action_run(LeaAction *self) {
	DraActionMarkCopyPrivate *priv = dra_action_mark_copy_get_instance_private((DraActionMarkCopy *) self);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaUowLineMarker *uow = cha_uow_line_marker_new(CHA_UOW_MARK_COPY);
		cha_editor_run((ChaEditor *) editor, (ChaUow *) uow);
		cat_unref_ptr(uow);
		dra_clipboard_refresh_has_text(priv->clipboard_handler);
	}
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionMarkCopy *instance = DRA_ACTION_MARK_COPY(self);
	DraActionMarkCopyPrivate *priv = dra_action_mark_copy_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
