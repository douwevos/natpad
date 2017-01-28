/*
   File:    jagdecsubcodeifthenelse.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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

#include "jagdecsubcodeifthenelse.h"
#include "../jagdecconsumer.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeIfThenElse"
#include <logging/catlog.h>

struct _JagDecSubCodeIfThenElsePrivate {
	JagDecISubCode *then_sub_code;
	JagDecISubCode *else_sub_code;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeIfThenElse, jag_dec_sub_code_if_then_else, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_if_then_else_class_init(JagDecSubCodeIfThenElseClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeIfThenElsePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_if_then_else_init(JagDecSubCodeIfThenElse *instance) {
	JagDecSubCodeIfThenElsePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE, JagDecSubCodeIfThenElsePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeIfThenElse *instance = JAG_DEC_SUB_CODE_IF_THEN_ELSE(object);
	JagDecSubCodeIfThenElsePrivate *priv = instance->priv;
	cat_unref_ptr(priv->then_sub_code);
	cat_unref_ptr(priv->else_sub_code);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeIfThenElse *jag_dec_sub_code_if_then_else_new(JagDecISubCode *then_sub_code, JagDecISubCode *else_sub_code) {
	JagDecSubCodeIfThenElse *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeIfThenElsePrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, -1);
	priv->then_sub_code = cat_ref_ptr(then_sub_code);
	priv->else_sub_code = cat_ref_ptr(else_sub_code);
	return result;
}




/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeIfThenElsePrivate *priv = JAG_DEC_SUB_CODE_IF_THEN_ELSE_GET_PRIVATE(self);

	JagAstBlock *blockThen = jag_ast_block_new();
	JagAstBlock *blockElse = NULL;

	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	JagAstIExpression *condExpression = (JagAstIExpression *) jag_dec_ivalue_producer_get_expression_ref(value_producer, NULL);

	JagAstIfThenElse *ifThenElse = NULL;
	if (priv->else_sub_code!=NULL) {
		blockElse = jag_ast_block_new();
	}
	ifThenElse = jag_ast_if_then_else_new(condExpression, blockThen, blockElse);
	jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) ifThenElse);

	JagDecModelStageTwoRuntime *then_runtime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, blockThen);
	jag_dec_isub_code_connect_stage_two(priv->then_sub_code, then_runtime);
	cat_unref_ptr(then_runtime);

	if (priv->else_sub_code!=NULL) {
		JagDecModelStageTwoRuntime *else_runtime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, blockElse);
		jag_dec_isub_code_connect_stage_two(priv->else_sub_code, else_runtime);
		cat_unref_ptr(else_runtime);
	}
	cat_unref_ptr(condExpression);
	cat_unref_ptr(ifThenElse);
	cat_unref_ptr(blockElse);
	cat_unref_ptr(blockThen);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
