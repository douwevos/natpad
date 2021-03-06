/*
   File:    draactioncut.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 21, 2015
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

#include "draactioncut.h"

#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionCut"
#include <logging/catlog.h>

struct _DraActionCutPrivate {
	DraEditorPanel *editor_panel;
	gboolean has_selection;
	gpointer clipboard_handler;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionCut, dra_action_cut, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionCut)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_cut_class_init(DraActionCutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_cut_init(DraActionCut *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionCut *instance = DRA_ACTION_CUT(object);
	DraActionCutPrivate *priv = dra_action_cut_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_cut_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_cut_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionCut *dra_action_cut_new(gpointer clipboard_handler) {
	DraActionCut *result = g_object_new(DRA_TYPE_ACTION_CUT, NULL);
	cat_ref_anounce(result);
	DraActionCutPrivate *priv = dra_action_cut_get_instance_private(result);
	priv->editor_panel = NULL;
	priv->has_selection = FALSE;
	priv->clipboard_handler = clipboard_handler;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.clipboard.cut"), cat_string_wo_new_with("Cu_t"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+X")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}

static void l_update_state(DraActionCut *action) {
	DraActionCutPrivate *priv = dra_action_cut_get_instance_private(action);
	cat_log_debug("updating, editor_panel=%p has_selection=%d", priv->editor_panel, priv->has_selection);
	lea_action_set_sensitive_self(LEA_ACTION(action), (priv->editor_panel!=NULL) && priv->has_selection);
}

void dra_action_cut_set_editor_panel(DraActionCut *action, DraEditorPanel *editor_panel) {
	DraActionCutPrivate *priv = dra_action_cut_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update_state(action);
}

void dra_action_cut_set_has_selection(DraActionCut *action, gboolean has_selection) {
	DraActionCutPrivate *priv = dra_action_cut_get_instance_private(action);
	cat_log_debug("has_selection=%d", has_selection);
	if (priv->has_selection != has_selection) {
		priv->has_selection = has_selection;
		l_update_state(action);
	}
}

static void l_action_run(LeaAction *self) {
	DraActionCutPrivate *priv = dra_action_cut_get_instance_private(DRA_ACTION_CUT(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaUowClipboard *uow = cha_uow_clipboard_new(CHA_UOW_CLIPBOARD_CUT, FALSE);
		cha_editor_run((ChaEditor *) editor, (ChaUow *) uow);
		cat_unref_ptr(uow);
		dra_clipboard_refresh_has_text(priv->clipboard_handler);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionCut *instance = DRA_ACTION_CUT(self);
	DraActionCutPrivate *priv = dra_action_cut_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p has_selection=%d]", iname, self, priv->has_selection);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
