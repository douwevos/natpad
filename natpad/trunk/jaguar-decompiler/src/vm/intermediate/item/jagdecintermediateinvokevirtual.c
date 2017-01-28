/*
   File:    jagdecintermediateinvokevirtual.c
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

#include "jagdecintermediateinvokevirtual.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/impl/jagdecsubcodeinvokevirtual.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateInvokeVirtual"
#include <logging/catlog.h>

struct _JagDecIntermediateInvokeVirtualPrivate {
	JagBytMethodHeader *method_header;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateInvokeVirtual, jag_dec_intermediate_invoke_virtual, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_invoke_virtual_class_init(JagDecIntermediateInvokeVirtualClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateInvokeVirtualPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_invoke_virtual_init(JagDecIntermediateInvokeVirtual *instance) {
	JagDecIntermediateInvokeVirtualPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL, JagDecIntermediateInvokeVirtualPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateInvokeVirtual *instance = JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL(object);
	JagDecIntermediateInvokeVirtualPrivate *priv = instance->priv;
	cat_unref_ptr(priv->method_header);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagDecIntermediateInvokeVirtual *jag_dec_intermediate_invoke_virtual_new(int mnemonic_index, JagBytMethodHeader *method_header) {
	JagDecIntermediateInvokeVirtual *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateInvokeVirtualPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->method_header = cat_ref_ptr(method_header);
	return result;
}







/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateInvokeVirtualPrivate *priv = JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL_GET_PRIVATE(self);
	JagAstArgumentList *argument_list = jag_byt_method_header_get_argument_list(priv->method_header);
	int argument_count = jag_ast_argument_list_count(argument_list);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeInvokeVirtual *sub_code_invoke = jag_dec_sub_code_invoke_virtual_new(mnemonic_index, priv->method_header, argument_count);

	int idx;
	for(idx=argument_count-1; idx>=0; idx--) {
		JagDecConsumer *argument_consumer = jag_dec_consumer_new(self);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_invoke, (JagDecIConsumer *) argument_consumer);
		jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) argument_consumer);
		cat_unref_ptr(argument_consumer);
	}

	JagDecConsumer *instance_consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_invoke, (JagDecIConsumer *) instance_consumer);
	jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) instance_consumer);
	cat_unref_ptr(instance_consumer);


	JagDecIValueProducer *valueProducer = NULL;
	gboolean isVoid = FALSE;
	JagAstDeclarationType *returnType = jag_byt_method_header_get_return_type(priv->method_header);
	if (returnType!=NULL) {
		isVoid = jag_ast_declaration_type_is_void(returnType);
	}
	if (!isVoid) {
		valueProducer = (JagDecIValueProducer *) jag_dec_default_value_producer_new(self);
		JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set(valueProducer);
		JagDecValueTypeDefinition *definition = jag_dec_value_type_definition_new(returnType, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_RETURN);
		jag_dec_value_type_definition_list_add_definition(type_set, definition);
		cat_unref(definition);
	}


	if (valueProducer!=NULL) {
		jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_invoke, valueProducer);
		jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, valueProducer);
		cat_unref_ptr(valueProducer);
	}

	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_invoke);
	cat_unref_ptr(sub_code_invoke);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateInvokeVirtualPrivate *priv = JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_INVOKE_VIRTUAL(other_node)) {
		JagDecIntermediateInvokeVirtualPrivate *o_priv = JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL_GET_PRIVATE(other_node);
		return jag_byt_method_header_equal(priv->method_header, o_priv->method_header);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateInvokeVirtualPrivate *priv = JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_invoke_virtual_new(mnemonic_index, priv->method_header);
}


static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->isSame = l_intermediate_node_is_same;

	iface->contains = p_iface->contains;
	iface->getScopeRef = p_iface->getScopeRef;
	iface->resolveBreaks = p_iface->resolveBreaks;
	iface->tryPrepend = p_iface->tryPrepend;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/

