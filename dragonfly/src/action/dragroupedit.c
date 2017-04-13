/*
   File:    dragroupedit.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 20, 2015
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

#include "dragroupedit.h"
#include "draactiongotoline.h"


#include "edit/draactioncopy.h"
#include "edit/draactioncut.h"
#include "edit/draactiondelete.h"
#include "edit/draactionpaste.h"
#include "edit/draactionredo.h"
#include "edit/draactionselectall.h"
#include "edit/draactionundo.h"
#include "edit/draactionindent.h"
#include "edit/draactiondedent.h"
#include "edit/draactiontogglelinecomment.h"
#include "edit/draactiontolowercase.h"
#include "edit/draactiontouppercase.h"
#include "edit/draactionformat.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraGroupEdit"
#include <logging/catlog.h>

struct _DraGroupEditPrivate {
	gboolean clipboard_has_text;
	gpointer clipboard_handler;

	LeaKeyContext *key_context;
	DraActionUndo *action_undo;
	DraActionRedo *action_redo;
	DraActionCopy *action_copy;
	DraActionCut *action_cut;
	DraActionPaste *action_paste;
	DraActionSelectAll *action_select_all;
	DraActionDelete *action_delete;

	DraActionIndent *action_indent;
	DraActionDedent *action_dedent;
	DraActionToggleLineComment *action_toggle_line_comment;

	DraActionToLowerCase *action_to_lower_case;
	DraActionToUpperCase *action_to_upper_case;
	DraActionFormat *action_format;

	DraActionGotoLine *action_goto_line;
	DraActionGotoLine *action_open_declaration;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraGroupEdit, dra_group_edit, LEA_TYPE_ACTION_GROUP,
		G_ADD_PRIVATE(DraGroupEdit)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_group_edit_class_init(DraGroupEditClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_group_edit_init(DraGroupEdit *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraGroupEdit *instance = DRA_GROUP_EDIT(object);
	DraGroupEditPrivate *priv = dra_group_edit_get_instance_private(instance);
	cat_unref_ptr(priv->action_copy);
	cat_unref_ptr(priv->action_cut);
	cat_unref_ptr(priv->action_delete);
	cat_unref_ptr(priv->action_goto_line);
	cat_unref_ptr(priv->action_open_declaration);
	cat_unref_ptr(priv->action_paste);
	cat_unref_ptr(priv->action_redo);
	cat_unref_ptr(priv->action_select_all);
	cat_unref_ptr(priv->action_undo);

	cat_unref_ptr(priv->action_indent);
	cat_unref_ptr(priv->action_dedent);
	cat_unref_ptr(priv->action_toggle_line_comment);
	cat_unref_ptr(priv->action_to_lower_case);
	cat_unref_ptr(priv->action_to_upper_case);
	cat_unref_ptr(priv->action_format);

	cat_unref_ptr(priv->key_context);
	G_OBJECT_CLASS(dra_group_edit_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_group_edit_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraGroupEdit *dra_group_edit_new(LeaKeyContext *key_context, gpointer clipboard_handler) {
	DraGroupEdit *result = g_object_new(DRA_TYPE_GROUP_EDIT, NULL);
	cat_ref_anounce(result);
	DraGroupEditPrivate *priv = dra_group_edit_get_instance_private(result);
	priv->clipboard_handler = clipboard_handler;
	priv->clipboard_has_text = FALSE;
	priv->key_context = cat_ref_ptr(key_context);
	lea_action_group_construct((LeaActionGroup *) result, cat_ref_ptr(lea_prov_edit_group_name()), cat_string_wo_new_with("_Edit"));

	LeaActionGroup *grp_undo_redo = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("edit.undo.redo.group"), NULL);
	lea_action_set_order((LeaAction *) grp_undo_redo, -4);

	priv->action_undo = dra_action_undo_new();
	lea_action_set_key_context((LeaAction *) priv->action_undo, key_context);
	lea_action_set_order((LeaAction *) priv->action_undo, -2);
	lea_action_group_add(grp_undo_redo, (LeaAction *) priv->action_undo);

	priv->action_redo = dra_action_redo_new();
	lea_action_set_key_context((LeaAction *) priv->action_redo, key_context);
	lea_action_set_order((LeaAction *) priv->action_redo, -1);
	lea_action_group_add(grp_undo_redo, (LeaAction *) priv->action_redo);

	cat_unref(grp_undo_redo);


	LeaActionGroup *grp_clipboard = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("edit.clipboard.group"), NULL);
	lea_action_set_order((LeaAction *) grp_clipboard, -3);

	priv->action_cut = dra_action_cut_new(clipboard_handler);
	lea_action_set_key_context((LeaAction *) priv->action_cut, key_context);
	lea_action_set_order((LeaAction *) priv->action_cut, -3);
	lea_action_group_add(grp_clipboard, (LeaAction *) priv->action_cut);

	priv->action_copy = dra_action_copy_new(clipboard_handler);
	lea_action_set_key_context((LeaAction *) priv->action_copy, key_context);
	lea_action_set_order((LeaAction *) priv->action_copy, -2);
	lea_action_group_add(grp_clipboard, (LeaAction *) priv->action_copy);

	priv->action_paste = dra_action_paste_new();
	lea_action_set_key_context((LeaAction *) priv->action_paste, key_context);
	lea_action_set_order((LeaAction *) priv->action_paste, -1);
	lea_action_group_add(grp_clipboard, (LeaAction *) priv->action_paste);

	cat_unref(grp_clipboard);



	LeaActionGroup *grp_delete = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("edit.delete.group"), NULL);
	lea_action_set_order((LeaAction *) grp_delete, -2);

	priv->action_delete = dra_action_delete_new(key_context);
	lea_action_set_key_context((LeaAction *) priv->action_delete, key_context);
	lea_action_set_order((LeaAction *) priv->action_delete, -2);
	lea_action_group_add(grp_delete, (LeaAction *) priv->action_delete);

	priv->action_select_all = dra_action_select_all_new();
	lea_action_set_key_context((LeaAction *) priv->action_select_all, key_context);
	lea_action_set_order((LeaAction *) priv->action_select_all, -1);
	lea_action_group_add(grp_delete, (LeaAction *) priv->action_select_all);

	cat_unref(grp_delete);


	LeaActionGroup *grp_context = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("edit.context.group"), NULL);
	lea_action_set_order((LeaAction *) grp_context, -1);

	priv->action_indent = dra_action_indent_new();
	lea_action_set_key_context((LeaAction *) priv->action_indent, key_context);
	lea_action_set_order((LeaAction *) priv->action_indent, -6);
	lea_action_group_add(grp_context, (LeaAction *) priv->action_indent);

	priv->action_dedent = dra_action_dedent_new();
	lea_action_set_key_context((LeaAction *) priv->action_dedent, key_context);
	lea_action_set_order((LeaAction *) priv->action_dedent, -5);
	lea_action_group_add(grp_context, (LeaAction *) priv->action_dedent);

	priv->action_toggle_line_comment = dra_action_toggle_line_comment_new();
	lea_action_set_key_context((LeaAction *) priv->action_toggle_line_comment, key_context);
	lea_action_set_order((LeaAction *) priv->action_toggle_line_comment, -4);
	lea_action_group_add(grp_context, (LeaAction *) priv->action_toggle_line_comment);

	priv->action_to_lower_case = dra_action_to_lower_case_new();
	lea_action_set_key_context((LeaAction *) priv->action_to_lower_case, key_context);
	lea_action_set_order((LeaAction *) priv->action_to_lower_case, -3);
	lea_action_group_add(grp_context, (LeaAction *) priv->action_to_lower_case);

	priv->action_to_upper_case = dra_action_to_upper_case_new();
	lea_action_set_key_context((LeaAction *) priv->action_to_upper_case, key_context);
	lea_action_set_order((LeaAction *) priv->action_to_upper_case, -2);
	lea_action_group_add(grp_context, (LeaAction *) priv->action_to_upper_case);

	priv->action_format = dra_action_format_new();
	lea_action_set_key_context((LeaAction *) priv->action_format, key_context);
	lea_action_set_order((LeaAction *) priv->action_format, -1);
	lea_action_group_add(grp_context, (LeaAction *) priv->action_format);

	cat_unref(grp_context);


	priv->action_goto_line = dra_action_goto_line_new();
	lea_action_set_key_context((LeaAction *) priv->action_goto_line, key_context);
	lea_action_set_order((LeaAction *) priv->action_goto_line, 200);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_goto_line);

	priv->action_open_declaration = dra_action_open_declaration_new();
	lea_action_set_key_context((LeaAction *) priv->action_open_declaration, key_context);
	lea_action_set_order((LeaAction *) priv->action_open_declaration, 201);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_open_declaration);

	return result;
}

void dra_group_edit_set_has_selection(DraGroupEdit *group_edit, gboolean has_selection) {
	DraGroupEditPrivate *priv = dra_group_edit_get_instance_private(group_edit);
	dra_action_copy_set_has_selection(priv->action_copy, has_selection);
	dra_action_cut_set_has_selection(priv->action_cut, has_selection);
}

void dra_group_edit_set_has_clipboard(DraGroupEdit *group_edit, gboolean has_clipboard) {
	DraGroupEditPrivate *priv = dra_group_edit_get_instance_private(group_edit);
	dra_action_paste_set_has_clipboard(priv->action_paste, has_clipboard);
}


void dra_group_edit_set_history_info(DraGroupEdit *group_edit, int history_index, int history_length) {
	DraGroupEditPrivate *priv = dra_group_edit_get_instance_private(group_edit);
	dra_action_undo_set_has_undo(priv->action_undo, history_index>0);
	dra_action_redo_set_has_redo(priv->action_redo, history_index+1<history_length);
}


void dra_group_edit_set_editor_panel(DraGroupEdit *group_edit, DraEditorPanel *editor_panel) {
	DraGroupEditPrivate *priv = dra_group_edit_get_instance_private(group_edit);

	dra_action_select_all_set_editor_panel(priv->action_select_all, editor_panel);
	dra_action_cut_set_editor_panel(priv->action_cut, editor_panel);
	dra_action_copy_set_editor_panel(priv->action_copy, editor_panel);
	dra_action_paste_set_editor_panel(priv->action_paste, editor_panel);
	dra_action_delete_set_editor_panel(priv->action_delete, editor_panel);
	dra_action_undo_set_editor_panel(priv->action_undo, editor_panel);
	dra_action_redo_set_editor_panel(priv->action_redo, editor_panel);

	dra_action_indent_set_editor_panel(priv->action_indent, editor_panel);
	dra_action_dedent_set_editor_panel(priv->action_dedent, editor_panel);
	dra_action_toggle_line_comment_set_editor_panel(priv->action_toggle_line_comment, editor_panel);
	dra_action_to_lower_case_set_editor_panel(priv->action_to_lower_case, editor_panel);
	dra_action_to_upper_case_set_editor_panel(priv->action_to_upper_case, editor_panel);

	dra_action_goto_line_set_editor_panel(priv->action_goto_line, editor_panel);
	dra_action_open_declaration_set_editor_panel(priv->action_open_declaration, editor_panel);
	dra_action_format_set_editor_panel(priv->action_format, editor_panel);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraGroupEdit *instance = DRA_GROUP_EDIT(self);
	DraGroupEditPrivate *priv = dra_group_edit_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p clipboard_has_text=%d]", iname, self, priv->clipboard_has_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
