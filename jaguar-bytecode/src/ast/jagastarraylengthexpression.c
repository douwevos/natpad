/*
   File:    jagastarraylengthexpression.c
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

#include "jagastarraylengthexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstArrayLengthExpression"
#include <logging/catlog.h>

struct _JagAstArrayLengthExpressionPrivate {
	JagAstIExpression *main_expression;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstArrayLengthExpression, jag_ast_array_length_expression, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstArrayLengthExpression)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_array_length_expression_class_init(JagAstArrayLengthExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_array_length_expression_init(JagAstArrayLengthExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstArrayLengthExpression *instance = JAG_AST_ARRAY_LENGTH_EXPRESSION(object);
	JagAstArrayLengthExpressionPrivate *priv = jag_ast_array_length_expression_get_instance_private(instance);
	cat_unref_ptr(priv->main_expression);
	G_OBJECT_CLASS(jag_ast_array_length_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_array_length_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstArrayLengthExpression *jag_ast_array_length_expression_new(JagAstIExpression *main_expression) {
	JagAstArrayLengthExpression *result = g_object_new(JAG_AST_TYPE_ARRAY_LENGTH_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstArrayLengthExpressionPrivate *priv = jag_ast_array_length_expression_get_instance_private(result);
	priv->main_expression = cat_ref_ptr(main_expression);
	return result;
}


/********************* start JagAstIExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstArrayLengthExpression *instance = JAG_AST_ARRAY_LENGTH_EXPRESSION(self);
	JagAstArrayLengthExpressionPrivate *priv = jag_ast_array_length_expression_get_instance_private(instance);

	jag_ast_iexpression_write(priv->main_expression, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(".length"));
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}

/********************* end JagAstIExpression implementation *********************/
