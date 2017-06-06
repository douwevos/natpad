/*
   File:    jagpjcassign.h
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

#ifndef TREE_JAGPJCASSIGN_H_
#define TREE_JAGPJCASSIGN_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCASSIGN              (jagp_jcassign_get_type())
#define JAGP_JCASSIGN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcassign_get_type(), JagPJCAssign))
#define JAGP_JCASSIGN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCASSIGN, JagPJCAssignClass))
#define JAGP_IS_JCASSIGN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCASSIGN))
#define JAGP_IS_JCASSIGN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCASSIGN))
#define JAGP_JCASSIGN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCASSIGN, JagPJCAssignClass))


typedef struct _JagPJCAssign               JagPJCAssign;
typedef struct _JagPJCAssignClass          JagPJCAssignClass;


struct _JagPJCAssign {
	JagPJCExpression parent;
	JagPJCExpression *lhs;
	JagPJCExpression *rhs;
};

struct _JagPJCAssignClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcassign_get_type();

JagPJCAssign *jagp_jcassign_new(JagPJCExpression *lhs, JagPJCExpression *rhs);

G_END_DECLS

#endif /* TREE_JAGPJCASSIGN_H_ */
