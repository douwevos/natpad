/*
   File:    jagpjcfieldaccess.h
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

#ifndef TREE_JAGPJCFIELDACCESS_H_
#define TREE_JAGPJCFIELDACCESS_H_

#include "../code/jagpname.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCFIELD_ACCESS              (jagp_jcfield_access_get_type())
#define JAGP_JCFIELD_ACCESS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcfield_access_get_type(), JagPJCFieldAccess))
#define JAGP_JCFIELD_ACCESS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCFIELD_ACCESS, JagPJCFieldAccessClass))
#define JAGP_IS_JCFIELD_ACCESS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCFIELD_ACCESS))
#define JAGP_IS_JCFIELD_ACCESS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCFIELD_ACCESS))
#define JAGP_JCFIELD_ACCESS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCFIELD_ACCESS, JagPJCFieldAccessClass))


typedef struct _JagPJCFieldAccess               JagPJCFieldAccess;
typedef struct _JagPJCFieldAccessClass          JagPJCFieldAccessClass;


struct _JagPJCFieldAccess {
	JagPJCExpression parent;
	JagPJCExpression *selected;
	JagPJCTree *name;
};

struct _JagPJCFieldAccessClass {
	JagPJCExpressionClass parent_class;
};

GType jagp_jcfield_access_get_type();

JagPJCFieldAccess *jagp_jcfield_access_new(JagPJCExpression *selected, JagPJCTree *name);

G_END_DECLS

#endif /* TREE_JAGPJCFIELDACCESS_H_ */
