/*
   File:    jagastforloopstatement.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Dec 24, 2012
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


#ifndef JAGASTFORLOOPSTATEMENT_H_
#define JAGASTFORLOOPSTATEMENT_H_

#include "jagastiastnode.h"
#include "jagastiexpression.h"
#include "jagastblock.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_FOR_LOOP_STATEMENT              (jag_ast_for_loop_statement_get_type())
#define JAG_AST_FOR_LOOP_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_for_loop_statement_get_type(), JagAstForLoopStatement))
#define JAG_AST_FOR_LOOP_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_FOR_LOOP_STATEMENT, JagAstForLoopStatementClass))
#define JAG_AST_IS_FOR_LOOP_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_FOR_LOOP_STATEMENT))
#define JAG_AST_IS_FOR_LOOP_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_FOR_LOOP_STATEMENT))
#define JAG_AST_FOR_LOOP_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_FOR_LOOP_STATEMENT, JagAstForLoopStatementClass))
#define JAG_AST_FOR_LOOP_STATEMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_FOR_LOOP_STATEMENT, JagAstForLoopStatementPrivate))

typedef struct _JagAstForLoopStatement               JagAstForLoopStatement;
typedef struct _JagAstForLoopStatementPrivate        JagAstForLoopStatementPrivate;
typedef struct _JagAstForLoopStatementClass          JagAstForLoopStatementClass;


struct _JagAstForLoopStatement {
	GObject parent;
	JagAstForLoopStatementPrivate *priv;
};

struct _JagAstForLoopStatementClass {
	GObjectClass parent_class;
};


GType jag_ast_for_loop_statement_get_type();

JagAstForLoopStatement *jag_ast_for_loop_statement_new(JagAstIAstNode *forInit, JagAstIExpression *forCondition, JagAstIAstNode *forUpdate, JagAstBlock *body);

G_END_DECLS


#endif /* JAGASTFORLOOPSTATEMENT_H_ */
