/*
   File:    jagdeciconsumer.h
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


#ifndef JAGDECICONSUMER_H_
#define JAGDECICONSUMER_H_

#include <jaguarbytecode.h>
#include <glib-object.h>

#define JAG_DEC_TYPE_ICONSUMER                 (jag_dec_iconsumer_get_type())
#define JAG_DEC_ICONSUMER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_DEC_TYPE_ICONSUMER, JagDecIConsumer))
#define JAG_DEC_IS_ICONSUMER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_DEC_TYPE_ICONSUMER))
#define JAG_DEC_ICONSUMER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_DEC_TYPE_ICONSUMER, JagDecIConsumerInterface))


typedef struct _JagDecIConsumer               JagDecIConsumer; /* dummy object */
typedef struct _JagDecIConsumerInterface      JagDecIConsumerInterface;

struct _JagDecIValueProducer;
struct _JagDecIIntermediateNode;

struct _JagDecIConsumerInterface {
	GTypeInterface parent_iface;

	void (*consume)(JagDecIConsumer *self, struct _JagDecIValueProducer *value_producer);
	JagAstIExpression *(*getExpressionRef)(JagDecIConsumer *self, JagAstDeclarationType *description);
	struct _JagDecIIntermediateNode *(*getIntermediateNode)(JagDecIConsumer *self);
	struct _JagDecIValueProducer *(*getProducer)(JagDecIConsumer *self);
};

GType jag_dec_iconsumer_get_type(void);

void jag_dec_iconsumer_consume(JagDecIConsumer *self, struct _JagDecIValueProducer *value_producer);
JagAstIExpression *jag_dec_iconsumer_get_expression_ref(JagDecIConsumer *self, JagAstDeclarationType *description);
struct _JagDecIIntermediateNode *jag_dec_iconsumer_get_intermediate_node(JagDecIConsumer *self);
struct _JagDecIValueProducer *jag_dec_iconsumer_get_producer(JagDecIConsumer *self);


#endif /* JAGDECICONSUMER_H_ */
