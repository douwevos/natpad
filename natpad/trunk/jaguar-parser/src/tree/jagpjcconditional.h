/*
   File:    jagpjcconditional.h
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

#ifndef TREE_JAGPJCCONDITIONAL_H_
#define TREE_JAGPJCCONDITIONAL_H_

#include "jagpjcpolyexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCCONDITIONAL              (jagp_jcconditional_get_type())
#define JAGP_JCCONDITIONAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcconditional_get_type(), JagPJCConditional))
#define JAGP_JCCONDITIONAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCCONDITIONAL, JagPJCConditionalClass))
#define JAGP_IS_JCCONDITIONAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCCONDITIONAL))
#define JAGP_IS_JCCONDITIONAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCCONDITIONAL))
#define JAGP_JCCONDITIONAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCCONDITIONAL, JagPJCConditionalClass))


typedef struct _JagPJCConditional               JagPJCConditional;
typedef struct _JagPJCConditionalClass          JagPJCConditionalClass;


struct _JagPJCConditional {
	JagPJCPolyExpression parent;
	JagPJCExpression *cond;
	JagPJCExpression *truepart;
	JagPJCExpression *falsepart;

};

struct _JagPJCConditionalClass {
	JagPJCPolyExpressionClass parent_class;
};


GType jagp_jcconditional_get_type();

JagPJCConditional *jagp_jcconditional_new(JagPJCExpression *cond, JagPJCExpression *truepart, JagPJCExpression *falsepart);

G_END_DECLS

#endif /* TREE_JAGPJCCONDITIONAL_H_ */
