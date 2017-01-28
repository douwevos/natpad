/*
   File:    jagdecframevalueproducer.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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

#include "jagdecframevalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecFrameValueProducer"
#include <logging/catlog.h>

struct _JagDecFrameValueProducerPrivate {
	PostOrPrefixAppendix ppa;
};

static void l_value_producer_iface_init(JagDecIValueProducerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecFrameValueProducer, jag_dec_frame_value_producer, JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IVALUE_PRODUCER, l_value_producer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_frame_value_producer_class_init(JagDecFrameValueProducerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecFrameValueProducerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_frame_value_producer_init(JagDecFrameValueProducer *instance) {
	JagDecFrameValueProducerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_FRAME_VALUE_PRODUCER, JagDecFrameValueProducerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecFrameValueProducer *instance = JAG_DEC_FRAME_VALUE_PRODUCER(object);
//	JagDecFrameValueProducerPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecFrameValueProducer *jag_dec_frame_value_producer_new(JagDecIIntermediateNode *owner, JagDecMethodFrameValue *method_frame_value) {
	JagDecFrameValueProducer *result = g_object_new(JAG_DEC_TYPE_FRAME_VALUE_PRODUCER, NULL);
	cat_ref_anounce(result);
	JagDecFrameValueProducerPrivate *priv = result->priv;
	JagDecExpressionHolder *expression_holder = jag_dec_expression_holder_new();
	jag_dec_default_value_producer_construct_with((JagDecDefaultValueProducer *) result, owner, (JagDecValueTypeDefinitionList *) method_frame_value, expression_holder);
	cat_unref(expression_holder);
	priv->ppa = JAG_DEC_PPA_NONE;
	return result;
}


static JagDecFrameValueProducer *jag_dec_frame_value_producer_new_with(JagDecIIntermediateNode *owner, JagDecMethodFrameValue *method_frame_value, JagDecExpressionHolder *expression_holder) {
	JagDecFrameValueProducer *result = g_object_new(JAG_DEC_TYPE_FRAME_VALUE_PRODUCER, NULL);
	cat_ref_anounce(result);
	JagDecFrameValueProducerPrivate *priv = result->priv;
	jag_dec_default_value_producer_construct_with((JagDecDefaultValueProducer *) result, owner, (JagDecValueTypeDefinitionList *) method_frame_value, expression_holder);
	priv->ppa = JAG_DEC_PPA_NONE;
	return result;
}




PostOrPrefixAppendix jag_dec_frame_value_producer_get_ppa(JagDecFrameValueProducer *producer) {
	return JAG_DEC_FRAME_VALUE_PRODUCER_GET_PRIVATE(producer)->ppa;
}

void jag_dec_frame_value_producer_set_ppa(JagDecFrameValueProducer *producer, PostOrPrefixAppendix ppa) {
	JAG_DEC_FRAME_VALUE_PRODUCER_GET_PRIVATE(producer)->ppa = ppa;
}

int jag_dec_frame_value_producer_get_frame_index(JagDecFrameValueProducer *producer) {
	JagDecMethodFrameValue *method_frame_value = JAG_DEC_METHOD_FRAME_VALUE(jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) producer));
	return jag_dec_method_frame_value_get_frame_index(method_frame_value);
}








/********************* start JagDecIValueProducerInterface implementation *********************/


static JagAstIExpression *l_value_producer_get_expression_ref(JagDecIValueProducer *self, JagAstDeclarationType *description) {
	JagDecFrameValueProducerPrivate *priv = JAG_DEC_FRAME_VALUE_PRODUCER_GET_PRIVATE(self);
	JagDecExpressionHolder *expression_holder = jag_dec_default_value_producer_get_expression_holder((JagDecDefaultValueProducer *) self);
	JagAstIExpression *expression = jag_dec_expression_holder_get(expression_holder);
	if (expression) {
		return cat_ref_ptr(expression);
	}

	JagDecMethodFrameValue *method_frame_value = JAG_DEC_METHOD_FRAME_VALUE(jag_dec_ivalue_producer_get_type_set(self));
	expression = jag_dec_method_frame_value_get_expression_ref(method_frame_value, description);
	switch(priv->ppa) {
		case JAG_DEC_PPA_NONE : {
			return expression;
		}
		case JAG_DEC_PPA_POSTFIX_INCREMENT : {
			JagAstIExpression *result = (JagAstIExpression *) jag_ast_postfix_expression_new(TRUE, expression);
			cat_unref_ptr(expression);
			return result;
		}
		case JAG_DEC_PPA_POSTFIX_DECREMENT : {
			JagAstIExpression *result = (JagAstIExpression *) jag_ast_postfix_expression_new(FALSE, expression);
			cat_unref_ptr(expression);
			return result;
		}
		case JAG_DEC_PPA_PREFIX_INCREMENT : {
			JagAstIExpression *result = (JagAstIExpression *) jag_ast_prefix_expression_new(TRUE, expression);
			cat_unref_ptr(expression);
			return result;
		}
		case JAG_DEC_PPA_PREFIX_DECREMENT : {
			JagAstIExpression *result = (JagAstIExpression *) jag_ast_prefix_expression_new(FALSE, expression);
			cat_unref_ptr(expression);
			return result;
		}
	}
	return expression;
}

static JagDecIValueProducer *l_value_producer_duplicate(JagDecIValueProducer *self) {
	JagDecIIntermediateNode *owner = jag_dec_ivalue_producer_get_intermediate_node(self);
	JagDecMethodFrameValue *frame_value = JAG_DEC_METHOD_FRAME_VALUE(jag_dec_ivalue_producer_get_type_set(self));
	return (JagDecIValueProducer *) jag_dec_frame_value_producer_new_with(owner, frame_value, jag_dec_default_value_producer_get_expression_holder((JagDecDefaultValueProducer *) self));
}



static void l_value_producer_iface_init(JagDecIValueProducerInterface *iface) {
	JagDecIValueProducerInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->duplicate = l_value_producer_duplicate;
	iface->getExpressionRef = l_value_producer_get_expression_ref;

	iface->getTypeSet = p_iface->getTypeSet;
	iface->setExpression = p_iface->setExpression;
	iface->setConsumer = p_iface->setConsumer;
	iface->isConsumed = p_iface->isConsumed;
}

/********************* end JagDecIValueProducerInterface implementation *********************/
