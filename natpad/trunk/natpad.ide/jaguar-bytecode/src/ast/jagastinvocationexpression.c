/*
   File:    jagastinvocationexpression.c
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

#include "jagastinvocationexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstInvocationExpression"
#include <logging/catlog.h>

struct _JagAstInvocationExpressionPrivate {
	JagAstIExpression *instance_expression;
	JagAstIdentifier *method_name;
	CatArrayWo *e_argument_expressions;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstInvocationExpression, jag_ast_invocation_expression, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_invocation_expression_class_init(JagAstInvocationExpressionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstInvocationExpressionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_invocation_expression_init(JagAstInvocationExpression *instance) {
	JagAstInvocationExpressionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_INVOCATION_EXPRESSION, JagAstInvocationExpressionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstInvocationExpression *instance = JAG_AST_INVOCATION_EXPRESSION(object);
	JagAstInvocationExpressionPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_argument_expressions);
	cat_unref_ptr(priv->instance_expression);
	cat_unref_ptr(priv->method_name);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstInvocationExpression *jag_ast_invocation_expression_new(JagAstIExpression *instance_expression, JagAstIdentifier *method_name, CatArrayWo *e_argument_expressions) {
	JagAstInvocationExpression *result = g_object_new(JAG_AST_TYPE_INVOCATION_EXPRESSION, NULL);
	cat_ref_anounce(result);
	JagAstInvocationExpressionPrivate *priv = result->priv;
	priv->instance_expression = cat_ref_ptr(instance_expression);
	priv->method_name = cat_ref_ptr(method_name);
	priv->e_argument_expressions = cat_ref_ptr(e_argument_expressions);
	return result;
}




/********************* start JagAstIExpression implementation *********************/

static CatS l_s_txt_this = CAT_S_DEF("this");

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstInvocationExpressionPrivate *priv = JAG_AST_INVOCATION_EXPRESSION_GET_PRIVATE(self);

	if (priv->instance_expression==NULL) {
		cat_log_warn("instance_expression is NULL");
		return;
	}

	gboolean is_this_expression = FALSE;
	if (JAG_AST_IS_IDENTIFIER(priv->instance_expression)) {
		CatStringWo *a_id_text = jag_ast_identifier_get_text((JagAstIdentifier *) priv->instance_expression);
		is_this_expression = cat_string_wo_equal(CAT_S(l_s_txt_this), a_id_text);
	}

	if (is_this_expression) {
		if (priv->method_name!=NULL) {
			jag_ast_iexpression_write((JagAstIExpression *) priv->method_name, out);
		} else {
			jag_ast_iexpression_write(priv->instance_expression, out);
		}
	} else {
		jag_ast_iexpression_write(priv->instance_expression, out);
		if (priv->method_name!=NULL) {
			jag_ast_writer_print(out, cat_string_wo_new_with("."));
			jag_ast_iexpression_write((JagAstIExpression *) priv->method_name, out);
		}
	}
	jag_ast_writer_print(out, cat_string_wo_new_with("("));

	if (priv->e_argument_expressions!=NULL && cat_array_wo_size(priv->e_argument_expressions)>0) {
		int arg_count = cat_array_wo_size(priv->e_argument_expressions);
		int arg_idx;
		for(arg_idx=0; arg_idx<arg_count; arg_idx++) {
			if (arg_idx!=0) {
				jag_ast_writer_print(out, cat_string_wo_new_with(", "));
			}
			JagAstIExpression *arg_expr = (JagAstIExpression *) cat_array_wo_get(priv->e_argument_expressions, arg_idx);
			if (arg_expr==NULL) {
				jag_ast_writer_print(out, cat_string_wo_new_with("???"));
			} else {
				jag_ast_iexpression_write(arg_expr, out);
			}
		}
	}

	jag_ast_writer_print(out, cat_string_wo_new_with(")"));

}

static gboolean l_expression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (self==other) {
		return TRUE;
	}
	if (self==NULL || other==NULL) {
		return FALSE;
	}
	JagAstInvocationExpressionPrivate *priv = JAG_AST_INVOCATION_EXPRESSION_GET_PRIVATE(self);
	JagAstInvocationExpressionPrivate *o_priv = JAG_AST_INVOCATION_EXPRESSION_GET_PRIVATE(other);
	if (!jag_ast_iexpression_equal((JagAstIExpression *) priv->method_name, (JagAstIExpression *) o_priv->method_name)) {
		return FALSE;
	}
	if (!jag_ast_iexpression_equal(priv->instance_expression, o_priv->instance_expression)) {
		return FALSE;
	}
	if (!cat_array_wo_equal(priv->e_argument_expressions, o_priv->e_argument_expressions, (GEqualFunc) jag_ast_iexpression_equal)) {
		return FALSE;
	}
	return TRUE;
}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
	iface->equal = l_expression_equal;
}

/********************* end JagAstIExpression implementation *********************/
