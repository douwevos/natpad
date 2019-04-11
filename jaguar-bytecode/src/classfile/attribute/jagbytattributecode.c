/*
   File:    jagbytattributecode.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Mar 6, 2012
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

#include "jagbytattributecode.h"
#include "jagbytattributelocalvariabletable.h"
#include "jagbytattributecodeexception.h"
#include "jagbytattributemap.h"
#include "../../jagjobjectinputstream.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeCode"
#include <logging/catlog.h>

CatS jag_txt_attribute_code  = CAT_S_DEF("Code");

struct _JagBytAttributeCodePrivate {
	uint16_t max_stack, max_locals;

	CatStringWo *e_bytecode;

	CatArrayWo *e_exceptions;

	JagBytAttributeMap *attribute_map;
};

G_DEFINE_TYPE (JagBytAttributeCode, jag_byt_attribute_code, JAG_BYT_TYPE_ATTRIBUTE) // @suppress("Unused static function")

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_attribute_code_class_init(JagBytAttributeCodeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytAttributeCodePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_attribute_code_init(JagBytAttributeCode *instance) {
	JagBytAttributeCodePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_ATTRIBUTE_CODE, JagBytAttributeCodePrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytAttributeCode *instance = JAG_BYT_ATTRIBUTE_CODE(object);
	JagBytAttributeCodePrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_bytecode);
	cat_unref_ptr(priv->e_exceptions);
	cat_unref_ptr(priv->attribute_map);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeCode *jag_byt_attribute_code_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider) {
	JagBytAttributeCode *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_CODE, NULL);
	cat_ref_anounce(result);
	JagBytAttributeCodePrivate *priv = result->priv;
	jag_byt_attribute_construct((JagBytAttribute *) result);
	JagJObjectInputStream *obj_istream = jag_jobject_input_stream_new_from_array(e_attribute_data);
	priv->max_stack = jag_jobject_input_stream_read_short(obj_istream);
	priv->max_locals = jag_jobject_input_stream_read_short(obj_istream);
	int codeLength = jag_jobject_input_stream_read_int(obj_istream);

	char *data = g_malloc(codeLength+1);
	jag_jobject_input_stream_read_bytes(obj_istream, data, codeLength);
	priv->e_bytecode = cat_string_wo_new_nocopy(data, codeLength);

	priv->e_exceptions = cat_array_wo_new();
	int exceptionTableLength = (int) jag_jobject_input_stream_read_short(obj_istream) & 0xFFFF;
	int idx;
	for(idx=0; idx<exceptionTableLength; idx++) {
		uint16_t startPc = jag_jobject_input_stream_read_short(obj_istream);
		uint16_t endPc = jag_jobject_input_stream_read_short(obj_istream);
		uint16_t handlerPc = jag_jobject_input_stream_read_short(obj_istream);
		uint16_t catchType = jag_jobject_input_stream_read_short(obj_istream);
		JagBytAttributeCodeException *exception = jag_byt_attribute_code_exception_new(startPc, endPc, handlerPc, catchType);
		cat_array_wo_append(priv->e_exceptions, (GObject *) exception);
		cat_unref_ptr(exception);
	}

	int attributeCount = jag_jobject_input_stream_read_short(obj_istream) & 0xFFFF;
	priv->attribute_map = jag_byt_attribute_map_new(obj_istream, constant_provider, attributeCount);

	cat_unref_ptr(obj_istream);
	return result;
}

CatStringWo *jag_byt_attribute_code_get_bytecode(JagBytAttributeCode *attribute_code) {
	JagBytAttributeCodePrivate *priv = JAG_BYT_ATTRIBUTE_CODE_GET_PRIVATE(attribute_code);
	return priv->e_bytecode;
}

CatArrayWo *jag_byt_attribute_code_get_exceptions(JagBytAttributeCode *attribute_code) {
	JagBytAttributeCodePrivate *priv = JAG_BYT_ATTRIBUTE_CODE_GET_PRIVATE(attribute_code);
	return priv->e_exceptions;
}

JagBytAttributeLocalVariableTable *jag_byt_attribute_code_get_local_variable_table(JagBytAttributeCode *attribute_code) {
	JagBytAttributeCodePrivate *priv = JAG_BYT_ATTRIBUTE_CODE_GET_PRIVATE(attribute_code);
	JagBytAttributeLocalVariableTable *result = NULL;
	JagBytAttribute *attr = jag_byt_attribute_map_get(priv->attribute_map, CAT_S(jag_txt_attribute_local_variable_table));
	if (JAG_BYT_IS_ATTRIBUTE_LOCAL_VARIABLE_TABLE(attr)) {
		result = JAG_BYT_ATTRIBUTE_LOCAL_VARIABLE_TABLE(attr);
	}
	return result;
}

JagBytAttributeLineNumberTable *jag_byt_attribute_code_get_line_number_table(JagBytAttributeCode *attribute_code) {
	JagBytAttributeCodePrivate *priv = JAG_BYT_ATTRIBUTE_CODE_GET_PRIVATE(attribute_code);
	JagBytAttributeLineNumberTable *result = NULL;
	JagBytAttribute *attr = jag_byt_attribute_map_get(priv->attribute_map, CAT_S(jag_txt_attribute_line_number_table));
	if (JAG_BYT_IS_ATTRIBUTE_LINE_NUMBER_TABLE(attr)) {
		result = JAG_BYT_ATTRIBUTE_LINE_NUMBER_TABLE(attr);
	}
	return result;
}


static char jag_txt_hex[] = "0123456789ABCDEF";


static void jag_hex8(CatStringWo *e_output, int val) {
	int p;
	for(p=28; p>=0; p-=4) {
		cat_string_wo_append_char(e_output, jag_txt_hex[(val>>p) & 0xF]);
	}


}

void jag_byt_attribute_code_as_source(JagBytAttributeCode *code_attribute, CatStringWo *e_source, CatStringWo *a_indent) {
	JagBytAttributeCodePrivate *priv = code_attribute->priv;

	CatStringWo *e_mnemonic = cat_string_wo_new();

	int idx = 4;
	int count = cat_string_wo_char_at(priv->e_bytecode, idx++) & 0xFF;
	count = (count<<8) + (cat_string_wo_char_at(priv->e_bytecode, idx++) & 0xFF);
	count = (count<<8) + (cat_string_wo_char_at(priv->e_bytecode, idx++) & 0xFF);
	count = (count<<8) + (cat_string_wo_char_at(priv->e_bytecode, idx++) & 0xFF);
	count += idx;

	if (count>cat_string_wo_length(priv->e_bytecode)) {
		count = cat_string_wo_length(priv->e_bytecode);
	}

	while(idx<count) {
		cat_string_wo_append(e_source, a_indent);

		int start_idx = idx;
		cat_string_wo_clear(e_mnemonic);

		switch(((int) cat_string_wo_char_at(priv->e_bytecode, idx++)) & 0xFF) {
			case 0x0 : cat_string_wo_append_chars(e_mnemonic, "nop"); break;
			case 0x1 : cat_string_wo_append_chars(e_mnemonic, "aconst_null"); break;
			case 0x2 : cat_string_wo_append_chars(e_mnemonic, "iconst_m1"); break;
			case 0x3 : cat_string_wo_append_chars(e_mnemonic, "iconst_0"); break;
			case 0x4 : cat_string_wo_append_chars(e_mnemonic, "iconst_1"); break;
			case 0x5 : cat_string_wo_append_chars(e_mnemonic, "iconst_2"); break;
			case 0x6 : cat_string_wo_append_chars(e_mnemonic, "iconst_3"); break;
			case 0x7 : cat_string_wo_append_chars(e_mnemonic, "iconst_4"); break;
			case 0x8 : cat_string_wo_append_chars(e_mnemonic, "iconst_5"); break;
			case 0x9 : cat_string_wo_append_chars(e_mnemonic, "lconst_0"); break;
			case 0xa : cat_string_wo_append_chars(e_mnemonic, "lconst_1"); break;
			case 0xb : cat_string_wo_append_chars(e_mnemonic, "fconst_0"); break;
			case 0xc : cat_string_wo_append_chars(e_mnemonic, "fconst_1"); break;
			case 0xd : cat_string_wo_append_chars(e_mnemonic, "fconst_2"); break;
			case 0xe : cat_string_wo_append_chars(e_mnemonic, "dconst_0"); break;
			case 0xf : cat_string_wo_append_chars(e_mnemonic, "dconst_1"); break;
			case 0x10 : {
				cat_string_wo_append_chars(e_mnemonic, "bipush\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x11 : {
				cat_string_wo_append_chars(e_mnemonic, "sipush\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					if (index&0x8000) {
						index = index+~0xFFFF;
					}
					cat_string_wo_append_decimal(e_mnemonic, index & 0xFFFF);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x12 : {
				cat_string_wo_append_chars(e_mnemonic, "ldc\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & 0xFF);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x13 : {
				cat_string_wo_append_chars(e_mnemonic, "ldc_w\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index & 0xFFFF);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x14 : {
				cat_string_wo_append_chars(e_mnemonic, "ldc2_w\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index & 0xFFFF);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x15 : {
				cat_string_wo_append_chars(e_mnemonic, "iload\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x16 : {
				cat_string_wo_append_chars(e_mnemonic, "lload\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x17 : {
				cat_string_wo_append_chars(e_mnemonic, "fload\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x18 : {
				cat_string_wo_append_chars(e_mnemonic, "dload\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x19 : {
				cat_string_wo_append_chars(e_mnemonic, "aload\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x1a : cat_string_wo_append_chars(e_mnemonic, "iload_0"); break;
			case 0x1b : cat_string_wo_append_chars(e_mnemonic, "iload_1"); break;
			case 0x1c : cat_string_wo_append_chars(e_mnemonic, "iload_2"); break;
			case 0x1d : cat_string_wo_append_chars(e_mnemonic, "iload_3"); break;
			case 0x1e : cat_string_wo_append_chars(e_mnemonic, "lload_0"); break;
			case 0x1f : cat_string_wo_append_chars(e_mnemonic, "lload_1"); break;
			case 0x20 : cat_string_wo_append_chars(e_mnemonic, "lload_2"); break;
			case 0x21 : cat_string_wo_append_chars(e_mnemonic, "lload_3"); break;
			case 0x22 : cat_string_wo_append_chars(e_mnemonic, "fload_0"); break;
			case 0x23 : cat_string_wo_append_chars(e_mnemonic, "fload_1"); break;
			case 0x24 : cat_string_wo_append_chars(e_mnemonic, "fload_2"); break;
			case 0x25 : cat_string_wo_append_chars(e_mnemonic, "fload_3"); break;
			case 0x26 : cat_string_wo_append_chars(e_mnemonic, "dload_0"); break;
			case 0x27 : cat_string_wo_append_chars(e_mnemonic, "dload_1"); break;
			case 0x28 : cat_string_wo_append_chars(e_mnemonic, "dload_2"); break;
			case 0x29 : cat_string_wo_append_chars(e_mnemonic, "dload_3"); break;
			case 0x2a : cat_string_wo_append_chars(e_mnemonic, "aload_0"); break;
			case 0x2b : cat_string_wo_append_chars(e_mnemonic, "aload_1"); break;
			case 0x2c : cat_string_wo_append_chars(e_mnemonic, "aload_2"); break;
			case 0x2d : cat_string_wo_append_chars(e_mnemonic, "aload_3"); break;
			case 0x2e : cat_string_wo_append_chars(e_mnemonic, "iaload"); break;
			case 0x2f : cat_string_wo_append_chars(e_mnemonic, "laload"); break;
			case 0x30 : cat_string_wo_append_chars(e_mnemonic, "faload"); break;
			case 0x31 : cat_string_wo_append_chars(e_mnemonic, "daload"); break;
			case 0x32 : cat_string_wo_append_chars(e_mnemonic, "aaload"); break;
			case 0x33 : cat_string_wo_append_chars(e_mnemonic, "baload"); break;
			case 0x34 : cat_string_wo_append_chars(e_mnemonic, "caload"); break;
			case 0x35 : cat_string_wo_append_chars(e_mnemonic, "saload"); break;
			case 0x36 : {
				cat_string_wo_append_chars(e_mnemonic, "istore\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x37 : {
				cat_string_wo_append_chars(e_mnemonic, "lstore\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x38 : {
				cat_string_wo_append_chars(e_mnemonic, "fstore\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x39 : {
				cat_string_wo_append_chars(e_mnemonic, "dstore\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x3a : {
				cat_string_wo_append_chars(e_mnemonic, "astore\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index & (0xFF));
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x3b : cat_string_wo_append_chars(e_mnemonic, "istore_0"); break;
			case 0x3c : cat_string_wo_append_chars(e_mnemonic, "istore_1"); break;
			case 0x3d : cat_string_wo_append_chars(e_mnemonic, "istore_2"); break;
			case 0x3e : cat_string_wo_append_chars(e_mnemonic, "istore_3"); break;
			case 0x3f : cat_string_wo_append_chars(e_mnemonic, "lstore_0"); break;
			case 0x40 : cat_string_wo_append_chars(e_mnemonic, "lstore_1"); break;
			case 0x41 : cat_string_wo_append_chars(e_mnemonic, "lstore_2"); break;
			case 0x42 : cat_string_wo_append_chars(e_mnemonic, "lstore_3"); break;
			case 0x43 : cat_string_wo_append_chars(e_mnemonic, "fstore_0"); break;
			case 0x44 : cat_string_wo_append_chars(e_mnemonic, "fstore_1"); break;
			case 0x45 : cat_string_wo_append_chars(e_mnemonic, "fstore_2"); break;
			case 0x46 : cat_string_wo_append_chars(e_mnemonic, "fstore_3"); break;
			case 0x47 : cat_string_wo_append_chars(e_mnemonic, "dstore_0"); break;
			case 0x48 : cat_string_wo_append_chars(e_mnemonic, "dstore_1"); break;
			case 0x49 : cat_string_wo_append_chars(e_mnemonic, "dstore_2"); break;
			case 0x4a : cat_string_wo_append_chars(e_mnemonic, "dstore_3"); break;
			case 0x4b : cat_string_wo_append_chars(e_mnemonic, "astore_0"); break;
			case 0x4c : cat_string_wo_append_chars(e_mnemonic, "astore_1"); break;
			case 0x4d : cat_string_wo_append_chars(e_mnemonic, "astore_2"); break;
			case 0x4e : cat_string_wo_append_chars(e_mnemonic, "astore_3"); break;
			case 0x4f : cat_string_wo_append_chars(e_mnemonic, "iastore"); break;
			case 0x50 : cat_string_wo_append_chars(e_mnemonic, "lastore"); break;
			case 0x51 : cat_string_wo_append_chars(e_mnemonic, "fastore"); break;
			case 0x52 : cat_string_wo_append_chars(e_mnemonic, "dastore"); break;
			case 0x53 : cat_string_wo_append_chars(e_mnemonic, "aastore"); break;
			case 0x54 : cat_string_wo_append_chars(e_mnemonic, "bastore"); break;
			case 0x55 : cat_string_wo_append_chars(e_mnemonic, "castore"); break;
			case 0x56 : cat_string_wo_append_chars(e_mnemonic, "sastore"); break;
			case 0x57 : cat_string_wo_append_chars(e_mnemonic, "pop"); break;
			case 0x58 : cat_string_wo_append_chars(e_mnemonic, "pop2"); break;
			case 0x59 : cat_string_wo_append_chars(e_mnemonic, "dup"); break;
			case 0x5a : cat_string_wo_append_chars(e_mnemonic, "dup_x1"); break;
			case 0x5b : cat_string_wo_append_chars(e_mnemonic, "dup_x2"); break;
			case 0x5c : cat_string_wo_append_chars(e_mnemonic, "dup2"); break;
			case 0x5d : cat_string_wo_append_chars(e_mnemonic, "dup2_x1"); break;
			case 0x5e : cat_string_wo_append_chars(e_mnemonic, "dup2_x2"); break;
			case 0x5f : cat_string_wo_append_chars(e_mnemonic, "swap"); break;
			case 0x60 : cat_string_wo_append_chars(e_mnemonic, "iadd"); break;
			case 0x61 : cat_string_wo_append_chars(e_mnemonic, "ladd"); break;
			case 0x62 : cat_string_wo_append_chars(e_mnemonic, "fadd"); break;
			case 0x63 : cat_string_wo_append_chars(e_mnemonic, "dadd"); break;
			case 0x64 : cat_string_wo_append_chars(e_mnemonic, "isub"); break;
			case 0x65 : cat_string_wo_append_chars(e_mnemonic, "lsub"); break;
			case 0x66 : cat_string_wo_append_chars(e_mnemonic, "fsub"); break;
			case 0x67 : cat_string_wo_append_chars(e_mnemonic, "dsub"); break;
			case 0x68 : cat_string_wo_append_chars(e_mnemonic, "imul"); break;
			case 0x69 : cat_string_wo_append_chars(e_mnemonic, "lmul"); break;
			case 0x6a : cat_string_wo_append_chars(e_mnemonic, "fmul"); break;
			case 0x6b : cat_string_wo_append_chars(e_mnemonic, "dmul"); break;
			case 0x6c : cat_string_wo_append_chars(e_mnemonic, "idiv"); break;
			case 0x6d : cat_string_wo_append_chars(e_mnemonic, "ldiv"); break;
			case 0x6e : cat_string_wo_append_chars(e_mnemonic, "fdiv"); break;
			case 0x6f : cat_string_wo_append_chars(e_mnemonic, "ddiv"); break;
			case 0x70 : cat_string_wo_append_chars(e_mnemonic, "irem"); break;
			case 0x71 : cat_string_wo_append_chars(e_mnemonic, "lrem"); break;
			case 0x72 : cat_string_wo_append_chars(e_mnemonic, "frem"); break;
			case 0x73 : cat_string_wo_append_chars(e_mnemonic, "drem"); break;
			case 0x74 : cat_string_wo_append_chars(e_mnemonic, "ineg"); break;
			case 0x75 : cat_string_wo_append_chars(e_mnemonic, "lneg"); break;
			case 0x76 : cat_string_wo_append_chars(e_mnemonic, "fneg"); break;
			case 0x77 : cat_string_wo_append_chars(e_mnemonic, "dneg"); break;
			case 0x78 : cat_string_wo_append_chars(e_mnemonic, "ishl"); break;
			case 0x79 : cat_string_wo_append_chars(e_mnemonic, "lshl"); break;
			case 0x7a : cat_string_wo_append_chars(e_mnemonic, "ishr"); break;
			case 0x7b : cat_string_wo_append_chars(e_mnemonic, "lshr"); break;
			case 0x7c : cat_string_wo_append_chars(e_mnemonic, "iushr"); break;
			case 0x7d : cat_string_wo_append_chars(e_mnemonic, "lushr"); break;
			case 0x7e : cat_string_wo_append_chars(e_mnemonic, "iand"); break;
			case 0x7f : cat_string_wo_append_chars(e_mnemonic, "land"); break;
			case 0x80 : cat_string_wo_append_chars(e_mnemonic, "ior"); break;
			case 0x81 : cat_string_wo_append_chars(e_mnemonic, "lor"); break;
			case 0x82 : cat_string_wo_append_chars(e_mnemonic, "ixor"); break;
			case 0x83 : cat_string_wo_append_chars(e_mnemonic, "lxor"); break;
			case 0x84 : {
				cat_string_wo_append_chars(e_mnemonic, "iinc\t");
				if (idx+1<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int constval = cat_string_wo_char_at(priv->e_bytecode, idx++);

					cat_string_wo_append_decimal(e_mnemonic, index&0xFF);
					cat_string_wo_append_chars(e_mnemonic, ",");
					cat_string_wo_append_decimal(e_mnemonic, constval);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0x85 : cat_string_wo_append_chars(e_mnemonic, "i2l");  break;
			case 0x86 : cat_string_wo_append_chars(e_mnemonic, "i2f");  break;
			case 0x87 : cat_string_wo_append_chars(e_mnemonic, "i2d");  break;
			case 0x88 : cat_string_wo_append_chars(e_mnemonic, "l2i");  break;
			case 0x89 : cat_string_wo_append_chars(e_mnemonic, "l2f");  break;
			case 0x8a : cat_string_wo_append_chars(e_mnemonic, "l2d");  break;
			case 0x8b : cat_string_wo_append_chars(e_mnemonic, "f2i");  break;
			case 0x8c : cat_string_wo_append_chars(e_mnemonic, "f2l");  break;
			case 0x8d : cat_string_wo_append_chars(e_mnemonic, "f2d");  break;
			case 0x8e : cat_string_wo_append_chars(e_mnemonic, "d2i");  break;
			case 0x8f : cat_string_wo_append_chars(e_mnemonic, "d2l");  break;
			case 0x90 : cat_string_wo_append_chars(e_mnemonic, "d2f");  break;
			case 0x91 : cat_string_wo_append_chars(e_mnemonic, "i2b");  break;
			case 0x92 : cat_string_wo_append_chars(e_mnemonic, "i2c");  break;
			case 0x93 : cat_string_wo_append_chars(e_mnemonic, "i2s");  break;
			case 0x94 : cat_string_wo_append_chars(e_mnemonic, "lcmp");  break;
			case 0x95 : cat_string_wo_append_chars(e_mnemonic, "fcmpl");  break;
			case 0x96 : cat_string_wo_append_chars(e_mnemonic, "fcmpg");  break;
			case 0x97 : cat_string_wo_append_chars(e_mnemonic, "dcmpl");  break;
			case 0x98 : cat_string_wo_append_chars(e_mnemonic, "dcmpg");  break;


			case 0x99 :
			case 0x9a :
			case 0x9b :
			case 0x9c :
			case 0x9d :
			case 0x9e :
			case 0x9f :
			case 0xa0 :
			case 0xa1 :
			case 0xa2 :
			case 0xa3 :
			case 0xa4 :
			case 0xa5 :
			case 0xa6 :
			case 0xa7 :

			case 0xc6 :
			case 0xc7 :
			{

				switch(((int) cat_string_wo_char_at(priv->e_bytecode, idx-1)) & 0xFF) {
					case 0x99 : cat_string_wo_append_chars(e_mnemonic, "ifeq\t"); break;
					case 0x9a : cat_string_wo_append_chars(e_mnemonic, "ifne\t"); break;
					case 0x9b : cat_string_wo_append_chars(e_mnemonic, "iflt\t"); break;
					case 0x9c : cat_string_wo_append_chars(e_mnemonic, "ifge\t"); break;
					case 0x9d : cat_string_wo_append_chars(e_mnemonic, "ifgt\t"); break;
					case 0x9e : cat_string_wo_append_chars(e_mnemonic, "ifle\t"); break;
					case 0x9f : cat_string_wo_append_chars(e_mnemonic, "if_icmpeq\t"); break;
					case 0xa0 : cat_string_wo_append_chars(e_mnemonic, "if_icmpne\t"); break;
					case 0xa1 : cat_string_wo_append_chars(e_mnemonic, "if_icmplt\t"); break;
					case 0xa2 : cat_string_wo_append_chars(e_mnemonic, "if_icmpge\t"); break;
					case 0xa3 : cat_string_wo_append_chars(e_mnemonic, "if_icmpgt\t"); break;
					case 0xa4 : cat_string_wo_append_chars(e_mnemonic, "if_icmple\t"); break;
					case 0xa5 : cat_string_wo_append_chars(e_mnemonic, "if_acmpeq\t"); break;
					case 0xa6 : cat_string_wo_append_chars(e_mnemonic, "if_acmpne\t"); break;
					case 0xa7 : cat_string_wo_append_chars(e_mnemonic, "goto\t"); break;
					case 0xa8 : cat_string_wo_append_chars(e_mnemonic, "jsr\t"); break;

					case 0xc6 : cat_string_wo_append_chars(e_mnemonic, "ifnull\t"); break;
					case 0xc7 : cat_string_wo_append_chars(e_mnemonic, "ifnonnull\t"); break;
				}
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					if (index&0x8000) {
						index = index+~0xFFFF;
					}
					int offset = idx-3+index;
					jag_hex8(e_mnemonic, offset);
					cat_string_wo_append_chars(e_mnemonic, "  // ");
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;


			case 0xa9 : {
				cat_string_wo_append_chars(e_mnemonic, "ret\t");
				if (idx<count) {
					int index = cat_string_wo_char_at(priv->e_bytecode, idx++) & 0xFF;
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;



			case 0xaa : {
				cat_string_wo_append_chars(e_mnemonic, "tableswitch\t");
				// TODO
			} break;

			case 0xab : {
				cat_string_wo_append_chars(e_mnemonic, "lookupswitch\t");
				while(idx%4!=0) {
					idx++;
				}
				if (idx+7<count) {
					int defbyte1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int defbyte2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int defbyte3 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int defbyte4 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int defbyte = (defbyte1 & 0xFF);
					defbyte = (defbyte<<8) + (defbyte2&0xFF);
					defbyte = (defbyte<<8) + (defbyte3&0xFF);
					defbyte = (defbyte<<8) + (defbyte4&0xFF);

					int npairs1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int npairs2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int npairs3 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int npairs4 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int npairs = (npairs1 & 0xFF);
					npairs = (npairs<<8) + (npairs2&0xFF);
					npairs = (npairs<<8) + (npairs3&0xFF);
					npairs = (npairs<<8) + (npairs4&0xFF);

					cat_string_wo_append_decimal(e_mnemonic, defbyte);
					cat_string_wo_append_chars(e_mnemonic, ", ");
					cat_string_wo_append_decimal(e_mnemonic, npairs);

					cat_string_wo_append_chars(e_mnemonic, ", {");

					while(npairs>0) {
						npairs--;

						if (idx+7<count) {
							int match1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int match2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int match3 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int match4 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int match = (match1 & 0xFF);
							match = (match<<8) + (match2&0xFF);
							match = (match<<8) + (match3&0xFF);
							match = (match<<8) + (match4&0xFF);

							int offset1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int offset2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int offset3 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int offset4 = cat_string_wo_char_at(priv->e_bytecode, idx++);
							int offset = (offset1 & 0xFF);
							offset = (offset<<8) + (offset2&0xFF);
							offset = (offset<<8) + (offset3&0xFF);
							offset = (offset<<8) + (offset4&0xFF);

							cat_string_wo_append_decimal(e_mnemonic, match);
							cat_string_wo_append_chars(e_mnemonic, "=>");
							jag_hex8(e_mnemonic, offset);
						} else {
							cat_string_wo_append_chars(e_mnemonic, "<error>");
						}
						if (npairs!=0) {
							cat_string_wo_append_chars(e_mnemonic, ", ");
						}
					}
					cat_string_wo_append_chars(e_mnemonic, ", }");
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xac : cat_string_wo_append_chars(e_mnemonic, "ireturn"); break;
			case 0xad : cat_string_wo_append_chars(e_mnemonic, "lreturn"); break;
			case 0xae : cat_string_wo_append_chars(e_mnemonic, "freturn"); break;
			case 0xaf : cat_string_wo_append_chars(e_mnemonic, "dreturn"); break;
			case 0xb0 : cat_string_wo_append_chars(e_mnemonic, "areturn"); break;
			case 0xb1 : cat_string_wo_append_chars(e_mnemonic, "return"); break;

			case 0xb2 : {
				cat_string_wo_append_chars(e_mnemonic, "getstatic\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xb3 : {
				cat_string_wo_append_chars(e_mnemonic, "putstatic\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xb4 : {
				cat_string_wo_append_chars(e_mnemonic, "getfield\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xb5 : {
				cat_string_wo_append_chars(e_mnemonic, "putfield\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xb6 : {
				cat_string_wo_append_chars(e_mnemonic, "invokevirtual\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xb7 : {
				cat_string_wo_append_chars(e_mnemonic, "invokespecial\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xb8 : {
				cat_string_wo_append_chars(e_mnemonic, "invokestatic\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xb9 : {
				cat_string_wo_append_chars(e_mnemonic, "invokeinterface\t");
				if (idx+3<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					int count = cat_string_wo_char_at(priv->e_bytecode, idx++);
					cat_string_wo_append_decimal(e_mnemonic, index);
					cat_string_wo_append_chars(e_mnemonic, ", ");
					cat_string_wo_append_decimal(e_mnemonic, count &0xFF);
					idx++;
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xba : {
				cat_string_wo_append_chars(e_mnemonic, "invokedynamic\t");
				if (idx+3<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
					idx+=2;
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xbb : {
				cat_string_wo_append_chars(e_mnemonic, "new\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;

			case 0xbc : {
				cat_string_wo_append_chars(e_mnemonic, "newarray\t");
				if (idx<count) {
					int toa = cat_string_wo_char_at(priv->e_bytecode, idx++);
					switch(toa) {
						case 4 : cat_string_wo_append_chars(e_mnemonic, "boolean"); break;
						case 5 : cat_string_wo_append_chars(e_mnemonic, "char"); break;
						case 6 : cat_string_wo_append_chars(e_mnemonic, "float"); break;
						case 7 : cat_string_wo_append_chars(e_mnemonic, "double"); break;
						case 8 : cat_string_wo_append_chars(e_mnemonic, "byte"); break;
						case 9 : cat_string_wo_append_chars(e_mnemonic, "short"); break;
						case 10 : cat_string_wo_append_chars(e_mnemonic, "int"); break;
						case 11 : cat_string_wo_append_chars(e_mnemonic, "long"); break;
						default : cat_string_wo_append_chars(e_mnemonic, "<error>"); break;
					}
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;

			case 0xbd : {
				cat_string_wo_append_chars(e_mnemonic, "anewarray\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xbe : cat_string_wo_append_chars(e_mnemonic, "arraylength"); break;
			case 0xbf : cat_string_wo_append_chars(e_mnemonic, "athrow"); break;
			case 0xc0 : {
				cat_string_wo_append_chars(e_mnemonic, "checkcast\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xc1 : {
				cat_string_wo_append_chars(e_mnemonic, "instanceof\t");
				if (idx+1<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					cat_string_wo_append_decimal(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xc2 : cat_string_wo_append_chars(e_mnemonic, "monitorenter"); break;
			case 0xc3 : cat_string_wo_append_chars(e_mnemonic, "monitorexit"); break;
			case 0xc4 : {
				cat_string_wo_append_chars(e_mnemonic, "wide"); // TODO
			} break;
			case 0xc5 : cat_string_wo_append_chars(e_mnemonic, "multieanewarray"); {
				if (idx+2<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					int dimensions = cat_string_wo_char_at(priv->e_bytecode, idx++) & 0xFF;
					cat_string_wo_append_decimal(e_mnemonic, index);
					cat_string_wo_append_chars(e_mnemonic, ", ");
					cat_string_wo_append_decimal(e_mnemonic, dimensions);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;
			case 0xc8 :
			case 0xc9 : {
				switch(((int) cat_string_wo_char_at(priv->e_bytecode, idx-1)) & 0xFF) {
					case 0xc8 : cat_string_wo_append_chars(e_mnemonic, "jsr_w\t"); break;
					case 0xc9 : cat_string_wo_append_chars(e_mnemonic, "goto_w\t"); break;
				}

				if (idx+3<count) {
					int index1 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index2 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index3 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index4 = cat_string_wo_char_at(priv->e_bytecode, idx++);
					int index = ((index1 & 0xFF)<<8) | (index2 & 0xFF);
					index = (index<<8) + (index3&0xFF);
					index = (index<<8) + (index4&0xFF);

					jag_hex8(e_mnemonic, index);
				} else {
					cat_string_wo_append_chars(e_mnemonic, "<error>");
				}
			} break;

		}


		jag_hex8(e_source, start_idx-8);
		cat_string_wo_append_chars(e_source, "  ");

		int p;
		for(p=0; p<5; p++) {
			if (start_idx+p<idx) {
				int b = cat_string_wo_char_at(priv->e_bytecode, start_idx+p);
				cat_string_wo_append_char(e_source, jag_txt_hex[(b>>4) & 0xf]);
				cat_string_wo_append_char(e_source, jag_txt_hex[b & 0xf]);
				cat_string_wo_append_char(e_source, ' ');
			} else {
				cat_string_wo_append_chars(e_source, "   ");
			}
		}

		cat_string_wo_append(e_source, e_mnemonic);

		cat_string_wo_append_chars(e_source, "\n");
	}

}








