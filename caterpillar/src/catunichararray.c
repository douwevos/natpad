/*
   File:    catunichararray.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 1, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "stdlib.h"
#include "catunichararray.h"
#include <stdio.h>
#include <string.h>
#include "catlib.h"

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatUnicharArray"
#include "logging/catlog.h"

struct _CatUnicharArrayPrivate {
	gunichar *data;
	int data_size;
	int size;
	unsigned int hash;
};


G_DEFINE_TYPE_WITH_PRIVATE(CatUnicharArray, cat_unichar_array, G_TYPE_INITIALLY_UNOWNED)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_unichar_array_class_init(CatUnicharArrayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_unichar_array_init(CatUnicharArray *char_array) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cat_unichar_array_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	CatUnicharArray *instance = CAT_UNICHAR_ARRAY(object);
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(instance);
	cat_free_ptr(priv->data);
	G_OBJECT_CLASS(cat_unichar_array_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatUnicharArray *cat_unichar_array_new() {
	CatUnicharArray *result = g_object_new(CAT_TYPE_UNICHAR_ARRAY, NULL);
	cat_ref_anounce(result);
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(result);
	priv->data = NULL;
	priv->size = 0;
	priv->data_size = 0;
	priv->hash = 0;
	return result;
}

static void _ensure_capicity(CatUnicharArray *char_array, int requested_size) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(char_array);
	requested_size++;
	requested_size = ((requested_size+15)/16)*16;

	if (priv->data_size<requested_size) {
		if (priv->data_size==0) {
			priv->data = g_malloc(requested_size*sizeof(gunichar));
		} else {
			priv->data = g_realloc(priv->data, requested_size*sizeof(gunichar));
		}
		priv->data_size = requested_size;
	}
}


int cat_unichar_array_length(CatUnicharArray *char_array) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(char_array);
	return priv->size;
}

void cat_unichar_array_clear(CatUnicharArray *char_array, gboolean full) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(char_array);
	priv->size = 0;
	priv->hash = 0;
	if (full) {
		cat_free_ptr(priv->data);
		priv->data = NULL;
		priv->data_size = 0;
	}
}


gunichar cat_unichar_array_char_at(CatUnicharArray *char_array, int idx) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(char_array);
	return priv->data[idx];
}

CatUnicharArray *cat_unichar_array_substring(CatUnicharArray *char_array, int offset, int length) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(char_array);
	cat_ref_sink_ptr(char_array);
	if (offset<0) {
		offset = 0;
	}
	CatUnicharArray *result = g_object_new(CAT_TYPE_UNICHAR_ARRAY, NULL);
	cat_ref_anounce(result);
	CatUnicharArrayPrivate *rpriv = cat_unichar_array_get_instance_private(result);
	rpriv->hash = 0;
	if (offset>=priv->size || length<0) {
		rpriv->data = NULL;
		rpriv->size = 0;
		rpriv->data_size = 0;
	} else {
		if (length+offset>priv->size) {
			length = priv->size-offset;
		}
		gunichar *new_data = (gunichar *) g_malloc((length+1)*sizeof(gunichar));
		memcpy(new_data, priv->data+offset, length*sizeof(gunichar));
		rpriv->data = new_data;
		rpriv->size = length;
		rpriv->data_size = length;
	}
	cat_unref(char_array);
	return result;
}

void cat_unichar_array_append_uni_char(CatUnicharArray *this, gunichar uch) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(this);
	int new_length = priv->size+1;
	_ensure_capicity(this, new_length);
	priv->data[new_length-1] = uch;
	priv->size++;
	priv->hash = 0;
}


void cat_unichar_array_remove(CatUnicharArray *char_array, int offset, int length) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(char_array);
	if (offset<0) {
		offset = 0;
	}
	if (offset>=priv->size) {
		priv->size = 0;
		return;
	}

	if (length<0) {
		priv->size = offset;
		return;
	}

	int end = offset+length;
	memmove(priv->data+offset, priv->data+end, (priv->size-end)*sizeof(gunichar));
	priv->size -= length;

	cat_log_on_trace({
		gchar *tmp = cat_unichar_array_to_gchars(char_array);
		cat_log_trace("after remove offset=%d, length=%d, text=%s, size=%d", offset, length, tmp, priv->size);
		cat_free_ptr(tmp);
	});

}


gboolean cat_unichar_array_equal(const CatUnicharArray *char_array, const CatUnicharArray *with) {
	if (char_array==with) {
		return TRUE;
	}
	if (char_array==NULL || with==NULL) {
		return FALSE;
	}
	CatUnicharArrayPrivate *priv_a = cat_unichar_array_get_instance_private((CatUnicharArray *) char_array);
	CatUnicharArrayPrivate *priv_b = cat_unichar_array_get_instance_private((CatUnicharArray *) with);
	if (priv_a->size != priv_b->size) {
		return FALSE;
	}
	return memcmp(priv_a->data, priv_b->data, sizeof(gunichar)*priv_b->size)==0;
}

guint cat_unichar_array_hash(CatUnicharArray *this) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(this);
	if (priv->hash == 0) {
		unsigned int code = 0;
		int n = priv->size>255 ? 255 : priv->size;
		const gunichar *p = priv->data;

		for(n=priv->size; n; n--) {
			code = (code * 71) + *p;
			p++;
		}
		if (code==0) {
			code++;
		}
		priv->hash = code;
	}
	return priv->hash;
}

gchar *cat_unichar_array_to_gchars(const CatUnicharArray *char_array) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private((CatUnicharArray *) char_array);
	glong items_read;
	glong items_written;
	GError *error = NULL;
	gchar *result = g_ucs4_to_utf8(priv->data, (glong) priv->size, &items_read, &items_written, &error);
	return result;
}

CatStringWo *cat_unichar_array_to_string(const CatUnicharArray *char_array) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private((CatUnicharArray *) char_array);
	glong items_read;
	glong items_written;
	GError *error = NULL;
	gchar *utf8_txt = g_ucs4_to_utf8(priv->data, (glong) priv->size, &items_read, &items_written, &error);
	CatStringWo *result = cat_string_wo_new_with_len(utf8_txt, items_written);
	g_free(utf8_txt);
	return result;
}


gint cat_unichar_array_uni_char_last_index_of(CatUnicharArray *txt, gunichar ch) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(txt);
	int idx;
	for(idx=priv->size-1; idx>=0; idx--) {
		if (priv->data[idx]==ch) {
			return idx;
		}
	}
	return -1;
}


gint cat_unichar_array_uni_char_index_of(CatUnicharArray *txt, gunichar ch) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(txt);
	int idx;
	for(idx=0; idx<priv->size; idx++) {
		if (priv->data[idx]==ch) {
			return idx;
		}
	}
	return -1;
}



void cat_unichar_array_trim(CatUnicharArray *this) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(this);
//	printf("cat_string_trim:txt=%lx\n", txt);
	if (priv->data_size==0) {
		return;
	}
	int start_idx=0;
	while(start_idx<priv->size) {
		gunichar ch = priv->data[start_idx];
		if (ch== (gunichar) ' ' || ch== (gunichar) '\t') {
			start_idx++;
		} else {
			break;
		}
	}

	int end_idx = priv->size;
	while(end_idx>0) {
		gunichar ch = priv->data[end_idx-1];
		if (ch== (gunichar) ' ' || ch== (gunichar) '\t') {
			end_idx--;
		} else {
			break;
		}
	}

	int size = end_idx-start_idx;
	if (start_idx>0) {
		memmove(priv->data, priv->data+start_idx, size*sizeof(gunichar));
	}
	priv->size = size;
	priv->hash = 0;
}

void cat_unichar_array_set_length(CatUnicharArray *char_array, int length) {
	CatUnicharArrayPrivate *priv = cat_unichar_array_get_instance_private(char_array);
	if (length<0) {
		cat_log_warn("length smaller then 0: %d", length);
		length = 0;
	}
	if (priv->size==length) {
		return;
	}
	if (priv->size>length) {
		priv->size = length;
		priv->hash = 0;
	}
}

