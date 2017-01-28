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

G_DEFINE_TYPE(CatUnicharArray, cat_unichar_array, G_TYPE_INITIALLY_UNOWNED)

static void _dispose(GObject *object);

static void cat_unichar_array_class_init(CatUnicharArrayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_unichar_array_init(CatUnicharArray *char_array) {
}

static void _dispose(GObject *object) {
	CatUnicharArray *instance = CAT_UNICHAR_ARRAY(object);
	cat_log_detail("dispose start: instance=%p", instance);
	cat_log_detail("unreffing data=%p", instance->data);
	cat_free_ptr(instance->data);
}


CatUnicharArray *cat_unichar_array_new() {
	CatUnicharArray *result = g_object_new(CAT_TYPE_UNICHAR_ARRAY, NULL);
	cat_ref_anounce(result);
	result->data = NULL;
	result->size = 0;
	result->data_size = 0;
	result->hash = 0;
	return result;
}

static void _ensure_capicity(CatUnicharArray *char_array, int requested_size) {
	requested_size++;
	requested_size = ((requested_size+15)/16)*16;

	if (char_array->data_size<requested_size) {
		if (char_array->data_size==0) {
			char_array->data = g_malloc(requested_size*sizeof(gunichar));
		} else {
			char_array->data = g_realloc(char_array->data, requested_size*sizeof(gunichar));
		}
		char_array->data_size = requested_size;
	}
}


int cat_unichar_array_length(CatUnicharArray *char_array) {
	return char_array->size;
}

void cat_unichar_array_clear(CatUnicharArray *char_array, gboolean full) {
	char_array->size = 0;
	char_array->hash = 0;
	if (full) {
		cat_free_ptr(char_array->data);
		char_array->data = NULL;
		char_array->data_size = 0;
	}
}


gunichar cat_unichar_array_char_at(CatUnicharArray *char_array, int idx) {
	return char_array->data[idx];
}

CatUnicharArray *cat_unichar_array_substring(CatUnicharArray *char_array, int offset, int length) {
	cat_ref_sink_ptr(char_array);
	if (offset<0) {
		offset = 0;
	}
	CatUnicharArray *result = g_object_new(CAT_TYPE_UNICHAR_ARRAY, NULL);
	cat_ref_anounce(result);
	result->hash = 0;
	if (offset>=char_array->size || length<0) {
		result->data = NULL;
		result->size = 0;
		result->data_size = 0;
	} else {
		if (length+offset>char_array->size) {
			length = char_array->size-offset;
		}
		gunichar *new_data = (gunichar *) g_malloc((length+1)*sizeof(gunichar));
		memcpy(new_data, char_array->data+offset, length*sizeof(gunichar));
		result->data = new_data;
		result->size = length;
		result->data_size = length;
	}
	cat_unref(char_array);
	return result;
}

void cat_unichar_array_append_uni_char(CatUnicharArray *this, gunichar uch) {
	int new_length = this->size+1;
	_ensure_capicity(this, new_length);
	this->data[new_length-1] = uch;
	this->size++;
	this->hash = 0;
}


void cat_unichar_array_remove(CatUnicharArray *char_array, int offset, int length) {
	if (offset<0) {
		offset = 0;
	}
	if (offset>=char_array->size) {
		char_array->size = 0;
		return;
	}

	if (length<0) {
		char_array->size = offset;
		return;
	}

	int end = offset+length;
	memmove(char_array->data+offset, char_array->data+end, (char_array->size-end)*sizeof(gunichar));
	char_array->size -= length;

	cat_log_on_trace({
		gchar *tmp = cat_unichar_array_to_gchars(char_array);
		cat_log_trace("after remove offset=%d, length=%d, text=%s, size=%d", offset, length, tmp, char_array->size);
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
	if (char_array->size != with->size) {
		return FALSE;
	}
	return memcmp(char_array->data, with->data, sizeof(gunichar)*with->size)==0;
}

guint cat_unichar_array_hash(CatUnicharArray *this) {
	if (this->hash == 0) {
		unsigned int code = 0;
		int n = this->size>255 ? 255 : this->size;
		const gunichar *p = this->data;

		for(n=this->size; n; n--) {
			code = (code * 71) + *p;
			p++;
		}
		if (code==0) {
			code++;
		}
		this->hash = code;
	}
	return this->hash;
}

gchar *cat_unichar_array_to_gchars(const CatUnicharArray *char_array) {
	glong items_read;
	glong items_written;
	GError *error = NULL;
	gchar *result = g_ucs4_to_utf8(char_array->data, (glong) char_array->size, &items_read, &items_written, &error);
	return result;
}

CatStringWo *cat_unichar_array_to_string(const CatUnicharArray *char_array) {
	glong items_read;
	glong items_written;
	GError *error = NULL;
	gchar *utf8_txt = g_ucs4_to_utf8(char_array->data, (glong) char_array->size, &items_read, &items_written, &error);
	CatStringWo *result = cat_string_wo_new_with_len(utf8_txt, items_written);
	g_free(utf8_txt);
	return result;
}


gint cat_unichar_array_uni_char_last_index_of(CatUnicharArray *txt, gunichar ch) {
	int idx;
	for(idx=txt->size-1; idx>=0; idx--) {
		if (txt->data[idx]==ch) {
			return idx;
		}
	}
	return -1;
}


gint cat_unichar_array_uni_char_index_of(CatUnicharArray *txt, gunichar ch) {
	int idx;
	for(idx=0; idx<txt->size; idx++) {
		if (txt->data[idx]==ch) {
			return idx;
		}
	}
	return -1;
}



void cat_unichar_array_trim(CatUnicharArray *this) {
//	printf("cat_string_trim:txt=%lx\n", txt);
	if (this->data_size==0) {
		return;
	}
	int start_idx=0;
	while(start_idx<this->size) {
		gunichar ch = this->data[start_idx];
		if (ch== (gunichar) ' ' || ch== (gunichar) '\t') {
			start_idx++;
		} else {
			break;
		}
	}

	int end_idx = this->size;
	while(end_idx>0) {
		gunichar ch = this->data[end_idx-1];
		if (ch== (gunichar) ' ' || ch== (gunichar) '\t') {
			end_idx--;
		} else {
			break;
		}
	}

	int size = end_idx-start_idx;
	if (start_idx>0) {
		memmove(this->data, this->data+start_idx, size*sizeof(gunichar));
	}
	this->size = size;
	this->hash = 0;
}

void cat_unichar_array_set_length(CatUnicharArray *char_array, int length) {
	if (length<0) {
		cat_log_warn("length smaller then 0: %d", length);
		length = 0;
	}
	if (char_array->size==length) {
		return;
	}
	if (char_array->size>length) {
		char_array->size = length;
		char_array->hash = 0;
	}
}

