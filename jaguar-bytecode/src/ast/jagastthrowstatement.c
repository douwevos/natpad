/*
   File:    jagastthrowstatement.c
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

#include "jagastthrowstatement.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstThrowStatement"
#include <logging/catlog.h>

struct _JagAstThrowStatementPrivate {
	JagAstIExpression *throw_expression;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstThrowStatement, jag_ast_throw_statement, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_throw_statement_class_init(JagAstThrowStatementClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstThrowStatementPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_throw_statement_init(JagAstThrowStatement *instance) {
	JagAstThrowStatementPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_THROW_STATEMENT, JagAstThrowStatementPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstThrowStatement *instance = JAG_AST_THROW_STATEMENT(object);
	JagAstThrowStatementPrivate *priv = instance->priv;
	cat_unref_ptr(priv->throw_expression);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstThrowStatement *jag_ast_throw_statement_new(JagAstIExpression *throw_expression) {
	JagAstThrowStatement *result = g_object_new(JAG_AST_TYPE_THROW_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstThrowStatementPrivate *priv = result->priv;
	priv->throw_expression = cat_ref_ptr(throw_expression);
	priv->statement_line_nr = -1;
	return result;
}







/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstThrowStatementPrivate *priv = JAG_AST_THROW_STATEMENT_GET_PRIVATE(self);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstThrowStatementPrivate *priv = JAG_AST_THROW_STATEMENT_GET_PRIVATE(self);
	if (priv->throw_expression==NULL) {
		return;
	}
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_writer_print(out, cat_string_wo_new_with("throw "));
	jag_ast_iexpression_write(priv->throw_expression, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(";\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/


