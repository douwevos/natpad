/*
   File:    jagastnewexpression.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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

#include "jagastnewexpression.h"
#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstNewExpression"
#include <logging/catlog.h>

struct _JagAstNewExpressionPrivate {
	JagAstIdentifier *identifier;
	CatArrayWo *e_dim_expressions;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstNewExpression, jag_ast_new_expression, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstNewExpression)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_new_expression_class_init(JagAstNewExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_new_expression_init(JagAstNewExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstNewExpression *instance = JAG_AST_NEW_EXPRESSION(object);
	JagAstNewExpressionPrivate *priv = jag_ast_new_expression_get_instance_private(instance);
	cat_unref_ptr(priv->identifier);
	cat_unref_ptr(priv->e_dim_expressions);
	G_OBJECT_CLASS(jag_ast_new_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_new_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstNewExpression *jag_ast_new_expression_new(JagAstIdentifier *identifier, CatArrayWo *e_dim_expressions) {
	JagAstNewExpression *result = g_object_new(JAG_AST_TYPE_NEW_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstNewExpressionPrivate *priv = jag_ast_new_expression_get_instance_private(result);
	priv->identifier = cat_ref_ptr(identifier);
	priv->e_dim_expressions = cat_ref_ptr(e_dim_expressions);
	return result;
}


/********************* start JagAstIConditionalExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstNewExpression *instance = JAG_AST_NEW_EXPRESSION(self);
	JagAstNewExpressionPrivate *priv = jag_ast_new_expression_get_instance_private(instance);
	jag_ast_writer_print(out, cat_string_wo_new_with("new "));
	cat_log_trace("priv->identifier=%p", priv->identifier);
	cat_log_trace("priv->identifier=%s", g_type_name_from_instance((GTypeInstance *) priv->identifier));
	jag_ast_iexpression_write((JagAstIExpression *) priv->identifier, out);
	if (priv->e_dim_expressions) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_dim_expressions);
		while(cat_iiterator_has_next(iter)) {
			JagAstIExpression *dim_expr = (JagAstIExpression *) cat_iiterator_next(iter);
			jag_ast_writer_print(out, cat_string_wo_new_with("["));
			jag_ast_iexpression_write(dim_expr, out);
			jag_ast_writer_print(out, cat_string_wo_new_with("]"));
		}
		cat_unref_ptr(iter);
	}
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}

/********************* end JagAstIConditionalExpression implementation *********************/

