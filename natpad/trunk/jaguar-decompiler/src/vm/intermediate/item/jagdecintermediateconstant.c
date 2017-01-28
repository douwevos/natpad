/*
   File:    jagdecintermediateconstant.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 9, 2012
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

#include "jagdecintermediateconstant.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateConstant"
#include <logging/catlog.h>

struct _JagDecIntermediateConstantPrivate {
	JagDecValueTypeDefinitionList *type_set;
	JagAstIExpression *constant_expression;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateConstant, jag_dec_intermediate_constant, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_constant_class_init(JagDecIntermediateConstantClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateConstantPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_constant_init(JagDecIntermediateConstant *instance) {
	JagDecIntermediateConstantPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_CONSTANT, JagDecIntermediateConstantPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateConstant *instance = JAG_DEC_INTERMEDIATE_CONSTANT(object);
	JagDecIntermediateConstantPrivate *priv = instance->priv;
	cat_unref_ptr(priv->constant_expression);
	cat_unref_ptr(priv->type_set);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateConstant *jag_dec_intermediate_constant_new(int mnemonic_index, JagDecValueTypeDefinitionList *type_set, JagAstIExpression *constant_expression) {
	JagDecIntermediateConstant *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_CONSTANT, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateConstantPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->type_set = cat_ref_ptr(type_set);
	priv->constant_expression = cat_ref_ptr(constant_expression);
	return result;
}


JagAstIExpression *jag_dec_intermediate_constant_get_value_expression(JagDecIntermediateConstant *im_constant) {
	return JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(im_constant)->constant_expression;
}


/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateConstantPrivate *priv = JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeConstantLiteral *sub_code = jag_dec_sub_code_constant_literal_new(mnemonic_index, priv->constant_expression);
	JagDecDefaultValueProducer *value_producer = jag_dec_default_value_producer_new(self);
	jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) value_producer, priv->constant_expression);
	JagDecValueTypeDefinitionList *dest_type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
	jag_dec_value_type_definition_list_copy_to(priv->type_set, dest_type_set);

	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code, (JagDecIValueProducer *) value_producer);
	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode*) sub_code);

	cat_unref_ptr(value_producer);
	cat_unref_ptr(sub_code);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateConstantPrivate *priv = JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_CONSTANT(other_node)) {
		JagDecIntermediateConstantPrivate *o_priv = JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(other_node);
		if (jag_ast_iexpression_equal(priv->constant_expression, o_priv->constant_expression)) {
			return TRUE;
		}
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateConstantPrivate *priv = JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_constant_new(mnemonic_index, priv->type_set, priv->constant_expression);
}

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *buf);

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	cat_string_wo_append(e_dump_buffer, prefix);
	l_stringable_print((CatIStringable *) self, e_dump_buffer);
	cat_string_wo_append_char(e_dump_buffer, '\n');
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

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_buf) {
	JagDecIntermediateConstantPrivate *priv = JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(self);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_buf, "%s[expr=%o, typed-set=%o]", name+6, priv->constant_expression, priv->type_set);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringableInterface implementation *********************/
