/*
   File:    jagdecsubcodemathbasic.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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

#include "jagdecsubcodemathbasic.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeMathBasic"
#include <logging/catlog.h>

struct _JagDecSubCodeMathBasicPrivate {
	JagBytMathOperator math_operator;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeMathBasic, jag_dec_sub_code_math_basic, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_math_basic_class_init(JagDecSubCodeMathBasicClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeMathBasicPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_math_basic_init(JagDecSubCodeMathBasic *instance) {
	JagDecSubCodeMathBasicPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_MATH_BASIC, JagDecSubCodeMathBasicPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeMathBasic *jag_dec_sub_code_math_basic_new(int mnemonic_index, JagBytMathOperator math_operator) {
	JagDecSubCodeMathBasic *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_MATH_BASIC, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeMathBasicPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->math_operator = math_operator;
	return result;
}






/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeMathBasicPrivate *priv = JAG_DEC_SUB_CODE_MATH_BASIC_GET_PRIVATE(self);
	JagDecIConsumer *left_consumer = jag_dec_isub_code_get_input_consumer_at(self, 1);
	JagDecIConsumer *right_consumer = jag_dec_isub_code_get_input_consumer_at(self, 0);
	JagAstIExpression *leftCondExpr = jag_dec_iconsumer_get_expression_ref(left_consumer, NULL);
	JagAstIExpression *rightCondExpr = jag_dec_iconsumer_get_expression_ref(right_consumer, NULL);
//	cat_log_debug("valueProducer.expressionHolder.expression set for "+valueProducer);
	CatArrayWo *e_expressions = cat_array_wo_new();
	cat_array_wo_append(e_expressions, (GObject *) leftCondExpr);
	cat_array_wo_append(e_expressions, (GObject *) rightCondExpr);
	cat_unref_ptr(leftCondExpr);
	cat_unref_ptr(rightCondExpr);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	JagAstInfixExpression *infix_expr = jag_ast_infix_expression_new(priv->math_operator, e_expressions);
	jag_dec_ivalue_producer_set_expression(value_producer, (JagAstIExpression *) infix_expr);
	cat_unref_ptr(infix_expr);
	cat_unref_ptr(e_expressions);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
