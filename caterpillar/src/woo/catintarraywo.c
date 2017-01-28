/*
   File:    catintarraywo.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jan 10, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "catintarraywoprivate.h"
#include "../catistringable.h"
#include "catstringwo.h"

#include <string.h>

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatIntArrayWo"
#include "../logging/catlog.h"


typedef struct _CatIntArrayWoInfo CatIntArrayWoInfo;

struct _CatIntArrayWoPrivate {
	int *data;
	int data_size;
	int size;
	int modification_count;
	int resize_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatIntArrayWo, cat_int_array_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(CatIntArrayWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);
static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info);


static void cat_int_array_wo_class_init(CatIntArrayWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->cloneContent = l_clone_content;
	wo_class->createEditInfo = l_create_edit_info;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_int_array_wo_init(CatIntArrayWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatIntArrayWo *instance = CAT_INT_ARRAY_WO(object);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(instance);

	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) instance);
	if (array_info) {
		if (array_info->original_data == priv->data) {
			priv->data = NULL;
			priv->data_size = 0;
		}
	}

	if (priv->data) {
		int *d_data = priv->data;
		priv->data = NULL;
		priv->data_size = 0;
		cat_free_ptr(d_data);
	}
	priv->size = 0;

	G_OBJECT_CLASS(cat_int_array_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_int_array_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatIntArrayWo *cat_int_array_wo_new() {
	CatIntArrayWo *result = g_object_new(CAT_TYPE_INT_ARRAY_WO, NULL);
	cat_ref_anounce(result);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = NULL;
	priv->data_size = 0;
	priv->size = 0;
	priv->modification_count = 0;
	priv->resize_count = 8;
	return result;
}


CatIntArrayWo *cat_int_array_wo_new_size(int initial_size) {
	CatIntArrayWo *result = g_object_new(CAT_TYPE_INT_ARRAY_WO, NULL);
	cat_ref_anounce(result);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = initial_size<=0 ? NULL : g_malloc(initial_size*sizeof(int));
	priv->data_size = initial_size<=0 ? 0 : initial_size;
	priv->size = 0;
	priv->modification_count = 0;
	priv->resize_count = 8;
	return result;
}

CatIntArrayWo *cat_int_array_wo_new_sub(const CatIntArrayWo *source, int offset, int length) {
	CatIntArrayWo *result = g_object_new(CAT_TYPE_INT_ARRAY_WO, NULL);
	cat_ref_anounce(result);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = length<=0 ? NULL : g_malloc0(length*sizeof(int));
	priv->data_size = length<=0 ? 0 : length;
	priv->size = priv->data_size;
	priv->modification_count = 0;
	priv->resize_count = 8;
	const CatIntArrayWoPrivate *spriv = cat_int_array_wo_get_instance_private((CatIntArrayWo *) source);
	int cplen = length+offset<spriv->data_size ? length : spriv->data_size-offset;
	memcpy(priv->data, spriv->data, sizeof(int)*cplen);
	return result;
}


//------------------------------------------


int cat_int_array_wo_size(CatIntArrayWo *array) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(array);
	return priv->size;
}


int cat_int_array_wo_get(CatIntArrayWo *array, int index) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(array);
	if (index>=0 && index<priv->size) {
		return priv->data[index];
	}
	return 0;
}

int cat_int_array_wo_get_first(CatIntArrayWo *array) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(array);
	if (priv->size>0) {
		return priv->data[0];
	}
	return 0;
}

int cat_int_array_wo_get_last(CatIntArrayWo *array) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(array);
	if (priv->size>0) {
		return priv->data[priv->size-1];
	}
	return 0;
}



int cat_int_array_wo_find_index(CatIntArrayWo *array, int search_item, int hint_index) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(array);
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

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_array)) { \
			cat_log_error("Object is read only:%o", e_array); \
			return rval; \
		} \



void cat_int_array_wo_clear(CatIntArrayWo *e_array) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	CHECK_IF_WRITABLE()
	if (priv->size<=0) {
		return;
	}

	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		/* its the original data */
		priv->data = NULL;
		priv->size = 0;
		priv->data_size = 0;
		priv->modification_count++;
		return;
	}

	priv->size = 0;
	priv->modification_count++;
}


static void l_ensure_size(CatIntArrayWo *e_array, int minimal_size) {
	cat_log_debug("minimal_size=%d", minimal_size);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	cat_log_detail("array_info=%p, original_data=%p, priv->data=%p", array_info, array_info->original_data, priv->data);
	if (array_info->original_data==priv->data) {
		minimal_size += priv->resize_count;
		if (minimal_size>priv->data_size) {
			priv->data_size = minimal_size;
		}

		priv->data = g_malloc(priv->data_size*sizeof(int));
		memcpy(priv->data, array_info->original_data, priv->size*sizeof(int));
		return;
	}

	if (priv->data_size>=minimal_size) {
		return;
	}

	minimal_size += priv->resize_count;
	if (priv->data_size==0) {
		priv->data_size = minimal_size;
		priv->data = g_malloc(priv->data_size*sizeof(int));
	} else {
		priv->data_size += minimal_size;
		priv->data = g_realloc(priv->data, priv->data_size*sizeof(int));
	}
}

int cat_int_array_wo_append(CatIntArrayWo *e_array, int to_add) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	CHECK_IF_WRITABLE(-1)
	l_ensure_size(e_array, priv->size+1);
	priv->data[priv->size] = to_add;
	priv->modification_count++;
	priv->size++;
	return priv->size-1;
}

void cat_int_array_wo_append_all(CatIntArrayWo *e_array, CatIntArrayWo *list_to_append) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	CatIntArrayWoPrivate *apriv = cat_int_array_wo_get_instance_private(list_to_append);
	if (apriv->size==0) {
		return;
	}
	CHECK_IF_WRITABLE()
	l_ensure_size(e_array, priv->size+apriv->size);

	memcpy(priv->data+priv->size, apriv->data, apriv->size * sizeof(int));
	priv->size += apriv->size;
	priv->modification_count++;
}

void cat_int_array_wo_insert(CatIntArrayWo *e_array, int to_insert, int index) {
	cat_log_debug("e_array=%p, index=%d, to_insert=%p", e_array, index, to_insert);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	if (index>=priv->size) {
		cat_int_array_wo_append(e_array, to_insert);
		return;
	}
	CHECK_IF_WRITABLE()
	if (index<0) {
		index = 0;
	}

	l_ensure_size(e_array, priv->size+1);
	memmove(priv->data+index+1, priv->data+index, (priv->size-index)*sizeof(int));
	priv->data[index] = to_insert;
	priv->modification_count++;
	priv->size++;
}




gboolean cat_int_array_wo_remove(CatIntArrayWo *e_array, int index, int *removed) {
	cat_log_debug("e_array=%p, index=%d, removed=%p", e_array, index, removed);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	if (index<0 || index>=priv->size) {
		if (removed!=NULL) {
			*removed = 0;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)

	if (removed!=NULL) {
		*removed = priv->data[index];
	}

	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(int));
		memcpy(priv->data, array_info->original_data, index*sizeof(int));
		memcpy(priv->data+index, array_info->original_data+index+1, priv->size-(index+1)*sizeof(int));
	} else {
		memmove(priv->data+index, priv->data+index+1, (priv->size-(index+1))*sizeof(int));
	}

	priv->size--;
	priv->modification_count++;
	return TRUE;
}

void cat_int_array_wo_remove_range(CatIntArrayWo *e_array, int offset, int length) {
	cat_log_debug("e_array=%p, offset=%d, length=%d", e_array, offset, length);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	CHECK_IF_WRITABLE()


	if (offset>=priv->size || offset+length<0 || length<=0) {
		return;
	}

	if (offset+length>priv->size) {
		length = priv->size-offset;
	}

	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);

	if (array_info->original_data==priv->data) {
		priv->data_size = priv->size-length;
		priv->data = g_malloc(priv->data_size*sizeof(int));
		memcpy(priv->data, array_info->original_data, offset*sizeof(int));
		memcpy(priv->data+offset, array_info->original_data+offset+length, priv->size-(offset+length)*sizeof(int));
		priv->size = priv->size-length;
	} else {
		int end_off = offset+length;
		memmove(priv->data+offset, priv->data+end_off, (priv->size-(end_off))*sizeof(int));
		priv->size = priv->size-length;
	}
	priv->modification_count++;
}


gboolean cat_int_array_wo_remove_last(CatIntArrayWo *e_array, int *removed) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	if (priv->size<=0) {
		if (removed!=NULL) {
			*removed = 0;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)
	if (removed!=NULL) {
		*removed = priv->data[priv->size-1];
	}
	priv->size--;
	priv->modification_count++;
	return TRUE;
}

gboolean cat_int_array_wo_remove_first(CatIntArrayWo *e_array,  int *removed) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	if (priv->size<=0) {
		if (removed!=NULL) {
			*removed = 0;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)
	if (removed!=NULL) {
		*removed = priv->data[0];
	}
	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		memcpy(priv->data, array_info->original_data+1, (priv->size-1)*sizeof(int));
	} else {
		memmove(priv->data, priv->data+1, (priv->size-1)*sizeof(int));
	}
	priv->size--;
	priv->modification_count++;
	return TRUE;
}

gboolean cat_int_array_wo_set(CatIntArrayWo *e_array, int to_set, int index, int *old_value) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	if (index<0 || index>=priv->size) {
		if (old_value!=NULL) {
			*old_value = 0;
		}
		return FALSE;
	}

	if (priv->data[index] == to_set) {
		if (old_value!=NULL) {
			*old_value = to_set;
		}
		return FALSE;
	}

	CHECK_IF_WRITABLE(FALSE)
	if (old_value!=NULL) {
		*old_value = priv->data[index];
	}
	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(int));
		memcpy(priv->data, array_info->original_data, (priv->size)*sizeof(int));
	}
	priv->data[index] = to_set;
	priv->modification_count++;
	return TRUE;
}

void cat_int_array_wo_limit(CatIntArrayWo *e_array, int offset, int length) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
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
	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(length*sizeof(int));
		priv->data_size = length;
		priv->size = length;
		memcpy(priv->data, array_info->original_data+offset, (length)*sizeof(int));
		priv->modification_count++;
		return;
	}

	memmove(priv->data, priv->data+offset, length*sizeof(void *));
	priv->size = length;
	priv->modification_count++;
}

void cat_int_array_wo_pad(CatIntArrayWo *e_array, int length, int fill_with) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	if (length<=priv->size) {
		return;
	}

	CHECK_IF_WRITABLE()
	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(length*sizeof(int));
		priv->data_size = length;
		memcpy(priv->data, array_info->original_data, (priv->size)*sizeof(int));
	} else {
		l_ensure_size(e_array, length);
	}

	int idx = priv->size;
	while(idx<length) {
		priv->data[idx] = fill_with;
		idx++;
	}

	priv->modification_count++;
	priv->size = length;
	return;
}


void cat_int_array_wo_reverse(CatIntArrayWo *e_array) {
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(e_array);
	if (priv->size<=0) {
		return;
	}
	CHECK_IF_WRITABLE()
	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) cat_wo_get_info((CatWo *) e_array);
	if (array_info->original_data==priv->data) {
		priv->data = g_malloc(priv->data_size*sizeof(int));
		int idx = priv->size-1;
		int out = 0;
		while(idx>=0) {
			priv->data[out] = array_info->original_data[idx];
			idx--;
			out++;
		}
	} else {
		int start = 0;
		int end = priv->size-1;
		int swap;
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




//-------------------------------------------------



static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(CatIntArrayWoInfo, 1);
	}
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_INT_ARRAY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private((CatIntArrayWo *) e_uninitialized);
	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) info;
	if (original) {
		CatIntArrayWoPrivate *rpriv = cat_int_array_wo_get_instance_private((CatIntArrayWo *) original);
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
	return CAT_WO_CLASS(cat_int_array_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const CatIntArrayWoPrivate *priv_a = cat_int_array_wo_get_instance_private((CatIntArrayWo *) wo_a);
	const CatIntArrayWoPrivate *priv_b = cat_int_array_wo_get_instance_private((CatIntArrayWo *) wo_b);
	if ((priv_a->size!=priv_b->size) || (priv_a->resize_count!=priv_b->resize_count)) {
		return FALSE;
	}
	if (priv_a->data == priv_b->data) {
		return TRUE;
	}
	if (memcmp(priv_a->data, priv_b->data, sizeof(int) * priv_a->size)==0) {
		return TRUE;
	}
	return FALSE;
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_INT_ARRAY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(CAT_INT_ARRAY_WO(e_uninitialized));
	if (wo_source) {
		CatIntArrayWoPrivate *priv_src = cat_int_array_wo_get_instance_private(CAT_INT_ARRAY_WO(wo_source));
		if (priv_src->size>0) {
			priv->data = g_malloc(priv_src->size*sizeof(int));
			memcpy(priv->data, priv_src->data, (priv_src->size)*sizeof(int));
		} else {
			priv->data = NULL;
		}
		priv->size = priv_src->size;
		priv->data_size = priv_src->size;
		priv->modification_count = priv_src->modification_count+1;
		priv->resize_count = priv_src->resize_count+1;
	} else {
		priv->data = NULL;
		priv->data_size = 0;
		priv->modification_count = 0;
		priv->size = 0;
		priv->resize_count = 8;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_int_array_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(CatIntArrayWoInfo, 1);
	}

	CatIntArrayWoInfo *array_info = (CatIntArrayWoInfo *) info;
	if (original) {
		CatIntArrayWoPrivate *rpriv = cat_int_array_wo_get_instance_private((CatIntArrayWo *) original);
		array_info->original_data = rpriv->data;
	} else {
		array_info->original_data = NULL;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_int_array_wo_parent_class);
	if (wocls->createEditInfo) {
		return wocls->createEditInfo(e_wo, original, info);
	}
	return info;
}


CAT_WO_BASE_C(CatIntArrayWo,cat_int_array_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatIntArrayWo *instance = CAT_INT_ARRAY_WO(self);
	CatIntArrayWoPrivate *priv = cat_int_array_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s, data=%p]", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->data);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

