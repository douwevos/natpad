/*
   File:    jagdecintermediatemonitorenter.c
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

#include "jagdecintermediatemonitorenter.h"
#include "../../subcode/block/jagdecsubcodesynchronizedblock.h"
#include "../jagdecintermediateruncontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateMonitorEnter"
#include <logging/catlog.h>

struct _JagDecIntermediateMonitorEnterPrivate {
	gboolean is_resolved;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateMonitorEnter, jag_dec_intermediate_monitor_enter, JAG_DEC_TYPE_INTERMEDIATE_GROUP, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_monitor_enter_class_init(JagDecIntermediateMonitorEnterClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateMonitorEnterPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_monitor_enter_init(JagDecIntermediateMonitorEnter *instance) {
	JagDecIntermediateMonitorEnterPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_MONITOR_ENTER, JagDecIntermediateMonitorEnterPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateMonitorEnter *instance = JAG_DEC_INTERMEDIATE_MONITOR_ENTER(object);
//	JagDecIntermediateMonitorEnterPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateMonitorEnter *jag_dec_intermediate_monitor_enter_new() {
	JagDecIntermediateMonitorEnter *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_MONITOR_ENTER, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateMonitorEnterPrivate *priv = result->priv;
	jag_dec_intermediate_group_construct((JagDecIntermediateGroup *) result);
	priv->is_resolved = FALSE;
	return result;
}




gboolean jag_dec_intermediate_monitor_enter_is_resolved(JagDecIntermediateMonitorEnter *monitor_enter) {
	return JAG_DEC_INTERMEDIATE_MONITOR_ENTER_GET_PRIVATE(monitor_enter)->is_resolved;
}

void jag_dec_intermediate_monitor_enter_set_resolved(JagDecIntermediateMonitorEnter *monitor_enter, gboolean new_val) {
	JAG_DEC_INTERMEDIATE_MONITOR_ENTER_GET_PRIVATE(monitor_enter)->is_resolved = new_val;
}





/********************* start JagDecIIntermediateNode implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
//	JagDecIntermediateMonitorEnterPrivate *priv = JAG_DEC_INTERMEDIATE_MONITOR_ENTER_GET_PRIVATE(self);
	JagDecConsumer *consumer = jag_dec_consumer_new(self);
	JagDecIntermediateRunContext *branch_runtime = jag_dec_intermediate_run_context_create_branch(group_connect_runtime);

	JagDecSubCodeBlock *branch_sub_code_block = jag_dec_intermediate_run_context_get_sub_code_block(branch_runtime);

	JagDecSubCodeSynchronizedBlock *sub_code_synchronized_block = jag_dec_sub_code_synchronized_block_new(0, (JagDecISubCode *) branch_sub_code_block, consumer);
	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_synchronized_block);
	jag_dec_intermediate_run_context_push_value_consumer(branch_runtime, (JagDecIConsumer *) consumer);

	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self));
	p_iface->connectStageOne(self, branch_runtime);
	cat_unref_ptr(sub_code_synchronized_block);
	cat_unref_ptr(consumer);
	cat_unref_ptr(branch_runtime);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	if (JAG_DEC_IS_INTERMEDIATE_MONITOR_ENTER(other_node)) {
		return TRUE;
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	return (JagDecIIntermediateNode *) jag_dec_intermediate_monitor_enter_new();
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
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

/********************* end JagDecIIntermediateNode implementation *********************/

