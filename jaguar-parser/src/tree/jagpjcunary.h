/*
   File:    jagpjcunary.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 5, 2017
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

#ifndef TREE_JAGPJCUNARY_H_
#define TREE_JAGPJCUNARY_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCUNARY              (jagp_jcunary_get_type())
#define JAGP_JCUNARY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcunary_get_type(), JagPJCUnary))
#define JAGP_JCUNARY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCUNARY, JagPJCUnaryClass))
#define JAGP_IS_JCUNARY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCUNARY))
#define JAGP_IS_JCUNARY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCUNARY))
#define JAGP_JCUNARY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCUNARY, JagPJCUnaryClass))


typedef struct _JagPJCUnary               JagPJCUnary;
typedef struct _JagPJCUnaryClass          JagPJCUnaryClass;


struct _JagPJCUnary {
	JagPJCExpression parent;
	JagPTag opcode;
	JagPJCExpression *arg;
};

struct _JagPJCUnaryClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcunary_get_type();

JagPJCUnary *jagp_jcunary_new(JagPTag opcode, JagPJCExpression *arg);

G_END_DECLS

#endif /* TREE_JAGPJCUNARY_H_ */
