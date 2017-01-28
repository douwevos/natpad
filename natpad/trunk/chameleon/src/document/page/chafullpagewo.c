/*
   File:    chafullpagewo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Apr 21, 2015
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

#include "chafullpagewo.h"
#include "../chapagewoprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaFullPageWo"
#include <logging/catlog.h>

struct _ChaFullPageWoPrivate {
	CatArrayWo *line_array;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaFullPageWo, cha_full_page_wo, CHA_TYPE_PAGE_WO,
		G_ADD_PRIVATE(ChaFullPageWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void l_page_enrich(ChaPageWo *page, ChaEnrichmentDataMapWo *a_map);
static void l_page_impoverish(ChaPageWo *page);
static void l_page_enrichment_remap(ChaPageWo *page, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);


static void l_page_write_to_stream(ChaPageWo *page, GOutputStream *out_stream);
static void l_page_add_line(ChaPageWo *e_page, ChaLineWo *line);
static void l_page_insert_line(ChaPageWo *e_page, ChaLineWo *line, int index) ;
static void l_page_remove_range(ChaPageWo *e_page, int first, int last);
static ChaLineWo *l_page_editable_line_at(ChaPageWo *e_page, int index);
static int l_page_line_count(ChaPageWo *page);
static ChaLineWo *l_page_line_ref_at(ChaPageWo *page, int index);
static const ChaUtf8Text l_page_utf8_at(ChaPageWo *page, int line_index, gboolean do_scan);

static void cha_full_page_wo_class_init(ChaFullPageWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	ChaPageWoClass *page_wo_class = CHA_PAGE_WO_CLASS(clazz);
	page_wo_class->writeToStream = l_page_write_to_stream;
	page_wo_class->addLine = l_page_add_line;
	page_wo_class->insertLine = l_page_insert_line;
	page_wo_class->removeRange = l_page_remove_range;
	page_wo_class->editableLineAt = l_page_editable_line_at;
	page_wo_class->lineCount = l_page_line_count;
	page_wo_class->lineRefAt = l_page_line_ref_at;
	page_wo_class->utf8At = l_page_utf8_at;
	page_wo_class->impoverish = l_page_impoverish;
	page_wo_class->enrich = l_page_enrich;
	page_wo_class->enrichmentRemap = l_page_enrichment_remap;


	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_full_page_wo_init(ChaFullPageWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaFullPageWo *instance = CHA_FULL_PAGE_WO(object);
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private(instance);
	cat_unref_ptr(priv->line_array);
	G_OBJECT_CLASS(cha_full_page_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_full_page_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void cha_full_page_wo_construct(ChaFullPageWo *page, CatArrayWo *c_lines) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private(page);
	cha_page_wo_construct((ChaPageWo *) page, TRUE);

	if (c_lines) {
		priv->line_array = c_lines;
	} else {
		priv->line_array = cat_array_wo_new();
	}
}


ChaFullPageWo *cha_full_page_wo_new(CatArrayWo *c_pages) {
	ChaFullPageWo *result = g_object_new(CHA_TYPE_FULL_PAGE_WO, NULL);
	cat_ref_anounce(result);
	cha_full_page_wo_construct(result, c_pages);
	return result;
}

static void l_page_write_to_stream(ChaPageWo *page, GOutputStream *out_stream) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) page);
	CatIIterator *iter = cat_array_wo_iterator(priv->line_array);
	char let[] = { 13, 10, 13 };
	while(cat_iiterator_has_next(iter)) {
		ChaLineWo *line = (ChaLineWo *) cat_iiterator_next(iter);
		cat_log_debug("line=%o", line);
		CatStringWo *text = cha_line_wo_get_text(line);
		const char *txt_data = cat_string_wo_getchars(text);
		int txt_len = cat_string_wo_length(text);
		gsize written = 0;
		GError *error = NULL;
		g_output_stream_write_all(out_stream, txt_data, txt_len, &written, NULL, (GError **) &error);
		switch(cha_line_wo_get_line_end(line)) {
			case CHA_LINE_END_CR :
				g_output_stream_write_all(out_stream, let, 1, &written, NULL, (GError **) &error);
				break;
			case CHA_LINE_END_CRLF :
				g_output_stream_write_all(out_stream, let, 2, &written, NULL, (GError **) &error);
				break;
			case CHA_LINE_END_LF :
				g_output_stream_write_all(out_stream, let+1, 1, &written, NULL, (GError **) &error);
				break;
			case CHA_LINE_END_LFCR :
				g_output_stream_write_all(out_stream, let+1, 2, &written, NULL, (GError **) &error);
				break;
			case CHA_LINE_END_NONE :
				break;
		}
		if (error) {
			cat_log_debug("error=%s", error->message);
		}
	}
	cat_unref_ptr(iter);
}


static void l_page_enrich(ChaPageWo *page, ChaEnrichmentDataMapWo *a_map) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) page);

	int enr_count = cha_page_wo_get_enrichment_count(page);
	CHA_PAGE_WO_CLASS(cha_full_page_wo_parent_class)->enrich(page, a_map);

	if (enr_count==0) {
		CatLock *lock = cha_page_wo_get_lock(page);
		CatIIterator *iter = cat_array_wo_iterator(priv->line_array);
		while(cat_iiterator_has_next(iter)) {
			ChaLineWo *line = (ChaLineWo *) cat_iiterator_next(iter);
			cha_line_wo_enrich(line, a_map, lock);
		}
		cat_unref_ptr(iter);
	}
	cat_log_debug("page=%o", page);
}

static void l_page_impoverish(ChaPageWo *page) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) page);
	int enr_count = cha_page_wo_get_enrichment_count(page);
	CHA_PAGE_WO_CLASS(cha_full_page_wo_parent_class)->impoverish(page);
	if (enr_count==1) {
		CatIIterator *iter = cat_array_wo_iterator(priv->line_array);
		while(cat_iiterator_has_next(iter)) {
			ChaLineWo *line = (ChaLineWo *) cat_iiterator_next(iter);
			cha_line_wo_impoverish(line);
		}
		cat_unref_ptr(iter);
	}
}

static void l_page_enrichment_remap(ChaPageWo *page, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) page);
	CatIIterator *iter = cat_array_wo_iterator(priv->line_array);
	while(cat_iiterator_has_next(iter)) {
		ChaLineWo *line = (ChaLineWo *) cat_iiterator_next(iter);
		cha_line_wo_enrichment_remap(line, a_old_map, a_new_map, add_idx, rem_idx);
	}
	cat_unref_ptr(iter);
	CHA_PAGE_WO_CLASS(cha_full_page_wo_parent_class)->enrichmentRemap(page, a_old_map, a_new_map, add_idx, rem_idx);
}


#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_page)) { \
			cat_log_error("Object is read only:%o", e_page); \
			return rval; \
		} \

#define CHECK_IF_WRITABLE_LINES(rval) \
		if (cat_wo_is_anchored((CatWo *) e_page)) { \
			cat_log_error("Object is read only:%o", e_page); \
			return rval; \
		} \
		if (cat_array_wo_is_anchored(priv->line_array)) { \
			CatArrayWo *e_array = cat_array_wo_create_editable(priv->line_array); \
			cat_unref_ptr(priv->line_array); \
			priv->line_array = e_array; \
		} \


static void l_page_add_line(ChaPageWo *e_page, ChaLineWo *line) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) e_page);
	CHECK_IF_WRITABLE_LINES()
	cat_array_wo_append(priv->line_array, (GObject *) line);
}

static void l_page_insert_line(ChaPageWo *e_page, ChaLineWo *line, int index) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) e_page);
	CHECK_IF_WRITABLE_LINES()
	cat_array_wo_insert(priv->line_array, (GObject *) line, index);
}

static void l_page_remove_range(ChaPageWo *e_page, int first, int last) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) e_page);
	CHECK_IF_WRITABLE_LINES()
	cat_array_wo_remove_range(priv->line_array, first, (last+1)-first);
}

static ChaLineWo *l_page_editable_line_at(ChaPageWo *e_page, int index) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) e_page);
	CHECK_IF_WRITABLE(NULL)
	ChaLineWo *result = (ChaLineWo *) cat_array_wo_get(priv->line_array, index);
	if (cha_line_wo_is_anchored(result)) {
		if (cat_array_wo_is_anchored(priv->line_array)) {
			CatArrayWo *e_array = cat_array_wo_create_editable(priv->line_array);
			cat_unref_ptr(priv->line_array);
			priv->line_array = e_array;
		}
		result = cha_line_wo_create_editable(result);
		cat_array_wo_set(priv->line_array, (GObject *) result, index, NULL);
		cat_unref(result);
	}
	return result;
}

static int l_page_line_count(ChaPageWo *page) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) page);
	return cat_array_wo_size(priv->line_array);
}

static ChaLineWo *l_page_line_ref_at(ChaPageWo *page, int index) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) page);
	ChaLineWo *result = (ChaLineWo *) cat_array_wo_get(priv->line_array, index);
	return cat_ref_ptr(result);
}


static const ChaUtf8Text l_page_utf8_at(ChaPageWo *page, int line_index, gboolean do_scan) {
	ChaUtf8Text result;
	result.text = NULL;
	result.text_len = 0;
	result.text_needs_cleanup = FALSE;

	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) page);
	ChaLineWo *line = (ChaLineWo *) cat_array_wo_get(priv->line_array, line_index);
	CatStringWo *line_text = cha_line_wo_get_text(line);
	const char *xtxt = (char *) cat_string_wo_getchars(line_text);
	int xlen = cat_string_wo_length(line_text);
	result.line_end = cha_line_wo_get_line_end(line);

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



static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_FULL_PAGE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	cat_log_debug("%p original=%p", CAT_WO_CLASS(cha_full_page_wo_parent_class)->constructEditable, original);

	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) e_uninitialized);
	if (original) {
		ChaFullPageWoPrivate *rpriv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) original);
		priv->line_array = cat_ref_ptr(rpriv->line_array);
	} else {
		priv->line_array = NULL;
	}
	return CAT_WO_CLASS(cha_full_page_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private((ChaFullPageWo *) wo);
	priv->line_array = cat_array_wo_anchor(priv->line_array, version);
	CAT_WO_CLASS(cha_full_page_wo_parent_class)->anchorChildren(wo, version);
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaFullPageWoPrivate *priv_a = cha_full_page_wo_get_instance_private((ChaFullPageWo *) wo_a);
	const ChaFullPageWoPrivate *priv_b = cha_full_page_wo_get_instance_private((ChaFullPageWo *) wo_b);
	return cat_array_wo_equal(priv_a->line_array, priv_b->line_array, NULL) && CAT_WO_CLASS(cha_full_page_wo_parent_class)->equal(wo_a, wo_b);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_FULL_PAGE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private(CHA_FULL_PAGE_WO(e_uninitialized));
	if (wo_source) {
		ChaFullPageWoPrivate *priv_src = cha_full_page_wo_get_instance_private(CHA_FULL_PAGE_WO(wo_source));
		priv->line_array = cat_array_wo_clone(priv_src->line_array, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->line_array = cat_array_wo_new();
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_full_page_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(ChaFullPageWo,cha_full_page_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaFullPageWo *instance = CHA_FULL_PAGE_WO(self);
	ChaFullPageWoPrivate *priv = cha_full_page_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s line-array:%o]", iname, self, cat_wo_get_version((CatWo *) instance), cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->line_array);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
