/*
   File:    draactiontogglespelling.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    1 Jul 2019
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

#include "draactiontogglespelling.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "DraActionToggleSpelling"
#include <logging/catlog.h>

struct _DraActionToggleSpellingPrivate {
	DraEditorPanel *editor_panel;
};

G_DEFINE_TYPE_WITH_CODE(DraActionToggleSpelling, dra_action_toggle_spelling, LEA_TYPE_ACTION,  // @suppress("Unused static function")
	G_ADD_PRIVATE(DraActionToggleSpelling)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_toggle_spelling_class_init(DraActionToggleSpellingClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_toggle_spelling_init(DraActionToggleSpelling *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionToggleSpelling *instance = DRA_ACTION_TOGGLE_SPELLING(object);
	DraActionToggleSpellingPrivate *priv = dra_action_toggle_spelling_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_toggle_spelling_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_toggle_spelling_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionToggleSpelling *dra_action_toggle_spelling_new() {
	DraActionToggleSpelling *result = g_object_new(DRA_TYPE_ACTION_TOGGLE_SPELLING, NULL);
	cat_ref_anounce(result);
	DraActionToggleSpellingPrivate *priv = dra_action_toggle_spelling_get_instance_private(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.toggle.spelling"), cat_string_wo_new_with("_Spelling"), cat_string_wo_new_with("document-toggle-spelling"));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}


void dra_action_toggle_spelling_set_editor_panel(DraActionToggleSpelling *action, DraEditorPanel *editor_panel) {
	DraActionToggleSpellingPrivate *priv = dra_action_toggle_spelling_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	lea_action_set_sensitive_self(LEA_ACTION(action), editor_panel!=NULL);
	if (editor_panel!=NULL) {
		DraEditor *editor = dra_editor_panel_get_editor(editor_panel);
		DraPreferencesWo *prefs = (DraPreferencesWo *) cha_editor_get_preferences(editor);
		dra_action_toggle_spelling_reconfigured(action, (DraPreferencesWo *) prefs);
	}
}

void dra_action_toggle_spelling_reconfigured(DraActionToggleSpelling *action, DraPreferencesWo *a_prefs) {
	DraActionToggleSpellingPrivate *priv = dra_action_toggle_spelling_get_instance_private(action);
	gboolean spelling = FALSE;
	if (a_prefs) {
		DraPrefsSpellingWo *prefs_spelling = dra_preferences_wo_get_spelling(a_prefs);
		spelling = dra_prefs_spelling_wo_is_enabled(prefs_spelling);
	}
	cat_log_error("spelling=%d", spelling);
	lea_action_set_toggled(action, spelling);
}


static void l_action_run(LeaAction *self) {
	DraActionToggleSpellingPrivate *priv = dra_action_toggle_spelling_get_instance_private(DRA_ACTION_TOGGLE_SPELLING(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		dra_editor_toggle_spelling((ChaEditor *) editor);
	}
}
