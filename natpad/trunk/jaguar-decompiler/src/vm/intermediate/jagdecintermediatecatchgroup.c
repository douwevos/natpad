/*
   File:    jagdecintermediatecatchgroup.c
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

#include "jagdecintermediatecatchgroup.h"
#include "jagdeciintermediatenode.h"
#include "jagdecintermediateruncontext.h"
#include "../subcode/jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateCatchGroup"
#include <logging/catlog.h>

struct _JagDecIntermediateCatchGroupPrivate {
	JagAstDeclarationType *exception_type;
	JagDecSubCodeCatch *sub_code_catch;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateCatchGroup, jag_dec_intermediate_catch_group, JAG_DEC_TYPE_INTERMEDIATE_GROUP, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_catch_group_class_init(JagDecIntermediateCatchGroupClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateCatchGroupPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_catch_group_init(JagDecIntermediateCatchGroup *instance) {
	JagDecIntermediateCatchGroupPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP, JagDecIntermediateCatchGroupPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateCatchGroup *instance = JAG_DEC_INTERMEDIATE_CATCH_GROUP(object);
	JagDecIntermediateCatchGroupPrivate *priv = instance->priv;
	cat_unref_ptr(priv->exception_type);
	cat_unref_ptr(priv->sub_code_catch);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateCatchGroup *jag_dec_intermediate_catch_group_new(JagAstDeclarationType *exception_type) {
	JagDecIntermediateCatchGroup *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateCatchGroupPrivate *priv = result->priv;
	jag_dec_intermediate_group_construct((JagDecIntermediateGroup *) result);
	priv->exception_type = cat_ref_ptr(exception_type);
	priv->sub_code_catch = NULL;
	return result;
}



JagAstDeclarationType *jag_dec_intermediate_catch_group_get_exception_type(JagDecIntermediateCatchGroup *catch_group) {
	JagDecIntermediateCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_CATCH_GROUP_GET_PRIVATE(catch_group);
	return priv->exception_type;
}

JagDecSubCodeCatch *jag_dec_intermediate_catch_group_get_sub_code_catch(JagDecIntermediateCatchGroup *catch_group) {
	JagDecIntermediateCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_CATCH_GROUP_GET_PRIVATE(catch_group);
	return priv->sub_code_catch;
}



/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *model_runtime) {
	JagDecIntermediateCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_CATCH_GROUP_GET_PRIVATE(self);
	cat_unref_ptr(priv->sub_code_catch);

	JagDecIntermediateRunContext *catchcontext = jag_dec_intermediate_run_context_create_branch(model_runtime);

	priv->sub_code_catch = jag_dec_sub_code_catch_new(0, priv->exception_type, jag_dec_intermediate_run_context_get_sub_code_block(catchcontext));

	JagDecDefaultValueProducer *value_producer = jag_dec_default_value_producer_new(self);
	JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
	JagDecValueTypeDefinition *definition = jag_dec_value_type_definition_new(priv->exception_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CATCHED_EXCEPTION);
	jag_dec_value_type_definition_list_add_definition(type_set, definition);
	cat_unref_ptr(definition);
	JagAstIdentifier *excp_id = jag_ast_identifier_new(cat_string_wo_new_with("exception"));
	jag_dec_ivalue_producer_set_expression((JagDecIValueProducer *) value_producer, (JagAstIExpression *) excp_id);
	cat_unref_ptr(excp_id);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) priv->sub_code_catch, (JagDecIValueProducer *) value_producer);
	jag_dec_intermediate_run_context_push_value_provider(catchcontext, (JagDecIValueProducer *) value_producer);
	cat_unref(value_producer);


	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self));
	p_iface->connectStageOne(self, catchcontext);
	cat_unref_ptr(catchcontext);
}

struct _JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_CATCH_GROUP_GET_PRIVATE(self);
	JagDecIntermediateCatchGroup *result = jag_dec_intermediate_catch_group_new(priv->exception_type);
	CatArrayWo *e_children = jag_dec_intermediate_group_get_all((JagDecIntermediateGroup *) self);
	CatIIterator *iter = cat_array_wo_iterator(e_children);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *child = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		if (deep) {
			JagDecIIntermediateNode *copy_of = jag_dec_iintermediate_node_make_copy(child, TRUE);
			jag_dec_intermediate_group_add((JagDecIntermediateGroup *) result, copy_of);
			cat_unref_ptr(copy_of);
		} else {
			jag_dec_intermediate_group_add((JagDecIntermediateGroup *) result, child);
		}
	}
	cat_unref_ptr(iter);

	return (struct _JagDecIIntermediateNode *) result;
}


static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->contains = p_iface->contains;
	iface->getScopeRef = p_iface->getScopeRef;
	iface->isSame = p_iface->isSame;
	iface->resolveBreaks = p_iface->resolveBreaks;
	iface->tryPrepend = p_iface->tryPrepend;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
