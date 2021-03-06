/*
   File:    jagastwrappedconditionalexpression.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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

#include "jagastwrappedconditionalexpression.h"
#include "jagasticonditionalexpression.h"
#include "jagastunaryexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstWrappedConditionalExpression"
#include <logging/catlog.h>

struct _JagAstWrappedConditionalExpressionPrivate {
	JagAstIExpression *expression;
};

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface);
static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstWrappedConditionalExpression, jag_ast_wrapped_conditional_expression, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstWrappedConditionalExpression)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ICONDITIONAL_EXPRESSION, l_conditional_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_wrapped_conditional_expression_class_init(JagAstWrappedConditionalExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_wrapped_conditional_expression_init(JagAstWrappedConditionalExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstWrappedConditionalExpression *instance = JAG_AST_WRAPPED_CONDITIONAL_EXPRESSION(object);
	JagAstWrappedConditionalExpressionPrivate *priv = jag_ast_wrapped_conditional_expression_get_instance_private(instance);
	cat_unref_ptr(priv->expression)
	G_OBJECT_CLASS(jag_ast_wrapped_conditional_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_wrapped_conditional_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstWrappedConditionalExpression *jag_ast_wrapped_conditional_expression_new(JagAstIExpression *expression) {
	JagAstWrappedConditionalExpression *result = g_object_new(JAG_AST_TYPE_WRAPPED_CONDITIONAL_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstWrappedConditionalExpressionPrivate *priv = jag_ast_wrapped_conditional_expression_get_instance_private(result);
	priv->expression = cat_ref_ptr(expression);
	return result;
}

JagAstIConditionalExpression *jag_ast_wrapped_conditional_expression_ensure_is_conditional(JagAstIExpression *expression) {
	if (JAG_AST_IS_ICONDITIONAL_EXPRESSION(expression)) {
		return (JagAstIConditionalExpression *) cat_ref_ptr(expression);
	}
	return (JagAstIConditionalExpression *) jag_ast_wrapped_conditional_expression_new(expression);
}


/********************* start JagAstIConditionalExpression implementation *********************/

static JagAstIConditionalExpression *l_conditional_expression_invert(JagAstIConditionalExpression *self) {
	return (JagAstIConditionalExpression *) jag_ast_unary_expression_create((JagAstIConditionalExpression *) self);
}

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface) {
	iface->invert = l_conditional_expression_invert;
}

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstWrappedConditionalExpression *instance = JAG_AST_WRAPPED_CONDITIONAL_EXPRESSION(self);
	JagAstWrappedConditionalExpressionPrivate *priv = jag_ast_wrapped_conditional_expression_get_instance_private(instance);
	jag_ast_iexpression_write(priv->expression, out);
}

static gboolean l_expression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (JAG_AST_IS_WRAPPED_CONDITIONAL_EXPRESSION(other)) {
		JagAstWrappedConditionalExpressionPrivate *priv = jag_ast_wrapped_conditional_expression_get_instance_private(JAG_AST_WRAPPED_CONDITIONAL_EXPRESSION(self));
		JagAstWrappedConditionalExpressionPrivate *o_priv = jag_ast_wrapped_conditional_expression_get_instance_private(JAG_AST_WRAPPED_CONDITIONAL_EXPRESSION(other));
		return jag_ast_iexpression_equal(priv->expression, o_priv->expression);
	}
	return FALSE;
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
	iface->equal = l_expression_equal;
}

/********************* end JagAstIConditionalExpression implementation *********************/
