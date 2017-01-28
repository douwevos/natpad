/*
   File:    jagdecsubcodesynchronizedblock.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 19, 2012
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

#include "jagdecsubcodesynchronizedblock.h"
#include "../../jagdecmodelstagetworuntime.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeSynchronizedBlock"
#include <logging/catlog.h>

struct _JagDecSubCodeSynchronizedBlockPrivate {
	JagDecISubCode *sub_code_body;
	JagDecConsumer *consumer;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeSynchronizedBlock, jag_dec_sub_code_synchronized_block, JAG_DEC_TYPE_ABSTRACT_SUB_CODE, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_synchronized_block_class_init(JagDecSubCodeSynchronizedBlockClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeSynchronizedBlockPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_synchronized_block_init(JagDecSubCodeSynchronizedBlock *instance) {
	JagDecSubCodeSynchronizedBlockPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_SYNCHRONIZED_BLOCK, JagDecSubCodeSynchronizedBlockPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeSynchronizedBlock *instance = JAG_DEC_SUB_CODE_SYNCHRONIZED_BLOCK(object);
	JagDecSubCodeSynchronizedBlockPrivate *priv = instance->priv;
	cat_unref_ptr(priv->consumer);
	cat_unref_ptr(priv->sub_code_body);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeSynchronizedBlock *jag_dec_sub_code_synchronized_block_new(int mnemonic_index, JagDecISubCode *sub_code_body, JagDecConsumer *consumer) {
	JagDecSubCodeSynchronizedBlock *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_SYNCHRONIZED_BLOCK, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeSynchronizedBlockPrivate *priv = result->priv;
	jag_dec_abstract_sub_code_construct((JagDecAbstractSubCode *) result, mnemonic_index);
	priv->consumer = cat_ref_ptr(consumer);
	priv->sub_code_body = cat_ref_ptr(sub_code_body);
	return result;
}








/********************* start JagDecISubCodeInterface implementation *********************/


static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeSynchronizedBlockPrivate *priv = JAG_DEC_SUB_CODE_SYNCHRONIZED_BLOCK_GET_PRIVATE(self);

	JagAstIExpression *syncExpression = jag_dec_iconsumer_get_expression_ref((JagDecIConsumer *) priv->consumer, NULL);
	JagAstSynchronizedBlock *block_code = jag_ast_synchronized_block_new(syncExpression);
	cat_unref_ptr(syncExpression);
	jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) block_code);
	cat_unref(block_code);

	JagDecModelStageTwoRuntime *body_runtime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, (JagAstBlock *) block_code);
	jag_dec_isub_code_connect_stage_two(priv->sub_code_body, body_runtime);
	cat_unref_ptr(body_runtime);

}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	iface->connectStageTwo = l_sub_code_connect_stage_two;
}


/********************* end JagDecISubCodeInterface implementation *********************/
