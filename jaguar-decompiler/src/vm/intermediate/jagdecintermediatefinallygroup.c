/*
   File:    jagdecintermediatefinallygroup.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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

#include "jagdecintermediatefinallygroup.h"
#include "jagdeciintermediatenode.h"
#include "jagdecintermediateruncontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateFinallyGroup"
#include <logging/catlog.h>

struct _JagDecIntermediateFinallyGroupPrivate {
	JagDecSubCodeFinally *sub_code_finally;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateFinallyGroup, jag_dec_intermediate_finally_group, JAG_DEC_TYPE_INTERMEDIATE_GROUP, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_finally_group_class_init(JagDecIntermediateFinallyGroupClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateFinallyGroupPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_finally_group_init(JagDecIntermediateFinallyGroup *instance) {
	JagDecIntermediateFinallyGroupPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_FINALLY_GROUP, JagDecIntermediateFinallyGroupPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateFinallyGroup *instance = JAG_DEC_INTERMEDIATE_FINALLY_GROUP(object);
	JagDecIntermediateFinallyGroupPrivate *priv = instance->priv;
	cat_unref_ptr(priv->sub_code_finally);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateFinallyGroup *jag_dec_intermediate_finally_group_new() {
	JagDecIntermediateFinallyGroup *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_FINALLY_GROUP, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateFinallyGroupPrivate *priv = result->priv;
	jag_dec_intermediate_group_construct((JagDecIntermediateGroup *) result);
	priv->sub_code_finally = NULL;
	return result;
}



JagDecSubCodeFinally *jag_dec_intermediate_finally_group_get_sub_code_finally(JagDecIntermediateFinallyGroup *finally_group) {
	JagDecIntermediateFinallyGroupPrivate *priv = JAG_DEC_INTERMEDIATE_FINALLY_GROUP_GET_PRIVATE(finally_group);
	return priv->sub_code_finally;
}





/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *model_runtime) {
	JagDecIntermediateFinallyGroupPrivate *priv = JAG_DEC_INTERMEDIATE_FINALLY_GROUP_GET_PRIVATE(self);
	cat_unref_ptr(priv->sub_code_finally);

	JagDecIntermediateRunContext *finallycontext = jag_dec_intermediate_run_context_create_branch(model_runtime);
	priv->sub_code_finally = jag_dec_sub_code_finally_new(0, (JagDecISubCode *) jag_dec_intermediate_run_context_get_sub_code_block(finallycontext));

	JagDecValueTypeDefinitionList *hidden_exception_type_set = jag_dec_value_type_definition_list_new_hidden_exception();
	JagDecExpressionHolder *expression_holder = jag_dec_expression_holder_new();
	JagDecDefaultValueProducer *value_producer = jag_dec_default_value_producer_new_with(self, hidden_exception_type_set, expression_holder);
	JagAstIdentifier *ident = jag_ast_identifier_new(cat_string_wo_new_with("/* throw-expression */"));
	jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) value_producer, (JagAstIExpression *) ident);
	cat_unref_ptr(ident);

	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) priv->sub_code_finally, (JagDecIValueProducer *) value_producer);
	jag_dec_intermediate_run_context_push_value_provider(finallycontext, (JagDecIValueProducer *) value_producer);
	cat_unref_ptr(value_producer);
	cat_unref_ptr(hidden_exception_type_set);
	cat_unref_ptr(expression_holder);


	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self));
	p_iface->connectStageOne(self, finallycontext);
	cat_unref_ptr(finallycontext);
}

static struct _JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateFinallyGroup *result = jag_dec_intermediate_finally_group_new();
	JagDecIntermediateGroup *dst_group = JAG_DEC_INTERMEDIATE_GROUP(result);
	JagDecIntermediateGroup *src_group = JAG_DEC_INTERMEDIATE_GROUP(self);
	int count = jag_dec_intermediate_group_count(src_group);
	int idx;
	for(idx=0; idx<count; idx++) {
		JagDecIIntermediateNode *sub = jag_dec_intermediate_group_get(src_group, idx);
		if (deep) {
			JagDecIIntermediateNode *copy_of_sub = jag_dec_iintermediate_node_make_copy(sub, TRUE);
			jag_dec_intermediate_group_add(dst_group, copy_of_sub);
			cat_unref_ptr(copy_of_sub);
		} else {
			jag_dec_intermediate_group_add(dst_group, sub);
		}
	}
	return (struct _JagDecIIntermediateNode *) result;
}


static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->contains = p_iface->contains;
	iface->getScopeRef = p_iface->getScopeRef;
	iface->isSame = p_iface->isSame;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->resolveBreaks = p_iface->resolveBreaks;
	iface->tryPrepend = p_iface->tryPrepend;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
