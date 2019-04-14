/*
   File:    chapagelistwo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 25, 2015
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

#include "chapagelistwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPageListWo"
#include <logging/catlog.h>

struct _ChaPageListWoPrivate {
	int version;
	int enriched_count;
	CatArrayWo *page_array;
	ChaLineEnd line_ends;
	gboolean line_ends_are_mixed;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPageListWo, cha_page_list_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaPageListWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void cha_page_list_wo_class_init(ChaPageListWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_page_list_wo_init(ChaPageListWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPageListWo *instance = CHA_PAGE_LIST_WO(object);
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(instance);

	cat_unref_ptr(priv->page_array);
	G_OBJECT_CLASS(cha_page_list_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_page_list_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPageListWo *cha_page_list_wo_new() {
	ChaPageListWo *result = g_object_new(CHA_TYPE_PAGE_LIST_WO, NULL);
	cat_ref_anounce(result);
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->page_array = cat_array_wo_new();
	priv->line_ends = CHA_LINE_END_LF;
	priv->line_ends_are_mixed = FALSE;
	return result;
}

void cha_page_list_wo_enrich(ChaPageListWo *page_list, ChaEnrichmentDataMapWo *a_map) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(page_list);
	if (priv->enriched_count==0) {
		CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
		while(cat_iiterator_has_next(iter)) {
			ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
			cha_page_wo_enrich(page, a_map);
		}
		cat_unref_ptr(iter);
	}
	priv->enriched_count++;
}

void cha_page_list_wo_impoverish(ChaPageListWo *page_list) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(page_list);
	if (priv->enriched_count==1) {
		CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
		while(cat_iiterator_has_next(iter)) {
			ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
			cha_page_wo_impoverish(page);
		}
		cat_unref_ptr(iter);
	}
	priv->enriched_count--;
}

void cha_page_list_wo_enrichment_remap(ChaPageListWo *page_list, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(page_list);
	CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
	while(cat_iiterator_has_next(iter)) {
		ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
		cha_page_wo_enrichment_remap(page, a_old_map, a_new_map, add_idx, rem_idx);
	}
	cat_unref_ptr(iter);
}



int cha_page_list_wo_page_count(ChaPageListWo *page_list) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(page_list);
	return cat_array_wo_size(priv->page_array);
}

long long cha_page_list_wo_page_line_count(ChaPageListWo *page_list) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(page_list);
	CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
	long long result = 0;
	while(cat_iiterator_has_next(iter)) {
		ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
		result += cha_page_wo_line_count(page);
	}
	cat_unref_ptr(iter);
	return result;
}


ChaLineEnd cha_page_list_wo_get_line_ends(const ChaPageListWo *page_list) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) page_list);
	return priv->line_ends;
}

gboolean cha_page_list_wo_get_line_ends_are_mixed(const ChaPageListWo *page_list) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) page_list);
	return priv->line_ends_are_mixed;
}


long long cha_page_list_wo_line_count_till_page(const ChaPageListWo *page_list, int page_index) {
	const ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) page_list);
	CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
	long long result = 0;
	while(cat_iiterator_has_next(iter) && page_index>0) {
		ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
		result += cha_page_wo_line_count(page);
		page_index--;
	}
	cat_unref_ptr(iter);
	return result;
}

long long cha_page_list_wo_calculate_row(const ChaPageListWo *page_list, const ChaLineLocationWo *line_location) {
	const ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) page_list);
	CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
	long long result = 0;
	int page_index = cha_line_location_wo_get_page_index(line_location);
	while(cat_iiterator_has_next(iter) && page_index>0) {
		ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
		result += cha_page_wo_line_count(page);
		page_index--;
	}
	cat_unref_ptr(iter);
	if (page_index==0) {
		result += cha_line_location_wo_get_page_line_index(line_location);
	}
	return result;
}


ChaLineLocationWo *cha_page_list_wo_calculate_line_location(const ChaPageListWo *page_list, long long row) {
	const ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) page_list);
	CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
	long long page_start_row = 0;
	int page_index = 0;
	ChaLineLocationWo *result = NULL;
	while(cat_iiterator_has_next(iter)) {
		ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
		long long page_end_row = page_start_row + cha_page_wo_line_count(page);
		if (row>=page_start_row && row<page_end_row) {
			result = cha_line_location_wo_new_with(page_index, (int) (row-page_start_row));
			break;
		}
		page_index++;
		page_start_row = page_end_row;
	}
	cat_unref_ptr(iter);
	return result;
}

ChaLineLocationWo *cha_page_list_wo_calculate_line_location_end(const ChaPageListWo *page_list) {
	const ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) page_list);
	ChaPageWo *page = (ChaPageWo *) cat_array_wo_get_last(priv->page_array);
	return cha_line_location_wo_new_with(cat_array_wo_size(priv->page_array)-1, cha_page_wo_line_count(page)-1);
}



ChaPageWo *cha_page_list_wo_page_at(ChaPageListWo *page_list, int page_index) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(page_list);
	return (ChaPageWo *) cat_array_wo_get(priv->page_array, page_index);
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_page_list)) { \
			cat_log_error("Object is read only:%o", e_page_list); \
			return rval; \
		} \


#define CHECK_IF_WRITABLE_ARRAY(rval) \
		if (cat_wo_is_anchored((CatWo *) e_page_list)) { \
			cat_log_error("Object is read only:%o", e_page_list); \
			return rval; \
		} \
		if (cat_array_wo_is_anchored(priv->page_array)) { \
			CatArrayWo *e_array = cat_array_wo_create_editable(priv->page_array); \
			cat_unref_ptr(priv->page_array); \
			priv->page_array = e_array; \
		} \


ChaPageWo *cha_page_list_wo_editable_page_at(ChaPageListWo *e_page_list, int page_index) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	ChaPageWo *result = (ChaPageWo *) cat_array_wo_get(priv->page_array, page_index);
	if (cha_page_wo_is_anchored(result)) {
		CHECK_IF_WRITABLE_ARRAY(NULL);
		result = cha_page_wo_create_editable(result);
		cat_log_debug("priv->page_array=%o, result=%o", priv->page_array, result);
		cat_log_debug("rc-pre:page=%p, rc=%d", result, ((GObject *) result)->ref_count);
		cat_array_wo_set(priv->page_array, (GObject *) result, page_index, NULL);
		cat_log_debug("rc-pst:page=%p, rc=%d", result, ((GObject *) result)->ref_count);
		cat_unref(result);
	}
	return result;
}


void cha_page_list_wo_add_page(ChaPageListWo *e_page_list, ChaPageWo *page) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	CHECK_IF_WRITABLE_ARRAY()
	cat_array_wo_append(priv->page_array, (GObject *) page);
}


void cha_page_list_wo_add_page_at(ChaPageListWo *e_page_list, ChaPageWo *page, int index) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	CHECK_IF_WRITABLE_ARRAY()
	cat_array_wo_insert(priv->page_array, (GObject *) page, index);
}

void cha_page_list_wo_set_page_at(ChaPageListWo *e_page_list, ChaPageWo *page, int index) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	CHECK_IF_WRITABLE_ARRAY()
	cat_array_wo_set(priv->page_array, (GObject *) page, index, NULL);
}

void cha_page_list_wo_remove_page_at(ChaPageListWo *e_page_list, int index) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	CHECK_IF_WRITABLE_ARRAY()
	cat_array_wo_remove(priv->page_array, index, NULL);
}

void cha_page_list_wo_remove_range(ChaPageListWo *e_page_list, int first, int last) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	CHECK_IF_WRITABLE_ARRAY()
	cat_array_wo_remove_range(priv->page_array, first, (last+1)-first);
}


void cha_page_list_wo_set_line_ends(ChaPageListWo *e_page_list, ChaLineEnd line_ends, gboolean line_ends_are_mixed) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	CHECK_IF_WRITABLE();
	priv->line_ends = line_ends;
	priv->line_ends_are_mixed = line_ends_are_mixed;
}

void cha_page_list_wo_clear(ChaPageListWo *e_page_list) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(e_page_list);
	CHECK_IF_WRITABLE_ARRAY()
	cat_array_wo_clear(priv->page_array);
	priv->line_ends = CHA_LINE_END_NONE;
	priv->line_ends_are_mixed = FALSE;
}

void cha_page_list_wo_dump(const ChaPageListWo *list, CatStringWo *e_buf) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) list);
	CatIIterator *iter = cat_array_wo_iterator(priv->page_array);
	int page_idx=0;
	while(cat_iiterator_has_next(iter)) {
		ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
		cat_string_wo_format(e_buf, "page[%d] = %o\n", page_idx, page);
		page_idx++;
	}
	cat_unref_ptr(iter);

}


ChaPageListWo *cha_page_list_wo_create_editable(ChaPageListWo *instance) {
	return (ChaPageListWo *) cat_wo_create_editable((CatWo *)instance);
}

ChaPageListWo *cha_page_list_wo_anchor(ChaPageListWo *instance, int version) {
	return (ChaPageListWo *) cat_wo_anchor((CatWo*) instance, version);
}

ChaPageListWo *cha_page_list_wo_get_original(ChaPageListWo *instance) {
	return (ChaPageListWo *) cat_wo_get_original((CatWo*) instance);
}

gboolean cha_page_list_wo_is_anchored(const ChaPageListWo *instance) {
	return cat_wo_is_anchored((const CatWo*) instance);
}

gboolean cha_page_list_wo_equal(const ChaPageListWo *instance_a, const ChaPageListWo *instance_b) {
	if (instance_a==instance_b) {
		return TRUE;
	} else if (instance_a==NULL || instance_b==NULL) {
		return FALSE;
	}
	return CAT_WO_GET_CLASS(instance_a)->equal((const CatWo *) instance_a, (const CatWo *) instance_b);
}

ChaPageListWo *cha_page_list_wo_clone(const ChaPageListWo *source, CatCloneDepth clone_depth) {
	if (source==NULL) {
		return NULL;
	}
	return (ChaPageListWo *) cat_wo_clone((CatWo *) source, clone_depth);
}

static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PAGE_LIST_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) e_uninitialized);
	if (original) {
		ChaPageListWoPrivate *rpriv = cha_page_list_wo_get_instance_private((ChaPageListWo *) original);
		priv->page_array = cat_ref_ptr(rpriv->page_array);
		priv->line_ends = rpriv->line_ends;
		priv->line_ends_are_mixed = rpriv->line_ends_are_mixed;
	} else {
		priv->page_array = NULL;
		priv->line_ends = CHA_LINE_END_NONE;
		priv->line_ends_are_mixed = FALSE;
	}
	return CAT_WO_CLASS(cha_page_list_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_anchor_children(CatWo *wo, int version) {
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private((ChaPageListWo *) wo);
	priv->page_array = cat_array_wo_anchor(priv->page_array, version);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PAGE_LIST_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(CHA_PAGE_LIST_WO(e_uninitialized));
	if (wo_source) {
		ChaPageListWoPrivate *priv_src = cha_page_list_wo_get_instance_private(CHA_PAGE_LIST_WO(wo_source));
		priv->page_array = cat_array_wo_clone(priv_src->page_array, CAT_CLONE_DEPTH_NONE);
		priv->enriched_count = priv_src->enriched_count;
		priv->line_ends = priv_src->line_ends;
		priv->line_ends_are_mixed = priv_src->line_ends_are_mixed;
	} else {
		priv->page_array = cat_array_wo_new();
		priv->enriched_count = 0;
		priv->line_ends = CHA_LINE_END_NONE;
		priv->line_ends_are_mixed = FALSE;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_page_list_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaPageListWoPrivate *priv_a = cha_page_list_wo_get_instance_private((ChaPageListWo *) wo_a);
	const ChaPageListWoPrivate *priv_b = cha_page_list_wo_get_instance_private((ChaPageListWo *) wo_b);
	return cat_array_wo_equal(priv_a->page_array, priv_b->page_array, NULL)&& priv_a->line_ends == priv_b->line_ends
			&& priv_a->line_ends_are_mixed == priv_b->line_ends_are_mixed;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPageListWo *instance = CHA_PAGE_LIST_WO(self);
	ChaPageListWoPrivate *priv = cha_page_list_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s pages=%o]", iname, self, cat_wo_get_version((CatWo *) instance), cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->page_array);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
