/*
   File:    jagdecarraygetinstanceconsumer.h
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


#ifndef JAGDECARRAYGETINSTANCECONSUMER_H_
#define JAGDECARRAYGETINSTANCECONSUMER_H_

#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/jagdecivalueproducer.h"
#include "../jagdeciintermediatenode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER              (jag_dec_array_get_instance_consumer_get_type())
#define JAG_DEC_ARRAY_GET_INSTANCE_CONSUMER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_array_get_instance_consumer_get_type(), JagDecArrayGetInstanceConsumer))
#define JAG_DEC_ARRAY_GET_INSTANCE_CONSUMER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER, JagDecArrayGetInstanceConsumerClass))
#define JAG_DEC_IS_ARRAY_GET_INSTANCE_CONSUMER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER))
#define JAG_DEC_IS_ARRAY_GET_INSTANCE_CONSUMER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER))
#define JAG_DEC_ARRAY_GET_INSTANCE_CONSUMER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER, JagDecArrayGetInstanceConsumerClass))
#define JAG_DEC_ARRAY_GET_INSTANCE_CONSUMER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_ARRAY_GET_INSTANCE_CONSUMER, JagDecArrayGetInstanceConsumerPrivate))

typedef struct _JagDecArrayGetInstanceConsumer               JagDecArrayGetInstanceConsumer;
typedef struct _JagDecArrayGetInstanceConsumerPrivate        JagDecArrayGetInstanceConsumerPrivate;
typedef struct _JagDecArrayGetInstanceConsumerClass          JagDecArrayGetInstanceConsumerClass;


struct _JagDecArrayGetInstanceConsumer {
	JagDecConsumer parent;
	JagDecArrayGetInstanceConsumerPrivate *priv;
};

struct _JagDecArrayGetInstanceConsumerClass {
	JagDecConsumerClass parent_class;
};


GType jag_dec_array_get_instance_consumer_get_type();

JagDecArrayGetInstanceConsumer *jag_dec_array_get_instance_consumer_new(JagDecIIntermediateNode *owner, JagDecIValueProducer *result_value_producer);

G_END_DECLS


#endif /* JAGDECARRAYGETINSTANCECONSUMER_H_ */
