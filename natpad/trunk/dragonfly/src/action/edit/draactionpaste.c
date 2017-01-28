/*
   File:    draactionpaste.c
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

#include "draactionpaste.h"

#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionPaste"
#include <logging/catlog.h>

struct _DraActionPastePrivate {
	DraEditorPanel *editor_panel;
	gboolean clipboard_has_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionPaste, dra_action_paste, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionPaste)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_paste_class_init(DraActionPasteClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_paste_init(DraActionPaste *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionPaste *instance = DRA_ACTION_PASTE(object);
	DraActionPastePrivate *priv = dra_action_paste_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_paste_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_paste_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionPaste *dra_action_paste_new() {
	DraActionPaste *result = g_object_new(DRA_TYPE_ACTION_PASTE, NULL);
	cat_ref_anounce(result);
	DraActionPastePrivate *priv = dra_action_paste_get_instance_private(result);
	priv->editor_panel = NULL;
	priv->clipboard_has_text = FALSE;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.clipboard.paste"), cat_string_wo_new_with("_Paste"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+V")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}


static void l_update_state(DraActionPaste *action) {
	DraActionPastePrivate *priv = dra_action_paste_get_instance_private(DRA_ACTION_PASTE(action));
	cat_log_debug("updating, editor=%p has_selection=%d", priv->editor_panel, priv->clipboard_has_text);
	lea_action_set_sensitive_self(LEA_ACTION(action), (priv->editor_panel!=NULL) && priv->clipboard_has_text);
}

void dra_action_paste_set_editor_panel(DraActionPaste *action, DraEditorPanel *editor_panel) {
	DraActionPastePrivate *priv = dra_action_paste_get_instance_private(DRA_ACTION_PASTE(action));
	cat_ref_swap(priv->editor_panel, editor_panel);
	l_update_state(action);
}

void dra_action_paste_set_has_clipboard(DraActionPaste *action, gboolean clipboard_has_text) {
	DraActionPastePrivate *priv = dra_action_paste_get_instance_private(DRA_ACTION_PASTE(action));
	cat_log_debug("clipboard_has_text=%d", clipboard_has_text);
	if (priv->clipboard_has_text!=clipboard_has_text) {
		priv->clipboard_has_text = clipboard_has_text;
		l_update_state(action);
	}
}

static void l_action_run(LeaAction *self) {
	DraActionPastePrivate *priv = dra_action_paste_get_instance_private(DRA_ACTION_PASTE(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaUowClipboard *uow = cha_uow_clipboard_new(CHA_UOW_CLIPBOARD_PASTE, FALSE);
		cha_editor_run((ChaEditor *) editor, (ChaUow *) uow);
		cat_unref_ptr(uow);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionPaste *instance = DRA_ACTION_PASTE(self);
	DraActionPastePrivate *priv = dra_action_paste_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor=%o]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
