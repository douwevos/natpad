/*
   File:    jagastfieldaccessexpression.h
   Project: jaguar-bytecode
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

#ifndef JAGASTFIELDACCESSEXPRESSION_H_
#define JAGASTFIELDACCESSEXPRESSION_H_

#include "jagastiexpression.h"
#include "jagastidentifier.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_FIELD_ACCESS_EXPRESSION              (jag_ast_field_access_expression_get_type())
#define JAG_AST_FIELD_ACCESS_EXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_field_access_expression_get_type(), JagAstFieldAccessExpression))
#define JAG_AST_FIELD_ACCESS_EXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_FIELD_ACCESS_EXPRESSION, JagAstFieldAccessExpressionClass))
#define JAG_AST_IS_FIELD_ACCESS_EXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_FIELD_ACCESS_EXPRESSION))
#define JAG_AST_IS_FIELD_ACCESS_EXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_FIELD_ACCESS_EXPRESSION))
#define JAG_AST_FIELD_ACCESS_EXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_FIELD_ACCESS_EXPRESSION, JagAstFieldAccessExpressionClass))

typedef struct _JagAstFieldAccessExpression               JagAstFieldAccessExpression;
typedef struct _JagAstFieldAccessExpressionPrivate        JagAstFieldAccessExpressionPrivate;
typedef struct _JagAstFieldAccessExpressionClass          JagAstFieldAccessExpressionClass;


struct _JagAstFieldAccessExpression {
	GObject parent;
};

struct _JagAstFieldAccessExpressionClass {
	GObjectClass parent_class;
};


GType jag_ast_field_access_expression_get_type();

JagAstFieldAccessExpression *jag_ast_field_access_expression_new(JagAstIExpression *instance_expression, JagAstIdentifier *field_name);

G_END_DECLS

#endif /* JAGASTFIELDACCESSEXPRESSION_H_ */
