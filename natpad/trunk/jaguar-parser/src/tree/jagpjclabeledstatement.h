/*
   File:    jagpjclabeledstatement.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 13, 2017
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

#ifndef TREE_JAGPJCLABELEDSTATEMENT_H_
#define TREE_JAGPJCLABELEDSTATEMENT_H_

#include "../code/jagpname.h"
#include "jagpjcstatement.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCLABELED_STATEMENT              (jagp_jclabeled_statement_get_type())
#define JAGP_JCLABELED_STATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jclabeled_statement_get_type(), JagPJCLabeledStatement))
#define JAGP_JCLABELED_STATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCLABELED_STATEMENT, JagPJCLabeledStatementClass))
#define JAGP_IS_JCLABELED_STATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCLABELED_STATEMENT))
#define JAGP_IS_JCLABELED_STATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCLABELED_STATEMENT))
#define JAGP_JCLABELED_STATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCLABELED_STATEMENT, JagPJCLabeledStatementClass))


typedef struct _JagPJCLabeledStatement               JagPJCLabeledStatement;
typedef struct _JagPJCLabeledStatementClass          JagPJCLabeledStatementClass;


struct _JagPJCLabeledStatement {
	JagPJCStatement parent;
    JagPName *label;
    JagPJCStatement *body;
};

struct _JagPJCLabeledStatementClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jclabeled_statement_get_type();

JagPJCLabeledStatement *jagp_jclabeled_statement_new(JagPName *label, JagPJCStatement *body);

G_END_DECLS

#endif /* TREE_JAGPJCLABELEDSTATEMENT_H_ */
