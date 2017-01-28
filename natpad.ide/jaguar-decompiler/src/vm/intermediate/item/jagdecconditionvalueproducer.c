/*
   File:    jagdecconditionvalueproducer.c
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

#include "jagdecconditionvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecConditionValueProducer"
#include <logging/catlog.h>

struct _JagDecConditionValueProducerPrivate {
	JagDecConsumer *consumer;
	gboolean inverted_expression;
};

static void l_value_producer_iface_init(JagDecIValueProducerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecConditionValueProducer, jag_dec_condition_value_producer, JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IVALUE_PRODUCER, l_value_producer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_condition_value_producer_class_init(JagDecConditionValueProducerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecConditionValueProducerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_condition_value_producer_init(JagDecConditionValueProducer *instance) {
	JagDecConditionValueProducerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER, JagDecConditionValueProducerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecConditionValueProducer *instance = JAG_DEC_CONDITION_VALUE_PRODUCER(object);
	JagDecConditionValueProducerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->consumer);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecConditionValueProducer *jag_dec_condition_value_producer_new(JagDecIIntermediateNode *owner, JagDecConsumer *consumer, gboolean inverted_expression) {
	JagDecConditionValueProducer *result = g_object_new(JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER, NULL);
	cat_ref_anounce(result);
	JagDecConditionValueProducerPrivate *priv = result->priv;
	jag_dec_default_value_producer_construct((JagDecDefaultValueProducer *) result, owner);
	priv->consumer = cat_ref_ptr(consumer);
	priv->inverted_expression = inverted_expression;
	return result;
}



/********************* start JagDecIValueProducerInterface implementation *********************/


static JagAstIExpression *l_value_producer_get_expression_ref(JagDecIValueProducer *self, JagAstDeclarationType *requested) {
	JagDecConditionValueProducerPrivate *priv = JAG_DEC_CONDITION_VALUE_PRODUCER_GET_PRIVATE(self);

	JagAstDeclarationType *bool_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_BOOLEAN, 0);
	JagAstIConditionalExpression  *condExpression = (JagAstIConditionalExpression *) jag_dec_iconsumer_get_expression_ref((JagDecIConsumer *) priv->consumer, bool_decl_type);
	cat_unref_ptr(bool_decl_type);
	if (condExpression!=NULL && priv->inverted_expression) {
		JagAstIConditionalExpression  *oldExpression = condExpression;
		condExpression = jag_ast_iconditional_expression_invert(condExpression);
		cat_unref_ptr(oldExpression);
	}
	return (JagAstIExpression *) condExpression;
}

static JagDecValueTypeDefinitionList *l_value_producer_get_type_set(JagDecIValueProducer *self) {
	JagDecConditionValueProducerPrivate *priv = JAG_DEC_CONDITION_VALUE_PRODUCER_GET_PRIVATE(self);
	return jag_dec_consumer_get_type_set(priv->consumer);
}


static void l_value_producer_iface_init(JagDecIValueProducerInterface *iface) {
	JagDecIValueProducerInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->getExpressionRef = l_value_producer_get_expression_ref;
	iface->duplicate = p_iface->duplicate;
	iface->getTypeSet = l_value_producer_get_type_set;
	iface->setExpression = p_iface->setExpression;
	iface->setConsumer = p_iface->setConsumer;
	iface->isConsumed = p_iface->isConsumed;
}

/********************* end JagDecIValueProducerInterface implementation *********************/

