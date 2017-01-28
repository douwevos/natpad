/*
   File:    jagastidentifier.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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

#include "jagastidentifier.h"
#include "jagasticonditionalexpression.h"
#include "jagastunaryexpression.h"
#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstIdentifier"
#include <logging/catlog.h>

struct _JagAstIdentifierPrivate {
	CatStringWo *a_id;
};

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface);
static void l_expression_iface_init(JagAstIExpressionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstIdentifier, jag_ast_identifier, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IEXPRESSION, l_expression_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ICONDITIONAL_EXPRESSION, l_conditional_expression_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_identifier_class_init(JagAstIdentifierClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstIdentifierPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_identifier_init(JagAstIdentifier *instance) {
	JagAstIdentifierPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_IDENTIFIER, JagAstIdentifierPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstIdentifier *instance = JAG_AST_IDENTIFIER(object);
	JagAstIdentifierPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_id);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstIdentifier *jag_ast_identifier_new(CatStringWo *a_id) {
	JagAstIdentifier *result = g_object_new(JAG_AST_TYPE_IDENTIFIER, NULL);
	cat_ref_anounce(result);
	JagAstIdentifierPrivate *priv = result->priv;
	priv->a_id = cat_ref_ptr(a_id);
	return result;
}

JagAstIdentifier *jag_ast_identifier_new_from_name(JagBytName *name) {
	JagAstIdentifier *result = g_object_new(JAG_AST_TYPE_IDENTIFIER, NULL);
	cat_ref_anounce(result);
	JagAstIdentifierPrivate *priv = result->priv;
	priv->a_id = cat_ref_ptr(jag_byt_name_create_fqn(name));
	return result;
}

CatStringWo *jag_ast_identifier_get_text(JagAstIdentifier *identifier) {
	JagAstIdentifierPrivate *priv = JAG_AST_IDENTIFIER_GET_PRIVATE(identifier);
	return priv->a_id;
}



/********************* start JagAstIConditionalExpression implementation *********************/


static JagAstIConditionalExpression *l_conditional_expression_invert(JagAstIConditionalExpression *self) {
	return jag_ast_unary_expression_create(self);
}

static void l_conditional_expression_iface_init(JagAstIConditionalExpressionInterface *iface) {
	iface->invert = l_conditional_expression_invert;
}


static void l_expression_write(JagAstIExpression *self, JagAstWriter *out) {
	JagAstIdentifierPrivate *priv = JAG_AST_IDENTIFIER_GET_PRIVATE(self);
	jag_ast_writer_print(out, priv->a_id);
}


static void l_expression_iface_init(JagAstIExpressionInterface *iface) {
	iface->write = l_expression_write;
}


/********************* end JagAstIConditionalExpression implementation *********************/
