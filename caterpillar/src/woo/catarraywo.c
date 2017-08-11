/*
   File:    catarraywo.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 6, 2014
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

#include "catarraywoprivate.h"
#include "../catistringable.h"
#include "catstringwo.h"

#include <string.h>

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatArrayWo"
#include "../logging/catlog.h"


typedef struct _CatArrayWoIter	      CatArrayWoIter;
typedef struct _CatArrayWoIterClass	  CatArrayWoIterClass;

struct _CatArrayWoIter {
	GObject parent;
	CatArrayWo *array;
	int size;
	int offset;
};

GType cat_array_wo_iter_get_type(void);
#define CAT_TYPE_ARRAY_WO_ITER            (cat_array_wo_iter_get_type())


typedef struct _CatArrayWoInfo CatArrayWoInfo;

struct _CatArrayWoPrivate {
	void **data;
	int data_size;
	int size;
	int modification_count;
	int resize_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatArrayWo, cat_array_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(CatArrayWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
//static gboolean l_check_for_modifications(CatWo *wo);
static void l_wo_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);
static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info);


static void cat_array_wo_class_init(CatArrayWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_wo_construct_editable;
//	wo_class->checkForModifications = l_check_for_modifications;
	wo_class->anchorChildren = l_wo_anchor_children;
	wo_class->cloneContent = l_clone_content;
	wo_class->createEditInfo = l_create_edit_info;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_array_wo_init(CatArrayWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatArrayWo *instance = CAT_ARRAY_WO(object);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(instance);

	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) instance);
	if (array_info) {
		if (array_info->original_data == priv->data) {
			priv->data = NULL;
			priv->data_size = 0;
		}
	}

	if (priv->data) {
		void **d_data = priv->data;
		int d_size = priv->size;
		priv->data = NULL;
		priv->data_size = 0;
		int idx;
		for(idx=d_size-1; idx>=0; idx--) {
			cat_unref_ptr(d_data[idx]);
		}
		cat_free_ptr(d_data);
	}
	priv->size = 0;

	G_OBJECT_CLASS(cat_array_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_array_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatArrayWo *cat_array_wo_new() {
	CatArrayWo *result = g_object_new(CAT_TYPE_ARRAY_WO, NULL);
	cat_ref_anounce(result);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = NULL;
	priv->data_size = 0;
	priv->size = 0;
	priv->modification_count = 0;
	priv->resize_count = 8;
	return result;
}

CatArrayWo *cat_array_wo_new_size(int initial_size) {
	CatArrayWo *result = g_object_new(CAT_TYPE_ARRAY_WO, NULL);
	cat_ref_anounce(result);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = initial_size<=0 ? NULL : g_malloc(initial_size*sizeof(GObject *));
	priv->data_size = initial_size<=0 ? 0 : initial_size;
	priv->size = 0;
	priv->modification_count = 0;
	priv->resize_count = 8;
	return result;
}

CatArrayWo *cat_array_wo_new_sub(const CatArrayWo *source, int offset, int length) {
	CatArrayWo *result = g_object_new(CAT_TYPE_ARRAY_WO, NULL);
	cat_ref_anounce(result);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = length<=0 ? NULL : g_malloc0(length*sizeof(GObject *));
	priv->data_size = length<=0 ? 0 : length;
	priv->size = priv->data_size;
	priv->modification_count = 0;
	priv->resize_count = 8;
	const CatArrayWoPrivate *spriv = cat_array_wo_get_instance_private((CatArrayWo *) source);
	int out = 0;
	while(out<length) {
		if (offset>=0 && offset<spriv->data_size) {
			priv->data[out] = cat_ref_ptr(spriv->data[offset]);
		} else {
			priv->data[out] = NULL;
		}
		out++;
		offset++;
	}
	return result;
}


int cat_array_wo_size(CatArrayWo *array) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(array);
	return priv->size;
}


GObject *cat_array_wo_get(CatArrayWo *array, int index) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(array);
	if (index>=0 && index<priv->size) {
		return priv->data[index];
	}
	return NULL;
}

GObject *cat_array_wo_get_first(CatArrayWo *array) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(array);
	if (priv->size>0) {
		return priv->data[0];
	}
	return NULL;
}

GObject *cat_array_wo_get_last(CatArrayWo *array) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(array);
	if (priv->size>0) {
		return priv->data[priv->size-1];
	}
	return NULL;
}

CatIIterator *cat_array_wo_iterator(CatArrayWo *array) {
	CatArrayWoIter *result = g_object_new(CAT_TYPE_ARRAY_WO_ITER, NULL);
	cat_ref_anounce(result);
	result->array = cat_ref_ptr(array);
	result->offset = 0;
	result->size = cat_array_wo_size(array);
	return (CatIIterator *) result;
}

int cat_array_wo_find_index(CatArrayWo *array, const void *search_item, int hint_index) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(array);
	cat_log_detail("array=%p", array);
	if (hint_index>=0 && hint_index<priv->size) {
		if (priv->data[hint_index]==search_item) {
			return hint_index;
		}
	}
	int idx=0;
	for(idx=0; idx<priv->size; idx++) {
		if (priv->data[idx]==search_item) {
			return idx;
		}
	}
	return -1;
}


int cat_array_wo_find_index_ext(CatArrayWo *array, const void *search_item, int hint_index, GEqualFunc equal_func) {
	g_return_val_if_fail(equal_func!=NULL, -1);
	cat_log_detail("cat_array_find_index:array=%p", array);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(array);
	if (hint_index>=0 && hint_index<priv->size) {
		if (equal_func(priv->data[hint_index], search_item)) {
			return hint_index;
		}
	}
	int idx=0;
	for(idx=0; idx<priv->size; idx++) {
		if (equal_func(priv->data[idx], search_item)) {
			return idx;
		}
	}
	return -1;
}





#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_array)) { \
			cat_log_error("Object is read only:%o", e_array); \
			return rval; \
		} \



void cat_array_wo_clear(CatArrayWo *e_array) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	CHECK_IF_WRITABLE()
	if (priv->size<=0) {
		return;
	}

	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		/* its the original data */
		priv->data = NULL;
		priv->size = 0;
		priv->data_size = 0;
		priv->modification_count++;
		return;
	}

	int index;
	for(index = priv->size-1; index>=0; index--) {
		cat_unref_ptr(priv->data[index]);
	}
	priv->size = 0;
	priv->modification_count++;
}


static void l_ensure_size(CatArrayWo *e_array, int minimal_size) {
	cat_log_debug("minimal_size=%d", minimal_size);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	cat_log_detail("array_info=%p, original_data=%p, priv->data=%p", array_info, array_info->original_data, priv->data);
	if (array_info->original_data==priv->data) {
		minimal_size += priv->resize_count;
		if (minimal_size>priv->data_size) {
			priv->data_size = minimal_size;
		}

		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int idx;
		for(idx=priv->size-1; idx>=0; idx--) {
			priv->data[idx] = cat_ref_ptr(array_info->original_data[idx]);
		}
		return;
	}

	if (priv->data_size>=minimal_size) {
		return;
	}

	minimal_size += priv->resize_count;
	if (priv->data_size==0) {
		priv->data_size = minimal_size;
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
	} else {
		priv->data_size += minimal_size;
		priv->data = g_realloc(priv->data, priv->data_size*sizeof(GObject*));
	}
}

int cat_array_wo_append(CatArrayWo *e_array, GObject *to_add) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	CHECK_IF_WRITABLE(-1)
	l_ensure_size(e_array, priv->size+1);
	priv->data[priv->size] = cat_ref_ptr(to_add);
	priv->modification_count++;
	priv->size++;
	return priv->size-1;
}

void cat_array_wo_append_all(CatArrayWo *e_array, CatArrayWo *list_to_append) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	CatArrayWoPrivate *apriv = cat_array_wo_get_instance_private(list_to_append);
	if (apriv->size==0) {
		return;
	}
	CHECK_IF_WRITABLE()
	l_ensure_size(e_array, priv->size+apriv->size);

	void **out = priv->data+priv->size;
	void **in = apriv->data;
	int idx;
	for(idx=apriv->size; idx>0; idx--) {
		*out = cat_ref_ptr(*in);
		out++;
		in++;
	}
	priv->size += apriv->size;
	priv->modification_count++;
}

void cat_array_wo_insert(CatArrayWo *e_array, GObject *to_insert, int index) {
	cat_log_debug("e_array=%p, index=%d, to_insert=%p", e_array, index, to_insert);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (index>=priv->size) {
		cat_array_wo_append(e_array, to_insert);
		return;
	}
	CHECK_IF_WRITABLE()
	if (index<0) {
		index = 0;
	}

	l_ensure_size(e_array, priv->size+1);
	memmove(priv->data+index+1, priv->data+index, (priv->size-index)*sizeof(void *));
	priv->data[index] = cat_ref_ptr(to_insert);
	priv->modification_count++;
	priv->size++;
}




gboolean cat_array_wo_remove(CatArrayWo *e_array, int index, GObject **removed) {
	cat_log_debug("e_array=%p, index=%d, removed=%p", e_array, index, removed);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (index<0 || index>=priv->size) {
		if (removed!=NULL) {
			*removed = NULL;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)

	if (removed!=NULL) {
		*removed = cat_ref_ptr(priv->data[index]);
	}

	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int in_idx = priv->size-1;
		int out_idx = priv->size-2;
		while(in_idx>=0) {
			if (in_idx==index) {
				in_idx--;
				continue;
			}
			priv->data[out_idx] = cat_ref_ptr(array_info->original_data[in_idx]);
			in_idx--;
			out_idx--;
		}
	} else {
		cat_unref_ptr(priv->data[index]);
		memmove(priv->data+index, priv->data+index+1, (priv->size-(index+1))*sizeof(void *));
	}

	priv->size--;
	priv->modification_count++;
	return TRUE;
}

void cat_array_wo_remove_range(CatArrayWo *e_array, int offset, int length) {
	cat_log_debug("e_array=%p, offset=%d, length=%d", e_array, offset, length);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	CHECK_IF_WRITABLE()


	if (offset>=priv->size || offset+length<0 || length<=0) {
		return;
	}

	if (offset+length>priv->size) {
		length = priv->size-offset;
	}

	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);

	if (array_info->original_data==priv->data) {
		priv->data_size = priv->size-length;
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int out_idx=0;
		int in_idx;
		for(in_idx=0; in_idx<offset; in_idx++) {
			priv->data[out_idx] = cat_ref_ptr(array_info->original_data[in_idx]);
			out_idx++;
		}
		for(in_idx=offset+length; in_idx<priv->size; in_idx++) {
			priv->data[out_idx] = cat_ref_ptr(array_info->original_data[in_idx]);
			out_idx++;
		}
		cat_log_detail("out_idx=%d, size=%d", out_idx, priv->size);
		priv->size = priv->size-length;
	} else {
		int end_off = offset+length;
		int in_idx;
		for(in_idx=offset; in_idx<end_off; in_idx++) {
			cat_unref_ptr(priv->data[in_idx]);
		}
		memmove(priv->data+offset, priv->data+end_off, (priv->size-(end_off))*sizeof(void *));
		priv->size = priv->size-length;
	}
	priv->modification_count++;
}


gboolean cat_array_wo_remove_last(CatArrayWo *e_array, GObject **removed) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (priv->size<=0) {
		if (removed!=NULL) {
			*removed = NULL;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)
	if (removed!=NULL) {
		*removed = cat_ref_ptr(priv->data[priv->size-1]);
	}
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int idx = priv->size-2;
		while(idx>=0) {
			priv->data[idx] = cat_ref_ptr(array_info->original_data[idx]);
			idx--;
		}
	} else {
		cat_unref_ptr(priv->data[priv->size-1]);
	}
	priv->size--;
	priv->modification_count++;
	return TRUE;
}

gboolean cat_array_wo_remove_first(CatArrayWo *e_array, GObject **removed) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (priv->size<=0) {
		if (removed!=NULL) {
			*removed = NULL;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)
	if (removed!=NULL) {
		*removed = cat_ref_ptr(priv->data[0]);
	}
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int idx = priv->size-2;
		while(idx>=0) {
			priv->data[idx] = cat_ref_ptr(array_info->original_data[idx]);
			idx--;
		}
	} else {
		cat_unref_ptr(priv->data[0]);
		memmove(priv->data, priv->data+1, (priv->size-1)*sizeof(void *));
	}
	priv->size--;
	priv->modification_count++;
	return TRUE;
}

gboolean cat_array_wo_set(CatArrayWo *e_array, GObject *to_insert, int index, GObject **old_value) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (index<0 || index>=priv->size) {
		if (old_value!=NULL) {
			*old_value = NULL;
		}
		return FALSE;
	}

	if (priv->data[index] == to_insert) {
		if (old_value!=NULL) {
			*old_value = NULL;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)
	if (old_value!=NULL) {
		*old_value = cat_ref_ptr(priv->data[index]);
	}
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int idx = priv->size-1;
		while(idx>=0) {
			priv->data[idx] = cat_ref_ptr(array_info->original_data[idx]);
			idx--;
		}
	}
	cat_ref_swap(priv->data[index], to_insert);
	priv->modification_count++;
	return TRUE;
}

void cat_array_wo_limit(CatArrayWo *e_array, int offset, int length) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (offset<0) {
		length += offset;
		offset = 0;
	}
	if (length>priv->size) {
		length = priv->size;
	}
	if ((length==priv->size) && (offset==0)) {
		return;
	}
	CHECK_IF_WRITABLE()
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(length*sizeof(GObject*));
		priv->data_size = length;
		priv->size = length;
		int idx = length-1;
		while(idx>=0) {
			priv->data[idx] = cat_ref_ptr(array_info->original_data[offset+idx]);
			idx--;
		}
		priv->modification_count++;
		return;
	}

	int idx;
	for(idx=offset-1; idx>=0; idx--) {
		cat_unref_ptr(priv->data[idx]);
	}
	for(idx=offset+length; idx<priv->size; idx++) {
		cat_unref_ptr(priv->data[idx]);
	}

	memmove(priv->data, priv->data+offset, length*sizeof(void *));
	priv->size = length;
	priv->modification_count++;
}

void cat_array_wo_pad(CatArrayWo *e_array, int length, GObject *fill_with) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (length<=priv->size) {
		return;
	}

	CHECK_IF_WRITABLE()
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(length*sizeof(GObject*));
		priv->data_size = length;
		int idx = priv->size-1;
		while(idx>=0) {
			priv->data[idx] = cat_ref_ptr(array_info->original_data[idx]);
			idx--;
		}
	} else {
		l_ensure_size(e_array, length);
	}

	int idx = priv->size;
	while(idx<length) {
		priv->data[idx] = cat_ref_ptr(fill_with);
		idx++;
	}

	priv->modification_count++;
	priv->size = length;
	return;
}


static gint l_sort_unref_compare(void **a, void **b, gpointer user_data) {
	const GCompareFunc cmp_fn = (GCompareFunc) user_data;
	return cmp_fn(*a, *b);
}

void cat_array_wo_sort(CatArrayWo *e_array, GCompareFunc cmp_func) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (priv->size<=0) {
		return;
	}
	CHECK_IF_WRITABLE()
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int idx = priv->size-1;
		while(idx>=0) {
			priv->data[idx] = cat_ref_ptr(array_info->original_data[idx]);
			idx--;
		}
	}
	priv->modification_count++;
//	qsort(priv->data, priv->size, sizeof(gpointer), cmp_func);
	if (priv->size>0) {
		GPtrArray ptr_array;
		ptr_array.pdata = priv->data;
		ptr_array.len = priv->size;
		g_ptr_array_sort_with_data(&ptr_array, (GCompareDataFunc) l_sort_unref_compare, cmp_func);
	}
}

void cat_array_wo_reverse(CatArrayWo *e_array) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(e_array);
	if (priv->size<=0) {
		return;
	}
	CHECK_IF_WRITABLE()
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(GObject*));
		int idx = priv->size-1;
		int out = 0;
		while(idx>=0) {
			priv->data[out] = cat_ref_ptr(array_info->original_data[idx]);
			idx--;
			out++;
		}
	} else {
		int start = 0;
		int end = priv->size-1;
		void *swap;
		while(start<end) {
			swap = priv->data[start];
			priv->data[start] = priv->data[end];
			priv->data[end] = swap;
			start++;
			end--;
		}
	}
	priv->modification_count++;
}




CatArrayWo *cat_array_wo_create_editable(CatArrayWo *instance) {
	return (CatArrayWo *) cat_wo_create_editable((CatWo *) instance);
}

CatArrayWo *cat_array_wo_anchor(CatArrayWo *instance, int version) {
	return (CatArrayWo *) cat_wo_anchor((CatWo *) instance, version);
}

CatArrayWo *cat_array_wo_get_original(CatArrayWo *instance) {
	return (CatArrayWo *) cat_wo_get_original((CatWo *) instance);
}

gboolean cat_array_wo_is_anchored(const CatArrayWo *instance) {
	return cat_wo_is_anchored((const CatWo *) instance);
}

gboolean cat_array_wo_equal(const CatArrayWo *instance_a, const CatArrayWo *instance_b, GEqualFunc equal_func) {
	if (instance_a==instance_b) {
		return TRUE;
	}
	if (instance_a==NULL || instance_b==NULL) {
		return FALSE;
	}
	const CatArrayWoPrivate *priv_a = cat_array_wo_get_instance_private((CatArrayWo *) instance_a);
	const CatArrayWoPrivate *priv_b = cat_array_wo_get_instance_private((CatArrayWo *) instance_b);
	if (priv_a->size!=priv_b->size) {
		return FALSE;
	}
	/* we assume the memcmp function to be extremely fast */
	if (memcmp(priv_a->data, priv_b->data, sizeof(void *) * priv_a->size)==0) {
		return TRUE;
	}
	int idx = priv_a->size;
	for(idx--; idx>=0; idx--) {
		void *da = priv_a->data[idx];
		void *db = priv_b->data[idx];
		if (da==db) {
			continue;
		} else if (equal_func) {
			if (!equal_func(da, db)) {
				return FALSE;
			}
		} else if (CAT_IS_WO(da) && CAT_IS_WO(db)) {
			if (!cat_wo_equal((CatWo *) da, (CatWo *) db)) {
				return FALSE;
			}
		} else {
			return FALSE;
		}
	}
	return TRUE;
}

CatArrayWo *cat_array_wo_clone(const CatArrayWo *source, CatCloneDepth clone_depth) {
	return (CatArrayWo *) cat_wo_clone((CatWo *) source, clone_depth);
}


static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(CatArrayWoInfo, 1);
	}
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_ARRAY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private((CatArrayWo *) e_uninitialized);
	CatArrayWoInfo *array_info = (CatArrayWoInfo *) info;
	if (original) {
		CatArrayWoPrivate *rpriv = cat_array_wo_get_instance_private((CatArrayWo *) original);
		priv->data = rpriv->data;
		priv->data_size = rpriv->data_size;
		priv->modification_count = rpriv->modification_count;
		priv->size = rpriv->size;
		priv->resize_count = rpriv->resize_count;
		array_info->original_data = rpriv->data;
	} else {
		priv->data = NULL;
		priv->data_size = 0;
		priv->modification_count = 0;
		priv->size = 0;
		priv->resize_count = 8;
		array_info->original_data = NULL;
	}
	return CAT_WO_CLASS(cat_array_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_wo_anchor_children(CatWo *wo, int version) {
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private((CatArrayWo *) wo);
	if (priv->data && priv->size>0) {
		CatArrayWoInfo *array_info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) wo);
		if (array_info && array_info->original_data==priv->data) {
			return;
		}

		int idx;
		for(idx=priv->size-1; idx>=0; idx--) {
			void *data = priv->data[idx];
			if (CAT_IS_WO(data)) {
				CatWo *child_wo = (CatWo *) data;
				if (!cat_wo_is_anchored(child_wo)) {
					priv->data[idx] = cat_wo_anchor(child_wo, version);
				}
			}
		}
	}
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	return cat_array_wo_equal((const CatArrayWo *) wo_a, (const CatArrayWo *) wo_b, NULL);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_ARRAY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(CAT_ARRAY_WO(e_uninitialized));
	if (wo_source) {
		CatArrayWoPrivate *priv_src = cat_array_wo_get_instance_private(CAT_ARRAY_WO(wo_source));
		CatArrayWoInfo *info = (CatArrayWoInfo *) cat_wo_get_info((CatWo *) wo_source);
		if (info!=NULL && info->original_data==priv_src->data) {
			priv->data = priv_src->data;
		} else if (priv_src->size>0) {

			int idx = priv_src->size;
			priv->data = g_malloc(idx*sizeof(GObject *));
			for(idx--; idx>=0; idx--) {
				void *val = priv_src->data[idx];
				cat_log_debug("should clone:%o", val);
				if (CAT_IS_WO(val)) {
					priv->data[idx] = cat_wo_clone((CatWo *) val, CAT_CLONE_DEPTH_NONE);
				} else {
					priv->data[idx] = cat_ref_ptr(val);
				}
				cat_log_debug("cloned:%o", priv->data[idx]);
			}

		} else {
			priv->data = NULL;
		}
		priv->size = priv_src->size;
		priv->data_size = priv->size;
		priv->modification_count = priv_src->modification_count;
		priv->resize_count = priv_src->resize_count;
	} else {
		priv->data = NULL;
		priv->data_size = 0;
		priv->modification_count = 0;
		priv->size = 0;
		priv->resize_count = 8;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_array_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(CatArrayWoInfo, 1);
	}

	CatArrayWoInfo *array_info = (CatArrayWoInfo *) info;
	if (original) {
		CatArrayWoPrivate *rpriv = cat_array_wo_get_instance_private((CatArrayWo *) original);
		array_info->original_data = rpriv->data;
	} else {
		array_info->original_data = NULL;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_array_wo_parent_class);
	if (wocls->createEditInfo) {
		return wocls->createEditInfo(e_wo, original, info);
	}
	return info;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatArrayWo *instance = CAT_ARRAY_WO(self);
	CatArrayWoPrivate *priv = cat_array_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s, size=%d", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->size);
//
	if (priv->size>0) {
		int idx = 0;
		for(idx=0; idx<priv->size && idx<5; idx++) {
			cat_string_wo_format(append_to, ", %o", priv->data[idx]);
		}
		if (priv->size>=5) {
			cat_string_wo_append_chars(append_to, ", ...");
		}
	}
	cat_string_wo_append_chars(append_to, "]");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



#define CAT_ARRAY_WO_ITER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_array_wo_iter_get_type(), CatArrayWoIter))
#define CAT_ARRAY_WO_ITER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_ARRAY_WO_ITER, CatArrayWoIterClass))
#define CAT_IS_ARRAY_WO_ITER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_ARRAY_WO_ITER))
#define CAT_IS_ARRAY_WO_ITER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_ARRAY_WO_ITER))
#define CAT_ARRAY_WO_ITER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_ARRAY_WO_ITER, CatArrayWoIterClass))


struct _CatArrayWoIterClass {
	GObjectClass parent_class;
};



static gboolean l_iter_has_next(CatIIterator *self) {
	CatArrayWoIter *this = (CatArrayWoIter *) self;
	cat_log_trace("_has_next:this->offset=%d", this->offset);
	return this->offset<this->size;
}

static gboolean l_iter_is_last(CatIIterator *self) {
	CatArrayWoIter *this = (CatArrayWoIter *) self;
	return this->offset==this->size;
}

static gpointer l_iter_next(CatIIterator *self) {
	CatArrayWoIter *this = (CatArrayWoIter *) self;
	cat_log_trace("_iter_next:this->offset=%d", this->offset);
	return cat_array_wo_get(this->array, this->offset++);
}


static void l_iterator_interface_init(CatIIteratorInterface *iface) {
	iface->has_next = l_iter_has_next;
	iface->next = l_iter_next;
	iface->is_last = l_iter_is_last;
}


G_DEFINE_TYPE_WITH_CODE(CatArrayWoIter, cat_array_wo_iter, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IITERATOR, l_iterator_interface_init)
);

static void l_dispose_iter(GObject *object);


static void cat_array_wo_iter_class_init(CatArrayWoIterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose_iter;
}

static void cat_array_wo_iter_init(CatArrayWoIter *array_iter) {
}

static void l_dispose_iter(GObject *object) {
	CatArrayWoIter *array_iter = CAT_ARRAY_WO_ITER(object);
	cat_unref_ptr(array_iter->array);
	G_OBJECT_CLASS(cat_array_wo_iter_parent_class)->dispose(object);

}
