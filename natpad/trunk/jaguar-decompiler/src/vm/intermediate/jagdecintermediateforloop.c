/*
   File:    jagdecintermediateforloop.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Dec 24, 2012
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

#include "jagdecintermediateforloop.h"
#include "jagdecintermediatenodescope.h"
#include "jagdecintermediategroup.h"
#include "jagdecintermediateruncontext.h"
#include "../subcode/block/jagdecsubcodeforloop.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateForLoop"
#include <logging/catlog.h>

struct _JagDecIntermediateForLoopPrivate {
	JagDecIIntermediateNode *initNode;
	JagDecIIntermediateNode *conditionNode;
	JagDecIIntermediateNode *incrementNode;
	JagDecIIntermediateNode *loopNode;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateForLoop, jag_dec_intermediate_for_loop, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_for_loop_class_init(JagDecIntermediateForLoopClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateForLoopPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_for_loop_init(JagDecIntermediateForLoop *instance) {
	JagDecIntermediateForLoopPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP, JagDecIntermediateForLoopPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateForLoop *instance = JAG_DEC_INTERMEDIATE_FOR_LOOP(object);
	JagDecIntermediateForLoopPrivate *priv = instance->priv;
	cat_unref_ptr(priv->initNode);
	cat_unref_ptr(priv->conditionNode);
	cat_unref_ptr(priv->incrementNode);
	cat_unref_ptr(priv->loopNode);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateForLoop *jag_dec_intermediate_for_loop_new(JagDecIIntermediateNode *initNode, JagDecIIntermediateNode *conditionNode, JagDecIIntermediateNode *incrementNode, JagDecIIntermediateNode *loopNode) {
	JagDecIntermediateForLoop *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateForLoopPrivate *priv = result->priv;
	priv->initNode = cat_ref_ptr(initNode);
	priv->conditionNode = cat_ref_ptr(conditionNode);
	priv->incrementNode = cat_ref_ptr(incrementNode);
	priv->loopNode = cat_ref_ptr(loopNode);
	return result;
}











/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);
	JagDecIntermediateRunContext *branch_runtime = jag_dec_intermediate_run_context_create_branch(group_connect_runtime);
	jag_dec_iintermediate_node_connect_stage_one(priv->initNode, branch_runtime);
	JagDecSubCodeBlock *initBlock = jag_dec_intermediate_run_context_get_sub_code_block(branch_runtime);
	cat_ref_ptr(initBlock);

	JagDecSubCodeBlock *bodyBlock = jag_dec_sub_code_block_new();
	jag_dec_intermediate_run_context_set_sub_code_block(branch_runtime, bodyBlock);
	jag_dec_intermediate_run_context_clear_pseudo_stack(branch_runtime);
	jag_dec_iintermediate_node_connect_stage_one(priv->loopNode, branch_runtime);

	JagDecSubCodeBlock *incrementBlock = jag_dec_sub_code_block_new();
	jag_dec_intermediate_run_context_set_sub_code_block(branch_runtime, incrementBlock);
	jag_dec_intermediate_run_context_clear_pseudo_stack(branch_runtime);
	jag_dec_iintermediate_node_connect_stage_one(priv->incrementNode, branch_runtime);

	JagDecSubCodeBlock *conditionBlock = jag_dec_sub_code_block_new();
	jag_dec_intermediate_run_context_set_sub_code_block(branch_runtime, conditionBlock);
	jag_dec_intermediate_run_context_clear_pseudo_stack(branch_runtime);
	jag_dec_iintermediate_node_connect_stage_one(priv->conditionNode, branch_runtime);

	JagDecConsumer *consumer = jag_dec_consumer_new(self);

	JagDecSubCodeForLoop *subCodeForLoop = jag_dec_sub_code_for_loop_new((JagDecISubCode *) initBlock, (JagDecISubCode *) conditionBlock, consumer, (JagDecISubCode *) incrementBlock, (JagDecISubCode *) bodyBlock);
	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) subCodeForLoop);
	jag_dec_intermediate_run_context_push_value_consumer(branch_runtime, (JagDecIConsumer *) consumer);

	cat_unref_ptr(branch_runtime);
	cat_unref_ptr(initBlock);
	cat_unref_ptr(conditionBlock);
	cat_unref_ptr(consumer);
	cat_unref_ptr(incrementBlock);
	cat_unref_ptr(bodyBlock);
	cat_unref_ptr(subCodeForLoop);
}

static void l_intermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jump_index) {
	JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);
	jag_dec_iintermediate_node_resolve_breaks(priv->initNode, jump_index);
	jag_dec_iintermediate_node_resolve_breaks(priv->conditionNode, jump_index);
	jag_dec_iintermediate_node_resolve_breaks(priv->incrementNode, jump_index);
	jag_dec_iintermediate_node_resolve_breaks(priv->loopNode, jump_index);
}

static JagDecIntermediateNodeScope *l_intermediate_node_get_scope_ref(JagDecIIntermediateNode *self) {
	JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);

	JagDecIntermediateNodeScope *lcScope = jag_dec_intermediate_node_scope_new(JAG_DEC_SCOPE_INDEX_NONE, JAG_DEC_SCOPE_INDEX_NONE);
	JagDecIntermediateNodeScope *cond_scope = jag_dec_iintermediate_node_get_scope_ref(priv->conditionNode);
	int cn_exit_index = jag_dec_intermediate_node_scope_get_exit_index(cond_scope);
	jag_dec_intermediate_node_scope_set_exit_index(lcScope, cn_exit_index);
	cat_unref_ptr(cond_scope);

	return lcScope;
}

static gboolean l_intermediate_node_contains(JagDecIIntermediateNode *self, int mnemonic_index) {
	JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);
	return jag_dec_iintermediate_node_contains(priv->initNode, mnemonic_index)
			|| jag_dec_iintermediate_node_contains(priv->conditionNode, mnemonic_index)
			|| jag_dec_iintermediate_node_contains(priv->incrementNode, mnemonic_index)
			|| jag_dec_iintermediate_node_contains(priv->loopNode, mnemonic_index);
}

static gboolean l_intermediate_node_try_prepend(JagDecIIntermediateNode *self, JagDecIIntermediateNode *node) {
	if (node==NULL) {
		cat_log_error("node is NULL");
		return FALSE;
	}
	JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);
	return jag_dec_iintermediate_node_try_prepend(priv->initNode, node);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	if (other_node == self) {
		return TRUE;
	}
	if (JAG_DEC_IS_INTERMEDIATE_FOR_LOOP(other_node)) {
		JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);
		JagDecIntermediateForLoopPrivate *that = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(other_node);
		return jag_dec_iintermediate_node_is_same(priv->initNode, that->initNode)
				&& jag_dec_iintermediate_node_is_same(priv->conditionNode, that->conditionNode)
				&& jag_dec_iintermediate_node_is_same(priv->incrementNode, that->incrementNode)
				&& jag_dec_iintermediate_node_is_same(priv->loopNode, that->loopNode);
	}
	return FALSE;

}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);
	if (deep) {
		// TODO ??
	}
	return (JagDecIIntermediateNode *) jag_dec_intermediate_for_loop_new(priv->initNode, priv->conditionNode, priv->incrementNode, priv->loopNode);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateForLoopPrivate *priv = JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[");
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref(self);
	jag_dec_intermediate_node_scope_as_text(scope, e_dump_buffer);
	cat_unref_ptr(scope);
	cat_string_wo_append_chars(e_dump_buffer, "]\n");
	CatStringWo *e_prefix_head = cat_string_wo_clone(prefix, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix_head, "  ");
	CatStringWo *e_prefix_sub = cat_string_wo_clone(e_prefix_head, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix_sub, "  ");

	cat_string_wo_append(e_dump_buffer, e_prefix_head);
	cat_string_wo_append_chars(e_dump_buffer, "for-init\n");
	if (priv->initNode) {
		jag_dec_iintermediate_node_dump(priv->initNode, e_dump_buffer, e_prefix_sub);
	} else {
		cat_string_wo_append(e_dump_buffer, e_prefix_sub);
		cat_string_wo_append_chars(e_dump_buffer, "null\n");
	}

	cat_string_wo_append(e_dump_buffer, e_prefix_head);
	cat_string_wo_append_chars(e_dump_buffer, "for-condition\n");
	if (priv->conditionNode) {
		jag_dec_iintermediate_node_dump(priv->conditionNode, e_dump_buffer, e_prefix_sub);
	} else {
		cat_string_wo_append(e_dump_buffer, e_prefix_sub);
		cat_string_wo_append_chars(e_dump_buffer, "null\n");
	}

	cat_string_wo_append(e_dump_buffer, e_prefix_head);
	cat_string_wo_append_chars(e_dump_buffer, "for-increment\n");
	if (priv->incrementNode) {
		jag_dec_iintermediate_node_dump(priv->incrementNode, e_dump_buffer, e_prefix_sub);
	} else {
		cat_string_wo_append(e_dump_buffer, e_prefix_sub);
		cat_string_wo_append_chars(e_dump_buffer, "null\n");
	}

	cat_string_wo_append(e_dump_buffer, e_prefix_head);
	cat_string_wo_append_chars(e_dump_buffer, "for-content\n");
	if (priv->loopNode) {
		jag_dec_iintermediate_node_dump(priv->loopNode, e_dump_buffer, e_prefix_sub);
	} else {
		cat_string_wo_append(e_dump_buffer, e_prefix_sub);
		cat_string_wo_append_chars(e_dump_buffer, "null\n");
	}

	cat_unref_ptr(e_prefix_sub);
	cat_unref_ptr(e_prefix_head);
}

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->contains = l_intermediate_node_contains;
	iface->getScopeRef = l_intermediate_node_get_scope_ref;
	iface->isSame = l_intermediate_node_is_same;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->resolveBreaks = l_intermediate_node_resolve_breaks;
	iface->tryPrepend = l_intermediate_node_try_prepend;
	iface->dump = l_intermediate_node_dump;

}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
