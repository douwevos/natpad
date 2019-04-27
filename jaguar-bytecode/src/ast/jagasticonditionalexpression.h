/*
   File:    jagasticonditionalexpression.h
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

#ifndef JAGASTICONDITIONALEXPRESSION_H_
#define JAGASTICONDITIONALEXPRESSION_H_

#include "formater/jagastwriter.h"
#include "jagastiexpression.h"
#include <glib-object.h>

#define JAG_AST_TYPE_ICONDITIONAL_EXPRESSION                 (jag_ast_iconditional_expression_get_type())
#define JAG_AST_ICONDITIONAL_EXPRESSION(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_AST_TYPE_ICONDITIONAL_EXPRESSION, JagAstIConditionalExpression))
#define JAG_AST_IS_ICONDITIONAL_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_AST_TYPE_ICONDITIONAL_EXPRESSION))
#define JAG_AST_ICONDITIONAL_EXPRESSION_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_AST_TYPE_ICONDITIONAL_EXPRESSION, JagAstIConditionalExpressionInterface))


typedef struct _JagAstIConditionalExpression               JagAstIConditionalExpression; /* dummy object */
typedef struct _JagAstIConditionalExpressionInterface      JagAstIConditionalExpressionInterface;

struct _JagAstIConditionalExpressionInterface {
	GTypeInterface parent_iface;

	JagAstIConditionalExpression *(*invert)(JagAstIConditionalExpression *self);
};

GType jag_ast_iconditional_expression_get_type(void);

JagAstIConditionalExpression *jag_ast_iconditional_expression_invert(JagAstIConditionalExpression *self);

#endif /* JAGASTICONDITIONALEXPRESSION_H_ */
