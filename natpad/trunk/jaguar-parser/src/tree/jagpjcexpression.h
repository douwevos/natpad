/*
   File:    jagpjcexpression.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 3, 2017
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

#ifndef TREE_JAGPJCEXPRESSION_H_
#define TREE_JAGPJCEXPRESSION_H_

#include "jagpjctree.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCEXPRESSION              (jagp_jcexpression_get_type())
#define JAGP_JCEXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcexpression_get_type(), JagPJCExpression))
#define JAGP_JCEXPRESSION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCEXPRESSION, JagPJCExpressionClass))
#define JAGP_IS_JCEXPRESSION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCEXPRESSION))
#define JAGP_IS_JCEXPRESSION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCEXPRESSION))
#define JAGP_JCEXPRESSION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCEXPRESSION, JagPJCExpressionClass))


typedef struct _JagPJCExpression               JagPJCExpression;
typedef struct _JagPJCExpressionClass          JagPJCExpressionClass;


struct _JagPJCExpression {
	JagPJCTree parent;
};

struct _JagPJCExpressionClass {
	JagPJCTreeClass parent_class;
	gboolean (*isPoly)(JagPJCExpression *expression);
	gboolean (*isStandalone)(JagPJCExpression *expression);
};


GType jagp_jcexpression_get_type();

JagPJCExpression *jagp_jcexpression_new();

gboolean jagp_jcexpression_is_poly(JagPJCExpression *expression);
gboolean jagp_jcexpression_is_standalone(JagPJCExpression *expression);

G_END_DECLS

#endif /* TREE_JAGPJCEXPRESSION_H_ */
