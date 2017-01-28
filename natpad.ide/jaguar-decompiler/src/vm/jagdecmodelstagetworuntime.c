/*
   File:    jagdecmodelstagetworuntime.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#include "jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecModelStageTwoRuntime"
#include <logging/catlog.h>

struct _JagDecModelStageTwoRuntimePrivate {
	JagDecISubCode *sub_code_block;
	JagDecImportRepository *import_repository;
	JagDecMnemonicLineNumberTable *line_number_table;

	JagAstBlock *current_block;
	int level;
	int mnemonic_index;
	int statement_line_nr;
};

G_DEFINE_TYPE (JagDecModelStageTwoRuntime, jag_dec_model_stage_two_runtime, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_model_stage_two_runtime_class_init(JagDecModelStageTwoRuntimeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecModelStageTwoRuntimePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_model_stage_two_runtime_init(JagDecModelStageTwoRuntime *instance) {
	JagDecModelStageTwoRuntimePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME, JagDecModelStageTwoRuntimePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecModelStageTwoRuntime *instance = JAG_DEC_MODEL_STAGE_TWO_RUNTIME(object);
	JagDecModelStageTwoRuntimePrivate *priv = instance->priv;
	cat_unref_ptr(priv->current_block);
	cat_unref_ptr(priv->import_repository);
	cat_unref_ptr(priv->line_number_table);
	cat_unref_ptr(priv->sub_code_block);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecModelStageTwoRuntime *jag_dec_model_stage_two_runtime_new(JagDecISubCode *sub_code_block, JagDecImportRepository *import_repository, JagDecMnemonicLineNumberTable *line_number_table) {
	JagDecModelStageTwoRuntime *result = g_object_new(JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME, NULL);
	cat_ref_anounce(result);
	JagDecModelStageTwoRuntimePrivate *priv = result->priv;
	priv->sub_code_block = cat_ref_ptr(sub_code_block);
	priv->import_repository = cat_ref_ptr(import_repository);
	priv->line_number_table = cat_ref_ptr(line_number_table);
	priv->level = 0;
	priv->mnemonic_index = -1;
	priv->statement_line_nr = -1;
	priv->current_block = jag_ast_block_new();
	return result;
}


JagDecModelStageTwoRuntime *jag_dec_model_stage_two_runtime_create_branch(JagDecModelStageTwoRuntime *runtime, JagAstBlock *new_current_block) {
	JagDecModelStageTwoRuntimePrivate *srcpriv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	JagDecModelStageTwoRuntime *result = g_object_new(JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME, NULL);
	cat_ref_anounce(result);
	JagDecModelStageTwoRuntimePrivate *priv = result->priv;
	priv->sub_code_block = cat_ref_ptr(srcpriv->sub_code_block);
	priv->import_repository = cat_ref_ptr(srcpriv->import_repository);
	priv->line_number_table = cat_ref_ptr(srcpriv->line_number_table);
	priv->level = srcpriv->level+1;
	priv->mnemonic_index = srcpriv->mnemonic_index;
	priv->statement_line_nr = srcpriv->statement_line_nr;
	priv->current_block = cat_ref_ptr(new_current_block);
	return result;
}



JagDecImportRepository *jag_dec_model_stage_two_runtime_get_import_repository(JagDecModelStageTwoRuntime *runtime) {
	JagDecModelStageTwoRuntimePrivate *priv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	return priv->import_repository;
}


void jag_dec_model_stage_two_runtime_append_statement(JagDecModelStageTwoRuntime *runtime, JagAstIStatement *statement) {
	JagDecModelStageTwoRuntimePrivate *priv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	jag_ast_block_append_statement(priv->current_block, statement);
	cat_log_debug("runtime=%p - %d, appending statement:%s", runtime, priv->level , statement==NULL ? "null" : g_type_name_from_instance((GTypeInstance *) statement));
	if (priv->statement_line_nr!=-1) {
		jag_ast_istatement_set_at_least_line_number(statement, priv->statement_line_nr);
	}
}


void jag_dec_model_stage_two_runtime_set_mnemonic_index(JagDecModelStageTwoRuntime *runtime, int mnemonic_index) {
	JagDecModelStageTwoRuntimePrivate *priv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	priv->mnemonic_index = mnemonic_index;
	if (priv->line_number_table) {
		CatIIterator *iter = jag_byt_line_number_table_iterator((JagBytLineNumberTable *) priv->line_number_table);
		while(cat_iiterator_has_next(iter)) {
			JagBytLineNumberTableEntry *entry = (JagBytLineNumberTableEntry *) cat_iiterator_next(iter);
			int e_start_pc = jag_byt_line_number_table_entry_get_start_pc(entry);
			if (e_start_pc==mnemonic_index) {
				priv->statement_line_nr = jag_byt_line_number_table_entry_get_line_number(entry);
				break;
			}
		}
		cat_unref_ptr(iter);
	}
}


JagAstBlock *jag_dec_model_stage_two_runtime_connect(JagDecModelStageTwoRuntime *runtime) {
	JagDecModelStageTwoRuntimePrivate *priv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	jag_dec_isub_code_connect_stage_two(priv->sub_code_block, runtime);
	jag_ast_block_remove_last_return(priv->current_block);
	return priv->current_block;
}



JagAstFieldDeclaration *l_create_final(JagDecModelStageTwoRuntime *runtime, JagAstArgument *parsedField, CatStringWo *a_localVarName) {
	JagDecModelStageTwoRuntimePrivate *priv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	JagAstIExpression *fieldValue = NULL;
//		if (parsedField.valueExpression!=null) {
//			if (parsedField.valueExpression instanceof ConstantPrimitive) {
//				ConstantPrimitive<?> cprim = (ConstantPrimitive<?>) parsedField.valueExpression;
//				Object value = cprim.value;
//				fieldValue = new Literal(value);
//			}
//		}
	if (a_localVarName==NULL) {
		a_localVarName = jag_ast_argument_get_name(parsedField);
	}
		// TODO return typ should be ast.Argument
	JagAstModifiers *arg_modifiers = jag_ast_argument_get_modifiers(parsedField);  /// TODO implement
	JagAstDeclarationType *final_type = jag_dec_import_repository_create_final_declaration_type(priv->import_repository, jag_ast_argument_get_declaration_type(parsedField));
	JagAstIdentifier *variable_name = jag_ast_identifier_new(a_localVarName);
	JagAstFieldDeclaration *result = jag_ast_field_declaration_new(arg_modifiers, final_type, variable_name, fieldValue);
	cat_unref_ptr(final_type);
	cat_unref_ptr(variable_name);
	return result;
}


JagAstMethodHeader *jag_dec_model_stage_two_runtime_create_final_method_header(JagDecModelStageTwoRuntime *runtime, JagBytMethod *method) {
	JagDecModelStageTwoRuntimePrivate *priv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	JagBytMethodHeader *methodHeader = jag_byt_method_get_method_header(method);

	JagAstDeclarationType *finalDeclType = NULL;
	JagAstDeclarationType *method_return_type = jag_byt_method_header_get_return_type(methodHeader);
	if (method_return_type!=NULL) {
		finalDeclType = jag_dec_import_repository_create_final_declaration_type(priv->import_repository, method_return_type);
	}

	int frameIndex = 0;
	JagAstModifiers *mods = jag_byt_method_get_modifiers(method);
	if (!jag_ast_modifiers_is_static(mods)) {
		frameIndex++;
	}

	JagBytLocalVariableTable *localVariableTable = NULL;
	JagBytAttributeLocalVariableTable *attr_loc_var_tab = jag_byt_method_get_local_variable_table(method);
	if (attr_loc_var_tab!=NULL) {
		localVariableTable = jag_byt_attribute_local_variable_table_get_table(attr_loc_var_tab);
	}

	JagAstArgumentList *argument_list = jag_byt_method_header_get_argument_list(methodHeader);
	int argcount = jag_ast_argument_list_count(argument_list);
	CatArrayWo *e_arguments = cat_array_wo_new();
	int argIdx;
	for(argIdx=0; argIdx<argcount; argIdx++, frameIndex++) {
		JagAstArgument *parsedField = jag_ast_argument_list_get_at(argument_list, argIdx);
		CatStringWo *a_localVarName = NULL;


		if (localVariableTable!=NULL) {
			a_localVarName = jag_byt_local_variable_table_find_name(localVariableTable, frameIndex, 0);
		}

		JagAstFieldDeclaration *final_field = l_create_final(runtime, parsedField, a_localVarName);
		cat_array_wo_append(e_arguments, (GObject *) final_field);
		cat_unref(final_field);
		if (jag_ast_declaration_type_is_category2(jag_ast_argument_get_declaration_type(parsedField))) {
			frameIndex++;
		}
	}

	JagBytExceptions *parsedExceptions = jag_byt_method_header_get_exceptions(methodHeader);
	CatArrayWo *e_exceptions = NULL;
	if (parsedExceptions!=NULL && jag_byt_exceptions_count(parsedExceptions)>0) {
		int excCount = jag_byt_exceptions_count(parsedExceptions);
		e_exceptions = cat_array_wo_new();
		int idx;
		for(idx=0; idx<excCount; idx++) {
			// TODO can not just call getShortTypeName need to test import-repository
			JagBytName *exception_name = jag_byt_exceptions_get(parsedExceptions, idx);
			CatStringWo *a_resolvedName = jag_byt_name_get_short_type_name(exception_name);
			JagBytName *exception_fn = jag_byt_name_new(a_resolvedName, FALSE);
			cat_array_wo_append(e_exceptions, (GObject *) exception_fn);
			cat_unref_ptr(exception_fn);
		}
	}

	JagAstIdentifier *method_name = jag_ast_identifier_new(jag_byt_method_header_get_method_name(methodHeader));
	JagAstMethodHeader *result = jag_ast_method_header_new(method_name, jag_byt_method_header_is_constructor(methodHeader), finalDeclType, e_arguments, e_exceptions);
	cat_unref_ptr(finalDeclType);
	cat_unref_ptr(e_exceptions);
	cat_unref_ptr(method_name);
	cat_unref_ptr(e_arguments);
	return result;
}

JagAstIdentifier *jag_dec_model_stage_two_runtime_create_plain_name(JagDecModelStageTwoRuntime *runtime, JagAstDeclarationType *declarationType, JagAstIdentifier *fieldSourceClass) {
	JagDecModelStageTwoRuntimePrivate *priv = JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(runtime);
	JagAstIdentifier *result = NULL;
	if (jag_ast_declaration_type_is_reference(declarationType)) {
		JagBytName *decl_typename = jag_ast_declaration_type_get_reference_type_name(declarationType);
		JagBytName *resolvedTypename = jag_dec_import_repository_create_resolved_typename(priv->import_repository, decl_typename);
		if (fieldSourceClass!=NULL) {
			JagBytName *main_type_name = jag_dec_import_repository_get_main_type_name(priv->import_repository);
			if (jag_byt_name_equal(main_type_name, decl_typename)) {
				result = cat_ref_ptr(fieldSourceClass);
			} else {
				CatStringWo *a_id = jag_byt_name_create_fqn(resolvedTypename);
				CatStringWo *e_id = cat_string_wo_clone(a_id, CAT_CLONE_DEPTH_MAIN);
				cat_string_wo_append_unichar(e_id, '.');
				cat_string_wo_append(e_id, jag_ast_identifier_get_text(fieldSourceClass));
				result = jag_ast_identifier_new(e_id);
				cat_unref_ptr(e_id);
			}
		} else {
			CatStringWo *a_id = jag_byt_name_create_fqn(resolvedTypename);
			result = jag_ast_identifier_new(a_id);
		}
		cat_unref_ptr(resolvedTypename);
	} else {
		JagBytType prim_type = jag_ast_declaration_type_get_primitive_type(declarationType);
		CatStringWo* id = NULL;
		switch(prim_type) {
			case JAG_BYT_TYPE_BOOLEAN : id = cat_string_wo_new_with("boolean"); break;
			case JAG_BYT_TYPE_BYTE : id = cat_string_wo_new_with("byte"); break;
			case JAG_BYT_TYPE_SHORT : id = cat_string_wo_new_with("short"); break;
			case JAG_BYT_TYPE_CHAR : id = cat_string_wo_new_with("char"); break;
			case JAG_BYT_TYPE_INT : id = cat_string_wo_new_with("int"); break;
			case JAG_BYT_TYPE_LONG : id = cat_string_wo_new_with("long"); break;
			case JAG_BYT_TYPE_FLOAT : id = cat_string_wo_new_with("float"); break;
			case JAG_BYT_TYPE_DOUBLE : id = cat_string_wo_new_with("double"); break;
			case JAG_BYT_TYPE_VOID : id = cat_string_wo_new_with("void"); break;
			case JAG_BYT_TYPE_REFERENCE : id = cat_string_wo_new_with("<invalid-reference>"); break;
		}
		result = jag_ast_identifier_new(id);
	}
	return result;
}
