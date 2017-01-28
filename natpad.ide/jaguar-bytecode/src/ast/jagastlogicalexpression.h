/*
   File:    jagastlogicalexpression.h
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


#ifndef JAGASTLOGICALEXPRESSION_H_
#define JAGASTLOGICALEXPRESSION_H_

#include "../classfile/bytecode/jagbytmathoperator.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_LOGICAL_EXPRESSION              (jag_ast_logical_expression_get_type())
#define JAG_AST_LOGICAL_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_logical_expression_get_type(), JagAstLogicalExpression))
#define JAG_AST_LOGICAL_EXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_LOGICAL_EXPRESSION, JagAstLogicalExpressionClass))
#define JAG_AST_IS_LOGICAL_EXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_LOGICAL_EXPRESSION))
#define JAG_AST_IS_LOGICAL_EXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_LOGICAL_EXPRESSION))
#define JAG_AST_LOGICAL_EXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_LOGICAL_EXPRESSION, JagAstLogicalExpressionClass))
#define JAG_AST_LOGICAL_EXPRESSION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_LOGICAL_EXPRESSION, JagAstLogicalExpressionPrivate))

typedef struct _JagAstLogicalExpression               JagAstLogicalExpression;
typedef struct _JagAstLogicalExpressionPrivate        JagAstLogicalExpressionPrivate;
typedef struct _JagAstLogicalExpressionClass          JagAstLogicalExpressionClass;


struct _JagAstLogicalExpression {
	GObject parent;
	JagAstLogicalExpressionPrivate *priv;
};

struct _JagAstLogicalExpressionClass {
	GObjectClass parent_class;
};


GType jag_ast_logical_expression_get_type();

JagAstLogicalExpression *jag_ast_logical_expression_new(JagBytMathOperator logical_operator, CatArrayWo *e_expressions);

G_END_DECLS


#endif /* JAGASTLOGICALEXPRESSION_H_ */
