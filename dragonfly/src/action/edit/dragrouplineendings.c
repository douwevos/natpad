/*
   File:    dragrouplineendings.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 13, 2019
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

#include "dragrouplineendings.h"
#include "draactionselectlineending.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "DraGroupLineEndings"
#include <logging/catlog.h>

struct _DraGroupLineEndingsPrivate {
	DraActionSelectLineEnding *action_select_linux;
	DraActionSelectLineEnding *action_select_macos;
	DraActionSelectLineEnding *action_select_windows;
	DraActionSelectLineEnding *action_select_aix;
};

G_DEFINE_TYPE_WITH_CODE(DraGroupLineEndings, dra_group_line_endings, LEA_TYPE_ACTION_GROUP,  // @suppress("Unused static function")
	G_ADD_PRIVATE(DraGroupLineEndings)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_group_line_endings_class_init(DraGroupLineEndingsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_group_line_endings_init(DraGroupLineEndings *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraGroupLineEndings *instance = DRA_GROUP_LINE_ENDINGS(object);
	DraGroupLineEndingsPrivate *priv = dra_group_line_endings_get_instance_private(instance);
	cat_unref_ptr(priv->action_select_linux);
	cat_unref_ptr(priv->action_select_macos);
	cat_unref_ptr(priv->action_select_windows);
	cat_unref_ptr(priv->action_select_aix);
	G_OBJECT_CLASS(dra_group_line_endings_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_group_line_endings_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraGroupLineEndings *dra_group_line_endings_new(LeaKeyContext *key_context) {
	DraGroupLineEndings *result = g_object_new(DRA_TYPE_GROUP_LINE_ENDINGS, NULL);
	cat_ref_anounce(result);
	DraGroupLineEndingsPrivate *priv = dra_group_line_endings_get_instance_private(result);
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("dra.group.line.endings"), cat_string_wo_new_with("Line Delimiters"));

	priv->action_select_linux = dra_action_select_line_ending_new(CHA_LINE_END_LF);
	lea_action_set_key_context((LeaAction *) priv->action_select_linux, key_context);
	lea_action_set_order((LeaAction *) priv->action_select_linux, 100);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_select_linux);

	priv->action_select_macos = dra_action_select_line_ending_new(CHA_LINE_END_CR);
	lea_action_set_key_context((LeaAction *) priv->action_select_macos, key_context);
	lea_action_set_order((LeaAction *) priv->action_select_macos, 101);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_select_macos);

	priv->action_select_windows = dra_action_select_line_ending_new(CHA_LINE_END_CRLF);
	lea_action_set_key_context((LeaAction *) priv->action_select_windows, key_context);
	lea_action_set_order((LeaAction *) priv->action_select_windows, 102);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_select_windows);

	priv->action_select_aix = dra_action_select_line_ending_new(CHA_LINE_END_NL);
	lea_action_set_key_context((LeaAction *) priv->action_select_aix, key_context);
	lea_action_set_order((LeaAction *) priv->action_select_aix, 103);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_select_aix);

	return result;
}

void dra_group_line_endings_set_editor_panel(DraGroupLineEndings *group, DraEditorPanel *editor_panel) {
	DraGroupLineEndingsPrivate *priv = dra_group_line_endings_get_instance_private(group);
	dra_action_select_line_ending_set_editor_panel(priv->action_select_linux, editor_panel);
	dra_action_select_line_ending_set_editor_panel(priv->action_select_macos, editor_panel);
	dra_action_select_line_ending_set_editor_panel(priv->action_select_windows, editor_panel);
	dra_action_select_line_ending_set_editor_panel(priv->action_select_aix, editor_panel);
}


void dra_group_line_endings_set_line_ends(DraGroupLineEndings *group, ChaLineEnd line_ends, gboolean line_ends_are_mixed) {
	DraGroupLineEndingsPrivate *priv = dra_group_line_endings_get_instance_private(group);
	dra_action_select_line_ending_set_line_ends(priv->action_select_linux, line_ends, line_ends_are_mixed);
	dra_action_select_line_ending_set_line_ends(priv->action_select_macos, line_ends, line_ends_are_mixed);
	dra_action_select_line_ending_set_line_ends(priv->action_select_windows, line_ends, line_ends_are_mixed);
	dra_action_select_line_ending_set_line_ends(priv->action_select_aix, line_ends, line_ends_are_mixed);
}
