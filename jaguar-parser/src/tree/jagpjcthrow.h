/*
   File:    jagpjcthrow.h
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

#ifndef TREE_JAGPJCTHROW_H_
#define TREE_JAGPJCTHROW_H_


#include "jagpjcstatement.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCTHROW              (jagp_jcthrow_get_type())
#define JAGP_JCTHROW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcthrow_get_type(), JagPJCThrow))
#define JAGP_JCTHROW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTHROW, JagPJCThrowClass))
#define JAGP_IS_JCTHROW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTHROW))
#define JAGP_IS_JCTHROW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTHROW))
#define JAGP_JCTHROW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTHROW, JagPJCThrowClass))


typedef struct _JagPJCThrow               JagPJCThrow;
typedef struct _JagPJCThrowClass          JagPJCThrowClass;


struct _JagPJCThrow {
	JagPJCStatement parent;
	JagPJCExpression *expr;
};

struct _JagPJCThrowClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcthrow_get_type();

JagPJCThrow *jagp_jcthrow_new(JagPJCExpression *expr);

G_END_DECLS

#endif /* TREE_JAGPJCTHROW_H_ */
