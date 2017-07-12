/*
   File:    jagpjcinstanceof.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Jul 3, 2017
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

#ifndef SRC_TREE_JAGPJCINSTANCEOF_H_
#define SRC_TREE_JAGPJCINSTANCEOF_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCINSTANCE_OF              (jagp_jcinstance_of_get_type())
#define JAGP_JCINSTANCE_OF(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcinstance_of_get_type(), JagPJCInstanceOf))
#define JAGP_JCINSTANCE_OF_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCINSTANCE_OF, JagPJCInstanceOfClass))
#define JAGP_IS_JCINSTANCE_OF(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCINSTANCE_OF))
#define JAGP_IS_JCINSTANCE_OF_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCINSTANCE_OF))
#define JAGP_JCINSTANCE_OF_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCINSTANCE_OF, JagPJCInstanceOfClass))


typedef struct _JagPJCInstanceOf               JagPJCInstanceOf;
typedef struct _JagPJCInstanceOfClass          JagPJCInstanceOfClass;


struct _JagPJCInstanceOf {
	JagPJCExpression parent;
    JagPJCExpression *expr;
    JagPJCTree *clazz;
};

struct _JagPJCInstanceOfClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcinstance_of_get_type();

JagPJCInstanceOf *jagp_jcinstance_of_new(JagPJCExpression *expr, JagPJCTree *clazz);

G_END_DECLS

#endif /* SRC_TREE_JAGPJCINSTANCEOF_H_ */
