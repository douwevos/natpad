/*
   File:    jagdecintermediatearrayput.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 14, 2012
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

#include "jagdecintermediatearrayput.h"
#include "../../subcode/impl/jagdecsubcodearrayput.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateArrayPut"
#include <logging/catlog.h>

struct _JagDecIntermediateArrayPutPrivate {
	JagBytType store_type;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateArrayPut, jag_dec_intermediate_array_put, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_array_put_class_init(JagDecIntermediateArrayPutClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateArrayPutPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_array_put_init(JagDecIntermediateArrayPut *instance) {
	JagDecIntermediateArrayPutPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_ARRAY_PUT, JagDecIntermediateArrayPutPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateArrayPut *instance = JAG_DEC_INTERMEDIATE_ARRAY_PUT(object);
//	JagDecIntermediateArrayPutPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateArrayPut *jag_dec_intermediate_array_put_new(int mnemonic_index, JagBytType store_type) {
	JagDecIntermediateArrayPut *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_ARRAY_PUT, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateArrayPutPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->store_type = store_type;
	return result;
}








/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecSubCodeArrayPut *sub_code = jag_dec_sub_code_array_put_new(mnemonic_index);

	JagDecConsumer *value_consumer = jag_dec_consumer_new(self);
	JagDecConsumer *index_consumer = jag_dec_consumer_new(self);
	JagDecConsumer *main_consumer = jag_dec_consumer_new(self);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code, (JagDecIConsumer *) value_consumer);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code, (JagDecIConsumer *) index_consumer);
	jag_dec_abstract_sub_code_add_consumer((JagDecAbstractSubCode *) sub_code, (JagDecIConsumer *) main_consumer);

	jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) value_consumer);
	jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) index_consumer);
	jag_dec_intermediate_run_context_push_value_consumer(group_connect_runtime, (JagDecIConsumer *) main_consumer);

	cat_unref_ptr(value_consumer);
	cat_unref_ptr(index_consumer);
	cat_unref_ptr(main_consumer);

	jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code);
	cat_unref_ptr(sub_code);
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateArrayPutPrivate *priv = JAG_DEC_INTERMEDIATE_ARRAY_PUT_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_ARRAY_PUT(other_node)) {
		JagDecIntermediateArrayPutPrivate *o_priv = JAG_DEC_INTERMEDIATE_ARRAY_PUT_GET_PRIVATE(other_node);
		return priv->store_type==o_priv->store_type;
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateArrayPutPrivate *priv = JAG_DEC_INTERMEDIATE_ARRAY_PUT_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_array_put_new(mnemonic_index, priv->store_type);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateArrayPutPrivate *priv = JAG_DEC_INTERMEDIATE_ARRAY_PUT_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, "[store-type=");
	cat_string_wo_append_chars(e_dump_buffer, jag_byt_type_as_text(priv->store_type));
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


/********************* start CatIStringableInterface implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagDecIntermediateArrayPutPrivate *priv = JAG_DEC_INTERMEDIATE_ARRAY_PUT_GET_PRIVATE(self);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(append_to, name+6);
	cat_string_wo_append_chars(append_to, "[store-type=");
	cat_string_wo_append_chars(append_to, jag_byt_type_as_text(priv->store_type));
	cat_string_wo_append_chars(append_to, "]\n");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringableInterface implementation *********************/
