/*
   File:    jagpjctypeparameter.h
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

#ifndef TREE_JAGPJCTYPEPARAMETER_H_
#define TREE_JAGPJCTYPEPARAMETER_H_

#include "../code/jagpname.h"
#include "jagpjctree.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCTYPE_PARAMETER              (jagp_jctype_parameter_get_type())
#define JAGP_JCTYPE_PARAMETER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jctype_parameter_get_type(), JagPJCTypeParameter))
#define JAGP_JCTYPE_PARAMETER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTYPE_PARAMETER, JagPJCTypeParameterClass))
#define JAGP_IS_JCTYPE_PARAMETER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTYPE_PARAMETER))
#define JAGP_IS_JCTYPE_PARAMETER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTYPE_PARAMETER))
#define JAGP_JCTYPE_PARAMETER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTYPE_PARAMETER, JagPJCTypeParameterClass))


typedef struct _JagPJCTypeParameter               JagPJCTypeParameter;
typedef struct _JagPJCTypeParameterClass          JagPJCTypeParameterClass;


struct _JagPJCTypeParameter {
	JagPJCTree parent;
	JagPName *name;
	CatArrayWo /*<JCExpression>*/ *bounds;
	CatArrayWo /*<JCAnnotation>*/ *annotations;
};

struct _JagPJCTypeParameterClass {
	JagPJCTreeClass parent_class;
};


GType jagp_jctype_parameter_get_type();

JagPJCTypeParameter *jagp_jctype_parameter_new(JagPName *name, CatArrayWo /*<JCExpression>*/ *bounds, CatArrayWo /*<JCAnnotation>*/ *annotations);

G_END_DECLS

#endif /* TREE_JAGPJCTYPEPARAMETER_H_ */
