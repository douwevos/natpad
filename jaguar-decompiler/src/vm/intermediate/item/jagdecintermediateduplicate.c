/*
   File:    jagdecintermediateduplicate.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 16, 2012
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

#include "jagdecintermediateduplicate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateDuplicate"
#include <logging/catlog.h>

struct _JagDecIntermediateDuplicatePrivate {
	JagBytOperation dup_operation;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateDuplicate, jag_dec_intermediate_duplicate, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_duplicate_class_init(JagDecIntermediateDuplicateClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateDuplicatePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_duplicate_init(JagDecIntermediateDuplicate *instance) {
	JagDecIntermediateDuplicatePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_DUPLICATE, JagDecIntermediateDuplicatePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateDuplicate *instance = JAG_DEC_INTERMEDIATE_DUPLICATE(object);
//	JagDecIntermediateDuplicatePrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateDuplicate *jag_dec_intermediate_duplicate_new(int mnemonic_index, JagBytOperation dup_operation) {
	JagDecIntermediateDuplicate *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_DUPLICATE, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateDuplicatePrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->dup_operation = dup_operation;
	return result;
}




JagBytOperation jag_dec_intermediate_duplicate_get_duplicate_operator(JagDecIntermediateDuplicate *in_dup) {
	return JAG_DEC_INTERMEDIATE_DUPLICATE_GET_PRIVATE(in_dup)->dup_operation;
}





/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateDuplicatePrivate *priv = JAG_DEC_INTERMEDIATE_DUPLICATE_GET_PRIVATE(self);

	switch(priv->dup_operation) {
		case OP_DUP : {
			JagDecIValueProducer *value_producer = jag_dec_intermediate_run_context_peek_producer(group_connect_runtime, 0);
			if (value_producer!=NULL) {
				JagDecIValueProducer *dup_value_producer = jag_dec_ivalue_producer_duplicate(value_producer);
				jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, dup_value_producer);
				cat_unref_ptr(dup_value_producer);
			} else {
				cat_log_error("invalid stack:");
			}
		} break;
		case OP_DUP2 : {
			JagDecIValueProducer *value_producer2 = jag_dec_intermediate_run_context_peek_producer(group_connect_runtime, 0);
			if (value_producer2!=NULL) {
				JagDecValueTypeDefinitionList *type_set2 = jag_dec_ivalue_producer_get_type_set(value_producer2);
				JagAstDeclarationType *bestType = jag_dec_value_type_definition_list_get_best_type_ref(type_set2);
				gboolean useForm2 = FALSE;
				if (bestType!=NULL) {
					if (jag_ast_declaration_type_is_category2(bestType)) {
						useForm2 = TRUE;
					}
				}
				cat_unref_ptr(bestType);
//				log.detail("dup2: useForm2="+useForm2+", valueProducer2="+valueProducer2);
				if (!useForm2) {
					JagDecIValueProducer *value_producer1 = jag_dec_intermediate_run_context_peek_producer(group_connect_runtime, 1);
					if (value_producer1!=NULL) {
						JagDecIValueProducer *dup_value_producer1 = jag_dec_ivalue_producer_duplicate(value_producer1);
						jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, dup_value_producer1);
						cat_unref_ptr(dup_value_producer1);
					} else {
						cat_log_error("form2: invalid stack:");
					}
				}
				JagDecIValueProducer *dup_value_producer2 = jag_dec_ivalue_producer_duplicate(value_producer2);
				jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, dup_value_producer2);
				cat_unref_ptr(dup_value_producer2);
			} else {
				cat_log_error("no producer: invalid stack:");
			}
		} break;
		case OP_DUP_X1 : {
			JagDecIValueProducer *value_producer = jag_dec_intermediate_run_context_peek_producer(group_connect_runtime, 0);
			if (value_producer!=NULL) {
				JagDecIValueProducer *dup_value_producer = jag_dec_ivalue_producer_duplicate(value_producer);
				jag_dec_intermediate_run_context_push_value_provider_at(group_connect_runtime, dup_value_producer, 2);
				cat_unref_ptr(dup_value_producer);
			} else {
				cat_log_error("invalid stack:");
			}
		} break;
		case OP_DUP_X2 : {
			JagDecIValueProducer *value_producer = jag_dec_intermediate_run_context_peek_producer(group_connect_runtime, 0);
			JagDecIValueProducer *value_test_producer = jag_dec_intermediate_run_context_peek_producer(group_connect_runtime, 1);
			if (value_producer!=NULL && value_test_producer!=NULL) {

				JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set(value_test_producer);
				JagAstDeclarationType *bestType = jag_dec_value_type_definition_list_get_best_type_ref(type_set);
				gboolean useForm2 = FALSE;
				if (bestType!=NULL) {
					if (jag_ast_declaration_type_is_category2(bestType)) {
						useForm2 = TRUE;
					}
				}

				JagDecIValueProducer *dup_value_producer = jag_dec_ivalue_producer_duplicate(value_producer);
				jag_dec_intermediate_run_context_push_value_provider_at(group_connect_runtime, dup_value_producer, useForm2 ? 2 : 3);
				cat_unref_ptr(dup_value_producer);
			} else {
				cat_log_error("invalid stack:");
			}
		} break;
		case OP_DUP2_X1 : {
			cat_log_error("OP_DUP2_X1 needs implementation");
		} break;
		case OP_DUP2_X2 : {
			cat_log_error("OP_DUP2_X2 needs implementation");
		} break;
		default : {} break;
	}



}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node){
	JagDecIntermediateDuplicatePrivate *priv = JAG_DEC_INTERMEDIATE_DUPLICATE_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_DUPLICATE(other_node)) {
		JagDecIntermediateDuplicatePrivate *o_priv = JAG_DEC_INTERMEDIATE_DUPLICATE_GET_PRIVATE(other_node);
		return (priv->dup_operation==o_priv->dup_operation);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateDuplicatePrivate *priv = JAG_DEC_INTERMEDIATE_DUPLICATE_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_duplicate_new(mnemonic_index, priv->dup_operation);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateDuplicatePrivate *priv = JAG_DEC_INTERMEDIATE_DUPLICATE_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	switch(priv->dup_operation) {
		case OP_DUP : cat_string_wo_append_chars(e_dump_buffer, " type=dup"); break;
		case OP_DUP2 : cat_string_wo_append_chars(e_dump_buffer, " type=dup2"); break;
		case OP_DUP_X1 : cat_string_wo_append_chars(e_dump_buffer, " type=dup_x1"); break;
		case OP_DUP_X2 : cat_string_wo_append_chars(e_dump_buffer, " type=dup_x2"); break;
		case OP_DUP2_X1 : cat_string_wo_append_chars(e_dump_buffer, " type=dup2_x1"); break;
		case OP_DUP2_X2 : cat_string_wo_append_chars(e_dump_buffer, " type=dup2_x2"); break;
		default : {} break;
	}
	cat_string_wo_append_char(e_dump_buffer, '\n');

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
