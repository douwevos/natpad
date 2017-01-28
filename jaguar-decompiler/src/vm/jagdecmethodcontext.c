/*
   File:    jagdecmethodcontext.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#include "jagdecmethodcontext.h"
#include "jagdecgroupconnector.h"
#include "jagdecmnemoniclinenumbertable.h"
#include "jagdecmodelstagetworuntime.h"
#include "intermediate/jagdecintermediateruncontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecMethodContext"
#include <logging/catlog.h>

struct _JagDecMethodContextPrivate {
	JagDecImportRepository *import_repository;
	JagBytClassfile *parsedClass;
	JagBytMethod *parsedMethod;

	JagDecMnemonicLineNumberTable *mnemonicLineNumberTable;
	JagDecIntermediateRunContext *connectContext;
};

G_DEFINE_TYPE (JagDecMethodContext, jag_dec_method_context, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_method_context_class_init(JagDecMethodContextClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecMethodContextPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_method_context_init(JagDecMethodContext *instance) {
	JagDecMethodContextPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_METHOD_CONTEXT, JagDecMethodContextPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecMethodContext *instance = JAG_DEC_METHOD_CONTEXT(object);
	JagDecMethodContextPrivate *priv = instance->priv;
	cat_unref_ptr(priv->import_repository);
	cat_unref_ptr(priv->parsedClass);
	cat_unref_ptr(priv->parsedMethod);
	cat_unref_ptr(priv->connectContext);
	cat_unref_ptr(priv->mnemonicLineNumberTable);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecMethodContext *jag_dec_method_context_new(JagDecImportRepository *import_repository, JagBytClassfile *parsedClass, JagBytMethod *parsedMethod) {
	JagDecMethodContext *result = g_object_new(JAG_DEC_TYPE_METHOD_CONTEXT, NULL);
	cat_ref_anounce(result);
	JagDecMethodContextPrivate *priv = result->priv;
	priv->import_repository = cat_ref_ptr(import_repository);
	priv->parsedClass = cat_ref_ptr(parsedClass);
	priv->parsedMethod = cat_ref_ptr(parsedMethod);

	JagBytMethodHeader *method_header = jag_byt_method_get_method_header(parsedMethod);

	cat_log_on_debug({
		CatStringWo *e_mh_as_text = jag_byt_method_header_as_text(method_header, NULL);
		cat_log_debug("%s", cat_string_wo_getchars(e_mh_as_text));
		cat_unref_ptr(e_mh_as_text);
	});
//	cat_log_debug(""+parsedMethod.methodHeader);

	CatStringWo *e_bytecode = jag_byt_method_get_bytecode(parsedMethod);
	JagBytMnemonicBlock *mnemonicBlock = jag_byt_mnemonic_block_new(e_bytecode);
//		mnemonicBlock.dumpCode(System.out);
	JagBytTryCatchList *try_catch_list = jag_byt_method_get_try_catch_list(parsedMethod);
	if (try_catch_list!=NULL) {
//			try_catch_list.dump(System.out);
	}

	JagDecGroupConnector *blocker = jag_dec_group_connector_new(import_repository, parsedClass, parsedMethod, mnemonicBlock, try_catch_list);
	priv->connectContext = cat_ref_ptr(jag_dec_group_connector_get_run_context(blocker));

	int frameIndex = 0;
	JagAstModifiers *mmods = jag_byt_method_get_modifiers(parsedMethod);
	if (!jag_ast_modifiers_is_static(mmods)) {

		JagBytName *super_type_name = jag_byt_classfile_get_super_type_name(parsedClass);
		if (super_type_name) {
			jag_dec_import_repository_add_name(import_repository, super_type_name);
		}
		jag_dec_intermediate_run_context_set_this(priv->connectContext, jag_byt_classfile_get_main_type_name(parsedClass), super_type_name);
		frameIndex++;
	}


	JagBytLineNumberTable *lineNumberTable = NULL;
	JagBytAttributeLineNumberTable *attr_line_num_tab = jag_byt_method_get_line_number_table(parsedMethod);
	if (attr_line_num_tab) {
		lineNumberTable = jag_byt_attribute_line_number_table_get_table(attr_line_num_tab);
	}
	priv->mnemonicLineNumberTable = jag_dec_mnemonic_line_number_table_new((JagBytIMnemonicBlock *) mnemonicBlock, lineNumberTable);
	cat_unref_ptr(mnemonicBlock);


	JagBytAttributeLocalVariableTable *attr_loc_var_tab = jag_byt_method_get_local_variable_table(parsedMethod);
	JagBytLocalVariableTable *localVariableTable = NULL;
	if (attr_loc_var_tab) {
		localVariableTable = jag_byt_attribute_local_variable_table_get_table(attr_loc_var_tab);
	}

	JagAstArgumentList *argument_list = jag_byt_method_header_get_argument_list(method_header);
	if (argument_list!=NULL && jag_ast_argument_list_count(argument_list)>0) {
		int arg_count = jag_ast_argument_list_count(argument_list);
		int arg_idx;
		for(arg_idx=0; arg_idx<arg_count; arg_idx++) {
			JagAstArgument *parsedField = jag_ast_argument_list_get_at(argument_list, arg_idx);
			CatStringWo *a_name = jag_ast_argument_get_name(parsedField);
			if (a_name==NULL) {
				CatStringWo *e_buf = cat_string_wo_new_with("arg");
				cat_string_wo_append_decimal(e_buf, frameIndex);
				a_name = cat_string_wo_anchor(e_buf, 0);
			} else {
				cat_ref_ptr(a_name);
			}
//				log.detail("creating argument frame-value:frameIndex="+frameIndex+", name="+name+" localVariableTable="+localVariableTable);



			if (localVariableTable!=NULL) {
				CatStringWo *a_tabName = jag_byt_local_variable_table_find_name(localVariableTable, frameIndex, 0);
				if (a_tabName!=NULL) {
					cat_ref_swap(a_name, a_tabName);
				}
			}


			JagAstIdentifier *fvid = jag_ast_identifier_new(a_name);
			cat_unref_ptr(a_name);
			JagDecMethodFrameValue *frameValue = jag_dec_method_frame_value_new(frameIndex, fvid);
			cat_unref_ptr(fvid);
			JagAstDeclarationType *argument_declaration_type = jag_ast_argument_get_declaration_type(parsedField);
			JagDecValueTypeDefinition *value_type_definition = jag_dec_value_type_definition_new(argument_declaration_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_ARGUMENT);
			jag_dec_value_type_definition_list_add_definition((JagDecValueTypeDefinitionList *) frameValue, value_type_definition);
			cat_unref_ptr(value_type_definition);
			jag_dec_intermediate_run_context_add_frame_value(priv->connectContext, frameIndex, frameValue);
			frameIndex++;
			if (jag_ast_declaration_type_is_category2(argument_declaration_type)) {
				jag_dec_intermediate_run_context_add_frame_value(priv->connectContext, frameIndex, frameValue);
				frameIndex++;
			}
			cat_unref_ptr(frameValue);
		}
	}

	JagDecIntermediateGroup *model_container = jag_dec_group_connector_get_model_container(blocker);
	jag_dec_iintermediate_node_connect_stage_one((JagDecIIntermediateNode *) model_container, priv->connectContext);
	jag_dec_intermediate_run_context_connect(priv->connectContext, priv->import_repository);
	cat_unref_ptr(blocker);
	return result;
}


JagAstMethod *jag_dec_method_context_create_method(JagDecMethodContext *method_context) {
	JagDecMethodContextPrivate *priv = JAG_DEC_METHOD_CONTEXT_GET_PRIVATE(method_context);
	JagDecSubCodeBlock *subCodeBlock = jag_dec_intermediate_run_context_get_sub_code_block(priv->connectContext);
	JagDecModelStageTwoRuntime *modelConnector = jag_dec_model_stage_two_runtime_new((JagDecISubCode *) subCodeBlock, priv->import_repository, priv->mnemonicLineNumberTable);
	JagAstBlock *block = jag_dec_model_stage_two_runtime_connect(modelConnector);
	JagAstMethodHeader *method_header = jag_dec_model_stage_two_runtime_create_final_method_header(modelConnector, priv->parsedMethod);
	JagAstModifiers *method_modifiers = jag_byt_method_get_modifiers(priv->parsedMethod);
	JagAstMethod *method = jag_ast_method_new(method_modifiers, method_header, block);
	if (priv->mnemonicLineNumberTable) {
		int lnr = jag_dec_mnemonic_line_number_table_get_least_line_number(priv->mnemonicLineNumberTable);
		if (lnr>0) {
			jag_ast_istatement_set_at_least_line_number((JagAstIStatement *) method, lnr-1);
		}
	}

	cat_unref_ptr(method_header);
	cat_unref_ptr(modelConnector);
	return method;
}


