/*
   File:    jagpjcassignop.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 4, 2017
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

#ifndef TREE_JAGPJCASSIGNOP_H_
#define TREE_JAGPJCASSIGNOP_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCASSIGNOP              (jagp_jcassignop_get_type())
#define JAGP_JCASSIGNOP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcassignop_get_type(), JagPJCAssignOp))
#define JAGP_JCASSIGNOP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCASSIGNOP, JagPJCAssignOpClass))
#define JAGP_IS_JCASSIGNOP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCASSIGNOP))
#define JAGP_IS_JCASSIGNOP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCASSIGNOP))
#define JAGP_JCASSIGNOP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCASSIGNOP, JagPJCAssignOpClass))


typedef struct _JagPJCAssignOp               JagPJCAssignOp;
typedef struct _JagPJCAssignOpClass          JagPJCAssignOpClass;


struct _JagPJCAssignOp {
	JagPJCExpression parent;
	JagPTag opcode;
	JagPJCExpression *lhs;
	JagPJCExpression *rhs;
};

struct _JagPJCAssignOpClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcassignop_get_type();

JagPJCAssignOp *jagp_jcassignop_new(JagPTag opcode, JagPJCExpression *lhs, JagPJCExpression *rhs);

G_END_DECLS

#endif /* TREE_JAGPJCASSIGNOP_H_ */
