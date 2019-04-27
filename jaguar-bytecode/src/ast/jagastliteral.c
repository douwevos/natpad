/*
   File:    jagastliteral.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 9, 2012
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

#include "jagastliteral.h"
#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstLiteral"
#include <logging/catlog.h>

struct _JagAstLiteralPrivate {
	CatIStringable *literal;
};

static void l_expression_iface_init(JagAstIExpressionInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstLiteral, jag_ast_literal, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstLiteral)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_literal_class_init(JagAstLiteralClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_literal_init(JagAstLiteral *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstLiteral *instance = JAG_AST_LITERAL(object);
	JagAstLiteralPrivate *priv = jag_ast_literal_get_instance_private(instance);
	cat_unref_ptr(priv->literal);
	G_OBJECT_CLASS(jag_ast_literal_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_literal_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstLiteral *jag_ast_literal_new(CatIStringable *literal) {
	JagAstLiteral *result = g_object_new(JAG_AST_TYPE_LITERAL, NULL);
	cat_ref_anounce(result);
//	cat_ref_ptr(result);
	JagAstLiteralPrivate *priv = jag_ast_literal_get_instance_private(result);
	priv->literal = cat_ref_sink_ptr(literal);
//	cat_ref_ptr(literal);

	if (!CAT_IS_ISTRINGABLE(literal)) {
		cat_stacktrace_print();
		cat_log_error("literal is not CatIStringable:%s", g_type_name_from_instance((GTypeInstance *) literal));
	} else {
		cat_log_detail("literal is CatIStringable:%p:%s", literal, g_type_name_from_instance((GTypeInstance *) literal));
	}

	return result;
}


CatIStringable *jag_ast_literal_get_value(JagAstLiteral *literal) {
	JagAstLiteralPrivate *priv = jag_ast_literal_get_instance_private(literal);
	return priv->literal;
}


/********************* start JagAstIExpression implementation *********************/

static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstLiteral *instance = JAG_AST_LITERAL(self);
	JagAstLiteralPrivate *priv = jag_ast_literal_get_instance_private(instance);

	if (priv==NULL) {
		char buf[500];
		sprintf(buf, "<priv=NULL:class=%p>", self);
		jag_ast_writer_print(out, cat_string_wo_new_with(buf));
		return;
	}
	cat_log_trace("<literal:class=%p:%s, this=%s>", priv->literal, priv->literal==NULL ? "nil" : g_type_name_from_instance((GTypeInstance *) priv->literal), g_type_name_from_instance((GTypeInstance *) self));

	if (CAT_IS_STRING_WO(priv->literal)) {
		jag_ast_writer_print(out, (CatStringWo *) priv->literal);
	} else {
		CatStringWo *buf = cat_string_wo_new();

	//	if (priv->literal!=NULL && CAT_IS_ISTRINGABLE(priv->literal)) {
			cat_istringable_print(priv->literal, buf);
	//	} else {
	//		char buf[500];
	//		sprintf(buf, "<invalid-literal:class=%s>", priv->literal==NULL ? "nil" : g_type_name_from_instance((GTypeInstance *) priv->literal));
	//		jag_ast_writer_print(out, cat_string_wo_new_with(buf));
	//	}
		jag_ast_writer_print(out, buf);
		cat_unref_ptr(buf);
	}
}

static gboolean l_expression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (self==other) {
		return TRUE;
	}
	if (self==NULL || other==NULL) {
		return FALSE;
	}
	if (JAG_AST_IS_LITERAL(other)) {
		JagAstLiteralPrivate *priv = jag_ast_literal_get_instance_private(JAG_AST_LITERAL(self));
		JagAstLiteralPrivate *o_priv = jag_ast_literal_get_instance_private(JAG_AST_LITERAL(other));
		CatIStringable *lit = priv->literal;
		CatIStringable *o_lit = o_priv->literal;
		if (lit==o_lit) {
			return TRUE;
		}
		if (lit==NULL || o_lit==NULL) {
			return FALSE;
		}
		GType type = ((GTypeInstance *) lit)->g_class->g_type;
		GType o_type = ((GTypeInstance *) o_lit)->g_class->g_type;
		if (type==o_type) {
			if (type==CAT_TYPE_INTEGER) {
				return cat_integer_equal((CatInteger *) lit, (CatInteger *) o_lit);
			} else if (type==CAT_TYPE_BOOLEAN) {
				return cat_boolean_equal((CatBoolean *) lit, (CatBoolean *)o_lit);
			} else if (type==CAT_TYPE_STRING_WO) {
				return cat_string_wo_equal((CatStringWo *) lit, (CatStringWo *) o_lit);
			}
			/* no known type ... just print both and compare the result */
			CatStringWo *buf = cat_string_wo_new();
			CatStringWo *o_buf = cat_string_wo_new();
			cat_istringable_print(lit, buf);
			cat_istringable_print(o_lit, o_buf);
			gboolean result = cat_string_wo_equal(buf, o_buf);
			cat_unref_ptr(buf);
			cat_unref_ptr(o_buf);
			return result;
		}
	}
	return FALSE;

}

static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
	iface->equal = l_expression_equal;
}

/********************* end JagAstIExpression implementation *********************/


/********************* start CatIStringableInterface implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagAstLiteral *instance = JAG_AST_LITERAL(self);
	JagAstLiteralPrivate *priv = jag_ast_literal_get_instance_private(instance);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%o]", name, priv->literal);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringableInterface implementation *********************/
