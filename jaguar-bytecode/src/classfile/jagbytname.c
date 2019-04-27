/*
   File:    jagbytname.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 28, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "jagbytname.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytName"
#include <logging/catlog.h>

struct _JagBytNamePrivate {
	CatArrayWo *a_parts;
	int hashCode;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytName, jag_byt_name, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_name_class_init(JagBytNameClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_name_init(JagBytName *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytName *instance = JAG_BYT_NAME(object);
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(instance);
	cat_unref_ptr(priv->a_parts);
	G_OBJECT_CLASS(jag_byt_name_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_name_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytName *jag_byt_name_new(CatStringWo *a_full_name, gboolean from_pool) {
	JagBytName *result = g_object_new(JAG_BYT_TYPE_NAME, NULL);
	cat_ref_anounce(result);
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(result);

	char sep = from_pool ? '/' : '.';
	int idxa = 0;
	CatArrayWo *e_literals = cat_array_wo_new();
	int hashCode=0;
	gboolean keep_running = TRUE;
	while(keep_running) {
		int idxb = cat_string_wo_index_of_from(a_full_name, sep, idxa);
		if (idxb<0) {
			idxb = cat_string_wo_length(a_full_name);
			keep_running = FALSE;
		}
		CatStringWo *a_literalId = cat_string_wo_new_sub(a_full_name, idxa, idxb-idxa);
		a_literalId = cat_string_wo_anchor(a_literalId, 0);
		cat_array_wo_append(e_literals, (GObject *) a_literalId);
		hashCode += cat_string_wo_hash(a_literalId);
		idxa = idxb+1;
		cat_unref_ptr(a_literalId);
	}

	priv->a_parts = cat_array_wo_anchor(e_literals, 0);
	priv->hashCode = hashCode;
	return result;
}

static JagBytName *jag_byt_name_new_fast(CatArrayWo *a_direct) {
	JagBytName *result = g_object_new(JAG_BYT_TYPE_NAME, NULL);
	cat_ref_anounce(result);
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(result);
	priv->a_parts = cat_ref_ptr(a_direct);
	int hashCode = 0;
	CatIIterator *iter = cat_array_wo_iterator(a_direct);
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *a_id = (CatStringWo *) cat_iiterator_next(iter);
		hashCode += cat_string_wo_hash(a_id);
	}
	cat_unref_ptr(iter);
	priv->hashCode = hashCode;
	return result;
}

int jag_byt_name_count(JagBytName *name) {
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	return cat_array_wo_size(priv->a_parts);
}

CatStringWo *jag_byt_name_get(JagBytName *name, int index) {
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	int parts_length = cat_array_wo_size(priv->a_parts);
	if (index>=0 && index<parts_length) {
		return CAT_STRING_WO(cat_array_wo_get(priv->a_parts, index));
	}
	return NULL;
}

CatStringWo *jag_byt_name_get_short_type_name(JagBytName *name) {
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	int parts_length = cat_array_wo_size(priv->a_parts);
	return parts_length>0 ? CAT_STRING_WO(cat_array_wo_get_last(priv->a_parts)) : CAT_S(cat_s_empty_string);
}

JagBytName *jag_byt_name_create_package_name(JagBytName *name) {
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	int parts_length = cat_array_wo_size(priv->a_parts);
	if (parts_length>0) {
		CatArrayWo *new_parts = cat_array_wo_new_sub(priv->a_parts, 0, parts_length-1);
		return jag_byt_name_new_fast(new_parts);
	}
	return jag_byt_name_new_fast(cat_array_wo_new());
}

CatStringWo *jag_byt_name_create_fqn(JagBytName *name) {
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	CatStringWo *e_buf = cat_string_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(priv->a_parts);
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *a_id = (CatStringWo *) cat_iiterator_next(iter);
		if (cat_string_wo_length(e_buf)>0) {
			cat_string_wo_append_char(e_buf, '.');
		}
		cat_string_wo_append(e_buf, a_id);
	}
	cat_unref_ptr(iter);
	return cat_string_wo_anchor(e_buf, 0);
}


int jag_byt_name_hash_code(JagBytName *name) {
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	return priv->hashCode;
}

int jag_byt_name_equal(JagBytName *name, JagBytName *other) {
	if (other==name) {
		return TRUE;
	}
	if (other==NULL || name==NULL) {
		return  FALSE;
	}
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	JagBytNamePrivate *opriv = jag_byt_name_get_instance_private(other);
	if (cat_array_wo_size(priv->a_parts)!=cat_array_wo_size(opriv->a_parts)) {
		return FALSE;
	}
	int idx;
	for(idx=cat_array_wo_size(priv->a_parts)-1; idx>=0; idx--) {
		CatStringWo *a_part_a = (CatStringWo *) cat_array_wo_get(priv->a_parts, idx);
		CatStringWo *a_part_b = (CatStringWo *) cat_array_wo_get(opriv->a_parts, idx);
		if (!cat_string_wo_equal(a_part_a, a_part_b)) {
			return FALSE;
		}
	}
	return TRUE;
}

int jag_byt_name_compare(JagBytName *name, JagBytName *other) {
	if (name==other) {
		return 0;
	} else if (name==NULL) {
		return -1;
	} else if (other==NULL) {
		return 1;
	}

	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	JagBytNamePrivate *opriv = jag_byt_name_get_instance_private(other);

	int count = cat_array_wo_size(priv->a_parts);
	int ocount = cat_array_wo_size(opriv->a_parts);

	int result = 0;
	int idx;
	for(idx=0; result==0 && idx<ocount && idx<count; idx++) {
		CatStringWo *a_part_a = (CatStringWo *) cat_array_wo_get(priv->a_parts, idx);
		CatStringWo *a_part_b = (CatStringWo *) cat_array_wo_get(opriv->a_parts, idx);
		result = cat_string_wo_compare(a_part_a, a_part_b);
	}
	if (result==0) {
		if (count<ocount) {
			result = -1;
		} else if (count>ocount) {
			result = 1;
		}
	}

	return result;
}

gboolean jag_byt_name_starts_with(JagBytName *name, JagBytName *other) {
	JagBytNamePrivate *priv = jag_byt_name_get_instance_private(name);
	JagBytNamePrivate *opriv = jag_byt_name_get_instance_private(other);
	int length = cat_array_wo_size(priv->a_parts);
	int olength = cat_array_wo_size(opriv->a_parts);

	if (olength>length) {
		return FALSE;
	}
	int idx;
	for(idx=0; idx<olength; idx++) {
		CatStringWo *a_part_a = (CatStringWo *) cat_array_wo_get(priv->a_parts, idx);
		CatStringWo *a_part_b = (CatStringWo *) cat_array_wo_get(opriv->a_parts, idx);
		if (!cat_string_wo_equal(a_part_a, a_part_b)) {
			return FALSE;
		}
	}
	return TRUE;
}
