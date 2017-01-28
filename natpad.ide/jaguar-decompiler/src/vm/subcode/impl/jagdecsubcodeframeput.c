/*
   File:    jagdecsubcodeframeput.c
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

#include "jagdecsubcodeframeput.h"
#include "../jagdecisubcode.h"
#include "../../intermediate/jagdecipostimport.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeFramePut"
#include <logging/catlog.h>

struct _JagDecSubCodeFramePutPrivate {
	gboolean is_declarator;
	JagDecMethodFrameValue *frame_value;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);
static void l_post_import_iface_init(JagDecIPostImportInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeFramePut, jag_dec_sub_code_frame_put, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IPOST_IMPORT, l_post_import_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_frame_put_class_init(JagDecSubCodeFramePutClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeFramePutPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_frame_put_init(JagDecSubCodeFramePut *instance) {
	JagDecSubCodeFramePutPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_FRAME_PUT, JagDecSubCodeFramePutPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeFramePut *instance = JAG_DEC_SUB_CODE_FRAME_PUT(object);
	JagDecSubCodeFramePutPrivate *priv = instance->priv;
	cat_unref_ptr(priv->frame_value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeFramePut *jag_dec_sub_code_frame_put_new(int mnemonic_index, JagDecMethodFrameValue *frame_value, gboolean is_declarator) {
	JagDecSubCodeFramePut *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_FRAME_PUT, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeFramePutPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->frame_value = cat_ref_ptr(frame_value);
	priv->is_declarator = is_declarator;
	return result;
}





/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeFramePutPrivate *priv = JAG_DEC_SUB_CODE_FRAME_PUT_GET_PRIVATE(self);
	JagAstIExpression *lvalue = jag_dec_method_frame_value_get_expression_ref(priv->frame_value, NULL);
	JagAstDeclarationType *bestType = jag_dec_value_type_definition_list_get_best_type_ref((JagDecValueTypeDefinitionList *) priv->frame_value);
	cat_log_debug("bestType=%p", bestType);
	JagDecIConsumer *consumer = jag_dec_isub_code_get_input_consumer_at(self, 0);
	JagAstIExpression *rvalue = jag_dec_iconsumer_get_expression_ref(consumer, bestType);
	JagAstIdentifier *dummy_throw_id = jag_ast_identifier_new(cat_string_wo_new_with("/* throw-expression */"));
	if (!jag_ast_iexpression_equal(rvalue, (JagAstIExpression *) dummy_throw_id)) {
		JagAstAssignment *assignment = jag_ast_assignment_new(lvalue, rvalue);
		if (priv->is_declarator) {
			JagAstDeclarationStatement *declStat = NULL;
			JagDecImportRepository *import_repo = jag_dec_model_stage_two_runtime_get_import_repository(modelRuntime);
			JagAstDeclarationType *final_type = jag_dec_import_repository_create_final_declaration_type(import_repo, bestType);
			declStat = jag_ast_declaration_statement_new(final_type, assignment);
			cat_unref_ptr(final_type);
			jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) declStat);
			cat_unref_ptr(declStat);
		} else {
			jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) assignment);
		}
		cat_unref_ptr(assignment);
	}

	cat_unref(dummy_throw_id);
	cat_unref(lvalue);
	cat_unref(rvalue);
	cat_unref(bestType);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/


/********************* start JagDecIPostImport implementation *********************/

static void l_post_import_add_import(JagDecIPostImport *self, struct _JagDecImportRepository *import_repository) {
	JagDecSubCodeFramePutPrivate *priv = JAG_DEC_SUB_CODE_FRAME_PUT_GET_PRIVATE(self);
	JagAstDeclarationType *bestType = jag_dec_value_type_definition_list_get_best_type_ref((JagDecValueTypeDefinitionList *) priv->frame_value);
	jag_dec_import_repository_attach_declaration_type(import_repository, bestType, TRUE);
	cat_unref_ptr(bestType);
}


static void l_post_import_iface_init(JagDecIPostImportInterface *iface) {
	iface->addImport = l_post_import_add_import;
}

/********************* end JagDecIPostImport implementation *********************/
