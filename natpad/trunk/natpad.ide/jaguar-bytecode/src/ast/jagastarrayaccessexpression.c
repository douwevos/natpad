/*
   File:    jagastarrayaccessexpression.c
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



#include "jagastarrayaccessexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstArrayAccessExpression"
#include <logging/catlog.h>

struct _JagAstArrayAccessExpressionPrivate {
	JagAstIExpression *main_expression;
	JagAstIExpression *dim_expression;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstArrayAccessExpression, jag_ast_array_access_expression, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_array_access_expression_class_init(JagAstArrayAccessExpressionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstArrayAccessExpressionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_array_access_expression_init(JagAstArrayAccessExpression *instance) {
	JagAstArrayAccessExpressionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_ARRAY_ACCESS_EXPRESSION, JagAstArrayAccessExpressionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstArrayAccessExpression *instance = JAG_AST_ARRAY_ACCESS_EXPRESSION(object);
	JagAstArrayAccessExpressionPrivate *priv = instance->priv;
	cat_unref_ptr(priv->main_expression);
	cat_unref_ptr(priv->dim_expression);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstArrayAccessExpression *jag_ast_array_access_expression_new(JagAstIExpression *main_expression, JagAstIExpression *dim_expression) {
	JagAstArrayAccessExpression *result = g_object_new(JAG_AST_TYPE_ARRAY_ACCESS_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstArrayAccessExpressionPrivate *priv = result->priv;
	priv->main_expression = cat_ref_ptr(main_expression);
	priv->dim_expression = cat_ref_ptr(dim_expression);
	return result;
}







/********************* start JagAstIExpression implementation *********************/


static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstArrayAccessExpressionPrivate *priv = JAG_AST_ARRAY_ACCESS_EXPRESSION_GET_PRIVATE(self);
	jag_ast_iexpression_write(priv->main_expression, out);
	jag_ast_writer_print(out, cat_string_wo_new_with("["));
	jag_ast_iexpression_write(priv->dim_expression, out);
	jag_ast_writer_print(out, cat_string_wo_new_with("]"));
}

static gboolean l_expression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (self==other) {
		return TRUE;
	}
	if (self==NULL || other==NULL) {
		return FALSE;
	}

	gboolean result = FALSE;
	if (JAG_AST_IS_ARRAY_ACCESS_EXPRESSION(other)) {
		JagAstArrayAccessExpressionPrivate *priv = JAG_AST_ARRAY_ACCESS_EXPRESSION_GET_PRIVATE(self);
		JagAstArrayAccessExpressionPrivate *o_priv = JAG_AST_ARRAY_ACCESS_EXPRESSION_GET_PRIVATE(other);
		if (jag_ast_iexpression_equal(priv->dim_expression, o_priv->dim_expression) &&
				jag_ast_iexpression_equal(priv->main_expression, o_priv->main_expression)) {
			result = TRUE;
		}
	}
	return result;
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
	iface->equal = l_expression_equal;
}


/********************* end JagAstIExpression implementation *********************/
