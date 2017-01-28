/*
   File:    catatomicreference.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    May 9, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "catatomicreference.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatAtomicReference"
#include "../logging/catlog.h"

struct _CatAtomicReferencePrivate {
	void *value;
	int lock;
};

G_DEFINE_TYPE (CatAtomicReference, cat_atomic_reference, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_atomic_reference_class_init(CatAtomicReferenceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(CatAtomicReferencePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_atomic_reference_init(CatAtomicReference *instance) {
	CatAtomicReferencePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_ATOMIC_REFERENCE, CatAtomicReferencePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatAtomicReference *instance = CAT_ATOMIC_REFERENCE(object);
	CatAtomicReferencePrivate *priv = instance->priv;
	cat_unref_ptr(priv->value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatAtomicReference *cat_atomic_reference_new() {
	CatAtomicReference *result = g_object_new(CAT_TYPE_ATOMIC_REFERENCE, NULL);
	cat_ref_anounce(result);
	CatAtomicReferencePrivate *priv = result->priv;
	__atomic_store_n(&(priv->lock), 0, __ATOMIC_SEQ_CST);
	__atomic_store_n((void **) &(priv->value), NULL, __ATOMIC_SEQ_CST);
	return result;
}


CatAtomicReference *cat_atomic_reference_new_val(GObject *initial) {
	CatAtomicReference *result = g_object_new(CAT_TYPE_ATOMIC_REFERENCE, NULL);
	cat_ref_anounce(result);
	CatAtomicReferencePrivate *priv = result->priv;
	__atomic_store_n(&(priv->lock), 0, __ATOMIC_SEQ_CST);
	__atomic_store_n((void **) &(priv->value), initial, __ATOMIC_SEQ_CST);
	cat_ref_ptr(initial);
	return result;
}



gboolean cat_atomic_reference_compare_set(CatAtomicReference *ref_atomic, GObject *expect, GObject *set) {
	CatAtomicReferencePrivate *priv = CAT_ATOMIC_REFERENCE_GET_PRIVATE(ref_atomic);
	// set or increase write lock
	cat_log_trace("ref_atomic=%o, compare=%o, set=%o", ref_atomic, expect, set);
	while(TRUE) {
		int lock_val = __atomic_load_n(&(priv->lock), __ATOMIC_SEQ_CST);
		cat_log_trace("lock_val=%d", lock_val);
		if (lock_val==0) {
			if (__atomic_compare_exchange_n(&(priv->lock), &lock_val, lock_val-1, FALSE, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
				break;
			}
		}
	}
	gboolean result = __atomic_compare_exchange_n((void **) &(priv->value), &expect, set, FALSE, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	if (result) {
		cat_ref_sink_ptr(set);
		cat_unref_ptr(expect);
	}
	__atomic_add_fetch(&(priv->lock), 1, __ATOMIC_SEQ_CST);
	cat_log_debug("result=%d", result);
	return result;
}

GObject *cat_atomic_reference_get(CatAtomicReference *ref_atomic) {
	cat_log_trace("ref_atomic=%o", ref_atomic);
	CatAtomicReferencePrivate *priv = CAT_ATOMIC_REFERENCE_GET_PRIVATE(ref_atomic);
	// set or increase read lock
	while(TRUE) {
		int lock_val = __atomic_load_n(&(priv->lock), __ATOMIC_SEQ_CST);
		cat_log_trace("lock_val=%d", lock_val);
		if (lock_val>=0) {
			if (__atomic_compare_exchange_n(&(priv->lock), &lock_val, lock_val+1, FALSE, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
				break;
			}
		}
	}
	void *result = __atomic_load_n((void **) &(priv->value), __ATOMIC_SEQ_CST);
	cat_ref_ptr(result);


	cat_log_on_debug( int new_lock = )
	__atomic_sub_fetch(&(priv->lock), 1, __ATOMIC_SEQ_CST);

	cat_log_debug("result=%o, new_lock=%d", result, new_lock);
	return (GObject *) result;
}

GObject *cat_atomic_reference_get_unsafe(CatAtomicReference *ref_atomic) {
	cat_log_trace("ref_atomic=%o", ref_atomic);
	CatAtomicReferencePrivate *priv = CAT_ATOMIC_REFERENCE_GET_PRIVATE(ref_atomic);
	void *result = __atomic_load_n((void **) &(priv->value), __ATOMIC_SEQ_CST);
	cat_log_debug("result=%o, new_lock=%d", result);
	return (GObject *) result;
}


gboolean cat_atomic_reference_set(CatAtomicReference *ref_atomic, GObject *newVal) {
	CatAtomicReferencePrivate *priv = CAT_ATOMIC_REFERENCE_GET_PRIVATE(ref_atomic);
	gboolean ret = FALSE;
	// set or increase write lock
	cat_log_trace("ref_atomic=%o, set=%o", ref_atomic, newVal);
	while(TRUE) {
		int lock_val = __atomic_load_n(&(priv->lock), __ATOMIC_SEQ_CST);
		cat_log_trace("lock_val=%d", lock_val);
		if (lock_val==0) {
			if (__atomic_compare_exchange_n(&(priv->lock), &lock_val, lock_val-1, FALSE, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
				break;
			}
		}
	}
	GObject *result = __atomic_exchange_n((void **) &(priv->value), (newVal), __ATOMIC_SEQ_CST);
	cat_log_debug("result=%p, value=%p", result, priv->value);
	if (result!=newVal) {
		cat_ref_sink_ptr(newVal);
		cat_unref_ptr(result);
		ret = TRUE;
	}
	cat_log_trace("unlocking");
	__atomic_add_fetch(&(priv->lock), 1, __ATOMIC_SEQ_CST);
	cat_log_trace("unlocked");
	return ret;
}


