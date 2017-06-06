/*
   File:    jagpjctypecast.h
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

#ifndef TREE_JAGPJCTYPECAST_H_
#define TREE_JAGPJCTYPECAST_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCTYPE_CAST              (jagp_jctype_cast_get_type())
#define JAGP_JCTYPE_CAST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jctype_cast_get_type(), JagPJCTypeCast))
#define JAGP_JCTYPE_CAST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTYPE_CAST, JagPJCTypeCastClass))
#define JAGP_IS_JCTYPE_CAST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTYPE_CAST))
#define JAGP_IS_JCTYPE_CAST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTYPE_CAST))
#define JAGP_JCTYPE_CAST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTYPE_CAST, JagPJCTypeCastClass))


typedef struct _JagPJCTypeCast               JagPJCTypeCast;
typedef struct _JagPJCTypeCastClass          JagPJCTypeCastClass;


struct _JagPJCTypeCast {
	JagPJCExpression parent;
	JagPJCTree *clazz;
	JagPJCExpression *expr;
};

struct _JagPJCTypeCastClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jctype_cast_get_type();

JagPJCTypeCast *jagp_jctype_cast_new(JagPJCTree *clazz, JagPJCExpression *expr);

G_END_DECLS

#endif /* TREE_JAGPJCTYPECAST_H_ */
