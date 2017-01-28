/*
   File:    jagdecsubcodecast.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 20, 2012
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

#include "jagdecsubcodecast.h"
#include "../jagdecisubcode.h"
#include "../jagdeciconsumer.h"
#include "../jagdecconsumer.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeCast"
#include <logging/catlog.h>

struct _JagDecSubCodeCastPrivate {
	JagAstDeclarationType *cast_decl_type;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeCast, jag_dec_sub_code_cast, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_cast_class_init(JagDecSubCodeCastClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeCastPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_cast_init(JagDecSubCodeCast *instance) {
	JagDecSubCodeCastPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_CAST, JagDecSubCodeCastPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeCast *instance = JAG_DEC_SUB_CODE_CAST(object);
	JagDecSubCodeCastPrivate *priv = instance->priv;
	cat_unref_ptr(priv->cast_decl_type);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeCast *jag_dec_sub_code_cast_new(int mnemonic_index, JagAstDeclarationType *cast_decl_type) {
	JagDecSubCodeCast *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_CAST, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeCastPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->cast_decl_type = cat_ref_ptr(cast_decl_type);
	return result;
}








/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeCastPrivate *priv = JAG_DEC_SUB_CODE_CAST_GET_PRIVATE(self);
	JagDecImportRepository *import_repository = jag_dec_model_stage_two_runtime_get_import_repository(modelRuntime);
	JagAstDeclarationType *short_cast_decl_type = jag_dec_import_repository_create_final_declaration_type(import_repository, priv->cast_decl_type);

	JagDecIConsumer *consumer = jag_dec_isub_code_get_input_consumer_at(self, 0);
	JagAstIExpression *uncasted_expression = jag_dec_iconsumer_get_expression_ref(consumer, NULL);
	JagAstCastExpression *cast_expression = jag_ast_cast_expression_new(short_cast_decl_type, uncasted_expression);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	jag_dec_ivalue_producer_set_expression(value_producer, (JagAstIExpression *) cast_expression);
	cat_unref_ptr(cast_expression);
	cat_unref_ptr(uncasted_expression);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
