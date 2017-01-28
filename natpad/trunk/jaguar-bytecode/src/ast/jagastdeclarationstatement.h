/*
   File:    jagastdeclarationstatement.h
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


#ifndef JAGASTDECLARATIONSTATEMENT_H_
#define JAGASTDECLARATIONSTATEMENT_H_

#include "jagastdeclarationtype.h"
#include "jagastassignment.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_DECLARATION_STATEMENT              (jag_ast_declaration_statement_get_type())
#define JAG_AST_DECLARATION_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_declaration_statement_get_type(), JagAstDeclarationStatement))
#define JAG_AST_DECLARATION_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_DECLARATION_STATEMENT, JagAstDeclarationStatementClass))
#define JAG_AST_IS_DECLARATION_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_DECLARATION_STATEMENT))
#define JAG_AST_IS_DECLARATION_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_DECLARATION_STATEMENT))
#define JAG_AST_DECLARATION_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_DECLARATION_STATEMENT, JagAstDeclarationStatementClass))
#define JAG_AST_DECLARATION_STATEMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_DECLARATION_STATEMENT, JagAstDeclarationStatementPrivate))

typedef struct _JagAstDeclarationStatement               JagAstDeclarationStatement;
typedef struct _JagAstDeclarationStatementPrivate        JagAstDeclarationStatementPrivate;
typedef struct _JagAstDeclarationStatementClass          JagAstDeclarationStatementClass;


struct _JagAstDeclarationStatement {
	GObject parent;
	JagAstDeclarationStatementPrivate *priv;
};

struct _JagAstDeclarationStatementClass {
	GObjectClass parent_class;
};


GType jag_ast_declaration_statement_get_type();

JagAstDeclarationStatement *jag_ast_declaration_statement_new(JagAstDeclarationType *declaration_type, JagAstAssignment *assignment);

G_END_DECLS

#endif /* JAGASTDECLARATIONSTATEMENT_H_ */
