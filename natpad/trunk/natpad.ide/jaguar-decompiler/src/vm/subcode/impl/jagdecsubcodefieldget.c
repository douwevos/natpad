/*
   File:    jagdecsubcodefieldget.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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

#include "jagdecsubcodefieldget.h"
#include "jagdecexpressionholder.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeFieldGet"
#include <logging/catlog.h>

struct _JagDecSubCodeFieldGetPrivate {
	JagDecValueTypeDefinitionList *type_set;
	JagAstDeclarationType *field_source;
	JagAstIdentifier *field_name;
	gboolean is_non_static;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeFieldGet, jag_dec_sub_code_field_get, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_field_get_class_init(JagDecSubCodeFieldGetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeFieldGetPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_field_get_init(JagDecSubCodeFieldGet *instance) {
	JagDecSubCodeFieldGetPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_FIELD_GET, JagDecSubCodeFieldGetPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeFieldGet *instance = JAG_DEC_SUB_CODE_FIELD_GET(object);
	JagDecSubCodeFieldGetPrivate *priv = instance->priv;
	cat_unref_ptr(priv->type_set);
	cat_unref_ptr(priv->field_source);
	cat_unref_ptr(priv->field_name);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeFieldGet *jag_dec_sub_code_field_get_new(int mnemonic_index, JagAstDeclarationType *field_source, JagAstIdentifier *field_name, gboolean is_non_static) {
	JagDecSubCodeFieldGet *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_FIELD_GET, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeFieldGetPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->is_non_static = is_non_static;
	priv->field_source = cat_ref_ptr(field_source);
	priv->field_name = cat_ref_ptr(field_name);
	return result;
}




/********************* start JagDecISubCodeInterface implementation *********************/

static CatS l_s_txt_this = CAT_S_DEF("this");

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeFieldGetPrivate *priv = JAG_DEC_SUB_CODE_FIELD_GET_GET_PRIVATE(self);

	JagAstIExpression *accessExpr = NULL;
	if (priv->is_non_static) {
		JagDecIConsumer *source_consumer = jag_dec_isub_code_get_input_consumer_at(self, 0);
		JagAstIExpression *expression = jag_dec_iconsumer_get_expression_ref(source_consumer, NULL);
		if (JAG_AST_IS_IDENTIFIER(expression)) {
			if (cat_string_wo_equal(CAT_S(l_s_txt_this), jag_ast_identifier_get_text((JagAstIdentifier *) expression))) {
				accessExpr = cat_ref_ptr(priv->field_name);
			}
		}
		if (accessExpr == NULL) {
			accessExpr = (JagAstIExpression *) jag_ast_field_access_expression_new(expression, priv->field_name);
		}
		cat_unref_ptr(expression);
	} else {
		JagAstIdentifier *resolvedName = jag_dec_model_stage_two_runtime_create_plain_name(modelRuntime, priv->field_source, NULL);
		accessExpr = (JagAstIExpression *) jag_ast_field_access_expression_new((JagAstIExpression *) resolvedName, priv->field_name);
		cat_unref_ptr(resolvedName);
	}

	JagDecIValueProducer *value_producer = jag_dec_isub_code_get_output_producer(self);
	jag_dec_ivalue_producer_set_expression(value_producer, accessExpr);
	cat_unref(accessExpr);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
