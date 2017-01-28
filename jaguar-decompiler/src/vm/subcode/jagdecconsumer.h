/*
   File:    jagdecconsumer.h
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


#ifndef JAGDECCONSUMER_H_
#define JAGDECCONSUMER_H_

#include "jagdeciconsumer.h"
#include "../../type/jagdecvaluetypedefinitionlist.h"
#include "../intermediate/jagdeciintermediatenode.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_CONSUMER              (jag_dec_consumer_get_type())
#define JAG_DEC_CONSUMER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_consumer_get_type(), JagDecConsumer))
#define JAG_DEC_CONSUMER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_CONSUMER, JagDecConsumerClass))
#define JAG_DEC_IS_CONSUMER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_CONSUMER))
#define JAG_DEC_IS_CONSUMER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_CONSUMER))
#define JAG_DEC_CONSUMER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_CONSUMER, JagDecConsumerClass))
#define JAG_DEC_CONSUMER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_CONSUMER, JagDecConsumerPrivate))

typedef struct _JagDecConsumer               JagDecConsumer;
typedef struct _JagDecConsumerPrivate        JagDecConsumerPrivate;
typedef struct _JagDecConsumerClass          JagDecConsumerClass;


struct _JagDecConsumer {
	GObject parent;
	JagDecConsumerPrivate *priv;
};

struct _JagDecConsumerClass {
	GObjectClass parent_class;
};


GType jag_dec_consumer_get_type();

void jag_dec_consumer_construct(JagDecConsumer *consumer, JagDecIIntermediateNode *owner);

JagDecConsumer *jag_dec_consumer_new(JagDecIIntermediateNode *owner);

JagDecValueTypeDefinitionList *jag_dec_consumer_get_type_set(JagDecConsumer *consumer);

gboolean jag_dec_consumer_is_consumed(JagDecConsumer *consumer);


G_END_DECLS


#endif /* JAGDECCONSUMER_H_ */
