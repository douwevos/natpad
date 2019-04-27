/*
   File:    jagastwhilestatement.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 18, 2012
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

#include "jagastwhilestatement.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstWhileStatement"
#include <logging/catlog.h>

struct _JagAstWhileStatementPrivate {
	JagAstBlock *body;
	JagAstIConditionalExpression *conditional_expression;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstWhileStatement, jag_ast_while_statement, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstWhileStatement)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_while_statement_class_init(JagAstWhileStatementClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_while_statement_init(JagAstWhileStatement *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstWhileStatement *instance = JAG_AST_WHILE_STATEMENT(object);
	JagAstWhileStatementPrivate *priv = jag_ast_while_statement_get_instance_private(instance);
	cat_unref_ptr(priv->body);
	cat_unref_ptr(priv->conditional_expression);
	G_OBJECT_CLASS(jag_ast_while_statement_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_while_statement_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstWhileStatement *jag_ast_while_statement_new(JagAstBlock *body) {
	JagAstWhileStatement *result = g_object_new(JAG_AST_TYPE_WHILE_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstWhileStatementPrivate *priv = jag_ast_while_statement_get_instance_private(result);
	priv->body = cat_ref_ptr(body);
	priv->statement_line_nr = -1;
	priv->conditional_expression = NULL;
	return result;
}

void jag_ast_while_statement_set_conditional_expression(JagAstWhileStatement *while_statement, JagAstIConditionalExpression *conditional_expression) {
	JagAstWhileStatementPrivate *priv = jag_ast_while_statement_get_instance_private(while_statement);
	cat_ref_swap(priv->conditional_expression, conditional_expression);
}


/********************* start JagAstIStatementInterface implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstWhileStatement *instance = JAG_AST_WHILE_STATEMENT(self);
	JagAstWhileStatementPrivate *priv = jag_ast_while_statement_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstWhileStatement *instance = JAG_AST_WHILE_STATEMENT(self);
	JagAstWhileStatementPrivate *priv = jag_ast_while_statement_get_instance_private(instance);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);

	jag_ast_writer_print(out, cat_string_wo_new_with("while("));
	if (priv->conditional_expression) {
		jag_ast_iexpression_write((JagAstIExpression *) priv->conditional_expression, out);
	} else {
		jag_ast_writer_print(out, cat_string_wo_new_with("<missing-conditional-expression>"));
	}
	jag_ast_writer_print(out, cat_string_wo_new_with(")"));
	if (priv->body) {
		jag_ast_istatement_write_statement((JagAstIStatement *) priv->body, out);
	} else {
		jag_ast_writer_print(out, cat_string_wo_new_with("<missing-while-body>"));
	}
	jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatementInterface implementation *********************/
