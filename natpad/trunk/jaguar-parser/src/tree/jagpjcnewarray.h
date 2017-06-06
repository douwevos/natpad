/*
   File:    jagpjcnewarray.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 13, 2017
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

#ifndef TREE_JAGPJCNEWARRAY_H_
#define TREE_JAGPJCNEWARRAY_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCNEW_ARRAY              (jagp_jcnew_array_get_type())
#define JAGP_JCNEW_ARRAY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcnew_array_get_type(), JagPJCNewArray))
#define JAGP_JCNEW_ARRAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCNEW_ARRAY, JagPJCNewArrayClass))
#define JAGP_IS_JCNEW_ARRAY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCNEW_ARRAY))
#define JAGP_IS_JCNEW_ARRAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCNEW_ARRAY))
#define JAGP_JCNEW_ARRAY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCNEW_ARRAY, JagPJCNewArrayClass))


typedef struct _JagPJCNewArray               JagPJCNewArray;
typedef struct _JagPJCNewArrayClass          JagPJCNewArrayClass;


struct _JagPJCNewArray {
	JagPJCExpression parent;
	JagPJCExpression *elemtype;
	CatArrayWo /*<JCExpression>*/ *dims;

	// type annotations on inner-most component
    CatArrayWo /*<JCAnnotation>*/ *annotations;
    // type annotations on dimensions
    CatArrayWo /*<List<JCAnnotation>>*/ *dimAnnotations;

	CatArrayWo /*<JCExpression>*/ *elems;
};

struct _JagPJCNewArrayClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcnew_array_get_type();

JagPJCNewArray *jagp_jcnew_array_new(JagPJCExpression *elemtype, CatArrayWo /*<JCExpression>*/ *dims, CatArrayWo /*<JCExpression>*/ *elems);

G_END_DECLS

#endif /* TREE_JAGPJCNEWARRAY_H_ */
