/*
   File:    jagdecconditionalconsumer.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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


#ifndef JAGDECCONDITIONALCONSUMER_H_
#define JAGDECCONDITIONALCONSUMER_H_

#include "../../subcode/jagdecconsumer.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_CONDITIONAL_CONSUMER              (jag_dec_conditional_consumer_get_type())
#define JAG_DEC_CONDITIONAL_CONSUMER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_conditional_consumer_get_type(), JagDecConditionalConsumer))
#define JAG_DEC_CONDITIONAL_CONSUMER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_CONDITIONAL_CONSUMER, JagDecConditionalConsumerClass))
#define JAG_DEC_IS_CONDITIONAL_CONSUMER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_CONDITIONAL_CONSUMER))
#define JAG_DEC_IS_CONDITIONAL_CONSUMER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_CONDITIONAL_CONSUMER))
#define JAG_DEC_CONDITIONAL_CONSUMER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_CONDITIONAL_CONSUMER, JagDecConditionalConsumerClass))
#define JAG_DEC_CONDITIONAL_CONSUMER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_CONDITIONAL_CONSUMER, JagDecConditionalConsumerPrivate))

typedef struct _JagDecConditionalConsumer               JagDecConditionalConsumer;
typedef struct _JagDecConditionalConsumerPrivate        JagDecConditionalConsumerPrivate;
typedef struct _JagDecConditionalConsumerClass          JagDecConditionalConsumerClass;


struct _JagDecConditionalConsumer {
	JagDecConsumer parent;
	JagDecConditionalConsumerPrivate *priv;
};

struct _JagDecConditionalConsumerClass {
	JagDecConsumerClass parent_class;
};


GType jag_dec_conditional_consumer_get_type();

JagDecConditionalConsumer *jag_dec_conditional_consumer_new(JagDecIIntermediateNode *owner, gboolean invert);

G_END_DECLS


#endif /* JAGDECCONDITIONALCONSUMER_H_ */
