/*
   File:    chablockselection.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 18, 2015
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

#include "chaselectionprivate.h"
#include "chablockselection.h"
#include "../layout/chadocumentview.h"
#include "../layout/chamarking.h"
#include "../layout/chapangoattrholder.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaBlockSelection"
#include <logging/catlog.h>

struct _ChaBlockSelectionPrivate {
	int left_x_view;
	int right_x_view;
	int start_x_bytes;
	int end_x_bytes;
	int text_layout_width;
	int ps_space_width;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaBlockSelection, cha_block_selection, CHA_TYPE_SELECTION,
		G_ADD_PRIVATE(ChaBlockSelection)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_selection_pre_apply(ChaSelection *selection, ChaLineLayout *e_line_layout_start, ChaLineLayout *e_line_layout_end, int ps_space_width);
static void l_selection_apply(ChaSelection *selection, PangoContext *pango_context, ChaLineWo *a_line, ChaLineLayout *e_line_layout, int page_index, int line_index);
static CatStringWo *l_selection_get_as_text(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only);
static CatHashMapWo *l_selection_get_for_clipboard(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only);
static ChaCursorWo *l_selection_delete(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision);
static void l_selection_change_case(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, gboolean to_upper);

static void cha_block_selection_class_init(ChaBlockSelectionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaSelectionClass *selection_class = CHA_SELECTION_CLASS(clazz);
	selection_class->preApply = l_selection_pre_apply;
	selection_class->apply = l_selection_apply;
	selection_class->getAsText = l_selection_get_as_text;
	selection_class->getForClipboard = l_selection_get_for_clipboard;
	selection_class->delete = l_selection_delete;
	selection_class->changeCase = l_selection_change_case;

}

static void cha_block_selection_init(ChaBlockSelection *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_block_selection_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_block_selection_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaBlockSelection *cha_block_selection_new(CatAtomicInteger *sequence, ChaCursorWo *cursor) {
	ChaBlockSelection *result = g_object_new(CHA_TYPE_BLOCK_SELECTION, NULL);
	cat_ref_anounce(result);
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private(result);
	cha_selection_construct((ChaSelection *) result, sequence, cursor);
	priv->left_x_view = -1;
	priv->right_x_view = -1;
	priv->start_x_bytes = -1;
	priv->end_x_bytes = -1;
	return result;
}


static int l_index_in_line(PangoLayoutLine *pango_line, int view_x, gboolean is_last_line) {
	int index=0;
	int trailing = 0;
	if (!pango_layout_line_x_to_index(pango_line, view_x*PANGO_SCALE, &index, &trailing)) {
		index = pango_line->start_index;
		if (view_x>0) {
			index = pango_line->start_index+pango_line->length;
			if (!is_last_line) {
				index--;
			}
		}
	} else {
		index += trailing;
	}
	cat_log_debug("view_x=%d, index=%d", view_x, index);
	return index;
}

static void l_selection_pre_apply(ChaSelection *selection, ChaLineLayout *e_line_layout_start, ChaLineLayout *e_line_layout_end, int ps_space_width) {
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private((ChaBlockSelection *) selection);
	priv->ps_space_width = ps_space_width;
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);

	int start_x_bytes = cha_cursor_wo_get_x_cursor_bytes(prot->a_start);
	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(e_line_layout_start);

	int text_layout_width = pango_layout_get_width(pango_layout);

	int start_sub_line_idx = 0;
	int start_cursor_x_view = 0;
	pango_layout_index_to_line_x(pango_layout, start_x_bytes, FALSE, &start_sub_line_idx, &start_cursor_x_view);
	start_cursor_x_view += cha_cursor_wo_get_x_sub(prot->a_start)*ps_space_width;


	int end_x_bytes = cha_cursor_wo_get_x_cursor_bytes(prot->a_end);
	pango_layout = cha_line_layout_get_pango_layout(e_line_layout_end);

	int end_sub_line_idx = 0;
	int end_cursor_x_view = 0;
	pango_layout_index_to_line_x(pango_layout, end_x_bytes, FALSE, &end_sub_line_idx, &end_cursor_x_view);
	end_cursor_x_view += cha_cursor_wo_get_x_sub(prot->a_end)*ps_space_width;


	start_cursor_x_view = start_cursor_x_view/PANGO_SCALE;
	end_cursor_x_view = end_cursor_x_view/PANGO_SCALE;

	int left_x_view = start_cursor_x_view;
	int right_x_view = end_cursor_x_view;
	if (left_x_view>right_x_view) {
		left_x_view = end_cursor_x_view;
		right_x_view = start_cursor_x_view;
	}
	cat_log_debug("xpos-left=%d xpos-right=%d", left_x_view, right_x_view);

	if ((priv->left_x_view == left_x_view) && (priv->right_x_view==right_x_view) && (priv->text_layout_width == text_layout_width)
			&& (priv->start_x_bytes == start_x_bytes) && (priv->end_x_bytes == end_x_bytes)) {
		return;
	}

	priv->text_layout_width = text_layout_width;
	priv->left_x_view = left_x_view;
	priv->right_x_view = right_x_view;
	prot->modification_count++;
}


static void l_selection_apply(ChaSelection *selection, PangoContext *pango_context, ChaLineWo *a_line, ChaLineLayout *e_line_layout, int page_index, int line_index) {
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private((ChaBlockSelection *) selection);
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);
	if (cha_line_layout_get_selection_version(e_line_layout) == prot->modification_count) {
		return;
	}


	CatArrayWo *selection_attrs = NULL;
	CatArrayWo *selection_markings = NULL;

	cat_log_trace("top=%d/%d/%d, bottom=%d/%d/%d, page_index=%d, line_index=%d", prot->top_page_index, prot->top_page_line_index, prot->top_x_cursor_bytes, prot->bottom_page_index, prot->bottom_page_line_index, prot->bottom_x_cursor_bytes, page_index, line_index);

	if (((page_index>prot->top_page_index) || (page_index==prot->top_page_index && line_index>=prot->top_page_line_index))
			&& ((page_index<prot->bottom_page_index) || (page_index==prot->bottom_page_index && line_index<=prot->bottom_page_line_index))) {

//
//		int start = 0;
//		int end = cha_line_wo_byte_count(a_line);
//		if (page_index==prot->top_page_index && line_index==prot->top_page_line_index) {
//			start = prot->top_x_cursor_bytes;
//		}
//		if (page_index==prot->bottom_page_index && line_index==prot->bottom_page_line_index) {
//			end = prot->bottom_x_cursor_bytes;
//		}
//
//

		gboolean is_selection_top = (line_index==prot->top_page_line_index && page_index==prot->top_page_index);
		gboolean is_selection_bottom = (line_index==prot->bottom_page_line_index && page_index==prot->bottom_page_index);

		PangoLayout *pango_layout = cha_line_layout_update_pango_layout(e_line_layout, pango_context);

		selection_attrs = cat_array_wo_new();
		selection_markings = cat_array_wo_new();


		int sub_line_idx = 0;
		GSList *lines = pango_layout_get_lines_readonly(pango_layout);
		while(lines) {
			gboolean is_last_sub_line = lines->next==NULL;
			PangoLayoutLine *pango_line = (PangoLayoutLine *) lines->data;
			cat_log_trace("line-handle: top/botton x=%d/%d, pango-line-start/end=%d/%d", prot->top_x_cursor_bytes, prot->bottom_x_cursor_bytes, pango_line->start_index, pango_line->start_index+pango_line->length);

			gboolean should_add = TRUE;
			if (is_selection_top) {
				should_add = pango_line->start_index+pango_line->length >= prot->top_x_cursor_bytes ;
			}
			if (should_add && is_selection_bottom) {
				should_add = pango_line->start_index <= prot->bottom_x_cursor_bytes;
			}

			cat_log_trace("should_add: %d", should_add);

			if (should_add) {
				int l_index = l_index_in_line(pango_line, priv->left_x_view, is_last_sub_line);
				int r_index = l_index_in_line(pango_line, priv->right_x_view, is_last_sub_line);
				cat_log_trace("l_index=%d, r_index=%d", l_index, r_index);

				ChaMarking *marking = cha_marking_new(sub_line_idx, priv->left_x_view, priv->right_x_view, 0.2, CHA_COLOR_BLOCK_SELECTION);
				cat_array_wo_append(selection_markings, (GObject*) marking);
				cat_unref_ptr(marking);


				PangoAttribute *pango_attr = pango_attr_foreground_new(0xFFFF,0xFFFF,0xFFFF);
				pango_attr->start_index = l_index;
				pango_attr->end_index = r_index;
				ChaPangoAttrHolder *holder = cha_pango_attr_holder_new(pango_attr);
				cat_array_wo_append(selection_attrs, (GObject *) holder);
				cat_unref_ptr(holder);
				pango_attr = pango_attr_background_new(0x0,0x0,0x0);
				pango_attr->start_index = l_index;
				pango_attr->end_index = r_index;
				holder = cha_pango_attr_holder_new(pango_attr);
				cat_array_wo_append(selection_attrs, (GObject *) holder);
				cat_unref_ptr(holder);

			}

			lines = lines->next;
			sub_line_idx++;
		}

//		cairo_set_line_width(painter->back_cairo, 0.5);
//		cairo_set_source_rgba(painter->back_cairo, 0.0f, 0.0f, 0.5f, 1.0f);
		if (cat_array_wo_size(selection_markings)>0) {
			if (is_selection_top) {
				ChaMarking *top_mark = (ChaMarking *) cat_array_wo_get_first(selection_markings);
				cha_marking_set_mark_as_top(top_mark);
			}
			if (is_selection_bottom) {
				ChaMarking *bottom_mark = (ChaMarking *) cat_array_wo_get_last(selection_markings);
				cha_marking_set_mark_as_bottom(bottom_mark);
			}
		}
	}

	cha_line_layout_set_selection(e_line_layout, selection_attrs, FALSE, selection_markings, prot->modification_count);
}


static CatStringWo *l_selection_get_as_text(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only) {
	CatStringWo *buf = cat_string_wo_new();
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private((ChaBlockSelection *) selection);
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);
	ChaRevisionWo *a_rev = cha_document_view_get_revision(document_view);
	ChaPageWo *a_page = NULL;
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	PangoLayout *pango_layout = pango_layout_new(pango_context);
	const ChaDocumentViewContext dv_ctx = cha_document_view_get_context(document_view);
	if (dv_ctx.wrap_lines) {
		pango_layout_set_wrap(pango_layout, PANGO_WRAP_WORD);
		pango_layout_set_width(pango_layout, dv_ctx.text_view_width*PANGO_SCALE);
	} else {
		pango_layout_set_width(pango_layout, -1);
	}

	pango_layout_set_tabs(pango_layout, dv_ctx.tab_array);


	int page_index = prot->top_page_index;
	int page_line_index = prot->top_page_line_index;
	int page_line_count;
	ChaPageWoClass *page_class = NULL;
	gboolean should_add_lf = FALSE;
	while(TRUE) {
		if (a_page) {
			if (page_line_index>=page_line_count) {
				page_line_index = 0;
				page_index++;
				if (page_index>=cha_revision_wo_page_count(a_rev)) {
					break;
				}
				cha_page_wo_release_lines(a_page);
				a_page = NULL;
				continue;
			}
			const ChaUtf8Text utf8_text = page_class->utf8At(a_page, page_line_index, FALSE);

			gboolean is_selection_top = (page_line_index==prot->top_page_line_index && page_index==prot->top_page_index);
			gboolean is_selection_bottom = (page_line_index==prot->bottom_page_line_index && page_index==prot->bottom_page_index);

			cat_log_trace("is_selection: top%d, bottom=%d", is_selection_top, is_selection_bottom);

			pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);

			GSList *lines = pango_layout_get_lines_readonly(pango_layout);
			while(lines) {
				gboolean is_last_sub_line = lines->next==NULL;
				PangoLayoutLine *pango_line = (PangoLayoutLine *) lines->data;
				cat_log_trace("line-handle: top/botton x=%d/%d, pango-line-start/end=%d/%d", prot->top_x_cursor_bytes, prot->bottom_x_cursor_bytes, pango_line->start_index, pango_line->start_index+pango_line->length);

				gboolean should_add = TRUE;
				if (is_selection_top) {
					should_add = pango_line->start_index+pango_line->length >= prot->top_x_cursor_bytes ;
				}
				if (should_add && is_selection_bottom) {
					should_add = pango_line->start_index <= prot->bottom_x_cursor_bytes;
				}

				cat_log_trace("should_add: %d", should_add);

				if (should_add) {
					if (should_add_lf) {
						cat_string_wo_append_char(buf, 0xA);
					}
					should_add_lf = TRUE;
					int l_index = l_index_in_line(pango_line, priv->left_x_view, is_last_sub_line);
					int r_index = l_index_in_line(pango_line, priv->right_x_view, is_last_sub_line);
					cat_log_trace("l_index=%d, r_index=%d", l_index, r_index);
					if (r_index>l_index) {
						cat_string_wo_append_chars_len(buf, utf8_text.text+l_index, (r_index-l_index));
					}
				}

				lines = lines->next;
			}

			cha_utf8_text_cleanup(&utf8_text);

			page_line_index++;
			if (is_selection_bottom) {
				break;
			}

		} else {
			a_page = cha_revision_wo_page_at(a_rev, page_index);
			page_class = CHA_PAGE_WO_GET_CLASS(a_page);
			cha_page_wo_hold_lines(a_page);
			page_line_count = cha_page_wo_line_count(a_page);
		}
	}
	if (a_page) {
		cha_page_wo_release_lines(a_page);
	}
	return buf;

}


static CatHashMapWo *l_selection_get_for_clipboard(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only) {
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private((ChaBlockSelection *) selection);
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);

	CatStringWo *plain_text_buf = cat_string_wo_new();
	CatStringWo *richt_text_buf = NULL;

	ChaDocumentViewClass *dv_class = NULL;

	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_full_mode = !cha_document_is_big_file_mode(document);

	if (is_full_mode) {
		dv_class = CHA_DOCUMENT_VIEW_GET_CLASS(document_view);
		if (dv_class->createRichText==NULL) {
			dv_class = NULL;
		}

		richt_text_buf = cat_string_wo_new();

		cat_string_wo_append_chars_len(richt_text_buf, "{", 1);
		cat_string_wo_append_chars(richt_text_buf, "\\rtf1\\ansi\\uc1\\deff0");


		cat_string_wo_append_chars(richt_text_buf, "{\\colortbl");
		ChaPrefsColorMapWo *color_map = cha_document_view_get_color_map(document_view);
		int col_count = cha_prefs_color_map_wo_count(color_map);
		int col_idx;
		for(col_idx = 0; col_idx<col_count; col_idx++) {
			ChaColorEntryInt col = cha_prefs_color_map_wo_get_color_int(color_map, col_idx);
			cat_string_wo_append_chars(richt_text_buf, "\\red");
			cat_string_wo_append_decimal(richt_text_buf, col.red>>8);
			cat_string_wo_append_chars(richt_text_buf, "\\green");
			cat_string_wo_append_decimal(richt_text_buf, col.green>>8);
			cat_string_wo_append_chars(richt_text_buf, "\\blue");
			cat_string_wo_append_decimal(richt_text_buf, col.blue>>8);
			cat_string_wo_append_chars(richt_text_buf, ";");
		}
		cat_string_wo_append_chars(richt_text_buf, "}");
		// f0    select font 0
		// fs20  font size 20
		// fi0   first line indent in twips
		// ql    left aligned
		cat_string_wo_append_chars(richt_text_buf, "{\\f0\\fs20\\fi0\\ql");
	}


	ChaRevisionWo *a_rev = cha_document_view_get_revision(document_view);
	ChaPageWo *a_page = NULL;
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	PangoLayout *pango_layout = pango_layout_new(pango_context);
	const ChaDocumentViewContext dv_ctx = cha_document_view_get_context(document_view);
	if (dv_ctx.wrap_lines) {
		pango_layout_set_wrap(pango_layout, PANGO_WRAP_WORD);
		pango_layout_set_width(pango_layout, dv_ctx.text_view_width*PANGO_SCALE);
	} else {
		pango_layout_set_width(pango_layout, -1);
	}

	pango_layout_set_tabs(pango_layout, dv_ctx.tab_array);


	int page_index = prot->top_page_index;
	int page_line_index = prot->top_page_line_index;
	int page_line_count;
	ChaPageWoClass *page_class = NULL;
	gboolean should_add_lf = FALSE;
	while(TRUE) {
		if (a_page) {
			if (page_line_index>=page_line_count) {
				page_line_index = 0;
				page_index++;
				if (page_index>=cha_revision_wo_page_count(a_rev)) {
					break;
				}
				cha_page_wo_release_lines(a_page);
				a_page = NULL;
				continue;
			}
			const ChaUtf8Text utf8_text = page_class->utf8At(a_page, page_line_index, FALSE);

			ChaLineWo *a_line = NULL;
			if (is_full_mode) {
				a_line = page_class->lineRefAt(a_page, page_line_index);
			}

			gboolean is_selection_top = (page_line_index==prot->top_page_line_index && page_index==prot->top_page_index);
			gboolean is_selection_bottom = (page_line_index==prot->bottom_page_line_index && page_index==prot->bottom_page_index);

			cat_log_trace("is_selection: top%d, bottom=%d", is_selection_top, is_selection_bottom);

			pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);

			GSList *lines = pango_layout_get_lines_readonly(pango_layout);
			while(lines) {
				gboolean is_last_sub_line = lines->next==NULL;
				PangoLayoutLine *pango_line = (PangoLayoutLine *) lines->data;
				cat_log_trace("line-handle: top/botton x=%d/%d, pango-line-start/end=%d/%d", prot->top_x_cursor_bytes, prot->bottom_x_cursor_bytes, pango_line->start_index, pango_line->start_index+pango_line->length);

				gboolean should_add = TRUE;
				if (is_selection_top) {
					should_add = pango_line->start_index+pango_line->length >= prot->top_x_cursor_bytes ;
				}
				if (should_add && is_selection_bottom) {
					should_add = pango_line->start_index <= prot->bottom_x_cursor_bytes;
				}

				cat_log_trace("should_add: %d", should_add);

				if (should_add) {
					if (should_add_lf) {
						cat_string_wo_append_char(plain_text_buf, 0xA);
						if (richt_text_buf) {
							cat_string_wo_append_chars(richt_text_buf, "\n\\par ");
						}
					}
					should_add_lf = TRUE;
					int l_index = l_index_in_line(pango_line, priv->left_x_view, is_last_sub_line);
					int r_index = l_index_in_line(pango_line, priv->right_x_view, is_last_sub_line);
					cat_log_trace("l_index=%d, r_index=%d", l_index, r_index);
					if (r_index>l_index) {
						cat_string_wo_append_chars_len(plain_text_buf, utf8_text.text+l_index, (r_index-l_index));
						if (dv_class) {
							CatStringWo *rc_text = dv_class->createRichText(document_view, a_page, a_line, l_index, r_index);
							if (rc_text) {
								cat_string_wo_append(richt_text_buf, rc_text);
								cat_unref_ptr(rc_text);
							}
						}
					}
				}

				lines = lines->next;
			}

			cha_utf8_text_cleanup(&utf8_text);
			page_line_index++;
			if (is_selection_bottom) {
				break;
			}

		} else {
			a_page = cha_revision_wo_page_at(a_rev, page_index);
			page_class = CHA_PAGE_WO_GET_CLASS(a_page);
			cha_page_wo_hold_lines(a_page);
			page_line_count = cha_page_wo_line_count(a_page);
		}
	}
	if (a_page) {
		cha_page_wo_release_lines(a_page);
	}

	CatHashMapWo *result = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	cat_hash_map_wo_put(result, (GObject *) CAT_S(cha_selection_cd_plain_text), (GObject *) plain_text_buf);
	cat_hash_map_wo_put(result, (GObject *) CAT_S(cha_selection_cd_block_text), (GObject *) plain_text_buf);
	if (richt_text_buf) {
		cat_string_wo_append_chars(richt_text_buf, "\n}}");
		cat_log_debug("richt_text_buf=%o", richt_text_buf);
		cat_hash_map_wo_put(result, (GObject *) CAT_S(cha_selection_cd_rich_text), (GObject *) richt_text_buf);
	}
	return result;
}


static ChaCursorWo *l_selection_delete(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private((ChaBlockSelection *) selection);
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);

	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	PangoLayout *pango_layout = pango_layout_new(pango_context);
	const ChaDocumentViewContext dv_ctx = cha_document_view_get_context(document_view);
	if (dv_ctx.wrap_lines) {
		pango_layout_set_wrap(pango_layout, PANGO_WRAP_WORD);
		pango_layout_set_width(pango_layout, dv_ctx.text_view_width*PANGO_SCALE);
	} else {
		pango_layout_set_width(pango_layout, -1);
	}
	pango_layout_set_tabs(pango_layout, dv_ctx.tab_array);

	ChaPageWoClass *page_class = NULL;
	ChaPageWo *e_page = NULL;

	int page_index = prot->top_page_index;
	int page_line_index = prot->top_page_line_index;
	int page_line_count;

	ChaCursorWo *result = cha_cursor_wo_new();
	ChaLineLocationWo *result_ll = cha_cursor_wo_get_editable_line_location(result);
	cha_cursor_wo_set_x_cursor_bytes(result, prot->top_x_cursor_bytes);


	ChaLineLocationWo *tll = cha_line_location_wo_new_with(page_index, page_line_index);
	long long abs_line_index = cha_revision_wo_calculate_row(e_revision, tll);
	cat_unref_ptr(tll);

	while(TRUE) {
		if (e_page) {
			if (page_line_index>=page_line_count) {
				page_line_index = 0;
				page_index++;
				if (page_index>=cha_revision_wo_page_count(e_revision)) {
					break;
				}
				cha_page_wo_release_lines(e_page);
				e_page = NULL;
				continue;
			}

			gboolean is_selection_bottom = (page_line_index==prot->bottom_page_line_index && page_index==prot->bottom_page_index);
			gboolean is_selection_top = (page_line_index==prot->top_page_line_index && page_index==prot->top_page_index);
			const ChaUtf8Text utf8_text = page_class->utf8At(e_page, page_line_index, FALSE);
			pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
			cha_utf8_text_cleanup(&utf8_text);


			ChaLineMultiReplace *line_multiple_replace = NULL;
			GSList *lines = pango_layout_get_lines_readonly(pango_layout);
			while(lines) {
				gboolean is_last_sub_line = lines->next==NULL;
				PangoLayoutLine *pango_line = (PangoLayoutLine *) lines->data;
				cat_log_trace("line-handle: top/botton x=%d/%d, pango-line-start/end=%d/%d", prot->top_x_cursor_bytes, prot->bottom_x_cursor_bytes, pango_line->start_index, pango_line->start_index+pango_line->length);

				gboolean should_add = TRUE;
				if (is_selection_top) {
					should_add = pango_line->start_index+pango_line->length >= prot->top_x_cursor_bytes ;
				}
				if (should_add && is_selection_bottom) {
					should_add = pango_line->start_index <= prot->bottom_x_cursor_bytes;
				}

				cat_log_trace("should_add: %d", should_add);

				if (should_add) {
					int l_index = l_index_in_line(pango_line, priv->left_x_view, is_last_sub_line);
					int r_index = l_index_in_line(pango_line, priv->right_x_view, is_last_sub_line);
					cat_log_trace("l_index=%d, r_index=%d", l_index, r_index);
					if (r_index>l_index) {
						if (line_multiple_replace == NULL) {
							line_multiple_replace = cha_line_multi_replace_new();
						}
						cha_line_multi_replace_append(line_multiple_replace, NULL, l_index, r_index);
					}
				}

				lines = lines->next;
			}

			if (line_multiple_replace) {
				int left_cur_x = 0;
				cha_revision_wo_line_multi_replace(e_revision, abs_line_index, e_page, page_line_index, line_multiple_replace, &left_cur_x, NULL);
				if (is_selection_top && left_cur_x>=0) {
					cha_line_location_wo_set_page_index(result_ll, page_index);
					cha_line_location_wo_set_page_line_index(result_ll, page_line_index);
					cha_cursor_wo_set_x_cursor_bytes(result, left_cur_x);
				}
				cat_unref_ptr(line_multiple_replace);
			}

			page_line_index++;
			abs_line_index++;
			if (is_selection_bottom) {
				break;
			}

		} else {
			e_page = cha_revision_wo_editable_page_at(e_revision, page_index);
			page_class = CHA_PAGE_WO_GET_CLASS(e_page);
			cha_page_wo_hold_lines(e_page);
			page_line_count = cha_page_wo_line_count(e_page);
		}
	}
	return result;
}

static gboolean l_scanned_line(char *off_line_start, char *off_line_end, ChaLineEnd line_end, void *data) {
	CatStringWo *line = (CatStringWo *) cat_string_wo_new_data_len(off_line_start, off_line_end-off_line_start);
	cat_array_wo_append(CAT_ARRAY_WO(data), (GObject *) line);
	cat_unref_ptr(line);
	return TRUE;
}


ChaCursorWo *cha_block_selection_paste(struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, CatStringWo *text) {
	char *text_start = (char *) cat_string_wo_getchars(text);
	int text_length = cat_string_wo_length(text);
	char *text_end = text_start + text_length;
	int line_cnt = text_length/100;
	if (line_cnt<20) {
		line_cnt = 20;
	}
	CatArrayWo *e_lines = cat_array_wo_new_size(line_cnt);
	cha_scan_lines(text_start, text_end, l_scanned_line, e_lines);

	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	PangoLayout *pango_layout = pango_layout_new(pango_context);
	const ChaDocumentViewContext dv_ctx = cha_document_view_get_context(document_view);
	if (dv_ctx.wrap_lines) {
		pango_layout_set_wrap(pango_layout, PANGO_WRAP_WORD);
		pango_layout_set_width(pango_layout, dv_ctx.text_view_width*PANGO_SCALE);
	} else {
		pango_layout_set_width(pango_layout, -1);
	}

	pango_layout_set_tabs(pango_layout, dv_ctx.tab_array);

	ChaPageWoClass *page_class = NULL;
	ChaPageWo *e_page = NULL;

	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	int x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(e_cursor);
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(e_cursor);

	int page_index = cha_line_location_wo_get_page_index(line_location);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	int page_line_count;

	int marked_x_cursor = -1;

	CatIIterator *iter = cat_array_wo_iterator(e_lines);

	gboolean should_add = FALSE;
	CatStringWo *current_line = NULL;
	if (cat_iiterator_has_next(iter)) {
		current_line = (CatStringWo *) cat_iiterator_next(iter);
	}

	while(current_line!=NULL) {
		if (e_page) {
			if (page_line_index>=page_line_count) {
				page_line_index = 0;
				page_index++;
				if (page_index>=cha_revision_wo_page_count(e_revision)) {
					break;
				}
				cha_page_wo_release_lines(e_page);
				e_page = NULL;
				continue;
			}

			const ChaUtf8Text utf8_text = page_class->utf8At(e_page, page_line_index, FALSE);
			pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
			cha_utf8_text_cleanup(&utf8_text);

			if (marked_x_cursor==-1) {
				PangoRectangle pcursor;
				pango_layout_get_cursor_pos(pango_layout, x_cursor_bytes, &(pcursor), NULL);
				marked_x_cursor = pcursor.x/PANGO_SCALE;
			} else {
				x_cursor_bytes = -1;
			}
			cat_log_detail("x_cursor_bytes=%d, marked_x_cursor=%d", x_cursor_bytes, marked_x_cursor);


			ChaLineMultiReplace *line_multiple_replace = NULL;
			GSList *lines = pango_layout_get_lines_readonly(pango_layout);
			while(lines) {
				gboolean is_last_sub_line = lines->next==NULL;
				PangoLayoutLine *pango_line = (PangoLayoutLine *) lines->data;
				cat_log_detail("line-handle: pango-line-start/end=%d/%d", pango_line->start_index, pango_line->start_index+pango_line->length);

				should_add = should_add || x_cursor_bytes<=pango_line->start_index+pango_line->length;
				if (should_add) {
					int l_index = l_index_in_line(pango_line, marked_x_cursor, is_last_sub_line);
					if (line_multiple_replace == NULL) {
						line_multiple_replace = cha_line_multi_replace_new();
					}
					cha_line_multi_replace_append(line_multiple_replace, current_line, l_index, l_index);
					if (cat_iiterator_has_next(iter)) {
						current_line = (CatStringWo *) cat_iiterator_next(iter);
						cat_log_trace("current_line=%o", current_line);
					} else {
						current_line = NULL;
						break;
					}
				}

				lines = lines->next;
			}

			if (line_multiple_replace) {
				int left_cur_x = 0;
				cha_page_wo_line_multi_replace(e_page, page_line_index, line_multiple_replace, &left_cur_x, NULL);
		//		if (is_selection_top && left_cur_x>=0) {
		//			cha_line_location_wo_set_page_index(result_ll, page_index);
		//			cha_line_location_wo_set_page_line_index(result_ll, page_line_index);
		//			cha_cursor_wo_set_x_cursor_bytes(result, left_cur_x);
		//		}
				cat_unref_ptr(line_multiple_replace);
			}


			page_line_index++;

		} else {
			e_page = cha_revision_wo_editable_page_at(e_revision, page_index);
			page_class = CHA_PAGE_WO_GET_CLASS(e_page);
			cha_page_wo_hold_lines(e_page);
			page_line_count = cha_page_wo_line_count(e_page);
		}
	}

	if (e_page) {
		cha_page_wo_release_lines(e_page);
		e_page = NULL;
	}

	return NULL;
}







static void l_selection_change_case(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, gboolean to_upper) {
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private((ChaBlockSelection *) selection);
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);
	ChaPageWo *e_page = NULL;
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	PangoLayout *pango_layout = pango_layout_new(pango_context);
	const ChaDocumentViewContext dv_ctx = cha_document_view_get_context(document_view);
	if (dv_ctx.wrap_lines) {
		pango_layout_set_wrap(pango_layout, PANGO_WRAP_WORD);
		pango_layout_set_width(pango_layout, dv_ctx.text_view_width*PANGO_SCALE);
	} else {
		pango_layout_set_width(pango_layout, -1);
	}

	pango_layout_set_tabs(pango_layout, dv_ctx.tab_array);


	int page_index = prot->top_page_index;
	int page_line_index = prot->top_page_line_index;
	int page_line_count;
	ChaPageWoClass *page_class = NULL;
	while(TRUE) {
		if (e_page) {
			if (page_line_index>=page_line_count) {
				page_line_index = 0;
				page_index++;
				if (page_index>=cha_revision_wo_page_count(e_revision)) {
					break;
				}
				cha_page_wo_release_lines(e_page);
				e_page = NULL;
				continue;
			}
			const ChaUtf8Text utf8_text = page_class->utf8At(e_page, page_line_index, FALSE);
			gboolean is_selection_top = (page_line_index==prot->top_page_line_index && page_index==prot->top_page_index);
			gboolean is_selection_bottom = (page_line_index==prot->bottom_page_line_index && page_index==prot->bottom_page_index);

			ChaLineWo *current_line = NULL;

			cat_log_trace("is_selection: top%d, bottom=%d", is_selection_top, is_selection_bottom);

			pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);

			ChaLineMultiReplace *line_multiple_replace = NULL;

			GSList *lines = pango_layout_get_lines_readonly(pango_layout);
			while(lines) {
				gboolean is_last_sub_line = lines->next==NULL;
				PangoLayoutLine *pango_line = (PangoLayoutLine *) lines->data;
				cat_log_trace("line-handle: top/botton x=%d/%d, pango-line-start/end=%d/%d", prot->top_x_cursor_bytes, prot->bottom_x_cursor_bytes, pango_line->start_index, pango_line->start_index+pango_line->length);

				gboolean should_add = TRUE;
				if (is_selection_top) {
					should_add = pango_line->start_index+pango_line->length >= prot->top_x_cursor_bytes ;
				}
				if (should_add && is_selection_bottom) {
					should_add = pango_line->start_index <= prot->bottom_x_cursor_bytes;
				}

				cat_log_trace("should_add: %d", should_add);

				if (should_add) {
					int l_index = l_index_in_line(pango_line, priv->left_x_view, is_last_sub_line);
					int r_index = l_index_in_line(pango_line, priv->right_x_view, is_last_sub_line);
					cat_log_trace("l_index=%d, r_index=%d", l_index, r_index);
					if (r_index>l_index) {

						if (line_multiple_replace == NULL) {
							line_multiple_replace = cha_line_multi_replace_new();
						}

						if (current_line==NULL) {
							current_line = page_class->editableLineAt(e_page, page_line_index);
						}

						gchar *rep = NULL;
						if (to_upper) {
							rep = g_utf8_strup(utf8_text.text+l_index, r_index-l_index);
						} else {
							rep = g_utf8_strdown(utf8_text.text+l_index, r_index-l_index);
						}
						CatStringWo *replacement = cat_string_wo_new_nocopy(rep, strlen(rep));

						cha_line_multi_replace_append(line_multiple_replace, replacement, l_index, r_index);
					}
				}

				lines = lines->next;
			}

			cha_utf8_text_cleanup(&utf8_text);

			if (line_multiple_replace) {
				int left_cur_x = 0;
				cha_page_wo_line_multi_replace(e_page, page_line_index, line_multiple_replace, &left_cur_x, NULL);
		//		if (is_selection_top && left_cur_x>=0) {
		//			cha_line_location_wo_set_page_index(result_ll, page_index);
		//			cha_line_location_wo_set_page_line_index(result_ll, page_line_index);
		//			cha_cursor_wo_set_x_cursor_bytes(result, left_cur_x);
		//		}
				cat_unref_ptr(line_multiple_replace);
			}

			page_line_index++;
			if (is_selection_bottom) {
				break;
			}

		} else {
			e_page = cha_revision_wo_editable_page_at(e_revision, page_index);
			page_class = CHA_PAGE_WO_GET_CLASS(e_page);
			cha_page_wo_hold_lines(e_page);
			page_line_count = cha_page_wo_line_count(e_page);
		}
	}
	if (e_page) {
		cha_page_wo_release_lines(e_page);
	}
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	ChaBlockSelection *instance = CHA_BLOCK_SELECTION(self);
	ChaBlockSelectionPrivate *priv = cha_block_selection_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(e_append_to, "%s[%p left=%d, right=%d]", iname, self, priv->left_x_view, priv->right_x_view);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
