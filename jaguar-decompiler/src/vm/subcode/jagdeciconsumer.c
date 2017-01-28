/*
   File:    jagdeciconsumer.c
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

#include "jagdeciconsumer.h"

G_DEFINE_INTERFACE(JagDecIConsumer, jag_dec_iconsumer, G_TYPE_OBJECT);

static void jag_dec_iconsumer_default_init(JagDecIConsumerInterface *klass) {
}

void jag_dec_iconsumer_consume(JagDecIConsumer *self, struct _JagDecIValueProducer *value_producer) {
	g_return_if_fail(JAG_DEC_IS_ICONSUMER(self));
	JAG_DEC_ICONSUMER_GET_INTERFACE(self)->consume(self, value_producer);
}


JagAstIExpression *jag_dec_iconsumer_get_expression_ref(JagDecIConsumer *self, JagAstDeclarationType *description) {
	g_return_val_if_fail(JAG_DEC_IS_ICONSUMER(self), NULL);
	return JAG_DEC_ICONSUMER_GET_INTERFACE(self)->getExpressionRef(self, description);

}

struct _JagDecIIntermediateNode *jag_dec_iconsumer_get_intermediate_node(JagDecIConsumer *self) {
	g_return_val_if_fail(JAG_DEC_IS_ICONSUMER(self), NULL);
	return JAG_DEC_ICONSUMER_GET_INTERFACE(self)->getIntermediateNode(self);
}

struct _JagDecIValueProducer *jag_dec_iconsumer_get_producer(JagDecIConsumer *self) {
	g_return_val_if_fail(JAG_DEC_IS_ICONSUMER(self), NULL);
	return JAG_DEC_ICONSUMER_GET_INTERFACE(self)->getProducer(self);
}
