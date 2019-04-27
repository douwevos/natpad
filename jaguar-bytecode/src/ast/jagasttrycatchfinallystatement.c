/*
   File:    jagasttrycatchfinallystatement.c
   Project: jaguar-bytecode
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

#include "jagasttrycatchfinallystatement.h"
#include "jagastistatement.h"
#include "jagastcatchblock.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstTryCatchFinallyStatement"
#include <logging/catlog.h>

struct _JagAstTryCatchFinallyStatementPrivate {
	JagAstBlock *try_block;
	CatArrayWo *e_catch_blocks;
	JagAstFinallyBlock *finally_block;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstTryCatchFinallyStatement, jag_ast_try_catch_finally_statement, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstTryCatchFinallyStatement)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_try_catch_finally_statement_class_init(JagAstTryCatchFinallyStatementClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_try_catch_finally_statement_init(JagAstTryCatchFinallyStatement *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstTryCatchFinallyStatement *instance = JAG_AST_TRY_CATCH_FINALLY_STATEMENT(object);
	JagAstTryCatchFinallyStatementPrivate *priv = jag_ast_try_catch_finally_statement_get_instance_private(instance);
	cat_unref_ptr(priv->try_block);
	cat_unref_ptr(priv->e_catch_blocks);
	cat_unref_ptr(priv->finally_block);
	G_OBJECT_CLASS(jag_ast_try_catch_finally_statement_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_try_catch_finally_statement_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstTryCatchFinallyStatement *jag_ast_try_catch_finally_statement_new(JagAstBlock *try_block, CatArrayWo *e_catch_blocks, JagAstFinallyBlock *finally_block) {
	JagAstTryCatchFinallyStatement *result = g_object_new(JAG_AST_TYPE_TRY_CATCH_FINALLY_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstTryCatchFinallyStatementPrivate *priv = jag_ast_try_catch_finally_statement_get_instance_private(result);
	priv->try_block = cat_ref_ptr(try_block);
	priv->e_catch_blocks = cat_ref_ptr(e_catch_blocks);
	priv->finally_block = cat_ref_ptr(finally_block);
	priv->statement_line_nr = -1;
	return result;
}


/********************* start JagAstIStatementInterface implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstTryCatchFinallyStatement *instance = JAG_AST_TRY_CATCH_FINALLY_STATEMENT(self);
	JagAstTryCatchFinallyStatementPrivate *priv = jag_ast_try_catch_finally_statement_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstTryCatchFinallyStatement *instance = JAG_AST_TRY_CATCH_FINALLY_STATEMENT(self);
	JagAstTryCatchFinallyStatementPrivate *priv = jag_ast_try_catch_finally_statement_get_instance_private(instance);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_writer_print(out, cat_string_wo_new_with("try "));
	jag_ast_istatement_write_statement((JagAstIStatement *) priv->try_block, out);
	if (priv->e_catch_blocks!=NULL && cat_array_wo_size(priv->e_catch_blocks)>0) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_catch_blocks);
		while(cat_iiterator_has_next(iter)) {
			JagAstCatchBlock *catch_block = (JagAstCatchBlock *) cat_iiterator_next(iter);
			jag_ast_istatement_write_statement((JagAstIStatement *) catch_block, out);
		}
		cat_unref_ptr(iter);
	}
	if (priv->finally_block) {
		jag_ast_istatement_write_statement((JagAstIStatement *) priv->finally_block, out);
	}
	jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatementInterface implementation *********************/
