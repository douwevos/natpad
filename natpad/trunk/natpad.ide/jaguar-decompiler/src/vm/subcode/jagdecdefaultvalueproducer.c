/*
   File:    jagdecdefaultvalueproducer.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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

#include "jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecDefaultValueProducer"
#include <logging/catlog.h>

struct _JagDecDefaultValueProducerPrivate {
	JagDecValueTypeDefinitionList *type_set;
	JagDecExpressionHolder *expression_holder;
	JagDecIIntermediateNode *owner;
	CatArrayWo *e_input_consumers;
	JagDecIConsumer *consumer;
};

static void l_value_producer_iface_init(JagDecIValueProducerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecDefaultValueProducer, jag_dec_default_value_producer, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IVALUE_PRODUCER, l_value_producer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_default_value_producer_class_init(JagDecDefaultValueProducerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecDefaultValueProducerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_default_value_producer_init(JagDecDefaultValueProducer *instance) {
	JagDecDefaultValueProducerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, JagDecDefaultValueProducerPrivate);
	instance->priv = priv;
}

static void l_weak_notify_consumer(gpointer data, GObject *where_the_object_was) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(data);
	priv->consumer = NULL;
}

static void l_weak_notify_owner(gpointer data, GObject *where_the_object_was) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(data);
	priv->owner = NULL;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecDefaultValueProducer *instance = JAG_DEC_DEFAULT_VALUE_PRODUCER(object);
	JagDecDefaultValueProducerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->expression_holder);
	cat_unref_ptr(priv->type_set);
	cat_unref_ptr(priv->e_input_consumers);
	JagDecIConsumer *consumer = priv->consumer;
	priv->consumer = NULL;
	if (consumer) {
		g_object_weak_unref((GObject *) consumer, (GWeakNotify) l_weak_notify_consumer, instance);
	}
	JagDecIIntermediateNode *owner = priv->owner;
	priv->owner = NULL;
	if (owner) {
		g_object_weak_unref((GObject *) owner, (GWeakNotify) l_weak_notify_owner, instance);
	}
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_dec_default_value_producer_construct(JagDecDefaultValueProducer *producer, JagDecIIntermediateNode *owner) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(producer);
	priv->expression_holder = jag_dec_expression_holder_new();
	priv->type_set = jag_dec_value_type_definition_list_new();
	priv->consumer = NULL;
	priv->owner = owner;
	if (owner) {
		g_object_weak_ref((GObject *) owner, (GWeakNotify) l_weak_notify_owner, producer);
	}
	priv->e_input_consumers = cat_array_wo_new();
}


JagDecDefaultValueProducer *jag_dec_default_value_producer_new(JagDecIIntermediateNode *owner) {
	JagDecDefaultValueProducer *result = g_object_new(JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, NULL);
	cat_ref_anounce(result);
	jag_dec_default_value_producer_construct(result, owner);
	return result;
}


void jag_dec_default_value_producer_construct_with(JagDecDefaultValueProducer *producer, JagDecIIntermediateNode *owner, JagDecValueTypeDefinitionList *type_set, JagDecExpressionHolder *expression_holder) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(producer);
	priv->expression_holder = cat_ref_ptr(expression_holder);
	priv->type_set = cat_ref_ptr(type_set);
	priv->consumer = NULL;
	priv->owner = owner;
	if (owner) {
		g_object_weak_ref((GObject *) owner, (GWeakNotify) l_weak_notify_owner, producer);
	}
	priv->e_input_consumers = cat_array_wo_new();
}


JagDecDefaultValueProducer *jag_dec_default_value_producer_new_with(JagDecIIntermediateNode *owner, JagDecValueTypeDefinitionList *type_set, JagDecExpressionHolder *expression_holder) {
	JagDecDefaultValueProducer *result = g_object_new(JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, NULL);
	cat_ref_anounce(result);
	jag_dec_default_value_producer_construct_with(result, owner, type_set, expression_holder);
	return result;
}


JagDecExpressionHolder *jag_dec_default_value_producer_get_expression_holder(JagDecDefaultValueProducer *producer) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(producer);
	return priv->expression_holder;
}


/********************* start JagDecIValueProducerInterface implementation *********************/

static JagDecValueTypeDefinitionList *l_value_producer_get_type_set(JagDecIValueProducer *self) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	return priv->type_set;
}

static JagAstIExpression *l_value_producer_get_expression_ref(JagDecIValueProducer *self, JagAstDeclarationType *requested) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	JagAstIExpression *expression = jag_dec_expression_holder_get(priv->expression_holder);
	cat_ref_ptr(expression);
	if (requested) {
		if (JAG_AST_IS_LITERAL(expression)) {
			JagAstLiteral *lit = (JagAstLiteral *) expression;
			CatIStringable *lit_val = jag_ast_literal_get_value(lit);
			if (jag_ast_declaration_type_get_dim_count(requested)==0) {
				if (jag_ast_declaration_type_get_primitive_type(requested)==JAG_BYT_TYPE_BOOLEAN) {
					if (CAT_IS_INTEGER(lit_val)) {
						int liti = cat_integer_value((CatInteger *) lit_val);
						if (liti==0 || liti==1) {
							cat_unref_ptr(expression);
							CatBoolean *nbool = cat_boolean_new(liti==1);
							expression = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) nbool);
							cat_unref_ptr(nbool);
						}
					}
				} else if (jag_ast_declaration_type_get_primitive_type(requested)==JAG_BYT_TYPE_CHAR) {
					if (CAT_IS_INTEGER(lit_val)) {
						int liti = cat_integer_value((CatInteger *) lit_val);
						CatStringWo *e_t = cat_string_wo_new();
						cat_string_wo_append_char(e_t, (char) '\'');
						switch(liti) {
							case '\t' : {
								cat_string_wo_append_chars_len(e_t, "\\t", 2);
							} break;
							case '\r' : {
								cat_string_wo_append_chars_len(e_t, "\\r", 2);
							} break;
							case '\n' : {
								cat_string_wo_append_chars_len(e_t, "\\n", 2);
							} break;
							default : {
								cat_string_wo_append_char(e_t, (char) liti);
							} break;
						}
						cat_string_wo_append_char(e_t, (char) '\'');
						cat_unref_ptr(expression);
						expression = (JagAstIExpression *) jag_ast_identifier_new(cat_string_wo_anchor(e_t, 0));
					}
				}
			}
		}
	}
	return expression;
}

static void l_value_producer_set_expression(JagDecIValueProducer *self, JagAstIExpression *expression) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	jag_dec_expression_holder_set(priv->expression_holder, expression);
}

static void l_value_producer_set_consumer(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	if (priv->consumer) {
		g_object_weak_unref((GObject *) priv->consumer, (GWeakNotify) l_weak_notify_consumer, self);
		priv->consumer = NULL;
	}
	if (consumer) {
		priv->consumer = consumer;
		g_object_weak_ref((GObject *) consumer, (GWeakNotify) l_weak_notify_consumer, self);
	}

}

static struct _JagDecIConsumer *l_value_producer_get_consumer(JagDecIValueProducer *self) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	return priv->consumer;
}

static gboolean l_value_producer_is_consumed(JagDecIValueProducer *self) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	return priv->consumer!=NULL;
}

static JagDecIValueProducer *l_value_producer_duplicate(JagDecIValueProducer *self) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	return (JagDecIValueProducer *) jag_dec_default_value_producer_new_with(priv->owner, priv->type_set, priv->expression_holder);
}

static struct _JagDecIIntermediateNode *l_value_producer_get_intermediate_node(JagDecIValueProducer *self) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	return priv->owner;
}


static void l_value_producer_add_input_consumer(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	cat_array_wo_append(priv->e_input_consumers, (GObject *) consumer);
}

static CatArrayWo *l_value_producer_get_input_consumers(JagDecIValueProducer *self) {
	JagDecDefaultValueProducerPrivate *priv = JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(self);
	return priv->e_input_consumers;
}


static void l_value_producer_iface_init(JagDecIValueProducerInterface *iface) {
	iface->duplicate = l_value_producer_duplicate;
	iface->getTypeSet = l_value_producer_get_type_set;
	iface->getExpressionRef = l_value_producer_get_expression_ref;
	iface->setExpression = l_value_producer_set_expression;
	iface->setConsumer = l_value_producer_set_consumer;
	iface->getConsumer = l_value_producer_get_consumer;
	iface->isConsumed = l_value_producer_is_consumed;
	iface->getIntermediateNode = l_value_producer_get_intermediate_node;
	iface->addInputConsumer = l_value_producer_add_input_consumer;
	iface->getInputConsumers = l_value_producer_get_input_consumers;
}

/********************* end JagDecIValueProducerInterface implementation *********************/
