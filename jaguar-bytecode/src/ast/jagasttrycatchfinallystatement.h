/*
   File:    jagasttrycatchfinallystatement.h
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

#ifndef JAGASTTRYCATCHFINALLYSTATEMENT_H_
#define JAGASTTRYCATCHFINALLYSTATEMENT_H_

#include "jagastblock.h"
#include "jagastfinallyblock.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_TRY_CATCH_FINALLY_STATEMENT              (jag_ast_try_catch_finally_statement_get_type())
#define JAG_AST_TRY_CATCH_FINALLY_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_try_catch_finally_statement_get_type(), JagAstTryCatchFinallyStatement))
#define JAG_AST_TRY_CATCH_FINALLY_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_TRY_CATCH_FINALLY_STATEMENT, JagAstTryCatchFinallyStatementClass))
#define JAG_AST_IS_TRY_CATCH_FINALLY_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_TRY_CATCH_FINALLY_STATEMENT))
#define JAG_AST_IS_TRY_CATCH_FINALLY_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_TRY_CATCH_FINALLY_STATEMENT))
#define JAG_AST_TRY_CATCH_FINALLY_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_TRY_CATCH_FINALLY_STATEMENT, JagAstTryCatchFinallyStatementClass))

typedef struct _JagAstTryCatchFinallyStatement               JagAstTryCatchFinallyStatement;
typedef struct _JagAstTryCatchFinallyStatementPrivate        JagAstTryCatchFinallyStatementPrivate;
typedef struct _JagAstTryCatchFinallyStatementClass          JagAstTryCatchFinallyStatementClass;


struct _JagAstTryCatchFinallyStatement {
	GObject parent;
};

struct _JagAstTryCatchFinallyStatementClass {
	GObjectClass parent_class;
};


GType jag_ast_try_catch_finally_statement_get_type();

JagAstTryCatchFinallyStatement *jag_ast_try_catch_finally_statement_new(JagAstBlock *try_block, CatArrayWo *e_catch_blocks, JagAstFinallyBlock *finally_block);

G_END_DECLS

#endif /* JAGASTTRYCATCHFINALLYSTATEMENT_H_ */
