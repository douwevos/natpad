/*
   File:    jagdecabstractsubcode.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#include "jagdecabstractsubcode.h"
#include "jagdecisubcode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecAbstractSubCode"
#include <logging/catlog.h>

struct _JagDecAbstractSubCodePrivate {
	int index;
	JagDecIValueProducer *value_producer;
	CatArrayWo *e_consumer_list;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);


G_DEFINE_ABSTRACT_TYPE_WITH_CODE(JagDecAbstractSubCode, jag_dec_abstract_sub_code, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_abstract_sub_code_class_init(JagDecAbstractSubCodeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecAbstractSubCodePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_abstract_sub_code_init(JagDecAbstractSubCode *instance) {
	JagDecAbstractSubCodePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, JagDecAbstractSubCodePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecAbstractSubCode *instance = JAG_DEC_ABSTRACT_SUB_CODE(object);
	JagDecAbstractSubCodePrivate *priv = instance->priv;
	cat_unref_ptr(priv->value_producer);
	cat_unref_ptr(priv->e_consumer_list);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_dec_abstract_sub_code_construct(JagDecAbstractSubCode *sub_code, int index) {
	JagDecAbstractSubCodePrivate *priv = JAG_DEC_ABSTRACT_SUB_CODE_GET_PRIVATE(sub_code);
	priv->index = index;
	priv->value_producer = NULL;
	priv->e_consumer_list = cat_array_wo_new();
}


int jag_dec_abstract_sub_code_get_mnemonic_index(JagDecAbstractSubCode *sub_code) {
	JagDecAbstractSubCodePrivate *priv = JAG_DEC_ABSTRACT_SUB_CODE_GET_PRIVATE(sub_code);
	return priv->index;
}

void jag_dec_abstract_sub_code_set_value_producer(JagDecAbstractSubCode *sub_code, JagDecIValueProducer *value_producer) {
	JagDecAbstractSubCodePrivate *priv = JAG_DEC_ABSTRACT_SUB_CODE_GET_PRIVATE(sub_code);
	cat_ref_swap(priv->value_producer, value_producer);
}

void jag_dec_abstract_sub_code_add_consumer(JagDecAbstractSubCode *sub_code, JagDecIConsumer *consumer) {
	JagDecAbstractSubCodePrivate *priv = JAG_DEC_ABSTRACT_SUB_CODE_GET_PRIVATE(sub_code);
	cat_array_wo_append(priv->e_consumer_list, (GObject *) consumer);
}



/********************* start JagDecISubCodeInterface implementation *********************/


//static void l_sub_code_connect_stage_two(JagDecISubCode *self, struct _JagDecModelStageTwoRuntime *modelRuntime) {
//
//}
//

static struct _JagDecIValueProducer *l_sub_code_get_output_producer(JagDecISubCode *self) {
	JagDecAbstractSubCodePrivate *priv = JAG_DEC_ABSTRACT_SUB_CODE_GET_PRIVATE(self);
	return priv->value_producer;
}

struct _JagDecIConsumer *l_sub_code_get_input_consumer_at(JagDecISubCode *self, int index) {
	JagDecAbstractSubCodePrivate *priv = JAG_DEC_ABSTRACT_SUB_CODE_GET_PRIVATE(self);
	return (struct _JagDecIConsumer *) cat_array_wo_get(priv->e_consumer_list, index);
}



static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
//	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getOutputProducer = l_sub_code_get_output_producer;
	iface->getInputConsumerAt = l_sub_code_get_input_consumer_at;
}


/********************* end JagDecISubCodeInterface implementation *********************/
