/*
   File:    jagdecsubcodetrycatchfinal.c
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

#include "jagdecsubcodetrycatchfinal.h"
#include "jagdecsubcodecatch.h"
#include "../jagdecisubcode.h"
#include "../../jagdecmodelstagetworuntime.h"
#include <jaguarbytecode.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecSubCodeTryCatchFinal"
#include <logging/catlog.h>

struct _JagDecSubCodeTryCatchFinalPrivate {
	JagDecSubCodeBlock *sub_code_try;
	JagDecSubCodeFinally *sub_code_finally;
	CatArrayWo *e_catch_sub_codes;
};

static void l_sub_code_iface_init(JagDecISubCodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecSubCodeTryCatchFinal, jag_dec_sub_code_try_catch_final, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ISUB_CODE, l_sub_code_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_sub_code_try_catch_final_class_init(JagDecSubCodeTryCatchFinalClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecSubCodeTryCatchFinalPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_sub_code_try_catch_final_init(JagDecSubCodeTryCatchFinal *instance) {
	JagDecSubCodeTryCatchFinalPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL, JagDecSubCodeTryCatchFinalPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecSubCodeTryCatchFinal *instance = JAG_DEC_SUB_CODE_TRY_CATCH_FINAL(object);
	JagDecSubCodeTryCatchFinalPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_catch_sub_codes);
	cat_unref_ptr(priv->sub_code_try);
	cat_unref_ptr(priv->sub_code_finally);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecSubCodeTryCatchFinal *jag_dec_sub_code_try_catch_final_new(JagDecSubCodeBlock *sub_code_try, JagDecSubCodeFinally *sub_code_finally, CatArrayWo *e_catch_sub_codes) {
	JagDecSubCodeTryCatchFinal *result = g_object_new(JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL, NULL);
	cat_ref_anounce(result);
	JagDecSubCodeTryCatchFinalPrivate *priv = result->priv;
	priv->e_catch_sub_codes = cat_ref_ptr(e_catch_sub_codes);
	priv->sub_code_finally = cat_ref_ptr(sub_code_finally);
	priv->sub_code_try = cat_ref_ptr(sub_code_try);
	return result;
}




/********************* start JagDecISubCodeInterface implementation *********************/

static void l_sub_code_connect_stage_two(JagDecISubCode *self, JagDecModelStageTwoRuntime *modelRuntime) {
	JagDecSubCodeTryCatchFinalPrivate *priv = JAG_DEC_SUB_CODE_TRY_CATCH_FINAL_GET_PRIVATE(self);

	JagAstBlock *tryBlock = jag_ast_block_new();
	CatArrayWo *e_catchBlocks = NULL;			/* JagAstCatchBlock */
	JagAstFinallyBlock *finalBlock = NULL;


	JagDecModelStageTwoRuntime *tryRuntime = jag_dec_model_stage_two_runtime_create_branch(modelRuntime, tryBlock);
	jag_dec_isub_code_connect_stage_two((JagDecISubCode *) priv->sub_code_try, tryRuntime);

	if (priv->e_catch_sub_codes!=NULL && cat_array_wo_size(priv->e_catch_sub_codes)>0) {
		e_catchBlocks = cat_array_wo_new();
		CatIIterator *iter = cat_array_wo_iterator(priv->e_catch_sub_codes);
		while(cat_iiterator_has_next(iter)) {
			JagDecSubCodeCatch *subCodeCatch = (JagDecSubCodeCatch *) cat_iiterator_next(iter);
			jag_dec_isub_code_connect_stage_two((JagDecISubCode *) subCodeCatch, modelRuntime);	// TODO: no runtime-branch ??
			JagAstCatchBlock *catch_block = jag_dec_sub_code_catch_get_catch_block(subCodeCatch);
			cat_array_wo_append(e_catchBlocks, (GObject *) catch_block);
		}
		cat_unref_ptr(iter);
	}

	if (priv->sub_code_finally!=NULL) {
		jag_dec_isub_code_connect_stage_two((JagDecISubCode *) priv->sub_code_finally, modelRuntime);	// TODO: no runtime-branch ??
		finalBlock = jag_dec_sub_code_finally_get_finally_block(priv->sub_code_finally);
	}


	JagAstTryCatchFinallyStatement *statement = jag_ast_try_catch_finally_statement_new(tryBlock, e_catchBlocks, finalBlock);
	jag_dec_model_stage_two_runtime_append_statement(modelRuntime, (JagAstIStatement *) statement);
	cat_unref_ptr(tryBlock);
	cat_unref_ptr(e_catchBlocks);
	cat_unref_ptr(statement);
	cat_unref_ptr(tryRuntime);
}


static struct _JagDecIConsumer *l_sub_code_get_input_consumer_at(JagDecISubCode *self, int index) {
	return NULL;
}

static struct _JagDecIValueProducer *l_sub_code_get_output_producer(JagDecISubCode *self) {
	return NULL;
}


static void l_sub_code_iface_init(JagDecISubCodeInterface *iface) {
	iface->connectStageTwo = l_sub_code_connect_stage_two;
	iface->getInputConsumerAt = l_sub_code_get_input_consumer_at;
	iface->getOutputProducer = l_sub_code_get_output_producer;
}

/********************* end JagDecISubCodeInterface implementation *********************/
