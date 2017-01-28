/*
   File:    jagdecintermediatearraylength.c
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

#include "jagdecintermediatearraylength.h"
#include "../../subcode/impl/jagdecsubcodearraylength.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateArrayLength"
#include <logging/catlog.h>

struct _JagDecIntermediateArrayLengthPrivate {
	void *dummy;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateArrayLength, jag_dec_intermediate_array_length, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_array_length_class_init(JagDecIntermediateArrayLengthClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateArrayLengthPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_array_length_init(JagDecIntermediateArrayLength *instance) {
	JagDecIntermediateArrayLengthPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_ARRAY_LENGTH, JagDecIntermediateArrayLengthPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateArrayLength *instance = JAG_DEC_INTERMEDIATE_ARRAY_LENGTH(object);
//	JagDecIntermediateArrayLengthPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateArrayLength *jag_dec_intermediate_array_length_new(int mnemonic_index) {
	JagDecIntermediateArrayLength *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_ARRAY_LENGTH, NULL);
	cat_ref_anounce(result);
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	return result;
}








/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeArrayLength *sub_code = jag_dec_sub_code_array_length_new(mnemonic_index);
	JagDecConsumer *array_consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code, (JagDecIConsumer *) array_consumer);
	cat_unref_ptr(array_consumer);

	JagDecDefaultValueProducer *value_producer =  jag_dec_default_value_producer_new(self);
	JagDecValueTypeDefinitionList *type_set = jag_dec_ivalue_producer_get_type_set((JagDecIValueProducer *) value_producer);
	jag_dec_value_type_definition_list_add_basic(type_set, JAG_BYT_TYPE_INT, JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT);
	jag_dec_abstract_sub_code_set_value_producer((JagDecAbstractSubCode *) sub_code, (JagDecIValueProducer *) value_producer);
	cat_unref_ptr(value_producer);

	jag_dec_intermediate_run_context_add_and_run_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code);
	cat_unref_ptr(sub_code);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
//	JagDecIntermediateArrayLengthPrivate *priv = JAG_DEC_INTERMEDIATE_ARRAY_LENGTH_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_ARRAY_LENGTH(other_node)) {
//		JagDecIntermediateConstantPrivate *o_priv = JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(other_node);
		return TRUE;
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_array_length_new(mnemonic_index);
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

/********************* end JagDecIIntermediateNodeInterface implementation *********************/

/********************* start CatIStringableInterface implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *buf) {
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(buf, name+6);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringableInterface implementation *********************/
