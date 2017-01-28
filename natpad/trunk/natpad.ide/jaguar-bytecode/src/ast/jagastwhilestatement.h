/*
   File:    jagastwhilestatement.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 18, 2012
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


#ifndef JAGASTWHILESTATEMENT_H_
#define JAGASTWHILESTATEMENT_H_

#include "jagastblock.h"
#include "jagasticonditionalexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_WHILE_STATEMENT              (jag_ast_while_statement_get_type())
#define JAG_AST_WHILE_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_while_statement_get_type(), JagAstWhileStatement))
#define JAG_AST_WHILE_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_WHILE_STATEMENT, JagAstWhileStatementClass))
#define JAG_AST_IS_WHILE_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_WHILE_STATEMENT))
#define JAG_AST_IS_WHILE_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_WHILE_STATEMENT))
#define JAG_AST_WHILE_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_WHILE_STATEMENT, JagAstWhileStatementClass))
#define JAG_AST_WHILE_STATEMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_WHILE_STATEMENT, JagAstWhileStatementPrivate))

typedef struct _JagAstWhileStatement               JagAstWhileStatement;
typedef struct _JagAstWhileStatementPrivate        JagAstWhileStatementPrivate;
typedef struct _JagAstWhileStatementClass          JagAstWhileStatementClass;


struct _JagAstWhileStatement {
	GObject parent;
	JagAstWhileStatementPrivate *priv;
};

struct _JagAstWhileStatementClass {
	GObjectClass parent_class;
};


GType jag_ast_while_statement_get_type();

JagAstWhileStatement *jag_ast_while_statement_new(JagAstBlock *body);

void jag_ast_while_statement_set_conditional_expression(JagAstWhileStatement *while_statement, JagAstIConditionalExpression *conditional_expression);

G_END_DECLS


#endif /* JAGASTWHILESTATEMENT_H_ */
