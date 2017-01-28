/*
   File:    jagdecintermediatemathbasic.c
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

#include "jagdecintermediatemathbasic.h"
#include "../../subcode/impl/jagdecsubcodemathbasic.h"
#include "../../subcode/jagdecdefaultvalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateMathBasic"
#include <logging/catlog.h>

struct _JagDecIntermediateMathBasicPrivate {
	JagBytMathOperator math_operator;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateMathBasic, jag_dec_intermediate_math_basic, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_math_basic_class_init(JagDecIntermediateMathBasicClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateMathBasicPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_math_basic_init(JagDecIntermediateMathBasic *instance) {
	JagDecIntermediateMathBasicPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC, JagDecIntermediateMathBasicPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateMathBasic *instance = JAG_DEC_INTERMEDIATE_MATH_BASIC(object);
//	JagDecIntermediateMathBasicPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateMathBasic *jag_dec_intermediate_math_basic_new(int mnemonic_index, JagBytMathOperator math_operator) {
	JagDecIntermediateMathBasic *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateMathBasicPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->math_operator = math_operator;
	return result;
}



JagBytMathOperator jag_dec_intermediate_math_basic_get_operator(JagDecIntermediateMathBasic *math_basic) {
	return JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_PRIVATE(math_basic)->math_operator;
}




static void l_create_safe_type_set(JagDecValueTypeDefinitionList *leftTypeSet, JagDecValueTypeDefinitionList *rightTypeSet, JagDecValueTypeDefinitionList *resulTypeSet) {
	gboolean do_include_left = FALSE;
	gboolean do_include_right = FALSE;
	if (jag_dec_value_type_definition_list_is_empty(leftTypeSet)) {
		if (jag_dec_value_type_definition_list_is_empty(rightTypeSet)) {
			/* both typesets are empty we do nothing */
			return;
		}
		do_include_right = TRUE;
	} else if (jag_dec_value_type_definition_list_is_empty(rightTypeSet)) {
		do_include_left = TRUE;
	} else {
		do_include_right = TRUE;
		do_include_left = TRUE;
	}

	if (do_include_left) {
		CatIIterator *iter = jag_dec_value_type_definition_list_iterator(leftTypeSet);
		while(cat_iiterator_has_next(iter)) {
			JagDecValueTypeDefinition *val_type_def = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
			if (jag_dec_value_type_definition_is_included(val_type_def)) {
				val_type_def = jag_dec_value_type_definition_clone(val_type_def);
				jag_dec_value_type_definition_list_add_definition(resulTypeSet, val_type_def);
				cat_unref(val_type_def);
			}
		}
		cat_unref_ptr(iter);
	}

	if (do_include_right) {

	}
}



/********************* start JagDecIIntermediateNode implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateMathBasicPrivate *priv = JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_PRIVATE(self);

	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeMathBasic *sub_code_math_basic = jag_dec_sub_code_math_basic_new(mnemonic_index, priv->math_operator);

	JagDecConsumer *left_consumer = jag_dec_consumer_new(self);
	JagDecConsumer *right_consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_math_basic, (JagDecIConsumer *) right_consumer);
	jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) right_consumer);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code_math_basic, (JagDecIConsumer *) left_consumer);
	jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) left_consumer);

	JagDecValueTypeDefinitionList *rightTypeSet = jag_dec_consumer_get_type_set(right_consumer);
	JagDecValueTypeDefinitionList *leftTypeSet = jag_dec_consumer_get_type_set(left_consumer);


	JagDecIValueProducer *outputValueProducer = (JagDecIValueProducer *) jag_dec_default_value_producer_new(self);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code_math_basic, outputValueProducer);

	JagDecValueTypeDefinitionList *resultTypeSet = jag_dec_ivalue_producer_get_type_set(outputValueProducer);
	if ((leftTypeSet!=NULL) && (rightTypeSet!=NULL)) {
		l_create_safe_type_set(leftTypeSet, rightTypeSet, resultTypeSet);
	}

	jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, outputValueProducer);
	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code_math_basic);

	cat_unref_ptr(sub_code_math_basic);
	cat_unref_ptr(left_consumer);
	cat_unref_ptr(right_consumer);
	cat_unref_ptr(outputValueProducer);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateMathBasicPrivate *priv = JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_PRIVATE(self);

	if (JAG_DEC_IS_INTERMEDIATE_MATH_BASIC(other_node)) {
		JagDecIntermediateMathBasicPrivate *o_priv = JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_PRIVATE(other_node);
		return (priv->math_operator==o_priv->math_operator);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateMathBasicPrivate *priv = JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_math_basic_new(mnemonic_index, priv->math_operator);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateMathBasicPrivate *priv = JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[operator=");
	cat_string_wo_append_chars(e_dump_buffer, jag_byt_math_operator_to_text(priv->math_operator));
	cat_string_wo_append_chars(e_dump_buffer, "]\n");
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

