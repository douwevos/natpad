/*
   File:    jagastimportstatement.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 1, 2012
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

#ifndef JAGASTIMPORTSTATEMENT_H_
#define JAGASTIMPORTSTATEMENT_H_

#include "../classfile/jagbytname.h"
#include "formater/jagastwriter.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_IMPORT_STATEMENT              (jag_ast_import_statement_get_type())
#define JAG_AST_IMPORT_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_import_statement_get_type(), JagAstImportStatement))
#define JAG_AST_IMPORT_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_IMPORT_STATEMENT, JagAstImportStatementClass))
#define JAG_AST_IS_IMPORT_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_IMPORT_STATEMENT))
#define JAG_AST_IS_IMPORT_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_IMPORT_STATEMENT))
#define JAG_AST_IMPORT_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_IMPORT_STATEMENT, JagAstImportStatementClass))

typedef struct _JagAstImportStatement               JagAstImportStatement;
typedef struct _JagAstImportStatementPrivate        JagAstImportStatementPrivate;
typedef struct _JagAstImportStatementClass          JagAstImportStatementClass;


struct _JagAstImportStatement {
	GObject parent;
};

struct _JagAstImportStatementClass {
	GObjectClass parent_class;
};


GType jag_ast_import_statement_get_type();

JagAstImportStatement *jag_ast_import_statement_new(JagBytName *name);

JagBytName *jag_ast_import_statement_get_name(JagAstImportStatement *import_statement);

void jag_ast_import_statement_write(JagAstImportStatement *import_statement, JagAstWriter *out);

G_END_DECLS

#endif /* JAGASTIMPORTSTATEMENT_H_ */
