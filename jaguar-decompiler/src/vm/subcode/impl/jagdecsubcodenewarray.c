/*
   File:    jagdecsubcodenewarray.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 14, 2012
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

#include "jagdecsubcodenewarray.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../jagdecconsumer.h"
#include "../jagdecivalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeNewArray"
#include <logging/catlog.h>

struct _JagDecSubCodeNewArrayPrivate {
	JagAstDeclarationType *array_declaration_type;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeNewArray, jag_dec_sub_code_new_array, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_new_array_class_init(JagDecSubCodeNewArrayClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeNewArrayPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_new_array_init(JagDecSubCodeNewArray *instance) {
	JagDecSubCodeNewArrayPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_NEW_ARRAY, JagDecSubCodeNewArrayPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeNewArray *instance = JAG_DEC_SUB_CODE_NEW_ARRAY(object);
	JagDecSubCodeNewArrayPrivate *priv = instance->priv;
	cat_unref_ptr(priv->array_declaration_type);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeNewArray *jag_dec_sub_code_new_array_new(int mnemonic_index, JagAstDeclarationType *array_declaration_type){
	JagDecSubCodeNewArray *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_NEW_ARRAY, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeNewArrayPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->array_declaration_type = cat_ref_ptr(array_declaration_type);
	return result;
}






/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeNewArrayPrivate *priv = JAG_DEC_SUB_CODE_NEW_ARRAY_GET_PRIVATE(self);
	CatArrayWo *e_dim_expressions = cat_array_wo_new_size(1);
	JagAstDeclarationType *int_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_INT ,0);
	JagDecIConsumer *consumer = jag_dec_isub_code_get_input_consumer_at(self, 0);
	JagAstIExpression *dim_expression = jag_dec_iconsumer_get_expression_ref(consumer, int_decl_type);
	cat_array_wo_append(e_dim_expressions, (GObject *) dim_expression);
	cat_unref_ptr(int_decl_type);
	cat_unref_ptr(dim_expression);
	JagAstIdentifier *plain_type_name = jag_dec_model_stage_two_runtime_create_plain_name(modelRuntime, priv->array_declaration_type, NULL);
	JagAstNewExpression *new_array_expressions = jag_ast_new_expression_new(plain_type_name, e_dim_expressions);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	jag_dec_ivalue_producer_set_expression(value_producer, (JagAstIExpression *) new_array_expressions);
	cat_unref_ptr(new_array_expressions);
	cat_unref_ptr(e_dim_expressions);
	cat_unref_ptr(plain_type_name);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/




