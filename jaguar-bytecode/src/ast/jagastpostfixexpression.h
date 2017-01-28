/*
   File:    jagastpostfixexpression.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 19, 2012
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


#ifndef JAGASTPOSTFIXEXPRESSION_H_
#define JAGASTPOSTFIXEXPRESSION_H_

#include "jagastiexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_POSTFIX_EXPRESSION              (jag_ast_postfix_expression_get_type())
#define JAG_AST_POSTFIX_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_postfix_expression_get_type(), JagAstPostfixExpression))
#define JAG_AST_POSTFIX_EXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_POSTFIX_EXPRESSION, JagAstPostfixExpressionClass))
#define JAG_AST_IS_POSTFIX_EXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_POSTFIX_EXPRESSION))
#define JAG_AST_IS_POSTFIX_EXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_POSTFIX_EXPRESSION))
#define JAG_AST_POSTFIX_EXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_POSTFIX_EXPRESSION, JagAstPostfixExpressionClass))
#define JAG_AST_POSTFIX_EXPRESSION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_POSTFIX_EXPRESSION, JagAstPostfixExpressionPrivate))

typedef struct _JagAstPostfixExpression               JagAstPostfixExpression;
typedef struct _JagAstPostfixExpressionPrivate        JagAstPostfixExpressionPrivate;
typedef struct _JagAstPostfixExpressionClass          JagAstPostfixExpressionClass;


struct _JagAstPostfixExpression {
	GObject parent;
	JagAstPostfixExpressionPrivate *priv;
};

struct _JagAstPostfixExpressionClass {
	GObjectClass parent_class;
};


GType jag_ast_postfix_expression_get_type();

JagAstPostfixExpression *jag_ast_postfix_expression_new(gboolean is_increment, JagAstIExpression *sub_expression);

G_END_DECLS


#endif /* JAGASTPOSTFIXEXPRESSION_H_ */
