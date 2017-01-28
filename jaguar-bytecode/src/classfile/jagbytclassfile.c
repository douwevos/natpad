/*
   File:    jagbytclassfile.c
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

#include "jagbytclassfile.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytClassfile"
#include <logging/catlog.h>

struct _JagBytClassfilePrivate {
	uint32_t magicNr;
	uint16_t minorVersion;
	uint16_t majorVersion;
	JagBytConstantPool *constant_pool;
	uint16_t accessFlags;
	JagAstModifiers *modifiers;
	int thisClass;
	JagBytName *main_type_name;
	int superClass;
	JagBytName *super_type_name;
	int interfaceCount;
	uint16_t *interfaces;
	JagBytInterfaceList *interface_list;
	JagBytFieldList *fieldList;
	JagBytMethodList *methodList;
	JagBytAttributeMap *attribute_map;
};

G_DEFINE_TYPE (JagBytClassfile, jag_byt_classfile, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_classfile_class_init(JagBytClassfileClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytClassfilePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_classfile_init(JagBytClassfile *instance) {
	JagBytClassfilePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_CLASSFILE, JagBytClassfilePrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytClassfile *instance = JAG_BYT_CLASSFILE(object);
	JagBytClassfilePrivate *priv = instance->priv;
	cat_unref_ptr(priv->main_type_name);
	cat_unref_ptr(priv->super_type_name);
	cat_unref_ptr(priv->modifiers);
	cat_unref_ptr(priv->constant_pool);
	cat_free_ptr(priv->interfaces);
	cat_unref_ptr(priv->interface_list);
	cat_unref_ptr(priv->fieldList);
	cat_unref_ptr(priv->methodList);
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

JagBytClassfile *jag_byt_classfile_new(uint32_t magicNr, uint16_t minorVersion, uint16_t majorVersion,
			JagBytConstantPool *constant_pool, uint16_t accessFlags, int thisClass, int superClass,
			int interfaceCount, uint16_t *interfaces, JagBytInterfaceList *interface_list, JagBytFieldList *fieldList, JagBytMethodList *methodList,
			JagBytAttributeMap *attribute_map) {
	JagBytClassfile *result = g_object_new(JAG_BYT_TYPE_CLASSFILE, NULL);
	cat_ref_anounce(result);
	JagBytClassfilePrivate *priv = result->priv;

	priv->magicNr = magicNr;
	priv->minorVersion = minorVersion;
	priv->majorVersion = majorVersion;
	priv->constant_pool = cat_ref_sink_ptr(constant_pool);
	priv->accessFlags = accessFlags;
	priv->modifiers = NULL;
	priv->thisClass = thisClass;
	priv->main_type_name = NULL;
	priv->superClass = superClass;
	priv->super_type_name = NULL;
	priv->interfaceCount = interfaceCount;
	priv->interfaces = interfaces;
	priv->interface_list = cat_ref_ptr(interface_list);
	priv->fieldList = cat_ref_sink_ptr(fieldList);
	priv->methodList = cat_ref_sink_ptr(methodList);
	priv->attribute_map = cat_ref_ptr(attribute_map);
	return result;
}

JagAstModifiers *jag_byt_classfile_get_modifiers(JagBytClassfile *classfile) {
	JagBytClassfilePrivate *priv = JAG_BYT_CLASSFILE_GET_PRIVATE(classfile);
	if (priv->modifiers==NULL) {
		priv->modifiers = jag_ast_modifiers_new(priv->accessFlags & 0x411);
	}
	return priv->modifiers;
}


JagBytFieldList *jag_byt_classfile_get_field_list(JagBytClassfile *raw_classfile) {
	return raw_classfile->priv->fieldList;
}

JagBytInterfaceList *jag_byt_classfile_get_interface_list(JagBytClassfile *classfile) {
	JagBytClassfilePrivate *priv = JAG_BYT_CLASSFILE_GET_PRIVATE(classfile);
	return priv->interface_list;
}

JagBytMethodList *jag_byt_classfile_get_method_list(JagBytClassfile *classfile) {
	JagBytClassfilePrivate *priv = JAG_BYT_CLASSFILE_GET_PRIVATE(classfile);
	return priv->methodList;
}

JagBytConstantPool *jag_byt_classfile_get_constant_pool(JagBytClassfile *classfile) {
	JagBytClassfilePrivate *priv = JAG_BYT_CLASSFILE_GET_PRIVATE(classfile);
	return priv->constant_pool;
}


JagBytName *jag_byt_classfile_get_main_type_name(JagBytClassfile *classfile) {
	JagBytClassfilePrivate *priv = JAG_BYT_CLASSFILE_GET_PRIVATE(classfile);
	if (priv->main_type_name == NULL) {
		JagBytConstantClazz *constant_clazz = (JagBytConstantClazz *) jag_byt_constant_pool_get_resolved(priv->constant_pool, priv->thisClass-1);
		JagBytName *main_type_name = jag_byt_constant_clazz_get_refrence_type_name(constant_clazz);
		cat_log_debug("this-class-index=%d, constant_clazz=%p, main_type_name=%p", priv->thisClass, constant_clazz, main_type_name);
		cat_log_trace("main_type_name=%s", cat_string_wo_getchars(jag_byt_name_create_fqn(main_type_name)));
		priv->main_type_name = cat_ref_ptr(main_type_name);
	}
	return priv->main_type_name;
}



JagBytName *jag_byt_classfile_get_super_type_name(JagBytClassfile *classfile) {
	JagBytClassfilePrivate *priv = JAG_BYT_CLASSFILE_GET_PRIVATE(classfile);
	if (priv->super_type_name == NULL) {
		JagBytConstantClazz *constant_clazz = (JagBytConstantClazz *) jag_byt_constant_pool_get_resolved(priv->constant_pool, priv->superClass-1);
		JagBytName *super_type_name = jag_byt_constant_clazz_get_refrence_type_name(constant_clazz);
		priv->super_type_name = cat_ref_ptr(super_type_name);
	}
	return priv->super_type_name;
}



CatStringWo *jag_byt_classfile_get_fq_classname(JagBytClassfile *raw_classfile) {
	JagBytClassfilePrivate *priv = raw_classfile->priv;

	JagBytIConstant *constant = jag_byt_constant_pool_get_at(priv->constant_pool, priv->thisClass-1);
	if (JAG_BYT_IS_CONSTANT_CLAZZ(constant)) {
		JagBytConstantClazz *constant_clazz = JAG_BYT_CONSTANT_CLAZZ(constant);

		int name_index = jag_byt_constant_clazz_get_name_index(constant_clazz);
		constant = jag_byt_constant_pool_get_at(priv->constant_pool, name_index-1);
		if (JAG_BYT_IS_CONSTANT_UTF8_TEXT(constant)) {
			JagBytConstantUtf8Text *utf8text = JAG_BYT_CONSTANT_UTF8_TEXT(constant);
			CatStringWo *a_raw_name = jag_byt_constant_utf8_text_get_value(utf8text);
			CatStringWo *result = cat_string_wo_clone(a_raw_name, CAT_CLONE_DEPTH_MAIN);
			cat_string_wo_replace_all_char(result, '/', '.');
			return cat_string_wo_anchor(result, 0);
		} else {
			cat_log_error("invalid utf-text for 'this' for this-index:%d, name_index=%d", priv->thisClass, name_index);
		}
	} else {
		cat_log_error("invalid 'this' index for class-file:%d", priv->thisClass);
	}
	return NULL;
}




CatStringWo *jag_byt_classfile_get_fq_super_classname(JagBytClassfile *raw_classfile) {
	JagBytClassfilePrivate *priv = raw_classfile->priv;


	JagBytIConstant *constant = jag_byt_constant_pool_get_at(priv->constant_pool, priv->superClass-1);
	if (JAG_BYT_IS_CONSTANT_CLAZZ(constant)) {
		JagBytConstantClazz *constant_clazz = JAG_BYT_CONSTANT_CLAZZ(constant);

		int name_index = jag_byt_constant_clazz_get_name_index(constant_clazz);
		constant = jag_byt_constant_pool_get_at(priv->constant_pool, name_index-1);
		if (JAG_BYT_IS_CONSTANT_UTF8_TEXT(constant)) {
			JagBytConstantUtf8Text *utf8text = JAG_BYT_CONSTANT_UTF8_TEXT(constant);
			CatStringWo *a_raw_name = jag_byt_constant_utf8_text_get_value(utf8text);
			CatStringWo *result = cat_string_wo_clone(a_raw_name, CAT_CLONE_DEPTH_MAIN);
			cat_string_wo_replace_all_char(result, '/', '.');
			return cat_string_wo_anchor(result, 0);
		} else {
			cat_log_error("invalid utf-text for 'super' for super-index:%d, name_index=%d", priv->superClass, name_index);
		}
//	} else {
//		cat_log_error("invalid 'this' index for class-file:%d", priv->thisClass);
	}
	return NULL;
}
