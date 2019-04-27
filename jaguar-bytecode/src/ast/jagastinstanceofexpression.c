/*
   File:    jagastinstanceofexpression.c
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

#include "jagastinstanceofexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstInstanceOfExpression"
#include <logging/catlog.h>

struct _JagAstInstanceOfExpressionPrivate {
	JagAstDeclarationType *cast_type;
	JagAstIExpression *expression;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstInstanceOfExpression, jag_ast_instance_of_expression, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstInstanceOfExpression)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_instance_of_expression_class_init(JagAstInstanceOfExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_instance_of_expression_init(JagAstInstanceOfExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstInstanceOfExpression *instance = JAG_AST_INSTANCE_OF_EXPRESSION(object);
	JagAstInstanceOfExpressionPrivate *priv = jag_ast_instance_of_expression_get_instance_private(instance);
	cat_unref_ptr(priv->expression);
	cat_unref_ptr(priv->cast_type);
	G_OBJECT_CLASS(jag_ast_instance_of_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_instance_of_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstInstanceOfExpression *jag_ast_instance_of_expression_new(JagAstDeclarationType *cast_type, JagAstIExpression *expression) {
	JagAstInstanceOfExpression *result = g_object_new(JAG_AST_TYPE_INSTANCE_OF_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstInstanceOfExpressionPrivate *priv = jag_ast_instance_of_expression_get_instance_private(result);
	priv->cast_type = cat_ref_ptr(cast_type);
	priv->expression = cat_ref_ptr(expression);
	return result;
}


/********************* start JagAstIExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstInstanceOfExpression *instance = JAG_AST_INSTANCE_OF_EXPRESSION(self);
	JagAstInstanceOfExpressionPrivate *priv = jag_ast_instance_of_expression_get_instance_private(instance);

	jag_ast_iexpression_write(priv->expression, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(" instanceof "));
	jag_ast_declaration_type_write(priv->cast_type, out);
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}

/********************* end JagAstIExpression implementation *********************/
