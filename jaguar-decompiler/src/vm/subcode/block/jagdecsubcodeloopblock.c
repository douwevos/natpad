/*
   File:    jagdecsubcodeloopblock.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 18, 2012
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

#include "jagdecsubcodeloopblock.h"
#include "../jagdecisubcode.h"
#include "../jagdecabstractsubcode.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeLoopBlock"
#include <logging/catlog.h>

struct _JagDecSubCodeLoopBlockPrivate {
	JagDecISubCode *child_node;
	JagDecConsumer *condition_consumer;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeLoopBlock, jag_dec_sub_code_loop_block, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_loop_block_class_init(JagDecSubCodeLoopBlockClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeLoopBlockPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_loop_block_init(JagDecSubCodeLoopBlock *instance) {
	JagDecSubCodeLoopBlockPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK, JagDecSubCodeLoopBlockPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeLoopBlock *instance = JAG_DEC_SUB_CODE_LOOP_BLOCK(object);
	JagDecSubCodeLoopBlockPrivate *priv = instance->priv;
	cat_unref_ptr(priv->child_node);
	cat_unref_ptr(priv->condition_consumer);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeLoopBlock *jag_dec_sub_code_loop_block_new(JagDecConsumer *condition_consumer, JagDecISubCode *child_node) {
	JagDecSubCodeLoopBlock *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeLoopBlockPrivate *priv = result->priv;
	priv->child_node = cat_ref_ptr(child_node);
	priv->condition_consumer = cat_ref_ptr(condition_consumer);
	return result;
}



JagDecConsumer *jag_dec_sub_code_loop_block_get_consumer(JagDecSubCodeLoopBlock *sub_code_loop_block) {
	return JAG_DEC_SUB_CODE_LOOP_BLOCK_GET_PRIVATE(sub_code_loop_block)->condition_consumer;
}



/********************* start JagDecISubCodeInterface implementation *********************/


static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeLoopBlockPrivate *priv = JAG_DEC_SUB_CODE_LOOP_BLOCK_GET_PRIVATE(self);
	JagAstBlock *while_body = jag_ast_block_new();
	JagAstWhileStatement *while_statement = jag_ast_while_statement_new(while_body);
	jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) while_statement);
	JagDecModelStageTwoRuntime *body_runtime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, while_body);
	cat_unref_ptr(while_body);

	if (JAG_DEC_IS_ABSTRACT_SUB_CODE(priv->child_node)) {
		int mnemonic_index = jag_dec_abstract_sub_code_get_mnemonic_index((JagDecAbstractSubCode *) priv->child_node);
		jag_dec_model_stage_two_runtime_set_mnemonic_index(body_runtime, mnemonic_index);
	}
	jag_dec_isub_code_connect_stage_two(priv->child_node, body_runtime);

	JagAstDeclarationType *bool_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_BOOLEAN, 0);
	JagAstIExpression *consumer_expressions = jag_dec_iconsumer_get_expression_ref((JagDecIConsumer *) priv->condition_consumer, bool_decl_type);
	JagAstIConditionalExpression *conditional_expressions = jag_ast_wrapped_conditional_expression_ensure_is_conditional(consumer_expressions);
	cat_unref_ptr(consumer_expressions);
	cat_unref_ptr(bool_decl_type);
	JagAstIConditionalExpression *inverted_expressions = jag_ast_iconditional_expression_invert(conditional_expressions);
	cat_unref_ptr(conditional_expressions);
	jag_ast_while_statement_set_conditional_expression(while_statement, inverted_expressions);
	cat_unref_ptr(inverted_expressions);
	cat_unref_ptr(while_statement);
	cat_unref_ptr(body_runtime);
}

static struct _JagDecIConsumer *l_sub_code_get_input_consumer_at(JagDecISubCode *self, int index) {
	JagDecSubCodeLoopBlockPrivate *priv = JAG_DEC_SUB_CODE_LOOP_BLOCK_GET_PRIVATE(self);
	if (index==0) {
		return (struct _JagDecIConsumer *) priv->condition_consumer;
	}
	return NULL;
}

static struct _JagDecIValueProducer *l_sub_code_get_output_producer(JagDecISubCode *self) {
	return NULL;
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = l_sub_code_get_input_consumer_at;
	iface->getOutputProducer = l_sub_code_get_output_producer;
}


/********************* end JagDecISubCodeInterface implementation *********************/
