/*
   File:    jagbytattributemap.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 31, 2012
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

#include "jagbytattributemap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeMap"
#include <logging/catlog.h>

struct _JagBytAttributeMapPrivate {
	CatHashMapWo *e_map;
};

#define MAX_ATTRIBUTE_BLOCK_SIZE     0x200000

G_DEFINE_TYPE_WITH_PRIVATE(JagBytAttributeMap, jag_byt_attribute_map, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_attribute_map_class_init(JagBytAttributeMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_attribute_map_init(JagBytAttributeMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytAttributeMap *instance = JAG_BYT_ATTRIBUTE_MAP(object);
	JagBytAttributeMapPrivate *priv = jag_byt_attribute_map_get_instance_private(instance);
	cat_unref_ptr(priv->e_map);
	G_OBJECT_CLASS(jag_byt_attribute_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_attribute_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeMap *jag_byt_attribute_map_new(JagJObjectInputStream *dis, JagBytIConstantProvider *constant_provider, int cnt) {
	JagBytAttributeMap *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_MAP, NULL);
	cat_ref_anounce(result);
	JagBytAttributeMapPrivate *priv = jag_byt_attribute_map_get_instance_private(result);
	priv->e_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	int idx;
	for(idx=0; idx<cnt; idx++) {

		uint16_t attribute_name_index = jag_jobject_input_stream_read_short(dis);
		int length = jag_jobject_input_stream_read_int(dis);
		if (length<0 || length>MAX_ATTRIBUTE_BLOCK_SIZE) {
			cat_log_error("length of attribute exceeds limit: length=%d", length);
			break;
		}
		cat_log_debug("attribute_name_index:%d, length=%d", attribute_name_index, length);

		char *attribute_data = g_malloc(length+1);

		jag_jobject_input_stream_read_bytes(dis, attribute_data, length);
		CatStringWo *e_read_attr_data = cat_string_wo_new_nocopy(attribute_data, length);

		CatStringWo *a_attribute_name = jag_byt_iconstant_provider_get_constant_utf8_text(constant_provider, attribute_name_index-1);
		JagBytAttribute *attribute = NULL;
		if (cat_string_wo_equal(CAT_S(jag_txt_attribute_code) , a_attribute_name)) {
			a_attribute_name = CAT_S(jag_txt_attribute_code);
			attribute = (JagBytAttribute *) jag_byt_attribute_code_new(e_read_attr_data, constant_provider);
		} else if (cat_string_wo_equal(CAT_S(jag_txt_attribute_constant_value) , a_attribute_name)) {
			a_attribute_name = CAT_S(jag_txt_attribute_constant_value);
			attribute = (JagBytAttribute *) jag_byt_attribute_constant_value_new(e_read_attr_data);
		} else if (cat_string_wo_equal(CAT_S(jag_txt_attribute_exceptions) , a_attribute_name)) {
			a_attribute_name = CAT_S(jag_txt_attribute_exceptions);
			attribute = (JagBytAttribute *) jag_byt_attribute_exceptions_new(e_read_attr_data);
		} else if (cat_string_wo_equal(CAT_S(jag_txt_attribute_local_variable_table) , a_attribute_name)) {
			a_attribute_name = CAT_S(jag_txt_attribute_local_variable_table);
			attribute = (JagBytAttribute *) jag_byt_attribute_local_variable_table_new(e_read_attr_data, constant_provider);
		} else if (cat_string_wo_equal(CAT_S(jag_txt_attribute_line_number_table) , a_attribute_name)) {
			a_attribute_name = CAT_S(jag_txt_attribute_line_number_table);
			attribute = (JagBytAttribute *) jag_byt_attribute_line_number_table_new(e_read_attr_data, constant_provider);
		}

		if (attribute) {
			cat_hash_map_wo_put(priv->e_map, (GObject *) a_attribute_name, (GObject *) attribute);
			cat_unref_ptr(attribute);
		}
		cat_unref_ptr(e_read_attr_data);
	}

	return result;
}

JagBytAttribute *jag_byt_attribute_map_get(JagBytAttributeMap *attribute_map, CatStringWo *a_key) {
	JagBytAttributeMapPrivate *priv = jag_byt_attribute_map_get_instance_private(attribute_map);
	return (JagBytAttribute *) cat_hash_map_wo_get(priv->e_map, a_key);
}

JagBytAttributeCode *jag_byt_attribute_map_get_code(JagBytAttributeMap *attribute_map) {
	JagBytAttributeMapPrivate *priv = jag_byt_attribute_map_get_instance_private(attribute_map);
	return (JagBytAttributeCode *) cat_hash_map_wo_get(priv->e_map, CAT_S(jag_txt_attribute_code));
}

JagBytAttributeConstantValue *jag_byt_attribute_map_get_constant_value(JagBytAttributeMap *attribute_map) {
	JagBytAttributeMapPrivate *priv = jag_byt_attribute_map_get_instance_private(attribute_map);
	return (JagBytAttributeConstantValue *) cat_hash_map_wo_get(priv->e_map, CAT_S(jag_txt_attribute_constant_value));
}

JagBytAttributeExceptions *jag_byt_attribute_map_get_exceptions(JagBytAttributeMap *attribute_map) {
	JagBytAttributeMapPrivate *priv = jag_byt_attribute_map_get_instance_private(attribute_map);
	return (JagBytAttributeExceptions *) cat_hash_map_wo_get(priv->e_map, CAT_S(jag_txt_attribute_exceptions));
}
