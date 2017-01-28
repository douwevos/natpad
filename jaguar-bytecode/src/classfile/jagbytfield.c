/*
   File:    jagbytfield.c
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

#include "jagbytfield.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytField"
#include <logging/catlog.h>

struct _JagBytFieldPrivate {
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	JagBytAttributeMap *attribute_map;

	JagAstModifiers *modifiers;
	CatStringWo *a_name;
	JagAstDeclarationType *declaration_type;
	JagBytIConstant *value;
};

G_DEFINE_TYPE(JagBytField, jag_byt_field, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_field_class_init(JagBytFieldClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytFieldPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_field_init(JagBytField *instance) {
	JagBytFieldPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_FIELD, JagBytFieldPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytField *instance = JAG_BYT_FIELD(object);
	JagBytFieldPrivate *priv = instance->priv;
	cat_unref_ptr(priv->modifiers);
	cat_unref_ptr(priv->a_name);
	cat_unref_ptr(priv->attribute_map);
	cat_unref_ptr(priv->declaration_type);
	cat_unref_ptr(priv->value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytField *jag_byt_field_new(JagBytIConstantProvider *constant_provider, uint16_t accessFlags, uint16_t nameIndex, uint16_t descriptorIndex, JagBytAttributeMap *attribute_map) {
	JagBytField *result = g_object_new(JAG_BYT_TYPE_FIELD, NULL);
	cat_ref_anounce(result);
	JagBytFieldPrivate *priv = result->priv;
	priv->accessFlags = accessFlags;
	priv->nameIndex = nameIndex;
	priv->descriptorIndex = descriptorIndex;
	priv->attribute_map = cat_ref_ptr(attribute_map);

	priv->modifiers = jag_ast_modifiers_new(priv->accessFlags);
	priv->a_name = jag_byt_iconstant_provider_get_constant_utf8_text(constant_provider, ((int) nameIndex & 0xFFFF)-1);
	cat_ref_ptr(priv->a_name);
	CatStringWo *a_description = jag_byt_iconstant_provider_get_constant_utf8_text(constant_provider, ((int) descriptorIndex & 0xFFFF)-1);
	priv->declaration_type = jag_byt_field_parse_descriptor(a_description);

	JagBytAttributeConstantValue *constantValue = jag_byt_attribute_map_get_constant_value(attribute_map);
	if (constantValue!=NULL) {
		int constantValueIndex = jag_byt_attribute_constant_value_get_index(constantValue);
		priv->value = jag_byt_iconstant_provider_get(constant_provider, constantValueIndex-1);
		cat_ref_ptr(priv->value);
	} else {
		priv->value = NULL;
	}

	return result;
}



uint16_t jag_byt_field_get_access_flags(const JagBytField *field) {
	JagBytFieldPrivate *priv = JAG_BYT_FIELD_GET_PRIVATE(field);
	return priv->accessFlags;
}


uint16_t jag_byt_field_get_name_index(const JagBytField *field) {
	JagBytFieldPrivate *priv = JAG_BYT_FIELD_GET_PRIVATE(field);
	return priv->nameIndex;
}

uint16_t jag_byt_field_get_descriptor_index(const JagBytField *field) {
	JagBytFieldPrivate *priv = JAG_BYT_FIELD_GET_PRIVATE(field);
	return priv->descriptorIndex;
}

JagAstDeclarationType *jag_byt_field_get_declaration_type(const JagBytField *field) {
	JagBytFieldPrivate *priv = JAG_BYT_FIELD_GET_PRIVATE(field);
	return priv->declaration_type;
}


CatStringWo *jag_byt_field_get_name(const JagBytField *field) {
	JagBytFieldPrivate *priv = JAG_BYT_FIELD_GET_PRIVATE(field);
	return priv->a_name;
}

JagBytIConstant *jag_byt_field_get_constant_value(const JagBytField *field) {
	return JAG_BYT_FIELD_GET_PRIVATE(field)->value;
}







JagAstDeclarationType *jag_byt_field_parse_descriptor(CatStringWo *a_description) {
	cat_ref_sink_ptr(a_description);
	JagAstDeclarationType *desc = NULL;
	gboolean expectRef = FALSE;
	int referenceCount = 0;
	JagBytType dPrimitive = JAG_BYT_TYPE_VOID;
	CatStringWo *e_extBuf = cat_string_wo_new();
	int idx = 0;
	while(idx<cat_string_wo_length(a_description)) {
		gboolean storeAndCreateNew = FALSE;
		char ch = cat_string_wo_char_at(a_description, idx);
		if (expectRef) {
			if (ch==';') {
				storeAndCreateNew = TRUE;
			} else {
				cat_string_wo_append_char(e_extBuf, ch);
			}
		} else {
			switch(ch) {
				case 'B' : {
					dPrimitive = JAG_BYT_TYPE_BYTE;
					storeAndCreateNew = TRUE;
				} break;
				case 'C': {
					dPrimitive = JAG_BYT_TYPE_CHAR;
					storeAndCreateNew = TRUE;
				} break;
				case 'D': {
					dPrimitive = JAG_BYT_TYPE_DOUBLE;
					storeAndCreateNew = TRUE;
				} break;
				case 'F': {
					dPrimitive = JAG_BYT_TYPE_FLOAT;
					storeAndCreateNew = TRUE;
				} break;
				case 'I': {
					dPrimitive = JAG_BYT_TYPE_INT;
					storeAndCreateNew = TRUE;
				} break;
				case 'J': {
					dPrimitive = JAG_BYT_TYPE_LONG;
					storeAndCreateNew = TRUE;
				} break;
				case 'L': {
					expectRef = TRUE;
				} break;
				case 'S': {
					dPrimitive = JAG_BYT_TYPE_SHORT;
					storeAndCreateNew = TRUE;
				} break;
				case 'Z': {
					dPrimitive = JAG_BYT_TYPE_BOOLEAN;
					storeAndCreateNew = TRUE;
				} break;
				case 'V': {
					dPrimitive = JAG_BYT_TYPE_VOID;
					storeAndCreateNew = TRUE;
				} break;
				case '[': {
					referenceCount++;
				} break;

			}

		}
		if (storeAndCreateNew) {
			if (expectRef) {
				JagBytName *referenceTypeName = jag_byt_name_new(cat_string_wo_anchor(e_extBuf, 0), TRUE);
				e_extBuf = NULL;
				desc = jag_ast_declaration_type_new_reference(referenceTypeName, referenceCount);
				cat_unref_ptr(referenceTypeName);
			} else {
				desc = jag_ast_declaration_type_new(dPrimitive, referenceCount);
			}
			break;
		}
		idx++;
	}
	cat_unref_ptr(e_extBuf);
	cat_unref_ptr(a_description);
	return desc;
}
