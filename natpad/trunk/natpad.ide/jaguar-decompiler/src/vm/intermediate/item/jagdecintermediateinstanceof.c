/*
   File:    jagdecintermediateinstanceof.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 20, 2012
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

#include "jagdecintermediateinstanceof.h"
#include "../../subcode/impl/jagdecsubcodeinstanceof.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"
#include "../../subcode/jagdecconsumer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateInstanceof"
#include <logging/catlog.h>

struct _JagDecIntermediateInstanceofPrivate {
	JagAstDeclarationType *instanceof_decl_type;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateInstanceof, jag_dec_intermediate_instanceof, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_instanceof_class_init(JagDecIntermediateInstanceofClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateInstanceofPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_instanceof_init(JagDecIntermediateInstanceof *instance) {
	JagDecIntermediateInstanceofPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_INSTANCEOF, JagDecIntermediateInstanceofPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateInstanceof *instance = JAG_DEC_INTERMEDIATE_INSTANCEOF(object);
	JagDecIntermediateInstanceofPrivate *priv = instance->priv;
	cat_unref_ptr(priv->instanceof_decl_type);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagDecIntermediateInstanceof *jag_dec_intermediate_instanceof_new(int mnemonic_index, JagAstDeclarationType *instanceof_decl_type) {
	JagDecIntermediateInstanceof *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_INSTANCEOF, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateInstanceofPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->instanceof_decl_type = cat_ref_ptr(instanceof_decl_type);
	return result;
}









/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateInstanceofPrivate *priv = JAG_DEC_INTERMEDIATE_INSTANCEOF_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);

	JagDecSubCodeInstanceof *sub_code_instanceof = jag_dec_sub_code_instanceof_new(mnemonic_index, priv->instanceof_decl_type);
	JagDecConsumer *consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_instanceof, (JagDecIConsumer *) consumer);
	cat_unref_ptr(consumer);

	JagDecDefaultValueProducer *value_producer = jag_dec_default_value_producer_new(self);
	JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
	jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_BOOLEAN, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_INSTANCEOF);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_instanceof, (JagDecIValueProducer *) value_producer);
	cat_unref_ptr(value_producer);

	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_instanceof);
	cat_unref_ptr(sub_code_instanceof);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node){
	JagDecIntermediateInstanceofPrivate *priv = JAG_DEC_INTERMEDIATE_INSTANCEOF_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_INSTANCEOF(other_node)) {
		JagDecIntermediateInstanceofPrivate *o_priv = JAG_DEC_INTERMEDIATE_INSTANCEOF_GET_PRIVATE(other_node);
		return jag_ast_declaration_type_equal(priv->instanceof_decl_type, o_priv->instanceof_decl_type);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateInstanceofPrivate *priv = JAG_DEC_INTERMEDIATE_INSTANCEOF_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_instanceof_new(mnemonic_index, priv->instanceof_decl_type);
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateInstanceofPrivate *priv = JAG_DEC_INTERMEDIATE_INSTANCEOF_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[instance-of=");
	jag_ast_declaration_type_as_text(priv->instanceof_decl_type, e_dump_buffer);
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

