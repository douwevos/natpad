/*
   File:    jagastprefixexpression.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 19, 2012
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

#include "jagastprefixexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstPrefixExpression"
#include <logging/catlog.h>

struct _JagAstPrefixExpressionPrivate {
	gboolean is_increment;
	JagAstIExpression *sub_expression;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstPrefixExpression, jag_ast_prefix_expression, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_prefix_expression_class_init(JagAstPrefixExpressionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstPrefixExpressionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_prefix_expression_init(JagAstPrefixExpression *instance) {
	JagAstPrefixExpressionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_PREFIX_EXPRESSION, JagAstPrefixExpressionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstPrefixExpression *instance = JAG_AST_PREFIX_EXPRESSION(object);
	JagAstPrefixExpressionPrivate *priv = instance->priv;
	cat_unref_ptr(priv->sub_expression);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstPrefixExpression *jag_ast_prefix_expression_new(gboolean is_increment, JagAstIExpression *sub_expression) {
	JagAstPrefixExpression *result = g_object_new(JAG_AST_TYPE_PREFIX_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstPrefixExpressionPrivate *priv = result->priv;
	priv->is_increment = is_increment;
	priv->sub_expression = cat_ref_ptr(sub_expression);
	return result;
}







/********************* start JagAstIExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstPrefixExpressionPrivate *priv = JAG_AST_PREFIX_EXPRESSION_GET_PRIVATE(self);
	if (priv->is_increment) {
		jag_ast_writer_print(out, cat_string_wo_new_with("++"));
	} else {
		jag_ast_writer_print(out, cat_string_wo_new_with("--"));
	}
	jag_ast_iexpression_write((JagAstIExpression *) priv->sub_expression, out);
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}

/********************* end JagAstIExpression implementation *********************/

