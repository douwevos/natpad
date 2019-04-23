/*
   File:    chaplainselection.c
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

#include "chaplainselection.h"
#include "../layout/chadocumentview.h"
#include "chaselectionprivate.h"
#include "../layout/chapangoattrholder.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPlainSelection"
#include <logging/catlog.h>

struct _ChaPlainSelectionPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPlainSelection, cha_plain_selection, CHA_TYPE_SELECTION,
		G_ADD_PRIVATE(ChaPlainSelection)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_selection_apply(ChaSelection *selection, PangoContext *pango_context, ChaLineWo *a_line, ChaLineLayout *e_line_layout, int page_index, int line_index);
static CatStringWo *l_selection_get_as_text(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only);
static CatHashMapWo *l_selection_get_for_clipboard(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only);
static ChaCursorWo *l_selection_delete(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision);
static void l_selection_change_case(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, gboolean to_upper);

static void cha_plain_selection_class_init(ChaPlainSelectionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;


	ChaSelectionClass *selection_class = CHA_SELECTION_CLASS(clazz);
	selection_class->apply = l_selection_apply;
	selection_class->getAsText = l_selection_get_as_text;
	selection_class->getForClipboard = l_selection_get_for_clipboard;
	selection_class->delete = l_selection_delete;
	selection_class->changeCase = l_selection_change_case;
}

static void cha_plain_selection_init(ChaPlainSelection *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_plain_selection_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_plain_selection_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPlainSelection *cha_plain_selection_new(CatAtomicInteger *sequence, ChaCursorWo *cursor) {
	ChaPlainSelection *result = g_object_new(CHA_TYPE_PLAIN_SELECTION, NULL);
	cat_ref_anounce(result);
	cha_selection_construct((ChaSelection *) result, sequence, cursor);
	return result;
}



static void l_selection_apply(ChaSelection *selection, PangoContext *pango_context, ChaLineWo *a_line, ChaLineLayout *e_line_layout, int page_index, int line_index) {
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);
	if (cha_line_layout_get_selection_version(e_line_layout) == prot->modification_count) {
		return;
	}

	CatArrayWo *selection_attrs = NULL;
	gboolean selection_eol = FALSE;

	cat_log_trace("top=%d/%d/%d, bottom=%d/%d/%d, page_index=%d, line_index=%d", prot->top_page_index, prot->top_page_line_index, prot->top_x_cursor_bytes, prot->bottom_page_index, prot->bottom_page_line_index, prot->bottom_x_cursor_bytes, page_index, line_index);

	if (((page_index>prot->top_page_index) || (page_index==prot->top_page_index && line_index>=prot->top_page_line_index))
			&& ((page_index<prot->bottom_page_index) || (page_index==prot->bottom_page_index && line_index<=prot->bottom_page_line_index))) {
		int start = 0;
		int end = cha_line_wo_byte_count(a_line);
		if (page_index==prot->top_page_index && line_index==prot->top_page_line_index) {
			start = prot->top_x_cursor_bytes;
		}
		if (page_index==prot->bottom_page_index && line_index==prot->bottom_page_line_index) {
			end = prot->bottom_x_cursor_bytes;
		} else {
			selection_eol = TRUE;
		}

		cat_log_trace("start=%d, end=%d, e_line_layout=%p", start, end, e_line_layout);

		selection_attrs = cat_array_wo_new();
		PangoAttribute *pango_attr = pango_attr_foreground_new(0xFFFF,0xFFFF,0xFFFF);
		pango_attr->start_index = start;
		pango_attr->end_index = end;
		ChaPangoAttrHolder *holder = cha_pango_attr_holder_new(pango_attr);
		cat_array_wo_append(selection_attrs, (GObject *) holder);
		cat_unref_ptr(holder);
		pango_attr = pango_attr_background_new(0x0,0x0,0x0);
		pango_attr->start_index = start;
		pango_attr->end_index = end;
		holder = cha_pango_attr_holder_new(pango_attr);
		cat_array_wo_append(selection_attrs, (GObject *) holder);
		cat_unref_ptr(holder);
	}

	cha_line_layout_set_selection(e_line_layout, selection_attrs, selection_eol, NULL, prot->modification_count);
}

static CatStringWo *l_selection_get_as_text(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only) {
	CatStringWo *buf = cat_string_wo_new();
	ChaSelectionPrivate *priv = cha_selection_get_protected(selection);
	ChaDocument *doc = cha_document_view_get_document(document_view);
	ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(doc);
	ChaLineEnd line_ends = cha_revision_wo_get_line_ends(a_rev);
	gboolean line_ends_are_mixed = cha_revision_wo_get_line_ends_are_mixed(a_rev);
	ChaPageWo *a_page = NULL;
	if ((priv->top_page_index==priv->bottom_page_index) && (priv->top_page_line_index==priv->bottom_page_line_index)) {

		a_page = cha_revision_wo_page_at(a_rev, priv->top_page_index);
		ChaPageWoClass *page_class = CHA_PAGE_WO_GET_CLASS(a_page);
		cha_page_wo_hold_lines(a_page);
		const ChaUtf8Text utf8_text = page_class->utf8At(a_page, priv->top_page_line_index, FALSE);

		int ilen = priv->bottom_x_cursor_bytes - priv->top_x_cursor_bytes;
		if (ilen>0) {
			cat_string_wo_append_chars_len(buf, utf8_text.text+priv->top_x_cursor_bytes, ilen);
		}
		cha_utf8_text_cleanup(&utf8_text);

	} else {
		int page_index = priv->top_page_index;
		int page_line_index = priv->top_page_line_index;
		int page_line_count;
		ChaPageWoClass *page_class = NULL;
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

				if (page_line_index==priv->top_page_line_index && page_index==priv->top_page_index) {
					int s = utf8_text.text_len-priv->top_x_cursor_bytes;
					if (s>0) {
						cat_string_wo_append_chars_len(buf, utf8_text.text+priv->top_x_cursor_bytes, s);
					}
				} else if (page_line_index==priv->bottom_page_line_index && page_index==priv->bottom_page_index) {
					if (priv->bottom_x_cursor_bytes>0) {
						cat_string_wo_append_chars_len(buf, utf8_text.text, priv->bottom_x_cursor_bytes);
					}
					cha_utf8_text_cleanup(&utf8_text);
					break;
				} else {
					cat_string_wo_append_chars_len(buf, utf8_text.text, utf8_text.text_len);
				}
				ChaLineEnd line_end = utf8_text.line_end;
				if (line_end!=CHA_LINE_END_NONE && !line_ends_are_mixed) {
					line_end = line_ends;
				}
				switch(line_end) {
					case CHA_LINE_END_NL :
						cat_string_wo_append_char(buf, 0x15);
						break;
					case CHA_LINE_END_CR :
						cat_string_wo_append_char(buf, 0xD);
						break;
					case CHA_LINE_END_LF :
						cat_string_wo_append_char(buf, 0xA);
						break;
					case CHA_LINE_END_CRLF :
						cat_string_wo_append_char(buf, 0xD);
						cat_string_wo_append_char(buf, 0xA);
						break;
					case CHA_LINE_END_LFCR :
						cat_string_wo_append_char(buf, 0xA);
						cat_string_wo_append_char(buf, 0xD);
						break;
					case CHA_LINE_END_NONE :
						break;
				}
				page_line_index++;
				cha_utf8_text_cleanup(&utf8_text);
			} else {
				a_page = cha_revision_wo_page_at(a_rev, page_index);
				page_class = CHA_PAGE_WO_GET_CLASS(a_page);
				cha_page_wo_hold_lines(a_page);
				page_line_count = cha_page_wo_line_count(a_page);
			}
		}
	}
	if (a_page) {
		cha_page_wo_release_lines(a_page);
	}
	cat_unref_ptr(a_rev);
	return buf;
}


static CatHashMapWo *l_selection_get_for_clipboard(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only) {
	CatStringWo *plain_text_buf = cat_string_wo_new();
	CatStringWo *richt_text_buf = NULL;

	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_full_mode = !cha_document_is_big_file_mode(document);

	ChaDocumentViewClass *dv_class = NULL;

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



	ChaSelectionPrivate *priv = cha_selection_get_protected(selection);
	ChaDocument *doc = cha_document_view_get_document(document_view);
	ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(doc);
	ChaPageWo *a_page = NULL;
	if ((priv->top_page_index==priv->bottom_page_index) && (priv->top_page_line_index==priv->bottom_page_line_index)) {

		a_page = cha_revision_wo_page_at(a_rev, priv->top_page_index);
		ChaPageWoClass *page_class = CHA_PAGE_WO_GET_CLASS(a_page);
		cha_page_wo_hold_lines(a_page);
		const ChaUtf8Text utf8_text = page_class->utf8At(a_page, priv->top_page_line_index, FALSE);

		int ilen = priv->bottom_x_cursor_bytes - priv->top_x_cursor_bytes;
		if (ilen>0) {
			cat_string_wo_append_chars_len(plain_text_buf, utf8_text.text+priv->top_x_cursor_bytes, ilen);

			ChaLineWo *a_line = page_class->lineRefAt(a_page, priv->top_page_line_index);
			if (dv_class) {
				CatStringWo *rc_text = dv_class->createRichText(document_view, a_page, a_line, priv->top_x_cursor_bytes, priv->top_x_cursor_bytes+ilen);
				if (rc_text) {
					cat_string_wo_append(richt_text_buf, rc_text);
					cat_unref_ptr(rc_text);
				}
			}
			cat_unref_ptr(a_line);
		}
		cha_utf8_text_cleanup(&utf8_text);

	} else {
		ChaLineEnd line_ends = cha_revision_wo_get_line_ends(a_rev);
		gboolean line_ends_are_mixed = cha_revision_wo_get_line_ends_are_mixed(a_rev);

		int page_index = priv->top_page_index;
		int page_line_index = priv->top_page_line_index;
		int page_line_count;
		ChaPageWoClass *page_class = NULL;
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
					cat_log_trace("a_line_layout=%d, a_line=%p", a_line);
				}


				if (page_line_index==priv->top_page_line_index && page_index==priv->top_page_index) {
					int s = utf8_text.text_len-priv->top_x_cursor_bytes;
					if (s>0) {
						cat_string_wo_append_chars_len(plain_text_buf, utf8_text.text+priv->top_x_cursor_bytes, s);
						if (dv_class) {
							CatStringWo *rc_text = dv_class->createRichText(document_view, a_page, a_line, priv->top_x_cursor_bytes, priv->top_x_cursor_bytes+s);
							if (rc_text) {
								cat_string_wo_append(richt_text_buf, rc_text);
								cat_unref_ptr(rc_text);
							}
						}
					}
				} else if (page_line_index==priv->bottom_page_line_index && page_index==priv->bottom_page_index) {
					if (priv->bottom_x_cursor_bytes>0) {
						cat_string_wo_append_chars_len(plain_text_buf, utf8_text.text, priv->bottom_x_cursor_bytes);

						if (dv_class) {
							CatStringWo *rc_text = dv_class->createRichText(document_view, a_page, a_line, 0, priv->bottom_x_cursor_bytes);
							if (rc_text) {
								cat_string_wo_append(richt_text_buf, rc_text);
								cat_unref_ptr(rc_text);
							}
						}

					}
					cat_unref_ptr(a_line);
					cha_utf8_text_cleanup(&utf8_text);
					break;
				} else {
					cat_string_wo_append_chars_len(plain_text_buf, utf8_text.text, utf8_text.text_len);
					if (dv_class) {
						CatStringWo *rc_text = dv_class->createRichText(document_view, a_page, a_line, 0, utf8_text.text_len);
						if (rc_text) {
							cat_string_wo_append(richt_text_buf, rc_text);
							cat_unref_ptr(rc_text);
						}
					}
				}
				ChaLineEnd line_end = utf8_text.line_end;
				if (line_end!=CHA_LINE_END_NONE && !line_ends_are_mixed) {
					line_end = line_ends;
				}
				switch(line_end) {
					case CHA_LINE_END_NL :
						cat_string_wo_append_char(plain_text_buf, 0x15);
						break;
					case CHA_LINE_END_CR :
						cat_string_wo_append_char(plain_text_buf, 0xD);
						break;
					case CHA_LINE_END_LF :
						cat_string_wo_append_char(plain_text_buf, 0xA);
						break;
					case CHA_LINE_END_CRLF :
						cat_string_wo_append_char(plain_text_buf, 0xD);
						cat_string_wo_append_char(plain_text_buf, 0xA);
						break;
					case CHA_LINE_END_LFCR :
						cat_string_wo_append_char(plain_text_buf, 0xA);
						cat_string_wo_append_char(plain_text_buf, 0xD);
						break;
					case CHA_LINE_END_NONE :
						break;
				}
				if (richt_text_buf && line_end!=CHA_LINE_END_NONE) {
					cat_string_wo_append_chars(richt_text_buf, "\n\\par ");
				}

				page_line_index++;
				cat_unref_ptr(a_line);
				cha_utf8_text_cleanup(&utf8_text);
			} else {
				a_page = cha_revision_wo_page_at(a_rev, page_index);
				page_class = CHA_PAGE_WO_GET_CLASS(a_page);
				cha_page_wo_hold_lines(a_page);
				page_line_count = cha_page_wo_line_count(a_page);
			}
		}
	}
	if (a_page) {
		cha_page_wo_release_lines(a_page);
	}


	cat_log_debug("plain_text_buf=%o", plain_text_buf);

	CatHashMapWo *result = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	cat_hash_map_wo_put(result, (GObject *) CAT_S(cha_selection_cd_plain_text), (GObject *) plain_text_buf);
	if (richt_text_buf) {
		cat_string_wo_append_chars(richt_text_buf, "\n}}");
		cat_log_debug("richt_text_buf=%o", richt_text_buf);
		cat_hash_map_wo_put(result, (GObject *) CAT_S(cha_selection_cd_rich_text), (GObject *) richt_text_buf);
	}
	cat_unref_ptr(richt_text_buf);
	cat_unref_ptr(plain_text_buf);
	cat_unref_ptr(a_rev);
	return result;
}



void l_get_first_last(ChaSelection *selection, ChaCursorWo **first_cursor, ChaCursorWo **last_cursor) {
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);
	ChaCursorWo *a_top = prot->a_start;
	ChaCursorWo *a_bottom = prot->a_end;
	ChaLineLocationWo *a_location = cha_cursor_wo_get_line_location(a_top);
	int top_page_idx = cha_line_location_wo_get_page_index(a_location);
	int top_page_line_idx = cha_line_location_wo_get_page_line_index(a_location);
	int top_x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(a_top);

	a_location = cha_cursor_wo_get_line_location(a_bottom);
	int bottom_page_idx = cha_line_location_wo_get_page_index(a_location);
	int bottom_page_line_idx = cha_line_location_wo_get_page_line_index(a_location);
	int bottom_x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(a_bottom);

	if ((bottom_page_idx<top_page_idx)
			|| ((bottom_page_idx==top_page_idx) && (bottom_page_line_idx<top_page_line_idx))
			|| ((bottom_page_idx==top_page_idx) && (bottom_page_line_idx==top_page_line_idx) && (bottom_x_cursor_bytes<top_x_cursor_bytes))
			) {
		a_top = prot->a_end;
		a_bottom = prot->a_start;
	}

	*first_cursor = a_top;
	*last_cursor = a_bottom;
}

static ChaCursorWo *l_selection_delete(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaCursorWo *first_cursor = NULL;
	ChaCursorWo *last_cursor = NULL;

	l_get_first_last(selection, (ChaCursorWo **) (&first_cursor), (ChaCursorWo **) (&last_cursor));

	cat_log_debug("first_cursor=%o, last_cursor=%o", first_cursor, last_cursor);

	cha_revision_wo_set_cursor(e_revision, first_cursor);
	cha_revision_wo_remove(e_revision, last_cursor);
	return cha_cursor_wo_clone(first_cursor, CAT_CLONE_DEPTH_FULL);
}

static void l_selection_change_case(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, gboolean to_upper) {
	ChaSelectionPrivate *prot = cha_selection_get_protected(selection);

	int page_index = prot->top_page_index;
	int page_line_index = prot->top_page_line_index;
	int page_line_count;
	ChaPageWo *e_page = NULL;
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

			gboolean is_selection_top = (page_line_index==prot->top_page_line_index && page_index==prot->top_page_index);
			gboolean is_selection_bottom = (page_line_index==prot->bottom_page_line_index && page_index==prot->bottom_page_index);

			const ChaUtf8Text utf8_text = page_class->utf8At(e_page, page_line_index, FALSE);

			ChaLineMultiReplace *line_multiple_replace = NULL;
			if (utf8_text.text_len>0) {

				int r_offset = 0;
				int r_length = utf8_text.text_len;
				if (is_selection_top) {
					r_offset = prot->top_x_cursor_bytes;
					if (is_selection_bottom) {
						r_length = prot->bottom_x_cursor_bytes-r_offset;
					} else {
						r_length = utf8_text.text_len-r_offset;
					}
				} else if (is_selection_bottom) {
					r_length = prot->bottom_x_cursor_bytes;
				}

				gchar *rep = NULL;
				if (to_upper) {
					rep = g_utf8_strup(utf8_text.text+r_offset, r_length);
				} else {
					rep = g_utf8_strdown(utf8_text.text+r_offset, r_length);
				}
				CatStringWo *replacement = cat_string_wo_new_nocopy(rep, strlen(rep));

				line_multiple_replace = cha_line_multi_replace_new();
				cha_line_multi_replace_append(line_multiple_replace, replacement, r_offset, r_offset+r_length);
			}

			cha_utf8_text_cleanup(&utf8_text);

			if (line_multiple_replace) {
				int left_cur_x = 0;
				cha_page_wo_line_multi_replace(e_page, page_line_index, line_multiple_replace, &left_cur_x, NULL);
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
//	ChaPlainSelection *instance = CHA_PLAIN_SELECTION(self);
//	ChaPlainSelectionPrivate *priv = cha_plain_selection_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(e_append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

