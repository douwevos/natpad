/*
   File:    jagdecsubcodefinally.c
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

#include "jagdecsubcodefinally.h"
#include "../jagdecisubcode.h"
#include "../jagdecivalueproducer.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeFinally"
#include <logging/catlog.h>

struct _JagDecSubCodeFinallyPrivate {
	JagDecISubCode *block;
	JagAstFinallyBlock *finally_block;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeFinally, jag_dec_sub_code_finally, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_finally_class_init(JagDecSubCodeFinallyClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeFinallyPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_finally_init(JagDecSubCodeFinally *instance) {
	JagDecSubCodeFinallyPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_FINALLY, JagDecSubCodeFinallyPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeFinally *instance = JAG_DEC_SUB_CODE_FINALLY(object);
	JagDecSubCodeFinallyPrivate *priv = instance->priv;
	cat_unref_ptr(priv->block);
	cat_unref_ptr(priv->finally_block);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeFinally *jag_dec_sub_code_finally_new(int mnemonic_index, JagDecISubCode *block) {
	JagDecSubCodeFinally *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_FINALLY, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeFinallyPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->block = cat_ref_ptr(block);
	priv->finally_block = NULL;
	return result;
}



JagAstFinallyBlock *jag_dec_sub_code_finally_get_finally_block(JagDecSubCodeFinally *sub_code_finally) {
	JagDecSubCodeFinallyPrivate *priv = JAG_DEC_SUB_CODE_FINALLY_GET_PRIVATE(sub_code_finally);
	return priv->finally_block;
}



/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeFinallyPrivate *priv = JAG_DEC_SUB_CODE_FINALLY_GET_PRIVATE(self);

	cat_unref(priv->finally_block);
	priv->finally_block = jag_ast_finally_block_new();

	JagDecModelStageTwoRuntime *branchedRuntime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, (JagAstBlock *) priv->finally_block);
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
