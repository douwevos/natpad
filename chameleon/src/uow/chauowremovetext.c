/*
   File:    chauowremovetext.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 23, 2015
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

#include "chauowremovetext.h"
#include "chauowmovecursor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowRemoveText"
#include <logging/catlog.h>

struct _ChaUowRemoveTextPrivate {
	gboolean forward;
	gboolean word;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowRemoveText, cha_uow_remove_text, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowRemoveText)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_remove_text_class_init(ChaUowRemoveTextClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_remove_text_init(ChaUowRemoveText *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUowRemoveText *instance = CHA_UOW_REMOVE_TEXT(object);
//	ChaUowRemoveTextPrivate *priv = cha_uow_remove_text_get_instance_private(instance);
	G_OBJECT_CLASS(cha_uow_remove_text_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_remove_text_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowRemoveText *cha_uow_remove_text_new(gboolean forward_delete, gboolean per_word) {
	ChaUowRemoveText *result = g_object_new(CHA_TYPE_UOW_REMOVE_TEXT, NULL);
	cat_ref_anounce(result);
	ChaUowRemoveTextPrivate *priv = cha_uow_remove_text_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	priv->forward = forward_delete;
	priv->word = per_word;
	return result;
}

static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowRemoveTextPrivate *priv = cha_uow_remove_text_get_instance_private((ChaUowRemoveText *) self);

	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	if (!cha_document_view_remove_selection(document_view)) {

		ChaCursorWo *cursor = cha_revision_wo_get_cursor(e_revision);
		ChaCursorWo *start_cursor = cha_cursor_wo_clone(cursor, CAT_CLONE_DEPTH_FULL);

	//	if (priv->forward) {
			ChaUowMoveCursor *move_cursor = cha_uow_move_cursor_new(priv->forward ? CHA_MOVE_CURSOR_NEXT_COLUMN : CHA_MOVE_CURSOR_PREV_COLUMN, FALSE, FALSE);
			cat_log_debug("move_cursor=%o", move_cursor);
			cha_uow_run((ChaUow *) move_cursor, editor, document_view);
			cursor = cha_revision_wo_get_cursor(e_revision);
			cat_log_debug("cursor=%o", cursor);

			cha_revision_wo_remove(e_revision, start_cursor);
			cursor = cha_revision_wo_get_cursor(e_revision);
			cat_log_debug("cursor=%o", cursor);
//			start_cursor = cha_cursor_wo_clone(cursor, CAT_CLONE_DEPTH_FULL);

			cat_unref_ptr(start_cursor);
			cat_unref_ptr(move_cursor);
	//	} else {
	//		ChaUowMoveCursor *move_cursor = cha_uow_move_cursor_new(CHA_MOVE_CURSOR_PREV_COLUMN, FALSE);
	//		cha_uow_run(move_cursor, document_view);
	//	}
	}


	if (!is_editable) {
		cha_document_anchor_document(document);
		cha_document_view_move_view_to_focus(document_view, FALSE);
	}
	ChaRevisionWo *a_revision = cha_document_get_current_revision_ref(document);
	cat_log_debug("invoking mark, cursor=%o", cha_revision_wo_get_cursor(a_revision));

	cha_document_view_mark_layout_x_cursor(document_view, a_revision, cha_revision_wo_get_cursor(a_revision));
	cat_log_debug("done invoking mark");
	cat_unref_ptr(a_revision);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowRemoveText *instance = CHA_UOW_REMOVE_TEXT(self);
	ChaUowRemoveTextPrivate *priv = cha_uow_remove_text_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p forward=%d, word=%d]", iname, self, priv->forward, priv->word);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
