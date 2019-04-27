/*
   File:    jagbytecodereader.c
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

#include "jagbytecodereader.h"
#include "jagjobjectinputstream.h"
#include "classfile/constant/jagbytconstantpool.h"
#include "classfile/jagbytfieldlist.h"
#include "classfile/jagbytfield.h"
#include "classfile/jagbytmethodlist.h"
#include "classfile/jagbytmethod.h"
#include "classfile/jagbytclassfile.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytecodeReader"
#include <logging/catlog.h>

#define MAX_ATTRIBUTE_BLOCK_SIZE     0x200000

struct _JagBytecodeReaderPrivate {
	JagJObjectInputStream *ostream;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytecodeReader, jag_bytecode_reader, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_bytecode_reader_class_init(JagBytecodeReaderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_bytecode_reader_init(JagBytecodeReader *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytecodeReader *instance = JAG_BYTECODE_READER(object);
	JagBytecodeReaderPrivate *priv = jag_bytecode_reader_get_instance_private(instance);
	cat_unref_ptr(priv->ostream);
	G_OBJECT_CLASS(jag_bytecode_reader_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_bytecode_reader_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytecodeReader *jag_bytecode_reader_new(CatIInputStream *bytecode_stream) {
	JagBytecodeReader *result = g_object_new(JAG_TYPE_BYTECODE_READER, NULL);
	cat_ref_anounce(result);
	JagBytecodeReaderPrivate *priv = jag_bytecode_reader_get_instance_private(result);
	priv->ostream = jag_jobject_input_stream_new(bytecode_stream);
	return result;
}


static JagBytConstantPool *l_read_constant_pool(JagBytecodeReader *reader, int16_t pool_count) {
	JagBytecodeReaderPrivate *priv = jag_bytecode_reader_get_instance_private(reader);
	int real_count = ((int) pool_count) & 0xFFFF;
	cat_log_debug("real_count=%d", real_count);
	CatArrayWo *e_pool = cat_array_wo_new();

	int idx;
	for(idx = 1; idx < real_count; idx++) {
		int8_t type = jag_jobject_input_stream_read_byte(priv->ostream);
		cat_log_debug("cp[%d].type=%d", idx, type);
		switch (type) {
			case JAG_BYT_CONSTANT_UTF_TEXT_ID : {
				int16_t length = jag_jobject_input_stream_read_short(priv->ostream);
				CatStringWo *a_utfstring = jag_jobject_input_stream_read_string(priv->ostream, ((int) length) & 0xFFFF);
				GObject *cval = (GObject *) jag_byt_constant_utf8_text_new(a_utfstring);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_INT_ID : {
				int32_t value = jag_jobject_input_stream_read_int(priv->ostream);
				GObject *cval = (GObject *) jag_byt_constant_int_new(value);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_FLOAT_ID : {
				int32_t value = jag_jobject_input_stream_read_int(priv->ostream);
				GObject *cval = (GObject *) jag_byt_constant_float_new(value);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_LONG_ID : {
				int64_t value = jag_jobject_input_stream_read_long(priv->ostream);
				GObject *cval = (GObject *) jag_byt_constant_long_new(value);
				cat_array_wo_append(e_pool, cval);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
				idx++;
			} break;
			case JAG_BYT_CONSTANT_DOUBLE_ID : {
				int64_t value = jag_jobject_input_stream_read_long(priv->ostream);
				GObject *cval = (GObject *) jag_byt_constant_double_new(value);
				cat_array_wo_append(e_pool, cval);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
				idx++;
			} break;
			case JAG_BYT_CONSTANT_CLAZZ_ID : {
				int16_t index = jag_jobject_input_stream_read_short(priv->ostream);
				GObject *cval = (GObject *) jag_byt_constant_clazz_new(((int) index) & 0xFFFF);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_STRING_ID: {
				int16_t index = jag_jobject_input_stream_read_short(priv->ostream);
				GObject *cval = (GObject *) jag_byt_constant_string_new(((int) index) & 0xFFFF);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_FIELDREF_ID: {
				int class_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				int name_and_type_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				GObject *cval = (GObject *) jag_byt_constant_fieldref_new(class_index, name_and_type_index);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_METHODREF_ID: {
				int class_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				int name_and_type_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				GObject *cval = (GObject *) jag_byt_constant_methodref_new(class_index, name_and_type_index);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_INTERFACE_METHODREF_ID: {
				int class_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				int name_and_type_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				GObject *cval = (GObject *) jag_byt_constant_interface_methodref_new(class_index, name_and_type_index);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
			} break;
			case JAG_BYT_CONSTANT_NAME_AND_TP_ID: {
				int name_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				cat_log_debug("name_index=%d", name_index);
				int descriptor_index = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
				cat_log_debug("descriptor_index=%d", descriptor_index);
				GObject *cval = (GObject *) jag_byt_constant_name_and_tp_new(name_index, descriptor_index);
				cat_array_wo_append(e_pool, cval);
				cat_unref_ptr(cval);
//				cat_array_wo_append(e_pool, (GObject *) jag_byt_constant_name_and_type_new(name_index, descriptor_index));
				cat_log_debug("added");
			} break;
			default : {
				cat_log_error("unknown constant-pool tag %d", type);
			} break;
		}
	}

	cat_log_debug("done");

	CatArrayWo *a_new_pool = cat_array_wo_anchor(e_pool, 0);
	return jag_byt_constant_pool_new(a_new_pool);
}


static JagBytFieldList *l_read_field_list(JagBytecodeReader *reader, JagBytIConstantProvider *constant_provider, int fieldCount) {
	JagBytecodeReaderPrivate *priv = jag_bytecode_reader_get_instance_private(reader);
	CatArrayWo *e_fields = cat_array_wo_new();
	cat_log_debug("fieldCount:%d", fieldCount);

	int idx;
	for(idx=0; idx<fieldCount; idx++) {
		uint16_t accessFlags = jag_jobject_input_stream_read_short(priv->ostream);
		uint16_t nameIndex = jag_jobject_input_stream_read_short(priv->ostream);
		uint16_t descriptorIndex = jag_jobject_input_stream_read_short(priv->ostream);
		uint16_t attrCount = jag_jobject_input_stream_read_short(priv->ostream);
		JagBytAttributeMap *attribute_map = jag_byt_attribute_map_new(priv->ostream, constant_provider, ((int) attrCount) & 0xFFFF);
		JagBytField *field = jag_byt_field_new(constant_provider, accessFlags, nameIndex, descriptorIndex, attribute_map);
		cat_unref_ptr(attribute_map);
		cat_array_wo_append(e_fields, (GObject *) field);
		cat_unref_ptr(field);
	}

	return jag_byt_field_list_new(cat_array_wo_anchor(e_fields, 0));
}

static JagBytMethodList *l_read_method_list(JagBytecodeReader *reader, JagBytName *main_type_name, JagBytIConstantProvider *constant_provider, int methodCount) {
	JagBytecodeReaderPrivate *priv = jag_bytecode_reader_get_instance_private(reader);
	CatArrayWo *e_methods = cat_array_wo_new();

	cat_log_debug("methodCount:%d", methodCount);

	int idx;
	for(idx=0; idx<methodCount; idx++) {
		uint16_t accessFlags = jag_jobject_input_stream_read_short(priv->ostream);
		uint16_t nameIndex = jag_jobject_input_stream_read_short(priv->ostream);
		uint16_t descriptorIndex = jag_jobject_input_stream_read_short(priv->ostream);
		uint16_t attrCount = jag_jobject_input_stream_read_short(priv->ostream);
		JagBytAttributeMap *attribute_map = jag_byt_attribute_map_new(priv->ostream, constant_provider, ((int) attrCount) & 0xFFFF);
		JagBytMethod *method = jag_byt_method_new(main_type_name, constant_provider, accessFlags, nameIndex, descriptorIndex, attribute_map);
		cat_unref_ptr(attribute_map);
		cat_array_wo_append(e_methods, (GObject *) method);
		cat_unref_ptr(method);
	}

	return jag_byt_method_list_new(cat_array_wo_anchor(e_methods, 0));
}

JagBytClassfile *jag_bytecode_reader_parse_bytecode(JagBytecodeReader *reader) {
	JagBytecodeReaderPrivate *priv = jag_bytecode_reader_get_instance_private(reader);
	uint32_t magicNr = jag_jobject_input_stream_read_int(priv->ostream);
	uint16_t minorVersion = jag_jobject_input_stream_read_short(priv->ostream);
	uint16_t majorVersion = jag_jobject_input_stream_read_short(priv->ostream);
	uint16_t constantPoolCount = jag_jobject_input_stream_read_short(priv->ostream);

	JagBytConstantPool *constant_pool = l_read_constant_pool(reader, constantPoolCount);
	cat_log_trace("constant-pool:%p", constant_pool);


	uint16_t accessFlags = jag_jobject_input_stream_read_short(priv->ostream);
	int thisClass = jag_jobject_input_stream_read_short(priv->ostream) & 0xFFFF;
	int superClass = jag_jobject_input_stream_read_short(priv->ostream) & 0xFFFF;
	int interfaceCount = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;

	jag_byt_constant_pool_set_this_class_index(constant_pool, thisClass);

	cat_log_trace("interfaceCount:%d", interfaceCount);
	uint16_t *interfaces = g_malloc(sizeof(uint16_t)*interfaceCount);
	JagBytInterfaceList *interface_list = jag_byt_interface_list_new();
	int idx;
	for(idx=0; idx < interfaceCount; idx++) {
		interfaces[idx] = jag_jobject_input_stream_read_short(priv->ostream);
		JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, ((int) interfaces[idx]) & 0xFFFF);
		if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
			JagBytConstantClazz *constant_clazz = JAG_BYT_CONSTANT_CLAZZ(iconstant);
			JagAstDeclarationType *iface_type = jag_byt_constant_clazz_get_parsed_descriptor(constant_clazz);
			jag_byt_interface_list_add(interface_list, iface_type);
		}
	}

	uint16_t fieldCount = ((int) jag_jobject_input_stream_read_short(priv->ostream)) & 0xFFFF;
	JagBytFieldList *fieldList = l_read_field_list(reader, JAG_BYT_ICONSTANT_PROVIDER(constant_pool), fieldCount);

	uint16_t methodCount = jag_jobject_input_stream_read_short(priv->ostream);

	JagBytConstantClazz *constant_clazz = (JagBytConstantClazz *) jag_byt_constant_pool_get_resolved(constant_pool, thisClass-1);
	JagBytName *main_type_name = jag_byt_constant_clazz_get_refrence_type_name(constant_clazz);
	JagBytMethodList *methodList = l_read_method_list(reader, main_type_name, JAG_BYT_ICONSTANT_PROVIDER(constant_pool), methodCount);

	uint16_t attributeCount = jag_jobject_input_stream_read_short(priv->ostream);
	JagBytAttributeMap *attribute_map =jag_byt_attribute_map_new(priv->ostream, JAG_BYT_ICONSTANT_PROVIDER(constant_pool), ((int) attributeCount) & 0xFFFF);

	JagBytClassfile *result = jag_byt_classfile_new(magicNr, minorVersion, majorVersion, constant_pool, accessFlags, thisClass,
            superClass, interfaceCount, interfaces, interface_list, fieldList, methodList, attribute_map);
	cat_unref_ptr(interface_list);
	cat_unref_ptr(attribute_map);

	return result;
}
