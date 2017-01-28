/*
   File:    chauowmovecursor.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 21, 2015
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

#include "chauowmovecursor.h"
#include "chautf8.h"
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowMoveCursor"
#include <logging/catlog.h>

struct _ChaUowMoveCursorPrivate {
	ChaMoveCursorKind kind;
	gboolean mark_cursor;
	gboolean extend_selection;
	gboolean alt_pressed;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowMoveCursor, cha_uow_move_cursor, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowMoveCursor)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_move_cursor_class_init(ChaUowMoveCursorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_move_cursor_init(ChaUowMoveCursor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUowMoveCursor *instance = CHA_UOW_MOVE_CURSOR(object);
//	ChaUowMoveCursorPrivate *priv = cha_uow_move_cursor_get_instance_private(instance);
	G_OBJECT_CLASS(cha_uow_move_cursor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_move_cursor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowMoveCursor *cha_uow_move_cursor_new(ChaMoveCursorKind kind, gboolean extend_selection, gboolean alt_pressed) {
	ChaUowMoveCursor *result = g_object_new(CHA_TYPE_UOW_MOVE_CURSOR, NULL);
	cat_ref_anounce(result);
	ChaUowMoveCursorPrivate *priv = cha_uow_move_cursor_get_instance_private(result);
	priv->kind = kind;
	priv->extend_selection = extend_selection;
	priv->mark_cursor = TRUE;
	priv->alt_pressed = alt_pressed;
	cha_uow_construct((ChaUow *) result, TRUE);
	return result;
}



static void l_next_column(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision, gboolean per_word) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);
	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);

	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);

	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int cur_x_next = cur_x;
	int sub_x = cha_cursor_wo_get_x_sub(e_cursor);
	if (sub_x>0) {
		sub_x++;
	} else {
		if (per_word) {
			CatStringWo *text = cha_line_wo_get_text(line);
			const char *chrs = cat_string_wo_getchars(text);
			int chrs_len = cat_string_wo_length(text);
			gunichar uch = 0;

			int status = 0;
			while(cur_x_next<chrs_len) {
				int x_next = cha_utf8_next_char(chrs, cur_x_next, chrs_len, &uch);
				gboolean isnbc = g_unichar_isalnum(uch);
				if (status==0 && isnbc) {
					status=1;
				} else if (status==1 && !isnbc) {
					break;
				}
				cur_x_next = x_next;
			}

		} else {
			int trailing;
			pango_layout_move_cursor_visually(pango_layout, TRUE, cur_x, 0, 1, &cur_x_next, &trailing);
		}
		if (cur_x_next == cur_x) {
			cur_x_next++;
		}

		cat_log_debug("cur_x=%d, cur_x_next=%d, page_index=%d, page_line_index=%d, cha_revision_wo_page_count(e_revision)=%d", cur_x, cur_x_next, page_index, page_line_index, cha_revision_wo_page_count(e_revision));
		if (cur_x_next > cha_line_wo_byte_count(line)) {
			if (view_ctx.limit_cursor) {
				ChaLineLocationWo *e_line_location = NULL;
				if (page_line_index<cha_page_wo_line_count(page)-1) {
					page_line_index++;
					e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
					cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
					cur_x_next = 0;
				} else if (page_index<cha_revision_wo_page_count(e_revision)-1) {
					cur_x_next = 0;
					page_index++;
					page_line_index=0;
					e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
					cha_line_location_wo_set_page_index(e_line_location, page_index);
					cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
				} else {
					cur_x_next = cha_line_wo_byte_count(line);
				}
				sub_x = 0;
			} else {
				cur_x_next = cha_line_wo_byte_count(line);
				sub_x++;
			}

		}
	}

	cat_log_debug("cur_x=%d, cur_x_next=%d, sub_x=%d, page_index=%d, page_line_index=%d", cur_x, cur_x_next, sub_x, page_index, page_line_index);

	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x_next);
	cha_cursor_wo_set_x_sub(e_cursor, sub_x);
	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);
}


static void l_prev_column(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision, gboolean per_word) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);
	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);

	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int cur_x_next = cur_x;
	int sub_x = cha_cursor_wo_get_x_sub(e_cursor);
	if (view_ctx.limit_cursor) {
		sub_x = 0;
	}

	if (sub_x>0) {
		sub_x--;
	} else if (cur_x_next>0) {
		if (per_word) {
			CatStringWo *text = cha_line_wo_get_text(line);
			const char *chrs = cat_string_wo_getchars(text);
			gunichar uch = 0;

			int status = 0;
			while(cur_x_next>0) {
				int x_next = cha_utf8_prev_char(chrs, cur_x_next, &uch);
				gboolean isnbc = g_unichar_isalnum(uch);
				if (status==0 && isnbc) {
					status=1;
				} else if (status==1 && !isnbc) {
					break;
				}
				cur_x_next = x_next;
			}
		} else {
			int trailing;
			pango_layout_move_cursor_visually(pango_layout, TRUE, cur_x, 0, -1, &cur_x_next, &trailing);
		}
	} else {
		cur_x_next--;
	}

//	if (cur_x_next>0) {
//		int trailing;
//		pango_layout_move_cursor_visually(pango_layout, TRUE, cur_x, 0, -1, &cur_x_next, &trailing);
//	} else {
//		cur_x_next--;
//	}

	if (cur_x_next<0) {
		ChaLineLocationWo *e_line_location = NULL;
		if (page_line_index>0) {
			page_line_index--;
			e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
			cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
			cat_unref_ptr(line);
			line = cha_page_wo_line_at(page, page_line_index);
			cur_x_next = cha_line_wo_byte_count(line);
		} else if (page_index>0) {
			page_index--;
			page = cha_revision_wo_page_at(e_revision, page_index);
			page_line_index = cha_page_wo_line_count(page)-1;
			e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
			cha_line_location_wo_set_page_index(e_line_location, page_index);
			cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
			cat_unref_ptr(line);
			line = cha_page_wo_line_at(page, page_line_index);
			cur_x_next = cha_line_wo_byte_count(line);
		} else {
			cur_x_next = 0;
		}
	}
	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x_next);
	cha_cursor_wo_set_x_sub(e_cursor, sub_x);
	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);
}

static void l_prev_row(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);
	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);

	int marked_x_cursor = cha_document_view_get_marked_layout_x_cursor(document_view)*PANGO_SCALE;

	int line_count = pango_layout_get_line_count(pango_layout);
	PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, 0);
	int ll_end = layout_line->start_index+layout_line->length + ((line_count==1) ? 1 : 0);
	
	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int x_sub = 0;
	cat_log_debug("cur_x=%d, layout_line->start_index=%d, ll_end=%d", cur_x, layout_line->start_index, ll_end);
	if (cur_x>=layout_line->start_index && cur_x<=ll_end) {
		/* we are on the first (sub)line of the pango-layout we need to move to the last line of the preceding pango-layout */
		cat_log_debug("page_line_index=%d", page_line_index);
		if (page_line_index>0) {
			/* we are not on the first line of the current page thus we can stay on this page */
			page_line_index--;
			cat_unref_ptr(line);
			line = cha_page_wo_line_at(page, page_line_index);
		} else if (page_index>0) {
			page_index--;
			page = cha_revision_wo_page_at(e_revision, page_index);
			page_line_index = cha_page_wo_line_count(page)-1;
			cat_unref_ptr(line);
			line = cha_page_wo_line_at(page, page_line_index);
		} else {
			/* we are on the first sub-line of the first line on the first page. We can not move up! */
			cat_unref_ptr(line);
		}

		cat_log_debug("line=%p", line);
		if (line!=NULL) {

			ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
			cha_line_location_wo_set_page_index(e_line_location, page_index);
			cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
			ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);


			PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
			cha_line_layout_lock(a_line_layout, TRUE);
			cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
			cha_line_layout_update(a_line_layout, pango_context, color_map);
			PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);

			int line_count = pango_layout_get_line_count(pango_layout);
			PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_count-1);
			int trailing;
			if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
				cur_x = layout_line->start_index+layout_line->length;
				if (!view_ctx.limit_cursor) {
					int xpp = 0;
					pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
					xpp = marked_x_cursor - xpp;
					x_sub = (int) round(xpp/view_ctx.ps_space_width);
					if (x_sub<0) {
						x_sub = 0;
					}
				}
			} else {
				cur_x+=trailing;
			}

			cat_log_debug("marked-x=%d, cur_x=%d, trailing=%d", marked_x_cursor, cur_x, trailing);
			cha_line_layout_unlock(a_line_layout);
			cat_unref_ptr(a_line_layout);
		}
	} else {
		/* we are not on the first line */

		int line_count = pango_layout_get_line_count(pango_layout);
		cat_log_debug("line_count=%d", line_count);
		int line_idx;
		for(line_idx=1; line_idx<line_count; line_idx++) {
			PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
			ll_end = layout_line->start_index+layout_line->length + ((line_count==line_idx) ? 1 : 0);
			cat_log_debug("[%d] cur_x=%d, layout_line=%d/%d, ll_end=%d", line_idx, cur_x, layout_line->start_index, layout_line->length, ll_end);
			if (cur_x>=layout_line->start_index && cur_x<=ll_end) {
				PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx-1);
				int trailing;
				if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
					cur_x = layout_line->start_index+layout_line->length-1;

					if (!view_ctx.limit_cursor) {
						int xpp = 0;
						pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
						xpp = marked_x_cursor - xpp;
						x_sub = (int) round(xpp/view_ctx.ps_space_width);
						if (x_sub<0) {
							x_sub = 0;
						}
					}

				} else {
					cur_x += trailing;
				}
				break;
			}
		}
	}

	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x);
	cha_cursor_wo_set_x_sub(e_cursor, x_sub);
	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);
}



static void l_next_row(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);
	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);

	int marked_x_cursor = cha_document_view_get_marked_layout_x_cursor(document_view)*PANGO_SCALE;

	int line_count = pango_layout_get_line_count(pango_layout);
	PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_count-1);
	int ll_end = layout_line->start_index+layout_line->length + ((line_count==1) ? 1 : 0);
	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int x_sub = 0;

	cat_log_debug("cur_x=%d, layout_line->start_index=%d, ll_end=%d, page_index=%d, page_line_idx=%d", cur_x, layout_line->start_index, ll_end, page_index, page_line_index);
	if (cur_x>=layout_line->start_index && cur_x<=ll_end) {
		/* we are on the last (sub)line of the pango-layout we need to move to the first line of the next pango-layout */
		if (page_line_index<cha_page_wo_line_count(page)-1) {
			/* we are not on the first line of the current page thus we can stay on this page */
			page_line_index++;
			cat_unref_ptr(line);
			line = cha_page_wo_line_at(page, page_line_index);
		} else if (page_index<cha_revision_wo_page_count(e_revision)-1) {
			page_index++;
			page = cha_revision_wo_page_at(e_revision, page_index);
			page_line_index = 0;
			cat_unref_ptr(line);
			line = cha_page_wo_line_at(page, page_line_index);
		} else {
			/* we are on the first sub-line of the first line on the first page. We can not move up! */
			cat_unref_ptr(line);
		}

		cat_log_debug("page_index=%d, page_line_idx=%d, line=%o", page_index, page_line_index, line);


		if (line!=NULL) {

			ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
			cha_line_location_wo_set_page_index(e_line_location, page_index);
			cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
			ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
			cat_log_debug("a_line_layout=%o", a_line_layout);
			cha_line_layout_lock(a_line_layout, TRUE);
			cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
			cha_line_layout_update(a_line_layout, pango_context, color_map);

			PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);
			cat_log_debug("pango_layout=%o", pango_layout);
//			int line_count = pango_layout_get_line_count(pango_layout);
			PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, 0);
			int trailing;
			if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
				cur_x = layout_line->start_index+layout_line->length;
				int line_count = pango_layout_get_line_count(pango_layout);
				if (line_count>1) {
					cur_x--;
				}
				if (!view_ctx.limit_cursor) {
					int xpp = 0;
					pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
					xpp = marked_x_cursor - xpp;
					x_sub = (int) round(xpp/view_ctx.ps_space_width);
					if (x_sub<0) {
						x_sub = 0;
					}
				}

			} else {
				cur_x += trailing;
			}
			cha_line_layout_unlock(a_line_layout);
			cat_unref_ptr(a_line_layout);
		}
	} else {

		int line_count = pango_layout_get_line_count(pango_layout);
		int line_idx;
		for(line_idx=0; line_idx<line_count-1; line_idx++) {
			PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
			ll_end = layout_line->start_index+layout_line->length + ((line_count==1) ? 1 : 0);
			if (cur_x>=layout_line->start_index && cur_x<=ll_end) {
				PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx+1);
				int trailing;
				if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
					cur_x = layout_line->start_index+layout_line->length;
					int line_count = pango_layout_get_line_count(pango_layout);
					if (line_idx+2!=line_count) {
						cur_x--;
					}
					if (!view_ctx.limit_cursor) {
						int xpp = 0;
						pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
						xpp = marked_x_cursor - xpp;
						x_sub = (int) round(xpp/view_ctx.ps_space_width);
						if (x_sub<0) {
							x_sub = 0;
						}
					}

				} else {
					cur_x += trailing;
				}
				break;
			}
		}
	}

	cha_cursor_wo_set_x_sub(e_cursor, x_sub);
	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x);
	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);
}

static void l_begin_of_line(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);
	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);

	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int sub_line;
	pango_layout_index_to_line_x(pango_layout, cur_x, FALSE, &sub_line, NULL);
	PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, sub_line);
	cur_x = layout_line->start_index;

	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x);
	cha_cursor_wo_set_x_sub(e_cursor, 0);
	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);
}

static void l_end_of_line(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);
	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);

	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int sub_line;
	pango_layout_index_to_line_x(pango_layout, cur_x, FALSE, &sub_line, NULL);
	PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, sub_line);
	cur_x = layout_line->start_index+layout_line->length;

	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x);
	cha_cursor_wo_set_x_sub(e_cursor, 0);
	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);
}

static void l_begin_of_document(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
	cha_line_location_wo_set_page_index(e_line_location, 0);
	cha_line_location_wo_set_page_line_index(e_line_location, 0);
	cha_cursor_wo_set_x_cursor_bytes(e_cursor, 0);
	cha_cursor_wo_set_x_sub(e_cursor, 0);
}


static void l_end_of_document(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
	int page_idx = cha_revision_wo_page_count(e_revision)-1;
	cha_line_location_wo_set_page_index(e_line_location, page_idx);
	ChaPageWo *a_page = cha_revision_wo_page_at(e_revision, page_idx);
	int page_line_idx = cha_page_wo_line_count(a_page)-1;
	cha_line_location_wo_set_page_line_index(e_line_location, page_line_idx);
	ChaLineWo *line = cha_page_wo_line_at(a_page, page_line_idx);
	CatStringWo *text = cha_line_wo_get_text(line);
	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cat_string_wo_length(text));
	cha_cursor_wo_set_x_sub(e_cursor, 0);
	cat_unref_ptr(line);
}

static void l_page_down(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);

	int page_count = cha_revision_wo_page_count(e_revision);

	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	cha_page_wo_hold_lines(page);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);

	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);
	int line_count = pango_layout_get_line_count(pango_layout);
	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int x_sub = 0;

	int line_idx;
	for(line_idx=0; line_idx<line_count; line_idx++) {
		PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
		int ll_end = layout_line->start_index+layout_line->length;
		if (line_idx<line_count-1) {
			ll_end--;
		}
		if (cur_x>=layout_line->start_index && cur_x<=ll_end) {
			break;
		}
	}
	cat_log_debug("line_idx=%d", line_idx);

	int marked_x_cursor = cha_document_view_get_marked_layout_x_cursor(document_view)*PANGO_SCALE;

	int visible_lines = cha_document_view_get_visible_lines(document_view)-1;
	int visible_lines2 = visible_lines;
	if (line_idx+visible_lines<line_count) {
		line_idx+=visible_lines;
		visible_lines = -1;
		PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
		int trailing;
		if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
			cur_x = layout_line->start_index+layout_line->length;
			if (line_idx+1!=line_count) {
				cur_x--;
			}
			if (!view_ctx.limit_cursor) {
				int xpp = 0;
				pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
				xpp = marked_x_cursor - xpp;
				x_sub = (int) round(xpp/view_ctx.ps_space_width);
				if (x_sub<0) {
					x_sub = 0;
				}
			}
		} else {
			cur_x += trailing;
		}
	} else {
		page_line_index++;
		visible_lines = visible_lines - (line_count-line_idx);
	}

	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);



	while(visible_lines>=0) {
		cat_log_detail("visible_lines=%d, page=%p, page_index=%d, a_line_layout=%p, page_line_index=%d", visible_lines, page, page_index, a_line_layout, page_line_index);
		if (page==NULL) {
			if (page_index>=page_count) {
				l_end_of_document(move_cursor, document_view, e_revision);
				return;
			} else {
				page = cha_revision_wo_page_at(e_revision, page_index);
				cha_page_wo_hold_lines(page);
				page_line_index = 0;
			}
		} else if (a_line_layout==NULL) {
			if (page_line_index<cha_page_wo_line_count(page)) {
				cat_unref_ptr(line);
				line = cha_page_wo_line_at(page, page_line_index);
				a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
				cha_line_layout_lock(a_line_layout, TRUE);
				cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
				cha_line_layout_update(a_line_layout, pango_context, color_map);
				pango_layout = cha_line_layout_get_pango_layout(a_line_layout);
//				line_count = pango_layout_get_line_count(pango_layout);
			} else {
				cha_page_wo_release_lines(page);
				page = NULL;
				page_index++;
			}
		} else {
			line_count = pango_layout_get_line_count(pango_layout);
			cat_log_trace("visible_lines=%d, line_count=%d", visible_lines, line_count);
			if (visible_lines>=line_count) {
				visible_lines-=line_count;
				page_line_index++;
			} else {
				int line_idx = visible_lines;
				PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
				int trailing;
				if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
					cur_x = layout_line->start_index+layout_line->length;
					if (line_idx+1!=line_count) {
						cur_x--;
					}
					if (!view_ctx.limit_cursor) {
						int xpp = 0;
						pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
						xpp = marked_x_cursor - xpp;
						x_sub = (int) round(xpp/view_ctx.ps_space_width);
						if (x_sub<0) {
							x_sub = 0;
						}
					}
				} else {
					cur_x += trailing;
				}
				visible_lines = -1;
			}
			cha_line_layout_unlock(a_line_layout);
			cat_unref_ptr(a_line_layout);
			cat_unref_ptr(line);
		}
	}

	if (a_line_layout) {
		cha_line_layout_unlock(a_line_layout);
		cat_unref_ptr(a_line_layout);
	}
	cat_unref_ptr(line);

	if (page) {
		cha_page_wo_release_lines(page);
	}

	ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
	cha_line_location_wo_set_page_index(e_line_location, page_index);
	cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x);
	cha_cursor_wo_set_x_sub(e_cursor, x_sub);

	cha_document_view_move_view_vertical(document_view, visible_lines2);
}

static void l_page_up(ChaUowMoveCursor *move_cursor, ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_revision_wo_page_at(e_revision, page_index);
	cha_page_wo_hold_lines(page);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	ChaLineLayout *a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);

	cha_line_layout_lock(a_line_layout, TRUE);
	cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
	cha_line_layout_update(a_line_layout, pango_context, color_map);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(a_line_layout);
	int line_count = pango_layout_get_line_count(pango_layout);
	int cur_x = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	int x_sub = 0;

	int line_idx;
	for(line_idx=0; line_idx<line_count; line_idx++) {
		PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
		int ll_end = layout_line->start_index+layout_line->length;
		if (line_idx==line_count-1) {
			ll_end++;
		}
		if (cur_x>=layout_line->start_index && cur_x<ll_end) {
			break;
		}
	}
	cat_log_debug("line_idx=%d", line_idx);

	int marked_x_cursor = cha_document_view_get_marked_layout_x_cursor(document_view)*PANGO_SCALE;

	int visible_lines = cha_document_view_get_visible_lines(document_view)-1;
	int visible_lines2 = visible_lines;
	if (visible_lines<line_idx) {
		line_idx-=visible_lines;
		visible_lines = -1;
		PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
		int trailing;
		if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
			cur_x = layout_line->start_index+layout_line->length;
			if (line_idx+1!=line_count) {
				cur_x--;
			}
			if (!view_ctx.limit_cursor) {
				int xpp = 0;
				pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
				xpp = marked_x_cursor - xpp;
				x_sub = (int) round(xpp/view_ctx.ps_space_width);
				if (x_sub<0) {
					x_sub = 0;
				}
			}
		} else {
			cur_x += trailing;
		}
	} else {
		page_line_index--;
		visible_lines = visible_lines - (line_idx+1);
	}

	cha_line_layout_unlock(a_line_layout);
	cat_unref_ptr(a_line_layout);
	cat_unref_ptr(line);


	while(visible_lines>=0) {
		if (page==NULL) {
			if (page_index<0) {
				l_begin_of_document(move_cursor, document_view, e_revision);
				return;
			} else {
				page = cha_revision_wo_page_at(e_revision, page_index);
				cha_page_wo_hold_lines(page);
				page_line_index = cha_page_wo_line_count(page)-1;
			}
		} else if (a_line_layout==NULL) {
			if (page_line_index>=0) {
				cat_unref_ptr(line);
				line = cha_page_wo_line_at(page, page_line_index);
				a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
				cha_line_layout_lock(a_line_layout, TRUE);
				cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version);
				cha_line_layout_update(a_line_layout, pango_context, color_map);
				pango_layout = cha_line_layout_get_pango_layout(a_line_layout);
				line_count = pango_layout_get_line_count(pango_layout);
			} else {
				cha_page_wo_release_lines(page);
				page = NULL;
				page_index--;
			}
		} else {
			int line_count = pango_layout_get_line_count(pango_layout);
			if (visible_lines>=line_count) {
				visible_lines-=line_count;
				page_line_index--;
			} else {
				int line_idx = (line_count-1)-visible_lines;
				PangoLayoutLine *layout_line = pango_layout_get_line_readonly(pango_layout, line_idx);
				int trailing;
				if (!pango_layout_line_x_to_index(layout_line, marked_x_cursor, &cur_x, &trailing)) {
					cur_x = layout_line->start_index+layout_line->length;
					if (line_idx+1!=line_count) {
						cur_x--;
					}
					if (!view_ctx.limit_cursor) {
						int xpp = 0;
						pango_layout_line_index_to_x(layout_line, cur_x, TRUE, &xpp);
						xpp = marked_x_cursor - xpp;
						x_sub = (int) round(xpp/view_ctx.ps_space_width);
						if (x_sub<0) {
							x_sub = 0;
						}
					}
				} else {
					cur_x += trailing;
				}
				visible_lines = -1;
			}
			cha_line_layout_unlock(a_line_layout);
			cat_unref_ptr(a_line_layout);
			cat_unref_ptr(line);
		}
	}


	if (a_line_layout) {
		cha_line_layout_unlock(a_line_layout);
		cat_unref_ptr(a_line_layout);
	}
	cat_unref_ptr(line);

	if (page) {
		cha_page_wo_release_lines(page);
	}

	ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_cursor);
	cha_line_location_wo_set_page_index(e_line_location, page_index);
	cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cur_x);
	cha_cursor_wo_set_x_sub(e_cursor, x_sub);

	cha_document_view_move_view_vertical(document_view, -visible_lines2);
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowMoveCursor *move_cursor = CHA_UOW_MOVE_CURSOR(self);
	ChaUowMoveCursorPrivate *priv = cha_uow_move_cursor_get_instance_private((ChaUowMoveCursor *) self);
	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);

	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	ChaSelection *selection = NULL;
	if (priv->extend_selection) {
		selection = cha_document_view_get_or_create_selection(document_view, e_revision, cha_revision_wo_get_cursor(e_revision), priv->alt_pressed);
	} else {
		cha_document_view_clear_selection(document_view, e_revision);
		cha_document_view_notify_selection_changed(document_view);
	}

	switch(priv->kind) {
		case CHA_MOVE_CURSOR_NEXT_COLUMN :
			l_next_column(move_cursor, document_view, e_revision, FALSE);
			break;
		case CHA_MOVE_CURSOR_PREV_COLUMN :
			l_prev_column(move_cursor, document_view, e_revision, FALSE);
			break;
		case CHA_MOVE_CURSOR_NEXT_COLUMN_WORD :
			l_next_column(move_cursor, document_view, e_revision, TRUE);
			break;
		case CHA_MOVE_CURSOR_PREV_COLUMN_WORD :
			l_prev_column(move_cursor, document_view, e_revision, TRUE);
			break;
		case CHA_MOVE_CURSOR_UP :
			l_prev_row(move_cursor, document_view, e_revision);
			priv->mark_cursor = FALSE;
			break;
		case CHA_MOVE_CURSOR_DOWN :
			l_next_row(move_cursor, document_view, e_revision);
			priv->mark_cursor = FALSE;
			break;
		case CHA_MOVE_CURSOR_BEGIN_OF_LINE :
			l_begin_of_line(move_cursor, document_view, e_revision);
			break;
		case CHA_MOVE_CURSOR_END_OF_LINE :
			l_end_of_line(move_cursor, document_view, e_revision);
			break;
		case CHA_MOVE_CURSOR_BEGIN_OF_DOCUMENT :
			l_begin_of_document(move_cursor, document_view, e_revision);
			break;
		case CHA_MOVE_CURSOR_END_OF_DOCUMENT :
			l_end_of_document(move_cursor, document_view, e_revision);
			break;
		case CHA_MOVE_CURSOR_PAGE_DOWN :
			l_page_down(move_cursor, document_view, e_revision);
			priv->mark_cursor = FALSE;
			break;
		case CHA_MOVE_CURSOR_PAGE_UP :
			l_page_up(move_cursor, document_view, e_revision);
			priv->mark_cursor = FALSE;
			break;
	}

	cat_log_detail("finished");


	if (selection) {
		cha_selection_set_end(selection, cha_revision_wo_get_cursor(e_revision));
		cha_document_view_notify_selection_changed(document_view);
	}

	if (priv->mark_cursor) {
		ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
		cha_document_view_mark_layout_x_cursor(document_view, e_revision, e_cursor);
	}

	if (!is_editable) {
		cat_log_detail("anchoring document");
		cha_document_anchor_document(document);
	}

	cat_log_detail("move view to focus");
	cha_document_view_move_view_to_focus(document_view, FALSE);
	cat_log_detail("end");
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowMoveCursor *instance = CHA_UOW_MOVE_CURSOR(self);
	ChaUowMoveCursorPrivate *priv = cha_uow_move_cursor_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p kind=%d, extend=%d]", iname, self, priv->kind, priv->extend_selection);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
