/*
   File:    jagastfieldaccessexpression.c
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

#include "jagastfieldaccessexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstFieldAccessExpression"
#include <logging/catlog.h>

struct _JagAstFieldAccessExpressionPrivate {
	JagAstIExpression *instance_expression;
	JagAstIdentifier *field_name;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstFieldAccessExpression, jag_ast_field_access_expression, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstFieldAccessExpression)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_field_access_expression_class_init(JagAstFieldAccessExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_field_access_expression_init(JagAstFieldAccessExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstFieldAccessExpression *instance = JAG_AST_FIELD_ACCESS_EXPRESSION(object);
	JagAstFieldAccessExpressionPrivate *priv = jag_ast_field_access_expression_get_instance_private(instance);
	cat_unref_ptr(priv->instance_expression);
	cat_unref_ptr(priv->field_name);
	G_OBJECT_CLASS(jag_ast_field_access_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_field_access_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstFieldAccessExpression *jag_ast_field_access_expression_new(JagAstIExpression *instance_expression, JagAstIdentifier *field_name) {
	JagAstFieldAccessExpression *result = g_object_new(JAG_AST_TYPE_FIELD_ACCESS_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstFieldAccessExpressionPrivate *priv = jag_ast_field_access_expression_get_instance_private(result);
	priv->instance_expression = cat_ref_ptr(instance_expression);
	priv->field_name = cat_ref_ptr(field_name);
	return result;
}

/********************* start JagAstIExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstFieldAccessExpression *instance = JAG_AST_FIELD_ACCESS_EXPRESSION(self);
	JagAstFieldAccessExpressionPrivate *priv = jag_ast_field_access_expression_get_instance_private(instance);
	if (priv->instance_expression) {
		jag_ast_iexpression_write(priv->instance_expression, out);
		jag_ast_writer_print(out, cat_string_wo_new_with("."));
	}
	jag_ast_iexpression_write((JagAstIExpression *) priv->field_name, out);
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}

/********************* end JagAstIExpression implementation *********************/
