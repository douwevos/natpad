/*
   File:    jagastrelationalexpression.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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

#ifndef JAGASTRELATIONALEXPRESSION_H_
#define JAGASTRELATIONALEXPRESSION_H_

#include "jagastiexpression.h"
#include "../classfile/bytecode/jagbytconditiontype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_RELATIONAL_EXPRESSION              (jag_ast_relational_expression_get_type())
#define JAG_AST_RELATIONAL_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_relational_expression_get_type(), JagAstRelationalExpression))
#define JAG_AST_RELATIONAL_EXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_RELATIONAL_EXPRESSION, JagAstRelationalExpressionClass))
#define JAG_AST_IS_RELATIONAL_EXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_RELATIONAL_EXPRESSION))
#define JAG_AST_IS_RELATIONAL_EXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_RELATIONAL_EXPRESSION))
#define JAG_AST_RELATIONAL_EXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_RELATIONAL_EXPRESSION, JagAstRelationalExpressionClass))

typedef struct _JagAstRelationalExpression               JagAstRelationalExpression;
typedef struct _JagAstRelationalExpressionPrivate        JagAstRelationalExpressionPrivate;
typedef struct _JagAstRelationalExpressionClass          JagAstRelationalExpressionClass;


struct _JagAstRelationalExpression {
	GObject parent;
};

struct _JagAstRelationalExpressionClass {
	GObjectClass parent_class;
};


GType jag_ast_relational_expression_get_type();

JagAstRelationalExpression *jag_ast_relational_expression_new(JagBytConditionType condition_type, JagAstIExpression *left, JagAstIExpression *right);

G_END_DECLS

#endif /* JAGASTRELATIONALEXPRESSION_H_ */
