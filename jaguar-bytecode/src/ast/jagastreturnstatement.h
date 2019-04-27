/*
   File:    jagastreturnstatement.h
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

#ifndef JAGASTRETURNSTATEMENT_H_
#define JAGASTRETURNSTATEMENT_H_

#include "jagastiexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_RETURN_STATEMENT              (jag_ast_return_statement_get_type())
#define JAG_AST_RETURN_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_return_statement_get_type(), JagAstReturnStatement))
#define JAG_AST_RETURN_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_RETURN_STATEMENT, JagAstReturnStatementClass))
#define JAG_AST_IS_RETURN_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_RETURN_STATEMENT))
#define JAG_AST_IS_RETURN_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_RETURN_STATEMENT))
#define JAG_AST_RETURN_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_RETURN_STATEMENT, JagAstReturnStatementClass))

typedef struct _JagAstReturnStatement               JagAstReturnStatement;
typedef struct _JagAstReturnStatementPrivate        JagAstReturnStatementPrivate;
typedef struct _JagAstReturnStatementClass          JagAstReturnStatementClass;


struct _JagAstReturnStatement {
	GObject parent;
};

struct _JagAstReturnStatementClass {
	GObjectClass parent_class;
};


GType jag_ast_return_statement_get_type();

JagAstReturnStatement *jag_ast_return_statement_new(JagAstIExpression *return_expression);

JagAstIExpression *jag_ast_return_statement_get_expression(JagAstReturnStatement *return_statement);

G_END_DECLS

#endif /* JAGASTRETURNSTATEMENT_H_ */
