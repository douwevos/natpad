/*
   File:    jagastquotedstring.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 16, 2012
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

#include "jagastquotedstring.h"
#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstQuotedString"
#include <logging/catlog.h>

struct _JagAstQuotedStringPrivate {
	CatStringWo *a_raw_text;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstQuotedString, jag_ast_quoted_string, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstQuotedString)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_quoted_string_class_init(JagAstQuotedStringClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_quoted_string_init(JagAstQuotedString *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstQuotedString *instance = JAG_AST_QUOTED_STRING(object);
	JagAstQuotedStringPrivate *priv = jag_ast_quoted_string_get_instance_private(instance);
	cat_unref_ptr(priv->a_raw_text);
	G_OBJECT_CLASS(jag_ast_quoted_string_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_quoted_string_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstQuotedString *jag_ast_quoted_string_new(CatStringWo *a_raw_text) {
	JagAstQuotedString *result = g_object_new(JAG_AST_TYPE_QUOTED_STRING, NULL);
	cat_ref_anounce(result);
	JagAstQuotedStringPrivate *priv = jag_ast_quoted_string_get_instance_private(result);
	priv->a_raw_text = cat_ref_sink_ptr(a_raw_text);
	return result;
}


/********************* start JagAstIConditionalExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstQuotedString *instance = JAG_AST_QUOTED_STRING(self);
	JagAstQuotedStringPrivate *priv = jag_ast_quoted_string_get_instance_private(instance);
	jag_ast_writer_print(out, cat_string_wo_new_with("\""));
	jag_ast_writer_print(out, priv->a_raw_text);
	jag_ast_writer_print(out, cat_string_wo_new_with("\""));
}

static gboolean l_expression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (self==other) {
		return TRUE;
	}
	if (self==NULL || other==NULL) {
		return FALSE;
	}

	JagAstQuotedStringPrivate *priv = jag_ast_quoted_string_get_instance_private(JAG_AST_QUOTED_STRING(self));
	gboolean result = FALSE;
	if (JAG_AST_IS_QUOTED_STRING(other)) {
		JagAstQuotedStringPrivate *o_priv = jag_ast_quoted_string_get_instance_private(JAG_AST_QUOTED_STRING(other));
		result = cat_string_wo_equal(priv->a_raw_text, o_priv->a_raw_text);
	}
	return result;
}


static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
	iface->equal = l_expression_equal;
}

/********************* end JagAstIConditionalExpression implementation *********************/
