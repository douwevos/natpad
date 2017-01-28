/*
   File:    chaenrichmentdata.c
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

#include "chaenrichmentdata.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaEnrichmentData"
#include <logging/catlog.h>

struct _ChaEnrichmentDataPrivate {
	CatLock *lock;
	ChaEnrichmentDataMapWo *a_enrichment_map;
	CatArrayWo *data;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaEnrichmentData, cha_enrichment_data, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaEnrichmentData)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);


static void l_dispose(GObject *object);
static void l_finalize(GObject *object);


static void cha_enrichment_data_class_init(ChaEnrichmentDataClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_enrichment_data_init(ChaEnrichmentData *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaEnrichmentData *instance = CHA_ENRICHMENT_DATA(object);
	ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private(instance);
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->a_enrichment_map);
	cat_unref_ptr(priv->data);
	G_OBJECT_CLASS(cha_enrichment_data_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_enrichment_data_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaEnrichmentData *cha_enrichment_data_new(ChaEnrichmentDataMapWo *a_enrichment_map) {
	ChaEnrichmentData *result = g_object_new(CHA_TYPE_ENRICHMENT_DATA, NULL);
	cat_ref_anounce(result);
	ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private(result);
	priv->a_enrichment_map = cat_ref_ptr(a_enrichment_map);
	priv->data = NULL;
	priv->lock = cat_lock_new();
	return result;
}

ChaEnrichmentData *cha_enrichment_data_new_lock(ChaEnrichmentDataMapWo *a_enrichment_map, CatLock *lock) {
	ChaEnrichmentData *result = g_object_new(CHA_TYPE_ENRICHMENT_DATA, NULL);
	cat_ref_anounce(result);
	ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private(result);
	priv->a_enrichment_map = cat_ref_ptr(a_enrichment_map);
	priv->data = NULL;
	priv->lock = cat_ref_ptr(lock);
	return result;
}

CatLock *cha_enrichment_data_get_lock(ChaEnrichmentData *enrichment_data) {
	ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private(enrichment_data);
	return priv->lock;
}


static void l_ensure_data(ChaEnrichmentDataPrivate *priv) {
	if (priv->data==NULL) {
		priv->data = cha_enrichment_data_map_wo_get_default_nullified_data(priv->a_enrichment_map);
		priv->data = cat_array_wo_create_editable(priv->data);
	}
}

void cha_enrichment_data_remap(ChaEnrichmentData *enrichment_data, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx) {
	ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private(enrichment_data);
	// TODO old_map is not the current map... we need a manual synchronize here ... for now log a fatal msgs
	cat_lock_lock(priv->lock);
	cat_log_debug("enrichment_data=%p, new-map=%p, add_idx=%d", enrichment_data, a_new_map, add_idx);
	if (priv->a_enrichment_map != a_old_map) {
		cat_log_fatal("need to implement this");
	}

	cat_ref_swap(priv->a_enrichment_map, a_new_map);
	if (priv->data) {
		if (add_idx>=0) {
			cat_array_wo_insert(priv->data, NULL, add_idx);
		}
		if (rem_idx>=0) {
			cat_array_wo_remove(priv->data, rem_idx, NULL);
		}
	}
	cat_lock_unlock(priv->lock);
}


ChaEnrichmentDataMapWo *cha_enrichment_data_get_map(const ChaEnrichmentData *enrichment_data) {
	const ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private((ChaEnrichmentData *) enrichment_data);
	cat_lock_lock(priv->lock);
	ChaEnrichmentDataMapWo *result = priv->a_enrichment_map;
	cat_lock_unlock(priv->lock);
	return result;
}

GObject *cha_enrichment_data_get_slot_content_ref(const ChaEnrichmentData *enrichment_data, int slot_index, GObject *slot_key) {
	GObject *result = NULL;
	const ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private((ChaEnrichmentData *) enrichment_data);
	cat_lock_lock(priv->lock);
	if (priv->data) {
		if (slot_key) {
			slot_index = cha_enrichment_data_map_wo_find_slot_index(priv->a_enrichment_map, slot_key, slot_index);
		}
		if (slot_index>=0) {
			result = cat_array_wo_get(priv->data, slot_index);
			cat_ref_ptr(result);
		}
	}
	cat_lock_unlock(priv->lock);
	return result;
}

void cha_enrichment_data_set_slot_content(const ChaEnrichmentData *enrichment_data, int slot_index, GObject *slot_key, GObject *content) {
	ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private((ChaEnrichmentData *) enrichment_data);
	cat_lock_lock(priv->lock);
	if (slot_key) {
		slot_index = cha_enrichment_data_map_wo_find_slot_index(priv->a_enrichment_map, slot_key, slot_index);
	}
	cat_log_debug("map=%p, slot_index=%d, slot_key=%o, content=%o", priv->a_enrichment_map, slot_index, slot_key, content);
	if (slot_index>=0) {
		l_ensure_data(priv);
		cat_array_wo_set(priv->data, content, slot_index, NULL);
	}
	cat_lock_unlock(priv->lock);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaEnrichmentData *instance = CHA_ENRICHMENT_DATA(self);
	ChaEnrichmentDataPrivate *priv = cha_enrichment_data_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p map=%p, data=%o]", iname, self, priv->a_enrichment_map, priv->data);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
