/*
   File:    jagdecsubcodecatch.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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

#include "jagdecsubcodecatch.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include "../jagdecivalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeCatch"
#include <logging/catlog.h>

struct _JagDecSubCodeCatchPrivate {
	JagAstDeclarationType *exception_type;
	JagDecISubCode *block;
	JagAstCatchBlock *catch_block;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeCatch, jag_dec_sub_code_catch, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_catch_class_init(JagDecSubCodeCatchClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeCatchPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_catch_init(JagDecSubCodeCatch *instance) {
	JagDecSubCodeCatchPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_CATCH, JagDecSubCodeCatchPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeCatch *instance = JAG_DEC_SUB_CODE_CATCH(object);
	JagDecSubCodeCatchPrivate *priv = instance->priv;
	cat_unref_ptr(priv->block);
	cat_unref_ptr(priv->catch_block);
	cat_unref_ptr(priv->exception_type);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeCatch *jag_dec_sub_code_catch_new(int mnemonic_index, JagAstDeclarationType *exception_type, JagDecISubCode *block) {
	JagDecSubCodeCatch *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_CATCH, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeCatchPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->exception_type = cat_ref_ptr(exception_type);
	priv->block = cat_ref_ptr(block);
	return result;
}



JagAstCatchBlock *jag_dec_sub_code_catch_get_catch_block(JagDecSubCodeCatch *sub_code_catch) {
	JagDecSubCodeCatchPrivate *priv = JAG_DEC_SUB_CODE_CATCH_GET_PRIVATE(sub_code_catch);
	return priv->catch_block;
}

/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeCatchPrivate *priv = JAG_DEC_SUB_CODE_CATCH_GET_PRIVATE(self);
	JagDecImportRepository *import_repository = jag_dec_model_stage_two_runtime_get_import_repository(modelRuntime);
	JagBytName *resolved_name = jag_dec_import_repository_create_resolved_typename(import_repository, jag_ast_declaration_type_get_reference_type_name(priv->exception_type));
	JagAstDeclarationType *exception_type = jag_ast_declaration_type_set_reference_type_name(priv->exception_type, resolved_name);
	JagAstModifiers *modifiers = jag_ast_modifiers_new(0);
	JagAstIdentifier *exception_id = jag_ast_identifier_new(cat_string_wo_new_with("exception"));
	JagAstFieldDeclaration *field = jag_ast_field_declaration_new(modifiers, exception_type, exception_id, NULL);
	cat_unref_ptr(exception_id);
	cat_unref(modifiers);
	cat_unref(exception_type);
	cat_unref(resolved_name);
	priv->catch_block = jag_ast_catch_block_new(field);
	cat_unref_ptr(field);
	JagDecModelStageTwoRuntime *branchedRuntime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, (JagAstBlock *) priv->catch_block);
	jag_dec_isub_code_connect_stage_two(priv->block, branchedRuntime);
	cat_unref_ptr(branchedRuntime);
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	JagDecISubCodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = p_iface->getInputConsumerAt;
	iface->getOutputProducer = p_iface->getOutputProducer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
