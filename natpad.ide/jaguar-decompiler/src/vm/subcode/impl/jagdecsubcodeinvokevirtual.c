/*
   File:    jagdecsubcodeinvokevirtual.c
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

#include "jagdecsubcodeinvokevirtual.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeInvokeVirtual"
#include <logging/catlog.h>

struct _JagDecSubCodeInvokeVirtualPrivate {
	JagBytMethodHeader *method_header;
	int argument_count;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeInvokeVirtual, jag_dec_sub_code_invoke_virtual, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_invoke_virtual_class_init(JagDecSubCodeInvokeVirtualClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeInvokeVirtualPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_invoke_virtual_init(JagDecSubCodeInvokeVirtual *instance) {
	JagDecSubCodeInvokeVirtualPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_INVOKE_VIRTUAL, JagDecSubCodeInvokeVirtualPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeInvokeVirtual *instance = JAG_DEC_SUB_CODE_INVOKE_VIRTUAL(object);
	JagDecSubCodeInvokeVirtualPrivate *priv = instance->priv;
	cat_unref_ptr(priv->method_header);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeInvokeVirtual *jag_dec_sub_code_invoke_virtual_new(int mnemonic_index, JagBytMethodHeader *method_header, int argument_count) {
	JagDecSubCodeInvokeVirtual *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_INVOKE_VIRTUAL, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeInvokeVirtualPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->method_header = cat_ref_ptr(method_header);
	priv->argument_count = argument_count;
	return result;
}






/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeInvokeVirtualPrivate *priv = JAG_DEC_SUB_CODE_INVOKE_VIRTUAL_GET_PRIVATE(self);

	CatArrayWo *e_args = cat_array_wo_new();
	int idx;
	for(idx=0; idx<priv->argument_count; idx++) {
		JagDecIConsumer *arg_consumer = jag_dec_isub_code_get_input_consumer_at(self, priv->argument_count-1-idx);
		JagAstIExpression *arg_expression = jag_dec_iconsumer_get_expression_ref(arg_consumer, NULL);
		cat_array_wo_append(e_args, (GObject *) arg_expression);
		cat_unref_ptr(arg_expression);
	}



	JagAstIExpression *outputExpression = NULL;
	JagAstIExpression *instance_expression = jag_dec_iconsumer_get_expression_ref(jag_dec_isub_code_get_input_consumer_at(self, priv->argument_count), NULL);

	JagAstIdentifier *method_name_id = jag_ast_identifier_new(jag_byt_method_header_get_method_name(priv->method_header));
	outputExpression = (JagAstIExpression *) jag_ast_invocation_expression_new(instance_expression, method_name_id, e_args);
	cat_unref_ptr(method_name_id);
	cat_unref_ptr(e_args);
	cat_unref_ptr(instance_expression);

	gboolean isConsumed = FALSE;
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	if (value_producer!=NULL) {
		jag_dec_ivalue_producer_set_expression(value_producer, outputExpression);
		isConsumed = jag_dec_ivalue_producer_is_consumed(value_producer);
	}

	/* valueProducer neither exists or wasn't consumed, so we create an expression-statement */
	if (!isConsumed) {
		JagAstExpressionStatement *expression_statement = jag_ast_expression_statement_new(outputExpression);
		jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) expression_statement);
		cat_unref_ptr(expression_statement);
	}
	cat_unref_ptr(outputExpression);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/

