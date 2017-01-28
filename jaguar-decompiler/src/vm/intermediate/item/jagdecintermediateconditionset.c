/*
   File:    jagdecintermediateconditionset.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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

#include "jagdecintermediateconditionset.h"
#include "jagdecconditionalconsumer.h"
#include "../jagdeciintermediatenode.h"
#include "../jagdecintermediatenodescope.h"
#include "../jagdecintermediateruncontext.h"
#include "../../subcode/impl/jagdecsubcodeconditionset.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateConditionSet"
#include <logging/catlog.h>

struct _JagDecIntermediateConditionSetPrivate {
	CatArrayWo *e_node_list;
	int out_index;
	int parent_out_index;
	int final_out_index;
	JagDecIntermediateNodeScope *scope;
	CatArrayWo *e_consumer_list;

};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateConditionSet, jag_dec_intermediate_condition_set, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_set_condition_type(JagDecIntermediateConditionSet *condition_set, int parentOutIndex, int finalOutIndex);

static void jag_dec_intermediate_condition_set_class_init(JagDecIntermediateConditionSetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateConditionSetPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_condition_set_init(JagDecIntermediateConditionSet *instance) {
	JagDecIntermediateConditionSetPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET, JagDecIntermediateConditionSetPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateConditionSet *instance = JAG_DEC_INTERMEDIATE_CONDITION_SET(object);
	JagDecIntermediateConditionSetPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_consumer_list);
	cat_unref_ptr(priv->e_node_list);
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

JagDecIntermediateConditionSet *jag_dec_intermediate_condition_set_new(int out_index, CatArrayWo *e_node_list) {
	JagDecIntermediateConditionSet *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateConditionSetPrivate *priv = result->priv;
	priv->e_node_list = cat_ref_ptr(e_node_list);
	priv->e_consumer_list = cat_array_wo_new();
	JagDecIIntermediateNode *last_node = (JagDecIIntermediateNode *) cat_array_wo_get_last(e_node_list);
	JagDecIntermediateNodeScope *lcScope = jag_dec_iintermediate_node_get_scope_ref(last_node);
	int exit_index = jag_dec_intermediate_node_scope_get_exit_index(lcScope);
	int jump_index = jag_dec_intermediate_node_scope_get_jump_index(lcScope);
	cat_unref_ptr(lcScope);
	priv->scope = jag_dec_intermediate_node_scope_new(exit_index, jump_index);
	priv->out_index = out_index;
	l_set_condition_type(result, exit_index, exit_index);
	return result;
}



static void l_set_condition_type(JagDecIntermediateConditionSet *condition_set, int parentOutIndex, int finalOutIndex) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(condition_set);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_node_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		if (JAG_DEC_IS_INTERMEDIATE_CONDITION_SET(node)) {
			JagDecIntermediateConditionSet *child = (JagDecIntermediateConditionSet *) node;
			l_set_condition_type(child, priv->out_index, finalOutIndex);
		}
	}
	cat_unref_ptr(iter);
	priv->parent_out_index = parentOutIndex;
	priv->final_out_index = finalOutIndex;
}






/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self);
	JagDecSubCodeConditionSet *sub_code_condition_set = jag_dec_sub_code_condition_set_new(-1, priv->out_index, priv->parent_out_index, priv->final_out_index);

	CatIIterator *iter = cat_array_wo_iterator(priv->e_node_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *child_node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);

		jag_dec_iintermediate_node_connect_stage_one(child_node, group_connect_runtime);
		gboolean shouldInvert = FALSE;
		if (JAG_DEC_IS_INTERMEDIATE_GROUP(child_node)) {
			JagDecIntermediateNodeScope *child_scope = jag_dec_iintermediate_node_get_scope_ref(child_node);
			int child_jump_index = jag_dec_intermediate_node_scope_get_jump_index(child_scope);
			cat_unref_ptr(child_scope);
			gboolean is_last_and_inverted = priv->parent_out_index!=priv->final_out_index;
			shouldInvert = !((child_jump_index!=priv->parent_out_index)^is_last_and_inverted);
		}
		JagDecConditionalConsumer *child_consumer = jag_dec_conditional_consumer_new(self, shouldInvert);
		jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_condition_set, (JagDecIConsumer *) child_consumer);
		cat_unref(child_consumer);
	}
	cat_unref_ptr(iter);


	JagDecDefaultValueProducer *value_producer = jag_dec_default_value_producer_new(self);
	JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
	jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_BOOLEAN, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONDITION);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_condition_set, (JagDecIValueProducer *) value_producer);
	cat_unref_ptr(value_producer);

	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_condition_set);
	cat_unref(sub_code_condition_set);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_CONDITION_SET(other_node)) {
		JagDecIntermediateConditionSetPrivate *o_priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(other_node);
		return cat_array_wo_equal(priv->e_node_list, o_priv->e_node_list, (GEqualFunc) jag_dec_iintermediate_node_is_same);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self);
	JagDecIntermediateConditionSet *result = NULL;
	if (deep) {
		CatArrayWo *e_copied_node_list = cat_array_wo_new();
		CatIIterator *iter = cat_array_wo_iterator(priv->e_consumer_list);
		while(cat_iiterator_has_next(iter)) {
			JagDecIIntermediateNode *sub_node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
			JagDecIIntermediateNode *copied = jag_dec_iintermediate_node_make_copy(sub_node, TRUE);
			cat_array_wo_append(e_copied_node_list, (GObject *) copied);
			cat_unref_ptr(copied);
		}
		cat_unref_ptr(iter);

		result = jag_dec_intermediate_condition_set_new(priv->out_index, e_copied_node_list);
		cat_unref_ptr(e_copied_node_list);
	} else {
		result = jag_dec_intermediate_condition_set_new(priv->out_index, priv->e_node_list);
	}
	return (JagDecIIntermediateNode *) result;
}

static gboolean l_intermediate_node_contains(JagDecIIntermediateNode *self, int mnemonic_index) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self);
	gboolean result = FALSE;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_node_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		if (jag_dec_iintermediate_node_contains(node, mnemonic_index)) {
			result = TRUE;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}

static JagDecIntermediateNodeScope *l_intermediate_node_get_scope_ref(JagDecIIntermediateNode *self) {
	return cat_ref_ptr(JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self)->scope);
}

static void l_intermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jump_index) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_node_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		jag_dec_iintermediate_node_resolve_breaks(node, jump_index);
	}
	cat_unref_ptr(iter);

}

static gboolean l_intermediate_node_try_prepend(JagDecIIntermediateNode *self, JagDecIIntermediateNode *node) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self);
	gboolean result = FALSE;
	if (cat_array_wo_size(priv->e_node_list)>0) {
		JagDecIIntermediateNode *child_node = (JagDecIIntermediateNode *) cat_array_wo_get_first(priv->e_node_list);
		jag_dec_iintermediate_node_try_prepend(child_node, node);
		result = TRUE;
	}
	return result;
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateConditionSetPrivate *priv = JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[");
	jag_dec_intermediate_node_scope_as_text(priv->scope, e_dump_buffer);
	cat_string_wo_append_chars(e_dump_buffer, ", out-index=");
	cat_string_wo_append_decimal(e_dump_buffer, priv->out_index);
	cat_string_wo_append_chars(e_dump_buffer, ", parent-out-index=");
	cat_string_wo_append_decimal(e_dump_buffer, priv->parent_out_index);
	cat_string_wo_append_chars(e_dump_buffer, ", final-out-index=");
	cat_string_wo_append_decimal(e_dump_buffer, priv->final_out_index);
	cat_string_wo_append_chars(e_dump_buffer, "]\n");

	CatStringWo *e_prefix_head = cat_string_wo_clone(prefix, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix_head, "  [");
	CatStringWo *e_prefix_sub =  cat_string_wo_clone(prefix, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix_sub, "       ");

	int node_count = cat_array_wo_size(priv->e_node_list);
	int node_idx;
	for(node_idx=0; node_idx<node_count; node_idx++) {
		cat_string_wo_append(e_dump_buffer, e_prefix_head);
		cat_string_wo_append_decimal(e_dump_buffer, node_idx);
		cat_string_wo_append_chars(e_dump_buffer, "] ");
		JagDecIIntermediateNode *child = (JagDecIIntermediateNode *) cat_array_wo_get(priv->e_node_list, node_idx);
		jag_dec_iintermediate_node_dump(child, e_dump_buffer, e_prefix_sub);
	}

	cat_unref_ptr(e_prefix_sub);
	cat_unref_ptr(e_prefix_head);
}



static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->isSame = l_intermediate_node_is_same;
	iface->dump = l_intermediate_node_dump;
	iface->contains = l_intermediate_node_contains;
	iface->getScopeRef = l_intermediate_node_get_scope_ref;
	iface->resolveBreaks = l_intermediate_node_resolve_breaks;
	iface->tryPrepend = l_intermediate_node_try_prepend;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
