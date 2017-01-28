/*
   File:    draactionmarkclearall.c
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

#include "draactionmarkclearall.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionMarkClearAll"
#include <logging/catlog.h>

struct _DraActionMarkClearAllPrivate {
	DraEditorPanel *editor_panel;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionMarkClearAll, dra_action_mark_clear_all, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionMarkClearAll)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_mark_clear_all_class_init(DraActionMarkClearAllClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_mark_clear_all_init(DraActionMarkClearAll *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionMarkClearAll *instance = DRA_ACTION_MARK_CLEAR_ALL(object);
	DraActionMarkClearAllPrivate *priv = dra_action_mark_clear_all_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_mark_clear_all_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_mark_clear_all_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionMarkClearAll *dra_action_mark_clear_all_new() {
	DraActionMarkClearAll *result = g_object_new(DRA_TYPE_ACTION_MARK_CLEAR_ALL, NULL);
	cat_ref_anounce(result);
	DraActionMarkClearAllPrivate *priv = dra_action_mark_clear_all_get_instance_private(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.editor.marker.clear.all"), cat_string_wo_new_with("_Clear all marked"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+Shift+F2")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}

static void l_update(DraActionMarkClearAll *action) {
	DraActionMarkClearAllPrivate *priv = dra_action_mark_clear_all_get_instance_private(action);
	lea_action_set_sensitive_self(LEA_ACTION(action), priv->editor_panel!=NULL);
}


void dra_action_mark_clear_all_set_editor(DraActionMarkClearAll *action, DraEditorPanel *editor_panel) {
	DraActionMarkClearAllPrivate *priv = dra_action_mark_clear_all_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update(action);
}

static void l_action_run(LeaAction *self) {
	DraActionMarkClearAllPrivate *priv = dra_action_mark_clear_all_get_instance_private((DraActionMarkClearAll *) self);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaUowLineMarker *uow = cha_uow_line_marker_new(CHA_UOW_MARK_UNSET_ALL);
		cha_editor_run((ChaEditor *) editor, (ChaUow *) uow);
		cat_unref_ptr(uow);
	}
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionMarkClearAll *instance = DRA_ACTION_MARK_CLEAR_ALL(self);
	DraActionMarkClearAllPrivate *priv = dra_action_mark_clear_all_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
