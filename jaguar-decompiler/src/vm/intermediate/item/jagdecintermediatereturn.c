/*
   File:    jagdecintermediatereturn.c
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

#include "jagdecintermediatereturn.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/impl/jagdecsubcodereturn.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateReturn"
#include <logging/catlog.h>

struct _JagDecIntermediateReturnPrivate {
	JagAstDeclarationType *return_type;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateReturn, jag_dec_intermediate_return, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_return_class_init(JagDecIntermediateReturnClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateReturnPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_return_init(JagDecIntermediateReturn *instance) {
	JagDecIntermediateReturnPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_RETURN, JagDecIntermediateReturnPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateReturn *instance = JAG_DEC_INTERMEDIATE_RETURN(object);
	JagDecIntermediateReturnPrivate *priv = instance->priv;
	cat_unref_ptr(priv->return_type);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateReturn *jag_dec_intermediate_return_new(int mnemonic_index, JagAstDeclarationType *return_type) {
	JagDecIntermediateReturn *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_RETURN, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateReturnPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) result);
	jag_dec_intermediate_node_scope_set_exit_index(scope, JAG_DEC_SCOPE_INDEX_EXIT);
	cat_unref_ptr(scope);
	priv->return_type = cat_ref_ptr(return_type);
	return result;
}









/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateReturnPrivate *priv = JAG_DEC_INTERMEDIATE_RETURN_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);

	JagDecSubCodeReturn *subCodeReturn = jag_dec_sub_code_return_new(mnemonic_index, priv->return_type);
	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) subCodeReturn);
	if (priv->return_type!=NULL) {
		JagDecConsumer *return_consumer = jag_dec_consumer_new(self);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) subCodeReturn, (JagDecIConsumer *) return_consumer);
		jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) return_consumer);
		JagDecValueTypeDefinitionList *type_set = jag_dec_consumer_get_type_set(return_consumer);
		if (type_set) {
			JagDecValueTypeDefinition *return_val_type_definition = jag_dec_value_type_definition_new(priv->return_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_RETURN);
			jag_dec_value_type_definition_list_add_definition(type_set, return_val_type_definition);
			cat_unref_ptr(return_val_type_definition);
		}
		cat_unref_ptr(return_consumer);
	}
	cat_unref_ptr(subCodeReturn);

}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateReturnPrivate *priv = JAG_DEC_INTERMEDIATE_RETURN_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_RETURN(other_node)) {
		JagDecIntermediateReturnPrivate *o_priv = JAG_DEC_INTERMEDIATE_RETURN_GET_PRIVATE(other_node);
		return jag_ast_declaration_type_equal(priv->return_type, o_priv->return_type);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateReturnPrivate *priv = JAG_DEC_INTERMEDIATE_RETURN_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_return_new(mnemonic_index, priv->return_type);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateReturnPrivate *priv = JAG_DEC_INTERMEDIATE_RETURN_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	if (priv->return_type) {
		cat_string_wo_append_chars(e_dump_buffer, "[type=");
		jag_ast_declaration_type_as_text(priv->return_type, e_dump_buffer);
		cat_string_wo_append_chars(e_dump_buffer, "]\n");
	} else {
		cat_string_wo_append_chars(e_dump_buffer, "\n");
	}
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

