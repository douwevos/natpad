/*
   File:    catstringwo.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 9, 2014
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

#include "catstringwoprivate.h"
#include "../catistringable.h"
#include <string.h>
#include <ctype.h>

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatStringWo"
#include "../logging/catlog.h"

CatS cat_s_empty_string = CAT_S_DEF("");


typedef struct CatStringWoInfo CatStringWoInfo;

struct _CatStringWoPrivate {
	gchar *data;
	int data_size;
	int size;
	unsigned int hash;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatStringWo, cat_string_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(CatStringWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_anchor(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
//static CatWo *l_clone(const CatWo *wo_source, CatCloneDepth clone_depth);
static guint l_hash(const gchar *data, int size);
static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);
static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info);


static void cat_string_wo_class_init(CatStringWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->anchor = l_anchor;
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_wo_construct_editable;
	wo_class->cloneContent = l_clone_content;
	wo_class->createEditInfo = l_create_edit_info;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_string_wo_init(CatStringWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatStringWo *instance = CAT_STRING_WO(object);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(instance);
	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) instance);
	if (string_info && string_info->data == priv->data) {
		priv->data = NULL;
	}

	G_OBJECT_CLASS(cat_string_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	CatStringWo *instance = CAT_STRING_WO(object);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(instance);
	if (priv->data_size>=0) {
		cat_free_ptr(priv->data);
	}
	G_OBJECT_CLASS(cat_string_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatStringWo *cat_string_wo_get_static(CatS *static_string) {
	CatStringWo *result = static_string->s;
	if (result == NULL) {
		result = g_object_new(CAT_TYPE_STRING_WO, NULL);
		cat_ref_anounce(result);
		cat_ref_ignore(result);
		CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
		cat_wo_construct((CatWo *) result, FALSE);
		priv->data = (gchar *) static_string->txt;
		priv->size = strlen(static_string->txt);
		priv->data_size = -1;
		priv->hash = l_hash(priv->data, priv->size);
		static_string->s = result;
	}
	result->parent.parent.ref_count = 1000000;
	return result;
}

void cat_string_wo_construct(CatStringWo *string, gboolean editable, gchar *data, int data_size, int length) {
	cat_ref_anounce(string);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(string);
	cat_wo_construct((CatWo *) string, editable);
	priv->size = length;
	priv->data_size = data_size;
	priv->data = data;
	priv->hash = 0;
}


CatStringWo *cat_string_wo_new() {
	CatStringWo *result = g_object_new(CAT_TYPE_STRING_WO, NULL);
	cat_ref_anounce(result);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = g_malloc(16);
	priv->data[0] = 0;
	priv->size = 0;
	priv->data_size = 15;
	priv->hash = 0;
	return result;
}

CatStringWo *cat_string_wo_new_with(const gchar *data) {
	return cat_string_wo_new_with_len(data, strlen(data));
}

CatStringWo *cat_string_wo_new_with_len(const gchar *data, int length) {
	CatStringWo *result = g_object_new(CAT_TYPE_STRING_WO, NULL);
	cat_ref_anounce(result);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->size = length;
	priv->data_size = priv->size;
	if (data==NULL) {
		priv->data = g_malloc0(priv->data_size+1);
		priv->size = 0;
	} else {
		priv->data = g_malloc(priv->data_size+1);
		memcpy(priv->data, data, priv->size);
	}
	priv->data[priv->data_size] = 0;
	priv->hash = 0;
	return result;
}

CatStringWo *cat_string_wo_new_anchored(const gchar *data, int length) {
	CatStringWo *result = g_object_new(CAT_TYPE_STRING_WO, NULL);
	cat_ref_anounce(result);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, FALSE);
	priv->size = length;
	priv->data_size = priv->size;
	if (data==NULL) {
		priv->data = g_malloc0(priv->data_size+1);
		priv->size = 0;
	} else {
		priv->data = g_malloc(priv->data_size+1);
		memcpy(priv->data, data, priv->size);
	}
	priv->data[priv->data_size] = 0;
	priv->hash = l_hash(priv->data, priv->size);
	return result;
}


CatStringWo *cat_string_wo_new_nocopy(gchar *data, int length) {
	CatStringWo *result = g_object_new(CAT_TYPE_STRING_WO, NULL);
	cat_ref_anounce(result);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->size = length;
	priv->data_size = priv->size;
	priv->data = data;
	priv->data[priv->data_size] = 0;
	priv->hash = 0;
	return result;
}

const CatStringWo *cat_string_wo_new_data(const gchar *data) {
	return cat_string_wo_new_data_len(data, strlen(data));
}


const CatStringWo *cat_string_wo_new_data_len(const gchar *data, int length) {
	CatStringWo *result = g_object_new(CAT_TYPE_STRING_WO, NULL);
	cat_ref_anounce(result);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, FALSE);
	priv->data = (gchar *) data;
	priv->size = length;
	priv->data_size = -1;
	priv->hash = l_hash(priv->data, priv->size);
	return result;
}


CatStringWo *cat_string_wo_new_sub(const CatStringWo *source, int offset, int length) {
	const CatStringWoPrivate *spriv = cat_string_wo_get_instance_private((CatStringWo *) source);
	if (length<0) {
		length = spriv->size-offset;
	}

	CatStringWo *result = g_object_new(CAT_TYPE_STRING_WO, NULL);
	cat_ref_anounce(result);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->data = (gchar *) g_malloc0(length+1);
	priv->size = length;
	priv->data_size = length;
	memcpy(priv->data, spriv->data+offset, priv->size);
	priv->hash = l_hash(priv->data, priv->size);
	return result;
}


int cat_string_wo_length(const CatStringWo *string_wo) {
	if (string_wo==NULL) {
		return 0;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	return priv->size;
}

gchar cat_string_wo_char_at(const CatStringWo *string_wo, int idx) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	return priv->data[idx];
}


const char *cat_string_wo_getchars(const CatStringWo *string_wo) {
	if (string_wo==NULL) {
		return "";
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (priv->data==NULL) {
		return "";
	}
	return priv->data;
}


static guint l_hash(const gchar *data, int size) {
	unsigned int code = 0;
	int n = size>255 ? 255 : size;
	const gchar *p = data;

	for(; n; n--) {
		code = (code * 71) + *p;
		p++;
	}
	return code;
}

guint cat_string_wo_hash(const CatStringWo *string_wo) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (!cat_wo_is_anchored((CatWo *) string_wo)) {
		cat_log_info("WARN:hashing on a non anchored string is normally not a good idea");
		return l_hash(priv->data, priv->size);
	}
	return priv->hash;
}


int cat_string_wo_compare(const CatStringWo *string_wo, const CatStringWo *string_with) {
	if (string_wo==string_with) {
		return 0;
	} else if (string_wo==NULL) {
		return -1;
	} else if (string_with==NULL) {
		return 1;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	const CatStringWoPrivate *priv_w = cat_string_wo_get_instance_private((CatStringWo *) string_with);

	int result = 0;
	int min_chrs = priv_w->size;
	if (priv->size > priv_w->size) {
		result = 1;
	} else if (priv->size < priv_w->size) {
		min_chrs = priv->size;
		result = -1;
	}

	int cmp_res = memcmp(priv->data, priv_w->data, min_chrs);
	if (cmp_res != 0) {
		result = cmp_res;
	}
	return result;
}

int cat_string_wo_case_compare(const CatStringWo *string_wo, const CatStringWo *string_with) {
	if (string_wo==string_with) {
		return 0;
	} else if (string_wo==NULL) {
		return -1;
	} else if (string_with==NULL) {
		return 1;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	const CatStringWoPrivate *priv_w = cat_string_wo_get_instance_private((CatStringWo *) string_with);

	int result = 0;
	int min_chrs = priv_w->size;
	if (priv->size > priv_w->size) {
		result = 1;
	} else if (priv->size < priv_w->size) {
		min_chrs = priv->size;
		result = -1;
	}

	int res = strncasecmp(priv->data, priv_w->data, min_chrs);
	if (res != 0) {
		result = res;
	}
	return result;
}



gboolean cat_string_wo_endswith_chars_len(const CatStringWo *string_wo, const gchar *chars, const int chars_len) {
	if (chars_len<=0) {
		return TRUE;
	}
	if (string_wo==NULL) {
		return FALSE;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (priv->size < chars_len) {
		return FALSE;
	}
	int cmp_res = memcmp(priv->data+priv->size-chars_len, chars, chars_len);
	return cmp_res == 0;
}

gboolean cat_string_wo_endswith(const CatStringWo *string_wo, const CatStringWo *data) {
	if (data==NULL) {
		return FALSE;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) data);
	return cat_string_wo_endswith_chars_len(string_wo, priv->data, priv->size);
}

gboolean cat_string_wo_endswith_char(const CatStringWo *string_wo, const gchar ch) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (priv->size<1) {
		return FALSE;
	}
	return priv->data[priv->size-1] == ch;
}

gboolean cat_string_wo_beginswith_chars_len(const CatStringWo *string_wo, const gchar *chars, const int chars_len) {
	if (chars_len<=0) {
		return TRUE;
	}
	if (string_wo==NULL) {
		return FALSE;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (priv->size < chars_len) {
		return FALSE;
	}
	int cmp_res = memcmp(priv->data, chars, chars_len);
	return cmp_res == 0;
}

gboolean cat_string_wo_beginswith(const CatStringWo *string_wo, const CatStringWo *data) {
	if (data==NULL) {
		return FALSE;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) data);
	return cat_string_wo_beginswith_chars_len(string_wo, priv->data, priv->size);
}

gboolean cat_string_wo_beginswith_char(const CatStringWo *string_wo, const gchar ch) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (priv->size<1) {
		return FALSE;
	}
	return priv->data[0] == ch;
}


gint cat_string_wo_index_of(const CatStringWo *string_wo, gchar ch) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (priv->size<=0) {
		return -1;
	}
	gchar *offset = (gchar *) memchr(priv->data, 0xFF & ch, priv->size);
	if (offset==NULL) {
		return -1;
	}
	return offset-priv->data;
}

gint cat_string_wo_index_of_from(const CatStringWo *string_wo, gchar ch, int from) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	int size = priv->size - from;
	if (size<=0) {
		return -1;
	}

	gchar *offset = (gchar *) memchr(priv->data+from, 0xFF & ch, size);
	if (offset==NULL) {
		return -1;
	}
	return offset-priv->data;
}

gint cat_string_wo_last_index_of(const CatStringWo *string_wo, gchar ch) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	int idx;
	for(idx=priv->size-1; idx>=0; idx--) {
		if (priv->data[idx]==ch) {
			return idx;
		}
	}
	return -1;
}

gint cat_string_wo_index_of_string(const CatStringWo *string_wo, const CatStringWo *str) {
	return cat_string_wo_index_of_string_from(string_wo, str, 0);
}


gint cat_string_wo_index_of_string_from(const CatStringWo *string_wo, const CatStringWo *str, int from) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	const CatStringWoPrivate *priv_str = cat_string_wo_get_instance_private((CatStringWo *) str);
	if (priv_str->size<=0) {
		return -1;
	}
	guchar first_search_char = priv_str->data[0];
	if (priv_str->size==1) {
		return cat_string_wo_index_of_from(string_wo, first_search_char, from);
	}

	void *most_highest_offset = priv->data + (priv->size-priv_str->size);


	void *offset = priv->data+from;
	while(offset<=most_highest_offset) {
		gchar *next = (gchar *) memchr(offset, first_search_char, 1+most_highest_offset-offset);
		if (next == NULL) {
			return -1;
		}

		int cmp_res = memcmp(next, priv_str->data, priv_str->size);
		if (cmp_res==0) {
			return next - priv->data;
		}

		offset = next + 1;
	}
	return -1;
}



#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((const CatWo *) e_string_wo)) { \
			cat_log_error("Object is read only:%o", e_string_wo); \
			return rval; \
		} \



static void l_ensure_capicity(CatStringWo *e_string_wo, int requested_size) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	requested_size = ((requested_size+15)/16)*16;


	if (priv->data_size<requested_size) {
		CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
		if (priv->data_size<=0 || priv->data==string_info->data) {
			char *new_buf = g_malloc0(requested_size+1);
			int n = requested_size<priv->size ? requested_size : priv->size;
			memcpy(new_buf, priv->data, n);
			priv->data = new_buf;
		} else {
			priv->data = g_realloc(priv->data, requested_size+1);
		}
		priv->data_size = requested_size;
	}
}

void cat_string_wo_set_capacity(CatStringWo *e_string_wo, int min_capacity) {
	CHECK_IF_WRITABLE();
	l_ensure_capicity(e_string_wo, min_capacity);
}


void cat_string_wo_clear(CatStringWo *e_string_wo) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();
	priv->size = 0;
	priv->data[0] = 0;
}



void cat_string_wo_append(CatStringWo *e_string_wo, const CatStringWo *string_2_append) {
	if (string_2_append==NULL) {
		return;
	}
	const CatStringWoPrivate *a_priv = cat_string_wo_get_instance_private((CatStringWo *) string_2_append);
	if (a_priv->size==0) {
		return;
	}

	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();

	int new_length = priv->size+a_priv->size;
	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(new_length+1);
		priv->data_size = new_length;
		memcpy(priv->data, string_info->data, string_info->size);
	} else {
		l_ensure_capicity(e_string_wo, new_length);
	}
	memcpy(priv->data+priv->size, a_priv->data, a_priv->size);
	priv->data[new_length] = (gchar) 0;
	priv->size = new_length;
}

void cat_string_wo_append_char(CatStringWo *e_string_wo, gchar char_2_app) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();

	int new_length = priv->size + 1;
	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(new_length+1);
		priv->data_size = new_length;
		memcpy(priv->data, string_info->data, string_info->size);
	} else {
		l_ensure_capicity(e_string_wo, new_length);
	}
	priv->data[priv->size] = char_2_app;
	priv->size = new_length;
	priv->data[new_length] = (gchar) 0;
}


void cat_string_wo_append_chars(CatStringWo *e_string_wo, const char *chars_2_app) {
	if (chars_2_app==NULL) {
		return;
	}
	int l2a = strlen(chars_2_app);
	cat_string_wo_append_chars_len(e_string_wo, chars_2_app, l2a);
}


void cat_string_wo_append_chars_len(CatStringWo *e_string_wo, const char *chars_2_app, int l2a) {
	if (chars_2_app==NULL) {
		return;
	}
	if (l2a==0) {
		return;
	}

	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();

	int new_length = priv->size+l2a;
	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(new_length+1);
		priv->data_size = new_length;
		memcpy(priv->data, string_info->data, string_info->size);
	} else {
		l_ensure_capicity(e_string_wo, new_length);
	}
	memcpy(priv->data+priv->size, chars_2_app, l2a);
	priv->data[new_length] = (gchar) 0;
	priv->size = new_length;
}


int cat_string_wo_append_unichar(CatStringWo *e_string_wo, gunichar unichar) {
	unsigned char tmp[7];
	int out_length = 0;

	if (unichar<0x80) {
		tmp[0] = unichar;
		out_length = 1;
	} else if (unichar < (1<<12)) {
		tmp[0] = 0xC0 | (unichar>>6);
		tmp[1] = 0x80 | (unichar & 0x3f);
		out_length = 2;
	} else if (unichar < (1<<17)) {
		tmp[0] = 0xE0 | ((unichar>>12) & 0x0f);
		tmp[1] = 0x80 | ((unichar>>6) & 0x3f);
		tmp[2] = 0x80 | (unichar & 0x3f);
		out_length = 3;
	} else if (unichar < (1<<22)) {
		tmp[0] = 0xF0 | ((unichar>>18) & 0x07);
		tmp[1] = 0x80 | ((unichar>>12) & 0x3f);
		tmp[2] = 0x80 | ((unichar>>6) & 0x3f);
		tmp[3] = 0x80 | (unichar & 0x3f);
		out_length = 4;
	} else if (unichar< (1<<27)) {
		tmp[0] = 0xF8 | ((unichar>>24) & 0x03);
		tmp[1] = 0x80 | ((unichar>>18) & 0x3f);
		tmp[2] = 0x80 | ((unichar>>12) & 0x3f);
		tmp[3] = 0x80 | ((unichar>>6) & 0x3f);
		tmp[4] = 0x80 | (unichar & 0x3f);
		out_length = 5;
	} else {
		tmp[0] = 0xFC | ((unichar>>30) & 0x1);
		tmp[1] = 0x80 | ((unichar>>24) & 0x3f);
		tmp[2] = 0x80 | ((unichar>>18) & 0x3f);
		tmp[3] = 0x80 | ((unichar>>12) & 0x3f);
		tmp[4] = 0x80 | ((unichar>>6) & 0x3f);
		tmp[5] = 0x80 | (unichar & 0x3f);
		out_length = 6;
	}
	if (out_length==0) {
		return out_length;
	}

	cat_string_wo_append_chars_len(e_string_wo, (char *) tmp, out_length);
	return out_length;
}

void cat_string_wo_insert(CatStringWo *e_string_wo, const CatStringWo *to_insert, int offset) {
	if (to_insert==NULL) {
		return;
	}
	const CatStringWoPrivate *ins_priv = cat_string_wo_get_instance_private((CatStringWo *) to_insert);
	if (ins_priv->size==0) {
		return;
	}

	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();
	if (offset>=priv->size) {
		cat_string_wo_append(e_string_wo, to_insert);
		return;
	}
	int new_length = priv->size + ins_priv->size;
	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(new_length+1);
		priv->data_size = new_length;
		memcpy(priv->data, string_info->data, offset);
		memcpy(priv->data+offset+ins_priv->size, string_info->data+offset, priv->size - offset);
	} else {
		l_ensure_capicity(e_string_wo, new_length);
		memmove(priv->data+offset+ins_priv->size, priv->data+offset, priv->size - offset);
	}
	memcpy(priv->data+offset, ins_priv->data, ins_priv->size);
	priv->size = new_length;
	priv->data[new_length] = 0;
}


void cat_string_wo_remove_range(CatStringWo *e_string_wo, int offset, int length) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();

	if (offset<0) {
		offset = 0;
	} else if (offset>=priv->size) {
		return;
	}

	if (length<=0) {
		return;
	}
	if (offset+length>priv->size) {
		length = priv->size-offset;
	}

	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(priv->size+1);
		priv->data_size = priv->size;
		memcpy(priv->data, string_info->data, offset);
		memcpy(priv->data+offset, string_info->data+offset+length, priv->size-(offset+length));
	} else {
		memmove(priv->data+offset, priv->data+offset+length, priv->size-(offset+length));
	}
	priv->size -= length;
	priv->data[priv->size] = 0;
}


void cat_string_wo_replace_all_char(CatStringWo *e_string_wo, char replace, char with) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	if (replace==with) {
		return;
	}
	CHECK_IF_WRITABLE();

	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(priv->size+1);
		priv->data_size = priv->size;
		memcpy(priv->data, string_info->data, string_info->size);
		priv->data[priv->size] = 0;
	}
	int idx;
	for(idx=priv->size-1; idx>=0; idx--) {
		if (priv->data[idx]==replace) {
			priv->data[idx] = with;
		}
	}
}


void cat_string_wo_replace_range_with(CatStringWo *e_string_wo, int offset, int length, const CatStringWo *replacement) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CatStringWoPrivate *rpriv = cat_string_wo_get_instance_private((CatStringWo *) replacement);
	CHECK_IF_WRITABLE();

	if (offset<0) {
		offset = 0;
	} else if (offset>=priv->size) {
		cat_string_wo_append(e_string_wo, replacement);
		return;
	}

	if (length<0) {
		length = 0;
	}
	if (offset+length>priv->size) {
		length = priv->size-offset;
	}

	int rep_len = rpriv->size;

	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		int new_length = priv->size-length+rep_len;
		priv->data = g_malloc(new_length+1);
		priv->data_size = new_length;
		memcpy(priv->data, string_info->data, offset);
		memcpy(priv->data+offset+rep_len, string_info->data+offset+length, priv->size-(offset+length));
		priv->size = new_length;
	} else {
		if (length != rep_len) {
			int new_length = priv->size-length+rep_len;
			l_ensure_capicity(e_string_wo, new_length);

			memmove(priv->data+offset+rep_len, priv->data+offset+length, priv->size-(offset+length));
			priv->size = new_length;
		}
	}
	memcpy(priv->data+offset, rpriv->data, rep_len);
	priv->data[priv->size] = 0;
}

void cat_string_wo_to_uppercase(CatStringWo *e_string_wo) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	if (priv->size==0) {
		return;
	}
	CHECK_IF_WRITABLE();

	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(priv->size+1);
		priv->data_size = priv->size;
		memcpy(priv->data, string_info->data, string_info->size);
	}

	int idx;
	for(idx=priv->size-1; idx>=0; idx--) {
		priv->data[idx] = toupper(priv->data[idx]);
	}
}

void cat_string_wo_to_lowercase(CatStringWo *e_string_wo) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	if (priv->size==0) {
		return;
	}
	CHECK_IF_WRITABLE();

	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(priv->size+1);
		priv->data_size = priv->size;
		memcpy(priv->data, string_info->data, string_info->size);
	}

	int idx;
	for(idx=priv->size-1; idx>=0; idx--) {
		priv->data[idx] = tolower(priv->data[idx]);
	}
}

void cat_string_wo_limit(CatStringWo *e_string_wo, int offset, int length) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();
	if (offset<=0) {
		if (length<priv->size) {
			priv->size = length;
			priv->data[priv->size] = 0;
		}
		return;
	}

	if (offset>=priv->size) {
		priv->size = 0;
		priv->data[0] = 0;
		return;
	}

	if (offset+length>priv->size) {
		length = priv->size-offset;
	}

	if (length<=0) {
		return;
	}


	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(length+1);
		memcpy(priv->data, string_info->data+offset, length);
		priv->data_size = length;
	} else {
		memcpy(priv->data, priv->data+offset, length);
	}
	priv->size = length;
	priv->data[length] = (gchar) 0;
}







void cat_string_wo_append_decimal(CatStringWo *e_string_wo, long long val) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();
	int idx=0;
	gboolean sign = val<0;
	if (sign) {
		if (val==0x8000000000000000) {
			cat_string_wo_append_chars_len(e_string_wo, "-9223372036854775808", 20);
			return;
		}
		val = -val;
	}
	char buf[60];
	long rest;
	while(idx<59) {
		rest = val % 10l;
		val = val/10l;
		buf[idx] = '0'+rest;
		idx++;
		if (val==0) {
			break;
		}
	}
	if (sign) {
		buf[idx++] = '-';
	}

	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	int new_length = priv->size+idx;
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(new_length+1);
		priv->data_size = new_length;
		memcpy(priv->data, string_info->data, string_info->size);
	} else {
		l_ensure_capicity(e_string_wo, new_length);
	}

	gchar *new_data = priv->data;
	int out_idx = priv->size;
	for(idx--; idx>=0; idx--) {
		new_data[out_idx++] = buf[idx];
	}
	new_data[new_length] = (gchar) 0;
	priv->size = new_length;
}



static const char *l_txt_hex = "0123456789ABCDEF";

void cat_string_wo_append_hexadecimal(CatStringWo *e_string_wo, unsigned long long val, int digits) {
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_string_wo);
	CHECK_IF_WRITABLE();
	int idx=0;
	char buf[40];
	if (digits<1) {
		digits = 1;
	}
	while((val>0) || (digits>0)) {
		buf[idx] = l_txt_hex[val & 0xf];
		val = val>>4;
		idx++;
		digits--;
	}

	CatStringWoInfo *string_info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) e_string_wo);
	int new_length = priv->size+idx;
	if (string_info!=NULL && priv->data == string_info->data) {
		priv->data = g_malloc(new_length+1);
		priv->data_size = new_length;
		memcpy(priv->data, string_info->data, string_info->size);
	} else {
		l_ensure_capicity(e_string_wo, new_length);
	}
	gchar *new_data = priv->data;
	int out_idx = priv->size;
	for(idx--; idx>=0; idx--) {
		new_data[out_idx++] = buf[idx];
	}
	new_data[new_length] = (gchar) 0;
	priv->size = new_length;
}



#define FM_NONE         0
#define FM_START        1	// '%'
#define FM_ESCAPE       2	// '\'
#define FM_START_ZERO   3	// '%0'
#define FM_LONG         4	// '%l'

void cat_string_wo_format(CatStringWo *e_string_wo, const char *text, ...) {
	CHECK_IF_WRITABLE();
	va_list args;

	va_start(args, text);

	gboolean keep_running = TRUE;
	int mode = FM_NONE;
	int cidx=0;
	while(keep_running) {
		char ch = text[cidx];
		switch(mode) {
			case FM_NONE : {
				if(ch==0) {
					keep_running = FALSE;
				} else if (ch=='%') {
					mode = FM_START;
				} else if (ch=='\\') {
					mode = FM_ESCAPE;
				} else {
					cat_string_wo_append_char(e_string_wo, ch);
				}
			} break;
			case FM_START : {
				if(ch==0) {
					cat_string_wo_append_char(e_string_wo, '%');
					keep_running = FALSE;
				} else if (ch=='%') {
					cat_string_wo_append_char(e_string_wo, '%');
				} else if (ch=='0') {
					mode = FM_START_ZERO;
				} else if (ch=='d') {
					int val = va_arg(args, int);
					cat_string_wo_append_decimal(e_string_wo, val);
					mode = FM_NONE;
				} else if (ch=='f') {
					double val = va_arg(args, double);
					char ft[100];
					sprintf(ft, "%f", val);
					cat_string_wo_append_chars(e_string_wo, ft);
					mode = FM_NONE;
				} else if (ch=='l') {
					mode = FM_LONG;
				} else if (ch=='s') {
					const char *val = va_arg(args, const char *);
					cat_string_wo_append_chars(e_string_wo, val);
					mode = FM_NONE;
				} else if (ch=='o' || ch=='O') {
					GObject *val = va_arg(args, GObject *);
					if (val==NULL) {
						cat_string_wo_append_chars_len(e_string_wo, "<nil>", 5);
					} else if (ch=='O' && CAT_IS_STRING_WO(val)) {
						cat_string_wo_append_chars_len(e_string_wo, cat_string_wo_getchars((CatStringWo *) val), cat_string_wo_length((CatStringWo *) val));
					} else if (CAT_IS_ISTRINGABLE(val)) {
						CAT_ISTRINGABLE_GET_INTERFACE((CatIStringable *) val)->print((CatIStringable *) val, e_string_wo);
					} else {
						cat_string_wo_append_chars(e_string_wo, g_type_name_from_instance((GTypeInstance *) val));
						cat_string_wo_append_chars_len(e_string_wo, "[0x", 3);
						cat_string_wo_append_hexadecimal(e_string_wo, (unsigned long long) val, -1);
						cat_string_wo_append_chars_len(e_string_wo, "]", 1);
					}
					mode = FM_NONE;
				} else if (ch=='p') {
					void *val = va_arg(args, void *);
					cat_string_wo_append_chars_len(e_string_wo, "0x", 2);
					cat_string_wo_append_hexadecimal(e_string_wo, (unsigned long long) val, -1);
					mode = FM_NONE;
				} else if (ch=='x') {
					int val = va_arg(args, int);
					cat_string_wo_append_hexadecimal(e_string_wo, (unsigned long long) val, -1);
					mode = FM_NONE;
				} else if (ch=='c') {
					char val = (char) va_arg(args, int);
					cat_string_wo_append_char(e_string_wo, val);
					mode = FM_NONE;
				} else {
					cat_string_wo_append_char(e_string_wo, '%');
					cat_string_wo_append_char(e_string_wo, ch);
					mode = FM_NONE;
				}
			} break;

			case FM_ESCAPE : {
				if(ch==0) {
					cat_string_wo_append_char(e_string_wo, '\\');
					keep_running = FALSE;
				} else if (ch=='%') {
					cat_string_wo_append_char(e_string_wo, '%');
					mode = FM_NONE;
				} else if (ch=='n') {
					cat_string_wo_append_char(e_string_wo, '\n');
					mode = FM_NONE;
				} else if (ch=='t') {
					cat_string_wo_append_char(e_string_wo, '\t');
					mode = FM_NONE;
				} else {
					cat_string_wo_append_char(e_string_wo, '\\');
					cat_string_wo_append_char(e_string_wo, ch);
					mode = FM_NONE;
				}
			} break;

			case FM_START_ZERO : {
				if(ch==0) {
					cat_string_wo_append_chars_len(e_string_wo, "%0", 2);
					keep_running = FALSE;
				} else {
					cat_string_wo_append_chars_len(e_string_wo, "%0", 2);
					mode = FM_NONE;
				}
			} break;

			case FM_LONG : {
				if(ch==0) {
					cat_string_wo_append_chars_len(e_string_wo, "%l", 2);
					keep_running = FALSE;
				} else if (ch=='d') {
					long long val = va_arg(args, long long);
					cat_string_wo_append_decimal(e_string_wo, val);
					mode = FM_NONE;
				} else if (ch=='f') {
					double val = va_arg(args, double);
					char ft[150];
					sprintf(ft, "%lf", val);
					cat_string_wo_append_chars(e_string_wo, ft);
					mode = FM_NONE;
				} else {
					cat_string_wo_append_chars_len(e_string_wo, "%l", 2);
					mode = FM_NONE;
				}
			} break;


		}
		cidx++;
	}

	va_end(args);
}


int cat_string_wo_count_occurrence(const CatStringWo *instance, char ch) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) instance);
	int idx;
	int count=0;
	for(idx=0; idx<priv->size; idx++) {
		if (priv->data[idx]==ch) {
			count++;
		}
	}
	return count;
}

CatArrayWo *cat_string_wo_split(const CatStringWo *input, char ch, gboolean include_empty) {
	CatArrayWo *result = cat_array_wo_new();
	if (input==NULL) {
		return result;
	}
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) input);
	if (priv->size==0) {
		return result;
	}
	int idxa=0;
	int idxb=0;
	while(idxb<=priv->size) {
		char crch = idxb==priv->size ? ch : priv->data[idxb];
		if (crch==ch) {
			if (idxa!=idxb || include_empty) {
				CatStringWo *txt = cat_string_wo_new_sub(input, idxa, idxb-idxa);
				cat_array_wo_append(result, (GObject *) txt);
				cat_unref_ptr(txt);
			}
			idxa=idxb+1;
		}
		idxb++;
	}
	return result;
}

gunichar cat_string_wo_unichar_before(const CatStringWo *string_wo, int *index) {
	gunichar result = -1;
	int idx = *index;
	idx--;
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	while(idx>=0) {
		char c = priv->data[idx];
		if ((c & 0xC0)==0x80) {
			idx--;
		} else {
			*index = idx;
			int fb = priv->data[idx++];
			if ((fb & 0x80)==0) {
				result = fb;
			} else if ((fb & 0xE0) == 0xC0) {
				result = (fb & 0x1F)<<6;
				result = result | (priv->data[idx++] & 0x3F);
			} else if ((fb & 0xF0) == 0xE0) {
				result = (fb & 0xF)<<12;
				result = result | ((priv->data[idx++] & 0x3F)<<6);
				result = result | ((priv->data[idx++] & 0x3F));
			} else if ((fb & 0xF8) == 0xF0) {
				result = (fb & 0x7)<<18;
				result = result | ((priv->data[idx++] & 0x3F)<<12);
				result = result | ((priv->data[idx++] & 0x3F)<<6);
				result = result | ((priv->data[idx++] & 0x3F));
			} else if ((fb & 0xFC) == 0xF8) {
				result = (fb & 0x3)<<24;
				result = result | ((priv->data[idx++] & 0x3F)<<18);
				result = result | ((priv->data[idx++] & 0x3F)<<12);
				result = result | ((priv->data[idx++] & 0x3F)<<6);
				result = result | ((priv->data[idx++] & 0x3F));
			} else if ((fb & 0xFE) == 0xFC) {
				result = (fb & 0x1)<<30;
				result = result | ((priv->data[idx++] & 0x3F)<<24);
				result = result | ((priv->data[idx++] & 0x3F)<<18);
				result = result | ((priv->data[idx++] & 0x3F)<<12);
				result = result | ((priv->data[idx++] & 0x3F)<<6);
				result = result | ((priv->data[idx++] & 0x3F));
			} else if ((fb & 0xFF) == 0xFE) {
				result = result | ((priv->data[idx++] & 0x3F)<<30);
				result = result | ((priv->data[idx++] & 0x3F)<<24);
				result = result | ((priv->data[idx++] & 0x3F)<<18);
				result = result | ((priv->data[idx++] & 0x3F)<<12);
				result = result | ((priv->data[idx++] & 0x3F)<<6);
				result = result | ((priv->data[idx++] & 0x3F));
			}
			break;
		}

	}
	return result;
}



gunichar cat_string_wo_unichar_at(const CatStringWo *string_wo, int *index) {
	int idx = *index;
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (idx>=priv->size) {
		return -1;
	}

	gunichar result = -1;
	int fb = priv->data[idx++];
	if ((fb & 0x80)==0) {
		result = fb;
	} else if ((fb & 0xE0) == 0xC0) {
		result = (fb & 0x1F)<<6;
		result = result | (priv->data[idx++] & 0x3F);
	} else if ((fb & 0xF0) == 0xE0) {
		result = (fb & 0xF)<<12;
		result = result | ((priv->data[idx++] & 0x3F)<<6);
		result = result | ((priv->data[idx++] & 0x3F));			// TODO unsafe index in data ... should check against boundaries
	} else if ((fb & 0xF8) == 0xF0) {
		result = (fb & 0x7)<<18;
		result = result | ((priv->data[idx++] & 0x3F)<<12);
		result = result | ((priv->data[idx++] & 0x3F)<<6);
		result = result | ((priv->data[idx++] & 0x3F));
	} else if ((fb & 0xFC) == 0xF8) {
		result = (fb & 0x3)<<24;
		result = result | ((priv->data[idx++] & 0x3F)<<18);
		result = result | ((priv->data[idx++] & 0x3F)<<12);
		result = result | ((priv->data[idx++] & 0x3F)<<6);
		result = result | ((priv->data[idx++] & 0x3F));
	} else if ((fb & 0xFE) == 0xFC) {
		result = (fb & 0x1)<<30;
		result = result | ((priv->data[idx++] & 0x3F)<<24);
		result = result | ((priv->data[idx++] & 0x3F)<<18);
		result = result | ((priv->data[idx++] & 0x3F)<<12);
		result = result | ((priv->data[idx++] & 0x3F)<<6);
		result = result | ((priv->data[idx++] & 0x3F));
	} else if ((fb & 0xFF) == 0xFE) {
		result = result | ((priv->data[idx++] & 0x3F)<<30);
		result = result | ((priv->data[idx++] & 0x3F)<<24);
		result = result | ((priv->data[idx++] & 0x3F)<<18);
		result = result | ((priv->data[idx++] & 0x3F)<<12);
		result = result | ((priv->data[idx++] & 0x3F)<<6);
		result = result | ((priv->data[idx++] & 0x3F));
	}
	*index = idx;
	return result;
}

int cat_string_wo_unichar_next_offset(const CatStringWo *string_wo, int offset) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) string_wo);
	if (offset>=priv->size) {
		return priv->size;
	}

	while(offset<priv->size) {
		int fb = priv->data[offset];
		if ((fb & 0x80)==0) {
			offset++;
		} else if ((fb & 0xE0) == 0xC0) {
			offset += 2;
		} else if ((fb & 0xF0) == 0xE0) {
			offset += 3;
		} else if ((fb & 0xF8) == 0xF0) {
			offset += 4;
		} else if ((fb & 0xFC) == 0xF8) {
			offset += 5;
		} else if ((fb & 0xFE) == 0xFC) {
			offset += 6;
		} else if ((fb & 0xFF) == 0xFE) {
			offset += 7;
		} else {
			offset++;
			continue;
		}
		break;
	}
	if (offset>=priv->size) {
		return priv->size;
	}
	return offset;
}



int cat_string_wo_to_utf8_pos(const CatStringWo *instance, int offset) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) instance);
	int result = 0;
	int data_offset = 0;

	gboolean is_at_end = offset>=priv->size;
	if (is_at_end) {
		offset = priv->size;
	}

	while(data_offset<offset) {
		char ch = priv->data[data_offset];
		if ((ch<0x80) || (ch>=0xc0)) {
			result++;
		}
		data_offset++;
	}

	return result;
}


int cat_string_wo_from_utf8_pos(const CatStringWo *instance, int utf8_offset) {
	const CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) instance);
	int data_offset = 0;
	int result = 0;

	if (utf8_offset>=priv->size) {
		utf8_offset=priv->size;
	}

	while(data_offset<utf8_offset) {
		unsigned char ch = priv->data[data_offset];
		if ((ch<0x80) || (ch>=0xc0)) {
			result++;
		}
		data_offset++;
	}

	return result;
}


gboolean cat_string_wo_to_valid_utf8(CatStringWo *e_string_wo, const char *text, int length) {
	CHECK_IF_WRITABLE(FALSE);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(e_string_wo);
	int nl = priv->size+length+1;
	l_ensure_capicity(e_string_wo, nl);

	gboolean is_valid = TRUE;
	char *out = priv->data+priv->size;

	int cp_len = 0;
	int idx = 0;
	while(idx<length) {


		unsigned int fb = (int) text[idx++];
		if ((fb & 0x80)==0) {
			cp_len = 0;
		} else if (fb>=0xc2 && fb<=0xDF) {
			cp_len = 1;
		} else if (fb>=0xE0 && fb<0xEF) {
			cp_len = 2;
		} else if (fb >= 0xF0 && fb<=0xF4) {
			cp_len = 3;
//		} else if ((fb & 0xFC) == 0xF8) {
//			cp_len = 4;
//		} else if ((fb & 0xFE) == 0xFC) {
//			cp_len = 5;
		} else {
			is_valid = FALSE;
			*out = '.';
			out++;
			continue;
		}
		if (idx+cp_len>length) {
			is_valid = FALSE;
			*out = '.';
			out++;
			break;
		}
		*out = (char) fb;
		out++;
		while(cp_len>0) {
			fb = (int) text[idx++];
			char ob = (char) ((fb & 0x3f) | (0x80));
			if (ob!=fb) {
				is_valid = FALSE;
			}
			*out = ob;
			out++;
			cp_len--;
		}
	}

	priv->size = out-priv->data;
	priv->data[priv->size] = 0;
	return is_valid;
}



CatStringWo *cat_string_wo_create_editable(CatStringWo *instance) {
	return (CatStringWo *) cat_wo_create_editable((CatWo *) instance);
}

CatStringWo *cat_string_wo_anchor(CatStringWo *instance, int version) {
	return (CatStringWo *) cat_wo_anchor((CatWo *) instance, version);
}

CatStringWo *cat_string_wo_get_original(CatStringWo *instance) {
	return (CatStringWo *) cat_wo_get_original((CatWo *) instance);
}


gboolean cat_string_wo_is_anchored(const CatStringWo *instance) {
	return cat_wo_is_anchored((const CatWo *) instance);
}

gboolean cat_string_wo_equal(const CatStringWo *instance_a, const CatStringWo *instance_b) {
	if (instance_a==instance_b) {
		return TRUE;
	} else if (instance_a==NULL || instance_b==NULL) {
		return FALSE;
	}
	const CatStringWoPrivate *priv_a = cat_string_wo_get_instance_private((CatStringWo *) instance_a);
	const CatStringWoPrivate *priv_b = cat_string_wo_get_instance_private((CatStringWo *) instance_b);
	if (priv_a->size!=priv_b->size) {
		return FALSE;
	}
	return memcmp(priv_a->data, priv_b->data, priv_a->size)==0;
}

CatStringWo *cat_string_wo_clone(const CatStringWo *source, CatCloneDepth clone_depth) {
	if (source == NULL) {
		return NULL;
	} else if ((clone_depth==CAT_CLONE_DEPTH_NONE || clone_depth==CAT_CLONE_DEPTH_AS_ANCHORED) && cat_wo_is_anchored((CatWo *) source)) {
		return cat_ref_ptr((CatStringWo *) source);
	}
	const CatStringWoPrivate *spriv = cat_string_wo_get_instance_private((CatStringWo *) source);

	CatStringWo *result = cat_string_wo_new();
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
	if (spriv->size>0) {
		if (spriv->data_size<0) {
			priv->data_size = spriv->size+10;
		}  else {
			priv->data_size = spriv->data_size;
		}
		priv->size = spriv->size;
		priv->data = g_malloc(priv->data_size+1);
		memcpy(priv->data, spriv->data, priv->size);
		priv->data[priv->size] = 0;
	}
	if (clone_depth==CAT_CLONE_DEPTH_AS_ANCHORED) {
		result = cat_string_wo_anchor(result, cat_wo_get_version((CatWo *) source)+1);
	}
	return result;
}



static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(struct CatStringWoInfo, 1);
	}
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_STRING_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) e_uninitialized);
	CatStringWoInfo *string_info = (CatStringWoInfo *) info;

	if (original) {
		CatStringWoPrivate *rpriv = cat_string_wo_get_instance_private((CatStringWo *) original);
		priv->data = rpriv->data;
		priv->data_size = rpriv->data_size;
		priv->hash = rpriv->hash;
		priv->size = rpriv->size;
		string_info->data = rpriv->data;
		string_info->size = rpriv->size;
	} else {
		priv->data = NULL;
		priv->data_size = 0;
		priv->hash = 0;
		priv->size = 0;
		string_info->data = NULL;
		string_info->size = 0;
	}
	return CAT_WO_CLASS(cat_string_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static CatWo *l_anchor(CatWo *wo, int version) {
	if (wo == NULL) {
		return NULL;
	}
	if (cat_wo_is_anchored(wo)) {
		return wo;
	}
	CatWo *result = CAT_WO_CLASS(cat_string_wo_parent_class)->anchor(wo, version);
	if (wo == result) {
		CatStringWoPrivate *priv = cat_string_wo_get_instance_private((CatStringWo *) wo);
		priv->hash = l_hash(priv->data, priv->size);
	}
	return (CatWo *) result;
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	return cat_string_wo_equal((const CatStringWo *) wo_a, (const CatStringWo *) wo_b);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_STRING_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(CAT_STRING_WO(e_uninitialized));
	if (wo_source) {
		CatStringWoPrivate *priv_src = cat_string_wo_get_instance_private(CAT_STRING_WO(wo_source));
		CatStringWoInfo *info = (CatStringWoInfo *) cat_wo_get_info((CatWo *) wo_source);
		if (info!=NULL && info->data==priv_src->data) {
			priv->data = priv_src->data;
			priv->data_size = priv_src->data_size;
		} else if (priv_src->size>0) {
			priv->data_size = priv_src->size+10;
			priv->data = g_malloc(priv->data_size+1);
			memcpy(priv->data, priv_src->data, priv_src->size);
			priv->data[priv_src->size] = 0;
		} else {
			priv->data = NULL;
			priv->data_size = 0;
		}
		priv->hash = priv_src->hash;
		priv->size = priv_src->size;
	} else {
		priv->data = NULL;
		priv->data_size = 0;
		priv->hash = 0;
		priv->size = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_string_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = (struct CatWoInfo *) g_new0(CatStringWoInfo, 1);
	}

	CatStringWoInfo *string_info = (CatStringWoInfo *) info;
	if (original) {
		CatStringWoPrivate *rpriv = cat_string_wo_get_instance_private((CatStringWo *) original);
		string_info->data = rpriv->data;
		string_info->size = rpriv->size;
	} else {
		string_info->data = NULL;
		string_info->size = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_string_wo_parent_class);
	if (wocls->createEditInfo) {
		return wocls->createEditInfo(e_wo, original, info);
	}
	return info;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatStringWo *instance = CAT_STRING_WO(self);
	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(rc:%d): %s, '", iname, self, ((GObject *) self)->ref_count, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable");
	cat_string_wo_append_chars_len(append_to, priv->data, priv->size);
	cat_string_wo_format(append_to, "', size=%d, data-size=%d]", priv->size, priv->data_size);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
