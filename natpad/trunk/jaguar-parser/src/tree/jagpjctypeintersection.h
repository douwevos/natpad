/*
   File:    jagpjctypeintersection.h
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

#ifndef TREE_JAGPJCTYPEINTERSECTION_H_
#define TREE_JAGPJCTYPEINTERSECTION_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCTYPE_INTERSECTION              (jagp_jctype_intersection_get_type())
#define JAGP_JCTYPE_INTERSECTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jctype_intersection_get_type(), JagPJCTypeIntersection))
#define JAGP_JCTYPE_INTERSECTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTYPE_INTERSECTION, JagPJCTypeIntersectionClass))
#define JAGP_IS_JCTYPE_INTERSECTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTYPE_INTERSECTION))
#define JAGP_IS_JCTYPE_INTERSECTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTYPE_INTERSECTION))
#define JAGP_JCTYPE_INTERSECTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTYPE_INTERSECTION, JagPJCTypeIntersectionClass))


typedef struct _JagPJCTypeIntersection               JagPJCTypeIntersection;
typedef struct _JagPJCTypeIntersectionClass          JagPJCTypeIntersectionClass;


struct _JagPJCTypeIntersection {
	JagPJCExpression parent;
	CatArrayWo *bounds;
};

struct _JagPJCTypeIntersectionClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jctype_intersection_get_type();

JagPJCTypeIntersection *jagp_jctype_intersection_new(CatArrayWo *bounds);

G_END_DECLS

#endif /* TREE_JAGPJCTYPEINTERSECTION_H_ */
