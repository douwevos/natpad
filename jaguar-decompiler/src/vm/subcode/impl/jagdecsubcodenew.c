/*
   File:    jagdecsubcodenew.c
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

#include "jagdecsubcodenew.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../jagdecisubcode.h"
#include "../../jagdecmodelstagetworuntime.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeNew"
#include <logging/catlog.h>

struct _JagDecSubCodeNewPrivate {
	JagAstDeclarationType *declaration_type;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeNew, jag_dec_sub_code_new, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_new_class_init(JagDecSubCodeNewClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeNewPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_new_init(JagDecSubCodeNew *instance) {
	JagDecSubCodeNewPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_NEW, JagDecSubCodeNewPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeNew *instance = JAG_DEC_SUB_CODE_NEW(object);
	JagDecSubCodeNewPrivate *priv = instance->priv;
	cat_unref_ptr(priv->declaration_type);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeNew *jag_dec_sub_code_new_new(int mnemonic_index, JagAstDeclarationType *declaration_type) {
	JagDecSubCodeNew *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_NEW, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeNewPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->declaration_type = cat_ref_ptr(declaration_type);
	return result;
}






/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeNewPrivate *priv = JAG_DEC_SUB_CODE_NEW_GET_PRIVATE(self);

	JagDecImportRepository *import_repo = jag_dec_model_stage_two_runtime_get_import_repository(modelRuntime);
	JagAstDeclarationType *final_type = jag_dec_import_repository_create_final_declaration_type(import_repo, priv->declaration_type);

	JagBytName *ref_type_name = jag_ast_declaration_type_get_reference_type_name(final_type);
	JagAstIdentifier *identifier = jag_ast_identifier_new_from_name(ref_type_name);
	JagAstIExpression *output_expr = (JagAstIExpression *) jag_ast_new_expression_new(identifier, NULL);
	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	JagDecExpressionHolder *expression_holder = jag_dec_default_value_producer_get_expression_holder((JagDecDefaultValueProducer *) value_producer);
	jag_dec_expression_holder_set(expression_holder, output_expr);
	cat_unref_ptr(output_expr);
	cat_unref_ptr(identifier);
	cat_unref_ptr(final_type);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/



