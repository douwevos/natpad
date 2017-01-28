/*
   File:    jagdecintermediatefieldget.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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

#include "jagdecintermediatefieldget.h"
#include "../../subcode/impl/jagdecsubcodefieldget.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateFieldGet"
#include <logging/catlog.h>

struct _JagDecIntermediateFieldGetPrivate {
	JagAstDeclarationType *field_source;
	JagAstDeclarationType *field_decl_type;
	JagAstIdentifier *field_name;
	gboolean is_non_static;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateFieldGet, jag_dec_intermediate_field_get, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_field_get_class_init(JagDecIntermediateFieldGetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateFieldGetPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_field_get_init(JagDecIntermediateFieldGet *instance) {
	JagDecIntermediateFieldGetPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET, JagDecIntermediateFieldGetPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateFieldGet *instance = JAG_DEC_INTERMEDIATE_FIELD_GET(object);
	JagDecIntermediateFieldGetPrivate *priv = instance->priv;
	cat_unref_ptr(priv->field_source);
	cat_unref_ptr(priv->field_decl_type);
	cat_unref_ptr(priv->field_name);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateFieldGet *jag_dec_intermediate_field_get_new(int mnemonic_index, JagAstDeclarationType *field_type, JagAstDeclarationType *field_source, JagAstIdentifier *field_name, gboolean is_non_static) {
	JagDecIntermediateFieldGet *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateFieldGetPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->field_decl_type = cat_ref_ptr(field_type);
	priv->field_source = cat_ref_ptr(field_source);
	priv->field_name = cat_ref_ptr(field_name);
	priv->is_non_static = is_non_static;
	return result;
}










/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateFieldGetPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_GET_GET_PRIVATE(self);

	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeFieldGet *sub_code_field_get = jag_dec_sub_code_field_get_new(mnemonic_index, priv->field_source, priv->field_name, priv->is_non_static);
	if (priv->is_non_static) {

		JagDecConsumer *source_consumer = jag_dec_consumer_new(self);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_field_get, (JagDecIConsumer *) source_consumer);
		cat_unref_ptr(source_consumer);
	}

	JagDecDefaultValueProducer *value_producer = jag_dec_default_value_producer_new(self);
	JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
	JagDecValueTypeDefinition *type_definition = jag_dec_value_type_definition_new(priv->field_decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
	jag_dec_value_type_definition_list_add_definition(type_set, type_definition);
	cat_unref_ptr(type_definition);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_field_get, (JagDecIValueProducer *) value_producer);
	cat_unref_ptr(value_producer);

	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_field_get);
	cat_unref_ptr(sub_code_field_get);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateFieldGetPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_GET_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_FIELD_GET(other_node)) {
		JagDecIntermediateFieldGetPrivate *o_priv = JAG_DEC_INTERMEDIATE_FIELD_GET_GET_PRIVATE(other_node);
		return jag_ast_declaration_type_equal(o_priv->field_decl_type, priv->field_decl_type) &&
				jag_ast_declaration_type_equal(o_priv->field_source, priv->field_source) &&
				jag_ast_iexpression_equal((JagAstIExpression *) priv->field_name, (JagAstIExpression *) o_priv->field_name) &&
				(priv->is_non_static==o_priv->is_non_static);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateFieldGetPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_GET_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_field_get_new(mnemonic_index, priv->field_decl_type, priv->field_source, priv->field_name, priv->is_non_static);
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateFieldGetPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_GET_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[field-name=");
	cat_string_wo_append(e_dump_buffer, jag_ast_identifier_get_text(priv->field_name));
	cat_string_wo_append_chars(e_dump_buffer, ", src=");
	jag_ast_declaration_type_as_text(priv->field_source, e_dump_buffer);
	cat_string_wo_append_chars(e_dump_buffer, ", field-decl-type=");
	jag_ast_declaration_type_as_text(priv->field_decl_type, e_dump_buffer);
	if (priv->is_non_static) {
		cat_string_wo_append_chars(e_dump_buffer, ", is-non-static");
	} else {
		cat_string_wo_append_chars(e_dump_buffer, ", is-static");
	}
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


