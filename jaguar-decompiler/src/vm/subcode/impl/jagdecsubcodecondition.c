/*
   File:    jagdecsubcodecondition.c
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

#include "jagdecsubcodecondition.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../jagdecisubcode.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeCondition"
#include <logging/catlog.h>

struct _JagDecSubCodeConditionPrivate {
	gboolean with_zero;
	JagBytConditionType condition_type;

};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeCondition, jag_dec_sub_code_condition, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_condition_class_init(JagDecSubCodeConditionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeConditionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_condition_init(JagDecSubCodeCondition *instance) {
	JagDecSubCodeConditionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_CONDITION, JagDecSubCodeConditionPrivate);
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

JagDecSubCodeCondition *jag_dec_sub_code_condition_new(int mnemonic_index, gboolean with_zero, JagBytConditionType condition_type) {
	JagDecSubCodeCondition *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_CONDITION, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeConditionPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->with_zero = with_zero;
	priv->condition_type = condition_type;
	return result;
}


/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeConditionPrivate *priv = JAG_DEC_SUB_CODE_CONDITION_GET_PRIVATE(self);

	JagDecIConsumer *left_consumer = jag_dec_isub_code_get_input_consumer_at(self, 1);
	JagDecIConsumer *right_consumer = jag_dec_isub_code_get_input_consumer_at(self, 0);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);

	JagAstIExpression *rightExpression = jag_dec_iconsumer_get_expression_ref(right_consumer, NULL);
	JagAstIConditionalExpression *outputExpression = NULL;
	if (priv->with_zero) {
		JagDecValueTypeDefinitionList *right_type_set = jag_dec_consumer_get_type_set((JagDecConsumer *) right_consumer);
		JagAstDeclarationType *bestType = jag_dec_value_type_definition_list_get_best_type_ref(right_type_set);
		if (bestType!=NULL) {
			if(jag_ast_declaration_type_get_dim_count(bestType)==0) {
				if (jag_ast_declaration_type_get_primitive_type(bestType) == JAG_BYT_TYPE_BOOLEAN) {

					JagAstIConditionalExpression *condExpr = NULL;
					if (JAG_AST_IS_ICONDITIONAL_EXPRESSION(rightExpression)) {
						condExpr = JAG_AST_ICONDITIONAL_EXPRESSION(rightExpression);
						cat_ref_ptr(condExpr);
					} else {
						condExpr = (JagAstIConditionalExpression *) jag_ast_wrapped_conditional_expression_new(rightExpression);
					}
					cat_unref_ptr(rightExpression);

					if (priv->condition_type==JAG_BYT_CONDITION_TYPE_EQUAL) {
						outputExpression = jag_ast_iconditional_expression_invert(condExpr);
					} else if (priv->condition_type==JAG_BYT_CONDITION_TYPE_NOT_EQUAL) {
						outputExpression = cat_ref_ptr(condExpr);
					}

					cat_unref_ptr(condExpr);

					if (outputExpression!=NULL) {
						jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) value_producer, (JagAstIExpression *) outputExpression);
						cat_unref_ptr(outputExpression);
						cat_unref_ptr(bestType);
						return;
					}
				}
			}
		}
		cat_unref_ptr(bestType);
	}

	JagAstIExpression *leftExpression = jag_dec_iconsumer_get_expression_ref(left_consumer, NULL);
	outputExpression = (JagAstIConditionalExpression *) jag_ast_relational_expression_new(priv->condition_type, leftExpression, rightExpression);
	jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) value_producer, (JagAstIExpression *) outputExpression);
	cat_unref_ptr(outputExpression);
	cat_unref_ptr(rightExpression);
	cat_unref_ptr(leftExpression);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
