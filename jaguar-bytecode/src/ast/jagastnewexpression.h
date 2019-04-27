/*
   File:    jagastnewexpression.h
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

#ifndef JAGASTNEWEXPRESSION_H_
#define JAGASTNEWEXPRESSION_H_

#include "jagastidentifier.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_NEW_EXPRESSION              (jag_ast_new_expression_get_type())
#define JAG_AST_NEW_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_new_expression_get_type(), JagAstNewExpression))
#define JAG_AST_NEW_EXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_NEW_EXPRESSION, JagAstNewExpressionClass))
#define JAG_AST_IS_NEW_EXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_NEW_EXPRESSION))
#define JAG_AST_IS_NEW_EXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_NEW_EXPRESSION))
#define JAG_AST_NEW_EXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_NEW_EXPRESSION, JagAstNewExpressionClass))

typedef struct _JagAstNewExpression               JagAstNewExpression;
typedef struct _JagAstNewExpressionPrivate        JagAstNewExpressionPrivate;
typedef struct _JagAstNewExpressionClass          JagAstNewExpressionClass;


struct _JagAstNewExpression {
	GObject parent;
};

struct _JagAstNewExpressionClass {
	GObjectClass parent_class;
};


GType jag_ast_new_expression_get_type();

JagAstNewExpression *jag_ast_new_expression_new(JagAstIdentifier *identifier, CatArrayWo *e_dim_expressions);

G_END_DECLS

#endif /* JAGASTNEWEXPRESSION_H_ */
