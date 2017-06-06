/*
   File:    jagpjctypeunion.h
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

#ifndef TREE_JAGPJCTYPEUNION_H_
#define TREE_JAGPJCTYPEUNION_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCTYPE_UNION              (jagp_jctype_union_get_type())
#define JAGP_JCTYPE_UNION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jctype_union_get_type(), JagPJCTypeUnion))
#define JAGP_JCTYPE_UNION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTYPE_UNION, JagPJCTypeUnionClass))
#define JAGP_IS_JCTYPE_UNION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTYPE_UNION))
#define JAGP_IS_JCTYPE_UNION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTYPE_UNION))
#define JAGP_JCTYPE_UNION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTYPE_UNION, JagPJCTypeUnionClass))


typedef struct _JagPJCTypeUnion               JagPJCTypeUnion;
typedef struct _JagPJCTypeUnionClass          JagPJCTypeUnionClass;


struct _JagPJCTypeUnion {
	JagPJCExpression parent;
	CatArrayWo /*<JagPJCExpression>*/ *alternatives;
};

struct _JagPJCTypeUnionClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jctype_union_get_type();

JagPJCTypeUnion *jagp_jctype_union_new(CatArrayWo /*<JagPJCExpression>*/ *alternatives);

G_END_DECLS

#endif /* TREE_JAGPJCTYPEUNION_H_ */
