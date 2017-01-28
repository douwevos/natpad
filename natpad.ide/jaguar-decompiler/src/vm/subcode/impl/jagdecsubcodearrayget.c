/*
   File:    jagdecsubcodearrayget.c
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

#include "jagdecsubcodearrayget.h"
#include "../jagdecconsumer.h"
#include "../jagdeciconsumer.h"
#include "../jagdecivalueproducer.h"
#include "../jagdecisubcode.h"
#include "../../jagdecmodelstagetworuntime.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"
#include "../jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeArrayGet"
#include <logging/catlog.h>

struct _JagDecSubCodeArrayGetPrivate {
	void *dummy;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeArrayGet, jag_dec_sub_code_array_get, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_array_get_class_init(JagDecSubCodeArrayGetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeArrayGetPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_array_get_init(JagDecSubCodeArrayGet *instance) {
	JagDecSubCodeArrayGetPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_ARRAY_GET, JagDecSubCodeArrayGetPrivate);
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

JagDecSubCodeArrayGet *jag_dec_sub_code_array_get_new(int mnemonic_index) {
	JagDecSubCodeArrayGet *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_ARRAY_GET, NULL);
	cat_ref_anounce(result);
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	return result;
}






/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecIConsumer *index_consumer = jag_dec_isub_code_get_input_consumer_at(self, 0);
	JagDecIConsumer *instance_consumer = jag_dec_isub_code_get_input_consumer_at(self, 1);
	JagAstIExpression *expression = jag_dec_iconsumer_get_expression_ref(instance_consumer, NULL);
	JagAstDeclarationType *int_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_INT, 0);
	JagAstIExpression *index_expression = jag_dec_iconsumer_get_expression_ref(index_consumer, int_decl_type);
	cat_unref_ptr(int_decl_type);
	cat_log_debug("expression=%o, index_expression=%o", expression, index_expression);
	JagAstArrayAccessExpression *arrayAccessExpression = jag_ast_array_access_expression_new(expression, index_expression);
	cat_unref_ptr(expression);
	cat_unref_ptr(index_expression);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	jag_dec_ivalue_producer_set_expression(value_producer, (JagAstIExpression *) arrayAccessExpression);
	cat_unref_ptr(arrayAccessExpression);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/

