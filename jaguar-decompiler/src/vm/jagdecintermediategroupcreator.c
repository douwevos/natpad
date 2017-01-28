/*
   File:    jagdecintermediategroupcreator.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 5, 2012
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

#include "jagdecintermediategroupcreator.h"
#include "intermediate/item/jagdecintermediatearrayget.h"
#include "intermediate/item/jagdecintermediatearraylength.h"
#include "intermediate/item/jagdecintermediatearrayput.h"
#include "intermediate/item/jagdecintermediatebitwiseandorxor.h"
#include "intermediate/item/jagdecintermediatecast.h"
#include "intermediate/item/jagdecintermediatecondition.h"
#include "intermediate/item/jagdecintermediateconstant.h"
#include "intermediate/item/jagdecintermediateduplicate.h"
#include "intermediate/item/jagdecintermediateincrement.h"
#include "intermediate/item/jagdecintermediateinvokespecial.h"
#include "intermediate/item/jagdecintermediateinvokevirtual.h"
#include "intermediate/item/jagdecintermediatefieldget.h"
#include "intermediate/item/jagdecintermediatefieldput.h"
#include "intermediate/item/jagdecintermediateframeget.h"
#include "intermediate/item/jagdecintermediateframeput.h"
#include "intermediate/item/jagdecintermediategoto.h"
#include "intermediate/item/jagdecintermediatemathbasic.h"
#include "intermediate/item/jagdecintermediatemonitorenter.h"
#include "intermediate/item/jagdecintermediatemonitorexit.h"
#include "intermediate/item/jagdecintermediatemultinewarray.h"
#include "intermediate/item/jagdecintermediatenew.h"
#include "intermediate/item/jagdecintermediatenewarray.h"
#include "intermediate/item/jagdecintermediatereturn.h"
#include "intermediate/item/jagdecintermediatethrow.h"
#include "intermediate/jagdecintermediatetrycatchgroup.h"
#include "intermediate/jagdecintermediatecatchgroup.h"
#include "intermediate/jagdecintermediatefinallygroup.h"
#include "intermediate/jagdecintermediatenodescope.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateGroupCreator"
#include <logging/catlog.h>

struct _JagDecIntermediateGroupCreatorPrivate {
	JagBytMnemonicBlock *mnemonic_block;
	JagBytClassfile *type_definition;
	JagDecImportRepository *import_repository;
	JagBytMethod *method;
	JagBytTryCatchList *try_catch_list;
	JagDecIntermediateGroup *model_container;
};

G_DEFINE_TYPE (JagDecIntermediateGroupCreator, jag_dec_intermediate_group_creator, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_group_creator_class_init(JagDecIntermediateGroupCreatorClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateGroupCreatorPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_group_creator_init(JagDecIntermediateGroupCreator *instance) {
	JagDecIntermediateGroupCreatorPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR, JagDecIntermediateGroupCreatorPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateGroupCreator *instance = JAG_DEC_INTERMEDIATE_GROUP_CREATOR(object);
	JagDecIntermediateGroupCreatorPrivate *priv = instance->priv;
	cat_unref_ptr(priv->import_repository);
	cat_unref_ptr(priv->method);
	cat_unref_ptr(priv->mnemonic_block);
	cat_unref_ptr(priv->model_container);
	cat_unref_ptr(priv->try_catch_list);
	cat_unref_ptr(priv->type_definition);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static JagDecIntermediateGroup *l_create_initial_block(JagDecIntermediateGroupCreator *group_creator);

JagDecIntermediateGroupCreator *jag_dec_intermediate_group_creator_new(JagDecImportRepository *import_repository, JagBytClassfile *type_definition, JagBytMethod *method, JagBytMnemonicBlock *mnemonic_block, JagBytTryCatchList *try_catch_list) {
	JagDecIntermediateGroupCreator *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateGroupCreatorPrivate *priv = result->priv;
	priv->mnemonic_block = cat_ref_ptr(mnemonic_block);
	priv->type_definition = cat_ref_ptr(type_definition);
	priv->import_repository = cat_ref_ptr(import_repository);
	priv->method = cat_ref_ptr(method);
	priv->try_catch_list = cat_ref_ptr(try_catch_list);
//	if (tryCatchList!=null) {
//		tryCatchList.dump(System.out);
//	}
	priv->model_container = l_create_initial_block(result);

	return result;
}

JagDecIntermediateGroup *jag_dec_intermediate_group_creator_get_main_container(JagDecIntermediateGroupCreator *group_creator) {
	JagDecIntermediateGroupCreatorPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator);
	return priv->model_container;
}


static JagDecIIntermediateNode *l_run(JagDecIntermediateGroupCreator *group_creator, int oppIndex, JagBytIMnemonic *mnemonic);


static JagDecIntermediateGroup *l_create_initial_block(JagDecIntermediateGroupCreator *group_creator) {
	JagDecIntermediateGroupCreatorPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator);
	int oppIndex = 0;

	JagBytLabelRepository *labelRepository = jag_byt_mnemonic_block_get_label_repository(priv->mnemonic_block);

	JagDecIntermediateGroup *result = jag_dec_intermediate_group_new();


	JagDecIntermediateGroup *main_group = cat_ref_ptr(result);

	JagDecIntermediateGroup *group = NULL;


	CatArrayWo *e_pendingTryCatches = cat_array_wo_new();

	int tryCatchListIndex = 0;
	int mnemonic_idx;
	for(mnemonic_idx=0; mnemonic_idx<jag_byt_mnemonic_block_count(priv->mnemonic_block); mnemonic_idx++) {
		JagBytIMnemonic *mnemonic = jag_byt_mnemonic_block_get(priv->mnemonic_block, mnemonic_idx);
		int offset = jag_byt_imnemonic_get_offset(mnemonic);

		if (priv->try_catch_list!=NULL) {
			cat_unref_ptr(main_group);
			while(cat_array_wo_size(e_pendingTryCatches)!=0) {
				JagDecIntermediateTryCatchGroup *tryCatchGroup = (JagDecIntermediateTryCatchGroup *) cat_array_wo_get_last(e_pendingTryCatches);
				JagBytTryCatch *tryCatch = jag_dec_intermediate_try_catch_group_get_try_catch(tryCatchGroup);
				if (jag_byt_try_catch_get_end_pc(tryCatch)<=offset) {
					cat_array_wo_remove_last(e_pendingTryCatches, NULL);
					group = NULL;
				} else {
					main_group = cat_ref_ptr(tryCatchGroup);
					break;
				}
			}
			if (main_group == NULL) {
				main_group = cat_ref_ptr(result);
			}


			while(tryCatchListIndex<jag_byt_try_catch_list_count(priv->try_catch_list)) {
				JagBytTryCatch *tryCatch = jag_byt_try_catch_list_get(priv->try_catch_list, tryCatchListIndex);

				if (jag_byt_try_catch_get_start_pc(tryCatch)<=offset) {
					JagDecIntermediateTryCatchGroup *tryCatchGroup = jag_dec_intermediate_try_catch_group_new(tryCatch);
					int catchItemCount = jag_byt_try_catch_get_catch_count(tryCatch);
					int catchItemIdx;
					for(catchItemIdx=0; catchItemIdx<catchItemCount ; catchItemIdx++) {
						JagBytICatchOrFinally *catchItem = jag_byt_try_catch_get_catch_at(tryCatch, catchItemIdx);
						if (JAG_BYT_IS_CATCH(catchItem)) {
							JagBytCatch *catch = JAG_BYT_CATCH(catchItem);
							JagAstDeclarationType *exception_type = jag_byt_catch_get_exception_type(catch);
							jag_dec_import_repository_attach_declaration_type(priv->import_repository, exception_type, TRUE);
							JagDecIntermediateCatchGroup *catch_group = jag_dec_intermediate_catch_group_new(exception_type);
							jag_dec_intermediate_try_catch_group_add_catch_group(tryCatchGroup, catch_group);
							cat_unref_ptr(catch_group);
						} else {
							JagDecIntermediateFinallyGroup *finally_group = jag_dec_intermediate_finally_group_new();
							jag_dec_intermediate_try_catch_group_add_finally_group(tryCatchGroup, finally_group);
							cat_unref_ptr(finally_group);
						}
					}

					cat_array_wo_append(e_pendingTryCatches, (GObject *) tryCatchGroup);
					jag_dec_intermediate_group_add(main_group, (JagDecIIntermediateNode *) tryCatchGroup);
					group = NULL;
					cat_unref_ptr(main_group);
					main_group = (JagDecIntermediateGroup *) tryCatchGroup;
					tryCatchListIndex++;
				} else  {
					break;
				}
			}
		}

		int branchOffset = jag_byt_imnemonic_get_branch_offset(mnemonic);
		JagDecIIntermediateNode *intermediateNode = l_run(group_creator, oppIndex, mnemonic);

		if (intermediateNode!=NULL) {
			cat_log_debug("intermediateNode:%p", intermediateNode);
			cat_log_debug("intermediateNode:%s", g_type_name_from_instance((GTypeInstance *) intermediateNode));
			if (group==NULL) {
				group = jag_dec_intermediate_group_new();
				jag_dec_intermediate_group_add(main_group, (JagDecIIntermediateNode *) group);
				cat_unref(group);
			}
			jag_dec_intermediate_group_add(group, intermediateNode);
			cat_unref(intermediateNode);
//				log.detail("adding:"+oppIndex+" >> "+subCode);
		} else {
//			cat_log_warn(""+mnemonic+", class="+TextUtil.getName(mnemonic.getClass()));
		}


		JagBytLabel *nextOppLabel = jag_byt_label_repository_get(labelRepository, oppIndex+1);
		if (intermediateNode!=NULL) {
			JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref(intermediateNode);
			if (jag_dec_intermediate_node_scope_get_exit_index(scope)<0 && jag_dec_intermediate_node_scope_get_jump_index(scope)<0) {
				group = NULL;
			}
			cat_unref_ptr(scope);
		}
		if (branchOffset>=0 || (nextOppLabel!=NULL && jag_byt_label_get_label_type(nextOppLabel)==JAG_BYT_LABEL_TYPE_HARD) || oppIndex==jag_byt_mnemonic_block_count(priv->mnemonic_block)-1) {
			group = NULL;
		}
		oppIndex++;
	}
	cat_unref_ptr(main_group);
	cat_unref_ptr(e_pendingTryCatches);
	return result;
}



JagBytConstantPool *l_get_constant_pool(JagDecIntermediateGroupCreator *group_creator) {
	JagBytClassfile *classfile = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->type_definition;
	return jag_byt_classfile_get_constant_pool(classfile);
}

static JagDecIIntermediateNode *l_run(JagDecIntermediateGroupCreator *group_creator, int oppIndex, JagBytIMnemonic *mnemonic) {
	JagBytOperation mnemonic_op = jag_byt_imnemonic_get_operation(mnemonic);
	cat_log_debug("oppIndex=%d, mnemonic_op=%d", oppIndex, mnemonic_op);

	switch(mnemonic_op) {	// TODO implement
////			case OP_NOP : break;
		case OP_ACONST_NULL : {
			JagAstIExpression *null_expr = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) cat_string_wo_new_with("null"));
			JagDecValueTypeDefinitionList *null_type_def_list = jag_dec_value_type_definition_list_new();
			JagDecIntermediateConstant *result = jag_dec_intermediate_constant_new(oppIndex, null_type_def_list, null_expr);
			cat_unref_ptr(null_expr);
			cat_unref_ptr(null_type_def_list);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_ICONST_M1 :
		case OP_ICONST_0 :
		case OP_ICONST_1 :
		case OP_ICONST_2 :
		case OP_ICONST_3 :
		case OP_ICONST_4 :
		case OP_ICONST_5 :
		case OP_LCONST_0 :
		case OP_LCONST_1 :
		case OP_FCONST_0 :
		case OP_FCONST_1 :
		case OP_FCONST_2 :
		case OP_DCONST_0 :
		case OP_DCONST_1 : {
			JagBytOpLoadConstant *op_load_constant = JAG_BYT_OP_LOAD_CONSTANT(mnemonic);
			JagAstDeclarationType *constant_decl_type = jag_ast_declaration_type_new(jag_byt_op_load_constant_get_constant_type(op_load_constant), 0);
			JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
			JagDecValueTypeDefinition *type_definition = jag_dec_value_type_definition_new(constant_decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
			cat_unref(constant_decl_type);
			jag_dec_value_type_definition_list_add_definition(type_set, type_definition);
			cat_unref(type_definition);
			JagAstLiteral *lit = jag_ast_literal_new(jag_byt_op_load_constant_get_contant_value(op_load_constant));
			JagDecIntermediateConstant *result = jag_dec_intermediate_constant_new(oppIndex, type_set, (JagAstIExpression *) lit);
			cat_unref(type_set);
			cat_unref(lit);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_BIPUSH :
		case OP_SIPUSH : {
			JagBytOpPush *op_push = JAG_BYT_OP_PUSH(mnemonic);
			JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
			jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_INT, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
			jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_CHAR, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
			int val = jag_byt_op_push_get_value(op_push);
			CatInteger *pval = cat_integer_new(val);
			JagAstIExpression *defaultExpression = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) pval);
			cat_unref(pval);

			if (val==0 || val==1) {
				jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_BOOLEAN, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
			}

			JagDecIntermediateConstant *result = jag_dec_intermediate_constant_new(oppIndex, type_set, defaultExpression);
			cat_unref(defaultExpression);
			cat_unref(type_set);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_LDC :
		case OP_LDC_W :
		case OP_LDC2_W : {
			JagBytOpPushIndex *op_push_index = JAG_BYT_OP_PUSH_INDEX(mnemonic);
			int pool_index = jag_byt_op_push_index_get_pool_index(op_push_index);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, pool_index-1);
			if (iconstant) {
			cat_log_detail("poolConstant.class=%s", g_type_name_from_instance((GTypeInstance *) iconstant));
			if (JAG_BYT_IS_CONSTANT_UTF8_TEXT(iconstant)) {
				CatStringWo *a_text = jag_byt_constant_utf8_text_get_value((JagBytConstantUtf8Text *) iconstant);
				JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
				JagBytName *string_name = jag_byt_name_new(cat_string_wo_new_with("java.lang.String"), FALSE);
				JagAstDeclarationType *string_decl_type = jag_ast_declaration_type_new_reference(string_name, 0);
				cat_unref_ptr(string_name);
				JagDecValueTypeDefinition *string_definition = jag_dec_value_type_definition_new(string_decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
				cat_unref_ptr(string_decl_type);
				jag_dec_value_type_definition_list_add_definition(type_set, string_definition);
				cat_unref_ptr(string_definition);
				JagAstQuotedString *quoted_string = jag_ast_quoted_string_new(a_text);
				JagDecIIntermediateNode *result = (JagDecIIntermediateNode *) jag_dec_intermediate_constant_new(oppIndex, type_set, (JagAstIExpression *) quoted_string);
				cat_unref_ptr(quoted_string);
				cat_unref_ptr(type_set);
				return result;
			} else if (JAG_BYT_IS_CONSTANT_STRING(iconstant)) {
				CatStringWo *a_text = jag_byt_constant_string_get_value((JagBytConstantString *) iconstant);
				JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
				JagBytName *string_name = jag_byt_name_new(cat_string_wo_new_with("java.lang.String"), FALSE);
				JagAstDeclarationType *string_decl_type = jag_ast_declaration_type_new_reference(string_name, 0);
				cat_unref_ptr(string_name);
				JagDecValueTypeDefinition *string_definition = jag_dec_value_type_definition_new(string_decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
				cat_unref_ptr(string_decl_type);
				jag_dec_value_type_definition_list_add_definition(type_set, string_definition);
				cat_unref_ptr(string_definition);
				JagAstQuotedString *quoted_string = jag_ast_quoted_string_new(a_text);
				JagDecIIntermediateNode *result = (JagDecIIntermediateNode *) jag_dec_intermediate_constant_new(oppIndex, type_set, (JagAstIExpression *) quoted_string);
				cat_unref_ptr(quoted_string);
				cat_unref_ptr(type_set);
				return (JagDecIIntermediateNode *) result;
			} else if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
				JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
				JagBytName *string_name = jag_byt_name_new(cat_string_wo_new_with("java.lang.Class"), FALSE);
				JagAstDeclarationType *string_decl_type = jag_ast_declaration_type_new_reference(string_name, 0);
				cat_unref_ptr(string_name);
				JagDecValueTypeDefinition *string_definition = jag_dec_value_type_definition_new(string_decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
				cat_unref_ptr(string_decl_type);
				jag_dec_value_type_definition_list_add_definition(type_set, string_definition);
				cat_unref_ptr(string_definition);
				JagBytName *ref_type_name = jag_byt_constant_clazz_get_refrence_type_name((JagBytConstantClazz *) iconstant);
				CatStringWo *a_fqn = jag_byt_name_create_fqn(ref_type_name);
				CatStringWo *fqn = cat_string_wo_clone(a_fqn, CAT_CLONE_DEPTH_MAIN);
				cat_string_wo_append_chars(fqn, ".class");
				JagAstLiteral *literal = jag_ast_literal_new((CatIStringable *) fqn);
				JagDecIIntermediateNode *result = (JagDecIIntermediateNode *) jag_dec_intermediate_constant_new(oppIndex, type_set, (JagAstIExpression *) literal);
				cat_unref_ptr(literal);
				cat_unref_ptr(type_set);
				return (JagDecIIntermediateNode *) result;
			} else if (JAG_BYT_IS_CONSTANT_LONG(iconstant)) {
				int64_t val = jag_byt_constant_long_get_value((JagBytConstantLong *) iconstant);
				JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
				jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_LONG, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
				CatLong *long_val = cat_long_new(val);
				JagAstLiteral *long_lit = jag_ast_literal_new((CatIStringable *) long_val);
				cat_unref_ptr(long_val);
				JagDecIntermediateConstant *result = jag_dec_intermediate_constant_new(oppIndex, type_set, (JagAstIExpression *) long_lit);
				cat_unref_ptr(type_set);
				cat_unref_ptr(long_lit);
				return (JagDecIIntermediateNode *) result;
			} else if (JAG_BYT_IS_CONSTANT_INT(iconstant)) {
				int32_t val = jag_byt_constant_int_get_value((JagBytConstantInt *) iconstant);
				JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
				jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_INT, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
				jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_CHAR, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
				CatInteger *int_val = cat_integer_new(val);
				JagAstLiteral *int_lit = jag_ast_literal_new((CatIStringable *) int_val);
				cat_unref_ptr(int_val);
				JagDecIntermediateConstant *result = jag_dec_intermediate_constant_new(oppIndex, type_set, (JagAstIExpression *) int_lit);
				cat_unref_ptr(type_set);
				cat_unref_ptr(int_lit);
				return (JagDecIIntermediateNode *) result;
			} else if (JAG_BYT_IS_CONSTANT_FLOAT(iconstant)) {
				float val = jag_byt_constant_float_get_value((JagBytConstantFloat *) iconstant);
				JagDecValueTypeDefinitionList *type_set = jag_dec_value_type_definition_list_new();
				jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_FLOAT, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
				CatFloat *float_val = cat_float_new(val);
				cat_unref_ptr(float_val);
				JagAstLiteral *float_lit = jag_ast_literal_new((CatIStringable *) float_val);
				JagDecIntermediateConstant *result = jag_dec_intermediate_constant_new(oppIndex, type_set, (JagAstIExpression *) float_lit);
				cat_unref_ptr(type_set);
				cat_unref_ptr(float_lit);
				return (JagDecIIntermediateNode *) result;
	//				} else if (value instanceof Double) {	// TODO implement
	//					expr = new ValueTypeDefinitionList(new DeclarationType(Type.DOUBLE,0), ValueTypeDefinitionKind.CONSTANT);
	//				}
	//				IntermediateConstant result = new IntermediateConstant(mnemonicIndex, expr, new Literal(value));
	//				return (JagDecIIntermediateNode *) result;
	//
				}

			}
		} break;
		case OP_ILOAD :
		case OP_LLOAD :
		case OP_FLOAD :
		case OP_DLOAD :
		case OP_ALOAD : {
			JagBytOpLoadIndex *op_load_index = JAG_BYT_OP_LOAD_INDEX(mnemonic);
			int frame_index = jag_byt_op_load_index_get_frame_index(op_load_index);
			JagDecIntermediateFrameGet *result = jag_dec_intermediate_frame_get_new(oppIndex, frame_index);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_ILOAD_0 :
		case OP_ILOAD_1 :
		case OP_ILOAD_2 :
		case OP_ILOAD_3 :
		case OP_LLOAD_0 :
		case OP_LLOAD_1 :
		case OP_LLOAD_2 :
		case OP_LLOAD_3 :
		case OP_FLOAD_0 :
		case OP_FLOAD_1 :
		case OP_FLOAD_2 :
		case OP_FLOAD_3 :
		case OP_DLOAD_0 :
		case OP_DLOAD_1 :
		case OP_DLOAD_2 :
		case OP_DLOAD_3 :
		case OP_ALOAD_0 :
		case OP_ALOAD_1 :
		case OP_ALOAD_2 :
		case OP_ALOAD_3 : {
			JagBytOpLoadFastIndex *op_load_fast_index = JAG_BYT_OP_LOAD_FAST_INDEX(mnemonic);
			int frame_index = jag_byt_op_load_fast_index_get_frame_index(op_load_fast_index);
			JagDecIntermediateFrameGet *result = jag_dec_intermediate_frame_get_new(oppIndex, frame_index);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_IALOAD :
		case OP_LALOAD :
		case OP_FALOAD :
		case OP_DALOAD :
		case OP_AALOAD :
		case OP_BALOAD :
		case OP_CALOAD :
		case OP_SALOAD : {
			JagBytOpLoadArray *op_load_array = JAG_BYT_OP_LOAD_ARRAY(mnemonic);
			JagBytType load_type = jag_byt_op_load_array_get_load_type(op_load_array);
			JagDecIntermediateArrayGet *result = jag_dec_intermediate_array_get_new(oppIndex, load_type);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_ISTORE :
		case OP_LSTORE :
		case OP_FSTORE :
		case OP_DSTORE :
		case OP_ASTORE : {
			JagBytOpStoreIndex *op_store_index = JAG_BYT_OP_STORE_INDEX(mnemonic);
			int frame_index = jag_byt_op_store_index_get_frame_index(op_store_index);
			JagDecIntermediateFramePut *result = jag_dec_intermediate_frame_put_new(oppIndex, frame_index);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_ISTORE_0 :
		case OP_ISTORE_1 :
		case OP_ISTORE_2 :
		case OP_ISTORE_3 :
		case OP_LSTORE_0 :
		case OP_LSTORE_1 :
		case OP_LSTORE_2 :
		case OP_LSTORE_3 :
		case OP_FSTORE_0 :
		case OP_FSTORE_1 :
		case OP_FSTORE_2 :
		case OP_FSTORE_3 :
		case OP_DSTORE_0 :
		case OP_DSTORE_1 :
		case OP_DSTORE_2 :
		case OP_DSTORE_3 :
		case OP_ASTORE_0 :
		case OP_ASTORE_1 :
		case OP_ASTORE_2 :
		case OP_ASTORE_3 : {
			JagBytOpStoreFastIndex *op_store_fast_index = JAG_BYT_OP_STORE_FAST_INDEX(mnemonic);
			int frame_index = jag_byt_op_store_fast_index_get_frame_index(op_store_fast_index);
			JagDecIntermediateFramePut *result = jag_dec_intermediate_frame_put_new(oppIndex, frame_index);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_IASTORE :
		case OP_LASTORE :
		case OP_FASTORE :
		case OP_DASTORE :
		case OP_AASTORE :
		case OP_BASTORE :
		case OP_CASTORE :
		case OP_SASTORE : {
			JagBytOpStoreArray *op_store_array = JAG_BYT_OP_STORE_ARRAY(mnemonic);
			JagBytType store_type = jag_byt_op_store_array_get_store_type(op_store_array);
			JagDecIntermediateArrayPut *result = jag_dec_intermediate_array_put_new(oppIndex, store_type);
			return (JagDecIIntermediateNode *) result;
		}
////			case OP_POP : break;
////			case OP_POP2 : break;
		case OP_DUP :
		case OP_DUP_X1 :
		case OP_DUP_X2 :
		case OP_DUP2 :
		case OP_DUP2_X1 :
		case OP_DUP2_X2 : {
//			JagBytOpDuplicate *op_duplicate = JAG_BYT_OP_DUPLICATE(mnemonic);
			JagDecIntermediateDuplicate *result = jag_dec_intermediate_duplicate_new(oppIndex, mnemonic_op);
			return (JagDecIIntermediateNode *) result;
		}
////			case OP_SWAP : break;
		case OP_IADD :
		case OP_LADD :
		case OP_FADD :
		case OP_DADD :
		case OP_ISUB :
		case OP_LSUB :
		case OP_FSUB :
		case OP_DSUB :
		case OP_IMUL :
		case OP_LMUL :
		case OP_FMUL :
		case OP_DMUL :
		case OP_IDIV :
		case OP_LDIV :
		case OP_FDIV :
		case OP_DDIV :
		case OP_IREM :
		case OP_LREM :
		case OP_FREM :
		case OP_DREM :
		case OP_INEG :
		case OP_LNEG :
		case OP_FNEG :
		case OP_DNEG : {
			JagBytOpMathBasic *op_math_basic = JAG_BYT_OP_MATH_BASIC(mnemonic);
			JagDecIntermediateMathBasic *result = jag_dec_intermediate_math_basic_new(oppIndex, jag_byt_op_math_basic_get_math_operator(op_math_basic));
			return (JagDecIIntermediateNode *) result;
		}
		case OP_ISHL :
		case OP_LSHL :
		case OP_ISHR :
		case OP_LSHR :
		case OP_IUSHR :
		case OP_LUSHR :
		case OP_IAND :
		case OP_LAND :
		case OP_IOR :
		case OP_LOR :
		case OP_IXOR :
		case OP_LXOR : {
			JagBytOpMathLogic *op_math_logic = JAG_BYT_OP_MATH_LOGIC(mnemonic);
			JagDecIntermediateBitwiseAndOrXor *result = jag_dec_intermediate_bitwise_and_or_xor_new(oppIndex, jag_byt_op_math_logic_get_bitwise_operator(op_math_logic));
			return (JagDecIIntermediateNode *) result;
		}
		case OP_IINC : {
			JagBytOpIncrement *op_increment = JAG_BYT_OP_INCREMENT(mnemonic);
			int frame_index = jag_byt_op_increment_get_frame_index(op_increment);
			int increment_value = jag_byt_op_increment_get_increment_value(op_increment);
			return (JagDecIIntermediateNode *) jag_dec_intermediate_increment_new(oppIndex, frame_index, increment_value);
		}
////			case OP_I2L : break;
////			case OP_I2F : break;
////			case OP_I2D : break;
////			case OP_L2I : break;
////			case OP_L2F : break;
////			case OP_L2D : break;
////			case OP_F2I : break;
////			case OP_F2L : break;
////			case OP_F2D : break;
////			case OP_D2I : break;
////			case OP_D2L : break;
////			case OP_D2F : break;
////			case OP_I2B : break;
////			case OP_I2C : break;
////			case OP_I2S : break;
////			case OP_LCMP : break;
////			case OP_FCMPL : break;
////			case OP_FCMPG : break;
////			case OP_DCMPL : break;
////			case OP_DCMPG : break;
		case OP_IFEQ :
		case OP_IFNE :
		case OP_IFLT :
		case OP_IFGE :
		case OP_IFGT :
		case OP_IFLE :
		case OP_IF_ICMPEQ :
		case OP_IF_ICMPNE :
		case OP_IF_ICMPLT :
		case OP_IF_ICMPGE :
		case OP_IF_ICMPGT :
		case OP_IF_ICMPLE : {
			JagBytOpCompareInt *op_compare_int = JAG_BYT_OP_COMPARE_INT(mnemonic);
			JagBytConditionType condition_type = jag_byt_op_compare_int_get_condition_type(op_compare_int);
			gboolean is_with_zero = jag_byt_op_compare_int_is_with_zero(op_compare_int);
			JagBytMnemonicBlock *mnemonic_block = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->mnemonic_block;
			int branch_offset = jag_byt_imnemonic_get_branch_offset(mnemonic);
			int branch_index = jag_byt_mnemonic_block_find_by_bytecode_offset(mnemonic_block, branch_offset);

//			JagBytLabel *branch_label =
					jag_byt_mnemonic_block_find_or_create_label_at(mnemonic_block, branch_index);
			JagDecIntermediateCondition *result = jag_dec_intermediate_condition_new(oppIndex, condition_type, branch_index, is_with_zero, TRUE);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_IF_ACMPEQ :
		case OP_IF_ACMPNE :
		case OP_IFNULL :
		case OP_IFNONNULL : {
			JagBytOpCompareReference *op_compare_ref = JAG_BYT_OP_COMPARE_REFERENCE(mnemonic);

			JagBytConditionType condition_type = jag_byt_op_compare_reference_get_condition_type(op_compare_ref);
			gboolean is_with_zero = jag_byt_op_compare_reference_is_with_zero(op_compare_ref);
			JagBytMnemonicBlock *mnemonic_block = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->mnemonic_block;
			int branch_offset = jag_byt_imnemonic_get_branch_offset(mnemonic);
			int branch_index = jag_byt_mnemonic_block_find_by_bytecode_offset(mnemonic_block, branch_offset);

//			JagBytLabel *branch_label =
					jag_byt_mnemonic_block_find_or_create_label_at(mnemonic_block, branch_index);
			JagDecIntermediateCondition *result = jag_dec_intermediate_condition_new(oppIndex, condition_type, branch_index, is_with_zero, FALSE);
			return (JagDecIIntermediateNode *) result;
		}
		case OP_GOTO :
		case OP_GOTO_W : {
			JagBytMnemonicBlock *mnemonic_block = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->mnemonic_block;
			int branch_offset = jag_byt_imnemonic_get_branch_offset(mnemonic);
			int branch_index = jag_byt_mnemonic_block_find_by_bytecode_offset(mnemonic_block, branch_offset);
//			JagBytLabel *branch_label =
					jag_byt_mnemonic_block_find_or_create_label_at(mnemonic_block, branch_index);
			JagDecIntermediateGoto *result = jag_dec_intermediate_goto_new(oppIndex, branch_index);
			return (JagDecIIntermediateNode *) result;
		}
////			case OP_JSR : break;
////			case OP_RET : break;
////			case OP_TABLESWITCH : break;
////			case OP_LOOKUPSWITCH : break;
		case OP_IRETURN :
		case OP_LRETURN :
		case OP_FRETURN :
		case OP_DRETURN :
		case OP_ARETURN :
		case OP_RETURN : {
			JagBytOpReturn *op_return = JAG_BYT_OP_RETURN(mnemonic);
			JagAstDeclarationType *returnType = NULL;
			JagBytMethod *parsedMethod = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->method;
			if (parsedMethod!=NULL) {
				returnType = jag_byt_method_header_get_return_type(jag_byt_method_get_method_header(parsedMethod));
				if (returnType!=NULL && jag_ast_declaration_type_is_void(returnType)) {
					returnType = NULL;
				} else {
					cat_ref_ptr(returnType);
				}
			} else {
				JagBytType op_return_type = jag_byt_op_return_get_return_type(op_return);
				if (op_return_type==JAG_BYT_TYPE_REFERENCE) {
					returnType = jag_byt_method_header_get_return_type(jag_byt_method_get_method_header(parsedMethod));
					cat_ref_ptr(returnType);
				} else if (op_return_type!=JAG_BYT_TYPE_VOID) {
					returnType = jag_ast_declaration_type_new(op_return_type, 0);
				}
			}
			JagDecIIntermediateNode *result = (JagDecIIntermediateNode *) jag_dec_intermediate_return_new(oppIndex, returnType);
			cat_unref_ptr(returnType);
			return result;
		}
		case OP_GETSTATIC :
		case OP_GETFIELD : {
			JagBytOpGet *op_get = JAG_BYT_OP_GET(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

			int field_ref_index = jag_byt_op_get_get_field_reference_pool_index(op_get);
			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, field_ref_index-1);
			if (JAG_BYT_IS_CONSTANT_FIELDREF(iconstant)) {
				JagBytConstantFieldref *constant_field_ref = JAG_BYT_CONSTANT_FIELDREF(iconstant);
				JagAstDeclarationType *declaration_type = jag_byt_constant_fieldref_get_declaration_type(constant_field_ref);

				JagAstDeclarationType *field_source_declaration_type = jag_byt_constant_fieldref_get_field_source_declaration_type(constant_field_ref);
				JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
				jag_dec_import_repository_attach_declaration_type(import_repo, field_source_declaration_type, TRUE);

				JagAstIdentifier *field_name = jag_ast_identifier_new(jag_byt_constant_fieldref_get_field_name(constant_field_ref));
				JagDecIntermediateFieldGet *result = jag_dec_intermediate_field_get_new(oppIndex, declaration_type, field_source_declaration_type, field_name, mnemonic_op==OP_GETFIELD);
				cat_unref_ptr(field_name);
				return (JagDecIIntermediateNode *) result;
			}
		} break;
		case OP_PUTSTATIC :
		case OP_PUTFIELD : {
			JagBytOpPut *op_put = JAG_BYT_OP_PUT(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

			int field_ref_index = jag_byt_op_put_get_field_reference_pool_index(op_put);
			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, field_ref_index-1);
			if (JAG_BYT_IS_CONSTANT_FIELDREF(iconstant)) {
				JagBytConstantFieldref *constant_field_ref = JAG_BYT_CONSTANT_FIELDREF(iconstant);
				JagAstDeclarationType *declaration_type = jag_byt_constant_fieldref_get_declaration_type(constant_field_ref);
//				importRepository.attach(declarationType);
				JagAstDeclarationType *field_source_declaration_type = jag_byt_constant_fieldref_get_field_source_declaration_type(constant_field_ref);
				JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
				jag_dec_import_repository_attach_declaration_type(import_repo, field_source_declaration_type, TRUE);

				JagAstIdentifier *field_name = jag_ast_identifier_new(jag_byt_constant_fieldref_get_field_name(constant_field_ref));
				JagDecIntermediateFieldPut *result = jag_dec_intermediate_field_put_new(oppIndex, declaration_type, field_source_declaration_type, field_name, mnemonic_op==OP_PUTFIELD);
				cat_unref_ptr(field_name);
				return (JagDecIIntermediateNode *) result;
			}
		} break;
		case OP_INVOKEVIRTUAL : {
			JagBytOpInvoke *op_invoke = JAG_BYT_OP_INVOKE(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, jag_byt_op_invoke_get_methodref_pool_index(op_invoke)-1);
			if (JAG_BYT_IS_CONSTANT_METHODREF(iconstant)) {
				JagBytConstantMethodref *constant_methodref = (JagBytConstantMethodref *) iconstant;
				JagBytMethodHeader *method_header = jag_byt_constant_methodref_get_method_header(constant_methodref);
				JagDecIntermediateInvokeVirtual *result = jag_dec_intermediate_invoke_virtual_new(oppIndex, method_header);
				return (JagDecIIntermediateNode *) result;
			} else {
				cat_log_error("invalid constant for op invoke special");
			}
		} break;
		case OP_INVOKESPECIAL : {
			JagBytOpInvoke *op_invoke = JAG_BYT_OP_INVOKE(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, jag_byt_op_invoke_get_methodref_pool_index(op_invoke)-1);
			if (JAG_BYT_IS_CONSTANT_METHODREF(iconstant)) {
				JagBytConstantMethodref *constant_methodref = (JagBytConstantMethodref *) iconstant;
				JagBytMethodHeader *method_header = jag_byt_constant_methodref_get_method_header(constant_methodref);
				JagAstDeclarationType *method_class_decl_type = jag_byt_constant_methodref_get_method_class_declaration_type(constant_methodref);
				JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
				jag_dec_import_repository_attach_declaration_type(import_repo, method_class_decl_type, TRUE);
				JagDecIntermediateInvokeSpecial *result = jag_dec_intermediate_invoke_special_new(oppIndex, method_class_decl_type, method_header);
				return (JagDecIIntermediateNode *) result;
			} else {
				cat_log_error("invalid constant for op invoke special");
			}
		} break;
		case OP_INVOKESTATIC : {
			JagBytOpInvoke *op_invoke = JAG_BYT_OP_INVOKE(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, jag_byt_op_invoke_get_methodref_pool_index(op_invoke)-1);
			if (JAG_BYT_IS_CONSTANT_METHODREF(iconstant)) {
				JagBytConstantMethodref *constant_methodref = (JagBytConstantMethodref *) iconstant;
				JagBytMethodHeader *method_header = jag_byt_constant_methodref_get_method_header(constant_methodref);
				JagAstDeclarationType *method_class_decl_type = jag_byt_constant_methodref_get_method_class_declaration_type(constant_methodref);
				JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
				jag_dec_import_repository_attach_declaration_type(import_repo, method_class_decl_type, TRUE);
				JagDecIntermediateInvokeSpecial *result = jag_dec_intermediate_invoke_special_new(oppIndex, method_class_decl_type, method_header);
				return (JagDecIIntermediateNode *) result;
			} else {
				cat_log_error("invalid constant for op invoke special");
			}


			//			OpInvoke oppInvoke = (OpInvoke) mnemonic;
//
//			IConstant constant = typeDefinition.getConstantPool().getResolved(oppInvoke.index-1);
//			ConstantMethodReference poolConstant = (ConstantMethodReference) constant;
//			DeclarationType descriptor = poolConstant.getConstantClassInfo().getParsedDescriptor();
//
//			ClassfileMethodHeader parsedMethodHeader = poolConstant.getMethodHeader();
//			return new IntermediateInvokeStatic(mnemonicIndex, descriptor, parsedMethodHeader);
		} break;
//		case OP_INVOKEINTERFACE : {
//			OpInvoke oppInvoke = (OpInvoke) mnemonic;
//
//			IConstant constant = typeDefinition.getConstantPool().getResolved(oppInvoke.index-1);
//			ConstantMethodReference poolConstant = (ConstantMethodReference) constant;
//
//			ClassfileMethodHeader parsedMethodHeader = poolConstant.getMethodHeader();
//			return new IntermediateInvokeVirtual(mnemonicIndex, parsedMethodHeader);
//		}
////			case OP_INVOKEDYNAMIC : break;
		case OP_NEW : {
			JagBytOpNew *op_new = JAG_BYT_OP_NEW(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, jag_byt_op_new_get_pool_index(op_new)-1);
			if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
				JagAstDeclarationType *declarationType = jag_byt_constant_clazz_get_parsed_descriptor((JagBytConstantClazz *) iconstant);
				JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
				jag_dec_import_repository_attach_declaration_type(import_repo, declarationType, TRUE);
				return (JagDecIIntermediateNode *) jag_dec_intermediate_new_new(oppIndex, declarationType);
			} else {
				cat_log_error("invalid constant for op new");
			}
		} break;
		case OP_NEWARRAY :
		case OP_ANEWARRAY : {
			JagBytOpNewArray *op_new_array = JAG_BYT_OP_NEW_ARRAY(mnemonic);
			JagBytType array_type = jag_byt_op_new_array_get_primitive_type(op_new_array);
			switch(array_type) {
				case JAG_BYT_TYPE_BOOLEAN :
				case JAG_BYT_TYPE_BYTE :
				case JAG_BYT_TYPE_CHAR :
				case JAG_BYT_TYPE_DOUBLE :
				case JAG_BYT_TYPE_FLOAT :
				case JAG_BYT_TYPE_INT :
				case JAG_BYT_TYPE_LONG :
				case JAG_BYT_TYPE_SHORT : {
					JagAstDeclarationType *array_decl_type = jag_ast_declaration_type_new(array_type, 1);
					JagDecIntermediateNewArray *result = jag_dec_intermediate_new_array_new(oppIndex, array_decl_type);
					cat_unref_ptr(array_decl_type);
					return (JagDecIIntermediateNode *) result;
				}
				case JAG_BYT_TYPE_REFERENCE : {
					JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);

					JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, jag_byt_op_new_array_get_class_info_pool_index(op_new_array)-1);
					if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
						JagAstDeclarationType *array_decl_type = jag_byt_constant_clazz_get_parsed_descriptor((JagBytConstantClazz *) iconstant);
						array_decl_type = jag_ast_declaration_type_set_dim_count(array_decl_type, 1); // TODO refcount ??
						JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
						jag_dec_import_repository_attach_declaration_type(import_repo, array_decl_type, TRUE);
						JagDecIntermediateNewArray *result = jag_dec_intermediate_new_array_new(oppIndex, array_decl_type);
						return (JagDecIIntermediateNode *) result;
					} else {
						cat_log_error("invalid constant for op new-array");
					}
				} break;
				case JAG_BYT_TYPE_VOID : {
					cat_log_error("can not create an array of type void");
				} break;
			}
		} break;
		case OP_ARRAYLENGTH : {
			return (JagDecIIntermediateNode *) jag_dec_intermediate_array_length_new(oppIndex);
		}
		case OP_ATHROW : {
			return (JagDecIIntermediateNode *) jag_dec_intermediate_throw_new(oppIndex);
		}
		case OP_CHECKCAST : {
			JagBytOpCheckCast *op_check_cast = JAG_BYT_OP_CHECK_CAST(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);
			int class_pool_index = jag_byt_op_check_cast_get_class_info_pool_index(op_check_cast);

			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, class_pool_index-1);
			if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
				JagAstDeclarationType *cast_declaration_type = jag_byt_constant_clazz_get_parsed_descriptor((JagBytConstantClazz *) iconstant);
				JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
				jag_dec_import_repository_attach_declaration_type(import_repo, cast_declaration_type, TRUE);
				JagDecIntermediateCast *result = jag_dec_intermediate_cast_new(oppIndex, cast_declaration_type);
				return (JagDecIIntermediateNode *) result;
			} else {
				cat_log_error("invalid constant for op check-cast");
			}
		} break;
//		case OP_INSTANCEOF : {
//			OpCheckCast checkCast = (OpCheckCast) mnemonic;
//			ConstantClassInfo constantClass = (ConstantClassInfo) typeDefinition.getConstantPool().getResolved(checkCast.poolIndex-1);
//			DeclarationType declDescRef = constantClass.getParsedDescriptor();
//			importRepository.attach(declDescRef);
//			return new IntermediateInstanceOf(mnemonicIndex, declDescRef);
//		}
		case OP_MONITORENTER : {
			JagDecIntermediateMonitorEnter *result = jag_dec_intermediate_monitor_enter_new();
			return (JagDecIIntermediateNode *) result;
		}
		case OP_MONITOREXIT : {
			JagDecIntermediateMonitorExit *result = jag_dec_intermediate_monitor_exit_new(oppIndex);
			return (JagDecIIntermediateNode *) result;
		}
////			case OP_WIDE : break;
		case OP_MULTIANEWARRAY : {
			JagBytOpMultiANewArray *op_multi_new_array = JAG_BYT_OP_MULTI_A_NEW_ARRAY(mnemonic);
			JagBytConstantPool *constant_pool = l_get_constant_pool(group_creator);
			int dimensions = jag_byt_op_multi_a_new_array_get_nr_of_dimensions(op_multi_new_array);
			int class_info_pool_index = jag_byt_op_multi_a_new_array_get_class_info_pool_index(op_multi_new_array);

			JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(constant_pool, class_info_pool_index-1);
			if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
				JagAstDeclarationType *array_declaration_type = jag_byt_constant_clazz_get_parsed_descriptor((JagBytConstantClazz *) iconstant);
				JagDecImportRepository *import_repo = JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(group_creator)->import_repository;
				jag_dec_import_repository_attach_declaration_type(import_repo, array_declaration_type, TRUE);
				JagDecIntermediateMultiNewArray *result = jag_dec_intermediate_multi_new_array_new(oppIndex, dimensions, array_declaration_type);
				return (JagDecIIntermediateNode *) result;
			} else {
				cat_log_error("invalid constant for op multi-anew_array");
			}
		} break;
////			case OP_JSR_W : break;
////			case OP_BREAKPOINT : break;
		case OP_NOP :
		case OP_POP :
		case OP_POP2 :
		case OP_SWAP :
		case OP_I2L :
		case OP_I2F :
		case OP_I2D :
		case OP_L2I :
		case OP_L2F :
		case OP_L2D :
		case OP_F2I :
		case OP_F2L :
		case OP_F2D :
		case OP_D2I :
		case OP_D2L :
		case OP_D2F :
		case OP_I2B :
		case OP_I2C :
		case OP_I2S :
		case OP_LCMP :
		case OP_FCMPL :
		case OP_FCMPG :
		case OP_DCMPL :
		case OP_DCMPG :
		case OP_JSR :
		case OP_RET :
		case OP_TABLESWITCH :
		case OP_LOOKUPSWITCH :
		case OP_INVOKEINTERFACE :
		case OP_INVOKEDYNAMIC :
		case OP_INSTANCEOF :
		case OP_WIDE :
		case OP_JSR_W :
		case OP_BREAKPOINT :
		case OP_IMPDEP1 :
		case OP_IMPDEP2 : {
			cat_log_error("Opcode not implemented oppIndex=%d, mnemonic_op=%d", oppIndex, mnemonic_op);
			break;
		}
	}
	return NULL;
}




