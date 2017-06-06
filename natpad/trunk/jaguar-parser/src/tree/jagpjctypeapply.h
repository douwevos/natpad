/*
   File:    jagpjctypeapply.h
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

#ifndef TREE_JAGPJCTYPEAPPLY_H_
#define TREE_JAGPJCTYPEAPPLY_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCTYPE_APPLY              (jagp_jctype_apply_get_type())
#define JAGP_JCTYPE_APPLY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jctype_apply_get_type(), JagPJCTypeApply))
#define JAGP_JCTYPE_APPLY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTYPE_APPLY, JagPJCTypeApplyClass))
#define JAGP_IS_JCTYPE_APPLY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTYPE_APPLY))
#define JAGP_IS_JCTYPE_APPLY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTYPE_APPLY))
#define JAGP_JCTYPE_APPLY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTYPE_APPLY, JagPJCTypeApplyClass))


typedef struct _JagPJCTypeApply               JagPJCTypeApply;
typedef struct _JagPJCTypeApplyClass          JagPJCTypeApplyClass;


struct _JagPJCTypeApply {
	JagPJCExpression parent;
    JagPJCExpression *clazz;
    CatArrayWo */*JagPJCExpression*/ arguments;
};

struct _JagPJCTypeApplyClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jctype_apply_get_type();

JagPJCTypeApply *jagp_jctype_apply_new(JagPJCExpression *clazz, CatArrayWo *arguments);

G_END_DECLS

#endif /* TREE_JAGPJCTYPEAPPLY_H_ */
