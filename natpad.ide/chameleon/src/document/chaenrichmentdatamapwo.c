/*
   File:    chaenrichmentdatamapwo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 22, 2015
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

#include "chaenrichmentdatamapwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaEnrichmentDataMapWo"
#include <logging/catlog.h>

struct _ChaEnrichmentDataMapWoPrivate {
	CatArrayWo *keys;
	CatArrayWo *a_nullified_data;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaEnrichmentDataMapWo, cha_enrichment_data_map_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaEnrichmentDataMapWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void cha_enrichment_data_map_wo_class_init(ChaEnrichmentDataMapWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_enrichment_data_map_wo_init(ChaEnrichmentDataMapWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaEnrichmentDataMapWo *instance = CHA_ENRICHMENT_DATA_MAP_WO(object);
	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private(instance);
	cat_unref_ptr(priv->keys);
	cat_unref_ptr(priv->a_nullified_data);
	G_OBJECT_CLASS(cha_enrichment_data_map_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_enrichment_data_map_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaEnrichmentDataMapWo *cha_enrichment_data_map_wo_new() {
	ChaEnrichmentDataMapWo *result = g_object_new(CHA_TYPE_ENRICHMENT_DATA_MAP_WO, NULL);
	cat_ref_anounce(result);
	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, FALSE);
	priv->keys = cat_array_wo_anchor(cat_array_wo_new(), 0);
	priv->a_nullified_data = cat_array_wo_new();
	priv->a_nullified_data = cat_array_wo_anchor(priv->a_nullified_data, 0);
	return result;
}

int cha_enrichment_data_map_wo_count(const ChaEnrichmentDataMapWo *enrichment_map) {
	const ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) enrichment_map);
	return cat_array_wo_size(priv->keys);
}

CatArrayWo *cha_enrichment_data_map_wo_get_default_nullified_data(const ChaEnrichmentDataMapWo *enrichment_map) {
	const ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) enrichment_map);
	return priv->a_nullified_data;
}


int cha_enrichment_data_map_wo_find_slot_index(const ChaEnrichmentDataMapWo *enrichment_map, GObject *slot_key, int guess_index) {
	const ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) enrichment_map);
	return cat_array_wo_find_index(priv->keys, slot_key, guess_index);
}


#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_enrichment_map)) { \
			cat_log_error("Object is read only:%o", e_enrichment_map); \
			return rval; \
		} \


int cha_enrichment_data_map_wo_add_slot(ChaEnrichmentDataMapWo *e_enrichment_map, GObject *slot_key) {
	CHECK_IF_WRITABLE(-1);
	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private(e_enrichment_map);
	int result = cat_array_wo_find_index(priv->keys, slot_key, -1);
	if (result<0) {
		if (cat_array_wo_is_anchored(priv->keys)) {
			CatArrayWo *e_keys = cat_array_wo_create_editable(priv->keys);
			cat_unref_ptr(priv->keys);
			priv->keys = e_keys;
		}

		result = cat_array_wo_append(priv->keys, slot_key);

		CatArrayWo *e_nd = cat_array_wo_create_editable(priv->a_nullified_data);
		cat_array_wo_append(e_nd, NULL);
		e_nd = cat_array_wo_anchor(e_nd, 0);
		cat_ref_swap(priv->a_nullified_data, e_nd);
		cat_unref_ptr(e_nd);
	}
	return result;
}

int cha_enrichment_data_map_wo_remove_slot(ChaEnrichmentDataMapWo *e_enrichment_map, GObject *slot_key) {
	CHECK_IF_WRITABLE(-1);
	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private(e_enrichment_map);
	int result = cat_array_wo_find_index(priv->keys, slot_key, -1);
	if (result>=0) {
		if (cat_array_wo_is_anchored(priv->keys)) {
			CatArrayWo *e_keys = cat_array_wo_create_editable(priv->keys);
			cat_unref_ptr(priv->keys);
			priv->keys = e_keys;
		}
		cat_array_wo_remove(priv->keys, result, NULL);
		CatArrayWo *e_nd = cat_array_wo_create_editable(priv->a_nullified_data);
		cat_array_wo_remove_last(e_nd, NULL);
		e_nd = cat_array_wo_anchor(e_nd, 0);
		cat_ref_swap(priv->a_nullified_data, e_nd);
		cat_unref_ptr(e_nd);
	}
	return result;
}



static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_ENRICHMENT_DATA_MAP_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) e_uninitialized);

	if (original) {
		ChaEnrichmentDataMapWoPrivate *rpriv = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) original);
		priv->keys = cat_ref_ptr(rpriv->keys);
		priv->a_nullified_data = cat_ref_ptr(rpriv->a_nullified_data);
	} else {
		priv->keys = NULL;
		priv->a_nullified_data = NULL;
	}
	return CAT_WO_CLASS(cha_enrichment_data_map_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) wo);
	priv->keys = cat_array_wo_anchor(priv->keys, version);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_ENRICHMENT_DATA_MAP_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}
	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private(CHA_ENRICHMENT_DATA_MAP_WO(e_uninitialized));
	if (wo_source) {
		ChaEnrichmentDataMapWoPrivate *priv_src = cha_enrichment_data_map_wo_get_instance_private(CHA_ENRICHMENT_DATA_MAP_WO(wo_source));
		priv->a_nullified_data = cat_array_wo_clone(priv_src->a_nullified_data, CAT_CLONE_DEPTH_NONE);
		priv->keys = cat_array_wo_clone(priv_src->keys, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->keys = cat_array_wo_anchor(cat_array_wo_new(), 0);
		priv->a_nullified_data = cat_array_wo_new();
		priv->a_nullified_data = cat_array_wo_anchor(priv->a_nullified_data, 0);
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_enrichment_data_map_wo_parent_class);
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
	const ChaEnrichmentDataMapWoPrivate *priv_a = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) wo_a);
	const ChaEnrichmentDataMapWoPrivate *priv_b = cha_enrichment_data_map_wo_get_instance_private((ChaEnrichmentDataMapWo *) wo_b);
	return cat_array_wo_equal(priv_a->keys, priv_b->keys, NULL);
}



CAT_WO_BASE_C(ChaEnrichmentDataMapWo, cha_enrichment_data_map_wo);


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaEnrichmentDataMapWo *instance = CHA_ENRICHMENT_DATA_MAP_WO(self);
	ChaEnrichmentDataMapWoPrivate *priv = cha_enrichment_data_map_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s, keys=%o]", iname, self, cat_wo_get_version((CatWo *) instance), cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->keys);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

