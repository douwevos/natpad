/*
   File:    jagpjcwildcard.h
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

#ifndef TREE_JAGPJCWILDCARD_H_
#define TREE_JAGPJCWILDCARD_H_

#include "jagpjcexpression.h"
#include "jagpjctree.h"
#include "jagptypeboundkind.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCWILDCARD              (jagp_jcwildcard_get_type())
#define JAGP_JCWILDCARD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcwildcard_get_type(), JagPJCWildcard))
#define JAGP_JCWILDCARD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCWILDCARD, JagPJCWildcardClass))
#define JAGP_IS_JCWILDCARD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCWILDCARD))
#define JAGP_IS_JCWILDCARD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCWILDCARD))
#define JAGP_JCWILDCARD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCWILDCARD, JagPJCWildcardClass))


typedef struct _JagPJCWildcard               JagPJCWildcard;
typedef struct _JagPJCWildcardPrivate        JagPJCWildcardPrivate;
typedef struct _JagPJCWildcardClass          JagPJCWildcardClass;


struct _JagPJCWildcard {
	JagPJCExpression parent;
	JagPTypeBoundKind *kind;
	JagPJCTree *inner;
};

struct _JagPJCWildcardClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcwildcard_get_type();

JagPJCWildcard *jagp_jcwildcard_new(JagPTypeBoundKind *kind, JagPJCTree *inner);

G_END_DECLS

#endif /* TREE_JAGPJCWILDCARD_H_ */
