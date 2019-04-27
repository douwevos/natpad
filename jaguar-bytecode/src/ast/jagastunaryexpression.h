/*
   File:    jagastunaryexpression.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 29, 2012
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

#ifndef JAGASTUNARYEXPRESSION_H_
#define JAGASTUNARYEXPRESSION_H_

#include "jagasticonditionalexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_UNARY_EXPRESSION              (jag_ast_unary_expression_get_type())
#define JAG_AST_UNARY_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_unary_expression_get_type(), JagAstUnaryExpression))
#define JAG_AST_UNARY_EXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_UNARY_EXPRESSION, JagAstUnaryExpressionClass))
#define JAG_AST_IS_UNARY_EXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_UNARY_EXPRESSION))
#define JAG_AST_IS_UNARY_EXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_UNARY_EXPRESSION))
#define JAG_AST_UNARY_EXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_UNARY_EXPRESSION, JagAstUnaryExpressionClass))

typedef struct _JagAstUnaryExpression               JagAstUnaryExpression;
typedef struct _JagAstUnaryExpressionPrivate        JagAstUnaryExpressionPrivate;
typedef struct _JagAstUnaryExpressionClass          JagAstUnaryExpressionClass;


struct _JagAstUnaryExpression {
	GObject parent;
};

struct _JagAstUnaryExpressionClass {
	GObjectClass parent_class;
};


GType jag_ast_unary_expression_get_type();

//JagAstUnaryExpression *jag_ast_unary_expression_new();

JagAstIConditionalExpression *jag_ast_unary_expression_create(JagAstIConditionalExpression *expression);

G_END_DECLS

#endif /* JAGASTUNARYEXPRESSION_H_ */
