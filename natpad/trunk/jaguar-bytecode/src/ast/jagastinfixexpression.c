/*
   File:    jagastinfixexpression.c
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

#include "jagastinfixexpression.h"
#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstInfixExpression"
#include <logging/catlog.h>

struct _JagAstInfixExpressionPrivate {
	JagBytMathOperator infix_operator;
	CatArrayWo *e_expressions;
	CatStringWo *a_infix_operator_text;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstInfixExpression, jag_ast_infix_expression, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_infix_expression_class_init(JagAstInfixExpressionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstInfixExpressionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_infix_expression_init(JagAstInfixExpression *instance) {
	JagAstInfixExpressionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_INFIX_EXPRESSION, JagAstInfixExpressionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstInfixExpression *instance = JAG_AST_INFIX_EXPRESSION(object);
	JagAstInfixExpressionPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_expressions);
	cat_unref_ptr(priv->a_infix_operator_text);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstInfixExpression *jag_ast_infix_expression_new(JagBytMathOperator infix_operator, CatArrayWo *e_expressions) {
	JagAstInfixExpression *result = g_object_new(JAG_AST_TYPE_INFIX_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstInfixExpressionPrivate *priv = result->priv;
	priv->infix_operator = infix_operator;
	priv->e_expressions = cat_ref_ptr(e_expressions);
	priv->a_infix_operator_text = cat_string_wo_anchor(cat_string_wo_new_with(jag_byt_math_operator_to_text(infix_operator)), 0);
	return result;
}







/********************* start JagAstIExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstInfixExpressionPrivate *priv = JAG_AST_INFIX_EXPRESSION_GET_PRIVATE(self);


	gboolean isFirst = TRUE;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_expressions);
	while(cat_iiterator_has_next(iter)) {
		JagAstIExpression *expression = (JagAstIExpression *) cat_iiterator_next(iter);
		if (isFirst) {
			isFirst = FALSE;
		} else {
			jag_ast_writer_print(out, priv->a_infix_operator_text);
		}
		jag_ast_iexpression_write(expression, out);
	}
	cat_unref_ptr(iter);
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}

/********************* end JagAstIExpression implementation *********************/
