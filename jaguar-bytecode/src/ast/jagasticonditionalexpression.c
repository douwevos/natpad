/*
   File:    jagasticonditionalexpression.c
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

#include "jagasticonditionalexpression.h"
#include "jagastiexpression.h"

G_DEFINE_INTERFACE(JagAstIConditionalExpression, jag_ast_iconditional_expression, JAG_AST_TYPE_IEXPRESSION);

static void jag_ast_iconditional_expression_default_init(JagAstIConditionalExpressionInterface*klass) {
}

JagAstIConditionalExpression *jag_ast_iconditional_expression_invert(JagAstIConditionalExpression *self) {
	g_return_val_if_fail(JAG_AST_IS_ICONDITIONAL_EXPRESSION(self), self);
	return JAG_AST_ICONDITIONAL_EXPRESSION_GET_INTERFACE(self)->invert(self);
}
