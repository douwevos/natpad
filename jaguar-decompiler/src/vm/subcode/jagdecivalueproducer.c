/*
   File:    jagdecivalueproducer.c
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

#include "jagdecivalueproducer.h"

G_DEFINE_INTERFACE(JagDecIValueProducer, jag_dec_ivalue_producer, G_TYPE_OBJECT);

static void jag_dec_ivalue_producer_default_init (JagDecIValueProducerInterface *klass) {
}


JagDecValueTypeDefinitionList *jag_dec_ivalue_producer_get_type_set(JagDecIValueProducer *self) {
	g_return_val_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self), NULL);
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->getTypeSet(self);
}

JagAstIExpression *jag_dec_ivalue_producer_get_expression_ref(JagDecIValueProducer *self, JagAstDeclarationType *description) {
	g_return_val_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self), NULL);
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->getExpressionRef(self, description);
}

void jag_dec_ivalue_producer_set_expression(JagDecIValueProducer *self, JagAstIExpression *expression) {
	g_return_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self));
	JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->setExpression(self, expression);
}

void jag_dec_ivalue_producer_set_consumer(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer) {
	g_return_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self));
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->setConsumer(self, consumer);
}

struct _JagDecIConsumer *jag_dec_ivalue_producer_get_consumer(JagDecIValueProducer *self) {
	g_return_val_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self), NULL);
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->getConsumer(self);
}

gboolean jag_dec_ivalue_producer_is_consumed(JagDecIValueProducer *self) {
	g_return_val_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self), FALSE);
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->isConsumed(self);
}

JagDecIValueProducer *jag_dec_ivalue_producer_duplicate(JagDecIValueProducer *self)  {
	g_return_val_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self), NULL);
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->duplicate(self);
}


struct _JagDecIIntermediateNode *jag_dec_ivalue_producer_get_intermediate_node(JagDecIValueProducer *self) {
	g_return_val_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self), NULL);
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->getIntermediateNode(self);
}

void jag_dec_ivalue_producer_add_input_consumer(JagDecIValueProducer *self, struct _JagDecIConsumer *consumer) {
	g_return_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self));
	JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->addInputConsumer(self, consumer);
}

CatArrayWo *jag_dec_ivalue_producer_get_input_consumers(JagDecIValueProducer *self) {
	g_return_val_if_fail(JAG_DEC_IS_IVALUE_PRODUCER(self), NULL);
	return JAG_DEC_IVALUE_PRODUCER_GET_INTERFACE(self)->getInputConsumers(self);
}


CatStringWo *jag_dec_ivalue_producer_to_string(JagDecIValueProducer *producer) {
	CatStringWo *e_result = cat_string_wo_new();
	cat_string_wo_append_chars(e_result, g_type_name_from_instance((GTypeInstance *) producer));
	JagAstIExpression *expr = jag_dec_ivalue_producer_get_expression_ref(producer, NULL);
	if (expr) {
		cat_string_wo_append_chars(e_result, "[expr=");
		cat_string_wo_append_chars(e_result, g_type_name_from_instance((GTypeInstance *) expr));
//		char buf[50];
//		sprintf(buf, "%p", expr);
		cat_string_wo_append_chars(e_result, " : ");
		cat_string_wo_append_hexadecimal(e_result, (unsigned long long) expr, -1);
//		cat_string_wo_append_chars(e_result, buf);
		cat_string_wo_append_chars(e_result, " ## ");
		JagAstWriter *w = jag_ast_writer_new();
		jag_ast_iexpression_write(expr, w);
		CatStringWo *a_expr_text = jag_ast_writer_to_string(w);
		cat_string_wo_append(e_result, a_expr_text);
		cat_unref_ptr(a_expr_text);
		cat_string_wo_append_chars(e_result, "]");
		cat_unref_ptr(w);
		cat_unref_ptr(expr);
	}
	return e_result;
}
