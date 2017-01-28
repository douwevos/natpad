/*
   File:    jagdecframevalueproducer.h
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


#ifndef JAGDECFRAMEVALUEPRODUCER_H_
#define JAGDECFRAMEVALUEPRODUCER_H_

#include "../jagdecdefaultvalueproducer.h"
#include "jagdecmethodframevalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

enum _PostOrPrefixAppendix {
	JAG_DEC_PPA_NONE,
	JAG_DEC_PPA_POSTFIX_INCREMENT,
	JAG_DEC_PPA_POSTFIX_DECREMENT,
	JAG_DEC_PPA_PREFIX_INCREMENT,
	JAG_DEC_PPA_PREFIX_DECREMENT,
};

typedef enum _PostOrPrefixAppendix PostOrPrefixAppendix;

#define JAG_DEC_TYPE_FRAME_VALUE_PRODUCER              (jag_dec_frame_value_producer_get_type())
#define JAG_DEC_FRAME_VALUE_PRODUCER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_frame_value_producer_get_type(), JagDecFrameValueProducer))
#define JAG_DEC_FRAME_VALUE_PRODUCER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_FRAME_VALUE_PRODUCER, JagDecFrameValueProducerClass))
#define JAG_DEC_IS_FRAME_VALUE_PRODUCER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_FRAME_VALUE_PRODUCER))
#define JAG_DEC_IS_FRAME_VALUE_PRODUCER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_FRAME_VALUE_PRODUCER))
#define JAG_DEC_FRAME_VALUE_PRODUCER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_FRAME_VALUE_PRODUCER, JagDecFrameValueProducerClass))
#define JAG_DEC_FRAME_VALUE_PRODUCER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_FRAME_VALUE_PRODUCER, JagDecFrameValueProducerPrivate))

typedef struct _JagDecFrameValueProducer               JagDecFrameValueProducer;
typedef struct _JagDecFrameValueProducerPrivate        JagDecFrameValueProducerPrivate;
typedef struct _JagDecFrameValueProducerClass          JagDecFrameValueProducerClass;


struct _JagDecFrameValueProducer {
	JagDecDefaultValueProducer parent;
	JagDecFrameValueProducerPrivate *priv;
};

struct _JagDecFrameValueProducerClass {
	JagDecDefaultValueProducerClass parent_class;
};


GType jag_dec_frame_value_producer_get_type();

JagDecFrameValueProducer *jag_dec_frame_value_producer_new(JagDecIIntermediateNode *owner, JagDecMethodFrameValue *method_frame_value);

PostOrPrefixAppendix jag_dec_frame_value_producer_get_ppa(JagDecFrameValueProducer *producer);

void jag_dec_frame_value_producer_set_ppa(JagDecFrameValueProducer *producer, PostOrPrefixAppendix ppa);

int jag_dec_frame_value_producer_get_frame_index(JagDecFrameValueProducer *producer);


G_END_DECLS


#endif /* JAGDECFRAMEVALUEPRODUCER_H_ */
