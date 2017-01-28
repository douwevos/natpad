/*
   File:    jagdecsubcodeforloop.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Dec 24, 2012
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

#include "jagdecsubcodeforloop.h"
#include "../../jagdecmodelstagetworuntime.h"
#include <jaguarbytecode.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeForLoop"
#include <logging/catlog.h>

struct _JagDecSubCodeForLoopPrivate {
	JagDecISubCode *initBlock;
	JagDecISubCode *conditionBlock;
	JagDecConsumer *conditionConsumer;
	JagDecISubCode *forBlock;
	JagDecISubCode *bodyBlock;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeForLoop, jag_dec_sub_code_for_loop, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_for_loop_class_init(JagDecSubCodeForLoopClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeForLoopPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_for_loop_init(JagDecSubCodeForLoop *instance) {
	JagDecSubCodeForLoopPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_FOR_LOOP, JagDecSubCodeForLoopPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeForLoop *instance = JAG_DEC_SUB_CODE_FOR_LOOP(object);
	JagDecSubCodeForLoopPrivate *priv = instance->priv;
	cat_unref_ptr(priv->initBlock);
	cat_unref_ptr(priv->conditionBlock);
	cat_unref_ptr(priv->conditionConsumer);
	cat_unref_ptr(priv->forBlock);
	cat_unref_ptr(priv->bodyBlock);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeForLoop *jag_dec_sub_code_for_loop_new(JagDecISubCode *initBlock, JagDecISubCode *conditionBlock, JagDecConsumer *conditionConsumer, JagDecISubCode *forBlock, JagDecISubCode *bodyBlock) {
	JagDecSubCodeForLoop *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_FOR_LOOP, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeForLoopPrivate *priv = result->priv;
	priv->initBlock = cat_ref_ptr(initBlock);
	priv->conditionBlock = cat_ref_ptr(conditionBlock);
	priv->conditionConsumer = cat_ref_ptr(conditionConsumer);
	priv->forBlock = cat_ref_ptr(forBlock);
	priv->bodyBlock = cat_ref_ptr(bodyBlock);
	return result;
}







/********************* start JagDecISubCodeInterface implementation *********************/


static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeForLoopPrivate *priv = JAG_DEC_SUB_CODE_FOR_LOOP_GET_PRIVATE(self);

	JagAstBlock *initStatementBlock = jag_ast_block_new();
	JagDecModelStageTwoRuntime *branchRuntime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, initStatementBlock);
	jag_dec_isub_code_connect_stage_two(priv->initBlock, branchRuntime);


	JagAstIStatement *iStatement = jag_ast_block_get_statement_at(initStatementBlock, 0);
	JagAstIAstNode *forInit = NULL;
	if (JAG_AST_IS_IAST_NODE(iStatement)) {
		forInit = (JagAstIAstNode *) iStatement;
	}


	JagAstBlock *astConditionBlock = jag_ast_block_new();
	cat_unref_ptr(branchRuntime);
	branchRuntime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, astConditionBlock);
	jag_dec_isub_code_connect_stage_two(priv->conditionBlock, branchRuntime);


	JagAstDeclarationType *bool_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_BOOLEAN, 0);
	JagAstIConditionalExpression *condExpression = (JagAstIConditionalExpression*) jag_dec_iconsumer_get_expression_ref((JagDecIConsumer *) priv->conditionConsumer, bool_decl_type);
	cat_log_debug("priv->conditionConsumer=%o, condExpression:%p", priv->conditionConsumer, condExpression);
	cat_unref(bool_decl_type);

	JagAstBlock *astIncrementBlock = jag_ast_block_new();
	cat_unref_ptr(branchRuntime);
	branchRuntime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, astIncrementBlock);
	jag_dec_isub_code_connect_stage_two(priv->forBlock, branchRuntime);
	JagAstIStatement *iStatement2 = jag_ast_block_get_statement_at(astIncrementBlock, 0);
	JagAstIAstNode *forUpdate = NULL;
	if (JAG_AST_IS_IAST_NODE(iStatement2)) {
		forUpdate = (JagAstIAstNode *) iStatement2;
	}


	JagAstBlock *astBodyBlock = jag_ast_block_new();
	cat_unref_ptr(branchRuntime);
	branchRuntime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, astBodyBlock);
	jag_dec_isub_code_connect_stage_two(priv->bodyBlock, branchRuntime);


	cat_log_debug("condExpression=%o, count=%d", condExpression, (((GObject *) (condExpression))->ref_count));
	JagAstIExpression *inverted_expr = (JagAstIExpression *) jag_ast_iconditional_expression_invert(condExpression);
	cat_unref_ptr(condExpression);
	JagAstForLoopStatement *forLoopStatement = jag_ast_for_loop_statement_new(forInit,inverted_expr , forUpdate, astBodyBlock);
	jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) forLoopStatement);
	cat_unref_ptr(astBodyBlock);
	cat_unref_ptr(astIncrementBlock);
	cat_unref_ptr(astConditionBlock);
	cat_unref_ptr(initStatementBlock);
	cat_unref_ptr(inverted_expr);
	cat_unref_ptr(forLoopStatement);
	cat_unref_ptr(branchRuntime);
}

static struct _JagDecIConsumer *l_sub_code_get_input_consumer_at(JagDecISubCode *self, int index) {
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

