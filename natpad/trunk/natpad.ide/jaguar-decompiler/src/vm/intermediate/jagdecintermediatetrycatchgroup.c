/*
   File:    jagdecintermediatetrycatchgroup.c
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

#include "jagdecintermediatetrycatchgroup.h"
#include "jagdeciintermediatenode.h"
#include "jagdecintermediateruncontext.h"
#include "../subcode/impl/jagdecsubcodefinally.h"
#include "../subcode/impl/jagdecsubcodetrycatchfinal.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateTryCatchGroup"
#include <logging/catlog.h>

struct _JagDecIntermediateTryCatchGroupPrivate {
	JagBytTryCatch *try_catch;
	gboolean catchBlocksResolved;
	CatArrayWo *e_catch_groups;					/* JagDecIntermediateCatchGroup */
	JagDecIntermediateFinallyGroup *finally_group;

};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateTryCatchGroup, jag_dec_intermediate_try_catch_group, JAG_DEC_TYPE_INTERMEDIATE_GROUP, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_try_catch_group_class_init(JagDecIntermediateTryCatchGroupClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateTryCatchGroupPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_try_catch_group_init(JagDecIntermediateTryCatchGroup *instance) {
	JagDecIntermediateTryCatchGroupPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP, JagDecIntermediateTryCatchGroupPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateTryCatchGroup *instance = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP(object);
	JagDecIntermediateTryCatchGroupPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_catch_groups);
	cat_unref_ptr(priv->finally_group);
	cat_unref_ptr(priv->try_catch);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateTryCatchGroup *jag_dec_intermediate_try_catch_group_new(JagBytTryCatch *try_catch) {
	JagDecIntermediateTryCatchGroup *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateTryCatchGroupPrivate *priv = result->priv;
	jag_dec_intermediate_group_construct((JagDecIntermediateGroup *) result);
	priv->try_catch = cat_ref_ptr(try_catch);
	priv->catchBlocksResolved = FALSE;
	priv->e_catch_groups = cat_array_wo_new();
	priv->finally_group = NULL;
	return result;
}

JagBytTryCatch *jag_dec_intermediate_try_catch_group_get_try_catch(JagDecIntermediateTryCatchGroup *try_catch_group) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	return priv->try_catch;
}

void jag_dec_intermediate_try_catch_group_add_catch_group(JagDecIntermediateTryCatchGroup *try_catch_group, JagDecIntermediateCatchGroup *catch_group) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	cat_array_wo_append(priv->e_catch_groups, (GObject *) catch_group);
}

void jag_dec_intermediate_try_catch_group_add_finally_group(JagDecIntermediateTryCatchGroup *try_catch_group, JagDecIntermediateFinallyGroup *finally_group) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	cat_ref_swap(priv->finally_group, finally_group);
}

JagDecIntermediateCatchGroup *jag_dec_intermediate_try_catch_group_find_catch_group(JagDecIntermediateTryCatchGroup *try_catch_group, JagAstDeclarationType *exceptionType) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	JagDecIntermediateCatchGroup *result = NULL;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_catch_groups);
	while(cat_iiterator_has_next(iter)) {
		JagDecIntermediateCatchGroup *catchGroup = (JagDecIntermediateCatchGroup *) cat_iiterator_next(iter);
		if (jag_dec_intermediate_catch_group_get_exception_type(catchGroup) == exceptionType) {
			result = catchGroup;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}

JagDecIntermediateFinallyGroup *jag_dec_intermediate_try_catch_group_get_finally_group(JagDecIntermediateTryCatchGroup *try_catch_group) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	return priv->finally_group;
}


gboolean jag_dec_intermediate_try_catch_group_is_stale(JagDecIntermediateTryCatchGroup *try_catch_group) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	return (cat_array_wo_size(priv->e_catch_groups)==0) && (priv->finally_group==NULL || jag_dec_intermediate_group_count((JagDecIntermediateGroup *) priv->finally_group)==0);
}

void jag_dec_intermediate_try_catch_group_set_catch_blocks_resolved(JagDecIntermediateTryCatchGroup *try_catch_group, gboolean val) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	priv->catchBlocksResolved = val;
}

gboolean jag_dec_intermediate_try_catch_group_are_catch_blocks_resolved(JagDecIntermediateTryCatchGroup *try_catch_group) {
	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(try_catch_group);
	return priv->catchBlocksResolved;
}



/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *model_runtime) {

	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(self);

	JagDecIntermediateRunContext *mainContext = jag_dec_intermediate_run_context_create_branch(model_runtime);

	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self));
	p_iface->connectStageOne(self, mainContext);


	CatArrayWo *e_catchSubCodes = NULL;
//	JagDecSubCodeCatch *catchSubCodes[] = NULL;
	if (priv->e_catch_groups!=NULL && cat_array_wo_size(priv->e_catch_groups)>0) {
		e_catchSubCodes = cat_array_wo_new_size(cat_array_wo_size(priv->e_catch_groups));
		int idx = 0;
		CatIIterator *iter = cat_array_wo_iterator(priv->e_catch_groups);
		while(cat_iiterator_has_next(iter)) {
			JagDecIntermediateCatchGroup *catchGroup = (JagDecIntermediateCatchGroup *) cat_iiterator_next(iter);
			JagDecIntermediateRunContext *catch_branched_runtime = jag_dec_intermediate_run_context_create_branch(model_runtime);
			jag_dec_iintermediate_node_connect_stage_one((JagDecIIntermediateNode *) catchGroup, catch_branched_runtime);
			cat_array_wo_append(e_catchSubCodes, (GObject *) jag_dec_intermediate_catch_group_get_sub_code_catch(catchGroup));
			idx++;
			cat_unref(catch_branched_runtime);
		}
		cat_unref_ptr(iter);
	}

	JagDecSubCodeFinally *subCodeFinally = NULL;
	if (priv->finally_group!=NULL) {
		JagDecIntermediateRunContext *finally_branched_runtime = jag_dec_intermediate_run_context_create_branch(model_runtime);
		jag_dec_iintermediate_node_connect_stage_one((JagDecIIntermediateNode *) priv->finally_group, finally_branched_runtime);
		subCodeFinally = jag_dec_intermediate_finally_group_get_sub_code_finally(priv->finally_group);
		cat_unref_ptr(finally_branched_runtime);
	}
//	cat_log_detail("subCodeFinally="+subCodeFinally+", finallyGroup="+priv->finally_group);

	JagDecSubCodeBlock *sub_code_block = jag_dec_intermediate_run_context_get_sub_code_block(mainContext);
	JagDecSubCodeTryCatchFinal *subCode = jag_dec_sub_code_try_catch_final_new(sub_code_block, subCodeFinally, e_catchSubCodes);
	jag_dec_intermediate_run_context_add_sub_code(model_runtime, (JagDecISubCode *) subCode);
	cat_unref_ptr(subCode);
	cat_unref_ptr(e_catchSubCodes);

	cat_unref_ptr(mainContext);

}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "\n");
	CatStringWo *e_prefix = cat_string_wo_clone(prefix, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix, "   ");

	cat_string_wo_append(e_dump_buffer, e_prefix);
	cat_string_wo_append_chars(e_dump_buffer, "try\n");

	CatStringWo *e_prefix_sub = cat_string_wo_clone(e_prefix, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix_sub, "   ");

	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self));
	p_iface->dump(self, e_dump_buffer, e_prefix_sub);

	JagDecIntermediateTryCatchGroupPrivate *priv = JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(self);
	if (priv->e_catch_groups) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_catch_groups);
		while(cat_iiterator_has_next(iter)) {
			JagDecIIntermediateNode *catch_node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
			cat_string_wo_append(e_dump_buffer, e_prefix);
			cat_string_wo_append_chars(e_dump_buffer, "catch\n");
			jag_dec_iintermediate_node_dump(catch_node, e_dump_buffer, e_prefix_sub);
		}
		cat_unref_ptr(iter);
	}

	if (priv->finally_group) {
		cat_string_wo_append(e_dump_buffer, e_prefix);
		cat_string_wo_append_chars(e_dump_buffer, "finally\n");
		jag_dec_iintermediate_node_dump((JagDecIIntermediateNode *) priv->finally_group, e_dump_buffer, e_prefix_sub);
	}
	cat_unref_ptr(e_prefix);
	cat_unref_ptr(e_prefix_sub);
}




static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->contains = p_iface->contains;
	iface->getScopeRef = p_iface->getScopeRef;
	iface->isSame = p_iface->isSame;
	iface->makeCopy = p_iface->makeCopy;
	iface->resolveBreaks = p_iface->resolveBreaks;
	iface->tryPrepend = p_iface->tryPrepend;
	iface->dump = l_intermediate_node_dump;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
