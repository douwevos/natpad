/*
   File:    jagastifthenelse.c
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

#include "jagastifthenelse.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstIfThenElse"
#include <logging/catlog.h>

struct _JagAstIfThenElsePrivate {
	JagAstIExpression *expression;
	JagAstBlock *then_block;
	JagAstBlock *else_block;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstIfThenElse, jag_ast_if_then_else, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_if_then_else_class_init(JagAstIfThenElseClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstIfThenElsePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_if_then_else_init(JagAstIfThenElse *instance) {
	JagAstIfThenElsePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_IF_THEN_ELSE, JagAstIfThenElsePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstIfThenElse *instance = JAG_AST_IF_THEN_ELSE(object);
	JagAstIfThenElsePrivate *priv = instance->priv;
	cat_unref_ptr(priv->expression);
	cat_unref_ptr(priv->then_block);
	cat_unref_ptr(priv->else_block);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstIfThenElse *jag_ast_if_then_else_new(JagAstIExpression *expression, JagAstBlock *then_block, JagAstBlock *else_block) {
	JagAstIfThenElse *result = g_object_new(JAG_AST_TYPE_IF_THEN_ELSE, NULL);
	cat_ref_anounce(result);
	JagAstIfThenElsePrivate *priv = result->priv;
	priv->expression = cat_ref_ptr(expression);
	priv->then_block = cat_ref_ptr(then_block);
	priv->else_block = cat_ref_ptr(else_block);
	priv->statement_line_nr = -1;
	return result;
}








/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstIfThenElsePrivate *priv = JAG_AST_IF_THEN_ELSE_GET_PRIVATE(self);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstIfThenElsePrivate *priv = JAG_AST_IF_THEN_ELSE_GET_PRIVATE(self);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_writer_print(out, cat_string_wo_new_with("if ("));
	jag_ast_iexpression_write(priv->expression, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(") "));
	jag_ast_istatement_write_statement((JagAstIStatement *) priv->then_block, out);
	if (priv->else_block!=NULL) {
		jag_ast_writer_print(out, cat_string_wo_new_with(" else "));
		jag_ast_istatement_write_statement((JagAstIStatement *) priv->else_block, out);
	}
	jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
