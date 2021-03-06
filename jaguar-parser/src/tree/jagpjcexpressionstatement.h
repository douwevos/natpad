/*
   File:    jagpjcexpressionstatement.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 14, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef TREE_JAGPJCEXPRESSIONSTATEMENT_H_
#define TREE_JAGPJCEXPRESSIONSTATEMENT_H_

#include "jagpjcstatement.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCEXPRESSION_STATEMENT              (jagp_jcexpression_statement_get_type())
#define JAGP_JCEXPRESSION_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcexpression_statement_get_type(), JagPJCExpressionStatement))
#define JAGP_JCEXPRESSION_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCEXPRESSION_STATEMENT, JagPJCExpressionStatementClass))
#define JAGP_IS_JCEXPRESSION_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCEXPRESSION_STATEMENT))
#define JAGP_IS_JCEXPRESSION_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCEXPRESSION_STATEMENT))
#define JAGP_JCEXPRESSION_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCEXPRESSION_STATEMENT, JagPJCExpressionStatementClass))


typedef struct _JagPJCExpressionStatement               JagPJCExpressionStatement;
typedef struct _JagPJCExpressionStatementClass          JagPJCExpressionStatementClass;


struct _JagPJCExpressionStatement {
	JagPJCStatement parent;
	JagPJCExpression *expr;
};

struct _JagPJCExpressionStatementClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcexpression_statement_get_type();

JagPJCExpressionStatement *jagp_jcexpression_statement_new(JagPJCExpression *expr);

G_END_DECLS

#endif /* TREE_JAGPJCEXPRESSIONSTATEMENT_H_ */
