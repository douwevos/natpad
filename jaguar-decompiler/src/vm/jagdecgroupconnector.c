/*
   File:    jagdecgroupconnector.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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

#include "jagdecgroupconnector.h"
#include "jagdeccombineitem.h"
#include "jagdecintermediategroupcreator.h"
#include "intermediate/jagdecintermediatenodescope.h"
#include "intermediate/jagdeciintermediatenode.h"
#include "intermediate/jagdecintermediateloopgroup.h"
#include "intermediate/jagdecintermediateforloop.h"
#include "intermediate/item/jagdecintermediateifthenelse.h"
#include "intermediate/item/jagdecintermediateconditionpusher.h"
#include "intermediate/item/jagdecintermediateconditionset.h"
#include "intermediate/item/jagdecintermediateduplicate.h"
#include "intermediate/item/jagdecintermediateincrement.h"
#include "intermediate/item/jagdecintermediateconstant.h"
#include "intermediate/item/jagdecintermediategoto.h"
#include "intermediate/item/jagdecintermediatemathbasic.h"
#include "intermediate/item/jagdecintermediateframeget.h"
#include "intermediate/item/jagdecintermediateframeput.h"
#include "intermediate/item/jagdecintermediatethrow.h"
#include "intermediate/item/jagdecintermediatemonitorenter.h"
#include "intermediate/item/jagdecintermediatemonitorexit.h"
#include "intermediate/jagdecintermediatetrycatchgroup.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecGroupConnector"
#include <logging/catlog.h>

struct _JagDecGroupConnectorPrivate {
	JagDecIntermediateRunContext *connectContext;
	JagDecIntermediateGroup *model_container;
	JagDecImportRepository *import_repository;
	JagBytMnemonicBlock *mnemonic_block;
	JagBytLocalVariableTable *local_variable_table;
};

G_DEFINE_TYPE (JagDecGroupConnector, jag_dec_group_connector, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_group_connector_class_init(JagDecGroupConnectorClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecGroupConnectorPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

}

static void jag_dec_group_connector_init(JagDecGroupConnector *instance) {
	JagDecGroupConnectorPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_GROUP_CONNECTOR, JagDecGroupConnectorPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecGroupConnector *instance = JAG_DEC_GROUP_CONNECTOR(object);
	JagDecGroupConnectorPrivate *priv = instance->priv;
	cat_unref_ptr(priv->connectContext);
	cat_unref_ptr(priv->import_repository);
	cat_unref_ptr(priv->model_container);
	cat_unref_ptr(priv->local_variable_table);
	cat_unref_ptr(priv->mnemonic_block);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void l_convert_group(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *group_container);

JagDecGroupConnector *jag_dec_group_connector_new(JagDecImportRepository *import_repository, JagBytClassfile *classfile, JagBytMethod *method, JagBytMnemonicBlock *mnemonic_block, JagBytTryCatchList *try_catch_list) {
	JagDecGroupConnector *result = g_object_new(JAG_DEC_TYPE_GROUP_CONNECTOR, NULL);
	cat_ref_anounce(result);
	JagDecGroupConnectorPrivate *priv = result->priv;

	priv->import_repository = cat_ref_ptr(import_repository);
	priv->mnemonic_block = cat_ref_ptr(mnemonic_block);

	priv->local_variable_table = NULL;
	if (method!=NULL) {
		JagBytAttributeLocalVariableTable *attr_local_var_table = jag_byt_method_get_local_variable_table(method);
		if (attr_local_var_table!=NULL) {
			priv->local_variable_table = jag_byt_attribute_local_variable_table_get_table(attr_local_var_table);
			cat_ref_ptr(priv->local_variable_table);
		}
	}

	JagDecIntermediateGroupCreator *blockFactory = jag_dec_intermediate_group_creator_new(import_repository, classfile, method, mnemonic_block, try_catch_list);

	priv->model_container = jag_dec_intermediate_group_creator_get_main_container(blockFactory);
	cat_ref_ptr(priv->model_container);
	cat_log_debug("--------------------------------------------------------------------");
	jag_dec_intermediate_group_dump(priv->model_container, cat_string_wo_new_with("A:"));

//	groupContainer.dump("A:");
	l_convert_group(result, priv->model_container);
	cat_log_debug("--------------------------------------------------------------------");
	jag_dec_intermediate_group_dump(priv->model_container, cat_string_wo_new_with("B:"));
//	groupContainer.dump("B:");

	priv->connectContext = jag_dec_intermediate_run_context_new((JagBytIMnemonicBlock *) mnemonic_block, priv->local_variable_table);

	cat_unref_ptr(blockFactory);
	return result;
}



static void l_eliminate_duplicates(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);
static void l_detect_non_integer_increment_opps(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);

static gboolean l_trace_synchronized_blocks(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);
static gboolean l_trace_catch_blocks(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);
static gboolean l_trace_if_then_else_model(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);
static gboolean l_trace_and_condition_model(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);
static gboolean l_trace_loop(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);
static gboolean l_trace_for_loop(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer);
static int l_find_group_index_by_mnemonic_index(JagDecIntermediateGroup *modelContainer, int mnemonic_index);



void l_convert_group(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *group_container) {

	int round = 0;
	CatStringWo *e_round_buf = cat_string_wo_new();
	while(TRUE) {
		cat_string_wo_clear(e_round_buf);
		cat_string_wo_append_chars(e_round_buf, "round-");
		cat_string_wo_append_decimal(e_round_buf, round++);
		cat_string_wo_append_chars(e_round_buf, ":");
		e_round_buf = cat_string_wo_anchor(e_round_buf, 0);
		jag_dec_intermediate_group_dump(group_container, e_round_buf);

		l_eliminate_duplicates(group_connector, group_container);
		l_detect_non_integer_increment_opps(group_connector, group_container);
//
		if (l_trace_and_condition_model(group_connector, group_container)) {
			cat_log_debug("did trace and-condition-model");
			continue;
		}
		if (l_trace_loop(group_connector, group_container)) {
			cat_log_debug("did trace while-loop-statement");
			continue;
		}
		if (l_trace_for_loop(group_connector, group_container)) {
			cat_log_debug("did trace for-loop-statement");
			continue;
		}
////			if (traceBooleanCondition(groupContainer)) {
////				continue;
////			}
		if (l_trace_if_then_else_model(group_connector, group_container)) {
			continue;
		}
		if (l_trace_catch_blocks(group_connector, group_container)) {
			continue;
		}
		if (l_trace_synchronized_blocks(group_connector, group_container)) {
			continue;
		}
		break;
	}
	cat_unref_ptr(e_round_buf);
}

static JagDecIntermediateGroup *l_strip_combined(JagDecIntermediateGroup *groupTwo, JagDecIntermediateGroup *groupOne);
static CatHashSet *l_collect_forward(JagDecIntermediateGroup *groupContainer, JagDecIIntermediateNode *startGroup);



static void l_eliminate_duplicates(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {
//	JagDecGroupConnectorPrivate *priv = JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector);
	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *childGroup = jag_dec_intermediate_group_get(modelContainer, modelIndex);
		if (JAG_DEC_IS_INTERMEDIATE_GROUP(childGroup)) {
			l_eliminate_duplicates(group_connector, (JagDecIntermediateGroup *) childGroup);
			continue;
		}
		if (JAG_DEC_IS_INTERMEDIATE_DUPLICATE(childGroup)) {
			JagDecIntermediateDuplicate *grpDuplicate = (JagDecIntermediateDuplicate *) childGroup;
			JagBytOperation dup_type = jag_dec_intermediate_duplicate_get_duplicate_operator(grpDuplicate);
			// TODO: DUP2 its behavior depends on the type of the value pushed onto the stack. At this point we do not yet have a stack.
			if (dup_type==OP_DUP2) {

				/* detect           Duplicate.dup2
				 *                  FramePut x
				 * replace with:    FramePut x
				 *                  FrameGet x
				 */
				if (modelIndex+1<jag_dec_intermediate_group_count(modelContainer)) {
					JagDecIIntermediateNode *next = jag_dec_intermediate_group_get(modelContainer, modelIndex+1);
					if (JAG_DEC_IS_INTERMEDIATE_FRAME_PUT(next)) {
						int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) next);
						int frame_index = jag_dec_intermediate_frame_put_get_frame_index((JagDecIntermediateFramePut *) next);
						JagDecIntermediateFrameGet *replacementFrameGet = jag_dec_intermediate_frame_get_new(mnemonic_index, frame_index);
						JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
						cat_unref(removed);
						jag_dec_intermediate_group_add_at(modelContainer, modelIndex+1, (JagDecIIntermediateNode *) replacementFrameGet);
						cat_unref_ptr(replacementFrameGet);
						continue;
					}
				}

				/* detect           FrameGet x
				 *                  Duplicate.dup2
				 * replace with:    FrameGet x
				 *                  FrameGet x
				 */
				if (modelIndex>0) {
					JagDecIIntermediateNode *next = jag_dec_intermediate_group_get(modelContainer, modelIndex-1);
					if (JAG_DEC_IS_INTERMEDIATE_FRAME_GET(next)) {
						int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) grpDuplicate);
						int frame_index = jag_dec_intermediate_frame_get_get_frame_index((JagDecIntermediateFrameGet *) next);

						JagDecIntermediateFrameGet *replacementFrameGet = jag_dec_intermediate_frame_get_new(mnemonic_index, frame_index);
						jag_dec_intermediate_group_replace(modelContainer, modelIndex, (JagDecIIntermediateNode *) replacementFrameGet);
						cat_unref_ptr(replacementFrameGet);
					}
				}
			}
		}
	}
}





/* detects 'i++' where i is not an integer */
static void l_detect_non_integer_increment_opps(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {

	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *childGroup = jag_dec_intermediate_group_get(modelContainer, modelIndex);
		if (JAG_DEC_IS_INTERMEDIATE_GROUP(childGroup)) {
			l_detect_non_integer_increment_opps(group_connector, (JagDecIntermediateGroup *) childGroup);
			continue;
		}

		if (JAG_DEC_IS_INTERMEDIATE_FRAME_GET(childGroup)) {
			if (modelIndex+3<jag_dec_intermediate_group_count(modelContainer)) {
				JagDecIIntermediateNode *node1 = jag_dec_intermediate_group_get(modelContainer, modelIndex+1);
				JagDecIIntermediateNode *node2 = jag_dec_intermediate_group_get(modelContainer, modelIndex+2);
				JagDecIIntermediateNode *node3 = jag_dec_intermediate_group_get(modelContainer, modelIndex+3);
				if (JAG_DEC_IS_INTERMEDIATE_CONSTANT(node1) &&
						JAG_DEC_IS_INTERMEDIATE_MATH_BASIC(node2) &&
						JAG_DEC_IS_INTERMEDIATE_FRAME_PUT(node3)) {

					JagDecIntermediateFrameGet *grpFrameGet = (JagDecIntermediateFrameGet *) childGroup;
					JagDecIntermediateConstant *grpConstant = (JagDecIntermediateConstant *) node1;
					JagDecIntermediateMathBasic *grpMathBasic = (JagDecIntermediateMathBasic *) node2;
					JagDecIntermediateFramePut *grpFramePut = (JagDecIntermediateFramePut *) node3;

					int frame_index = jag_dec_intermediate_frame_get_get_frame_index(grpFrameGet);
					if (frame_index!=jag_dec_intermediate_frame_put_get_frame_index(grpFramePut)) {
						continue;
					}


					/* is ADD or SUB */
					gboolean isIncrease = TRUE;
					JagBytMathOperator math_op = jag_dec_intermediate_math_basic_get_operator(grpMathBasic);
					if (math_op==JAG_BYT_MATH_OPERATOR_SUB) {
						isIncrease = FALSE;
					} else if (math_op!=JAG_BYT_MATH_OPERATOR_ADD) {
						continue;
					}

					JagAstIExpression *grpConstantExpr = jag_dec_intermediate_constant_get_value_expression(grpConstant);
					/* check if constant is 1 */
					if (JAG_AST_IS_LITERAL(grpConstantExpr)) {
						JagAstLiteral *literal = (JagAstLiteral *) grpConstantExpr;
						CatIStringable *string_val = jag_ast_literal_get_value(literal);
						if (CAT_IS_LONG(string_val)) {
							if (cat_long_value((CatLong *) string_val)!=1) {
								continue;
							}
						} else if (CAT_IS_INTEGER(string_val)) {
							if (cat_integer_value((CatInteger *) string_val)!=1) {
								continue;
							}
						} else if (CAT_IS_FLOAT(string_val)) {
							if (cat_float_value((CatFloat *) string_val)!=1.0) {
								continue;
							}
						} else {
							continue;
						}
					} else {
						continue;
					}



					/* replace four */
					JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
					cat_unref_ptr(removed);
					removed = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
					cat_unref_ptr(removed);
					removed = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
					cat_unref_ptr(removed);
					removed = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
					cat_unref_ptr(removed);

					JagDecIntermediateIncrement *grpIncrement = jag_dec_intermediate_increment_new(-1, frame_index, isIncrease ? 1 : -1);
					jag_dec_intermediate_group_add_at(modelContainer, modelIndex, (JagDecIIntermediateNode *) grpIncrement);
					cat_unref_ptr(grpIncrement);

				}
			}
		}
	}
}


static gboolean l_trace_synchronized_blocks(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {
	JagDecGroupConnectorPrivate *priv = JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector);
	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *childGroup = jag_dec_intermediate_group_get(modelContainer, modelIndex);
		if (JAG_DEC_IS_INTERMEDIATE_GROUP(childGroup)) {
			JagDecIntermediateGroup *grp = (JagDecIntermediateGroup *) childGroup;
			JagDecIIntermediateNode *grpLast = jag_dec_intermediate_group_get_last(grp);
			if (JAG_DEC_IS_INTERMEDIATE_MONITOR_ENTER(grpLast)) {
				JagDecIntermediateMonitorEnter *monitorGroup = (JagDecIntermediateMonitorEnter *) grpLast;
				if (!jag_dec_intermediate_monitor_enter_is_resolved(monitorGroup)) {
					cat_log_detail("monitorGroup=%p",monitorGroup);
					if (modelIndex+1<jag_dec_intermediate_group_count(modelContainer)) {
						JagDecIIntermediateNode *monitorBlockNode = jag_dec_intermediate_group_get(modelContainer, modelIndex+1);
						cat_log_detail("monitorBlockNode=%p", monitorBlockNode);
						if (JAG_DEC_IS_INTERMEDIATE_TRY_CATCH_GROUP(monitorBlockNode)) {
							JagDecIntermediateTryCatchGroup *tryCatchGroup = (JagDecIntermediateTryCatchGroup *) monitorBlockNode;
							JagDecIntermediateGroup *finallyGroup = (JagDecIntermediateGroup *) jag_dec_intermediate_try_catch_group_get_finally_group(tryCatchGroup);
							JagDecIntermediateGroup *subGroup = (JagDecIntermediateGroup *) jag_dec_intermediate_group_get_last(finallyGroup);
							JagDecIIntermediateNode *last = jag_dec_intermediate_group_get_last(subGroup);

							cat_log_detail("last=%p", last);
							if (JAG_DEC_IS_INTERMEDIATE_THROW(last)) {
								JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(subGroup, jag_dec_intermediate_group_count(subGroup)-1);
								cat_unref(removed);
							}


							last = jag_dec_intermediate_group_get_last(subGroup);
							if (JAG_DEC_IS_INTERMEDIATE_MONITOR_EXIT(last)) {
								JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(subGroup, jag_dec_intermediate_group_count(subGroup)-1);
								cat_unref(removed);
							}

							last = jag_dec_intermediate_group_get_last(subGroup);
							if (JAG_DEC_IS_INTERMEDIATE_FRAME_GET(last)) {
								JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(subGroup, jag_dec_intermediate_group_count(subGroup)-1);
								cat_unref(removed);
							}



							if (jag_dec_intermediate_group_count(subGroup)==0) {
								JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(finallyGroup, jag_dec_intermediate_group_count(finallyGroup)-1);
								cat_unref(removed);
							}

							if (jag_dec_intermediate_try_catch_group_is_stale(tryCatchGroup)) {
								last = jag_dec_intermediate_group_get_last((JagDecIntermediateGroup *) tryCatchGroup);
								JagDecIIntermediateNode *replacement = jag_dec_iintermediate_node_make_copy(last, FALSE);
								JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex+1);
								cat_unref(removed);

								jag_dec_intermediate_group_add((JagDecIntermediateGroup *) monitorGroup, replacement);
								cat_unref_ptr(replacement);
							}

						} else {
							cat_log_error("expected a try-catch-finally-group following monitorenter:");
//							monitorGroup.dump("ERR:");
						}
					} else {
						cat_log_error("nothing following monitorenter:");
//						monitorGroup.dump("ERR:");
					}
					jag_dec_intermediate_monitor_enter_set_resolved(monitorGroup, TRUE);
					JagDecIntermediateRunContext *groupConnectRuntime = jag_dec_intermediate_run_context_new((JagBytIMnemonicBlock *) priv->mnemonic_block, priv->local_variable_table);
					jag_dec_iintermediate_node_connect_stage_one((JagDecIIntermediateNode *) monitorGroup, groupConnectRuntime);
					l_convert_group(group_connector, (JagDecIntermediateGroup *) monitorGroup);

					cat_unref_ptr(groupConnectRuntime);
//					groupContainer.dump("JJ:");

					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


static int l_pointer_hash(gconstpointer p) {
	long v = (long) p;
	if (sizeof(v)==4) {
		return (int) v;
	}
	return ((int) v)+(int) (v>>32);
}

static gboolean l_pointer_equal(gconstpointer a, gconstpointer b) {
	return a==b;
}

static gboolean l_trace_catch_blocks(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {
	JagDecGroupConnectorPrivate *priv = JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector);
	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *childGroup = jag_dec_intermediate_group_get(modelContainer, modelIndex);
		if (JAG_DEC_IS_INTERMEDIATE_TRY_CATCH_GROUP(childGroup)) {
			JagDecIntermediateTryCatchGroup *tryCatchGroup = (JagDecIntermediateTryCatchGroup *) childGroup;
			if (jag_dec_intermediate_try_catch_group_are_catch_blocks_resolved(tryCatchGroup)) {
				continue;
			}
			JagBytTryCatch *tryCatch = jag_dec_intermediate_try_catch_group_get_try_catch(tryCatchGroup);
			CatHashSet *mainSet = l_collect_forward(modelContainer, (JagDecIIntermediateNode *) tryCatchGroup);
			cat_log_trace("resolving try-catch-block:tryCatchGroup=%p, forward-set=%d", tryCatchGroup, cat_hash_set_size(mainSet));
			CatArrayWo *e_main_list = cat_hash_set_enlist_all(mainSet);
			cat_unref_ptr(mainSet);
//			Set<IIntermediateNode> mainSet = collectForward(groupContainer, tryCatchGroup);
			CatIIterator *iter = jag_byt_try_catch_iterator(tryCatch);
			while(cat_iiterator_has_next(iter)) {
				JagBytICatchOrFinally *catchItem = (JagBytICatchOrFinally *) cat_iiterator_next(iter);
				int handleBytecodePc = jag_byt_icatch_or_finally_get_handle_pc(catchItem);
				int handlePc = jag_byt_mnemonic_block_find_by_bytecode_offset(priv->mnemonic_block, handleBytecodePc);
				JagDecIIntermediateNode *findNode = jag_dec_intermediate_group_find_node(modelContainer, handlePc);
				if (findNode ==NULL) {
					cat_log_warn("%p findnode is NULL", catchItem);
					continue;
				}
				JagDecIntermediateGroup *catchOrFinallyGroup = NULL;
				if (JAG_BYT_IS_CATCH(catchItem)) {
					JagAstDeclarationType *catch_exception_type = jag_byt_catch_get_exception_type((JagBytCatch *) catchItem);
					catchOrFinallyGroup = (JagDecIntermediateGroup *) jag_dec_intermediate_try_catch_group_find_catch_group(tryCatchGroup, catch_exception_type);
				} else {
					catchOrFinallyGroup = (JagDecIntermediateGroup *) jag_dec_intermediate_try_catch_group_get_finally_group(tryCatchGroup);
				}

				if (catchOrFinallyGroup==NULL) {
					cat_log_warn("no group found :::%p, tryCatchGroup=%p",catchItem, tryCatchGroup);
//					tryCatchGroup.dump("---");
					continue;
				}

				jag_dec_intermediate_group_add(catchOrFinallyGroup, findNode);
				CatHashSet *inSet = cat_hash_set_new((GHashFunc) l_pointer_hash, (GEqualFunc) l_pointer_equal);
				cat_hash_set_add_all_from_list(inSet, e_main_list);

				cat_hash_set_add(inSet, (GObject *) findNode);
				CatArrayWo *e_stack = cat_array_wo_new();
				cat_array_wo_append(e_stack, (GObject *) findNode);
				while(cat_array_wo_size(e_stack)>0) {
					JagDecIIntermediateNode *groupNode = NULL;
					cat_array_wo_remove_last(e_stack, (GObject **) (&groupNode));

//					cat_log_detail("groupNode.class="+groupNode.getClass().getName());
					JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref(groupNode);
//					log.detail("scope="+scope);
					int exitIndex = jag_dec_intermediate_node_scope_get_exit_index(scope);
					if (exitIndex>=0) {
						JagDecIIntermediateNode *node = jag_dec_intermediate_group_find_node(modelContainer, exitIndex);
						if ((node!=NULL) && (cat_hash_set_get(inSet, (GObject *) node)==NULL)) {
							if (jag_dec_intermediate_group_add(catchOrFinallyGroup, node)) {
								cat_array_wo_append(e_stack, (GObject *) node);
							}
						}
					}
					int jumpIndex = jag_dec_intermediate_node_scope_get_jump_index(scope);
					if (jumpIndex>=0) {
						JagDecIIntermediateNode *node = jag_dec_intermediate_group_find_node(modelContainer, jumpIndex);
						if ((node!=NULL) && (cat_hash_set_get(inSet, (GObject *) node)==NULL)) {
							if (jag_dec_intermediate_group_add(catchOrFinallyGroup, node)) {
								cat_array_wo_append(e_stack, (GObject *) node);
							}
						}
					}
					cat_unref_ptr(scope);
					cat_unref_ptr(groupNode);
				}
				cat_unref_ptr(e_stack);
				cat_unref_ptr(inSet);
				int ridx;
				for(ridx=jag_dec_intermediate_group_count(catchOrFinallyGroup); ridx>=0; ridx--) {
					JagDecIIntermediateNode *rnode = jag_dec_intermediate_group_get(catchOrFinallyGroup, ridx);
					jag_dec_intermediate_group_remove(modelContainer, rnode);
				}
			}
			cat_unref_ptr(iter);


			l_convert_group(group_connector, (JagDecIntermediateGroup *) tryCatchGroup);


			/* match code from final block with continuation block */
			JagDecIntermediateNodeScope *tcscope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) tryCatchGroup);
			int continuationIndex = jag_dec_intermediate_node_scope_get_exit_index(tcscope);
			cat_unref_ptr(tcscope);
			cat_log_trace("continuationIndex=%d", continuationIndex);
			JagDecIIntermediateNode *foundContinuationNode = jag_dec_intermediate_group_find_node(modelContainer, continuationIndex);

//			if (log.logEnabbledFor(LogLevel.DETAIL)) {
//				StringBuilder buf = new StringBuilder();
//				foundContinuationNode.dump(buf, "C::");
//				log.detail(buf.toString());
//			}



			if (JAG_DEC_IS_INTERMEDIATE_GROUP(foundContinuationNode)) {
				JagDecIntermediateGroup *continuationGroup = (JagDecIntermediateGroup *) foundContinuationNode;

				int cntGrpCount = jag_dec_intermediate_group_count(continuationGroup);

				JagDecIntermediateGroup *finallyGroup = (JagDecIntermediateGroup *) jag_dec_intermediate_try_catch_group_get_finally_group(tryCatchGroup);
				if (finallyGroup!=NULL) {
					JagDecIIntermediateNode *finGrpNode = jag_dec_intermediate_group_get(finallyGroup, 0);
					if (JAG_DEC_IS_INTERMEDIATE_GROUP(finGrpNode)) {
						JagDecIntermediateGroup *finGrp = (JagDecIntermediateGroup *) finGrpNode;
						int finGrpCount = jag_dec_intermediate_group_count(finGrp);
//						finGrp.dump("D::");

						int fidx = 0;
						int fstart = -1;
						int midx=0;
						for(midx=0; midx<cntGrpCount && fidx<finGrpCount; midx++) {
							JagDecIIntermediateNode *continuationSubNode = jag_dec_intermediate_group_get(continuationGroup, midx);
							if (midx==0) {
								while(fidx<finGrpCount) {
									JagDecIIntermediateNode *finSubNode = jag_dec_intermediate_group_get(finGrp, fidx);

									fidx++;
									gboolean same = jag_dec_iintermediate_node_is_same(finSubNode, continuationSubNode);
									cat_log_trace("same=%d, first match:%p, with=%p", same, finSubNode, continuationSubNode);
									if (same) {
										fstart = fidx;
										break;
									}
								}
							} else {
								JagDecIIntermediateNode *finSubNode = jag_dec_intermediate_group_get(finGrp, fidx);
								fidx++;
								if (!jag_dec_iintermediate_node_is_same(finSubNode, continuationSubNode)) {
									break;
								}
							}
						}
						if (fstart>0 && midx>0) {
							while(midx>0) {
								JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(continuationGroup, 0);
								cat_unref(removed);
								midx--;
							}
						}
					}
				}
			}

			jag_dec_intermediate_try_catch_group_set_catch_blocks_resolved(tryCatchGroup, TRUE);
			cat_unref_ptr(e_main_list);
			return TRUE;
		}
	}
	return FALSE;
}




static CatHashSet *l_collect_forward(JagDecIntermediateGroup *groupContainer, JagDecIIntermediateNode *startGroup) {
	CatArrayWo *e_stack = cat_array_wo_new();
	cat_array_wo_append(e_stack, (GObject *) startGroup);
	CatHashSet *result = cat_hash_set_new((GHashFunc) l_pointer_hash, (GEqualFunc) l_pointer_equal);
	cat_hash_set_add(result, (GObject *) startGroup);
	while(cat_array_wo_size(e_stack)>0) {
		JagDecIIntermediateNode *group = NULL;
		cat_array_wo_remove_last(e_stack, (GObject **) (&group));
		cat_log_debug("group=%p %s", group, g_type_name_from_instance((GTypeInstance *) group));
		JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref(group);
		int exitIndex = jag_dec_intermediate_node_scope_get_exit_index(scope);
		if (exitIndex>=0) {
			JagDecIIntermediateNode *node = jag_dec_intermediate_group_find_node(groupContainer, exitIndex);
			cat_log_debug("node=%p, exitIndex=%d", node, exitIndex);
			if (node!=NULL) {
				if (cat_hash_set_add(result, (GObject *) node)==NULL) {
					cat_array_wo_append(e_stack, (GObject *) node);
				}
			}
		}
		int jumpIndex = jag_dec_intermediate_node_scope_get_jump_index(scope);
		cat_unref_ptr(scope);
		if (jumpIndex>=0) {
			JagDecIIntermediateNode *node = jag_dec_intermediate_group_find_node(groupContainer, jumpIndex);
			cat_log_debug("node=%p, jump_index=%d", node, jumpIndex);
			if (node!=NULL) {
				if (cat_hash_set_add(result, (GObject *) node)==NULL) {
					cat_array_wo_append(e_stack, (GObject *) node);
				}
			}
		}
		cat_unref_ptr(group);
	}
	cat_unref_ptr(e_stack);
	return result;
}


static gboolean l_trace_if_then_else_model(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {
	JagDecGroupConnectorPrivate *priv = JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector);
	gboolean changed = FALSE;
	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *nodeCond = jag_dec_intermediate_group_get(modelContainer, modelIndex);
		JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref(nodeCond);
		int condJumpIndex = jag_dec_intermediate_node_scope_get_jump_index(scope);
		int condExitIndex = jag_dec_intermediate_node_scope_get_exit_index(scope);
		cat_unref_ptr(scope);
		cat_log_debug("modelIndex=%d, condJumpIndex=%d, condExitIndex=%d", modelIndex, condJumpIndex, condExitIndex);
		if (condJumpIndex<0 || condExitIndex<0) {
			continue;
		}


		if (condExitIndex>=condJumpIndex) {
			// this is a conditional jump backwards ... thus it should be resolved as a loop
			continue;
		}


		int nextThenIndex = condExitIndex;
		JagDecIntermediateGroup *thenGroup = jag_dec_intermediate_group_new();
		int j=0;
		while(j++<100) {
			JagDecIIntermediateNode *findNode = jag_dec_intermediate_group_find_node(modelContainer, nextThenIndex);
			if (findNode==NULL) {
				cat_log_warn("WARNING: no node fount at next-then-index:%d", nextThenIndex);
				cat_unref_ptr(thenGroup);
				break;
			}

			jag_dec_intermediate_group_add(thenGroup, findNode);
			JagDecIntermediateNodeScope *findScope = jag_dec_iintermediate_node_get_scope_ref(findNode);

			nextThenIndex = jag_dec_intermediate_node_scope_get_out_index(findScope);
			cat_unref_ptr(findScope);
			if (nextThenIndex==JAG_DEC_SCOPE_INDEX_NONE) {
				cat_unref_ptr(thenGroup);
				break;
			}
			if ((nextThenIndex>=0 && nextThenIndex>=condJumpIndex) || nextThenIndex==JAG_DEC_SCOPE_INDEX_EXIT){
				break;
			}
		}
		if (j>=100) {
			cat_log_error("j >= 100");
			break;
		}

		if (thenGroup==NULL || jag_dec_intermediate_group_count(thenGroup)==0) {
			cat_unref_ptr(thenGroup);
			continue;
		}

		if (nextThenIndex==condJumpIndex) {
			/* this is a if-then-group (so there is no else-group) */
			JagDecIntermediateIfThenElse *ifThenElse = jag_dec_intermediate_if_then_else_new(nodeCond, (JagDecIIntermediateNode *) thenGroup, NULL);
			jag_dec_intermediate_group_remove(modelContainer, nodeCond);
			int gidx;
			for(gidx=jag_dec_intermediate_group_count(thenGroup)-1; gidx>=0; gidx--) {
				JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(thenGroup, gidx);
				jag_dec_intermediate_group_remove(modelContainer, subNode);
			}
			jag_dec_intermediate_group_add_at(modelContainer, modelIndex, (JagDecIIntermediateNode *) ifThenElse);
			cat_unref(ifThenElse);
			cat_unref_ptr(thenGroup);
			return TRUE;
		}


		int nextElseIndex = condJumpIndex;
		JagDecIntermediateGroup *elseGroup = jag_dec_intermediate_group_new();
		while(TRUE) {
			JagDecIIntermediateNode *findNode = jag_dec_intermediate_group_find_node(modelContainer, nextElseIndex);
			if (findNode==NULL) {
				cat_log_warn("no node fount at next-else-index:%d", nextElseIndex);
				cat_unref_ptr(elseGroup);
				break;
			}
			jag_dec_intermediate_group_add(elseGroup, findNode);
			JagDecIntermediateNodeScope *findScope = jag_dec_iintermediate_node_get_scope_ref(findNode);

			nextElseIndex = jag_dec_intermediate_node_scope_get_out_index(findScope);
			cat_unref_ptr(findScope);
			if (nextElseIndex==JAG_DEC_SCOPE_INDEX_NONE) {
				cat_unref_ptr(elseGroup);
				break;
			}
			if ((nextElseIndex>=0 && nextElseIndex>=nextThenIndex && nextThenIndex>=0) || nextElseIndex==JAG_DEC_SCOPE_INDEX_EXIT) {
				break;
			}
		}

		cat_log_debug("nextThenIndex=%d, nextElseIndex=%d", nextThenIndex, nextElseIndex);
		if (nextThenIndex==JAG_DEC_SCOPE_INDEX_EXIT && nextElseIndex==JAG_DEC_SCOPE_INDEX_EXIT) {
			/* this is a if-then-group (so there is no else-group) */

			JagDecIntermediateGroup *thenGroupCopy = (JagDecIntermediateGroup *) jag_dec_iintermediate_node_make_copy((JagDecIIntermediateNode *) thenGroup, FALSE);
			JagDecIntermediateGroup *elseGroupCopy = (JagDecIntermediateGroup *) jag_dec_iintermediate_node_make_copy((JagDecIIntermediateNode *) elseGroup, FALSE);

			JagDecIntermediateGroup *combinedGroup = l_strip_combined(thenGroupCopy, elseGroupCopy);


			JagDecIntermediateRunContext *thenContext = jag_dec_intermediate_run_context_new((JagBytIMnemonicBlock *) priv->mnemonic_block, priv->local_variable_table);
			jag_dec_iintermediate_node_connect_stage_one((JagDecIIntermediateNode *) thenGroupCopy, thenContext);

			JagDecIntermediateRunContext *elseContext = jag_dec_intermediate_run_context_new((JagBytIMnemonicBlock *) priv->mnemonic_block, priv->local_variable_table);
			jag_dec_iintermediate_node_connect_stage_one((JagDecIIntermediateNode *) elseGroupCopy, elseContext);

			JagDecIIntermediateNode *newNode = NULL;
			CatArrayWo *e_then_prod_stack = jag_dec_intermediate_run_context_get_producer_stack(thenContext);
			CatArrayWo *e_else_prod_stack = jag_dec_intermediate_run_context_get_producer_stack(elseContext);
			if (cat_array_wo_size(e_then_prod_stack)==1 && cat_array_wo_size(e_else_prod_stack)==1) {
				JagAstDeclarationType *boolean_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_BOOLEAN, 0);
				JagDecIValueProducer *valueProducer = (JagDecIValueProducer *) cat_array_wo_get_first(e_then_prod_stack);
				JagAstIExpression *thenExpr = jag_dec_ivalue_producer_get_expression_ref(valueProducer, boolean_decl_type);

				valueProducer = (JagDecIValueProducer *) cat_array_wo_get_first(e_else_prod_stack);
				JagAstIExpression *elseExpr = jag_dec_ivalue_producer_get_expression_ref(valueProducer, boolean_decl_type);
				cat_unref_ptr(boolean_decl_type);

				cat_log_debug("thenExpr=%o, elseExpr=%o", thenExpr, elseExpr);

				if (JAG_AST_IS_LITERAL(thenExpr) && JAG_AST_IS_LITERAL(elseExpr)) {
					gboolean isPusher = TRUE;
					JagAstLiteral *thenLit = JAG_AST_LITERAL(thenExpr);
					JagAstLiteral *elseLit = JAG_AST_LITERAL(elseExpr);


					CatIStringable *lit = jag_ast_literal_get_value(thenLit);
					cat_log_debug("thenLit=%o : %s", lit, g_type_name_from_instance((GTypeInstance *) lit));
					if (!CAT_IS_BOOLEAN(lit)) {
						isPusher = FALSE;
					}


					lit = jag_ast_literal_get_value(elseLit);
					cat_log_debug("elseLit=%o : %s", lit, g_type_name_from_instance((GTypeInstance *) lit));
					gboolean elseIsTrue = FALSE;
					if (!CAT_IS_BOOLEAN(lit)) {
						isPusher = FALSE;
					} else {
						elseIsTrue = cat_boolean_value((CAT_BOOLEAN(lit)));
					}



					if (isPusher) {
						newNode = (JagDecIIntermediateNode *) jag_dec_intermediate_condition_pusher_new(nodeCond, (JagDecIIntermediateNode *) thenGroupCopy, (JagDecIIntermediateNode *) elseGroupCopy, elseIsTrue);
					}
					cat_log_debug("isPusher=%d, newNode=%o", isPusher, newNode);

				}
				cat_unref_ptr(thenExpr);
				cat_unref_ptr(elseExpr);
			}

			if (newNode==NULL) {
				newNode = (JagDecIIntermediateNode *) jag_dec_intermediate_if_then_else_new(nodeCond, (JagDecIIntermediateNode *) thenGroupCopy, (JagDecIIntermediateNode *) elseGroupCopy);
			}

			cat_unref_ptr(thenContext);
			cat_unref_ptr(elseContext);

			int gidx;
			for(gidx=jag_dec_intermediate_group_count(thenGroup)-1; gidx>=0; gidx--) {
				JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(thenGroup, gidx);
				jag_dec_intermediate_group_remove(modelContainer, subNode);
			}
			for(gidx=jag_dec_intermediate_group_count(elseGroup)-1; gidx>=0; gidx--) {
				JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(elseGroup, gidx);
				jag_dec_intermediate_group_remove(modelContainer, subNode);
			}


			int findIndex = jag_dec_intermediate_group_find_index(modelContainer, nodeCond);
			JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, findIndex);
			cat_unref(removed);
			if (combinedGroup!=NULL && jag_dec_intermediate_group_count(combinedGroup)>0) {
				jag_dec_intermediate_group_add_at(modelContainer, findIndex, (JagDecIIntermediateNode *) combinedGroup);
			}
			jag_dec_intermediate_group_add_at(modelContainer, findIndex, newNode);
			cat_unref_ptr(newNode);
			cat_unref_ptr(thenGroupCopy);
			cat_unref_ptr(elseGroupCopy);
			cat_unref_ptr(combinedGroup);
			cat_unref_ptr(thenGroup);
			cat_unref_ptr(elseGroup);
			return TRUE;
		} else if (nextElseIndex==nextThenIndex) {
			JagDecIntermediateIfThenElse *ifThenElse = jag_dec_intermediate_if_then_else_new(nodeCond, (JagDecIIntermediateNode *) thenGroup, (JagDecIIntermediateNode *) elseGroup);

			int gidx;
			for(gidx=jag_dec_intermediate_group_count(thenGroup)-1; gidx>=0; gidx--) {
				JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(thenGroup, gidx);
				jag_dec_intermediate_group_remove(modelContainer, subNode);
			}
			for(gidx=jag_dec_intermediate_group_count(elseGroup)-1; gidx>=0; gidx--) {
				JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(elseGroup, gidx);
				jag_dec_intermediate_group_remove(modelContainer, subNode);
			}

			int findIndex = jag_dec_intermediate_group_find_index(modelContainer, nodeCond);
			JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, findIndex);
			cat_unref(removed);
			jag_dec_intermediate_group_add_at(modelContainer, findIndex, (JagDecIIntermediateNode *) ifThenElse);
			cat_unref_ptr(ifThenElse);
			cat_unref_ptr(thenGroup);
			cat_unref_ptr(elseGroup);
			return TRUE;
		} else {
			if (elseGroup==NULL || jag_dec_intermediate_group_count(elseGroup)==0) {
				/* this is a if-then-group (so there is no else-group) */
				JagDecIntermediateIfThenElse *ifThenElse = jag_dec_intermediate_if_then_else_new(nodeCond, (JagDecIIntermediateNode *) thenGroup, NULL);
				int gidx;
				for(gidx=jag_dec_intermediate_group_count(thenGroup)-1; gidx>=0; gidx--) {
					JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(thenGroup, gidx);
					jag_dec_intermediate_group_remove(modelContainer, subNode);
				}
				int findIndex = jag_dec_intermediate_group_find_index(modelContainer, nodeCond);
				JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, findIndex);
				cat_unref(removed);
				jag_dec_intermediate_group_add_at(modelContainer, findIndex, (JagDecIIntermediateNode *) ifThenElse);
				cat_unref_ptr(ifThenElse);
				cat_unref_ptr(thenGroup);
				cat_unref_ptr(elseGroup);
				return TRUE;
			}


			JagDecIntermediateIfThenElse *ifThenElse = jag_dec_intermediate_if_then_else_new(nodeCond, (JagDecIIntermediateNode *) thenGroup, (JagDecIIntermediateNode *) elseGroup);
			int gidx;
			for(gidx=jag_dec_intermediate_group_count(thenGroup)-1; gidx>=0; gidx--) {
				JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(thenGroup, gidx);
				jag_dec_intermediate_group_remove(modelContainer, subNode);
			}
			for(gidx=jag_dec_intermediate_group_count(elseGroup)-1; gidx>=0; gidx--) {
				JagDecIIntermediateNode *subNode = jag_dec_intermediate_group_get(elseGroup, gidx);
				jag_dec_intermediate_group_remove(modelContainer, subNode);
			}

			int findIndex = jag_dec_intermediate_group_find_index(modelContainer, nodeCond);
			JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, findIndex);
			cat_unref(removed);
			jag_dec_intermediate_group_add_at(modelContainer, findIndex, (JagDecIIntermediateNode *) ifThenElse);
			cat_unref_ptr(ifThenElse);
			cat_unref_ptr(thenGroup);
			cat_unref_ptr(elseGroup);
			return TRUE;
		}
		cat_unref_ptr(thenGroup);
	}
	return changed;
}

static JagDecIntermediateGroup *l_strip_combined(JagDecIntermediateGroup *groupTwo, JagDecIntermediateGroup *groupOne) {
	if (jag_dec_intermediate_group_count(groupOne)!=jag_dec_intermediate_group_count(groupTwo)) {
		return NULL;
	}
	JagDecCombineItem *combineItem = jag_dec_combine_item_new();
	combineItem->groupOne = cat_ref_ptr(groupOne);
	combineItem->groupTwo = cat_ref_ptr(groupTwo);

	CatArrayWo *e_stack = cat_array_wo_new();
	JagDecIntermediateGroup *result = NULL;
	while(TRUE) {
		if (combineItem->lastIndex==-1 || combineItem->doBreak) {
			if (cat_array_wo_size(e_stack)==0) {
				cat_unref_ptr(e_stack);
				result = combineItem->output;
				combineItem->output = NULL;
				break;

			}
			JagDecCombineItem *lastCombineItem = NULL;
			cat_array_wo_remove_last(e_stack, (GObject **) (&lastCombineItem));
			lastCombineItem->doBreak = combineItem->doBreak;
			jag_dec_combine_item_prepend(lastCombineItem, combineItem->output);
			cat_unref_ptr(combineItem);
			combineItem = lastCombineItem;
		} else {
			JagDecIIntermediateNode *nodeOne = jag_dec_intermediate_group_get(combineItem->groupOne, combineItem->lastIndex);
			JagDecIIntermediateNode *nodeTwo = jag_dec_intermediate_group_get(combineItem->groupTwo, combineItem->lastIndex);
			if (JAG_DEC_IS_INTERMEDIATE_GROUP(nodeOne) && JAG_DEC_IS_INTERMEDIATE_GROUP(nodeTwo)) {
				JagDecCombineItem *newCombineItem = jag_dec_combine_item_new();
				newCombineItem->groupOne = cat_ref_ptr(nodeOne);
				newCombineItem->groupTwo = cat_ref_ptr(nodeTwo);
				if (jag_dec_intermediate_group_count(newCombineItem->groupOne)==jag_dec_intermediate_group_count(newCombineItem->groupTwo)) {
					cat_array_wo_append(e_stack, (GObject *) combineItem);
					cat_unref(combineItem);
					newCombineItem->lastIndex = jag_dec_intermediate_group_count(newCombineItem->groupOne);
					combineItem = newCombineItem;
				} else {
					combineItem->doBreak = TRUE;
					cat_unref_ptr(newCombineItem);
				}
			} else {
				if (jag_dec_iintermediate_node_is_same(nodeOne, nodeTwo)) {
					JagDecIIntermediateNode *removedOne = jag_dec_intermediate_group_remove_at(combineItem->groupOne, combineItem->lastIndex);
					JagDecIIntermediateNode *removedTwo = jag_dec_intermediate_group_remove_at(combineItem->groupTwo, combineItem->lastIndex);
					jag_dec_combine_item_prepend_combined(combineItem, nodeOne, nodeTwo);
					cat_unref(removedOne);
					cat_unref(removedTwo);
				} else {
					combineItem->doBreak = TRUE;
					combineItem->lastIndex = 0;
				}
			}
		}
		combineItem->lastIndex--;
	}
	cat_unref_ptr(combineItem);
	cat_unref_ptr(e_stack);
	return result;
}




static gboolean l_trace_and_condition_model(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {
//	JagDecGroupConnectorPrivate *priv = JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector);

	gboolean changed = FALSE;

	int startIndex = -1;
	int jumpIndex = -1;
	gboolean keep = TRUE;
	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *group = jag_dec_intermediate_group_get(modelContainer, modelIndex);
//		log.trace("running group:"+group);
		keep = FALSE;
		JagDecIntermediateNodeScope *child_scope = jag_dec_iintermediate_node_get_scope_ref(group);
		int child_jump_index = jag_dec_intermediate_node_scope_get_jump_index(child_scope);
		int child_exit_index = jag_dec_intermediate_node_scope_get_exit_index(child_scope);
		cat_unref_ptr(child_scope);
		cat_log_debug("modelIndex=%d, child_jump_index=%d, child_exit_index=%d", modelIndex, child_jump_index, child_exit_index);
		if ((child_jump_index!=-1) && (child_exit_index!=-1)) {
			if (startIndex == -1) {
				startIndex = modelIndex;
				jumpIndex = child_jump_index;
				keep = TRUE;
			} else {
				if (jumpIndex == child_jump_index || jumpIndex == child_exit_index) {
					keep = TRUE;
				}
			}
		}

		cat_log_debug("modelIndex=%d, keep=%d, stat-index=%d, jump-index=%d", modelIndex, keep, startIndex, jumpIndex);

		if (!keep && (startIndex!=-1)) {
			if (modelIndex<=startIndex+1) {
				modelIndex = startIndex;
				startIndex = -1;
				cat_log_trace("continue on to small modelIndex");
				continue;
			}
			int elseBlockIndex = l_find_group_index_by_mnemonic_index(modelContainer, jumpIndex);
			if (elseBlockIndex==-1) {
				modelIndex = startIndex;
				startIndex = -1;
				cat_log_trace("continue on missing else-block-index");
				continue;
			}

			int count = (modelIndex)-startIndex;
			CatArrayWo *e_condGrpNodes = cat_array_wo_new();
			int i;
			for(i=count-1; i>=0; i--) {
				JagDecIIntermediateNode *removed_node = jag_dec_intermediate_group_remove_at(modelContainer,i+startIndex);
				cat_array_wo_insert(e_condGrpNodes, (GObject *) removed_node, 0);
				cat_unref_ptr(removed_node)
			}


			JagDecIntermediateConditionSet *new_cond_set = jag_dec_intermediate_condition_set_new(jumpIndex, e_condGrpNodes);
			jag_dec_intermediate_group_add_at(modelContainer, startIndex, (JagDecIIntermediateNode *) new_cond_set);
			cat_unref_ptr(new_cond_set);
			cat_unref_ptr(e_condGrpNodes);
			modelIndex = startIndex;
			startIndex=-1;
			changed=TRUE;
			return TRUE;
		}

	}
	return changed;
}


static CatArrayWo *l_collect_intermediate_nodes_for_producer(JagDecIValueProducer *topProducer);


static gboolean l_trace_for_loop(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {
	JagDecGroupConnectorPrivate *priv = JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector);
	gboolean changed = FALSE;
	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *group = jag_dec_intermediate_group_get(modelContainer, modelIndex);
		if (JAG_DEC_IS_INTERMEDIATE_LOOP_GROUP(group)) {
			JagDecIntermediateLoopGroup *loopGroup = (JagDecIntermediateLoopGroup *) group;
			int initFrameIndex = -1;
			JagDecIntermediateGroup *newInitGroup = NULL;

			/* search through  init nodes */
			JagDecIIntermediateNode *initNode = jag_dec_intermediate_group_get(modelContainer, modelIndex-1);
			if (JAG_DEC_IS_INTERMEDIATE_GROUP(initNode)) {
				JagDecIntermediateGroup *initGroup = (JagDecIntermediateGroup *) initNode;
				int count = jag_dec_intermediate_group_count(initGroup);
				int offset = count-1;
				while(offset>=0) {
					initNode = jag_dec_intermediate_group_get(initGroup, offset);
					if (JAG_DEC_IS_INTERMEDIATE_GOTO(initNode)) {
						offset--;
						continue;
					} else if (JAG_DEC_IS_INTERMEDIATE_FRAME_PUT(initNode)) {
						JagDecIntermediateFramePut *fp = (JagDecIntermediateFramePut *) initNode;
						initFrameIndex = jag_dec_intermediate_frame_put_get_frame_index(fp);
					}
					break;
				}

				if (initFrameIndex>=0) {
					JagDecIntermediateRunContext *irc = jag_dec_intermediate_run_context_new((JagBytIMnemonicBlock *) priv->mnemonic_block, priv->local_variable_table);
					int idx;
					for(idx=0; idx<offset; idx++) {
						initNode = jag_dec_intermediate_group_get(initGroup, idx);
//						System.out.println("initNode="+initNode);
						jag_dec_iintermediate_node_connect_stage_one((JagDecIIntermediateNode *) initNode, irc);
					}
					JagDecIValueProducer *topProducer = jag_dec_intermediate_run_context_peek_producer(irc, 0);
					if (topProducer) {
						CatArrayWo *e_nodes = l_collect_intermediate_nodes_for_producer(topProducer);
						cat_array_wo_append(e_nodes, (GObject *) jag_dec_intermediate_group_get(initGroup, offset));
						newInitGroup = jag_dec_intermediate_group_new_with_nodes(e_nodes);
						cat_unref(e_nodes);
					} else {
						cat_unref_ptr(irc);
						continue;
					}
					cat_unref_ptr(irc);
				}

			}




			int incFrameIndex = -1;
			/* search through condition nodes*/
			JagDecIIntermediateNode *last = jag_dec_intermediate_group_get((JagDecIntermediateGroup *) loopGroup, jag_dec_intermediate_group_count((JagDecIntermediateGroup *) loopGroup)-2);
			JagDecIIntermediateNode *newIncrementNode = NULL;
			if (JAG_DEC_IS_INTERMEDIATE_GROUP(last)) {
				JagDecIntermediateGroup *incrementGroup = (JagDecIntermediateGroup *) last;
				int count = jag_dec_intermediate_group_count(incrementGroup);
				int offset = count-1;
				while(offset>=0) {
					initNode = jag_dec_intermediate_group_get(incrementGroup, offset);
					if (JAG_DEC_IS_INTERMEDIATE_GOTO(initNode)) {
						offset--;
						continue;
					} else if (JAG_DEC_IS_INTERMEDIATE_INCREMENT(initNode)) {
						JagDecIntermediateIncrement *fp = (JagDecIntermediateIncrement *) initNode;
						incFrameIndex = jag_dec_intermediate_increment_get_frame_index(fp);
						newIncrementNode = cat_ref_ptr(initNode);
					}
					break;
				}

			}

			if (incFrameIndex==initFrameIndex && incFrameIndex>=0) {

				JagDecIIntermediateNode *newConditionNode = jag_dec_intermediate_group_get_last((JagDecIntermediateGroup *) loopGroup);



				JagDecIIntermediateNode *newLoopNode = NULL;
				if (jag_dec_intermediate_group_count((JagDecIntermediateGroup *) loopGroup)==2) {
					newLoopNode = jag_dec_intermediate_group_get((JagDecIntermediateGroup *) loopGroup, 0);
					cat_ref_ptr(newLoopNode);
				} else {
					JagDecIntermediateGroup *group2 = jag_dec_intermediate_group_new();
					int idx;
					for(idx=0; idx<jag_dec_intermediate_group_count((JagDecIntermediateGroup *) loopGroup)-1; idx++) {
						jag_dec_intermediate_group_add(group2, jag_dec_intermediate_group_get((JagDecIntermediateGroup *) loopGroup, idx));
					}
					newLoopNode = (JagDecIIntermediateNode *) group2;
				}

				last = jag_dec_intermediate_group_get((JagDecIntermediateGroup *) loopGroup, jag_dec_intermediate_group_count((JagDecIntermediateGroup *) loopGroup)-2);
				if (JAG_DEC_IS_INTERMEDIATE_GROUP(last)) {
					JagDecIntermediateGroup *incrementGroup = (JagDecIntermediateGroup *) last;
					jag_dec_intermediate_group_remove(incrementGroup, newIncrementNode);
				}

				JagDecIntermediateForLoop *intermediateForLoop = jag_dec_intermediate_for_loop_new((JagDecIIntermediateNode *) newInitGroup, newConditionNode, newIncrementNode, newLoopNode);
				cat_unref_ptr(newLoopNode);
//				StringBuilder dumpBuffer = new StringBuilder();
//				intermediateForLoop.dump(dumpBuffer, ">>");
//				System.out.println(dumpBuffer.toString());
//					System.exit(-1);


				initNode = jag_dec_intermediate_group_get(modelContainer, modelIndex-1);
				if (JAG_DEC_IS_INTERMEDIATE_GROUP(initNode)) {
					JagDecIntermediateGroup *initGroup = (JagDecIntermediateGroup *) initNode;
					int idx;
					for(idx=0; idx<jag_dec_intermediate_group_count(newInitGroup); idx++) {
						JagDecIIntermediateNode *to_remove_node = jag_dec_intermediate_group_get(newInitGroup, idx);
						jag_dec_intermediate_group_remove(initGroup, to_remove_node);
					}
				}

				JagDecIIntermediateNode *removed = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
				jag_dec_intermediate_group_add_at(modelContainer, modelIndex, (JagDecIIntermediateNode *) intermediateForLoop);
				cat_unref_ptr(intermediateForLoop);
				cat_unref_ptr(removed);
			}
			cat_unref_ptr(newIncrementNode);

			cat_unref_ptr(newInitGroup);
		}
	}
	return changed;
}


static CatArrayWo *l_collect_intermediate_nodes_for_producer(JagDecIValueProducer *topProducer) {
//	System.out.println("topProducer="+topProducer);
	CatArrayWo *e_result = cat_array_wo_new();
	CatArrayWo *e_stack = cat_array_wo_new();
	if (topProducer!=NULL) {
		JagDecIIntermediateNode *intermediateNode = jag_dec_ivalue_producer_get_intermediate_node(topProducer);
		if (intermediateNode!=NULL && cat_array_wo_find_index(e_result, intermediateNode, -1)<0) {
			cat_array_wo_append(e_result, (GObject *) intermediateNode);
		}
	}
	CatArrayWo *e_consumers = jag_dec_ivalue_producer_get_input_consumers(topProducer);
	if (e_consumers!=NULL) {
		cat_array_wo_append_all(e_stack, e_consumers);
	}
	while(cat_array_wo_size(e_stack)>0) {
		JagDecIConsumer *consumer = NULL;
		cat_array_wo_remove(e_stack, 0, (GObject **) consumer);
		JagDecIIntermediateNode *intermediateNode = jag_dec_iconsumer_get_intermediate_node(consumer);
//		System.out.println("consumer="+consumer+", stack.si="+stack.size()+", intermediateNode="+intermediateNode);
		if (intermediateNode!=NULL  && cat_array_wo_find_index(e_result, intermediateNode, -1)<0) {
			cat_array_wo_append(e_result, (GObject *) intermediateNode);
		}

		JagDecIValueProducer *producer = jag_dec_iconsumer_get_producer(consumer);
		if (producer!=NULL) {
			intermediateNode = jag_dec_ivalue_producer_get_intermediate_node(producer);
//			System.out.println("consumer.producer="+producer+" ,intermediateNode="+intermediateNode);
			if (intermediateNode!=NULL  && cat_array_wo_find_index(e_result, intermediateNode, -1)<0) {
				cat_array_wo_append(e_result, (GObject *) intermediateNode);
			}
			CatArrayWo *e_consumers = jag_dec_ivalue_producer_get_input_consumers(producer);
			if (e_consumers!=NULL) {
//				int idx;
//				for(idx=0; idx<cat_array_wo_size(consumers); idx++) {
//					JagDecIConsumer *iConsumer = (JagDecIConsumer *) cat_array_wo_get(consumers, idx);
////					System.out.println("consumer["+idx+"] = "+iConsumer);
//				}
				cat_array_wo_append_all(e_stack, e_consumers);
			}
		}
	}
	cat_unref_ptr(e_stack);
	cat_array_wo_reverse(e_result);
	return e_result;
}



void l_resolve_breaks(JagDecIntermediateGroup *loop_group) {
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) loop_group);
	int jump_index = jag_dec_intermediate_node_scope_get_exit_index(scope);
	cat_unref_ptr(scope);
	jag_dec_iintermediate_node_resolve_breaks((JagDecIIntermediateNode *) loop_group, jump_index);
}


//int loopTime;

static gboolean l_trace_loop(JagDecGroupConnector *group_connector, JagDecIntermediateGroup *modelContainer) {
	gboolean changed = FALSE;

//	if (loopTime++>300) {
//		(new RuntimeException("sdfguhruoigheuiqwrhgtiuhgfuisdhfuighdsfuighisdugd")).printStackTrace();
//		modelContainer.dump("EEEEE");
//		System.exit(-1);
//	}
	int modelIndex;
	for(modelIndex=0; modelIndex<jag_dec_intermediate_group_count(modelContainer); modelIndex++) {
		JagDecIIntermediateNode *group = jag_dec_intermediate_group_get(modelContainer, modelIndex);
		JagDecIntermediateNodeScope *child_scope = jag_dec_iintermediate_node_get_scope_ref(group);
		int jumpIndex = jag_dec_intermediate_node_scope_get_jump_index(child_scope);
		int last = jag_dec_intermediate_node_scope_get_exit_index(child_scope);
		cat_unref_ptr(child_scope);
		cat_log_trace("modelIndex=%d, jumpIndex=%d, child=%s", modelIndex, jumpIndex, g_type_name_from_instance((GTypeInstance *) group));
		if (jumpIndex==-1) {
			continue;
		}
		cat_log_trace("jumpIndex=%d, last=%d", jumpIndex, last);
		if ((jumpIndex!=-1) && (last!=-1) && (jumpIndex<last)) {

			int jumpModelIndex = l_find_group_index_by_mnemonic_index(modelContainer, jumpIndex);
			if (modelIndex==jag_dec_intermediate_group_count(modelContainer)-1 && jumpModelIndex==0) {
				continue;
			}

			cat_log_trace("modelIndex=%d, jumpModelIndex=%d", modelIndex, jumpModelIndex);
			if (jumpModelIndex==modelIndex) {
				JagDecIIntermediateNode *conditionModel = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
				CatArrayWo *e_t = cat_array_wo_new();
				cat_array_wo_append(e_t, (GObject *) conditionModel);
				cat_unref(conditionModel);
				JagDecIntermediateGroup *loopContainer = (JagDecIntermediateGroup *) jag_dec_intermediate_loop_group_new_with_nodes(e_t);
				cat_unref_ptr(e_t);
				l_resolve_breaks(loopContainer);
				l_convert_group(group_connector, loopContainer);
				jag_dec_intermediate_group_add_at(modelContainer, jumpModelIndex, (JagDecIIntermediateNode *) loopContainer);
				changed = TRUE;
			} else if (jumpModelIndex>0) {

				JagDecIntermediateGroup *loopContainer = (JagDecIntermediateGroup *) jag_dec_intermediate_loop_group_new();

				JagDecIIntermediateNode *conditionModel = jag_dec_intermediate_group_remove_at(modelContainer, modelIndex);
				int subCodeIdx;
				for(subCodeIdx=modelIndex-1; subCodeIdx>=jumpModelIndex; subCodeIdx--) {
					JagDecIIntermediateNode *removed_node = jag_dec_intermediate_group_remove_at(modelContainer, subCodeIdx);
					jag_dec_intermediate_group_add_at(loopContainer, 0, removed_node);
					cat_unref_ptr(removed_node);
				}

				l_resolve_breaks(loopContainer);
				l_convert_group(group_connector, loopContainer);
				jag_dec_intermediate_group_add_at(loopContainer, jag_dec_intermediate_group_count(loopContainer), conditionModel);
				cat_unref(conditionModel);
				jag_dec_intermediate_group_add_at(modelContainer, jumpModelIndex, (JagDecIIntermediateNode *) loopContainer);
				cat_unref_ptr(loopContainer);
				changed = TRUE;
			}
		}
	}
	return changed;
}


static int l_find_group_index_by_mnemonic_index(JagDecIntermediateGroup *modelContainer, int mnemonic_index) {
	int index;
	for(index=0; index<jag_dec_intermediate_group_count(modelContainer); index++) {
		JagDecIIntermediateNode *child = jag_dec_intermediate_group_get(modelContainer, index);
		if (jag_dec_iintermediate_node_contains(child, mnemonic_index)) {
			return index;
		}
	}
	return -1;
}


JagDecIntermediateRunContext *jag_dec_group_connector_get_run_context(JagDecGroupConnector *group_connector) {
	return JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector)->connectContext;
}

JagDecIntermediateGroup *jag_dec_group_connector_get_model_container(JagDecGroupConnector *group_connector) {
	return JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(group_connector)->model_container;
}

