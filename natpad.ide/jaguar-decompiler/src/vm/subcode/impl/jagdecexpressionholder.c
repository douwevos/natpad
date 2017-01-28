/*
   File:    jagdecexpressionholder.c
   Project: jaguar-decompiler
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

#include "jagdecexpressionholder.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecExpressionHolder"
#include <logging/catlog.h>

struct _JagDecExpressionHolderPrivate {
	JagAstIExpression *expression;
};

G_DEFINE_TYPE (JagDecExpressionHolder, jag_dec_expression_holder, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_expression_holder_class_init(JagDecExpressionHolderClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecExpressionHolderPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_expression_holder_init(JagDecExpressionHolder *instance) {
	JagDecExpressionHolderPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_EXPRESSION_HOLDER, JagDecExpressionHolderPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecExpressionHolder *instance = JAG_DEC_EXPRESSION_HOLDER(object);
	JagDecExpressionHolderPrivate *priv = instance->priv;
	cat_unref_ptr(priv->expression);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecExpressionHolder *jag_dec_expression_holder_new() {
	JagDecExpressionHolder *result = g_object_new(JAG_DEC_TYPE_EXPRESSION_HOLDER, NULL);
	cat_ref_anounce(result);
	JagDecExpressionHolderPrivate *priv = result->priv;
	priv->expression = NULL;
	return result;
}


JagAstIExpression *jag_dec_expression_holder_get(JagDecExpressionHolder *holder) {
	JagDecExpressionHolderPrivate *priv = JAG_DEC_EXPRESSION_HOLDER_GET_PRIVATE(holder);
	return priv->expression;
}

void jag_dec_expression_holder_set(JagDecExpressionHolder *holder, JagAstIExpression *to_set) {
	JagDecExpressionHolderPrivate *priv = JAG_DEC_EXPRESSION_HOLDER_GET_PRIVATE(holder);
	cat_ref_swap(priv->expression, to_set);
}


