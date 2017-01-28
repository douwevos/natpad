/*
   File:    jagdecdefaultvalueproducer.h
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


#ifndef JAGDECDEFAULTVALUEPRODUCER_H_
#define JAGDECDEFAULTVALUEPRODUCER_H_

#include "jagdecivalueproducer.h"
#include "jagdeciconsumer.h"
#include "impl/jagdecexpressionholder.h"
#include "../intermediate/jagdeciintermediatenode.h"
#include "../../type/jagdecvaluetypedefinitionlist.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER              (jag_dec_default_value_producer_get_type())
#define JAG_DEC_DEFAULT_VALUE_PRODUCER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_default_value_producer_get_type(), JagDecDefaultValueProducer))
#define JAG_DEC_DEFAULT_VALUE_PRODUCER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, JagDecDefaultValueProducerClass))
#define JAG_DEC_IS_DEFAULT_VALUE_PRODUCER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER))
#define JAG_DEC_IS_DEFAULT_VALUE_PRODUCER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER))
#define JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, JagDecDefaultValueProducerClass))
#define JAG_DEC_DEFAULT_VALUE_PRODUCER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_DEFAULT_VALUE_PRODUCER, JagDecDefaultValueProducerPrivate))

typedef struct _JagDecDefaultValueProducer               JagDecDefaultValueProducer;
typedef struct _JagDecDefaultValueProducerPrivate        JagDecDefaultValueProducerPrivate;
typedef struct _JagDecDefaultValueProducerClass          JagDecDefaultValueProducerClass;


struct _JagDecDefaultValueProducer {
	GObject parent;
	JagDecDefaultValueProducerPrivate *priv;
};

struct _JagDecDefaultValueProducerClass {
	GObjectClass parent_class;
};


GType jag_dec_default_value_producer_get_type();

void jag_dec_default_value_producer_construct(JagDecDefaultValueProducer *producer, JagDecIIntermediateNode *owner);
void jag_dec_default_value_producer_construct_with(JagDecDefaultValueProducer *producer, JagDecIIntermediateNode *owner, JagDecValueTypeDefinitionList *type_set, JagDecExpressionHolder *expression_holder);

JagDecDefaultValueProducer *jag_dec_default_value_producer_new(JagDecIIntermediateNode *owner);
JagDecDefaultValueProducer *jag_dec_default_value_producer_new_with(JagDecIIntermediateNode *owner, JagDecValueTypeDefinitionList *type_set, JagDecExpressionHolder *expression_holder);

JagDecExpressionHolder *jag_dec_default_value_producer_get_expression_holder(JagDecDefaultValueProducer *producer);


G_END_DECLS


#endif /* JAGDECDEFAULTVALUEPRODUCER_H_ */
