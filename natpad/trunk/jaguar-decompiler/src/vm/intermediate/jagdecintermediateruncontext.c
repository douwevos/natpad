/*
   File:    jagdecintermediateruncontext.c
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

#include "jagdecintermediateruncontext.h"
#include "jagdecintermediategroup.h"
#include "../subcode/impl/jagdecthisorsupervalue.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateRunContext"
#include <logging/catlog.h>

struct _JagDecIntermediateRunContextPrivate {
	JagBytIMnemonicBlock *mnemonicBlock;
	JagBytLocalVariableTable *localVariableTable;

	CatArrayWo *e_consumer_stack;					/* JagDecIConsumer */
	CatArrayWo *e_producer_stack;					/* JagDecIValueProducer */
	CatArrayWo *e_pseudo_stack;						/* JagDecIValueProducer */
	CatHashMapWo *e_method_frame_map;				/* CatInteger, JagDecMethodFrameValue */
	CatHashSet *value_type_definition_lists;	/* JagDecValueTypeDefinitionList */
	CatArrayWo *e_post_import_list;					/* JagDecIPostImport */
	JagDecSubCodeBlock *sub_code_block;
};

G_DEFINE_TYPE (JagDecIntermediateRunContext, jag_dec_intermediate_run_context, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_run_context_class_init(JagDecIntermediateRunContextClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateRunContextPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_run_context_init(JagDecIntermediateRunContext *instance) {
	JagDecIntermediateRunContextPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT, JagDecIntermediateRunContextPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateRunContext *instance = JAG_DEC_INTERMEDIATE_RUN_CONTEXT(object);
	JagDecIntermediateRunContextPrivate *priv = instance->priv;
	cat_unref_ptr(priv->mnemonicBlock);
	cat_unref_ptr(priv->localVariableTable);

	cat_unref_ptr(priv->e_consumer_stack);
	cat_unref_ptr(priv->e_producer_stack);
	cat_unref_ptr(priv->e_pseudo_stack);
	cat_unref_ptr(priv->e_method_frame_map);
	cat_unref_ptr(priv->value_type_definition_lists);
	cat_unref_ptr(priv->e_post_import_list);
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

JagDecIntermediateRunContext *jag_dec_intermediate_run_context_new(JagBytIMnemonicBlock *mnemonicBlock, JagBytLocalVariableTable *localVariableTable) {
	JagDecIntermediateRunContext *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateRunContextPrivate *priv = result->priv;
	priv->mnemonicBlock = cat_ref_ptr(mnemonicBlock);
	priv->localVariableTable = cat_ref_ptr(localVariableTable);

	priv->e_consumer_stack = cat_array_wo_new();
	priv->e_producer_stack = cat_array_wo_new();
	priv->e_pseudo_stack = cat_array_wo_new();
	priv->e_method_frame_map = cat_hash_map_wo_new((GHashFunc) cat_integer_hash, (GEqualFunc) cat_integer_equal);
	priv->value_type_definition_lists = cat_hash_set_new((GHashFunc) jag_dec_value_type_definition_list_hash, (GEqualFunc) jag_dec_value_type_definition_list_equal);
	priv->e_post_import_list = cat_array_wo_new();
	priv->sub_code_block = jag_dec_sub_code_block_new();

	return result;
}


JagBytLocalVariableTable *jag_dec_intermediate_run_context_get_local_variable_table(JagDecIntermediateRunContext *run_context) {
	return JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context)->localVariableTable;
}

JagBytIMnemonicBlock *jag_dec_intermediate_run_context_get_mnemonic_block(JagDecIntermediateRunContext *run_context) {
	return JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context)->mnemonicBlock;
}

JagDecSubCodeBlock *jag_dec_intermediate_run_context_get_sub_code_block(JagDecIntermediateRunContext *run_context) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	return priv->sub_code_block;
}

void jag_dec_intermediate_run_context_set_sub_code_block(JagDecIntermediateRunContext *run_context, JagDecSubCodeBlock *block) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_ref_swap(priv->sub_code_block, block);
}


void jag_dec_intermediate_run_context_clear_pseudo_stack(JagDecIntermediateRunContext *run_context) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_array_wo_clear(priv->e_pseudo_stack);
}

CatArrayWo *jag_dec_intermediate_run_context_get_pseudo_stack(JagDecIntermediateRunContext *run_context) {
	return JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context)->e_pseudo_stack;
}

CatArrayWo *jag_dec_intermediate_run_context_get_producer_stack(JagDecIntermediateRunContext *run_context) {
	return JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context)->e_producer_stack;
}


void jag_dec_intermediate_run_context_set_this(JagDecIntermediateRunContext *run_context, JagBytName *import_type, JagBytName *super_type) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	JagAstDeclarationType *this_decl_type = jag_ast_declaration_type_new_reference(import_type, 0);
	JagDecValueTypeDefinitionList *methodFrameValue = (JagDecValueTypeDefinitionList *) jag_dec_this_or_super_value_new(0, this_decl_type);
	JagDecValueTypeDefinition *this_value_type_def = jag_dec_value_type_definition_new(this_decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_THIS);
	jag_dec_value_type_definition_list_add_definition(methodFrameValue, this_value_type_def);
	cat_unref_ptr(this_value_type_def);
	cat_unref_ptr(this_decl_type);
	if (super_type!=NULL) {
		JagAstDeclarationType *super_decl_type = jag_ast_declaration_type_new_reference(super_type, 0);
		JagDecValueTypeDefinition *super_value_type_def = jag_dec_value_type_definition_new(super_decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_SUPER);
		jag_dec_value_type_definition_list_add_definition(methodFrameValue, super_value_type_def);
		cat_unref_ptr(super_value_type_def);
		cat_unref_ptr(super_decl_type);
	}
	CatInteger *zero_key = cat_integer_new(0);
	cat_hash_map_wo_put(priv->e_method_frame_map, (GObject *) zero_key, (GObject *) methodFrameValue);
	cat_unref_ptr(zero_key);
	cat_unref_ptr(methodFrameValue);
}


void jag_dec_intermediate_run_context_add_and_run_sub_code(JagDecIntermediateRunContext *run_context, JagDecISubCode *sub_code) {
	int consumerCount = 0;
	while(TRUE) {
		JagDecIConsumer *consumer = jag_dec_isub_code_get_input_consumer_at(sub_code, consumerCount);
		if (consumer!=NULL) {
			jag_dec_intermediate_run_context_push_value_consumer(run_context, consumer);
			consumerCount++;
		} else {
			break;
		}
	}

	JagDecIValueProducer *outputProducer = jag_dec_isub_code_get_output_producer(sub_code);
	if (outputProducer!=NULL) {
		int idx;
		for(idx=0; idx<consumerCount; idx++) {
			JagDecIConsumer *inputConsumer = jag_dec_isub_code_get_input_consumer_at(sub_code, idx);
			jag_dec_ivalue_producer_add_input_consumer(outputProducer, inputConsumer);
		}
		jag_dec_intermediate_run_context_push_value_provider(run_context, outputProducer);
	}

	jag_dec_intermediate_run_context_add_sub_code(run_context, sub_code);

}

static void l_resolve_types(JagDecIntermediateRunContext *run_context) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
//	cat_log_detail("resolve-types: valueTypeDefinitionLists.size=%d", valueTypeDefinitionLists.size());

	CatArrayWo *e_stack = cat_hash_set_enlist_all(priv->value_type_definition_lists);

//		for(int i=0; i<stack.size(); i++) {
//			log.debug("stack["+i+"]="+stack.get(i));
//		}
	while(cat_array_wo_size(e_stack)!=0) {
		JagDecValueTypeDefinitionList *valueTypeDefinitionList = NULL;
		cat_array_wo_remove_first(e_stack, (GObject **) (&valueTypeDefinitionList));
		cat_log_trace("resolve-types: valueTypeDefinitionList=%p", valueTypeDefinitionList);
		gboolean result = jag_dec_value_type_definition_list_apply_cross_rules(valueTypeDefinitionList);
		if (result==FALSE) {
			cat_array_wo_append(e_stack, (GObject *) valueTypeDefinitionList);
		}
		cat_unref_ptr(valueTypeDefinitionList);
	}
	cat_unref_ptr(e_stack);

}

void jag_dec_intermediate_run_context_connect(JagDecIntermediateRunContext *run_context, JagDecImportRepository *import_repository) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	l_resolve_types(run_context);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_post_import_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIPostImport *post_import = (JagDecIPostImport *) cat_iiterator_next(iter);
		jag_dec_ipost_import_add_import(post_import, import_repository);
	}
	cat_unref_ptr(iter);
}

JagDecIntermediateRunContext *jag_dec_intermediate_run_context_create_branch(JagDecIntermediateRunContext *run_context) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	JagDecIntermediateRunContext *result = jag_dec_intermediate_run_context_new(priv->mnemonicBlock, priv->localVariableTable);
	JagDecIntermediateRunContextPrivate *respriv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(result);

	cat_array_wo_append_all(respriv->e_consumer_stack, priv->e_consumer_stack);
	cat_array_wo_append_all(respriv->e_producer_stack, priv->e_producer_stack);
//		result.pseudoStack.addAll(pseudoStack);

	cat_hash_map_wo_put_all(respriv->e_method_frame_map, priv->e_method_frame_map);
	cat_ref_swap(respriv->e_post_import_list, priv->e_post_import_list);
	cat_ref_swap(respriv->value_type_definition_lists, priv->value_type_definition_lists);
	return result;
}


JagDecIValueProducer *jag_dec_intermediate_run_context_peek_producer(JagDecIntermediateRunContext *run_context, int index) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	int offset = cat_array_wo_size(priv->e_producer_stack)-1 - index;
	if (offset>=0) {
		return (JagDecIValueProducer *) cat_array_wo_get(priv->e_producer_stack, offset);
	}
	return NULL;
}


JagDecIValueProducer *jag_dec_intermediate_run_context_pop_producer(JagDecIntermediateRunContext *run_context) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	JagDecIValueProducer *result = NULL;
	cat_array_wo_remove_last(priv->e_producer_stack, (GObject **) (&result));
	return result;
}

void jag_dec_intermediate_run_context_push_value_provider(JagDecIntermediateRunContext *run_context, JagDecIValueProducer *producer) {
	cat_log_debug("pushing provider:%p", producer);
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_array_wo_append(priv->e_producer_stack, (GObject *) producer);
}

void jag_dec_intermediate_run_context_push_value_provider_at(JagDecIntermediateRunContext *run_context, JagDecIValueProducer *producer, int index) {
//		log.debug("pushing provider:"+producer);
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	int offset = cat_array_wo_size(priv->e_producer_stack)-index;
	if (offset>=0) {
		cat_array_wo_insert(priv->e_producer_stack, (GObject *) producer, offset);
	} else {
		cat_log_error("invalid stack:producer=%p, index=%d, offset=%d", producer, index, offset);
	}
}


void jag_dec_intermediate_run_context_push_pseudo_producer(JagDecIntermediateRunContext *run_context, JagDecIValueProducer *producer) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_array_wo_append(priv->e_pseudo_stack, (GObject *) producer);
}



void jag_dec_intermediate_run_context_push_value_consumer(JagDecIntermediateRunContext *run_context, JagDecIConsumer *consumer) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_log_debug("pushing consumer:%p, producerStack.size=%d",consumer, cat_array_wo_size(priv->e_producer_stack));
	if (cat_array_wo_size(priv->e_producer_stack)==0) {
		cat_array_wo_append(priv->e_consumer_stack, (GObject *) consumer);
		cat_log_error("no producer for consumer:%p", consumer);
	} else {
		JagDecIValueProducer *producer = jag_dec_intermediate_run_context_pop_producer(run_context);
		cat_log_debug("consumer:%p connected with producer:%p", consumer, producer);
		jag_dec_ivalue_producer_set_consumer(producer, consumer);
		jag_dec_iconsumer_consume(consumer, producer);
		cat_unref_ptr(producer);
	}
}


void jag_dec_intermediate_run_context_dump_stacks(JagDecIntermediateRunContext *run_context) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	if (cat_array_wo_size(priv->e_producer_stack)==0) {
		cat_log_debug("producer-stack:empty");
	} else {
		cat_log_debug("producer-stack:%d", cat_array_wo_size(priv->e_producer_stack));
		int idx;
		for(idx=0; idx<cat_array_wo_size(priv->e_producer_stack); idx++) {
			JagDecIValueProducer *producer = (JagDecIValueProducer *) cat_array_wo_get(priv->e_producer_stack, idx);
			CatStringWo *e_pastext = jag_dec_ivalue_producer_to_string(producer);
			cat_log_debug("[%d] %p-%s", idx, producer, cat_string_wo_getchars(e_pastext));
			cat_unref_ptr(e_pastext);
		}
	}
}


JagDecMethodFrameValue *jag_dec_intermediate_run_context_get_method_frame_value(JagDecIntermediateRunContext *run_context, int frameIndex) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	CatInteger *midx = cat_integer_new(frameIndex);
	JagDecMethodFrameValue *result = (JagDecMethodFrameValue *) cat_hash_map_wo_get(priv->e_method_frame_map, (GObject *) midx);
	cat_unref_ptr(midx);
	return result;
}

JagDecMethodFrameValue *jag_dec_intermediate_run_context_create_method_frame_value(JagDecIntermediateRunContext *run_context, int frameIndex) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	CatInteger *midx = cat_integer_new(frameIndex);
	JagDecMethodFrameValue *result = (JagDecMethodFrameValue *) cat_hash_map_wo_get(priv->e_method_frame_map, (GObject *) midx);
	if (result==NULL) {
		result = jag_dec_method_frame_value_new(frameIndex, NULL);
		cat_hash_map_wo_put(priv->e_method_frame_map, (GObject *) midx, (GObject *) result);
		cat_unref(result);
	}
	cat_unref_ptr(midx);
	return result;
}

void jag_dec_intermediate_run_context_add_frame_value(JagDecIntermediateRunContext *run_context, int frameIndex, JagDecMethodFrameValue *methodFrameValue) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	CatInteger *midx = cat_integer_new(frameIndex);
	cat_hash_map_wo_put(priv->e_method_frame_map, (GObject *) midx, (GObject *) methodFrameValue);
	cat_unref_ptr(midx);

}



void jag_dec_intermediate_run_context_add_value_type_definition_list(JagDecIntermediateRunContext *run_context, JagDecValueTypeDefinitionList *valueTypeDefList) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_hash_set_add(priv->value_type_definition_lists, (GObject *) valueTypeDefList);
}

void jag_dec_intermediate_run_context_add_sub_code(JagDecIntermediateRunContext *run_context, JagDecISubCode *subCode) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_log_debug(" >> adding subcode:%o", subCode);
	jag_dec_sub_code_block_append(priv->sub_code_block, subCode);
}


void jag_dec_intermediate_run_context_add_post_importer(JagDecIntermediateRunContext *run_context, JagDecIPostImport *postImport) {
	JagDecIntermediateRunContextPrivate *priv = JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(run_context);
	cat_array_wo_append(priv->e_post_import_list, (GObject *) postImport);
}






