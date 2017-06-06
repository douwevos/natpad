/*
   File:    jagpjcannotatedtype.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 7, 2017
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

#ifndef TREE_JAGPJCANNOTATEDTYPE_H_
#define TREE_JAGPJCANNOTATEDTYPE_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCANNOTATED_TYPE              (jagp_jcannotated_type_get_type())
#define JAGP_JCANNOTATED_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcannotated_type_get_type(), JagPJCAnnotatedType))
#define JAGP_JCANNOTATED_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCANNOTATED_TYPE, JagPJCAnnotatedTypeClass))
#define JAGP_IS_JCANNOTATED_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCANNOTATED_TYPE))
#define JAGP_IS_JCANNOTATED_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCANNOTATED_TYPE))
#define JAGP_JCANNOTATED_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCANNOTATED_TYPE, JagPJCAnnotatedTypeClass))


typedef struct _JagPJCAnnotatedType               JagPJCAnnotatedType;
typedef struct _JagPJCAnnotatedTypeClass          JagPJCAnnotatedTypeClass;


struct _JagPJCAnnotatedType {
	JagPJCExpression parent;
	CatArrayWo /*JagPJCAnnotation*/ *annotations;
	JagPJCExpression *underlying_type;
};

struct _JagPJCAnnotatedTypeClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcannotated_type_get_type();

JagPJCAnnotatedType *jagp_jcannotated_type_new(CatArrayWo *annotations, JagPJCExpression *underlying_type);

G_END_DECLS

#endif /* TREE_JAGPJCANNOTATEDTYPE_H_ */
