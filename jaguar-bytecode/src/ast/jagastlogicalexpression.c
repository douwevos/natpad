/*
   File:    jagastlogicalexpression.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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

#include "jagastlogicalexpression.h"
#include "jagasticonditionalexpression.h"
#include "jagastwrappedconditionalexpression.h"
#include "jagastunaryexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstLogicalExpression"
#include <logging/catlog.h>

struct _JagAstLogicalExpressionPrivate {
	JagBytMathOperator logical_operator;
	CatArrayWo *e_expressions;
};

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface);
static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstLogicalExpression, jag_ast_logical_expression, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstLogicalExpression)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ICONDITIONAL_EXPRESSION, l_conditional_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_logical_expression_class_init(JagAstLogicalExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_logical_expression_init(JagAstLogicalExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstLogicalExpression *instance = JAG_AST_LOGICAL_EXPRESSION(object);
	JagAstLogicalExpressionPrivate *priv = jag_ast_logical_expression_get_instance_private(instance);
	cat_unref_ptr(priv->e_expressions);
	G_OBJECT_CLASS(jag_ast_logical_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_logical_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstLogicalExpression *jag_ast_logical_expression_new(JagBytMathOperator logical_operator, CatArrayWo *e_expressions) {
	JagAstLogicalExpression *result = g_object_new(JAG_AST_TYPE_LOGICAL_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstLogicalExpressionPrivate *priv = jag_ast_logical_expression_get_instance_private(result);
	priv->logical_operator = logical_operator;
	priv->e_expressions = cat_ref_ptr(e_expressions);
	return result;
}


/********************* start JagAstIConditionalExpression implementation *********************/

static JagAstIConditionalExpression *l_conditional_expression_invert(JagAstIConditionalExpression *self) {
	JagAstLogicalExpression *instance = JAG_AST_LOGICAL_EXPRESSION(self);
	JagAstLogicalExpressionPrivate *priv = jag_ast_logical_expression_get_instance_private(instance);
	JagAstIConditionalExpression *result = NULL;
	if ((priv->logical_operator==JAG_BYT_MATH_OPERATOR_LOGICAL_AND) ||
			(priv->logical_operator==JAG_BYT_MATH_OPERATOR_LOGICAL_OR)) {
		CatArrayWo *e_new_expressions = cat_array_wo_new();
		CatIIterator *iter = cat_array_wo_iterator(priv->e_expressions);
		while(cat_iiterator_has_next(iter)) {
			JagAstIExpression *orig_expr = (JagAstIExpression *) cat_iiterator_next(iter);
			JagAstIConditionalExpression *cond_expr = jag_ast_wrapped_conditional_expression_ensure_is_conditional(orig_expr);
			JagAstIConditionalExpression *inverted_expr = jag_ast_iconditional_expression_invert(cond_expr);
			cat_unref_ptr(cond_expr);
			cat_array_wo_append(e_new_expressions, (GObject *) inverted_expr);
			cat_unref_ptr(inverted_expr);
		}
		cat_unref_ptr(iter);


		JagBytMathOperator new_op;
		if (priv->logical_operator==JAG_BYT_MATH_OPERATOR_LOGICAL_AND) {
			new_op = JAG_BYT_MATH_OPERATOR_LOGICAL_OR;
		} else /*if (priv->logical_operator==JAG_BYT_MATH_OPERATOR_LOGICAL_OR) */ {
			new_op = JAG_BYT_MATH_OPERATOR_LOGICAL_AND;
		}
		result = (JagAstIConditionalExpression *) jag_ast_logical_expression_new(new_op, e_new_expressions);
		cat_unref_ptr(e_new_expressions);
	} else {
		result = jag_ast_unary_expression_create(self);
	}

	return result;
}

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface) {
	iface->invert = l_conditional_expression_invert;
}


static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstLogicalExpression *instance = JAG_AST_LOGICAL_EXPRESSION(self);
	JagAstLogicalExpressionPrivate *priv = jag_ast_logical_expression_get_instance_private(instance);
	CatStringWo *e_op_text = cat_string_wo_new_with(" ");
	cat_string_wo_append_chars(e_op_text, jag_byt_math_operator_to_text(priv->logical_operator));
	cat_string_wo_append_unichar(e_op_text, ' ');
	int idx;
	for(idx=0; idx<cat_array_wo_size(priv->e_expressions); idx++) {
		if (idx!=0) {
			jag_ast_writer_print(out, e_op_text);
		}
		JagAstIExpression *expression = (JagAstIExpression *) cat_array_wo_get(priv->e_expressions, idx);
		jag_ast_iexpression_write(expression, out);
	}
	cat_unref_ptr(e_op_text);
}

static gboolean l_expression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (JAG_AST_IS_LOGICAL_EXPRESSION(other)) {
		JagAstLogicalExpressionPrivate *priv = jag_ast_logical_expression_get_instance_private(JAG_AST_LOGICAL_EXPRESSION(self));
		JagAstLogicalExpressionPrivate *o_priv = jag_ast_logical_expression_get_instance_private(JAG_AST_LOGICAL_EXPRESSION(other));
		return (priv->logical_operator==o_priv->logical_operator) &&
				cat_array_wo_equal(priv->e_expressions, o_priv->e_expressions, (GEqualFunc) jag_ast_iexpression_equal);

	}
	return FALSE;
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
	iface->equal = l_expression_equal;
}

/********************* end JagAstIConditionalExpression implementation *********************/
