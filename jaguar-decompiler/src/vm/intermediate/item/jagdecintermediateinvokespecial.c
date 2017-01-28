/*
   File:    jagdecintermediateinvokespecial.c
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

#include "jagdecintermediateinvokespecial.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/impl/jagdecsubcodeinvokespecial.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateInvokeSpecial"
#include <logging/catlog.h>

struct _JagDecIntermediateInvokeSpecialPrivate {
	JagAstDeclarationType *instance_declaration_type;
	JagBytMethodHeader *method_header;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateInvokeSpecial, jag_dec_intermediate_invoke_special, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_invoke_special_class_init(JagDecIntermediateInvokeSpecialClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateInvokeSpecialPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_invoke_special_init(JagDecIntermediateInvokeSpecial *instance) {
	JagDecIntermediateInvokeSpecialPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_INVOKE_SPECIAL, JagDecIntermediateInvokeSpecialPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateInvokeSpecial *instance = JAG_DEC_INTERMEDIATE_INVOKE_SPECIAL(object);
	JagDecIntermediateInvokeSpecialPrivate *priv = instance->priv;
	cat_unref_ptr(priv->instance_declaration_type);
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

JagDecIntermediateInvokeSpecial *jag_dec_intermediate_invoke_special_new(int mnemonic_index, JagAstDeclarationType *instance_declaration_type, JagBytMethodHeader *method_header) {
	JagDecIntermediateInvokeSpecial *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_INVOKE_SPECIAL, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateInvokeSpecialPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->instance_declaration_type = cat_ref_ptr(instance_declaration_type);
	priv->method_header = cat_ref_ptr(method_header);
	return result;
}








/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateInvokeSpecialPrivate *priv = JAG_DEC_INTERMEDIATE_INVOKE_SPECIAL_GET_PRIVATE(self);
	JagAstArgumentList *argument_list = jag_byt_method_header_get_argument_list(priv->method_header);
	int argument_count = jag_ast_argument_list_count(argument_list);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeInvokeSpecial *sub_code_invoke = jag_dec_sub_code_invoke_special_new(mnemonic_index, priv->instance_declaration_type, priv->method_header, argument_count);

	int idx;
	for(idx=argument_count-1; idx>=0; idx--) {
		JagDecConsumer *argument_consumer = jag_dec_consumer_new(self);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_invoke, (JagDecIConsumer *) argument_consumer);
		cat_unref_ptr(argument_consumer);
	}

	JagDecConsumer *instance_consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_invoke, (JagDecIConsumer *) instance_consumer);
	cat_unref_ptr(instance_consumer);

	if (jag_byt_method_header_is_constructor(priv->method_header)) {
		JagDecConsumer *dummy_consumer = jag_dec_consumer_new(self);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_invoke, (JagDecIConsumer *) dummy_consumer);
		cat_unref_ptr(dummy_consumer);
	}

	JagDecDefaultValueProducer *valueProducer = NULL;
	if (jag_byt_method_header_is_constructor(priv->method_header)) {
		valueProducer = jag_dec_default_value_producer_new(self);
		JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) valueProducer);
		JagDecValueTypeDefinition *definition = jag_dec_value_type_definition_new(priv->instance_declaration_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_NEW);
		jag_dec_value_type_definition_list_add_definition(type_set, definition);
		cat_unref(definition);
	} else {
		gboolean isVoid = FALSE;
		JagAstDeclarationType *returnType = jag_byt_method_header_get_return_type(priv->method_header);
		if (returnType!=NULL) {
			isVoid = jag_ast_declaration_type_is_void(returnType);
		}
		if (!isVoid) {
			valueProducer = jag_dec_default_value_producer_new(self);
			JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) valueProducer);
			JagDecValueTypeDefinition *definition = jag_dec_value_type_definition_new(returnType, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_RETURN);
			jag_dec_value_type_definition_list_add_definition(type_set, definition);
			cat_unref(definition);
		}
	}


	if (valueProducer!=NULL) {
		jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_invoke, (JagDecIValueProducer *) valueProducer);
		cat_unref_ptr(valueProducer)
	}

	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_invoke);
//
//
//
//	// TODO: try to understand what the code below does
////		IValueProducer peekProducer = modelRuntime.peekProducer(0);
//	/* the line below checks whether there is a consumer for the */
//	JagDecIValueProducer *outputValueProducer = jag_dec_sub_code_invoke_special_get_output_value_producer(sub_code_invoke);
////		if (peekProducer!=null && peekProducer==sub_code_invoke.instanceConsumer.getProducer()) {
////			modelRuntime.popProducer();
////			modelRuntime.pushValueProvider(outputValueProducer);
//		// TODO
////			modelRuntime.addCrossTypeMatcher("sub_code_invoke", sub_code_invoke.instanceConsumer.getTypeSet(), sub_code_invoke.getTypeSet());
////		}
////
////
//	if (outputValueProducer!=NULL) {
//		jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, outputValueProducer);
//	}
//
	cat_unref_ptr(sub_code_invoke);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateInvokeSpecialPrivate *priv = JAG_DEC_INTERMEDIATE_INVOKE_SPECIAL_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_INVOKE_SPECIAL(other_node)) {
		JagDecIntermediateInvokeSpecialPrivate *o_priv = JAG_DEC_INTERMEDIATE_INVOKE_SPECIAL_GET_PRIVATE(other_node);
		return jag_ast_declaration_type_equal(priv->instance_declaration_type, o_priv->instance_declaration_type) &&
				jag_byt_method_header_equal(priv->method_header, o_priv->method_header);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateInvokeSpecialPrivate *priv = JAG_DEC_INTERMEDIATE_INVOKE_SPECIAL_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_invoke_special_new(mnemonic_index, priv->instance_declaration_type, priv->method_header);
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
