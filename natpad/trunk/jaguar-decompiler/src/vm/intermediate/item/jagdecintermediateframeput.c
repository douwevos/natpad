/*
   File:    jagdecintermediateframeput.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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

#include "jagdecintermediateframeput.h"
#include "../../subcode/jagdeciconsumer.h"
#include "../../subcode/jagdecisubcode.h"
#include "../../subcode/impl/jagdecsubcodeframeput.h"
#include "../../subcode/impl/jagdecmethodframevalue.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateFramePut"
#include <logging/catlog.h>

struct _JagDecIntermediateFramePutPrivate {
	int frame_index;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateFramePut, jag_dec_intermediate_frame_put, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_frame_put_class_init(JagDecIntermediateFramePutClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateFramePutPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_frame_put_init(JagDecIntermediateFramePut *instance) {
	JagDecIntermediateFramePutPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_FRAME_PUT, JagDecIntermediateFramePutPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateFramePut *instance = JAG_DEC_INTERMEDIATE_FRAME_PUT(object);
//	JagDecIntermediateFramePutPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateFramePut *jag_dec_intermediate_frame_put_new(int mnemonic_index, int frame_index) {
	JagDecIntermediateFramePut *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_FRAME_PUT, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateFramePutPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->frame_index = frame_index;
	return result;
}




int jag_dec_intermediate_frame_put_get_frame_index(JagDecIntermediateFramePut *frame_put) {
	return JAG_DEC_INTERMEDIATE_FRAME_PUT_GET_PRIVATE(frame_put)->frame_index;
}




/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateFramePutPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_PUT_GET_PRIVATE(self);

	gboolean isDeclarator = FALSE;
	JagDecMethodFrameValue *frameValue = jag_dec_intermediate_run_context_get_method_frame_value(group_connect_runtime, priv->frame_index);
	if (frameValue==NULL) {
		frameValue = jag_dec_intermediate_run_context_create_method_frame_value(group_connect_runtime, priv->frame_index);
		isDeclarator = TRUE;
	}

	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);

	JagBytLocalVariableTable *loc_var_tab = jag_dec_intermediate_run_context_get_local_variable_table(group_connect_runtime);
	if (loc_var_tab!=NULL) {
		JagBytIMnemonicBlock *mnemonic_block = jag_dec_intermediate_run_context_get_mnemonic_block(group_connect_runtime);
		JagBytIMnemonic *mnemonic = jag_byt_imnemonic_block_get(mnemonic_block, mnemonic_index);
		int offset = jag_byt_imnemonic_get_offset(mnemonic);
		int length = jag_byt_imnemonic_get_length(mnemonic);
		JagBytLocalVariableTableEntry *var_tab_entry = jag_byt_local_variable_table_find_entry(loc_var_tab, priv->frame_index, offset+length);
		if (var_tab_entry!=NULL) {
			jag_dec_method_frame_value_set_name(frameValue, jag_byt_local_variable_table_entry_get_name(var_tab_entry));
			JagAstDeclarationType *decl_type = jag_byt_local_variable_table_entry_get_declaration_type(var_tab_entry);
			if (decl_type) {
				JagDecValueTypeDefinition *val_type_def =  jag_dec_value_type_definition_new(decl_type, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_LOCALVALUETABLE);
				jag_dec_value_type_definition_list_add_definition((JagDecValueTypeDefinitionList *) frameValue, val_type_def);
				cat_unref_ptr(val_type_def);
			}
		}
	}


	JagDecSubCodeFramePut *subCode = jag_dec_sub_code_frame_put_new(mnemonic_index, frameValue, isDeclarator);
	if (isDeclarator) {
		jag_dec_intermediate_run_context_add_post_importer(group_connect_runtime, JAG_DEC_IPOST_IMPORT(subCode));
	}
	JagDecIConsumer *sub_code_consumer = (JagDecIConsumer *) jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) subCode, sub_code_consumer);
	cat_unref(sub_code_consumer);



//	ValueTypeDefinitionList leftHandSide = frameValue;							// TODO implement
//	ValueTypeDefinitionList rightHandSide = sub_code_consumer.getTypeSet();
//	modelRuntime.addValueTypeDefinitionList(leftHandSide);
//	modelRuntime.addValueTypeDefinitionList(rightHandSide);
//	CrossRuleLeftValue crossRule = new CrossRuleLeftValue(leftHandSide, rightHandSide);
//	leftHandSide.addCrossRule(crossRule);
////		rightHandSide.addCrossRule(crossRule);


	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) subCode);
	cat_unref_ptr(subCode);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateFramePutPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_PUT_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_FRAME_PUT(other_node)) {
		JagDecIntermediateFramePutPrivate *o_priv = JAG_DEC_INTERMEDIATE_FRAME_PUT_GET_PRIVATE(other_node);
		return priv->frame_index == o_priv->frame_index;
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateFramePutPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_PUT_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_frame_put_new(mnemonic_index, priv->frame_index);
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateFramePutPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_PUT_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[frame-index=");
	cat_string_wo_append_decimal(e_dump_buffer, priv->frame_index);
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

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
