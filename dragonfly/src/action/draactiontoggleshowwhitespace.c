/*
   File:    draactiontoggleshowwhitespace.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 26, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#include "draactiontoggleshowwhitespace.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionToggleShowWhitespace"
#include <logging/catlog.h>

struct _DraActionToggleShowWhitespacePrivate {
	DraEditorPanel *editor_panel;
};

G_DEFINE_TYPE_WITH_CODE(DraActionToggleShowWhitespace, dra_action_toggle_show_whitespace, LEA_TYPE_ACTION,  // @suppress("Unused static function")
	G_ADD_PRIVATE(DraActionToggleShowWhitespace)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_toggle_show_whitespace_class_init(DraActionToggleShowWhitespaceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_toggle_show_whitespace_init(DraActionToggleShowWhitespace *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionToggleShowWhitespace *instance = DRA_ACTION_TOGGLE_SHOW_WHITESPACE(object);
	DraActionToggleShowWhitespacePrivate *priv = dra_action_toggle_show_whitespace_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_toggle_show_whitespace_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_toggle_show_whitespace_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionToggleShowWhitespace *dra_action_toggle_show_whitespace_new() {
	DraActionToggleShowWhitespace *result = g_object_new(DRA_TYPE_ACTION_TOGGLE_SHOW_WHITESPACE, NULL);
	cat_ref_anounce(result);
	DraActionToggleShowWhitespacePrivate *priv = dra_action_toggle_show_whitespace_get_instance_private(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.toggle.show.whitesoace"), cat_string_wo_new_with("_Show Whitespace"), cat_string_wo_new_with("document-toggle-show-whitespace"));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}

void dra_action_toggle_show_whitespace_set_editor_panel(DraActionToggleShowWhitespace *action, DraEditorPanel *editor_panel) {
	DraActionToggleShowWhitespacePrivate *priv = dra_action_toggle_show_whitespace_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	lea_action_set_sensitive_self(LEA_ACTION(action), editor_panel!=NULL);
	if (editor_panel!=NULL) {
		DraEditor *editor = dra_editor_panel_get_editor(editor_panel);
		ChaPreferencesWo *prefs = cha_editor_get_preferences((ChaEditor *) editor);
		dra_action_toggle_show_whitespace_reconfigured(action, (DraPreferencesWo *) prefs);
	}
}

void dra_action_toggle_show_whitespace_reconfigured(DraActionToggleShowWhitespace *action, DraPreferencesWo *a_prefs) {
	gboolean show_whitespace = a_prefs==NULL ? FALSE : cha_preferences_wo_get_show_whitespace((ChaPreferencesWo *) a_prefs);
	cat_log_debug("show_whitespace=%d", show_whitespace);
	lea_action_set_toggled((LeaAction *) action, show_whitespace);
}


static void l_action_run(LeaAction *self) {
	DraActionToggleShowWhitespacePrivate *priv = dra_action_toggle_show_whitespace_get_instance_private(DRA_ACTION_TOGGLE_SHOW_WHITESPACE(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		cha_editor_toggle_show_whitespace((ChaEditor *) editor);
	}
}
