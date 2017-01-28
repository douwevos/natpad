/*
   File:    jagdecintermediatemultinewarray.c
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

#include "jagdecintermediatemultinewarray.h"
#include "../../subcode/impl/jagdecsubcodemultinewarray.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateMultiNewArray"
#include <logging/catlog.h>

struct _JagDecIntermediateMultiNewArrayPrivate {
	int dimensions;
	JagAstDeclarationType *array_declaration_type;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateMultiNewArray, jag_dec_intermediate_multi_new_array, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_multi_new_array_class_init(JagDecIntermediateMultiNewArrayClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateMultiNewArrayPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_multi_new_array_init(JagDecIntermediateMultiNewArray *instance) {
	JagDecIntermediateMultiNewArrayPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_MULTI_NEW_ARRAY, JagDecIntermediateMultiNewArrayPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateMultiNewArray *instance = JAG_DEC_INTERMEDIATE_MULTI_NEW_ARRAY(object);
	JagDecIntermediateMultiNewArrayPrivate *priv = instance->priv;
	cat_unref_ptr(priv->array_declaration_type);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateMultiNewArray *jag_dec_intermediate_multi_new_array_new(int mnemonic_index, int dimensions, JagAstDeclarationType *array_declaration_type) {
	JagDecIntermediateMultiNewArray *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_MULTI_NEW_ARRAY, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateMultiNewArrayPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->dimensions = dimensions;
	priv->array_declaration_type = jag_ast_declaration_type_set_dim_count(array_declaration_type, dimensions);
	return result;
}







/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateMultiNewArrayPrivate *priv = JAG_DEC_INTERMEDIATE_MULTI_NEW_ARRAY_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);

	JagDecSubCodeMultiNewArray *sub_code_multi_array = jag_dec_sub_code_multi_new_array_new(mnemonic_index, priv->dimensions, priv->array_declaration_type);
	int idx;
	for(idx=0; idx<priv->dimensions; idx++) {
		JagDecConsumer *consumer = jag_dec_consumer_new(self);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_multi_array, (JagDecIConsumer *) consumer);
		jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) consumer);
		cat_unref_ptr(consumer);
	}
	JagDecIValueProducer *value_producer = (JagDecIValueProducer *) jag_dec_default_value_producer_new(self);
	JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set(value_producer);
	JagDecValueTypeDefinition *definition = jag_dec_value_type_definition_new(priv->array_declaration_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_NEW);
	jag_dec_value_type_definition_list_add_definition(type_set, definition);
	cat_unref_ptr(definition);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_multi_array, value_producer);
	jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, value_producer);
	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_multi_array);
	cat_unref_ptr(value_producer);
	cat_unref(sub_code_multi_array);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateMultiNewArrayPrivate *priv = JAG_DEC_INTERMEDIATE_MULTI_NEW_ARRAY_GET_PRIVATE(self);

	if (JAG_DEC_IS_INTERMEDIATE_MULTI_NEW_ARRAY(other_node)) {
		JagDecIntermediateMultiNewArrayPrivate *o_priv = JAG_DEC_INTERMEDIATE_MULTI_NEW_ARRAY_GET_PRIVATE(other_node);
		return (priv->dimensions==o_priv->dimensions) &&
				(jag_ast_declaration_type_equal(priv->array_declaration_type, o_priv->array_declaration_type));
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateMultiNewArrayPrivate *priv = JAG_DEC_INTERMEDIATE_MULTI_NEW_ARRAY_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_multi_new_array_new(mnemonic_index, priv->dimensions, priv->array_declaration_type);
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateMultiNewArrayPrivate *priv = JAG_DEC_INTERMEDIATE_MULTI_NEW_ARRAY_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[dim-count=");
	cat_string_wo_append_decimal(e_dump_buffer, priv->dimensions);
	cat_string_wo_append_chars(e_dump_buffer, ", array-type=");
	jag_ast_declaration_type_as_text(priv->array_declaration_type, e_dump_buffer);
	cat_string_wo_append_chars(e_dump_buffer, "]\n");
}

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->isSame = l_intermediate_node_is_same;
	iface->dump = l_intermediate_node_dump;

	iface->contains = p_iface->contains;
	iface->getScopeRef = p_iface->getScopeRef;
	iface->resolveBreaks = p_iface->resolveBreaks;
	iface->tryPrepend = p_iface->tryPrepend;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/

