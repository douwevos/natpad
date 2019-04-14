/*
   File:    chauowlinemarker.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 31, 2015
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

#include "chauowlinemarker.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowLineMarker"
#include <logging/catlog.h>

struct _ChaUowLineMarkerPrivate {
	ChaUowMarkMode mode;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowLineMarker, cha_uow_line_marker, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowLineMarker)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_line_marker_class_init(ChaUowLineMarkerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_line_marker_init(ChaUowLineMarker *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUowLineMarker *instance = CHA_UOW_LINE_MARKER(object);
//	ChaUowLineMarkerPrivate *priv = cha_uow_line_marker_get_instance_private(instance);
	G_OBJECT_CLASS(cha_uow_line_marker_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_line_marker_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowLineMarker *cha_uow_line_marker_new(ChaUowMarkMode mode) {
	ChaUowLineMarker *result = g_object_new(CHA_TYPE_UOW_LINE_MARKER, NULL);
	cat_ref_anounce(result);
	ChaUowLineMarkerPrivate *priv = cha_uow_line_marker_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, TRUE);
	priv->mode = mode;
	return result;
}

static void l_mark_block_or_line(ChaDocumentView *document_view, ChaRevisionWo *e_revision, ChaUowMarkMode mode) {
	ChaSelection *selection = cha_document_view_get_selection(document_view);
	if (selection) {
		int top_page_index, bottom_page_index;
		int top_line_index, bottom_line_index;
		cha_selection_get_top(selection, &top_page_index, &top_line_index, NULL);
		cha_selection_get_bottom(selection, &bottom_page_index, &bottom_line_index, NULL);

		ChaPageWo *e_page = NULL;
		int page_line_count;
		while(TRUE) {
			if (e_page==NULL) {
				if (top_page_index>=cha_revision_wo_page_count(e_revision)) {
					break;
				}
				e_page = cha_revision_wo_editable_page_at(e_revision, top_page_index);
				page_line_count = cha_page_wo_line_count(e_page);
			} else {
				if (top_line_index>=page_line_count) {
					top_line_index = 0;
					top_page_index++;
					e_page = NULL;
				} else {
					switch(mode) {
						case CHA_UOW_MARK_TOGGLE :
							cha_page_wo_toggle_line_mark(e_page, top_line_index);
							break;
						case CHA_UOW_MARK_SET :
							cha_page_wo_set_line_mark(e_page, top_line_index);
							break;
						case CHA_UOW_MARK_UNSET :
							cha_page_wo_unset_line_mark(e_page, top_line_index);
							break;
						default :
							break;
					}
					if ((top_page_index==bottom_page_index) && (top_line_index==bottom_line_index)) {
						break;
					}
					top_line_index++;
				}
			}
		}
	} else {
		ChaCursorWo *cursor = cha_revision_wo_get_cursor(e_revision);
		ChaLineLocationWo *ll = cha_cursor_wo_get_line_location(cursor);
		int page_index = cha_line_location_wo_get_page_index(ll);
		int page_line_index = cha_line_location_wo_get_page_line_index(ll);
		ChaPageWo *e_page = cha_revision_wo_editable_page_at(e_revision, page_index);
		cha_page_wo_toggle_line_mark(e_page, page_line_index);
	}
}

static void l_mark_all(ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	int idx;
	for(idx=cha_revision_wo_page_count(e_revision)-1; idx>=0; idx--) {
		ChaPageWo *e_page = cha_revision_wo_editable_page_at(e_revision, idx);
		cha_page_wo_mark_all_lines(e_page);
	}
}

static void l_unmark_all(ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	int idx;
	for(idx=cha_revision_wo_page_count(e_revision)-1; idx>=0; idx--) {
		ChaPageWo *e_page = cha_revision_wo_editable_page_at(e_revision, idx);
		cha_page_wo_unmark_all_lines(e_page);
	}
}

static void l_mark_toggle_all(ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	int idx;
	for(idx=cha_revision_wo_page_count(e_revision)-1; idx>=0; idx--) {
		ChaPageWo *e_page = cha_revision_wo_editable_page_at(e_revision, idx);
		cha_page_wo_mark_toggle_all_lines(e_page);
	}
}

static void l_mark_copy(ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	int page_idx;
	int page_count = cha_revision_wo_page_count(e_revision);
	ChaLineEnd line_ends = cha_revision_wo_get_line_ends(e_revision);
	gboolean line_ends_are_mixed = cha_revision_wo_get_line_ends_are_mixed(e_revision);
	cat_log_debug("page_count=%d", page_count);
	CatStringWo *e_buf = cat_string_wo_new();
	for(page_idx=0; page_idx<page_count; page_idx++) {
		cat_log_debug("page_idx=%d", page_idx);
		ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_idx);
		if (cha_page_wo_has_marked_lines(page)) {
			int line_count = cha_page_wo_line_count(page);
			int line_idx = 0;
			cat_log_debug("line_count=%d", line_count);
			for(line_idx=0; line_idx<line_count; line_idx++) {
				cat_log_debug("line_idx=%d", line_idx);
				if (cha_page_wo_is_line_marked(page, line_idx)) {
					const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, line_idx, FALSE);
					cat_string_wo_append_chars_len(e_buf, utf8_text.text, utf8_text.text_len);
					ChaLineEnd line_end = utf8_text.line_end;
					if (line_end!=CHA_LINE_END_NONE && !line_ends_are_mixed) {
						line_end = line_ends;
					}
					cha_line_end_to_string(line_end, e_buf);
					cha_utf8_text_cleanup(&utf8_text);
				}
			}
		}
	}
	GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);
	gtk_clipboard_set_text(clipboard, cat_string_wo_getchars(e_buf), cat_string_wo_length(e_buf));
	cat_unref_ptr(e_buf);
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowLineMarkerPrivate *priv = cha_uow_line_marker_get_instance_private((ChaUowLineMarker *) self);

	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	cat_log_debug("mode=%d", priv->mode);

	switch(priv->mode) {
		case CHA_UOW_MARK_SET :
			l_mark_block_or_line(document_view, e_revision, CHA_UOW_MARK_SET);
			break;
		case CHA_UOW_MARK_UNSET :
			l_mark_block_or_line(document_view, e_revision, CHA_UOW_MARK_UNSET);
			break;
		case CHA_UOW_MARK_TOGGLE :
			l_mark_block_or_line(document_view, e_revision, CHA_UOW_MARK_TOGGLE);
			break;

		case CHA_UOW_MARK_SET_ALL :
			l_mark_all(document_view, e_revision);
			break;
		case CHA_UOW_MARK_UNSET_ALL :
			l_unmark_all(document_view, e_revision);
			break;
		case CHA_UOW_MARK_TOGGLE_ALL :
			l_mark_toggle_all(document_view, e_revision);
			break;

		case CHA_UOW_MARK_COPY :
			l_mark_copy(document_view, e_revision);
			break;

	}


	if (!is_editable) {
		cha_document_anchor_document(document);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowLineMarker *instance = CHA_UOW_LINE_MARKER(self);
	ChaUowLineMarkerPrivate *priv = cha_uow_line_marker_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p mode=%d]", iname, self, priv->mode);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
