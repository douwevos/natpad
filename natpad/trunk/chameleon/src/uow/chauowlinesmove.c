/*
   File:    chauowlinesmove.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 16, 2015
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

#include "chauowlinesmove.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowLinesMove"
#include <logging/catlog.h>

struct _ChaUowLinesMovePrivate {
	gboolean down;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowLinesMove, cha_uow_lines_move, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowLinesMove)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_lines_move_class_init(ChaUowLinesMoveClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_lines_move_init(ChaUowLinesMove *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_uow_lines_move_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_lines_move_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowLinesMove *cha_uow_lines_move_new(gboolean down) {
	ChaUowLinesMove *result = g_object_new(CHA_TYPE_UOW_LINES_MOVE, NULL);
	cat_ref_anounce(result);
	ChaUowLinesMovePrivate *priv = cha_uow_lines_move_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	priv->down = down;
	return result;
}

static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowLinesMovePrivate *priv = cha_uow_lines_move_get_instance_private((ChaUowLinesMove *) self);
	ChaLineLocationWo *ll_first = NULL;
	ChaLineLocationWo *ll_last = NULL;
	int valid_end_column;
	cha_uow_get_first_and_last_row(document_view, &ll_first, &ll_last, &valid_end_column);

	cat_log_debug("ll_first=%o, ll_last=%o", ll_first, ll_last);
	if (ll_first==NULL || ll_last==NULL) {
		cat_unref_ptr(ll_first);
		cat_unref_ptr(ll_last);
		return;
	}

	ChaDocument *document = cha_document_view_get_document(document_view);

	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	int delta = priv->down ? 1 : -1;
	if (cha_revision_wo_move_lines_to(e_revision, ll_first, ll_last, delta)) {
		ChaSelection *selection = cha_document_view_get_selection(document_view);
		if (selection) {


			ChaCursorWo *cur_start = cha_selection_get_start(selection);
			ChaCursorWo *e_start = cha_cursor_wo_create_editable(cur_start);
			ChaLineLocationWo *ll_start = cha_cursor_wo_get_line_location(e_start);
			ll_start = cha_line_location_wo_clone(ll_start, CAT_CLONE_DEPTH_MAIN);
			cha_line_location_wo_set_page_line_index(ll_start, cha_line_location_wo_get_page_line_index(ll_start)+delta);
			cha_cursor_wo_set_line_location(e_start, ll_start);
			cha_selection_set_start(selection, e_start);
			cat_unref_ptr(e_start);

			ChaCursorWo *cur_end = cha_selection_get_end(selection);
			ChaCursorWo *e_end = cha_cursor_wo_create_editable(cur_end);
			ChaLineLocationWo *ll_end = cha_cursor_wo_get_line_location(e_end);
			ll_end = cha_line_location_wo_clone(ll_end, CAT_CLONE_DEPTH_MAIN);
			cha_line_location_wo_set_page_line_index(ll_end, cha_line_location_wo_get_page_line_index(ll_end)+delta);
			cha_cursor_wo_set_line_location(e_end, ll_end);
			cha_selection_set_end(selection, e_end);
			cat_unref_ptr(e_end);
			

			gboolean valid_bounds = cha_line_location_wo_bind_to_revision(ll_start, e_revision)
					&& cha_line_location_wo_bind_to_revision(ll_end, e_revision);
			if (!valid_bounds) {
				cha_document_view_clear_selection(document_view, NULL);
			}

			cat_unref_ptr(ll_start);
			cat_unref_ptr(ll_end);
		}

		ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
		ChaLineLocationWo *ll = cha_cursor_wo_get_editable_line_location(e_cursor);
		cha_line_location_wo_set_page_line_index(ll, cha_line_location_wo_get_page_line_index(ll)+delta);
		cha_line_location_wo_bind_to_revision(ll, e_revision);

		cha_document_view_notify_selection_changed(document_view);
	}

	if (!is_editable) {
		cat_log_debug("anchoring");
		cha_document_anchor_document(document);
	}
	cat_unref_ptr(ll_first);
	cat_unref_ptr(ll_last);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowLinesMove *instance = CHA_UOW_LINES_MOVE(self);
	ChaUowLinesMovePrivate *priv = cha_uow_lines_move_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p down=%d]", iname, self, priv->down);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

