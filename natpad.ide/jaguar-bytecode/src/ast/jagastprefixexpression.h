/*
   File:    jagastprefixexpression.h
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


#ifndef JAGASTPREFIXEXPRESSION_H_
#define JAGASTPREFIXEXPRESSION_H_

#include "jagastiexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_PREFIX_EXPRESSION              (jag_ast_prefix_expression_get_type())
#define JAG_AST_PREFIX_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_prefix_expression_get_type(), JagAstPrefixExpression))
#define JAG_AST_PREFIX_EXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_PREFIX_EXPRESSION, JagAstPrefixExpressionClass))
#define JAG_AST_IS_PREFIX_EXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_PREFIX_EXPRESSION))
#define JAG_AST_IS_PREFIX_EXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_PREFIX_EXPRESSION))
#define JAG_AST_PREFIX_EXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_PREFIX_EXPRESSION, JagAstPrefixExpressionClass))
#define JAG_AST_PREFIX_EXPRESSION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_PREFIX_EXPRESSION, JagAstPrefixExpressionPrivate))

typedef struct _JagAstPrefixExpression               JagAstPrefixExpression;
typedef struct _JagAstPrefixExpressionPrivate        JagAstPrefixExpressionPrivate;
typedef struct _JagAstPrefixExpressionClass          JagAstPrefixExpressionClass;


struct _JagAstPrefixExpression {
	GObject parent;
	JagAstPrefixExpressionPrivate *priv;
};

struct _JagAstPrefixExpressionClass {
	GObjectClass parent_class;
};


GType jag_ast_prefix_expression_get_type();

JagAstPrefixExpression *jag_ast_prefix_expression_new(gboolean is_increment, JagAstIExpression *sub_expression);

G_END_DECLS


#endif /* JAGASTPREFIXEXPRESSION_H_ */
