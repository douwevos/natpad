/*
   File:    jagbytmethod.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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

#include "jagbytmethod.h"
#include "jagbytdefines.h"
#include "attribute/jagbytattributecode.h"
#include "attribute/jagbytattributesignature.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytMethod"
#include <logging/catlog.h>

struct _JagBytMethodPrivate {
	JagBytIConstantProvider *constant_provider;
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	JagBytAttributeMap *attribute_map;

	JagAstModifiers *modifiers;
	CatStringWo *a_method_name;
	JagBytMethodHeader *method_header;

	JagBytTryCatchList *try_catch_list;
};

G_DEFINE_TYPE (JagBytMethod, jag_byt_method, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_method_class_init(JagBytMethodClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytMethodPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_method_init(JagBytMethod *instance) {
	JagBytMethodPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_METHOD, JagBytMethodPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytMethod *instance = JAG_BYT_METHOD(object);
	JagBytMethodPrivate *priv = instance->priv;
	cat_unref_ptr(priv->constant_provider);
	cat_unref_ptr(priv->attribute_map);
	cat_unref_ptr(priv->modifiers);
	cat_unref_ptr(priv->a_method_name);
	cat_unref_ptr(priv->try_catch_list);
	cat_unref_ptr(priv->method_header);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytMethod *jag_byt_method_new(JagBytName *mainTypeName, JagBytIConstantProvider *constant_provider, uint16_t accessFlags, uint16_t nameIndex, uint16_t descriptorIndex, JagBytAttributeMap *attribute_map) {
	JagBytMethod *result = g_object_new(JAG_BYT_TYPE_METHOD, NULL);
	cat_ref_anounce(result);
	JagBytMethodPrivate *priv = result->priv;
	priv->constant_provider = cat_ref_ptr(constant_provider);
	priv->accessFlags = accessFlags;
	priv->nameIndex = nameIndex;
	priv->descriptorIndex = descriptorIndex;
	priv->attribute_map = cat_ref_ptr(attribute_map);

	priv->modifiers = jag_ast_modifiers_new(accessFlags);

	priv->a_method_name = jag_byt_iconstant_provider_get_constant_utf8_text(constant_provider, nameIndex-1);
	cat_ref_ptr(priv->a_method_name);

	CatStringWo *a_method_descriptor_text = jag_byt_iconstant_provider_get_constant_utf8_text(constant_provider, descriptorIndex-1);
	priv->method_header = jag_byt_method_header_new(mainTypeName, priv->a_method_name, a_method_descriptor_text);


	JagBytAttributeExceptions *attributeExceptions = jag_byt_attribute_map_get_exceptions(attribute_map);
	JagBytExceptions *exceptions = NULL;
	if (attributeExceptions) {
		exceptions = jag_byt_exceptions_new();
		int exceptionCount = jag_byt_attribute_exceptions_count(attributeExceptions);
		int idx;
		for(idx=0; idx<exceptionCount; idx++) {
			int poolIndex = jag_byt_attribute_exceptions_get(attributeExceptions, idx);
			JagBytIConstant *iconstant = jag_byt_iconstant_provider_get(constant_provider, poolIndex);
			if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
				JagBytConstantClazz *constantClass = JAG_BYT_CONSTANT_CLAZZ(iconstant);
				JagBytName *exception_type_name = jag_byt_constant_clazz_get_refrence_type_name(constantClass);
				jag_byt_exceptions_add(exceptions, exception_type_name);
			}
		}
	}

	jag_byt_method_header_set_exceptions(priv->method_header, exceptions);
	cat_unref_ptr(exceptions);


	priv->try_catch_list = NULL;

	return result;
}


uint16_t jag_byt_method_get_access_flags(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	return priv->accessFlags;
}

JagAstModifiers *jag_byt_method_get_modifiers(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	return priv->modifiers;
}


uint16_t jag_byt_method_get_name_index(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	return priv->nameIndex;
}

uint16_t jag_byt_method_get_descriptor_index(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	return priv->descriptorIndex;
}

JagBytAttributeMap *jag_byt_method_get_attribute_map(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	return priv->attribute_map;
}







CatStringWo *jag_byt_method_get_name(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	return priv->a_method_name;
}


JagBytMethodHeader *jag_byt_method_get_method_header(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	return priv->method_header;
}


JagBytAttributeLocalVariableTable *jag_byt_method_get_local_variable_table(JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	JagBytAttributeLocalVariableTable *result = NULL;
	JagBytAttributeCode *attributeCode = jag_byt_attribute_map_get_code(priv->attribute_map);
	if (attributeCode!=NULL) {
		result = jag_byt_attribute_code_get_local_variable_table(attributeCode);
	}
	return result;
}

JagBytAttributeLineNumberTable *jag_byt_method_get_line_number_table(JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	JagBytAttributeLineNumberTable *result = NULL;
	JagBytAttributeCode *attributeCode = jag_byt_attribute_map_get_code(priv->attribute_map);
	if (attributeCode!=NULL) {
		result = jag_byt_attribute_code_get_line_number_table(attributeCode);
	}
	return result;
}



CatStringWo *jag_byt_method_get_bytecode(const JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	CatStringWo *e_result = NULL;
	JagBytAttributeCode *attribute_code = jag_byt_attribute_map_get_code(priv->attribute_map);
	if (attribute_code) {
		e_result = jag_byt_attribute_code_get_bytecode(attribute_code);
	}
	return e_result;
}


JagBytTryCatchList *jag_byt_method_get_try_catch_list(JagBytMethod *method) {
	JagBytMethodPrivate *priv = JAG_BYT_METHOD_GET_PRIVATE(method);
	if (priv->try_catch_list==NULL) {
		JagBytAttributeCode *attributeCode = jag_byt_attribute_map_get_code(priv->attribute_map);
		if (attributeCode!=NULL) {
			CatArrayWo *e_attr_code_exceptions = jag_byt_attribute_code_get_exceptions(attributeCode);
			priv->try_catch_list = jag_byt_try_catch_list_new(e_attr_code_exceptions, priv->constant_provider);
		}
	}
	return priv->try_catch_list;
}

