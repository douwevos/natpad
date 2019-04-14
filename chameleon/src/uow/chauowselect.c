/*
   File:    chauowselect.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 6, 2015
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

#include "chauowselect.h"
#include "chautf8.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowSelect"
#include <logging/catlog.h>

struct _ChaUowSelectPrivate {
	ChaUowSelectKind kind;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

G_DEFINE_TYPE_WITH_CODE(ChaUowSelect, cha_uow_select, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowSelect)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_uow_select_class_init(ChaUowSelectClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_select_init(ChaUowSelect *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_uow_select_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_select_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowSelect *cha_uow_select_new(ChaUowSelectKind kind) {
	ChaUowSelect *result = g_object_new(CHA_TYPE_UOW_SELECT, NULL);
	cat_ref_anounce(result);
	ChaUowSelectPrivate *priv = cha_uow_select_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, TRUE);
	priv->kind = kind;
	return result;
}


static gboolean l_select_line(ChaDocumentView *document_view) {
	ChaDocument *document = cha_document_view_get_document(document_view);
	ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);
	ChaCursorWo *cur_cursor = cha_revision_wo_get_cursor(a_rev);
	ChaLineLocationWo *cur_line_loc = cha_cursor_wo_get_line_location(cur_cursor);
	ChaPageWo *page = cha_revision_wo_page_at(a_rev, cha_line_location_wo_get_page_index(cur_line_loc));

	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, cha_line_location_wo_get_page_line_index(cur_line_loc), FALSE);
	int length = utf8_text.text_len;
	cha_utf8_text_cleanup(&utf8_text);

//	gboolean is_editable = cha_document_is_editable(document);
//	ChaRevisionWo *e_rev = cha_document_get_editable_revision(document);

	ChaCursorWo *tmp_start = cha_cursor_wo_new();
	cha_cursor_wo_set_line_location(tmp_start, cur_line_loc);
	cha_cursor_wo_set_x_cursor_bytes(tmp_start, 0);

	ChaCursorWo *tmp_end = cha_cursor_wo_new();
	cha_cursor_wo_set_line_location(tmp_end, cur_line_loc);
	cha_cursor_wo_set_x_cursor_bytes(tmp_end, length);
	cha_document_view_set_plain_selection(document_view, tmp_start, tmp_end);
	cat_unref(tmp_start);
	cat_unref(tmp_end);
	cat_unref_ptr(a_rev);
//	if (!is_editable) {
//		cha_document_anchor_document(document);
//	}
	return TRUE;
}

static gboolean l_select_word(ChaDocumentView *document_view) {
//	ChaUowSelectPrivate *priv = cha_uow_select_get_instance_private((ChaUowSelect *) self);
	ChaDocument *document = cha_document_view_get_document(document_view);
	ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);
	ChaCursorWo *cur_cursor = cha_revision_wo_get_cursor(a_rev);
	ChaLineLocationWo *cur_line_loc = cha_cursor_wo_get_line_location(cur_cursor);
	ChaPageWo *page = cha_revision_wo_page_at(a_rev, cha_line_location_wo_get_page_index(cur_line_loc));

	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, cha_line_location_wo_get_page_line_index(cur_line_loc), FALSE);

	int length = utf8_text.text_len;
	const char *text = utf8_text.text;
	const char *text_end = text + length;

	int start = cha_cursor_wo_get_x_cursor_bytes(cur_cursor);
	int end = start;


	gunichar unch = cha_utf8_unichar_at(text+start, text_end);
	gboolean isnbc = g_unichar_isalnum(unch);

	while(start>0) {
		gunichar nunch;
		int next = cha_utf8_prev_char(text, start, &nunch);
		gboolean nisnbc = g_unichar_isalnum(nunch);
		if (nisnbc==isnbc) {
			start = next;
		} else {
			break;
		}
	}

	while(end<length) {
		gunichar nunch;
		int next = cha_utf8_next_char(text, end, length, &nunch);
		gboolean nisnbc = g_unichar_isalnum(nunch);
		if (nisnbc==isnbc) {
			end = next;
		} else {
			break;
		}
	}

	cha_utf8_text_cleanup(&utf8_text);

	if (start==end) {
		cat_unref_ptr(a_rev);
		return FALSE;
	}

	gboolean is_editable = cha_document_is_editable(document);
	cha_document_get_editable_revision(document);

	ChaCursorWo *tmp_start = cha_cursor_wo_new();
	cha_cursor_wo_set_line_location(tmp_start, cur_line_loc);
	cha_cursor_wo_set_x_cursor_bytes(tmp_start, start);

	ChaCursorWo *tmp_end = cha_cursor_wo_new();
	cha_cursor_wo_set_line_location(tmp_end, cur_line_loc);
	cha_cursor_wo_set_x_cursor_bytes(tmp_end, end);
	cha_document_view_set_plain_selection(document_view, tmp_start, tmp_end);
	cat_unref(tmp_start);
	cat_unref(tmp_end);
	cat_unref_ptr(a_rev);
	if (!is_editable) {
		cha_document_anchor_document(document);
	}
	return TRUE;
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowSelectPrivate *priv = cha_uow_select_get_instance_private((ChaUowSelect *) self);
	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *a_rev = cha_document_get_editable_revision(document);
	switch(priv->kind) {
		case CHA_UOW_SELECT_ALL : {
			ChaSelection *selection = cha_document_view_get_selection(document_view);
			ChaCursorWo *cursor_start = cha_cursor_wo_new();
			if (CHA_IS_BLOCK_SELECTION(selection)) {
				cha_document_view_clear_selection(document_view, a_rev);
				selection = NULL;
			} else if (CHA_IS_PLAIN_SELECTION(selection)) {
				if (!cha_cursor_wo_equal(cha_selection_get_start(selection), cursor_start)) {
					cha_document_view_clear_selection(document_view, a_rev);
					selection = NULL;
				}
			}
			if (selection==NULL) {
				selection = cha_document_view_get_or_create_selection(document_view, a_rev, cursor_start, FALSE);
			}
			ChaCursorWo *cursor_end = cha_cursor_wo_new();
			ChaLineLocationWo *e_ll = cha_cursor_wo_get_editable_line_location(cursor_end);
			int page_index = cha_revision_wo_page_count(a_rev)-1;
			cha_line_location_wo_set_page_index(e_ll, page_index);
			ChaPageWo *page = cha_revision_wo_page_at(a_rev, page_index);
			int line_index = cha_page_wo_line_count(page)-1;
			cha_line_location_wo_set_page_line_index(e_ll, line_index);
			ChaLineWo *line = cha_page_wo_line_at(page, line_index);
			cha_cursor_wo_set_x_cursor_bytes(cursor_end, cha_line_wo_byte_count(line));
			cha_selection_set_end(selection, cursor_end);
			ChaCursorWo *a_cursor = cha_revision_wo_get_editable_cursor(a_rev);
			e_ll = cha_cursor_wo_get_editable_line_location(a_cursor);
			cha_line_location_wo_set_page_index(e_ll, page_index);
			cha_line_location_wo_set_page_line_index(e_ll, line_index);
			cha_cursor_wo_set_x_cursor_bytes(a_cursor, cha_line_wo_byte_count(line));
			cat_unref_ptr(line);

		} break;
		case CHA_UOW_SELECT_WORD : {
			l_select_word(document_view);
		} break;
		case CHA_UOW_SELECT_LINE : {
			l_select_line(document_view);
//			ast_document_view_select_line(view->document_view);
		} break;
	}
	cha_document_view_notify_selection_changed(document_view);
	if (!is_editable) {
		cha_document_anchor_document(document);
		cha_document_view_move_view_to_focus(document_view, FALSE);
		cha_document_view_invalidate_lines(document_view);
	}

}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowSelect *instance = CHA_UOW_SELECT(self);
	ChaUowSelectPrivate *priv = cha_uow_select_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p kind=%d]", iname, self, priv->kind);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

