/*
   File:    jagdecintermediateincrement.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 18, 2012
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

#include "jagdecintermediateincrement.h"
#include "../../subcode/impl/jagdecsubcodeincrement.h"
#include "../../subcode/impl/jagdecmethodframevalue.h"
#include "../../subcode/impl/jagdecframevalueproducer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateIncrement"
#include <logging/catlog.h>

struct _JagDecIntermediateIncrementPrivate {
	int frame_index;
	int value;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateIncrement, jag_dec_intermediate_increment, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_increment_class_init(JagDecIntermediateIncrementClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateIncrementPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_increment_init(JagDecIntermediateIncrement *instance) {
	JagDecIntermediateIncrementPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_INCREMENT, JagDecIntermediateIncrementPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateIncrement *jag_dec_intermediate_increment_new(int mnemonic_index, int frame_index, int value) {
	JagDecIntermediateIncrement *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_INCREMENT, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateIncrementPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->frame_index = frame_index;
	priv->value = value;
	return result;
}


int jag_dec_intermediate_increment_get_frame_index(JagDecIntermediateIncrement *inc) {
	return JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(inc)->frame_index;
}

int jag_dec_intermediate_increment_get_value(JagDecIntermediateIncrement *inc) {
	return JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(inc)->value;
}





/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateIncrementPrivate *priv = JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(self);

	CatArrayWo *e_producer_stack = jag_dec_intermediate_run_context_get_producer_stack(group_connect_runtime);
	gboolean commandConsumed = FALSE;
	if (priv->value==1 || priv->value ==-1) {
		/* TODO: we can not just trust the stack, or can we ? */
		int idx;
		for(idx=cat_array_wo_size(e_producer_stack)-1; !commandConsumed && idx>=0; idx--) {
			JagDecIValueProducer *producer = (JagDecIValueProducer *) cat_array_wo_get(e_producer_stack, idx);
			if (JAG_DEC_IS_FRAME_VALUE_PRODUCER(producer)) {
				JagDecFrameValueProducer *frameValueProducer = (JagDecFrameValueProducer *) producer;
				int fvp_frame_index = jag_dec_frame_value_producer_get_frame_index(frameValueProducer);
				if (fvp_frame_index == priv->frame_index) {
					if (priv->value==1) {
						jag_dec_frame_value_producer_set_ppa(frameValueProducer, JAG_DEC_PPA_POSTFIX_INCREMENT);
					} else {
						jag_dec_frame_value_producer_set_ppa(frameValueProducer, JAG_DEC_PPA_POSTFIX_DECREMENT);
					}
					commandConsumed = TRUE;
				}
			}
		}
	}

	if (!commandConsumed) {
		JagDecMethodFrameValue *methodFrameValue = jag_dec_intermediate_run_context_create_method_frame_value(group_connect_runtime, priv->frame_index);
//		if (modelRuntime.localVariableTable!=null) {	// TODO implement
//			LocalVariableTableEntry variableTableEntry = modelRuntime.localVariableTable.findEntry(frameIndex, mnemonicIndex);
//			if (variableTableEntry!=null) {
//				methodFrameValue.setName(variableTableEntry.nameText);
//				methodFrameValue.add(new ValueTypeDefinition(variableTableEntry.declarationType, ValueTypeDefinitionKind.LOCALVALUETABLE));
//			}
//		}

		int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
		JagDecSubCodeIncrement *subCodeIncrement = jag_dec_sub_code_increment_new(mnemonic_index, methodFrameValue, priv->value);
//		JagDecIValueProducer *value_producer = jag_dec_sub_code_increment_get_value_producer(subCodeIncrement);
//		if (value_producer!=NULL) {
//			jag_dec_intermediate_run_context_push_pseudo_producer(group_connect_runtime, value_producer);
//		}

		JagDecIValueProducer *valueProducer = NULL;
		if (priv->value==1) {
			JagDecFrameValueProducer *frameValueProducer = jag_dec_frame_value_producer_new(self, methodFrameValue);
			jag_dec_frame_value_producer_set_ppa(frameValueProducer, JAG_DEC_PPA_PREFIX_INCREMENT);
			valueProducer = (JagDecIValueProducer *) frameValueProducer;
		} else if (priv->value==-1) {
			JagDecFrameValueProducer *frameValueProducer = jag_dec_frame_value_producer_new(self, methodFrameValue);
			jag_dec_frame_value_producer_set_ppa(frameValueProducer, JAG_DEC_PPA_PREFIX_DECREMENT);
			valueProducer = (JagDecIValueProducer *) frameValueProducer;
		} else {
			CatInteger *ival =  cat_integer_new(priv->value);
			JagAstIExpression *incrementExpression = (JagAstIExpression *) jag_ast_literal_new((CatIStringable *) ival);
			cat_unref_ptr(ival);
			CatArrayWo *e_expressions = cat_array_wo_new();
			JagAstIExpression *mfv_expr = jag_dec_method_frame_value_get_expression_ref(methodFrameValue, NULL);
			cat_array_wo_append(e_expressions, (GObject *) mfv_expr);
			cat_unref_ptr(mfv_expr);
			cat_array_wo_append(e_expressions, (GObject *) incrementExpression);
			cat_unref_ptr(incrementExpression);
			JagAstInfixExpression *expression = jag_ast_infix_expression_new(JAG_BYT_MATH_OPERATOR_ADD, e_expressions);
			cat_unref_ptr(e_expressions);
			JagDecDefaultValueProducer *defaultValueProducer = jag_dec_default_value_producer_new(self);
			valueProducer = (JagDecIValueProducer *) defaultValueProducer;
			jag_dec_ivalue_producer_set_expression(valueProducer, (JagAstIExpression *) expression);
			cat_unref_ptr(expression);
		}


		if (valueProducer!=NULL) {
			jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) subCodeIncrement, valueProducer);
			jag_dec_intermediate_run_context_push_pseudo_producer(group_connect_runtime, valueProducer);
			cat_unref(valueProducer);
		}

		jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) subCodeIncrement);
		cat_unref_ptr(subCodeIncrement);
	}
}




static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateIncrementPrivate *priv = JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_INCREMENT(other_node)) {
		JagDecIntermediateIncrementPrivate *o_priv = JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(other_node);
		return (priv->frame_index=o_priv->frame_index) &&
				(priv->value=o_priv->value);
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateIncrementPrivate *priv = JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_increment_new(mnemonic_index, priv->frame_index, priv->value);
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[frame-index=");
	JagDecIntermediateIncrementPrivate *priv = JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(self);
	cat_string_wo_append_decimal(e_dump_buffer, priv->frame_index);
	cat_string_wo_append_chars(e_dump_buffer, ", increment-value=");
	cat_string_wo_append_decimal(e_dump_buffer, priv->value);
	cat_string_wo_append_char(e_dump_buffer, ']');
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
