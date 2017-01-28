/*
   File:    jagdecintermediatefieldput.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 19, 2012
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

#include "jagdecintermediatefieldput.h"
#include "../../subcode/impl/jagdecsubcodefieldput.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateFieldPut"
#include <logging/catlog.h>

struct _JagDecIntermediateFieldPutPrivate {
	JagAstDeclarationType *field_source;
	JagAstDeclarationType *field_decl_type;
	JagAstIdentifier *field_name;
	gboolean is_non_static;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateFieldPut, jag_dec_intermediate_field_put, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_field_put_class_init(JagDecIntermediateFieldPutClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateFieldPutPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_field_put_init(JagDecIntermediateFieldPut *instance) {
	JagDecIntermediateFieldPutPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT, JagDecIntermediateFieldPutPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateFieldPut *instance = JAG_DEC_INTERMEDIATE_FIELD_PUT(object);
	JagDecIntermediateFieldPutPrivate *priv = instance->priv;
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

JagDecIntermediateFieldPut *jag_dec_intermediate_field_put_new(int mnemonic_index, JagAstDeclarationType *field_decl_type, JagAstDeclarationType *field_source, JagAstIdentifier *field_name, gboolean is_non_static) {
	JagDecIntermediateFieldPut *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateFieldPutPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->field_decl_type = cat_ref_ptr(field_decl_type);
	priv->field_source = cat_ref_ptr(field_source);
	priv->field_name = cat_ref_ptr(field_name);
	priv->is_non_static = is_non_static;
	return result;
}









/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateFieldPutPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_PUT_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);


	JagDecSubCodeFieldPut *sub_code = jag_dec_sub_code_field_put_new(mnemonic_index, priv->field_decl_type, priv->field_source, priv->field_name, priv->is_non_static);
	JagDecConsumer *field_consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code, (JagDecIConsumer *) field_consumer);
	JagDecConsumer *source_consumer = NULL;
	if (priv->is_non_static) {
		source_consumer = jag_dec_consumer_new(self);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code, (JagDecIConsumer *) source_consumer);
	}
	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code);

	if (priv->is_non_static) {
		JagDecValueTypeDefinitionList *leftHandSide = jag_dec_consumer_get_type_set(field_consumer);
		if (leftHandSide) {
			jag_dec_intermediate_run_context_add_value_type_definition_list(group_connect_runtime, leftHandSide);
		} else {
			cat_log_warn("no left hand side");
		}
		JagDecValueTypeDefinitionList *rightHandSide = jag_dec_consumer_get_type_set(source_consumer);
		if (rightHandSide) {
			jag_dec_intermediate_run_context_add_value_type_definition_list(group_connect_runtime, rightHandSide);
		} else {
			cat_log_warn("no right hand side");
		}
	//	JagDecCrossRuleLeftValue *crossRule = jag_dec_cross_rule_left_value(leftHandSide, rightHandSide);	// TODO implement
	//	jag_dec_value_type_definition_list_add_cross_rule(leftHandSide, (JagDecICrossRule *) crossRule);
	//	cat_unref_ptr(crossRule);
	}
	cat_unref_ptr(sub_code);
	cat_unref_ptr(field_consumer);
	cat_unref_ptr(source_consumer);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node){
	JagDecIntermediateFieldPutPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_PUT_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_FIELD_PUT(other_node)) {
		JagDecIntermediateFieldPutPrivate *o_priv = JAG_DEC_INTERMEDIATE_FIELD_PUT_GET_PRIVATE(other_node);
		return jag_ast_declaration_type_equal(o_priv->field_decl_type, priv->field_decl_type) &&
				jag_ast_declaration_type_equal(o_priv->field_source, priv->field_source) &&
				jag_ast_iexpression_equal((JagAstIExpression *) priv->field_name, (JagAstIExpression *) o_priv->field_name) &&
				priv->is_non_static==o_priv->is_non_static;
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateFieldPutPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_PUT_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_field_put_new(mnemonic_index, priv->field_decl_type, priv->field_source, priv->field_name, priv->is_non_static);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateFieldPutPrivate *priv = JAG_DEC_INTERMEDIATE_FIELD_PUT_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[field-name=");
	cat_string_wo_append(e_dump_buffer, jag_ast_identifier_get_text(priv->field_name));
	cat_string_wo_append_chars(e_dump_buffer, ", field-type=");
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
