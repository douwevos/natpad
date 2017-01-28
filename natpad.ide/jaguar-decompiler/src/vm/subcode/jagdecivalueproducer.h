/*
   File:    jagdecivalueproducer.h
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


#ifndef JAGDECIVALUEPRODUCER_H_
#define JAGDECIVALUEPRODUCER_H_

#include "../../type/jagdecvaluetypedefinitionlist.h"
#include <glib-object.h>

#define JAG_DEC_TYPE_IVALUE_PRODUCER                 (jag_dec_ivalue_producer_get_type())
#define JAG_DEC_IVALUE_PRODUCER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_DEC_TYPE_IVALUE_PRODUCER, JagDecIValueProducer))
#define JAG_DEC_IS_IVALUE_PRODUCER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_DEC_TYPE_IVALUE_PRODUCER))
#define JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_DEC_TYPE_IVALUE_PRODUCER, JagDecIValueProducerInterface))


typedef struct _JagDecIValueProducer               JagDecIValueProducer; /* dummy object */
typedef struct _JagDecIValueProducerInterface      JagDecIValueProducerInterface;

struct _JagDecIConsumer;

struct _JagDecIValueProducerInterface {
	GTypeInterface parent_iface;

	JagDecValueTypeDefinitionList *(*getTypeSet)(JagDecIValueProducer *self);

	JagAstIExpression *(*getExpressionRef)(JagDecIValueProducer *self, JagAstDeclarationType *description);
	void (*setExpression)(JagDecIValueProducer *self, JagAstIExpression *expression);

	void (*setConsumer)(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer);
	struct _JagDecIConsumer *(*getConsumer)(JagDecIValueProducer *self);
	gboolean (*isConsumed)(JagDecIValueProducer *self);

	JagDecIValueProducer *(*duplicate)(JagDecIValueProducer *self);

	struct _JagDecIIntermediateNode *(*getIntermediateNode)(JagDecIValueProducer *self);

	void (*addInputConsumer)(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer);
	CatArrayWo *(*getInputConsumers)(JagDecIValueProducer *self);

};

GType jag_dec_ivalue_producer_get_type(void);


JagDecValueTypeDefinitionList *jag_dec_ivalue_producer_get_type_set(JagDecIValueProducer *self);

JagAstIExpression *jag_dec_ivalue_producer_get_expression_ref(JagDecIValueProducer *self, JagAstDeclarationType *description);
void jag_dec_ivalue_producer_set_expression(JagDecIValueProducer *self, JagAstIExpression *expression);

void jag_dec_ivalue_producer_set_consumer(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer);
struct _JagDecIConsumer *jag_dec_ivalue_producer_get_consumer(JagDecIValueProducer *self);
gboolean jag_dec_ivalue_producer_is_consumed(JagDecIValueProducer *self);

JagDecIValueProducer *jag_dec_ivalue_producer_duplicate(JagDecIValueProducer *self);


struct _JagDecIIntermediateNode *jag_dec_ivalue_producer_get_intermediate_node(JagDecIValueProducer *self);
void jag_dec_ivalue_producer_add_input_consumer(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer);
CatArrayWo *jag_dec_ivalue_producer_get_input_consumers(JagDecIValueProducer *self);


CatStringWo *jag_dec_ivalue_producer_to_string(JagDecIValueProducer *producer);

#endif /* JAGDECIVALUEPRODUCER_H_ */
