/*
   File:    jagdecompiler.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 26, 2012
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

#include "jagdecompiler.h"
#include "type/jagdecimportrepository.h"
#include "vm/jagdecmethodcontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecompiler"
#include <logging/catlog.h>

struct _JagDecompilerPrivate {
	JagBytClassfile *raw_classfile;
	JagDecImportRepository *import_repository;
};

G_DEFINE_TYPE (JagDecompiler, jag_decompiler, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_decompiler_class_init(JagDecompilerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecompilerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

}




static void jag_decompiler_init(JagDecompiler *instance) {
	JagDecompilerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_DECOMPILER, JagDecompilerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecompiler *instance = JAG_DECOMPILER(object);
	JagDecompilerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->raw_classfile);
	cat_unref_ptr(priv->import_repository);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecompiler *jag_decompiler_new(JagBytClassfile *classfile) {
	JagDecompiler *result = g_object_new(JAG_TYPE_DECOMPILER, NULL);
	cat_ref_anounce(result);
	JagDecompilerPrivate *priv = result->priv;
	priv->raw_classfile = cat_ref_ptr(classfile);
	JagBytName *main_type_name = jag_byt_classfile_get_main_type_name(classfile);
	priv->import_repository = jag_dec_import_repository_new(main_type_name);
	return result;
}


JagAstFieldDeclaration *l_field_create_final(JagDecompiler *decompiler, JagBytField *field) {
	JagDecompilerPrivate *priv = JAG_DECOMPILER_GET_PRIVATE(decompiler);
	JagAstDeclarationType *field_descriptor = jag_byt_field_get_declaration_type(field);
	field_descriptor = jag_dec_import_repository_create_final_declaration_type(priv->import_repository, field_descriptor);
	JagAstModifiers *field_modifiers = jag_ast_modifiers_new(jag_byt_field_get_access_flags(field));
	JagBytIConstant *constant = jag_byt_field_get_constant_value(field);
	JagAstIExpression *initial_value = NULL;
//	if (JAG_BYT_IS_CONSTANT_DOUBLE(constant)) {	// TODO implement
//	} else
	if (JAG_BYT_IS_CONSTANT_STRING(constant)) {
		CatStringWo *a_value = jag_byt_constant_string_get_value((JagBytConstantString *) constant);
		initial_value = (JagAstIExpression *) jag_ast_quoted_string_new(a_value);
	} else if (JAG_BYT_IS_CONSTANT_UTF8_TEXT(constant)) {
		CatStringWo *a_value = jag_byt_constant_utf8_text_get_value((JagBytConstantUtf8Text *) constant);
		initial_value = (JagAstIExpression *) jag_ast_identifier_new(a_value);
	} else if (JAG_BYT_IS_CONSTANT_INT(constant)) {
		int value = jag_byt_constant_int_get_value((JagBytConstantInt *) constant);
		CatIStringable *valobj = NULL;
		if (((value==0) || (value==1))
				&& (jag_ast_declaration_type_get_primitive_type(field_descriptor)==JAG_BYT_TYPE_BOOLEAN) &&
				(jag_ast_declaration_type_get_dim_count(field_descriptor)==0)) {
			valobj = (CatIStringable *) cat_boolean_new(value!=0);
		} else {
			valobj = (CatIStringable *) cat_integer_new(value);
		}
		initial_value = (JagAstIExpression *) jag_ast_literal_new(valobj);
		cat_unref_ptr(valobj);
	} else if (JAG_BYT_IS_CONSTANT_FLOAT(constant)) {
		float value = jag_byt_constant_float_get_value((JagBytConstantFloat *) constant);
		CatFloat *valobj = cat_float_new(value);
		initial_value = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) valobj);
		cat_unref_ptr(valobj);
	} else if (JAG_BYT_IS_CONSTANT_LONG(constant)) {
		long value = jag_byt_constant_long_get_value((JagBytConstantLong *) constant);
		CatLong *valobj = cat_long_new(value);
		initial_value = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) valobj);
		cat_unref_ptr(valobj);
	} else if (JAG_BYT_IS_CONSTANT_CLAZZ(constant)) {
		JagBytName *ref_type_name = jag_byt_constant_clazz_get_refrence_type_name((JagBytConstantClazz *) constant);
		CatStringWo *a_fqn = jag_byt_name_create_fqn(ref_type_name);
		CatStringWo *fqn = cat_string_wo_clone(a_fqn, CAT_CLONE_DEPTH_MAIN);
		cat_string_wo_append_chars(fqn, ".class");
		initial_value = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) fqn);
	}

	JagAstIdentifier *field_name = jag_ast_identifier_new(jag_byt_field_get_name(field));

	JagAstFieldDeclaration *result = jag_ast_field_declaration_new(field_modifiers, field_descriptor, field_name, initial_value);
	cat_unref_ptr(field_modifiers);
	cat_unref_ptr(field_name);
	cat_unref_ptr(field_descriptor);
	cat_unref_ptr(initial_value);
	return result;
}

JagAstTp *jag_decompiler_create_type(JagDecompiler *decompiler) {
	JagDecompilerPrivate *priv = JAG_DECOMPILER_GET_PRIVATE(decompiler);
	JagAstPackageStatement *package_statement = NULL;
	JagBytName *package_name =jag_byt_classfile_get_main_type_name(priv->raw_classfile);
	package_name = jag_byt_name_create_package_name(package_name);
	if (package_name!=NULL) {
		JagAstIdentifier *id = jag_ast_identifier_new_from_name(package_name);
		package_statement = jag_ast_package_statement_new(id);
		cat_unref_ptr(id);
	}
	cat_unref_ptr(package_name);


	JagBytFieldList *field_list = jag_byt_classfile_get_field_list(priv->raw_classfile);
	int fieldCount = jag_byt_field_list_count(field_list);
	int fieldIdx;
	for(fieldIdx=0; fieldIdx<fieldCount; fieldIdx++) {
		JagBytField *parsed_field = jag_byt_field_list_get_at(field_list, fieldIdx);
		JagAstDeclarationType *field_declaration_type = jag_byt_field_get_declaration_type(parsed_field);
		jag_dec_import_repository_attach_declaration_type(priv->import_repository, field_declaration_type, TRUE);
	}

	JagBytInterfaceList *interface_list = jag_byt_classfile_get_interface_list(priv->raw_classfile);
	int ifaceIdx;
	for(ifaceIdx=0; ifaceIdx<jag_byt_interface_list_count(interface_list); ifaceIdx++) {
		JagAstDeclarationType *interface_declaration_type = jag_byt_interface_list_get(interface_list, ifaceIdx);
		jag_dec_import_repository_attach_declaration_type(priv->import_repository, interface_declaration_type, TRUE);
	}


	JagBytMethodList *method_list = jag_byt_classfile_get_method_list(priv->raw_classfile);



//	byte[] bytecode = parsedClass.getByteCode();	// TODO implement
//	if (bytecode!=null) {
//		MnemonicBlock opperationBlock = new MnemonicBlock(bytecode);
//		GroupConnector blocker = new GroupConnector(importRepository, parsedClass, null, opperationBlock, parsedClass.getTryCatchList());
//
//		SubCodeBlock subCodeBlock = blocker.connectContext.subCodeBlock;
//
//		ModelStageTwoRuntime modelConnector = new ModelStageTwoRuntime(subCodeBlock, importRepository, null); /* is there a linenumbertable for a class if so then TODO implement*/
//		modelConnector.connect();
//	}
//
//
//
	CatArrayWo *e_method_context_list = cat_array_wo_new();
	int methodIdx;
	for(methodIdx=0; methodIdx<jag_byt_method_list_count(method_list); methodIdx++) {
		JagBytMethod *method = jag_byt_method_list_get_at(method_list, methodIdx);
		JagDecMethodContext *method_context = jag_dec_method_context_new(priv->import_repository, priv->raw_classfile, method);
		cat_array_wo_append(e_method_context_list, (GObject *) method_context);
		jag_dec_import_repository_attach_method(priv->import_repository, method);
		cat_unref_ptr(method_context);
	}


	CatArrayWo *e_imports = jag_dec_import_repository_create_statements(priv->import_repository);


	CatArrayWo* e_cmethods = cat_array_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(e_method_context_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecMethodContext *method_context = (JagDecMethodContext *) cat_iiterator_next(iter);
		JagAstMethod *method = jag_dec_method_context_create_method(method_context);
		cat_array_wo_append(e_cmethods, (GObject *) method);
		cat_unref(method);
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(e_method_context_list);

//	int index = 0;
//	for(MethodContext context : methodContextList) {
//		cmethods[index++] = context.createMethod();
//	}
//
//
	CatArrayWo *e_fields = cat_array_wo_new();
	for(fieldIdx=0; fieldIdx<fieldCount; fieldIdx++) {
		JagBytField *parsed_field = jag_byt_field_list_get_at(field_list, fieldIdx);
		JagAstFieldDeclaration *ast_field = l_field_create_final(decompiler, parsed_field);
		cat_array_wo_append(e_fields, (GObject *) ast_field);
		cat_unref_ptr(ast_field);
	}

	CatArrayWo *e_interfaces = cat_array_wo_new();

//	Name interfaces[] = new Name[interfaceList.length];	// TODO implement
//	for(int ifaceIdx=0; ifaceIdx<interfaceList.length; ifaceIdx++) {
//		DeclarationType interfaceDeclType = interfaceList[ifaceIdx];
//		Name resolvedTypename = importRepository.createResolvedTypename(interfaceDeclType.referenceTypeName);
//		interfaces[ifaceIdx] = resolvedTypename;
//	}
//

	JagBytName *cf_name = jag_byt_classfile_get_main_type_name(priv->raw_classfile);
	JagAstModifiers *cf_modifiers = jag_byt_classfile_get_modifiers(priv->raw_classfile);
	JagBytName *cf_super = jag_byt_classfile_get_super_type_name(priv->raw_classfile);
	JagAstTpHeader *type_header = jag_ast_tp_header_new(cf_name, cf_modifiers, cf_super, e_interfaces);
	cat_unref_ptr(e_interfaces);

	JagAstTp *result = jag_ast_tp_new(package_statement, e_imports, type_header, e_fields, e_cmethods);
	cat_unref_ptr(package_statement);
	cat_unref_ptr(e_imports);
	cat_unref_ptr(type_header);
	cat_unref_ptr(e_fields);
	cat_unref_ptr(e_cmethods);
	return result;
}








