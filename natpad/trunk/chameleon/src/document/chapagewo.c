/*
   File:    chapagewo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 24, 2015
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

#include "chapagewoprivate.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPageWo"
#include <logging/catlog.h>

struct _ChaPageWoPrivate {
	int enriched_count;
	ChaEnrichmentData *volatile enrichment_data;
	CatLock *lock;
	CatBitArrayWo *mark_list;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPageWo, cha_page_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaPageWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);

static void l_page_enrich(ChaPageWo *page, ChaEnrichmentDataMapWo *a_map);
static void l_page_impoverish(ChaPageWo *page);
static void l_page_enrichment_remap(ChaPageWo *page, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void cha_page_wo_class_init(ChaPageWoClass *clazz) {
	clazz->enrich = l_page_enrich;
	clazz->impoverish = l_page_impoverish;
	clazz->enrichmentRemap = l_page_enrichment_remap;
	clazz->holdLines = NULL;
	clazz->releaseLines = NULL;

	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_page_wo_init(ChaPageWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPageWo *instance = CHA_PAGE_WO(object);
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(instance);
	cat_unref_ptr(priv->enrichment_data);
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->mark_list);
	G_OBJECT_CLASS(cha_page_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_page_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cha_page_wo_construct(ChaPageWo *page, gboolean editable) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	cat_wo_construct((CatWo *) page, editable);
	priv->enriched_count = 0;
	priv->enrichment_data = NULL;
	priv->lock = cat_lock_new();
	priv->mark_list = NULL;
}


gboolean cha_page_wo_write_to_stream(ChaPageWo *page, ChaWriteReq *write_req) {
	return CHA_PAGE_WO_GET_CLASS(page)->writeToStream(page, write_req);
}

static char let[] = { 13, 10, 13 };

gboolean cha_page_wo_write_single_line(ChaPageWo *page, ChaWriteReq *write_req, const char *txt_data, int txt_len, ChaLineEnd line_end) {
	gsize written;
	GOutputStream *out_stream = write_req->out_stream;

	ChaConvertRequest request;
	request.text = txt_data;
	request.text_length = txt_len;
	request.output = NULL;
	request.forward_conversion = FALSE;
	cha_iconverter_convert(write_req->charset_converter, &request);

	const char *con_txt = cat_string_wo_getchars(request.output);
	int con_txt_len = cat_string_wo_length(request.output);

	if (!g_output_stream_write_all(out_stream, con_txt, con_txt_len, &written, NULL, (GError **) &(write_req->error))) {
		return FALSE;
	}

	if (line_end==CHA_LINE_END_NONE) {
		return TRUE;
	}

	if (write_req->force_line_end!=CHA_LINE_END_NONE) {
		line_end = write_req->force_line_end;
	}


	gboolean result = TRUE;
	switch(line_end) {
		case CHA_LINE_END_CR :
			result = g_output_stream_write_all(out_stream, let, 1, &written, NULL, (GError **) &(write_req->error));
			break;
		case CHA_LINE_END_CRLF :
			result = g_output_stream_write_all(out_stream, let, 2, &written, NULL, (GError **) &(write_req->error));
			break;
		case CHA_LINE_END_LF :
			result = g_output_stream_write_all(out_stream, let+1, 1, &written, NULL, (GError **) &(write_req->error));
			break;
		case CHA_LINE_END_LFCR :
			result = g_output_stream_write_all(out_stream, let+1, 2, &written, NULL, (GError **) &(write_req->error));
			break;
		case CHA_LINE_END_NONE :
			break;
	}
	return result;
}

gboolean cha_page_wo_is_line_marked(ChaPageWo *page, int line_index) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	if (priv->mark_list) {
		return cat_bit_array_wo_get(priv->mark_list, line_index);
	}
	return FALSE;
}

void cha_page_wo_set_line_mark(ChaPageWo *e_page, int line_index) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(e_page);
	if (priv->mark_list==NULL) {
		priv->mark_list = cat_bit_array_wo_new();
	}
	int bal = cat_bit_array_wo_length(priv->mark_list);
	if (bal<line_index+1) {
		cat_bit_array_wo_insert(priv->mark_list, bal, (line_index+1)-bal);
	}
	cat_bit_array_wo_set(priv->mark_list, line_index);
}

void cha_page_wo_unset_line_mark(ChaPageWo *e_page, int line_index) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(e_page);
	if (priv->mark_list==NULL) {
		priv->mark_list = cat_bit_array_wo_new();
	}
	int bal = cat_bit_array_wo_length(priv->mark_list);
	if (bal<line_index+1) {
		cat_bit_array_wo_insert(priv->mark_list, bal, (line_index+1)-bal);
	}
	cat_bit_array_wo_unset(priv->mark_list, line_index);
}

void cha_page_wo_toggle_line_mark(ChaPageWo *e_page, int line_index) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(e_page);
	if (priv->mark_list==NULL) {
		priv->mark_list = cat_bit_array_wo_new();
	}
	int bal = cat_bit_array_wo_length(priv->mark_list);
	if (bal<line_index+1) {
		cat_bit_array_wo_insert(priv->mark_list, bal, (line_index+1)-bal);
	}
	cat_bit_array_wo_toggle(priv->mark_list, line_index);
}

void cha_page_wo_unmark_all_lines(ChaPageWo *e_page) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(e_page);
	cat_unref_ptr(priv->mark_list);
}

void cha_page_wo_mark_all_lines(ChaPageWo *e_page) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(e_page);
	if (priv->mark_list==NULL) {
		priv->mark_list = cat_bit_array_wo_new();
	}
	int line_cnt = cha_page_wo_line_count(e_page);
	int bal = cat_bit_array_wo_length(priv->mark_list);
	if (bal<line_cnt) {
		cat_bit_array_wo_insert(priv->mark_list, bal, (line_cnt)-bal);
	}
	cat_bit_array_wo_set_all(priv->mark_list);
}

void cha_page_wo_mark_toggle_all_lines(ChaPageWo *e_page) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(e_page);
	if (priv->mark_list==NULL) {
		cha_page_wo_mark_all_lines(e_page);
		return;
	}
	int line_cnt = cha_page_wo_line_count(e_page);
	int bal = cat_bit_array_wo_length(priv->mark_list);
	if (bal<line_cnt) {
		cat_bit_array_wo_insert(priv->mark_list, bal, (line_cnt)-bal);
	}
	cat_bit_array_wo_invert(priv->mark_list);
}


gboolean cha_page_wo_has_marked_lines(const ChaPageWo *page) {
	const ChaPageWoPrivate *priv = cha_page_wo_get_instance_private((ChaPageWo *) page);
	return priv->mark_list!=NULL;
}

void cha_scan_lines(char *text, char *end, ChaScannedLine cha_scanned_line, void *data) {

	char *off_13 = (char *) memchr(text, 0xd, end-text);
	char *off_10 = (char *) memchr(text, 0xa, end-text);

	char *off_last = text;
	gboolean keep_running = TRUE;
	while(keep_running) {
		if (off_13==NULL) {
			if (off_10==NULL) {
				if (off_last<end) {
					cha_scanned_line(off_last, end, CHA_LINE_END_NONE, data);
				}
				break;
			}

			keep_running = cha_scanned_line(off_last, off_10, CHA_LINE_END_LF, data);
			off_last = off_10+1;
			off_10 = (char *) memchr(off_last, 0xa, end-off_last);
		} else if (off_10==NULL) {
			keep_running = cha_scanned_line(off_last, off_13, CHA_LINE_END_CR, data);
			off_last = off_13+1;
			off_13 = (char *) memchr(off_last, 0xd, end-off_last);
		} else {
			if (off_10<off_13) {
				if (off_10+1==off_13) {
					keep_running = cha_scanned_line(off_last, off_10, CHA_LINE_END_LFCR, data);
					off_last = off_13+1;
					off_10 = (char *) memchr(off_last, 0xa, end-off_last);
					off_13 = (char *) memchr(off_last, 0xd, end-off_last);
				} else {
					keep_running = cha_scanned_line(off_last, off_10, CHA_LINE_END_LF, data);
					off_last = off_10+1;
					off_10 = (char *) memchr(off_last, 0xa, end-off_last);
				}
			} else {
				if (off_13+1==off_10) {
					keep_running = cha_scanned_line(off_last, off_13, CHA_LINE_END_CRLF, data);
					off_last = off_10+1;
					off_10 = (char *) memchr(off_last, 0xa, end-off_last);
					off_13 = (char *) memchr(off_last, 0xd, end-off_last);
				} else {
					keep_running = cha_scanned_line(off_last, off_13, CHA_LINE_END_CR, data);
					off_last = off_13+1;
					off_13 = (char *) memchr(off_last, 0xd, end-off_last);
				}
			}
		}
	}
}

int cha_page_wo_get_enrichment_count(ChaPageWo *page) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	return priv->enriched_count;
}

CatLock *cha_page_wo_get_lock(ChaPageWo *page) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	return priv->lock;
}


static void l_page_enrich(ChaPageWo *page, ChaEnrichmentDataMapWo *a_map) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	if (priv->enriched_count==0) {
		if (priv->enrichment_data==NULL) {
			priv->enrichment_data = cha_enrichment_data_new_lock(a_map, priv->lock);
		}
	}
	priv->enriched_count++;
	cat_log_debug("page=%o", page);
}

void cha_page_wo_enrich(ChaPageWo *page, ChaEnrichmentDataMapWo *a_map) {
	CHA_PAGE_WO_GET_CLASS(page)->enrich(page, a_map);
}


static void l_page_impoverish(ChaPageWo *page) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	if (priv->enriched_count==1) {
		cat_unref_ptr(priv->enrichment_data);
	}
	priv->enriched_count--;
}

void cha_page_wo_impoverish(ChaPageWo *page) {
	CHA_PAGE_WO_GET_CLASS(page)->impoverish(page);
}


static void l_page_enrichment_remap(ChaPageWo *page, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	ChaEnrichmentData *enrichment_data = priv->enrichment_data;
	cat_log_debug("page=%p, remap:enrichment_data=%p", page, enrichment_data);
	if (enrichment_data) {
		cha_enrichment_data_remap(enrichment_data, a_old_map, a_new_map, add_idx, rem_idx);
	}
}

void cha_page_wo_enrichment_remap(ChaPageWo *page, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx) {
	CHA_PAGE_WO_GET_CLASS(page)->enrichmentRemap(page, a_old_map, a_new_map, add_idx, rem_idx);
}


GObject *cha_page_wo_get_slot_content_ref(ChaPageWo *page, int slot_index, GObject *slot_key) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	GObject *result = NULL;
	ChaEnrichmentData *enrichment_data = priv->enrichment_data;
	if (enrichment_data) {
		result = cha_enrichment_data_get_slot_content_ref(enrichment_data, slot_index, slot_key);
	}
	return result;
}

void cha_page_wo_set_slot_content(ChaPageWo *page, int slot_index, GObject *slot_key, GObject *content) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(page);
	ChaEnrichmentData *enrichment_data = priv->enrichment_data;
	cat_log_debug("enrichment_data=%p, page=%p, slot_index=%d, slot_key=%p, content=%o", enrichment_data, page, slot_index, slot_key, content);
	if (enrichment_data) {
		cha_enrichment_data_set_slot_content(enrichment_data, slot_index, slot_key, content);
	}
}




#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_page)) { \
			cat_log_error("Object is read only:%o", e_page); \
			return rval; \
		} \



void cha_page_wo_add_line(ChaPageWo *e_page, ChaLineWo *line) {
	CHA_PAGE_WO_GET_CLASS(e_page)->addLine(e_page, line);
}

void cha_page_wo_insert_line(ChaPageWo *e_page, ChaLineWo *line, int index) {
	CHA_PAGE_WO_GET_CLASS(e_page)->insertLine(e_page, line, index);
}

void cha_page_wo_remove_range(ChaPageWo *e_page, int first, int last) {
	CHA_PAGE_WO_GET_CLASS(e_page)->removeRange(e_page, first, last);
}

ChaLineWo *cha_page_wo_editable_line_at(ChaPageWo *e_page, int index) {
	return CHA_PAGE_WO_GET_CLASS(e_page)->editableLineAt(e_page, index);
}

int cha_page_wo_line_count(ChaPageWo *page) {
	return CHA_PAGE_WO_GET_CLASS(page)->lineCount(page);
}

ChaLineWo *cha_page_wo_line_at(ChaPageWo *page, int index) {
	return CHA_PAGE_WO_GET_CLASS(page)->lineRefAt(page, index);
}

const ChaUtf8Text cha_page_wo_utf8_at(ChaPageWo *page, int line_index, gboolean do_scan) {
	return CHA_PAGE_WO_GET_CLASS(page)->utf8At(page, line_index, do_scan);
}

void cha_utf8_text_cleanup(const ChaUtf8Text *text) {
	ChaUtf8Text *textm = (ChaUtf8Text *) text;
	if (text->text_needs_cleanup) {
		textm->text_needs_cleanup = FALSE;


		cat_free_ptr(textm->text);
	}
}


void cha_page_wo_hold_lines(ChaPageWo *page) {
	ChaPageWoClass *page_class = CHA_PAGE_WO_GET_CLASS(page);
	if (page_class->holdLines) {
		CHA_PAGE_WO_GET_CLASS(page)->holdLines(page);
	}
}

void cha_page_wo_release_lines(ChaPageWo *page) {
	ChaPageWoClass *page_class = CHA_PAGE_WO_GET_CLASS(page);
	if (page_class->holdLines) {
		CHA_PAGE_WO_GET_CLASS(page)->releaseLines(page);
	}
}



void cha_page_wo_line_multi_replace(ChaPageWo *e_page, int line_index, ChaLineMultiReplace *line_multi_replace, int *left_result, int *right_result) {
	CHECK_IF_WRITABLE();

	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(e_page, line_index, FALSE);
	CatStringWo *e_buf = cat_string_wo_new_with_len(NULL, utf8_text.text_len);

	int last_out = 0;
	int last_offset = 0;
	int replace_idx;
	int replace_count = cha_line_multi_replace_entry_count(line_multi_replace);
	for(replace_idx=0; replace_idx<replace_count; replace_idx++) {
		const ChaLineMultiReplaceEntry *entry = cha_line_multi_replace_entry_at(line_multi_replace, replace_idx);
		int next_offset = entry->start_offset;
		cat_log_debug("last_offset=%d, next_offset=%d", last_offset, next_offset);
		if (last_offset!=next_offset) {
			cat_string_wo_append_chars_len(e_buf, utf8_text.text+last_offset, next_offset-last_offset);
		}
		if (left_result && replace_idx==0) {
			*left_result = cat_string_wo_length(e_buf);
		}
		if (entry->replacement) {
			cat_string_wo_append(e_buf, entry->replacement);
		}
		cat_log_debug("entry->replacement=%o", entry->replacement);
		last_out = cat_string_wo_length(e_buf);
		last_offset = entry->end_offset;
		cat_log_debug("last_offset=%d",last_offset);
	}

	if ((last_offset>0) || (last_out>0)) {
		if (right_result) {
			*right_result = last_out;
		}
		if (last_offset<utf8_text.text_len) {
			cat_log_debug("append rest:left=%d", source_line_length-last_offset);
			cat_string_wo_append_chars_len(e_buf, utf8_text.text+last_offset, utf8_text.text_len-last_offset);
		}

		cat_log_debug("buf=%o", e_buf);

		cha_page_wo_hold_lines(e_page);

		ChaLineWo *e_line = cha_page_wo_editable_line_at(e_page, line_index);
		cha_line_wo_set_text(e_line, e_buf);
		cha_page_wo_release_lines(e_page);
	} else {
		if (left_result) {
			*left_result = 0;
		}
		if (right_result) {
			*right_result = utf8_text.text_len;
		}
		cat_unref_ptr(e_buf);
	}
	cha_utf8_text_cleanup(&utf8_text);
}


static  CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {

	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PAGE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private((ChaPageWo *) e_uninitialized);
	if (original) {
		ChaPageWoPrivate *rpriv = cha_page_wo_get_instance_private((ChaPageWo *) original);
		priv->enrichment_data = cat_ref_ptr(rpriv->enrichment_data);
		priv->lock = cat_ref_ptr(rpriv->lock);
		priv->mark_list = cat_bit_array_wo_create_editable(rpriv->mark_list);
		cat_log_debug("HIER2 !!!!!!!!!!!!!!!!!!!: %o original=%p", rpriv->mark_list, original);
	} else {
		priv->enrichment_data = NULL;
		priv->mark_list = NULL;
	}
	priv->enriched_count = 0;
	cat_log_debug("makr_list=%o", priv->mark_list);
	return CAT_WO_CLASS(cha_page_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private((ChaPageWo *) wo);
	cat_log_debug("priv->mark_list=%o", priv->mark_list);
	if (priv->mark_list && !cat_bit_array_wo_is_anchored(priv->mark_list)) {
		if (cat_bit_array_wo_has_at_least_one_set(priv->mark_list)) {
			priv->mark_list = cat_bit_array_wo_anchor(priv->mark_list, version);
		} else {
			cat_unref_ptr(priv->mark_list);
		}
	}
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	}
	if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	ChaPageWoPrivate *priv_a = cha_page_wo_get_instance_private((ChaPageWo *) wo_a);
	ChaPageWoPrivate *priv_b = cha_page_wo_get_instance_private((ChaPageWo *) wo_b);
	return cat_bit_array_wo_equal(priv_a->mark_list, priv_b->mark_list);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PAGE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(CHA_PAGE_WO(e_uninitialized));
	if (wo_source) {
		ChaPageWoPrivate *priv_src = cha_page_wo_get_instance_private(CHA_PAGE_WO(wo_source));
		priv->enrichment_data = cat_ref_ptr(priv_src->enrichment_data);
		priv->lock = cat_ref_ptr(priv_src->lock);
		priv->enriched_count = 0;
		priv->mark_list = cat_bit_array_wo_clone(priv_src->mark_list, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->enriched_count = 0;
		priv->enrichment_data = NULL;
		priv->lock = cat_lock_new();
		priv->mark_list = NULL;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_page_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

CAT_WO_BASE_C(ChaPageWo, cha_page_wo)

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPageWo *instance = CHA_PAGE_WO(self);
	ChaPageWoPrivate *priv = cha_page_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s enr-cnt=%d, enr-data=%o, mark_list=%o]", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->enriched_count, priv->enrichment_data, priv->mark_list);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
