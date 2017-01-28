/*
   File:    jagdecsubcodeincrement.c
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

#include "jagdecsubcodeincrement.h"
#include "jagdecframevalueproducer.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"
#include <jaguarbytecode.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeIncrement"
#include <logging/catlog.h>

struct _JagDecSubCodeIncrementPrivate {
	JagDecMethodFrameValue *method_frame_value;
	int increment_value;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeIncrement, jag_dec_sub_code_increment, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_increment_class_init(JagDecSubCodeIncrementClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeIncrementPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_increment_init(JagDecSubCodeIncrement *instance) {
	JagDecSubCodeIncrementPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_INCREMENT, JagDecSubCodeIncrementPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeIncrement *instance = JAG_DEC_SUB_CODE_INCREMENT(object);
	JagDecSubCodeIncrementPrivate *priv = instance->priv;
	cat_unref_ptr(priv->method_frame_value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeIncrement *jag_dec_sub_code_increment_new(int mnemonic_index, JagDecMethodFrameValue *method_frame_value, int increment_value) {
	JagDecSubCodeIncrement *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_INCREMENT, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeIncrementPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->method_frame_value = cat_ref_ptr(method_frame_value);
	priv->increment_value = increment_value;
	return result;
}





/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeIncrementPrivate *priv = JAG_DEC_SUB_CODE_INCREMENT_GET_PRIVATE(self);

	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	if ((value_producer!=NULL) && !jag_dec_ivalue_producer_is_consumed(value_producer)) {
		JagAstIExpression *expression = jag_dec_ivalue_producer_get_expression_ref(value_producer, NULL);
		if (JAG_AST_IS_INFIX_EXPRESSION(expression)) {
			JagAstIExpression *mfv_expr = jag_dec_method_frame_value_get_expression_ref(priv->method_frame_value, NULL);
			JagAstAssignment *assignment = jag_ast_assignment_new(mfv_expr, expression);
			cat_unref_ptr(mfv_expr);
			jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) assignment);
			cat_unref_ptr(assignment);
		} else {
			JagAstIStatement *expr_stat = (JagAstIStatement *) jag_ast_expression_statement_new(expression);
			jag_dec_model_stage_two_runtime_append_statement(modelRuntime, expr_stat);
			cat_unref_ptr(expr_stat);
		}
		cat_unref_ptr(expression);
	}
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
