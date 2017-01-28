/*
   File:    jagdecsubcodeconditionset.c
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

#include "jagdecsubcodeconditionset.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeConditionSet"
#include <logging/catlog.h>

struct _JagDecSubCodeConditionSetPrivate {
	int out_index;
	int parent_out_index;
	int final_out_index;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeConditionSet, jag_dec_sub_code_condition_set, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_condition_set_class_init(JagDecSubCodeConditionSetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeConditionSetPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_condition_set_init(JagDecSubCodeConditionSet *instance) {
	JagDecSubCodeConditionSetPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_CONDITION_SET, JagDecSubCodeConditionSetPrivate);
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

JagDecSubCodeConditionSet *jag_dec_sub_code_condition_set_new(int mnemonic_index, int out_index, int parent_out_index, int final_out_index) {
	JagDecSubCodeConditionSet *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_CONDITION_SET, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeConditionSetPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->out_index = out_index;
	priv->parent_out_index = parent_out_index;
	priv->final_out_index = final_out_index;
	return result;
}





/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeConditionSetPrivate *priv = JAG_DEC_SUB_CODE_CONDITION_SET_GET_PRIVATE(self);

	JagAstDeclarationType *bool_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_BOOLEAN, 0);
	CatArrayWo *e_expressions = cat_array_wo_new();
	int idx = 0;
	while(TRUE) {
		JagDecIConsumer *consumer = jag_dec_isub_code_get_input_consumer_at(self, idx);
		if (consumer==NULL) {
			break;
		}
		JagAstIExpression *expression = jag_dec_iconsumer_get_expression_ref(consumer, bool_decl_type);
		cat_array_wo_append(e_expressions, (GObject *) expression);
		cat_unref(expression);
		idx++;
	}
	cat_unref_ptr(bool_decl_type);

	gboolean is_last_and_inverted = priv->parent_out_index!=priv->final_out_index;
	JagBytMathOperator logical_operator = (priv->out_index==priv->parent_out_index)^is_last_and_inverted ? JAG_BYT_MATH_OPERATOR_LOGICAL_OR : JAG_BYT_MATH_OPERATOR_LOGICAL_AND;
	JagAstLogicalExpression *logical_expression = jag_ast_logical_expression_new(logical_operator, e_expressions);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	jag_dec_ivalue_producer_set_expression(value_producer, (JagAstIExpression *) logical_expression);
	cat_unref_ptr(logical_expression);
	cat_unref_ptr(e_expressions);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/

