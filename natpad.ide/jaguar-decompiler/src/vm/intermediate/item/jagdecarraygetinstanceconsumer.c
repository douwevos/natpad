/*
   File:    jagdecarraygetinstanceconsumer.c
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

#include "jagdecarraygetinstanceconsumer.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../../type/jagdeccrossrulearrayget.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecArrayGetInstanceConsumer"
#include <logging/catlog.h>

struct _JagDecArrayGetInstanceConsumerPrivate {
	JagDecIValueProducer *result_value_producer;
};

static void l_consumer_iface_init(JagDecIConsumerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecArrayGetInstanceConsumer, jag_dec_array_get_instance_consumer, JAG_DEC_TYPE_CONSUMER, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ICONSUMER, l_consumer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_array_get_instance_consumer_class_init(JagDecArrayGetInstanceConsumerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecArrayGetInstanceConsumerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_array_get_instance_consumer_init(JagDecArrayGetInstanceConsumer *instance) {
	JagDecArrayGetInstanceConsumerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER, JagDecArrayGetInstanceConsumerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecArrayGetInstanceConsumer *instance = JAG_DEC_ARRAY_GET_INSTANCE_CONSUMER(object);
//	JagDecArrayGetInstanceConsumerPrivate *priv = instance->priv;
//	cat_unref_ptr(priv->result_value_producer);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecArrayGetInstanceConsumer *jag_dec_array_get_instance_consumer_new(JagDecIIntermediateNode *owner, JagDecIValueProducer *result_value_producer) {
	JagDecArrayGetInstanceConsumer *result = g_object_new(JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER, NULL);
	cat_ref_anounce(result);
	JagDecArrayGetInstanceConsumerPrivate *priv = result->priv;
	jag_dec_consumer_construct((JagDecConsumer *) result, owner);

//	priv->result_value_producer = cat_ref_ptr(result_value_producer);
	priv->result_value_producer = result_value_producer;	/* TODO create weakref */
	return result;
}



/********************* start JagDecIConsumer implementation *********************/

static void l_consumer_consume(JagDecIConsumer *self, struct _JagDecIValueProducer *value_producer) {
	JagDecIConsumerInterface *piface = g_type_interface_peek_parent(JAG_DEC_ICONSUMER_GET_INTERFACE(self));
	piface->consume(self, value_producer);

	JagDecArrayGetInstanceConsumerPrivate *priv = JAG_DEC_ARRAY_GET_INSTANCE_CONSUMER_GET_PRIVATE(self);

	JagDecValueTypeDefinitionList *result_type_set = jag_dec_ivalue_producer_get_type_set(priv->result_value_producer);

	JagDecValueTypeDefinitionList *instance_type_set = jag_dec_consumer_get_type_set(JAG_DEC_CONSUMER(self));

	JagDecCrossRuleArrayGet *cross_rule = jag_dec_cross_rule_array_get_new(result_type_set, instance_type_set);
	jag_dec_value_type_definition_list_add_cross_rule(result_type_set, (JagDecICrossRule *) cross_rule);
	cat_unref_ptr(cross_rule);


}

static void l_consumer_iface_init(JagDecIConsumerInterface *iface) {
	JagDecIConsumerInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->consume = l_consumer_consume;
	iface->getExpressionRef = p_iface->getExpressionRef;
	iface->getIntermediateNode = p_iface->getIntermediateNode;
	iface->getProducer = p_iface->getProducer;
}

/********************* end JagDecIConsumer implementation *********************/




