/*
   File:    jagdecintermediatecondition.c
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

#include "jagdecintermediatecondition.h"
#include "../../../type/jagdeccrossrulematchforcondition.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"
#include "../../subcode/impl/jagdecsubcodecondition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateCondition"
#include <logging/catlog.h>

struct _JagDecIntermediateConditionPrivate {
	JagBytConditionType condition_type;
	gboolean with_zero;
	gboolean primitive;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateCondition, jag_dec_intermediate_condition, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_condition_class_init(JagDecIntermediateConditionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateConditionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_condition_init(JagDecIntermediateCondition *instance) {
	JagDecIntermediateConditionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_CONDITION, JagDecIntermediateConditionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateCondition *instance = JAG_DEC_INTERMEDIATE_CONDITION(object);
//	JagDecIntermediateConditionPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateCondition *jag_dec_intermediate_condition_new(int mnemonic_index, JagBytConditionType condition_type, int jump_index, gboolean with_zero, gboolean primitive) {
	JagDecIntermediateCondition *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_CONDITION, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateConditionPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->condition_type = jag_byt_condition_type_opposite(condition_type);
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) result);
	jag_dec_intermediate_node_scope_set_jump_index(scope, jump_index);
	cat_unref_ptr(scope);
	priv->with_zero = with_zero;
	priv->primitive = primitive;
	return result;
}









/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateConditionPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeCondition *subCode = jag_dec_sub_code_condition_new(mnemonic_index, priv->with_zero, priv->condition_type);

	JagDecConsumer *right_consumer = jag_dec_consumer_new(self);
	JagDecConsumer *left_consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) subCode, (JagDecIConsumer *) right_consumer);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) subCode, (JagDecIConsumer *) left_consumer);

	JagDecDefaultValueProducer *value_producer = jag_dec_default_value_producer_new(self);
	JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
	jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_BOOLEAN, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONDITION);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) subCode, (JagDecIValueProducer*) value_producer);

	if (priv->with_zero) {
		JagDecDefaultValueProducer *nil_producer = jag_dec_default_value_producer_new(self);
		if (priv->primitive) {

			JagAstIExpression *nul_expr = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) cat_string_wo_new_with("0"));
			jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) nil_producer, nul_expr);
			cat_unref_ptr(nul_expr);
		} else {
			JagAstIExpression *null_expr = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) cat_string_wo_new_with("null"));
			jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) nil_producer, null_expr);
			cat_unref_ptr(null_expr);
		}
		jag_dec_intermediate_run_context_push_value_provider_at(group_connect_runtime, (JagDecIValueProducer *) nil_producer, 1);
		cat_unref_ptr(nil_producer);
	}

	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) subCode);

	if (!priv->with_zero) {

		JagDecValueTypeDefinitionList *leftHandSide = jag_dec_consumer_get_type_set(left_consumer);
		JagDecValueTypeDefinitionList *rightHandSide = jag_dec_consumer_get_type_set(right_consumer);	// TODO check all calls to jag_dec_consumer_get_type_set
		if (leftHandSide) {
			jag_dec_intermediate_run_context_add_value_type_definition_list(group_connect_runtime, leftHandSide);
		}
		if (rightHandSide) {
			jag_dec_intermediate_run_context_add_value_type_definition_list(group_connect_runtime, rightHandSide);
		}
		if (leftHandSide!=NULL && (rightHandSide!=NULL)) {
			JagDecCrossRuleMatchForCondition *crossRule = jag_dec_cross_rule_match_for_condition_new(leftHandSide, rightHandSide);
			jag_dec_value_type_definition_list_add_cross_rule(leftHandSide, (JagDecICrossRule *) crossRule);
			jag_dec_value_type_definition_list_add_cross_rule(rightHandSide,(JagDecICrossRule *) crossRule);
			cat_unref_ptr(crossRule);
		}
	}
	cat_unref_ptr(subCode);
	cat_unref_ptr(value_producer);
	cat_unref_ptr(left_consumer);
	cat_unref_ptr(right_consumer);

}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateConditionPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_CONDITION(other_node)) {
		JagDecIntermediateConditionPrivate *o_priv = JAG_DEC_INTERMEDIATE_CONDITION_GET_PRIVATE(other_node);
		return (priv->condition_type == o_priv->condition_type) &&
				(priv->primitive == o_priv->primitive) &&
				(priv->with_zero == o_priv->with_zero);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateConditionPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) self);
	int jump_index = jag_dec_intermediate_node_scope_get_jump_index(scope);
	cat_unref_ptr(scope);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_condition_new(mnemonic_index, jag_byt_condition_type_opposite(priv->condition_type), jump_index, priv->with_zero, priv->primitive);
}

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *buf);

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	cat_string_wo_append(e_dump_buffer, prefix);
	l_stringable_print((CatIStringable *) self, e_dump_buffer);
	cat_string_wo_append_chars(e_dump_buffer, "\n");
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

/********************* start CatIStringableInterface implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *buf) {
	JagDecIntermediateConditionPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_GET_PRIVATE(self);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(buf, name+6);
	cat_string_wo_append_chars(buf, "[type='");
	cat_string_wo_append_chars(buf, jag_byt_condition_type_text(priv->condition_type));
	cat_string_wo_append_chars(buf, "', primitive=");
	cat_string_wo_append_chars(buf, priv->primitive ? "true" : "false");
	cat_string_wo_append_chars(buf, ", with-zero=");
	cat_string_wo_append_chars(buf, priv->with_zero ? "true" : "false");
	cat_string_wo_append_chars(buf, "]");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringableInterface implementation *********************/
