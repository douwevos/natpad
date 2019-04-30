/*
   File:    draactionselectlineending.c
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

#include "draactionselectlineending.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionSelectLineEnding"
#include <logging/catlog.h>

struct _DraActionSelectLineEndingPrivate {
	DraEditorPanel *editor_panel;
	ChaLineEnd line_end;
};

G_DEFINE_TYPE_WITH_CODE(DraActionSelectLineEnding, dra_action_select_line_ending, LEA_TYPE_ACTION,  // @suppress("Unused static function")
	G_ADD_PRIVATE(DraActionSelectLineEnding)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_select_line_ending_class_init(DraActionSelectLineEndingClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_select_line_ending_init(DraActionSelectLineEnding *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionSelectLineEnding *instance = DRA_ACTION_SELECT_LINE_ENDING(object);
	DraActionSelectLineEndingPrivate *priv = dra_action_select_line_ending_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_select_line_ending_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_select_line_ending_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionSelectLineEnding *dra_action_select_line_ending_new(ChaLineEnd line_end) {
	DraActionSelectLineEnding *result = g_object_new(DRA_TYPE_ACTION_SELECT_LINE_ENDING, NULL);
	cat_ref_anounce(result);
	DraActionSelectLineEndingPrivate *priv = dra_action_select_line_ending_get_instance_private(result);
	priv->editor_panel = NULL;
	priv->line_end = line_end;
	switch(line_end) {
		case CHA_LINE_END_CR : {
			lea_action_construct((LeaAction *) result, cat_string_wo_new_with("action.select.line.ends.cr"), cat_string_wo_new_with("Mac-OS (CR, 0x0D)"), cat_string_wo_new_with("edit-select-line-ends-cr"));
		} break;
		case CHA_LINE_END_LF : {
			lea_action_construct((LeaAction *) result, cat_string_wo_new_with("action.select.line.ends.lf"), cat_string_wo_new_with("Unix/Linux (LF, 0x0A)"), cat_string_wo_new_with("edit-select-line-ends-lf"));
		} break;
		case CHA_LINE_END_CRLF : {
			lea_action_construct((LeaAction *) result, cat_string_wo_new_with("action.select.line.ends.cr.lf"), cat_string_wo_new_with("Windows (CRLF, 0x0D0A)"), cat_string_wo_new_with("edit-select-line-ends-cr-lf"));
		} break;
		case CHA_LINE_END_NL : {
			lea_action_construct((LeaAction *) result, cat_string_wo_new_with("action.select.line.ends.nl"), cat_string_wo_new_with("AIX (NL, 0x15)"), cat_string_wo_new_with("edit-select-line-ends-nl"));
		} break;
	}
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	lea_action_set_toggable((LeaAction *) result, TRUE);
	lea_action_set_toggled((LeaAction *) result, FALSE);
	return result;
}

void dra_action_select_line_ending_set_editor_panel(DraActionSelectLineEnding *action, DraEditorPanel *editor_panel) {
	DraActionSelectLineEndingPrivate *priv = dra_action_select_line_ending_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	lea_action_set_sensitive_self(LEA_ACTION(action), editor_panel!=NULL);
}

void dra_action_select_line_ending_set_line_ends(DraActionSelectLineEnding *action, ChaLineEnd line_ends, gboolean line_ends_are_mixed) {
	if (line_ends_are_mixed) {
		lea_action_set_toggled((LeaAction *) action, FALSE);
	} else {
		DraActionSelectLineEndingPrivate *priv = dra_action_select_line_ending_get_instance_private(action);
		lea_action_set_toggled((LeaAction *) action, priv->line_end == line_ends);
	}
}


static void l_action_run(LeaAction *self) {
	DraActionSelectLineEndingPrivate *priv = dra_action_select_line_ending_get_instance_private((DraActionSelectLineEnding *) self);
	if (priv->editor_panel) {
		ChaUowSetLineEnds *select_uow = cha_uow_set_line_ends_new(priv->line_end);
		ChaEditor *editor = (ChaEditor *) dra_editor_panel_get_editor(priv->editor_panel);
		cha_editor_run(editor, (ChaUow *) select_uow);
		cat_unref_ptr(select_uow);
	}
}
