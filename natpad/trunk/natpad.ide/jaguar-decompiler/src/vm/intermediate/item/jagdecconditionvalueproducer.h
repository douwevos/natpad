/*
   File:    jagdecconditionvalueproducer.h
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


#ifndef JAGDECCONDITIONVALUEPRODUCER_H_
#define JAGDECCONDITIONVALUEPRODUCER_H_

#include "../../subcode/jagdecdefaultvalueproducer.h"
#include "../../subcode/jagdecconsumer.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER              (jag_dec_condition_value_producer_get_type())
#define JAG_DEC_CONDITION_VALUE_PRODUCER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_condition_value_producer_get_type(), JagDecConditionValueProducer))
#define JAG_DEC_CONDITION_VALUE_PRODUCER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER, JagDecConditionValueProducerClass))
#define JAG_DEC_IS_CONDITION_VALUE_PRODUCER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER))
#define JAG_DEC_IS_CONDITION_VALUE_PRODUCER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER))
#define JAG_DEC_CONDITION_VALUE_PRODUCER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER, JagDecConditionValueProducerClass))
#define JAG_DEC_CONDITION_VALUE_PRODUCER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_CONDITION_VALUE_PRODUCER, JagDecConditionValueProducerPrivate))

typedef struct _JagDecConditionValueProducer               JagDecConditionValueProducer;
typedef struct _JagDecConditionValueProducerPrivate        JagDecConditionValueProducerPrivate;
typedef struct _JagDecConditionValueProducerClass          JagDecConditionValueProducerClass;


struct _JagDecConditionValueProducer {
	JagDecDefaultValueProducer parent;
	JagDecConditionValueProducerPrivate *priv;
};

struct _JagDecConditionValueProducerClass {
	JagDecDefaultValueProducerClass parent_class;
};


GType jag_dec_condition_value_producer_get_type();

JagDecConditionValueProducer *jag_dec_condition_value_producer_new(JagDecIIntermediateNode *owner, JagDecConsumer *consumer, gboolean inverted_expression);

G_END_DECLS


#endif /* JAGDECCONDITIONVALUEPRODUCER_H_ */
