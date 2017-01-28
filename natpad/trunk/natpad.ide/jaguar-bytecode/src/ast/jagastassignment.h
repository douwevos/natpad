/*
   File:    jagastassignment.h
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


#ifndef JAGASTASSIGNMENT_H_
#define JAGASTASSIGNMENT_H_

#include "jagastiexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_ASSIGNMENT              (jag_ast_assignment_get_type())
#define JAG_AST_ASSIGNMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_assignment_get_type(), JagAstAssignment))
#define JAG_AST_ASSIGNMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_ASSIGNMENT, JagAstAssignmentClass))
#define JAG_AST_IS_ASSIGNMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_ASSIGNMENT))
#define JAG_AST_IS_ASSIGNMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_ASSIGNMENT))
#define JAG_AST_ASSIGNMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_ASSIGNMENT, JagAstAssignmentClass))
#define JAG_AST_ASSIGNMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_ASSIGNMENT, JagAstAssignmentPrivate))

typedef struct _JagAstAssignment               JagAstAssignment;
typedef struct _JagAstAssignmentPrivate        JagAstAssignmentPrivate;
typedef struct _JagAstAssignmentClass          JagAstAssignmentClass;


struct _JagAstAssignment {
	GObject parent;
	JagAstAssignmentPrivate *priv;
};

struct _JagAstAssignmentClass {
	GObjectClass parent_class;
};


GType jag_ast_assignment_get_type();

JagAstAssignment *jag_ast_assignment_new(JagAstIExpression *lvalue, JagAstIExpression *rvalue);

void jag_ast_assignment_write(JagAstAssignment *assignment, JagAstWriter *out);


G_END_DECLS


#endif /* JAGASTASSIGNMENT_H_ */
