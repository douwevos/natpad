/*
   File:    shoserializer.c
   Project: shoveler
   Author:  Douwe Vos
   Date:    May 7, 2012
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

#include "shoserializer.h"
#include "../model/shoreference.h"
#include "../model/shoivalue.h"
#include "../model/shosimplevalue.h"
#include "../model/shoblockvalue.h"
#include "../model/sholistvalue.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoSerializer"
#include <logging/catlog.h>

struct _ShoSerializerPrivate {
	ShoModel *model;
	int indent;
};

G_DEFINE_TYPE_WITH_PRIVATE(ShoSerializer, sho_serializer, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_serializer_class_init(ShoSerializerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_serializer_init(ShoSerializer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoSerializer *instance = SHO_SERIALIZER(object);
	ShoSerializerPrivate *priv = sho_serializer_get_instance_private(instance);
	cat_unref_ptr(priv->model);
	G_OBJECT_CLASS(sho_serializer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_serializer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoSerializer *sho_serializer_new(ShoModel *model) {
	ShoSerializer *result = g_object_new(SHO_TYPE_SERIALIZER, NULL);
	cat_ref_anounce(result);
	ShoSerializerPrivate *priv = sho_serializer_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->indent = 0;
	return result;
}

static void l_serialize_reference_to_char_array(ShoSerializerPrivate *priv, CatStringWo *e_out, ShoReference *reference, gboolean close_with_semicolon);
static void l_serialize_list_value_to_char_array(ShoSerializerPrivate *priv, CatStringWo *e_out, ShoListValue *list_value);

static void l_indent(ShoSerializerPrivate *priv, CatStringWo *e_out) {
	int cnt = priv->indent;
	while(cnt-->0) {
		cat_string_wo_append_chars_len(e_out, "  ", 2);
	}
}

static void l_serialize_block_value_to_char_array(ShoSerializerPrivate *priv, CatStringWo *e_out, ShoBlockValue *block_value) {
	if (sho_reference_list_count((ShoReferenceList *) block_value)==0) {
		cat_string_wo_append_chars(e_out, "{}");
	} else {
		l_indent(priv, e_out);
		cat_string_wo_append_chars(e_out, "{\n");
		priv->indent++;
		CatIIterator *iter = sho_reference_list_iterator((ShoReferenceList *) block_value);
		while(cat_iiterator_has_next(iter)) {
			ShoReference *ref = (ShoReference *) cat_iiterator_next(iter);
			l_indent(priv, e_out);
			l_serialize_reference_to_char_array(priv, e_out, ref, TRUE);
			cat_string_wo_append_chars(e_out, "\n");
		}
		priv->indent--;
		l_indent(priv, e_out);
		cat_string_wo_append_chars(e_out, "}");
		cat_unref_ptr(iter);
	}
}

static void l_encode(CatStringWo *in, CatStringWo *e_out, char marker) {
	int cnt = cat_string_wo_length(in);
	int idx;
	cat_string_wo_append_char(e_out, marker);
	for(idx=0; idx<cnt; idx++) {
		char ch = cat_string_wo_char_at(in, idx);
		if (ch==marker) {
			cat_string_wo_append_char(e_out, '\\');
			cat_string_wo_append_char(e_out, marker);
		} else {
			switch(ch) {
				case '\\' : cat_string_wo_append_chars(e_out, "\\\\"); break;
				case 7 : cat_string_wo_append_chars(e_out, "\\a"); break;
				case 8 : cat_string_wo_append_chars(e_out, "\\b"); break;
				case 9 : cat_string_wo_append_chars(e_out, "\\t"); break;
				case 10 : cat_string_wo_append_chars(e_out, "\\n"); break;
				case 11 : cat_string_wo_append_chars(e_out, "\\v"); break;
				case 12 : cat_string_wo_append_chars(e_out, "\\f"); break;
				case 13 : cat_string_wo_append_chars(e_out, "\\r"); break;
				case 27 : cat_string_wo_append_chars(e_out, "\\e"); break;
				default : cat_string_wo_append_char(e_out, ch); break;
			}
		}
	}
	cat_string_wo_append_char(e_out, marker);
}

static void l_serialize_simple_value_to_char_array(ShoSerializerPrivate *priv, CatStringWo *e_out, ShoSimpleValue *simple_value) {
	CatStringWo *str = sho_simple_value_get_string(simple_value);
	if (str==NULL) {
		cat_string_wo_append_chars(e_out, "nil");
	} else {
		int singleCount = cat_string_wo_count_occurrence(str, '\'');
		int doubleCount = cat_string_wo_count_occurrence(str, '"');

		if (doubleCount<singleCount) {
			l_encode(str, e_out, '"');
		} else {
			l_encode(str, e_out, '\'');
		}
	}
}

static void l_serialize_value_to_char_array(ShoSerializerPrivate *priv, CatStringWo *e_out, ShoIValue *value, gboolean close_with_semicolon) {
	if (value==NULL) {
		if (close_with_semicolon) {
			cat_string_wo_append_chars(e_out, "nil;");
		} else {
			cat_string_wo_append_chars(e_out, "nil");
		}
	} else if (SHO_IS_BLOCK_VALUE(value)) {
		ShoBlockValue *block_value = SHO_BLOCK_VALUE(value);
		l_serialize_block_value_to_char_array(priv, e_out, block_value);
	} else if (SHO_IS_SIMPLE_VALUE(value)) {
		ShoSimpleValue *simple_value = SHO_SIMPLE_VALUE(value);
		l_serialize_simple_value_to_char_array(priv, e_out, simple_value);
		if (close_with_semicolon) {
			cat_string_wo_append_chars(e_out, ";");
		}
	} else if (SHO_IS_LIST_VALUE(value)) {
		ShoListValue *list_value = SHO_LIST_VALUE(value);
		l_serialize_list_value_to_char_array(priv, e_out, list_value);
	}
}


static void l_serialize_list_value_to_char_array(ShoSerializerPrivate *priv, CatStringWo *e_out, ShoListValue *list_value) {
	CatIIterator *iter = sho_list_value_iterator(list_value);
	gboolean is_first = TRUE;
	cat_string_wo_append_chars(e_out, "{");
	while(cat_iiterator_has_next(iter)) {
		if (is_first) {
			is_first = FALSE;
		} else {
			cat_string_wo_append_chars(e_out, ", ");
		}
		ShoIValue *value = (ShoIValue *) cat_iiterator_next(iter);
		l_serialize_value_to_char_array(priv, e_out, value, FALSE);
	}
	cat_string_wo_append_chars(e_out, "}");
	cat_unref_ptr(iter);
}

static void l_serialize_reference_to_char_array(ShoSerializerPrivate *priv, CatStringWo *e_out, ShoReference *reference, gboolean close_with_semicolon) {
	CatStringWo *ref_id = sho_reference_get_id(reference);
	cat_string_wo_append(e_out, ref_id);
	cat_string_wo_append_char(e_out, ' ');
	ShoIValue *value = sho_reference_get_value(reference);
	l_serialize_value_to_char_array(priv, e_out, value, TRUE);
}

CatStringWo *sho_serialize_to_char_array(ShoSerializer *serializer) {
	ShoSerializerPrivate *priv = sho_serializer_get_instance_private(serializer);
	CatStringWo *result = cat_string_wo_new();

//	sho_model_create_reference_iterator(priv->model);

	CatIIterator *iter = sho_reference_list_iterator((ShoReferenceList *) priv->model);
	while(cat_iiterator_has_next(iter)) {
		ShoReference *reference = (ShoReference *) cat_iiterator_next(iter);
		l_serialize_reference_to_char_array(priv, result, reference, TRUE);
		cat_string_wo_append_chars(result, "\n");
	}
	cat_unref_ptr(iter);

	return result;
}
