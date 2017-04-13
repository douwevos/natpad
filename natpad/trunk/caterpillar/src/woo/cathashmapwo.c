/*
   File:    cathashmapwo.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 8, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "cathashmapwoprivate.h"
#include "../catistringable.h"
#include "catstringwo.h"
#include "catwoprivate.h"
#include <math.h>

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatHashMapWo"
#include "../logging/catlog.h"

typedef struct _CatHashMapWoIter	      CatHashMapWoIter;
typedef struct _CatHashMapWoIterClass	  CatHashMapWoIterClass;

struct _CatHashMapWoIter {
	GObject parent;
	CatHashMapWo *hash_map;
	CatHashMapWoPrivate *hash_map_priv;
	int bucket_idx;
	CatArrayWo *current_bucket;
	int current_bucket_index;
};

GType cat_hash_map_wo_iter_get_type(void);
#define CAT_TYPE_HASH_MAP_WO_ITER            (cat_hash_map_wo_iter_get_type())


#define LOAD_FACTOR           0.75
#define LOAD_FACTOR_INCREASE  13

typedef struct _CatHashMapWoInfo CatHashMapWoInfo;

struct _CatHashMapWoPrivate {
	GHashFunc hash_func;
	GEqualFunc equal_func;
	int size;
	int modification_count;
	int bucket_count;
	CatArrayWo **buckets;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatHashMapWo, cat_hash_map_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(CatHashMapWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_wo_anchor_children(CatWo *wo, int version);
static CatWo *l_wo_anchor(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);
struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info);


static void cat_hash_map_wo_class_init(CatHashMapWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_wo_construct_editable;
	wo_class->anchorChildren = l_wo_anchor_children;
	wo_class->anchor = l_wo_anchor;
	wo_class->cloneContent = l_clone_content;
	wo_class->createEditInfo = l_create_edit_info;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_hash_map_wo_init(CatHashMapWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatHashMapWo *instance = CAT_HASH_MAP_WO(object);
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(instance);
	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) instance);
	if (hash_map_info && hash_map_info->buckets) {
		 if (hash_map_info->buckets==priv->buckets) {
			 priv->buckets = NULL;
			 priv->bucket_count = 0;
		 }
	}
	int bucket_index = priv->bucket_count;
	priv->bucket_count = 0;
	for(bucket_index--; bucket_index>=0; bucket_index--) {
		if (priv->buckets[bucket_index]) {
			cat_log_detail("dispose-bucket:%p", priv->buckets[bucket_index]);
		}
		cat_unref_ptr(priv->buckets[bucket_index]);
	}

	cat_log_debug("free buckets:%p", priv->buckets);
	cat_free_ptr(priv->buckets);

	G_OBJECT_CLASS(cat_hash_map_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_hash_map_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatHashMapWo *cat_hash_map_wo_new(GHashFunc hash_func, GEqualFunc equal_func) {
	CatHashMapWo *result = g_object_new(CAT_TYPE_HASH_MAP_WO, NULL);
	cat_ref_anounce(result);
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->equal_func = equal_func;
	priv->hash_func = hash_func;
	priv->buckets = NULL;
	priv->bucket_count = 0;
	priv->size = 0;
	priv->modification_count = 0;
	return result;
}

static GObject *l_get_from_bucket_list(GEqualFunc equal_func, CatArrayWo *bucket, GObject *key, int *index) {
	GObject *skey, *svalue;
	GObject *result = NULL;
	CatIIterator *iterator = cat_array_wo_iterator(bucket);
	int cnt = 0;
	if (index) {
		*index =  -1;
	}
	while(cat_iiterator_has_next(iterator)) {
		skey = cat_iiterator_next(iterator);
		svalue = cat_iiterator_next(iterator);
		if (equal_func(key, skey)) {
			result = svalue;
			if (index) {
				*index = cnt;
			}
			break;
		}
		cnt+=2;
	}
	cat_unref(iterator);
	return result;
}

GObject *cat_hash_map_wo_get(const CatHashMapWo *map, void *key) {
	const CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) map);
	cat_log_detail("map=%p", map);
	if (priv->bucket_count==0) {
		return NULL;
	}
	unsigned int hash = priv->hash_func(key);
	cat_log_detail("priv->bucket_count=%d, buckets=%p, hash=%d", priv->bucket_count, priv->buckets, hash);
	CatArrayWo *bucket_list = priv->buckets[hash % priv->bucket_count];
	if (bucket_list) {
		return l_get_from_bucket_list(priv->equal_func, bucket_list, key, NULL);
	}
	return NULL;
}

int cat_hash_map_wo_size(const CatHashMapWo *map) {
	const CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) map);
	return priv->size;
}

int cat_hash_map_wo_modification_count(const CatHashMapWo *map) {
	const CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) map);
	return priv->modification_count;
}


gboolean cat_hash_map_wo_equal(const CatHashMapWo *map, const CatHashMapWo *other, GEqualFunc equal_func_for_value) {
	if (map==other) {
		return TRUE;
	}
	if ((map==NULL) || (other==NULL)) {
		return FALSE;
	}
	const CatHashMapWoPrivate *priv_a = cat_hash_map_wo_get_instance_private((CatHashMapWo *) map);
	const CatHashMapWoPrivate *priv_b = cat_hash_map_wo_get_instance_private((CatHashMapWo *) other);
	if (priv_a->size!=priv_b->size) {
		return FALSE;
	}
	if (priv_a->bucket_count==priv_b->bucket_count && priv_a->buckets==priv_b->buckets) {
		return TRUE;
	}

	gboolean result = TRUE;
	int bucket_index;
	GObject *value;
	GObject *other_val;
	GObject *key;
	for(bucket_index=0; bucket_index<priv_a->bucket_count && result; bucket_index++) {
		CatArrayWo *bucket = (CatArrayWo *) priv_a->buckets[bucket_index];
		if (bucket) {
			if (cat_array_wo_size(bucket)!=0) {
				if (priv_a->bucket_count==priv_b->bucket_count) {
					CatArrayWo *obucket = (CatArrayWo *) priv_b->buckets[bucket_index];
					if (obucket==bucket) {
						/* the two buckets are 100% identical */
						continue;
					}
				}
				CatIIterator *src_iter = cat_array_wo_iterator(bucket);
				while(cat_iiterator_has_next(src_iter) && result) {
					key = cat_iiterator_next(src_iter);
					value = cat_iiterator_next(src_iter);
					other_val = cat_hash_map_wo_get(other, key);
					if (other_val==value) {
						continue;
					} else if (equal_func_for_value==NULL) {
						if (CAT_IS_WO(value) && CAT_IS_WO(other_val)) {
							result = cat_wo_equal((CatWo *) value, (CatWo *) other_val);
						} else {
							result = FALSE;
						}
					} else if (!equal_func_for_value(value, other_val)) {
						result = FALSE;
					}
				}
				cat_unref_ptr(src_iter);
			}
		}
	}
	return result;
}



#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_map)) { \
			cat_log_error("Object is read only:%o", e_map); \
			return rval; \
		} \


static CatArrayWo **l_create_buckets_list(int count) {
	if (count == 0) {
		return NULL;
	}
	return (CatArrayWo **) g_malloc0(sizeof(CatArrayWo *)*count);
}

static CatArrayWo **l_clone_original_buckets(CatHashMapWo *e_map) {
	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) e_map);
	int count = hash_map_info->bucket_count;
	cat_log_detail("count = %d", count);
	CatArrayWo **result = l_create_buckets_list(count);
	CatArrayWo **out = result;
	CatArrayWo **in = hash_map_info->buckets;
	for(count--; count>=0; count--) {
		cat_log_trace("*in=%o", *in);
		*out = cat_ref_ptr(*in);
		out++;
		in++;
	}
	cat_log_detail("result=%p", result);
	return result;
}



static void l_rehash(CatHashMapWo *e_map, int new_bucket_count, gboolean force) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(e_map);
	if (new_bucket_count<LOAD_FACTOR_INCREASE) {
		new_bucket_count = LOAD_FACTOR_INCREASE;
	}
	cat_log_debug("new_bucket_count=%d, bucket_count=%d", new_bucket_count, priv->bucket_count);
	if (new_bucket_count==priv->bucket_count && !force) {
		return;
	}

	CatArrayWo **bucket_list = l_create_buckets_list(new_bucket_count);
	GHashFunc hash_func = priv->hash_func;

	unsigned int hash, new_bucket_index;
	int bucket_index;
	GObject *key, *value;
	CatArrayWo *new_bucket;
	for(bucket_index=0; bucket_index<priv->bucket_count; bucket_index++) {
		CatArrayWo *bucket = priv->buckets[bucket_index];
		if (bucket) {
			CatIIterator *bucket_iter = cat_array_wo_iterator(bucket);
			while(cat_iiterator_has_next(bucket_iter)) {
				key = cat_iiterator_next(bucket_iter);
				value = cat_iiterator_next(bucket_iter);
				hash = hash_func(key);
				new_bucket_index = hash%new_bucket_count;
				new_bucket = bucket_list[new_bucket_index];
				if (new_bucket == NULL) {
					new_bucket = cat_array_wo_new();
					bucket_list[new_bucket_index] = new_bucket;
				}
				cat_array_wo_append(new_bucket, (GObject *) key);
				cat_array_wo_append(new_bucket, (GObject *) value);
			}
			cat_unref_ptr(bucket_iter);
		}
	}


	/* clear current buckets*/
	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) e_map);
	if (hash_map_info && hash_map_info->buckets) {
		 if (hash_map_info->buckets==priv->buckets) {
			 priv->buckets = NULL;
			 priv->bucket_count = 0;
		 }
	}
	bucket_index = priv->bucket_count;
	for(bucket_index--; bucket_index>=0; bucket_index--) {
		cat_unref_ptr(priv->buckets[bucket_index]);
	}
	cat_free_ptr(priv->buckets);

	/* store new buckets and update modification count */
	priv->buckets = bucket_list;
	priv->bucket_count = new_bucket_count;
	priv->modification_count++;
}


static void l_rehash_check_len(CatHashMapWo *e_map, int size) {
	float nice_bucket_count = (float) size * LOAD_FACTOR;
	int wanted_bckt_cnt = ceilf(nice_bucket_count);
	wanted_bckt_cnt = (wanted_bckt_cnt-1+LOAD_FACTOR_INCREASE)/LOAD_FACTOR_INCREASE;
	wanted_bckt_cnt *= LOAD_FACTOR_INCREASE;
	l_rehash(e_map, wanted_bckt_cnt, FALSE);
}

static void l_rehash_check(CatHashMapWo *e_map) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(e_map);
	l_rehash_check_len(e_map, priv->size);
}


void cat_hash_map_wo_clear(CatHashMapWo *e_map) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(e_map);
	cat_log_debug("e_map=%p", e_map);
	CHECK_IF_WRITABLE()
	priv->size = 0;
	priv->modification_count++;
	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) e_map);
	if (hash_map_info && hash_map_info->buckets) {
		 if (hash_map_info->buckets==priv->buckets) {
			 priv->buckets = NULL;
			 priv->bucket_count = 0;
			 return;
		 }
	}
	int bucket_index = priv->bucket_count;
	for(bucket_index--; bucket_index>=0; bucket_index--) {
		cat_unref_ptr(priv->buckets[bucket_index]);
	}
}

CatHashMapWo *cat_hash_map_wo_clone(const CatHashMapWo *source, CatCloneDepth clone_depth) {
	cat_log_debug("source=%p", source);
	if (source==NULL) {
		return NULL;
	} else if (clone_depth==CAT_CLONE_DEPTH_NONE && cat_hash_map_wo_is_anchored(source)) {
		return cat_ref_ptr((CatHashMapWo *) source);
	}
	const CatHashMapWoPrivate *spriv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) source);
	CatHashMapWo *result = cat_hash_map_wo_new(spriv->hash_func, spriv->equal_func);

	if (spriv->size!=0) {
		CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(result);
		priv->bucket_count = spriv->bucket_count;
		priv->buckets = l_create_buckets_list(spriv->bucket_count);

		int bucket_index = priv->bucket_count;
		if (clone_depth==CAT_CLONE_DEPTH_MAIN) {
			clone_depth = CAT_CLONE_DEPTH_NONE;
		}
		for(bucket_index--; bucket_index>=0; bucket_index--) {
			CatArrayWo *source_bucket = spriv->buckets[bucket_index];
			if (source_bucket==NULL) {
				continue;
			} else {
				priv->buckets[bucket_index] = cat_array_wo_clone(source_bucket, clone_depth);
			}
		}

		priv->size = spriv->size;
		priv->modification_count = spriv->modification_count;
	}
	return result;
}

gboolean cat_hash_map_wo_put(CatHashMapWo *e_map, GObject *key, GObject *value) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(e_map);
	CHECK_IF_WRITABLE(FALSE)
	unsigned int hash = priv->hash_func(key);
	cat_log_debug("map=%p, bucket_count=%d, hash=%d, key=%o", e_map, priv->bucket_count, hash, key);
	cat_ref_sink_ptr(key);
	cat_ref_sink_ptr(value);
	if (priv->bucket_count==0) {
		int new_bucket_count = LOAD_FACTOR_INCREASE;
		priv->bucket_count = new_bucket_count;
		priv->buckets = l_create_buckets_list(new_bucket_count);
	}
	unsigned int bucket_index = hash % priv->bucket_count;
	cat_log_detail("map=%p, bucket_index=%d", e_map, bucket_index);

	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) e_map);
	if (hash_map_info && hash_map_info->buckets) {
		 if (hash_map_info->buckets==priv->buckets) {
			 priv->buckets = l_clone_original_buckets(e_map);
		 }
	}
	CatArrayWo *bai = priv->buckets[bucket_index];
	if (bai == NULL) {
		priv->buckets[bucket_index] = cat_array_wo_new();
	} else if (cat_array_wo_is_anchored(bai)) {
		priv->buckets[bucket_index] = cat_array_wo_create_editable(bai);
		cat_unref_ptr(bai);
	}

	CatArrayWo *e_bai = priv->buckets[bucket_index];
	cat_log_debug("%p, e_bucket=%p", e_map, e_bai);

	int key_index = -1;
	GObject *old_value = e_bai==NULL ? NULL : l_get_from_bucket_list(priv->equal_func, e_bai, key, &key_index);
	cat_log_trace("map=%p, old_value=%p, value=%p", e_map, old_value, value);
	if (old_value==value) {
		cat_unref_ptr(key);
		cat_unref_ptr(value);
		return FALSE;
	}

	cat_log_trace("map=%p, key_index=%d, buckets=%p", e_map, key_index, priv->buckets);
	if (key_index>=0) {
		cat_array_wo_set(e_bai, value, key_index+1, NULL);
	} else {
		cat_array_wo_append(e_bai, key);
		cat_array_wo_append(e_bai, value);
		priv->size++;
	}
	priv->modification_count++;
	cat_unref(key);
	cat_unref(value);
	l_rehash_check(e_map);
	cat_log_trace("buckets=%p", priv->buckets);
	return TRUE;
}

void cat_hash_map_wo_put_all(CatHashMapWo *e_map, CatHashMapWo *to_add) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(e_map);
	CatHashMapWoPrivate *t_priv = cat_hash_map_wo_get_instance_private(to_add);
	cat_log_debug("e_map=%p, to_add=%p", e_map, to_add);
	CHECK_IF_WRITABLE()
	if (t_priv->size==0) {
		return;
	}

	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) e_map);
	if (hash_map_info && hash_map_info->buckets) {
		 if (hash_map_info->buckets==priv->buckets) {
			 priv->buckets = l_clone_original_buckets(e_map);
		 }
	}

	l_rehash_check_len(e_map, priv->size+t_priv->size);

	int t_bucket_index = t_priv->bucket_count;
	for(t_bucket_index--; t_bucket_index>=0; t_bucket_index--) {
		CatArrayWo *t_bucket = t_priv->buckets[t_bucket_index];
		if (t_bucket==NULL) {
			continue;
		}
		CatIIterator *iter = cat_array_wo_iterator(t_bucket);
		while(cat_iiterator_has_next(iter)) {
			GObject *key = cat_iiterator_next(iter);
			GObject *val = cat_iiterator_next(iter);

			unsigned int hash = priv->hash_func(key);
			unsigned int bucket_index = hash % priv->bucket_count;

			CatArrayWo *bai = priv->buckets[bucket_index];
			if (bai == NULL) {
				priv->buckets[bucket_index] = cat_array_wo_new();
			} else if (cat_array_wo_is_anchored(bai)) {
				priv->buckets[bucket_index] = cat_array_wo_create_editable(bai);
				cat_unref_ptr(bai);
			}

			CatArrayWo *e_bai = priv->buckets[bucket_index];

			int key_index = -1;
			GObject *old_value = e_bai==NULL ? NULL : l_get_from_bucket_list(priv->equal_func, e_bai, key, &key_index);
			if (old_value==val) {
				continue;
			}

			cat_log_trace("map=%p, key_index=%d", e_map, key_index);
			if (key_index>=0) {
				cat_array_wo_set(e_bai, val, key_index+1, NULL);
			} else {
				cat_array_wo_append(e_bai, key);
				cat_array_wo_append(e_bai, val);
				priv->size++;
			}
		}
		cat_unref_ptr(iter);
	}
	priv->modification_count++;

}



GObject *cat_hash_map_wo_remove(CatHashMapWo *e_map, GObject *key) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(e_map);
	if (priv->bucket_count==0) {
		return NULL;
	}
	unsigned int hash = priv->hash_func(key);
	unsigned int bucket_index = hash % priv->bucket_count;
	CatArrayWo *bucket = priv->buckets[bucket_index];
	cat_log_trace("bucket=%p, bucket_index=%d, hash=%d", bucket, bucket_index, hash);
	if (bucket==NULL) {
		return NULL;
	}

	int key_index=-1;
	GObject *old_value = bucket==NULL ? NULL : l_get_from_bucket_list(priv->equal_func, bucket, key, &key_index);
	cat_log_trace("map=%p, key_index=%d", e_map, key_index);
	if (key_index<0) {
		return NULL;
	}

	CHECK_IF_WRITABLE(FALSE)

	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) e_map);
	if (hash_map_info && hash_map_info->buckets) {
		 if (hash_map_info->buckets==priv->buckets) {
			 priv->buckets = l_clone_original_buckets(e_map);
		 }
	}
	CatArrayWo *e_bai = priv->buckets[bucket_index];
	cat_log_trace("e_bai=%p", e_bai);
	if (cat_array_wo_is_anchored(e_bai)) {
		e_bai = cat_array_wo_create_editable(e_bai);
		cat_ref_swap(priv->buckets[bucket_index], e_bai);
	}
	cat_log_trace("e_bai=%p", e_bai);


	cat_ref_ptr(old_value);
	cat_array_wo_remove(e_bai, key_index, NULL);
	cat_array_wo_remove(e_bai, key_index, NULL);
	cat_log_trace("e_bai.size=%d", cat_array_wo_size(e_bai));
	if (cat_array_wo_size(e_bai)==0) {
		cat_unref_ptr(priv->buckets[bucket_index]);
	}

	priv->size--;
	priv->modification_count++;

	l_rehash_check(e_map);
	return old_value;
}


CatArrayWo *cat_hash_map_wo_enlist_keys(CatHashMapWo *map, CatArrayWo *e_into) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(map);
	cat_log_debug("map=%p, e_into=%p", map, e_into);
	if (e_into==NULL) {
		e_into = cat_array_wo_new();
	}

	if (priv->bucket_count>0) {
		int bucket_index = priv->bucket_count;
		for(bucket_index--; bucket_index>=0; bucket_index--) {
			CatArrayWo *bucket = priv->buckets[bucket_index];
			if (bucket==NULL) {
				continue;
			}
			cat_log_debug("priv->buckets=%p, bucket=%p", priv->buckets, bucket);
			CatIIterator *iter = cat_array_wo_iterator(bucket);
			while(cat_iiterator_has_next(iter)) {
				GObject *key = cat_iiterator_next(iter);
				cat_iiterator_next(iter);
				cat_log_debug("enlisting key:%p", key);
				cat_array_wo_append(e_into, key);
			}
			cat_unref_ptr(iter);
		}
	}
	return e_into;
}

CatArrayWo *cat_hash_map_wo_enlist_values(CatHashMapWo *map, CatArrayWo *e_into) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(map);
	cat_log_debug("map=%p, e_into=%p", map, e_into);
	if (e_into==NULL) {
		e_into = cat_array_wo_new();
	}

	if (priv->bucket_count>0) {
		int bucket_index = priv->bucket_count;
		for(bucket_index--; bucket_index>=0; bucket_index--) {
			CatArrayWo *bucket = priv->buckets[bucket_index];
			if (bucket==NULL) {
				continue;
			}
			CatIIterator *iter = cat_array_wo_iterator(bucket);
			while(cat_iiterator_has_next(iter)) {
				cat_iiterator_next(iter);
				GObject *value = cat_iiterator_next(iter);
				cat_array_wo_append(e_into, value);
			}
			cat_unref_ptr(iter);
		}
	}
	return e_into;
}

CatIMapIterator *cat_hash_map_wo_iterator(CatHashMapWo *instance) {
	CatHashMapWoIter *result = g_object_new(CAT_TYPE_HASH_MAP_WO_ITER, NULL);
	cat_ref_anounce(result);
	result->bucket_idx = -1;
	result->current_bucket = NULL;
	result->hash_map_priv = cat_hash_map_wo_get_instance_private(instance);
	result->hash_map = cat_ref_ptr(instance);
	return (CatIMapIterator *) result;
}

CatHashMapWo *cat_hash_map_wo_create_editable(CatHashMapWo *instance) {
	return (CatHashMapWo *) cat_wo_create_editable((CatWo *) instance);
}

CatHashMapWo *cat_hash_map_wo_anchor(CatHashMapWo *instance, int version) {
	cat_log_debug("instance=%o", instance);
	return (CatHashMapWo *) cat_wo_anchor((CatWo *) instance, version);
}

CatHashMapWo *cat_hash_map_wo_get_original(CatHashMapWo *instance) {
	return (CatHashMapWo *) cat_wo_get_original((CatWo *) instance);
}

gboolean cat_hash_map_wo_is_anchored(const CatHashMapWo *instance) {
	return cat_wo_is_anchored((const CatWo *) instance);
}



static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(CatHashMapWoInfo, 1);
	}
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_HASH_MAP_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) e_uninitialized);
	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) info;

	if (original) {
		CatHashMapWoPrivate *rpriv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) original);
		priv->hash_func = rpriv->hash_func;
		priv->equal_func = rpriv->equal_func;
		priv->bucket_count = rpriv->bucket_count;
		priv->buckets = rpriv->buckets;
		priv->size = rpriv->size;
		priv->modification_count = rpriv->modification_count;
		hash_map_info->bucket_count = rpriv->bucket_count;
		hash_map_info->buckets = rpriv->buckets;
	} else {
		priv->hash_func = NULL;
		priv->equal_func = NULL;
		priv->bucket_count = 0;
		priv->buckets = NULL;
		priv->size = 0;
		priv->modification_count = 0;
		hash_map_info->bucket_count = 0;
		hash_map_info->buckets = NULL;
	}
	return CAT_WO_CLASS(cat_hash_map_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}



static void l_wo_anchor_children(CatWo *wo, int version) {
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) wo);
	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info(wo);
	cat_log_debug("hash_map_info.buckets=%p, priv->buckets=%p", hash_map_info->buckets, priv->buckets);
	if (hash_map_info->buckets!=priv->buckets) {
		int bucket_index = priv->bucket_count;
		for(bucket_index--; bucket_index>=0; bucket_index--) {
			CatArrayWo *bucket = priv->buckets[bucket_index];
			if (bucket) {
				if (!cat_array_wo_is_anchored(bucket)) {
					priv->buckets[bucket_index] = cat_array_wo_anchor(bucket, version);
				}
			}
		}
	}
}

static CatWo *l_wo_anchor(CatWo *wo, int version) {
	cat_log_debug("pre anchor:%o", wo);
	CatWo *result = CAT_WO_CLASS(cat_hash_map_wo_parent_class)->anchor(wo, version);
	cat_log_debug("did anchor:%o", result);
	return result;

}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	return cat_hash_map_wo_equal((const CatHashMapWo *) wo_a, (const CatHashMapWo *) wo_b, NULL);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_ARRAY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(CAT_HASH_MAP_WO(e_uninitialized));
	if (wo_source) {
		CatHashMapWoPrivate *priv_src = cat_hash_map_wo_get_instance_private(CAT_HASH_MAP_WO(wo_source));
		CatHashMapWoInfo *info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) wo_source);
		if (info!=NULL && info->buckets==priv_src->buckets) {
			priv->buckets = priv_src->buckets;
			priv->bucket_count = priv_src->bucket_count;
		} else if (priv_src->size>0) {
			priv->bucket_count = priv_src->bucket_count;
			priv->buckets = l_create_buckets_list(priv_src->bucket_count);
			int bucket_index = priv->bucket_count;
			for(bucket_index--; bucket_index>=0; bucket_index--) {
				CatArrayWo *source_bucket = priv_src->buckets[bucket_index];
				if (source_bucket==NULL) {
					continue;
				} else {
					priv->buckets[bucket_index] = cat_array_wo_clone(source_bucket, CAT_CLONE_DEPTH_NONE);
				}
			}

		} else {
			priv->bucket_count = 0;
			priv->buckets = NULL;
		}

		priv->hash_func = priv_src->hash_func;
		priv->equal_func = priv_src->equal_func;
		priv->size = priv_src->size;
		priv->modification_count = priv_src->modification_count;

	} else {
		priv->hash_func = NULL;
		priv->equal_func = NULL;
		priv->bucket_count = 0;
		priv->buckets = NULL;
		priv->size = 0;
		priv->modification_count = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_hash_map_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(CatHashMapWoInfo, 1);
	}

	CatHashMapWoInfo *hmap_info = (CatHashMapWoInfo *) info;
	if (original) {
		CatHashMapWoPrivate *rpriv = cat_hash_map_wo_get_instance_private((CatHashMapWo *) original);
		hmap_info->buckets = rpriv->buckets;
		hmap_info->bucket_count = rpriv->bucket_count;
	} else {
		hmap_info->buckets = NULL;
		hmap_info->bucket_count = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_hash_map_wo_parent_class);
	if (wocls->createEditInfo) {
		return wocls->createEditInfo(e_wo, original, info);
	}
	return info;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatHashMapWo *instance = CAT_HASH_MAP_WO(self);
	CatHashMapWoPrivate *priv = cat_hash_map_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s, size=%d, buckets=%p", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->size, priv->buckets);

	if (priv->bucket_count>0) {
		int idx=0;
		for(idx=0; idx<priv->bucket_count; idx++) {
			CatArrayWo *bucket = priv->buckets[idx];
			if (bucket!=NULL) {
				cat_string_wo_format(append_to, ", bucket::%d::%o", idx, bucket);
			}
		}
	}

	CatHashMapWoInfo *hash_map_info = (CatHashMapWoInfo *) cat_wo_get_info((CatWo *) self);
	if (hash_map_info) {
		cat_string_wo_format(append_to, ", original-buckets=%p", hash_map_info->buckets);

	}


	cat_string_wo_append_chars(append_to, "]");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/






#define CAT_HASH_MAP_WO_ITER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_hash_map_wo_iter_get_type(), CatHashMapWoIter))
#define CAT_HASH_MAP_WO_ITER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_HASH_MAP_WO_ITER, CatHashMapWoIterClass))
#define CAT_IS_HASH_MAP_WO_ITER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_HASH_MAP_WO_ITER))
#define CAT_IS_HASH_MAP_WO_ITER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_HASH_MAP_WO_ITER))
#define CAT_HASH_MAP_WO_ITER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_HASH_MAP_WO_ITER, CatHashMapWoIterClass))


struct _CatHashMapWoIterClass {
	GObjectClass parent_class;
};


static gboolean l_iter_next(CatIMapIterator *self, gpointer **okey, gpointer **ovalue) {
	CatHashMapWoIter *this = (CatHashMapWoIter *) self;
	if (this->current_bucket==NULL) {
		if (this->bucket_idx>=this->hash_map_priv->bucket_count) {
			if (ovalue) {
				*ovalue = NULL;
			}
			if (okey) {
				*okey = NULL;
			}
			return FALSE;
		}
		while(this->current_bucket == NULL || cat_array_wo_size(this->current_bucket)==0) {
			this->bucket_idx++;
			if (this->bucket_idx>=this->hash_map_priv->bucket_count) {
				if (ovalue) {
					*ovalue = NULL;
				}
				if (okey) {
					*okey = NULL;
				}
				return FALSE;
			}
			this->current_bucket = this->hash_map_priv->buckets[this->bucket_idx];
		}
		this->current_bucket_index = 0;
	}
	GObject *key = cat_array_wo_get(this->current_bucket, this->current_bucket_index++);
	GObject *value = cat_array_wo_get(this->current_bucket, this->current_bucket_index++);
	if (this->current_bucket_index>=cat_array_wo_size(this->current_bucket)) {
		this->current_bucket = NULL;
	}
	if (ovalue) {
		*ovalue = value;
	}
	if (okey) {
		*okey = key;
	}
	return TRUE;
}

static void l_map_iterator_interface_init(CatIMapIteratorInterface *iface) {
	iface->next = l_iter_next;
}

G_DEFINE_TYPE_WITH_CODE(CatHashMapWoIter, cat_hash_map_wo_iter, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IMAP_ITERATOR, l_map_iterator_interface_init)
);

static void l_dispose_iter(GObject *object);


static void cat_hash_map_wo_iter_class_init(CatHashMapWoIterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose_iter;
}

static void cat_hash_map_wo_iter_init(CatHashMapWoIter *array_iter) {
}

static void l_dispose_iter(GObject *object) {
	CatHashMapWoIter *map_iter = CAT_HASH_MAP_WO_ITER(object);
	cat_unref_ptr(map_iter->hash_map);
	G_OBJECT_CLASS(cat_hash_map_wo_iter_parent_class)->dispose(object);

}
