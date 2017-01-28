/*
   File:    jagastpackagestatement.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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


#ifndef JAGASTPACKAGESTATEMENT_H_
#define JAGASTPACKAGESTATEMENT_H_

#include "jagastidentifier.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_PACKAGE_STATEMENT              (jag_ast_package_statement_get_type())
#define JAG_AST_PACKAGE_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_package_statement_get_type(), JagAstPackageStatement))
#define JAG_AST_PACKAGE_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_PACKAGE_STATEMENT, JagAstPackageStatementClass))
#define JAG_AST_IS_PACKAGE_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_PACKAGE_STATEMENT))
#define JAG_AST_IS_PACKAGE_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_PACKAGE_STATEMENT))
#define JAG_AST_PACKAGE_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_PACKAGE_STATEMENT, JagAstPackageStatementClass))
#define JAG_AST_PACKAGE_STATEMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_PACKAGE_STATEMENT, JagAstPackageStatementPrivate))

typedef struct _JagAstPackageStatement               JagAstPackageStatement;
typedef struct _JagAstPackageStatementPrivate        JagAstPackageStatementPrivate;
typedef struct _JagAstPackageStatementClass          JagAstPackageStatementClass;


struct _JagAstPackageStatement {
	GObject parent;
	JagAstPackageStatementPrivate *priv;
};

struct _JagAstPackageStatementClass {
	GObjectClass parent_class;
};


GType jag_ast_package_statement_get_type();

JagAstPackageStatement *jag_ast_package_statement_new(JagAstIdentifier *package_name);

G_END_DECLS



#endif /* JAGASTPACKAGESTATEMENT_H_ */
