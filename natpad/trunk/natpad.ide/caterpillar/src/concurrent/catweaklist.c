/*
   File:    catweaklist.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 25, 2013
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


#include "catweaklist.h"
#include "catweakreference.h"
#include "catlock.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatWeakList"
#include "../logging/catlog.h"

struct _CatWeakListPrivate {
	CatLock *lock;
	CatArrayWo *e_weak_list;
};

G_DEFINE_TYPE(CatWeakList, cat_weak_list, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_weak_list_class_init(CatWeakListClass *clazz) {
	g_type_class_add_private(clazz, sizeof(CatWeakListPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_weak_list_init(CatWeakList *instance) {
	CatWeakListPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_WEAK_LIST, CatWeakListPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatWeakList *instance = CAT_WEAK_LIST(object);
	CatWeakListPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_weak_list);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(cat_weak_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_weak_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatWeakList *cat_weak_list_new() {
	CatWeakList *result = g_object_new(CAT_TYPE_WEAK_LIST, NULL);
	cat_ref_anounce(result);
	CatWeakListPrivate *priv = result->priv;
	priv->lock = cat_lock_new();
	priv->e_weak_list = cat_array_wo_new();
	return result;
}

int cat_weak_list_size(CatWeakList *weak_list) {
	int result = 0;
	CatWeakListPrivate *priv = CAT_WEAK_LIST_GET_PRIVATE(weak_list);
	cat_lock_lock(priv->lock);
	result = cat_array_wo_size(priv->e_weak_list);
	cat_lock_unlock(priv->lock);
	return result;
}

void cat_weak_list_append(CatWeakList *weak_list, GObject *ref) {
	if (ref==NULL) {
		return;
	}
	CatWeakListPrivate *priv = CAT_WEAK_LIST_GET_PRIVATE(weak_list);
	cat_lock_lock(priv->lock);
	CatWeakReference *wref = cat_weak_reference_new(ref);
	cat_array_wo_append(priv->e_weak_list, (GObject *) wref);
	cat_unref_ptr(wref);
	cat_lock_unlock(priv->lock);
}

gboolean cat_weak_list_append_once(CatWeakList *weak_list, GObject *ref) {
	if (ref==NULL) {
		return FALSE;
	}
	CatWeakListPrivate *priv = CAT_WEAK_LIST_GET_PRIVATE(weak_list);
	gboolean result = TRUE;
	cat_lock_lock(priv->lock);

	int idx;
	for(idx=cat_array_wo_size(priv->e_weak_list)-1; idx>=0; idx--) {
		CatWeakReference *weak_ref = (CatWeakReference *) cat_array_wo_get(priv->e_weak_list, idx);
		GObject *strong_ref = cat_weak_reference_get(weak_ref);
		if (strong_ref) {
			cat_unref(strong_ref);
			if (strong_ref==ref) {
				result = FALSE;
				break;
			}
		} else {
			cat_array_wo_remove(priv->e_weak_list, idx, NULL);
		}
	}

	if (result) {
		CatWeakReference *wref = cat_weak_reference_new(ref);
		cat_array_wo_append(priv->e_weak_list, (GObject *) wref);
		cat_unref_ptr(wref);
	}
	cat_lock_unlock(priv->lock);
	return result;
}


gboolean cat_weak_list_remove(CatWeakList *weak_list, GObject *ref) {
	gboolean result = FALSE;
	CatWeakListPrivate *priv = CAT_WEAK_LIST_GET_PRIVATE(weak_list);
	cat_lock_lock(priv->lock);
	int idx;
	for(idx=cat_array_wo_size(priv->e_weak_list)-1; idx>=0; idx--) {
		CatWeakReference *weak_ref = (CatWeakReference *) cat_array_wo_get(priv->e_weak_list, idx);
		GObject *strong_ref = cat_weak_reference_get(weak_ref);
		if (strong_ref) {
			cat_unref(strong_ref);
			if (strong_ref==ref) {
				cat_array_wo_remove(priv->e_weak_list, idx, NULL);
				result = TRUE;
				break;
			}
		} else {
			cat_array_wo_remove(priv->e_weak_list, idx, NULL);
		}
	}
	cat_lock_unlock(priv->lock);
	return result;
}


CatArrayWo *cat_weak_list_enlist(CatWeakList *weak_list, CatArrayWo *e_enlist_to) {
	CatWeakListPrivate *priv = CAT_WEAK_LIST_GET_PRIVATE(weak_list);
	if (e_enlist_to==NULL) {
		e_enlist_to = cat_array_wo_new();
	}
	cat_lock_lock(priv->lock);
	int idx;
	for(idx=0; idx<cat_array_wo_size(priv->e_weak_list); idx++) {
		CatWeakReference *weak_ref = (CatWeakReference *) cat_array_wo_get(priv->e_weak_list, idx);
		GObject *strong_ref = cat_weak_reference_get(weak_ref);
		if (strong_ref) {
			cat_array_wo_append(e_enlist_to, strong_ref);
			cat_unref(strong_ref);
		} else {
			cat_array_wo_remove(priv->e_weak_list, idx, NULL);
			idx--;
		}
	}
	cat_lock_unlock(priv->lock);
	return e_enlist_to;
}


CatIIterator *cat_weak_list_iterator(CatWeakList *weak_list) {
	CatArrayWo *strong_list = cat_weak_list_enlist(weak_list, NULL);
	CatIIterator *result = cat_array_wo_iterator(strong_list);
	cat_unref_ptr(strong_list);
	return result;
}




