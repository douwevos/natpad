/*
   File:    jagdecintermediateframeget.c
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

#include "jagdecintermediateframeget.h"
#include "../../subcode/jagdecconsumer.h"
#include "../../subcode/impl/jagdecframevalueproducer.h"
#include "../../subcode/impl/jagdecmethodframevalue.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateFrameGet"
#include <logging/catlog.h>

struct _JagDecIntermediateFrameGetPrivate {
	int frame_index;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateFrameGet, jag_dec_intermediate_frame_get, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_frame_get_class_init(JagDecIntermediateFrameGetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateFrameGetPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_frame_get_init(JagDecIntermediateFrameGet *instance) {
	JagDecIntermediateFrameGetPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_FRAME_GET, JagDecIntermediateFrameGetPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateFrameGet *instance = JAG_DEC_INTERMEDIATE_FRAME_GET(object);
//	JagDecIntermediateFrameGetPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateFrameGet *jag_dec_intermediate_frame_get_new(int mnemonic_index, int frame_index) {
	JagDecIntermediateFrameGet *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_FRAME_GET, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateFrameGetPrivate *priv = result->priv;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	priv->frame_index = frame_index;
	return result;
}



int jag_dec_intermediate_frame_get_get_frame_index(JagDecIntermediateFrameGet *frame_get) {
	return JAG_DEC_INTERMEDIATE_FRAME_GET_GET_PRIVATE(frame_get)->frame_index;
}




/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateFrameGetPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_GET_GET_PRIVATE(self);

	JagDecMethodFrameValue *frameValue = jag_dec_intermediate_run_context_get_method_frame_value(group_connect_runtime, priv->frame_index);
	if (frameValue==NULL) {
		frameValue = jag_dec_intermediate_run_context_create_method_frame_value(group_connect_runtime, priv->frame_index);
		cat_log_warn("frame-value requested which did not exists yet: frameIndex=%d", priv->frame_index);
	}
	CatArrayWo *e_pseudoStack = jag_dec_intermediate_run_context_get_pseudo_stack(group_connect_runtime);
	int idx;
	for(idx=cat_array_wo_size(e_pseudoStack)-1; idx>=0; idx--) {
		JagDecIValueProducer *producer = (JagDecIValueProducer *) cat_array_wo_get(e_pseudoStack, idx);
		if (JAG_DEC_IS_FRAME_VALUE_PRODUCER(producer)) {
			JagDecFrameValueProducer *frame_value_producer = JAG_DEC_FRAME_VALUE_PRODUCER(producer);
			if (jag_dec_frame_value_producer_get_frame_index(frame_value_producer) == priv->frame_index) {
				JagDecConsumer *dummyconsumer = jag_dec_consumer_new(self);
				jag_dec_ivalue_producer_set_consumer(producer, (JagDecIConsumer *) dummyconsumer);
				cat_unref(dummyconsumer);
				jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, (JagDecIValueProducer *) frame_value_producer);
				cat_array_wo_remove(e_pseudoStack, idx, NULL);
				return;
			}
		}
	}


	JagDecFrameValueProducer *frame_value_producer = jag_dec_frame_value_producer_new(self, frameValue);
	jag_dec_intermediate_run_context_push_value_provider(group_connect_runtime, (JagDecIValueProducer *) frame_value_producer);
	cat_unref(frame_value_producer);


}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateFrameGetPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_GET_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_FRAME_GET(other_node)) {
		JagDecIntermediateFrameGetPrivate *o_priv = JAG_DEC_INTERMEDIATE_FRAME_GET_GET_PRIVATE(other_node);
		return priv->frame_index == o_priv->frame_index;
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateFrameGetPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_GET_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_frame_get_new(mnemonic_index, priv->frame_index);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateFrameGetPrivate *priv = JAG_DEC_INTERMEDIATE_FRAME_GET_GET_PRIVATE(self);
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

