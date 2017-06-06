/*
   File:    jagpjcannotation.h
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

#ifndef TREE_JAGPJCANNOTATION_H_
#define TREE_JAGPJCANNOTATION_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCANNOTATION              (jagp_jcannotation_get_type())
#define JAGP_JCANNOTATION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcannotation_get_type(), JagPJCAnnotation))
#define JAGP_JCANNOTATION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCANNOTATION, JagPJCAnnotationClass))
#define JAGP_IS_JCANNOTATION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCANNOTATION))
#define JAGP_IS_JCANNOTATION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCANNOTATION))
#define JAGP_JCANNOTATION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCANNOTATION, JagPJCAnnotationClass))


typedef struct _JagPJCAnnotation               JagPJCAnnotation;
typedef struct _JagPJCAnnotationClass          JagPJCAnnotationClass;


struct _JagPJCAnnotation {
	JagPJCExpression parent;
	gboolean is_type_annotation;
	JagPJCTree *annotation_type;
	CatArrayWo *args;	// <JagPJCExpression *>
};

struct _JagPJCAnnotationClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcannotation_get_type();

JagPJCAnnotation *jagp_jcannotation_new(gboolean is_type_annotation, JagPJCTree *annotation_type, CatArrayWo *args);

G_END_DECLS

#endif /* TREE_JAGPJCANNOTATION_H_ */
