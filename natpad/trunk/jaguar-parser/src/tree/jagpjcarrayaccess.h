/*
   File:    jagpjcarrayaccess.h
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

#ifndef TREE_JAGPJCARRAYACCESS_H_
#define TREE_JAGPJCARRAYACCESS_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCARRAY_ACCESS              (jagp_jcarray_access_get_type())
#define JAGP_JCARRAY_ACCESS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcarray_access_get_type(), JagPJCArrayAccess))
#define JAGP_JCARRAY_ACCESS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCARRAY_ACCESS, JagPJCArrayAccessClass))
#define JAGP_IS_JCARRAY_ACCESS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCARRAY_ACCESS))
#define JAGP_IS_JCARRAY_ACCESS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCARRAY_ACCESS))
#define JAGP_JCARRAY_ACCESS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCARRAY_ACCESS, JagPJCArrayAccessClass))


typedef struct _JagPJCArrayAccess               JagPJCArrayAccess;
typedef struct _JagPJCArrayAccessClass          JagPJCArrayAccessClass;


struct _JagPJCArrayAccess {
	JagPJCExpression parent;
	JagPJCExpression *indexed;
	JagPJCExpression *index;
};

struct _JagPJCArrayAccessClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcarray_access_get_type();

JagPJCArrayAccess *jagp_jcarray_access_new(JagPJCExpression *indexed, JagPJCExpression *index);

G_END_DECLS

#endif /* TREE_JAGPJCARRAYACCESS_H_ */
