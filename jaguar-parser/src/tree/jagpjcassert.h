/*
   File:    jagpjcassert.h
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

#ifndef TREE_JAGPJCASSERT_H_
#define TREE_JAGPJCASSERT_H_

#include "jagpjcstatement.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCASSERT              (jagp_jcassert_get_type())
#define JAGP_JCASSERT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcassert_get_type(), JagPJCAssert))
#define JAGP_JCASSERT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCASSERT, JagPJCAssertClass))
#define JAGP_IS_JCASSERT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCASSERT))
#define JAGP_IS_JCASSERT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCASSERT))
#define JAGP_JCASSERT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCASSERT, JagPJCAssertClass))


typedef struct _JagPJCAssert               JagPJCAssert;
typedef struct _JagPJCAssertClass          JagPJCAssertClass;


struct _JagPJCAssert {
	JagPJCStatement parent;
	JagPJCExpression *cond;
	JagPJCExpression *detail;
};

struct _JagPJCAssertClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcassert_get_type();

JagPJCAssert *jagp_jcassert_new(JagPJCExpression *cond, JagPJCExpression *detail);

G_END_DECLS

#endif /* TREE_JAGPJCASSERT_H_ */
