/*
   File:    jagastrelationalexpression.c
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

#include "jagastrelationalexpression.h"
#include "jagasticonditionalexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstRelationalExpression"
#include <logging/catlog.h>

struct _JagAstRelationalExpressionPrivate {
	JagBytConditionType condition_type;
	JagAstIExpression *left;
	JagAstIExpression *right;
};

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface);
static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstRelationalExpression, jag_ast_relational_expression, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ICONDITIONAL_EXPRESSION, l_conditional_expression_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_relational_expression_class_init(JagAstRelationalExpressionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstRelationalExpressionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_relational_expression_init(JagAstRelationalExpression *instance) {
	JagAstRelationalExpressionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_RELATIONAL_EXPRESSION, JagAstRelationalExpressionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstRelationalExpression *instance = JAG_AST_RELATIONAL_EXPRESSION(object);
	JagAstRelationalExpressionPrivate *priv = instance->priv;
	cat_unref_ptr(priv->left);
	cat_unref_ptr(priv->right);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstRelationalExpression *jag_ast_relational_expression_new(JagBytConditionType condition_type, JagAstIExpression *left, JagAstIExpression *right) {
	JagAstRelationalExpression *result = g_object_new(JAG_AST_TYPE_RELATIONAL_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstRelationalExpressionPrivate *priv = result->priv;
	priv->condition_type = condition_type;
	priv->left = cat_ref_ptr(left);
	priv->right = cat_ref_ptr(right);
	return result;
}








/********************* start JagAstIConditionalExpression implementation *********************/


static JagAstIConditionalExpression *l_conditional_expression_invert(JagAstIConditionalExpression *self) {
	JagAstRelationalExpressionPrivate *priv = JAG_AST_RELATIONAL_EXPRESSION_GET_PRIVATE(self);
	JagBytConditionType invert_cond_type = jag_byt_condition_type_opposite(priv->condition_type);
	return (JagAstIConditionalExpression *) jag_ast_relational_expression_new(invert_cond_type, priv->left, priv->right);
}

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface) {
	iface->invert = l_conditional_expression_invert;
}


static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstRelationalExpressionPrivate *priv = JAG_AST_RELATIONAL_EXPRESSION_GET_PRIVATE(self);
	jag_ast_iexpression_write(priv->left, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(jag_byt_condition_type_text(priv->condition_type)));
	jag_ast_iexpression_write(priv->right, out);
}

static gboolean l_expression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (JAG_AST_IS_RELATIONAL_EXPRESSION(other)) {
		JagAstRelationalExpressionPrivate *priv = JAG_AST_RELATIONAL_EXPRESSION_GET_PRIVATE(self);
		JagAstRelationalExpressionPrivate *o_priv = JAG_AST_RELATIONAL_EXPRESSION_GET_PRIVATE(other);
		return (priv->condition_type==o_priv->condition_type) &&
				jag_ast_iexpression_equal(priv->left, o_priv->left) &&
				jag_ast_iexpression_equal(priv->right, o_priv->right);

	}
	return FALSE;
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
	iface->equal = l_expression_equal;
}


/********************* end JagAstIConditionalExpression implementation *********************/
