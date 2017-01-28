/*
   File:    jagdecintermediateifthenelse.c
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

#include "jagdecintermediateifthenelse.h"
#include "jagdecconditionvalueproducer.h"
#include "../jagdeciintermediatenode.h"
#include "../jagdecintermediateruncontext.h"
#include "../jagdecintermediatenodescope.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/jagdeciconsumer.h"
#include "../../subcode/jagdecisubcode.h"
#include "../../subcode/impl/jagdecsubcodeifthenelse.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateIfThenElse"
#include <logging/catlog.h>

struct _JagDecIntermediateIfThenElsePrivate {
	JagDecIIntermediateNode *condition;
	JagDecIIntermediateNode *then_node;
	JagDecIIntermediateNode *else_node;
	JagDecIntermediateNodeScope *scope;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateIfThenElse, jag_dec_intermediate_if_then_else, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_if_then_else_class_init(JagDecIntermediateIfThenElseClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateIfThenElsePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_if_then_else_init(JagDecIntermediateIfThenElse *instance) {
	JagDecIntermediateIfThenElsePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_IF_THEN_ELSE, JagDecIntermediateIfThenElsePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateIfThenElse *instance = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE(object);
	JagDecIntermediateIfThenElsePrivate *priv = instance->priv;
	cat_unref_ptr(priv->condition);
	cat_unref_ptr(priv->then_node);
	cat_unref_ptr(priv->else_node);
	cat_unref_ptr(priv->scope);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateIfThenElse *jag_dec_intermediate_if_then_else_new(JagDecIIntermediateNode *condition, JagDecIIntermediateNode *then_node, JagDecIIntermediateNode *else_node) {
	JagDecIntermediateIfThenElse *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_IF_THEN_ELSE, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateIfThenElsePrivate *priv = result->priv;
	priv->condition = cat_ref_ptr(condition);
	priv->then_node = cat_ref_ptr(then_node);
	priv->else_node = cat_ref_ptr(else_node);
	priv->scope = jag_dec_intermediate_node_scope_new(-1, -1);
	return result;
}












/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static JagDecIntermediateNodeScope *l_intermediate_node_get_scope_ref(JagDecIIntermediateNode *self) {
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	if (priv->else_node==NULL) {
		JagDecIntermediateNodeScope *then_scope = jag_dec_iintermediate_node_get_scope_ref(priv->then_node);
		jag_dec_intermediate_node_scope_set_exit_index(priv->scope, jag_dec_intermediate_node_scope_get_exit_index(then_scope));
		cat_unref_ptr(then_scope);
	} else {
		JagDecIntermediateNodeScope *else_scope = jag_dec_iintermediate_node_get_scope_ref(priv->else_node);
		jag_dec_intermediate_node_scope_set_exit_index(priv->scope, jag_dec_intermediate_node_scope_get_exit_index(else_scope));
		cat_unref_ptr(else_scope);
	}
	return cat_ref_ptr(priv->scope);
}

static void l_intermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jump_index) {
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	jag_dec_iintermediate_node_resolve_breaks(priv->condition, jump_index);
	jag_dec_iintermediate_node_resolve_breaks(priv->then_node, jump_index);
	if (priv->else_node!=NULL) {
		jag_dec_iintermediate_node_resolve_breaks(priv->else_node, jump_index);
	}
}

static gboolean l_intermediate_node_contains(JagDecIIntermediateNode *self, int mnemonic_index) {
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	return jag_dec_iintermediate_node_contains(priv->condition, mnemonic_index)
		|| jag_dec_iintermediate_node_contains(priv->then_node, mnemonic_index)
		|| (priv->else_node!=NULL && jag_dec_iintermediate_node_contains(priv->else_node, mnemonic_index));
}

static gboolean l_intermediate_node_try_prepend(JagDecIIntermediateNode *self, JagDecIIntermediateNode *node) {
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	return jag_dec_iintermediate_node_try_prepend(priv->condition, node);
}


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	jag_dec_iintermediate_node_connect_stage_one(priv->condition, group_connect_runtime);

	JagDecIntermediateRunContext *branchRuntime = jag_dec_intermediate_run_context_create_branch(group_connect_runtime);
	jag_dec_iintermediate_node_connect_stage_one(priv->then_node, branchRuntime);
	JagDecSubCodeBlock *thenSubCodes = jag_dec_intermediate_run_context_get_sub_code_block(branchRuntime);

	JagDecIntermediateRunContext *else_branch_runtime = NULL;
	JagDecSubCodeBlock *elseSubCodes = NULL;

	JagDecSubCodeIfThenElse *sub_code_if_then_else = NULL;
	if (priv->else_node!=NULL) {
		else_branch_runtime = jag_dec_intermediate_run_context_create_branch(group_connect_runtime);
		jag_dec_iintermediate_node_connect_stage_one(priv->else_node, else_branch_runtime);
		elseSubCodes = jag_dec_intermediate_run_context_get_sub_code_block(else_branch_runtime);
	}
	sub_code_if_then_else = jag_dec_sub_code_if_then_else_new((JagDecISubCode *) thenSubCodes, (JagDecISubCode *) elseSubCodes);


	JagDecConsumer *consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_if_then_else, (JagDecIConsumer *) consumer);
	jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) consumer);

	JagDecIValueProducer *value_producer = (JagDecIValueProducer *) jag_dec_condition_value_producer_new(self, consumer, FALSE);
	cat_unref_ptr(consumer);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_if_then_else, value_producer);
//	jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, value_producer);
	cat_unref_ptr(value_producer);


	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_if_then_else);

	cat_unref_ptr(sub_code_if_then_else);
	cat_unref_ptr(branchRuntime);
	cat_unref_ptr(else_branch_runtime);
}




static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	if (self==other_node) {
		return TRUE;
	}
	if (self==NULL || other_node==NULL) {
		return FALSE;
	}
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_IF_THEN_ELSE(other_node)) {
		JagDecIntermediateIfThenElsePrivate *o_priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(other_node);
		return jag_dec_iintermediate_node_is_same(o_priv->condition, priv->condition) &&
				jag_dec_iintermediate_node_is_same(o_priv->then_node, priv->then_node) &&
				jag_dec_iintermediate_node_is_same(o_priv->else_node, priv->else_node);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	if (deep) {
		JagDecIIntermediateNode *condition_node_copy = jag_dec_iintermediate_node_make_copy(priv->condition, TRUE);
		JagDecIIntermediateNode *then_node_copy = jag_dec_iintermediate_node_make_copy(priv->then_node, TRUE);
		JagDecIIntermediateNode *else_node_copy = jag_dec_iintermediate_node_make_copy(priv->else_node, TRUE);
		JagDecIIntermediateNode *result = (JagDecIIntermediateNode *) jag_dec_intermediate_if_then_else_new(condition_node_copy, then_node_copy, else_node_copy);
		cat_unref_ptr(condition_node_copy);
		cat_unref_ptr(then_node_copy);
		cat_unref_ptr(else_node_copy);
		return result;
	}
	return (JagDecIIntermediateNode *) jag_dec_intermediate_if_then_else_new(priv->condition, priv->then_node, priv->else_node);
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateIfThenElsePrivate *priv = JAG_DEC_INTERMEDIATE_IF_THEN_ELSE_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[");
	jag_dec_intermediate_node_scope_as_text(priv->scope, e_dump_buffer);
	cat_string_wo_append_chars(e_dump_buffer, "]\n");
	CatStringWo *e_prefix_head = cat_string_wo_clone(prefix, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix_head, "  ");
	CatStringWo *e_prefix_sub = cat_string_wo_clone(e_prefix_head, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix_sub, "  ");

	cat_string_wo_append(e_dump_buffer, e_prefix_head);
	cat_string_wo_append_chars(e_dump_buffer, "condition\n");
	jag_dec_iintermediate_node_dump(priv->condition, e_dump_buffer, e_prefix_sub);

	cat_string_wo_append(e_dump_buffer, e_prefix_head);
	cat_string_wo_append_chars(e_dump_buffer, "then\n");
	jag_dec_iintermediate_node_dump(priv->then_node, e_dump_buffer, e_prefix_sub);


	if (priv->else_node) {
		cat_string_wo_append(e_dump_buffer, e_prefix_head);
		cat_string_wo_append_chars(e_dump_buffer, "else\n");
		jag_dec_iintermediate_node_dump(priv->else_node, e_dump_buffer, e_prefix_sub);
	}

	cat_unref_ptr(e_prefix_sub);
	cat_unref_ptr(e_prefix_head);
}

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->contains = l_intermediate_node_contains;
	iface->getScopeRef = l_intermediate_node_get_scope_ref;
	iface->isSame = l_intermediate_node_is_same;
	iface->dump = l_intermediate_node_dump;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->resolveBreaks = l_intermediate_node_resolve_breaks;
	iface->tryPrepend = l_intermediate_node_try_prepend;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
