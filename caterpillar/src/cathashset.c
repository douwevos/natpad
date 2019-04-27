/*
   File:    cathashset.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Sep 13, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#include "cathashset.h"
#include "woo/catstringwo.h"
#include "catistringable.h"
#include <math.h>

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatHashSet"
#include "logging/catlog.h"


#define LOAD_FACTOR           0.75
#define LOAD_FACTOR_INCREASE  61

struct _CatHashSetPrivate {
	  GHashFunc hash_func;
	  GEqualFunc equal_func;
	  int modification_count;
	  int total_count;
	  int bucket_count;
	  CatArrayWo **e_buckets;
};

/********* HashSet-Iterator *********/

#define CAT_TYPE_HASH_SET_ITER            (cat_hash_set_iter_get_type())
#define CAT_HASH_SET_ITER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_hash_set_iter_get_type(), CatHashSetIter))
#define CAT_HASH_SET_ITER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_HASH_SET_ITER, CatHashSetIterClass))
#define CAT_IS_HASH_SET_ITER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_HASH_SET_ITER))
#define CAT_IS_HASH_SET_ITER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_HASH_SET_ITER))
#define CAT_HASH_SET_ITER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_HASH_SET_ITER, CatHashSetIterClass))


typedef struct _CatHashSetIter	      CatHashSetIter;

typedef struct _CatHashSetIterClass	  CatHashSetIterClass;


struct _CatHashSetIter {
	GObject parent;
	CatHashSet *hash_set;
	int bucket_idx;
	int bucket_offset;
	gboolean next_value_valid;
	gpointer next_value;
	gboolean ahead_value_valid;
	gpointer ahead_value;

};

struct _CatHashSetIterClass {
	GObjectClass parent_class;
};


GType cat_hash_set_iter_get_type(void);

static gboolean l_iter_has_next(CatIIterator *self);
static gboolean l_iter_is_last(CatIIterator *self);
static gpointer l_iter_next(CatIIterator *self);

static void l_iterator_interface_init(CatIIteratorInterface *iface) {
	iface->has_next = l_iter_has_next;
	iface->next = l_iter_next;
	iface->is_last = l_iter_is_last;
}

G_DEFINE_TYPE_WITH_CODE(CatHashSetIter, cat_hash_set_iter, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IITERATOR, l_iterator_interface_init);
});

static void l_dispose_iter(GObject *object);


static void cat_hash_set_iter_class_init(CatHashSetIterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose_iter;
}

static void cat_hash_set_iter_init(CatHashSetIter *array_iter) {
}

static void l_dispose_iter(GObject *object) {
	CatHashSetIter *instance = CAT_HASH_SET_ITER(object);
	cat_unref_ptr(instance->hash_set);
}


/********* HashSet-Iterator *********/

static void l_stringable_iface_init(CatIStringableInterface *iface);


G_DEFINE_TYPE_WITH_CODE(CatHashSet, cat_hash_set, G_TYPE_OBJECT,{
		G_ADD_PRIVATE(CatHashSet)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
})

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_destroy_buckets(CatArrayWo **e_buckets, int bucket_count);


static void cat_hash_set_class_init(CatHashSetClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_hash_set_init(CatHashSet *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatHashSet *instance = CAT_HASH_SET(object);
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(instance);
	if (priv->bucket_count>0) {
		l_destroy_buckets(priv->e_buckets, priv->bucket_count);
		priv->e_buckets = NULL;
		priv->bucket_count = 0;
	}
	priv->total_count = 0;

	G_OBJECT_CLASS(cat_hash_set_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_hash_set_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatHashSet *cat_hash_set_new(GHashFunc hash_func, GEqualFunc equal_func) {
	CatHashSet *result = g_object_new(CAT_TYPE_HASH_SET, NULL);
	cat_ref_anounce(result);
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(result);
	priv->hash_func = hash_func;
	priv->equal_func = equal_func;
	priv->e_buckets = NULL;
	priv->total_count = 0;
	priv->bucket_count = 0;
	return result;
}

void cat_hash_set_clear(CatHashSet *hash_set) {
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	l_destroy_buckets(priv->e_buckets, priv->bucket_count);
	priv->e_buckets = NULL;
	priv->bucket_count = 0;
	priv->total_count = 0;
}

int cat_hash_set_size(CatHashSet *hash_set) {
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	return priv->total_count;
}


static void l_destroy_buckets(CatArrayWo **e_buckets, int bucket_count) {
	if (e_buckets==NULL || bucket_count==0) {
		return;
	}
	int bucket_idx;
	for(bucket_idx=0; bucket_idx<bucket_count; bucket_idx++)  {
		cat_unref_ptr(e_buckets[bucket_idx]);
	}
	cat_free_ptr(e_buckets);
}

static CatArrayWo *l_get_bucket(CatHashSetPrivate *priv, GObject *key) {
	if (priv->bucket_count==0) {
		return NULL;
	}
	unsigned int hash = priv->hash_func(key);
	return (CatArrayWo *) priv->e_buckets[hash % priv->bucket_count];
}

static void l_rehash(CatHashSetPrivate *priv, int new_bucket_count, gboolean force) {
	if (new_bucket_count<=3) {
		new_bucket_count=3;
	}
	if (new_bucket_count==priv->bucket_count && !force) {
		return;
	}

	CatArrayWo **e_new_buckets = g_malloc0(sizeof(CatArrayWo *)*new_bucket_count);
	int old_bucket_idx;
	for(old_bucket_idx=0; old_bucket_idx<priv->bucket_count; old_bucket_idx++) {
		CatArrayWo *e_old_bucket = priv->e_buckets[old_bucket_idx];
		if (e_old_bucket) {
			CatIIterator *old_iter = cat_array_wo_iterator(e_old_bucket);
			while(cat_iiterator_has_next(old_iter)) {
				GObject *value = (GObject *) cat_iiterator_next(old_iter);
				unsigned int hash = priv->hash_func(value);
				int bucket_offset = hash%new_bucket_count;
				CatArrayWo *e_new_bucket = e_new_buckets[bucket_offset];
				if (e_new_bucket==NULL) {
					e_new_bucket = cat_array_wo_new();
					e_new_buckets[bucket_offset] = e_new_bucket;
				}
				cat_array_wo_append(e_new_bucket, value);
			}
			cat_unref_ptr(old_iter);
		}
	}


	l_destroy_buckets(priv->e_buckets, priv->bucket_count);
	priv->e_buckets = e_new_buckets;
	priv->bucket_count = new_bucket_count;
}

static void l_check_rehash(CatHashSetPrivate *priv) {
	float nice_bucket_count = (float) priv->total_count * LOAD_FACTOR;
	int wanted_bckt_cnt = ceilf(nice_bucket_count);
	wanted_bckt_cnt = (wanted_bckt_cnt-1+LOAD_FACTOR_INCREASE)/LOAD_FACTOR_INCREASE;
	wanted_bckt_cnt *= LOAD_FACTOR_INCREASE;

	l_rehash(priv, wanted_bckt_cnt, FALSE);
}

GObject *cat_hash_set_add(CatHashSet *hash_set, GObject *to_add) {
	GObject *result = NULL;
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	CatArrayWo *e_bucket = l_get_bucket(priv, to_add);
	if (e_bucket) {
		CatIIterator *bucket_iter = cat_array_wo_iterator(e_bucket);
		while(cat_iiterator_has_next(bucket_iter)) {
			GObject *bucket_entry = cat_iiterator_next(bucket_iter);
			if (priv->equal_func(to_add, bucket_entry)) {
				result = bucket_entry;
				break;
			}
		}
		cat_unref_ptr(bucket_iter);
	} else {
		if (priv->bucket_count==0) {
			priv->bucket_count = 7;
			priv->e_buckets = g_malloc0(sizeof(CatArrayWo *)*priv->bucket_count);
		}
		unsigned int hash = priv->hash_func(to_add);
		e_bucket = cat_array_wo_new();
		priv->e_buckets[hash%priv->bucket_count] = e_bucket;
	}

	if (result == NULL) {
		cat_array_wo_append(e_bucket, to_add);
		priv->modification_count++;
		priv->total_count++;
	}
	l_check_rehash(priv);
	return result;
}




gboolean cat_hash_set_add_all(CatHashSet *hash_set, const CatHashSet *set_to_add) {
	gboolean result = FALSE;
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	CatHashSetPrivate *priv_to_add = cat_hash_set_get_instance_private((CatHashSet *) set_to_add);

	cat_log_debug("hash_set=%p, set_to_add=%p", hash_set,set_to_add);


	int in_buck_idx;
	for(in_buck_idx=priv_to_add->bucket_count-1; in_buck_idx>=0; in_buck_idx--) {
		CatArrayWo *bucket_in = priv_to_add->e_buckets[in_buck_idx];
		cat_log_debug("bucket_in=%o", bucket_in)
		if (bucket_in==NULL) {
			continue;
		}
		CatIIterator *in_buck_iter = cat_array_wo_iterator(bucket_in);
		while(cat_iiterator_has_next(in_buck_iter)) {
			GObject *in_entry = cat_iiterator_next(in_buck_iter);
			cat_log_debug("in_entry=%o", in_entry)


			GObject *existing_entry = NULL;
			CatArrayWo *e_bucket = l_get_bucket(priv, in_entry);
			cat_log_debug("e_bucket=%o", e_bucket)
			if (e_bucket) {
				CatIIterator *present_buck_iter = cat_array_wo_iterator(e_bucket);
				while(cat_iiterator_has_next(present_buck_iter)) {
					GObject *present_entry = cat_iiterator_next(present_buck_iter);
					if (priv->equal_func(in_entry, present_entry)) {
						existing_entry = present_entry;
						break;
					}
				}
				cat_unref_ptr(present_buck_iter);
			} else {
				if (priv->bucket_count==0) {
					priv->bucket_count = priv_to_add->bucket_count;
					priv->e_buckets = g_malloc0(sizeof(CatArrayWo *)*priv->bucket_count);
				}
				unsigned int hash = priv->hash_func(in_entry);
				e_bucket = cat_array_wo_new();
				priv->e_buckets[hash%priv->bucket_count] = e_bucket;
			}

			cat_log_debug("e_bucket=%o, existing_entry=%p", e_bucket, existing_entry)


			if (existing_entry == NULL) {
				cat_array_wo_append(e_bucket, in_entry);
				result = TRUE;
				priv->total_count++;
			}

		}
		cat_unref_ptr(in_buck_iter);


	}

	if (result) {
		priv->modification_count++;
		l_check_rehash(priv);
	}
	return result;
}



GObject *cat_hash_set_get(CatHashSet *hash_set, GObject *to_get) {
	GObject *result = NULL;
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	CatArrayWo *e_bucket = l_get_bucket(priv, to_get);
	if (e_bucket) {
		CatIIterator *bucket_iter = cat_array_wo_iterator(e_bucket);
		while(cat_iiterator_has_next(bucket_iter)) {
			GObject *bucket_entry = cat_iiterator_next(bucket_iter);
			if (priv->equal_func(to_get, bucket_entry)) {
				result = bucket_entry;
				break;
			}
		}
		cat_unref_ptr(bucket_iter);
	}
	return result;
}

GObject *cat_hash_set_remove(CatHashSet *hash_set, GObject *to_remove) {
	GObject *result = NULL;
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	CatArrayWo *e_bucket = l_get_bucket(priv, to_remove);
	if (e_bucket) {
		int idx = cat_array_wo_find_index_ext(e_bucket, to_remove, -1, priv->equal_func);
		if (idx>=0) {
			if (cat_array_wo_remove(e_bucket, idx, &result)) {
				priv->modification_count++;
				priv->total_count--;
			} else {
				cat_log_error("could not remove object form array");	/* should never happen */
			}
		}
	}
	l_check_rehash(priv);
	return result;
}


CatArrayWo *cat_hash_set_enlist_all(CatHashSet *hash_set) {
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	CatArrayWo *e_result = cat_array_wo_new();
	int bucket_idx;
	for(bucket_idx=0; bucket_idx<priv->bucket_count; bucket_idx++) {
		CatArrayWo *e_bucket = priv->e_buckets[bucket_idx];
		if (e_bucket) {
			cat_array_wo_append_all(e_result, e_bucket);
		}
	}
	return e_result;
}


int cat_hash_set_add_all_from_list(CatHashSet *hash_set, CatArrayWo *list_to_add) {
	int result = 0;
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	CatIIterator *iter = cat_array_wo_iterator(list_to_add);
	while(cat_iiterator_has_next(iter)) {
		GObject *to_add = cat_iiterator_next(iter);
		if (cat_hash_set_add(hash_set, to_add)==NULL) {
			result++;
			if ((result%50) == 0) {
				l_check_rehash(priv);
			}
		}
	}
	cat_unref_ptr(iter);
	l_check_rehash(priv);
	return result;
}


CatIIterator *cat_hash_set_iterator(CatHashSet *hash_set) {
	CatHashSetIter *iter = g_object_new(CAT_TYPE_HASH_SET_ITER, NULL);
	cat_ref_anounce(iter);
	iter->hash_set = cat_ref_ptr(hash_set);
	iter->bucket_idx = 0;
	iter->bucket_offset = 0;
	CatIIterator *result = (CatIIterator *) iter;
	l_iter_next(result);	/* initialize next_value */
	l_iter_next(result);	/* initialize ahead_value */
	return result;
}


gboolean cat_hash_set_intersection(CatHashSet *hash_set, CatHashSet *to_intersect_with) {
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(hash_set);
	int remove_count = 0;
	int bucket_idx, bucket_offset;
	for(bucket_idx=priv->bucket_count-1; bucket_idx>=0; bucket_idx--) {
		CatArrayWo *e_bucket = priv->e_buckets[bucket_idx];
		if (e_bucket!=NULL) {
			for(bucket_offset=cat_array_wo_size(e_bucket)-1; bucket_offset>=0; bucket_offset--) {
				GObject *test_entry = cat_array_wo_get(e_bucket, bucket_offset);
				if (!cat_hash_set_get(to_intersect_with, test_entry)) {
					cat_array_wo_remove(e_bucket, bucket_offset, NULL);
					remove_count++;
				}
			}
			if (cat_array_wo_size(e_bucket)==0) {
				cat_unref_ptr(priv->e_buckets[bucket_idx]);
			}
		}
	}
	gboolean result = FALSE;
	if (remove_count>0) {
		result = TRUE;
		priv->total_count-=remove_count;
		priv->modification_count++;
		l_check_rehash(priv);
	}
	return result;
}


gboolean cat_hash_set_equal(const CatHashSet *hash_set_a, const CatHashSet *hash_set_b) {
	if (hash_set_a==hash_set_b) {
		return TRUE;
	}
	if (hash_set_a==NULL || hash_set_b==NULL) {
		return FALSE;
	}

	CatHashSetPrivate *priv_a = cat_hash_set_get_instance_private((CatHashSet *) hash_set_a);
	CatHashSetPrivate *priv_b = cat_hash_set_get_instance_private((CatHashSet *) hash_set_b);
	if (priv_a->total_count!=priv_b->total_count) {
		return FALSE;
	}

	gboolean result = TRUE;
	int a_bucket_idx;
	for(a_bucket_idx=priv_a->bucket_count-1; result && a_bucket_idx>=0; a_bucket_idx--) {
		CatArrayWo *a_bucket = priv_a->e_buckets[a_bucket_idx];
		if (a_bucket!=NULL) {
			CatIIterator *a_iter = cat_array_wo_iterator(a_bucket);
			while(result && cat_iiterator_has_next(a_iter)) {
				GObject *a_val = (GObject *) cat_iiterator_next(a_iter);
				unsigned int hash = priv_a->hash_func(a_val);
				CatArrayWo *b_bucket = (CatArrayWo *) priv_b->e_buckets[hash % priv_b->bucket_count];


				gboolean did_find = FALSE;
				if (b_bucket!=NULL) {
					CatIIterator *b_iter = cat_array_wo_iterator(b_bucket);
					while(cat_iiterator_has_next(b_iter)) {
						GObject *b_val = (GObject *) cat_iiterator_next(b_iter);
						if (priv_a->equal_func(a_val, b_val)) {
							did_find = TRUE;
						}
					}
					cat_unref_ptr(b_iter);
				}
				if (!did_find) {
					result = FALSE;
				}

			}
			cat_unref_ptr(a_iter);
		}
	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private((CatHashSet *) self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: size=%d", iname, self, priv->total_count);

	CatIIterator *iter = cat_hash_set_iterator((CatHashSet *) self);
	int idx = 0;
	while(cat_iiterator_has_next(iter) && idx++<5) {
		GObject *obj = cat_iiterator_next(iter);
		cat_string_wo_format(append_to, ", %o", obj);
	}
	if (cat_iiterator_has_next(iter)) {
		cat_string_wo_append_chars(append_to, ", ...");
	}
	cat_unref_ptr(iter);
	cat_string_wo_append_chars(append_to, "]");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/




static gboolean l_iter_has_next(CatIIterator *self) {
	CatHashSetIter *iter = CAT_HASH_SET_ITER(self);
	return iter->next_value_valid;
}

static gboolean l_iter_is_last(CatIIterator *self) {
	CatHashSetIter *iter = CAT_HASH_SET_ITER(self);
	return iter->next_value_valid && !iter->ahead_value_valid;
}

static gpointer l_iter_next(CatIIterator *self) {
	CatHashSetIter *iter = CAT_HASH_SET_ITER(self);
	gpointer result = iter->next_value;

	iter->next_value = iter->ahead_value;
	iter->next_value_valid = iter->ahead_value_valid;

	iter->ahead_value = NULL;
	iter->ahead_value_valid = FALSE;
	CatHashSetPrivate *priv = cat_hash_set_get_instance_private(iter->hash_set);


	while(iter->bucket_idx<priv->bucket_count) {
		CatArrayWo *e_bucket = priv->e_buckets[iter->bucket_idx];
		if (e_bucket) {
			if (iter->bucket_offset<cat_array_wo_size(e_bucket)) {
				iter->ahead_value = cat_array_wo_get(e_bucket, iter->bucket_offset++);
				iter->ahead_value_valid = TRUE;
				break;
			} else {
				iter->bucket_idx++;
				iter->bucket_offset = 0;
			}
		} else {
			iter->bucket_idx++;
			iter->bucket_offset = 0;
		}
	}
	return result;
}
