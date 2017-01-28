/*
   File:    jagdecconsumer.c
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

#include "jagdecconsumer.h"
#include "jagdecivalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecConsumer"
#include <logging/catlog.h>

struct _JagDecConsumerPrivate {
	JagDecIValueProducer *value_producer;
	JagDecIIntermediateNode *owner;
};

static void l_consumer_iface_init(JagDecIConsumerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecConsumer, jag_dec_consumer, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ICONSUMER, l_consumer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_consumer_class_init(JagDecConsumerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecConsumerPrivate));


	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_consumer_init(JagDecConsumer *instance) {
	JagDecConsumerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_CONSUMER, JagDecConsumerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecConsumer *instance = JAG_DEC_CONSUMER(object);
	JagDecConsumerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->value_producer);
	cat_unref_ptr(priv->owner);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_dec_consumer_construct(JagDecConsumer *consumer, JagDecIIntermediateNode *owner) {
	JagDecConsumerPrivate *priv = JAG_DEC_CONSUMER_GET_PRIVATE(consumer);
	priv->value_producer = NULL;
	priv->owner = cat_ref_ptr(owner);
}


JagDecConsumer *jag_dec_consumer_new(JagDecIIntermediateNode *owner) {
	JagDecConsumer *result = g_object_new(JAG_DEC_TYPE_CONSUMER, NULL);
	cat_ref_anounce(result);
	jag_dec_consumer_construct(result, owner);
	return result;
}



JagDecValueTypeDefinitionList *jag_dec_consumer_get_type_set(JagDecConsumer *consumer) {
	JagDecConsumerPrivate *priv = JAG_DEC_CONSUMER_GET_PRIVATE(consumer);

	JagDecValueTypeDefinitionList *result = NULL;
	if (priv->value_producer) {
		result = jag_dec_ivalue_producer_get_type_set(priv->value_producer);
	} else {
		cat_log_error("no producer for consumer");
	}
	return result;
}


gboolean jag_dec_consumer_is_consumed(JagDecConsumer *consumer) {
	return JAG_DEC_CONSUMER_GET_PRIVATE(consumer)->value_producer != NULL;
}



/********************* start JagDecIConsumer implementation *********************/

static void l_consumer_consume(JagDecIConsumer *self, struct _JagDecIValueProducer *value_producer) {
	JagDecConsumerPrivate *priv = JAG_DEC_CONSUMER_GET_PRIVATE(self);
	cat_unref_ptr(priv->value_producer);
	priv->value_producer = cat_ref_ptr(value_producer);
}


static JagAstIExpression *l_consumer_get_expression_ref(JagDecIConsumer *self, JagAstDeclarationType *description) {
	if (description==NULL) {
		JagDecValueTypeDefinitionList *type_set = jag_dec_consumer_get_type_set((JagDecConsumer *) self);
		if (type_set) {
			description = jag_dec_value_type_definition_list_get_best_type_ref(type_set);
		}
	} else {
		cat_ref_ptr(description);
	}
	JagDecConsumerPrivate *priv = JAG_DEC_CONSUMER_GET_PRIVATE(self);
	JagAstIExpression *result = NULL;
	if (priv->value_producer) {
		result = jag_dec_ivalue_producer_get_expression_ref(priv->value_producer, description);
	} else {
		cat_log_error("no value_producer for consumer:%p", self);
	}
	cat_unref_ptr(description);
	return result;
}

static struct _JagDecIIntermediateNode *l_consumer_get_intermediate_node(JagDecIConsumer *self) {
	JagDecConsumerPrivate *priv = JAG_DEC_CONSUMER_GET_PRIVATE(self);
	return priv->owner;
}

static struct _JagDecIValueProducer *l_consumer_get_producer(JagDecIConsumer *self) {
	JagDecConsumerPrivate *priv = JAG_DEC_CONSUMER_GET_PRIVATE(self);
	return priv->value_producer;
}


static void l_consumer_iface_init(JagDecIConsumerInterface *iface) {
	iface->consume = l_consumer_consume;
	iface->getExpressionRef = l_consumer_get_expression_ref;
	iface->getIntermediateNode = l_consumer_get_intermediate_node;
	iface->getProducer = l_consumer_get_producer;
}

/********************* end JagDecIConsumer implementation *********************/



