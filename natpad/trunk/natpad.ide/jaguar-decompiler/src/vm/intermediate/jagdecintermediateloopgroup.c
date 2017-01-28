/*
   File:    jagdecintermediateloopgroup.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 17, 2012
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

#include "jagdecintermediateloopgroup.h"
#include "jagdecintermediatenodescope.h"
#include "jagdecintermediateruncontext.h"
#include "../subcode/block/jagdecsubcodeloopblock.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateLoopGroup"
#include <logging/catlog.h>

struct _JagDecIntermediateLoopGroupPrivate {
	void *dummy;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateLoopGroup, jag_dec_intermediate_loop_group, JAG_DEC_TYPE_INTERMEDIATE_GROUP, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_loop_group_class_init(JagDecIntermediateLoopGroupClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateLoopGroupPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_loop_group_init(JagDecIntermediateLoopGroup *instance) {
	JagDecIntermediateLoopGroupPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP, JagDecIntermediateLoopGroupPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateLoopGroup *instance = JAG_DEC_INTERMEDIATE_LOOP_GROUP(object);
//	JagDecIntermediateLoopGroupPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateLoopGroup *jag_dec_intermediate_loop_group_new() {
	JagDecIntermediateLoopGroup *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP, NULL);
	cat_ref_anounce(result);
	jag_dec_intermediate_group_construct((JagDecIntermediateGroup *) result);
	return result;
}


JagDecIntermediateLoopGroup *jag_dec_intermediate_loop_group_new_with_nodes(CatArrayWo *e_node_list) {
	JagDecIntermediateLoopGroup *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP, NULL);
	cat_ref_anounce(result);
	jag_dec_intermediate_group_construct_with_nodes((JagDecIntermediateGroup *) result, e_node_list);
	return result;
}




static JagDecIIntermediateNode *l_get_condition_block(JagDecIntermediateLoopGroup *intermediate_loop_group) {
	return jag_dec_intermediate_group_get_last((JagDecIntermediateGroup *) intermediate_loop_group);

}

/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static JagDecIntermediateNodeScope *l_intermediate_node_get_scope_ref(JagDecIIntermediateNode *self) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self));
	JagDecIntermediateNodeScope *lcScope = p_iface->getScopeRef(self);
	JagDecIIntermediateNode *condition_node = l_get_condition_block((JagDecIntermediateLoopGroup *) self);
	JagDecIntermediateNodeScope *condition_scope = jag_dec_iintermediate_node_get_scope_ref(condition_node);
	jag_dec_intermediate_node_scope_set_exit_index(lcScope, jag_dec_intermediate_node_scope_get_exit_index(condition_scope));
	jag_dec_intermediate_node_scope_set_jump_index(lcScope, JAG_DEC_SCOPE_INDEX_NONE);
	cat_unref_ptr(condition_scope);
	return lcScope;
}

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *model_runtime) {
	JagDecIIntermediateNode *condition_node = l_get_condition_block((JagDecIntermediateLoopGroup *) self);
	jag_dec_iintermediate_node_connect_stage_one(condition_node, model_runtime);

	JagDecIntermediateRunContext *branch_runtime = jag_dec_intermediate_run_context_create_branch(model_runtime);

	int count = jag_dec_intermediate_group_count((JagDecIntermediateGroup *) self);
	int child_idx;
	for(child_idx=0; child_idx<count; child_idx++) {
		JagDecIIntermediateNode *child = jag_dec_intermediate_group_get((JagDecIntermediateGroup *) self, child_idx);
		jag_dec_iintermediate_node_connect_stage_one(child, branch_runtime);
	}

	JagDecSubCodeBlock *branch_sub_code_block = jag_dec_intermediate_run_context_get_sub_code_block(branch_runtime);
	JagDecConsumer *consumer = jag_dec_consumer_new(self);
	JagDecSubCodeLoopBlock *loop_block = jag_dec_sub_code_loop_block_new(consumer, (JagDecISubCode *) branch_sub_code_block);
	jag_dec_intermediate_run_context_add_sub_code(model_runtime, (JagDecISubCode *) loop_block);
	jag_dec_intermediate_run_context_push_value_consumer(model_runtime, (JagDecIConsumer *) consumer);
	cat_unref_ptr(branch_runtime);
	cat_unref_ptr(consumer);
	cat_unref_ptr(loop_block);
}


static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	gboolean result = FALSE;
	if (JAG_DEC_IS_INTERMEDIATE_LOOP_GROUP(other_node)) {
		JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self));
		result = p_iface->isSame(self, other_node);
	}
	return result;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
//	JagDecIntermediateLoopGroupPrivate *priv = JAG_DEC_INTERMEDIATE_LOOP_GROUP_GET_PRIVATE(self);
	CatArrayWo *e_children = jag_dec_intermediate_group_get_all((JagDecIntermediateGroup *) self);
	if (deep) {
		CatArrayWo *e_new_children = cat_array_wo_new();
		CatIIterator *iter = cat_array_wo_iterator(e_new_children);
		while(cat_iiterator_has_next(iter)) {
			JagDecIIntermediateNode *src_node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
			JagDecIIntermediateNode *dst_node = jag_dec_iintermediate_node_make_copy(src_node, TRUE);
			cat_array_wo_append(e_new_children, (GObject *) dst_node);
			cat_unref_ptr(dst_node);
		}
		cat_unref_ptr(iter);
		return (JagDecIIntermediateNode *) jag_dec_intermediate_loop_group_new_with_nodes(e_new_children);
	}
	return (JagDecIIntermediateNode *) jag_dec_intermediate_loop_group_new_with_nodes(e_children);
}




static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
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
	cat_string_wo_append_chars(e_prefix_head, "  ");

	cat_string_wo_append(e_dump_buffer, e_prefix_head);
	cat_string_wo_append_chars(e_dump_buffer, "block\n");

	int count = jag_dec_intermediate_group_count((JagDecIntermediateGroup *) self);
	int child_idx;
	for(child_idx=0; child_idx<count; child_idx++) {
		if (child_idx==count-1) {
			cat_string_wo_append(e_dump_buffer, e_prefix_head);
			cat_string_wo_append_chars(e_dump_buffer, "condition\n");
		}
		JagDecIIntermediateNode *child = jag_dec_intermediate_group_get((JagDecIntermediateGroup *) self, child_idx);
		jag_dec_iintermediate_node_dump(child, e_dump_buffer, e_prefix_sub);
	}

	cat_unref_ptr(e_prefix_sub);
	cat_unref_ptr(e_prefix_head);
}


static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(iface);

	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->getScopeRef = l_intermediate_node_get_scope_ref;
	iface->isSame = l_intermediate_node_is_same;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->dump = l_intermediate_node_dump;

	iface->contains = p_iface->contains;
	iface->resolveBreaks = p_iface->resolveBreaks;
	iface->tryPrepend = p_iface->tryPrepend;

}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
