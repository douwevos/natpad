/*
   File:    chammappagewo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Apr 23, 2015
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

#include "chammappagewo.h"
#include "../chapagewoprivate.h"
#include <woo/catwoprivate.h>
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ERROR
#define CAT_LOG_CLAZZ "ChaMMapPageWo"
#include <logging/catlog.h>

enum _ChaMappedAs {
	CHA_MAPPED_AS_RAW,
	CHA_MAPPED_AS_LINE,
	CHA_MAPPED_AS_MODIFIED_LINE,

};

typedef enum _ChaMappedAs ChaMappedAs;

struct _ChaLineDescr {
	void *mappedData;
	int length;
	ChaLineEnd line_end;
	ChaMappedAs mappedAs;
};


struct _ChaMMapPageWoPrivate {
	ChaMMap *map;
	gsize offset;
	gsize length;
	short map_line_count;
	short line_count;
	struct _ChaLineDescr *lines;
	gboolean hold;
	short lines_hold_cnt;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaMMapPageWo, cha_mmap_page_wo, CHA_TYPE_PAGE_WO,
		G_ADD_PRIVATE(ChaMMapPageWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static gboolean l_page_write_to_stream(ChaPageWo *page, ChaWriteReq *write_req);
static void l_page_hold_lines(ChaPageWo *page);
static void l_page_release_lines(ChaPageWo *page);
static int l_page_line_count(ChaPageWo *page);
static ChaLineWo *l_page_line_ref_at(ChaPageWo *page, int line_index);
static ChaLineWo *l_page_editable_line_at(ChaPageWo *e_page, int index);

static const ChaUtf8Text l_page_utf8_at(ChaPageWo *page, int line_index, gboolean do_scan);
static void l_page_add_line(ChaPageWo *e_page, ChaLineWo *line);
static void l_page_remove_range(ChaPageWo *e_page, int first, int last);
static void l_page_insert_line(ChaPageWo *e_page, ChaLineWo *line, int index);


static void cha_mmap_page_wo_class_init(ChaMMapPageWoClass *clazz) {
	ChaPageWoClass *page_wo_class = CHA_PAGE_WO_CLASS(clazz);
	page_wo_class->writeToStream = l_page_write_to_stream;
	page_wo_class->holdLines = l_page_hold_lines;
	page_wo_class->releaseLines = l_page_release_lines;
	page_wo_class->lineCount = l_page_line_count;
	page_wo_class->lineRefAt = l_page_line_ref_at;
	page_wo_class->utf8At = l_page_utf8_at;
	page_wo_class->addLine = l_page_add_line;
	page_wo_class->insertLine = l_page_insert_line;
	page_wo_class->removeRange = l_page_remove_range;
	page_wo_class->editableLineAt = l_page_editable_line_at;



	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_mmap_page_wo_init(ChaMMapPageWo *instance) {
}

static void l_free_lines(ChaMMapPageWoPrivate *priv) {
	if (priv->lines) {
		int idx;
		for(idx=priv->line_count-1; idx>=0; idx--) {
			struct _ChaLineDescr *cld = priv->lines+idx;
			if (cld->mappedAs==CHA_MAPPED_AS_LINE || cld->mappedAs==CHA_MAPPED_AS_MODIFIED_LINE) {
				cat_unref_ptr(cld->mappedData);
			}
		}
		cat_free_ptr(priv->lines);
	}
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaMMapPageWo *instance = CHA_MMAP_PAGE_WO(object);
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private(instance);
	cat_unref_ptr(priv->map);
	l_free_lines(priv);
	G_OBJECT_CLASS(cha_mmap_page_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_mmap_page_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ChaMMapPageWo *cha_mmap_page_wo_new(ChaMMap *map, gsize offset, gsize length, short line_count) {
	ChaMMapPageWo *result = g_object_new(CHA_TYPE_MMAP_PAGE_WO, NULL);
	cat_ref_anounce(result);
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private(result);
	cha_page_wo_construct((ChaPageWo *) result, FALSE);
	priv->map = cat_ref_ptr(map);
	priv->offset = offset;
	priv->length = length;
	priv->map_line_count = line_count;
	priv->line_count = line_count;
	priv->hold = FALSE;
	priv->lines = NULL;
	priv->lines_hold_cnt = 0;
	return result;
}

static gboolean l_scanned_line_enlist(char *off_line_start, char *off_line_end, ChaLineEnd line_end, void *data) {
	ChaMMapPageWoPrivate *priv = (ChaMMapPageWoPrivate *) data;
	struct _ChaLineDescr *ldscr = priv->lines+priv->lines_hold_cnt;
//	cat_log_trace("lines_hold_cnt=%d, line_count=%d", (int) priv->lines_hold_cnt, (int) priv->line_count);
	ldscr->mappedData = off_line_start;
	ldscr->line_end = line_end;
	ldscr->length = (off_line_end-off_line_start);
	ldscr->mappedAs = CHA_MAPPED_AS_RAW;
	priv->lines_hold_cnt++;
	return TRUE;
}

static void l_build_lines(ChaMMapPageWo *page) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private(page);
	priv->lines = g_malloc(sizeof(struct _ChaLineDescr) * (priv->line_count+1));
	cat_log_trace("lines=%p, cnt=%d", priv->lines, (priv->line_count+1));

	char *start = (char *) cha_mmap_get_data(priv->map) + priv->offset;
	char *end = start+priv->length;

	cat_log_debug("start=%p, end=%p", start, end);

	int olhc = priv->lines_hold_cnt;
	priv->lines_hold_cnt = 0;
	cha_scan_lines(start, end, l_scanned_line_enlist, priv);
	cat_log_debug("nr-of-lines-scanned:%d", priv->lines_hold_cnt);
	priv->lines_hold_cnt = olhc;

	cat_log_debug("build-lines:page=%p", page);
}

static gboolean l_page_write_to_stream(ChaPageWo *page, ChaWriteReq *write_req) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) page);
	gboolean force_per_line_write = write_req->needs_conversion;
	CatLock *lock = cha_page_wo_get_lock((ChaPageWo *) page);
	gboolean did_hold = FALSE;
	cat_lock_lock(lock);
	if (force_per_line_write) {
		l_build_lines((ChaMMapPageWo *) page);
	}
	if (priv->lines) {
		priv->lines_hold_cnt++;
		did_hold = TRUE;
	}
	cat_lock_unlock(lock);

	gboolean result = TRUE;
	gsize written = 0;

	if (priv->lines) {

		int idx;
		void *txt_data = NULL;
		int txt_len = 0;
		ChaLineEnd line_end;
		for(idx=0; idx<priv->line_count; idx++) {
			struct _ChaLineDescr *cld = priv->lines+idx;
			if (cld->mappedAs==CHA_MAPPED_AS_RAW) {
				txt_data = cld->mappedData;
				txt_len = cld->length;
				line_end = cld->line_end;
			} else {
				ChaLineWo *real_line = (ChaLineWo *) cld->mappedData;
				CatStringWo *real_text = cha_line_wo_get_text(real_line);
				txt_data = (char *) cat_string_wo_getchars(real_text);
				txt_len = cat_string_wo_length(real_text);
				line_end = cha_line_wo_get_line_end(real_line);
			}
			if (line_end!=CHA_LINE_END_NONE && !write_req->line_ends_are_mixed) {
				line_end = write_req->line_ends;
			}


			if (!cha_page_wo_write_single_line(page, write_req, txt_data, txt_len, line_end)) {
				result = FALSE;
				break;
			}
		}

	} else {
		/* we can write the whole mmapped data section to the out_stream */
		char *start = (char *) cha_mmap_get_data(priv->map) + priv->offset;
		result = g_output_stream_write_all(write_req->out_stream, start, priv->length, &written, NULL, (GError **) &(write_req->error));
	}

	if (did_hold) {
		l_page_release_lines(page);
	}
	return result;
}

static void l_page_hold_lines(ChaPageWo *page) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) page);
	CatLock *lock = cha_page_wo_get_lock((ChaPageWo *) page);
	cat_lock_lock(lock);

	cat_log_on_debug({
		if (cat_time_running_in_ms()>30000) {
			cat_log_error("hold: hold-cnt=%d, page=%p", priv->lines_hold_cnt, page);
		}
	});

	if (priv->lines==NULL) {
		l_build_lines((ChaMMapPageWo *) page);
	}
	priv->lines_hold_cnt++;

	cat_lock_unlock(lock);
}

static void l_page_release_lines(ChaPageWo *page) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) page);
	CatLock *lock = cha_page_wo_get_lock((ChaPageWo *) page);
	cat_lock_lock(lock);
	cat_log_on_debug({
		if (cat_time_running_in_ms()>30000) {
			cat_log_error("release: hold-cnt=%d, page=%p", priv->lines_hold_cnt, page);
		}
	});
	if (priv->lines_hold_cnt==1) {
		if (!priv->hold) {
			l_free_lines(priv);
		}
	}
	priv->lines_hold_cnt--;
	cat_lock_unlock(lock);
}


struct l_scanned_line {
	int cnt;
	char *start;
	char *end;
	ChaLineEnd line_end;
};

static gboolean l_scanned_line(char *off_line_start, char *off_line_end, ChaLineEnd line_end, void *data) {
	struct l_scanned_line *scanned_line = (struct l_scanned_line *) data;
	if (scanned_line->cnt==0) {
		scanned_line->start = off_line_start;
		scanned_line->end = off_line_end;
		scanned_line->line_end = line_end;
	}
//	cat_log_trace("scanned_line->cnt=%d, start=%p", scanned_line->cnt, scanned_line->start);
	scanned_line->cnt--;
	return scanned_line->cnt<0 ? FALSE : TRUE;
}


static const ChaUtf8Text l_page_utf8_at(ChaPageWo *page, int line_index, gboolean do_scan) {
	ChaUtf8Text result;
	result.text = NULL;
	result.text_len = 0;
	result.text_needs_cleanup = FALSE;

	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) page);

	char *xtxt = NULL;
	int xlen = 0;
	if (priv->lines) {
		struct _ChaLineDescr *cld = priv->lines+line_index;
		if (line_index>priv->line_count-1) {
			cat_log_debug("line_index=%d, line-count=%d", line_index, priv->line_count);
		}
		if (cld->mappedAs == CHA_MAPPED_AS_RAW) {
			xtxt = cld->mappedData;
			xlen = cld->length;
			result.line_end = cld->line_end;
		} else {
			ChaLineWo *real_line = (ChaLineWo *) cld->mappedData;
			CatStringWo *real_text = cha_line_wo_get_text(real_line);
			xtxt = (char *) cat_string_wo_getchars(real_text);
			xlen = cat_string_wo_length(real_text);
			result.line_end = cha_line_wo_get_line_end(real_line);
		}
	} else {
		cat_log_warn("lines miss ... did you call hold_lines on page ?");
		struct l_scanned_line scanned_line;
		scanned_line.start = NULL;
		scanned_line.cnt = line_index;
		char *start = (char *) cha_mmap_get_data(priv->map) + priv->offset;
		char *end = start+priv->length;

		cat_log_debug("start=%p, end=%p", start, end);

		cha_scan_lines(start, end, l_scanned_line, &scanned_line);


		if (scanned_line.start) {
			xtxt = scanned_line.start;
			xlen = scanned_line.end-scanned_line.start;
			result.line_end = scanned_line.line_end;
		} else {
			return result;
		}
	}


	if (do_scan) {
		unsigned char *ptr = (unsigned char *) xtxt;
		unsigned char *end = ptr+xlen;

		gboolean oa = TRUE;
		gboolean has_tabs = FALSE;
		while((!has_tabs || oa) && ptr<end) {
			unsigned char ch = *ptr;
			if (ch==9) {
				has_tabs = TRUE;
			} else if (ch>127) {
				oa = FALSE;
			}
			ptr++;
		}
		result.only_ascii = oa;
		result.has_tabs = has_tabs;
	}

	result.text = xtxt;
	result.text_len = xlen;
	return result;
}



static int l_page_line_count(ChaPageWo *page) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) page);
	return priv->line_count;
}

static ChaLineWo *l_page_line_ref_at(ChaPageWo *page, int line_index) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) page);
	ChaLineWo *result = NULL;
	CatLock *lock = cha_page_wo_get_lock((ChaPageWo *) page);
	cat_lock_lock(lock);
	if (priv->lines) {
		struct _ChaLineDescr *cld = priv->lines+line_index;
		if (cld->mappedAs == CHA_MAPPED_AS_RAW) {
			result = cha_line_wo_new_anchored(cat_string_wo_new_with_len(cld->mappedData, cld->length), cld->line_end);
			cld->mappedData = result;
			cld->mappedAs = CHA_MAPPED_AS_LINE;
		}
		result = (ChaLineWo *) cat_ref_ptr(cld->mappedData);
	}
	cat_lock_unlock(lock);
	cat_log_on_debug({
		if (cat_time_running_in_ms()>30000) {
			if (line_index==30) {
				cat_log_error("line_index=%d, page=%p, result=%p", line_index, page, result);
			} else {
				cat_log_info("line_index=%d, page=%p, result=%p", line_index, page, result);
			}
		}
	});
	if (result) {
		return result;
	}

	struct l_scanned_line scanned_line;
	scanned_line.start = NULL;
	scanned_line.cnt = line_index;
	char *start = (char *) cha_mmap_get_data(priv->map) + priv->offset;
	char *end = start+priv->length;

	cat_log_debug("start=%p, end=%p", start, end);

	cha_scan_lines(start, end, l_scanned_line, &scanned_line);


	if (scanned_line.start) {
		cat_log_debug("length=%d", (int) (scanned_line.end-scanned_line.start));

		return cha_line_wo_new_with(cat_string_wo_new_with_len(scanned_line.start, (int) (scanned_line.end-scanned_line.start)), scanned_line.line_end);
	}
	return NULL;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_page)) { \
			cat_log_error("Object is read only:%o", e_page); \
			return rval; \
		} \


static ChaLineWo *l_page_editable_line_at(ChaPageWo *e_page, int index) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) e_page);
	CHECK_IF_WRITABLE(NULL)
	ChaLineWo *result = NULL;
	struct _ChaLineDescr *cld = priv->lines+index;
	if (cld->mappedAs == CHA_MAPPED_AS_RAW) {
		result = cha_line_wo_new_with(cat_string_wo_new_with_len(cld->mappedData, cld->length), cld->line_end);
		cld->mappedData = result;
	} else {
		ChaLineWo *line_wo = (ChaLineWo *) cld->mappedData;
		if (cha_line_wo_is_anchored(line_wo)) {
			result = cha_line_wo_create_editable(line_wo);
			cat_ref_swap(cld->mappedData, result);
			cat_unref(result);
		} else {
			result = line_wo;
		}
	}
	cld->mappedAs = CHA_MAPPED_AS_MODIFIED_LINE;
	cat_log_debug("result=%o, mapped=%p, cld=%p", result, cld->mappedData, cld);
	return result;
}

static void l_page_add_line(ChaPageWo *e_page, ChaLineWo *line) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) e_page);
	CHECK_IF_WRITABLE();
	cat_log_debug("e_page=%o", e_page);
	struct _ChaLineDescr *new_lines = g_malloc(sizeof(struct _ChaLineDescr) * (priv->line_count+2));
	cat_log_debug("lines=%p, cnt=%d", new_lines, (priv->line_count+2));
	memcpy(new_lines, priv->lines, sizeof(struct _ChaLineDescr) * (priv->line_count));

	struct _ChaLineDescr *cld = new_lines+priv->line_count;
	cld->mappedAs = CHA_MAPPED_AS_MODIFIED_LINE;
	cld->mappedData = cat_ref_ptr(line);
	cld->length = 0;
	cat_free_ptr(priv->lines);
	priv->lines = new_lines;
	priv->line_count++;
}

static void l_page_remove_range(ChaPageWo *e_page, int first, int last) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) e_page);
	CHECK_IF_WRITABLE();
	if (first>=priv->line_count || last<0) {
		return;
	}
	if (first<0) {
		first = 0;
	}
	if (last>=priv->line_count-1) {
		last=priv->line_count-1;
	}
	if (first>last) {
		return;
	}
	int i;
	for(i=first; i<=last; i++) {
		struct _ChaLineDescr *cld = priv->lines+i;
		if (cld->mappedAs != CHA_MAPPED_AS_RAW) {
			cat_unref_ptr(cld->mappedData);
		}
	}
	last++;
	int rem_count = last-first;
	if (last<priv->line_count) {
		memmove(priv->lines+first, priv->lines+last, (priv->line_count+1-last) * sizeof(struct _ChaLineDescr));
	}
	priv->line_count-=rem_count;
}

static void l_page_insert_line(ChaPageWo *e_page, ChaLineWo *line, int index) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) e_page);
	CHECK_IF_WRITABLE();
	if (index<0) {
		index = 0;
	} else if (index>priv->line_count) {
		index = priv->line_count;
	}

	struct _ChaLineDescr *new_lines = g_malloc(sizeof(struct _ChaLineDescr) * (priv->line_count+2));
	cat_log_debug("lines=%p, cnt=%d", new_lines, (priv->line_count+2));
	memcpy(new_lines, priv->lines, sizeof(struct _ChaLineDescr) * (priv->line_count));

	int rest = priv->line_count-index;
	if (rest>0) {
		memmove(new_lines+index+1, new_lines+index, rest * sizeof(struct _ChaLineDescr));
	}


	struct _ChaLineDescr *cld = new_lines+index;
	cld->mappedAs = CHA_MAPPED_AS_MODIFIED_LINE;
	cld->mappedData = cat_ref_ptr(line);
	cld->length = 0;
	cat_free_ptr(priv->lines);
	priv->lines = new_lines;
	priv->line_count++;
}



static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_MMAP_PAGE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}


	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) e_uninitialized);

	if (original) {
		ChaMMapPageWoPrivate *rpriv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) original);
		priv->map = cat_ref_ptr(rpriv->map);
		priv->hold = TRUE;
		priv->length = rpriv->length;
		priv->map_line_count = rpriv->map_line_count;
		priv->line_count = rpriv->line_count;
		priv->lines_hold_cnt = 0;
		priv->lines = NULL;
		if (rpriv->lines) {
			priv->lines = g_malloc(sizeof(struct _ChaLineDescr) * (priv->line_count+2));
			cat_log_debug("lines=%p, cnt=%d", priv->lines, (priv->line_count));
			memcpy(priv->lines, rpriv->lines, sizeof(struct _ChaLineDescr) * priv->line_count);
			int idx;
			for(idx=priv->line_count-1; idx>=0; idx--) {
				struct _ChaLineDescr *cld = priv->lines+idx;
				if (cld->mappedAs != CHA_MAPPED_AS_RAW) {
					cat_ref_ptr(cld->mappedData);
				}
			}

		} else {
			l_build_lines((ChaMMapPageWo *) e_uninitialized);
		}
		cat_log_debug("original=%o, lines=%p, o-lines=%p, e_uninitialized=%p", original, priv->lines, rpriv->lines, e_uninitialized);
	} else {
//		priv->size = 0;
//		priv->modification_count = 0;
	}
	return CAT_WO_CLASS(cha_mmap_page_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) wo);

	gboolean keep_hold = FALSE;
	/* test if we really need to hold the line list */
	int line_idx = 0;
	char *last = NULL;
	for(line_idx=0; line_idx<priv->line_count; line_idx++) {
		struct _ChaLineDescr *cld = priv->lines+line_idx;
		if (cld->mappedAs != CHA_MAPPED_AS_RAW) {
			cld->mappedData = cha_line_wo_anchor(cld->mappedData, version);
			if (cld->mappedAs == CHA_MAPPED_AS_MODIFIED_LINE) {
				keep_hold = TRUE;
			}
		} else {
			if (((char *) cld->mappedData)<last) {
				keep_hold = TRUE;
			}
			last = (char *) cld->mappedData;
		}
	}
	if (priv->line_count != priv->map_line_count) {
		keep_hold = TRUE;
	}
	priv->hold = keep_hold;
	CAT_WO_CLASS(cha_mmap_page_wo_parent_class)->anchorChildren(wo, version);
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaMMapPageWoPrivate *priv_a = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) wo_a);
	const ChaMMapPageWoPrivate *priv_b = cha_mmap_page_wo_get_instance_private((ChaMMapPageWo *) wo_b);
	if (priv_a->line_count!=priv_b->line_count) {
		return FALSE;
	}


	if (priv_a->lines==priv_b->lines) {
		return TRUE;
	}

	if (priv_a->lines==NULL || priv_b->lines==NULL) {
		return FALSE;
	}
	return memcmp(priv_a->lines, priv_b->lines, priv_a->line_count*sizeof(struct _ChaLineDescr))==0;
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_MMAP_PAGE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private(CHA_MMAP_PAGE_WO(e_uninitialized));
	if (wo_source) {
		ChaMMapPageWoPrivate *priv_src = cha_mmap_page_wo_get_instance_private(CHA_MMAP_PAGE_WO(wo_source));
		priv->map = cat_ref_ptr(priv_src->map);
		priv->hold = priv_src->hold;
		priv->offset = priv_src->offset;
		priv->length = priv_src->length;
		priv->lines_hold_cnt = 0;
		priv->map_line_count = priv_src->map_line_count;
	} else {
		priv->map = NULL;
		priv->offset = 0;
		priv->length = 0;
		priv->map_line_count = 0;
		priv->line_count = 0;
		priv->hold = FALSE;
		priv->lines = NULL;
		priv->lines_hold_cnt = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_mmap_page_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}
CAT_WO_BASE_C(ChaMMapPageWo,cha_mmap_page_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaMMapPageWo *instance = CHA_MMAP_PAGE_WO(self);
	ChaMMapPageWoPrivate *priv = cha_mmap_page_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s, lines=%p(%d), hold=%d, hold_cnt=%d, hc=%d]", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->lines, priv->line_count, priv->hold, priv->lines_hold_cnt, priv->lines_hold_cnt);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

