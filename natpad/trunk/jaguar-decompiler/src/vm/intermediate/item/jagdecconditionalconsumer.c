/*
   File:    jagdecconditionalconsumer.c
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

#include "jagdecconditionalconsumer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecConditionalConsumer"
#include <logging/catlog.h>

struct _JagDecConditionalConsumerPrivate {
	gboolean invert;
};


static void l_consumer_iface_init(JagDecIConsumerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecConditionalConsumer, jag_dec_conditional_consumer, JAG_DEC_TYPE_CONSUMER, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ICONSUMER, l_consumer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_conditional_consumer_class_init(JagDecConditionalConsumerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecConditionalConsumerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

}

static void jag_dec_conditional_consumer_init(JagDecConditionalConsumer *instance) {
	JagDecConditionalConsumerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_CONDITIONAL_CONSUMER, JagDecConditionalConsumerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecConditionalConsumer *instance = JAG_DEC_CONDITIONAL_CONSUMER(object);
//	JagDecConditionalConsumerPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecConditionalConsumer *jag_dec_conditional_consumer_new(JagDecIIntermediateNode *owner, gboolean invert) {
	JagDecConditionalConsumer *result = g_object_new(JAG_DEC_TYPE_CONDITIONAL_CONSUMER, NULL);
	cat_ref_anounce(result);
	JagDecConditionalConsumerPrivate *priv = result->priv;
	jag_dec_consumer_construct((JagDecConsumer *) result, owner);
	priv->invert = invert;
	return result;
}



/********************* start JagDecIConsumer implementation *********************/


static JagAstIExpression *l_consumer_get_expression_ref(JagDecIConsumer *self, JagAstDeclarationType *description) {
	JagAstDeclarationType *bool_decl_type = jag_ast_declaration_type_new(JAG_BYT_TYPE_BOOLEAN, 0);
	JagDecIConsumerInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_ICONSUMER_GET_INTERFACE(self));
	JagAstIExpression *result = p_iface->getExpressionRef(self, bool_decl_type);
	if (result) {
		JagDecConditionalConsumerPrivate *priv = JAG_DEC_CONDITIONAL_CONSUMER_GET_PRIVATE(self);
		if (priv->invert) {
			JagAstIConditionalExpression *conditional_expression = jag_ast_wrapped_conditional_expression_ensure_is_conditional(result);
			cat_unref_ptr(result);
			result = (JagAstIExpression *) jag_ast_iconditional_expression_invert(conditional_expression);
			cat_unref_ptr(conditional_expression);
		}
	}
	cat_unref_ptr(bool_decl_type);
	return result;
}

static void l_consumer_iface_init(JagDecIConsumerInterface *iface) {
	JagDecIConsumerInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->consume = p_iface->consume;
	iface->getExpressionRef = l_consumer_get_expression_ref;
	iface->getIntermediateNode = p_iface->getIntermediateNode;
	iface->getProducer = p_iface->getProducer;

}

/********************* end JagDecIConsumer implementation *********************/



