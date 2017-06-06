/*
   File:    jagpjcprimitivetypetree.h
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

#ifndef TREE_JAGPJCPRIMITIVETYPETREE_H_
#define TREE_JAGPJCPRIMITIVETYPETREE_H_

#include "jagpjcexpression.h"
#include "../code/jagptypetag.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCPRIMITIVE_TYPE_TREE              (jagp_jcprimitive_type_tree_get_type())
#define JAGP_JCPRIMITIVE_TYPE_TREE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcprimitive_type_tree_get_type(), JagPJCPrimitiveTypeTree))
#define JAGP_JCPRIMITIVE_TYPE_TREE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCPRIMITIVE_TYPE_TREE, JagPJCPrimitiveTypeTreeClass))
#define JAGP_IS_JCPRIMITIVE_TYPE_TREE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCPRIMITIVE_TYPE_TREE))
#define JAGP_IS_JCPRIMITIVE_TYPE_TREE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCPRIMITIVE_TYPE_TREE))
#define JAGP_JCPRIMITIVE_TYPE_TREE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCPRIMITIVE_TYPE_TREE, JagPJCPrimitiveTypeTreeClass))


typedef struct _JagPJCPrimitiveTypeTree               JagPJCPrimitiveTypeTree;
typedef struct _JagPJCPrimitiveTypeTreeClass          JagPJCPrimitiveTypeTreeClass;


struct _JagPJCPrimitiveTypeTree {
	JagPJCExpression parent;
	JagPTypeTag type_tag;
};

struct _JagPJCPrimitiveTypeTreeClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcprimitive_type_tree_get_type();

JagPJCPrimitiveTypeTree *jagp_jcprimitive_type_tree_new(JagPTypeTag type_tag);

G_END_DECLS

#endif /* TREE_JAGPJCPRIMITIVETYPETREE_H_ */
