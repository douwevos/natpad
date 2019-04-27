/*
   File:    jagastunaryexpression.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 29, 2012
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

#include "jagastunaryexpression.h"
#include "jagasticonditionalexpression.h"
#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstUnaryExpression"
#include <logging/catlog.h>

struct _JagAstUnaryExpressionPrivate {
	JagAstIConditionalExpression *expression;
};

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface);
static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstUnaryExpression, jag_ast_unary_expression, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstUnaryExpression)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ICONDITIONAL_EXPRESSION, l_conditional_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_unary_expression_class_init(JagAstUnaryExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_unary_expression_init(JagAstUnaryExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstUnaryExpression *instance = JAG_AST_UNARY_EXPRESSION(object);
	JagAstUnaryExpressionPrivate *priv = jag_ast_unary_expression_get_instance_private(instance);
	cat_unref_ptr(priv->expression);
	G_OBJECT_CLASS(jag_ast_unary_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_unary_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstUnaryExpression *jag_ast_unary_expression_new(JagAstIConditionalExpression *expression) {
	JagAstUnaryExpression *result = g_object_new(JAG_AST_TYPE_UNARY_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstUnaryExpressionPrivate *priv = jag_ast_unary_expression_get_instance_private(result);
	priv->expression = cat_ref_ptr(expression);
	return result;
}

JagAstIConditionalExpression *jag_ast_unary_expression_create(JagAstIConditionalExpression *expression) {
	if (JAG_AST_IS_UNARY_EXPRESSION(expression)) {
		JagAstUnaryExpression *instance = JAG_AST_UNARY_EXPRESSION(expression);
		JagAstUnaryExpressionPrivate *priv = jag_ast_unary_expression_get_instance_private(instance);
		return cat_ref_ptr(priv->expression);
	}
	return (JagAstIConditionalExpression *) jag_ast_unary_expression_new(expression);
}


/********************* start JagAstIConditionalExpression implementation *********************/

static JagAstIConditionalExpression *l_conditional_expression_invert(JagAstIConditionalExpression *self) {
	JagAstUnaryExpression *instance = JAG_AST_UNARY_EXPRESSION(self);
	JagAstUnaryExpressionPrivate *priv = jag_ast_unary_expression_get_instance_private(instance);
	return cat_ref_ptr(priv->expression);
}


static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface) {
	iface->invert = l_conditional_expression_invert;
}

static CatS l_s_txt_un_start = CAT_S_DEF("!(");
static CatS l_s_txt_un_end = CAT_S_DEF(")");

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstUnaryExpression *instance = JAG_AST_UNARY_EXPRESSION(self);
	JagAstUnaryExpressionPrivate *priv = jag_ast_unary_expression_get_instance_private(instance);
	jag_ast_writer_print(out, CAT_S(l_s_txt_un_start));
	jag_ast_iexpression_write(JAG_AST_IEXPRESSION(priv->expression), out);
	jag_ast_writer_print(out, CAT_S(l_s_txt_un_end));
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}

/********************* end JagAstIConditionalExpression implementation *********************/
