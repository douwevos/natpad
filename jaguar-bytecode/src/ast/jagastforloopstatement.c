/*
   File:    jagastforloopstatement.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Dec 24, 2012
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

#include "jagastforloopstatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstForLoopStatement"
#include <logging/catlog.h>

struct _JagAstForLoopStatementPrivate {
	JagAstIAstNode *forInit;
	JagAstIExpression *forCondition;
	JagAstIAstNode *forUpdate;
	JagAstBlock *body;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstForLoopStatement, jag_ast_for_loop_statement, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstForLoopStatement)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_for_loop_statement_class_init(JagAstForLoopStatementClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_for_loop_statement_init(JagAstForLoopStatement *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstForLoopStatement *instance = JAG_AST_FOR_LOOP_STATEMENT(object);
	JagAstForLoopStatementPrivate *priv = jag_ast_for_loop_statement_get_instance_private(instance);
	cat_unref_ptr(priv->forInit);
	cat_unref_ptr(priv->forCondition);
	cat_unref_ptr(priv->forUpdate);
	cat_unref_ptr(priv->body);
	G_OBJECT_CLASS(jag_ast_for_loop_statement_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_for_loop_statement_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstForLoopStatement *jag_ast_for_loop_statement_new(JagAstIAstNode *forInit, JagAstIExpression *forCondition, JagAstIAstNode *forUpdate, JagAstBlock *body) {
	JagAstForLoopStatement *result = g_object_new(JAG_AST_TYPE_FOR_LOOP_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstForLoopStatementPrivate *priv = jag_ast_for_loop_statement_get_instance_private(result);
	priv->forInit = cat_ref_ptr(forInit);
	priv->forCondition = cat_ref_ptr(forCondition);
	priv->forUpdate = cat_ref_ptr(forUpdate);
	priv->body = cat_ref_ptr(body);
	priv->statement_line_nr = -1;
	return result;
}


/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstForLoopStatement *instance = JAG_AST_FOR_LOOP_STATEMENT(self);
	JagAstForLoopStatementPrivate *priv = jag_ast_for_loop_statement_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstForLoopStatement *instance = JAG_AST_FOR_LOOP_STATEMENT(self);
	JagAstForLoopStatementPrivate *priv = jag_ast_for_loop_statement_get_instance_private(instance);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);

	jag_ast_writer_print(out, cat_string_wo_new_with("for("));
	jag_ast_iast_node_write(priv->forInit, out);
	jag_ast_writer_print(out, cat_string_wo_new_with("; "));
	jag_ast_iexpression_write(priv->forCondition, out);
	jag_ast_writer_print(out, cat_string_wo_new_with("; "));
	jag_ast_iast_node_write(priv->forUpdate, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(") "));
	jag_ast_istatement_write_statement((JagAstIStatement *) priv->body, out);
	jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
