/*
   File:    jagdecsubcodebitwiseandorxor.c
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

#include "jagdecsubcodebitwiseandorxor.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeBitwiseAndOrXor"
#include <logging/catlog.h>

struct _JagDecSubCodeBitwiseAndOrXorPrivate {
	JagBytMathOperator bitwise_operator;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeBitwiseAndOrXor, jag_dec_sub_code_bitwise_and_or_xor, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_bitwise_and_or_xor_class_init(JagDecSubCodeBitwiseAndOrXorClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeBitwiseAndOrXorPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_bitwise_and_or_xor_init(JagDecSubCodeBitwiseAndOrXor *instance) {
	JagDecSubCodeBitwiseAndOrXorPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR, JagDecSubCodeBitwiseAndOrXorPrivate);
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

JagDecSubCodeBitwiseAndOrXor *jag_dec_sub_code_bitwise_and_or_xor_new(int mnemonic_index, JagBytMathOperator bitwise_operator) {
	JagDecSubCodeBitwiseAndOrXor *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeBitwiseAndOrXorPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->bitwise_operator = bitwise_operator;

	return result;
}


/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeBitwiseAndOrXorPrivate *priv = JAG_DEC_SUB_CODE_BITWISE_AND_OR_XOR_GET_PRIVATE(self);
	JagDecConsumer *left_consumer = (JagDecConsumer *) jag_dec_isub_code_get_input_consumer_at(self, 1);
	JagDecConsumer *right_consumer = (JagDecConsumer *) jag_dec_isub_code_get_input_consumer_at(self, 0);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	JagAstIExpression *leftCondExpr = jag_dec_iconsumer_get_expression_ref((JagDecIConsumer*) left_consumer, NULL);
	JagAstIExpression *rightCondExpr = jag_dec_iconsumer_get_expression_ref((JagDecIConsumer*) right_consumer, NULL);

	JagDecValueTypeDefinitionList *left_type_set = jag_dec_consumer_get_type_set(left_consumer);
	JagAstDeclarationType *left_best_type = jag_dec_value_type_definition_list_get_best_type_ref(left_type_set);
	if (left_best_type!=NULL && jag_ast_declaration_type_get_primitive_type(left_best_type)==JAG_BYT_TYPE_BOOLEAN) {
		JagDecValueTypeDefinitionList *right_type_set = jag_dec_consumer_get_type_set(right_consumer);
		JagAstDeclarationType *right_best_type = jag_dec_value_type_definition_list_get_best_type_ref(right_type_set);
		if (right_best_type!=NULL && jag_ast_declaration_type_get_primitive_type(right_best_type)==JAG_BYT_TYPE_BOOLEAN) {
			JagDecValueTypeDefinitionList *result_type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
			JagAstDeclarationType *result_best_type = jag_dec_value_type_definition_list_get_best_type_ref(result_type_set);
			if (result_best_type!=NULL && jag_ast_declaration_type_get_primitive_type(result_best_type)==JAG_BYT_TYPE_BOOLEAN) {
				JagAstIConditionalExpression *rleftCondExpr = jag_ast_wrapped_conditional_expression_ensure_is_conditional(leftCondExpr);
				JagAstIConditionalExpression *rrightCondExpr = jag_ast_wrapped_conditional_expression_ensure_is_conditional(rightCondExpr);
				CatArrayWo *e_expressions = cat_array_wo_new();
				cat_array_wo_append(e_expressions, (GObject *) rleftCondExpr);
				cat_array_wo_append(e_expressions, (GObject *) rrightCondExpr);
				cat_unref_ptr(rleftCondExpr);
				cat_unref_ptr(rrightCondExpr);
				JagAstLogicalExpression *logical_expression = jag_ast_logical_expression_new(priv->bitwise_operator, e_expressions);
				jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) value_producer, (JagAstIExpression *) logical_expression);
				cat_unref_ptr(e_expressions);
				cat_unref_ptr(logical_expression);
				cat_unref_ptr(leftCondExpr);
				cat_unref_ptr(rightCondExpr);
				cat_unref_ptr(left_best_type);
				cat_unref_ptr(right_best_type);
				cat_unref_ptr(result_best_type);
				return;
			}
			cat_unref_ptr(result_best_type);
		}
		cat_unref_ptr(right_best_type);
	}

	CatArrayWo *e_expressions = cat_array_wo_new();
	cat_array_wo_append(e_expressions, (GObject *) leftCondExpr);
	cat_array_wo_append(e_expressions, (GObject *) rightCondExpr);
	JagAstInfixExpression *infix_expression = jag_ast_infix_expression_new(priv->bitwise_operator, e_expressions);
	jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) value_producer, (JagAstIExpression *) infix_expression);
	cat_unref_ptr(e_expressions);
	cat_unref_ptr(infix_expression);

	cat_unref_ptr(left_best_type);
	cat_unref_ptr(leftCondExpr);
	cat_unref_ptr(rightCondExpr);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/

