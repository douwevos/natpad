/*
   File:    jagpjcreturn.h
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

#ifndef TREE_JAGPJCRETURN_H_
#define TREE_JAGPJCRETURN_H_

#include "jagpjcstatement.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCRETURN              (jagp_jcreturn_get_type())
#define JAGP_JCRETURN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcreturn_get_type(), JagPJCReturn))
#define JAGP_JCRETURN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCRETURN, JagPJCReturnClass))
#define JAGP_IS_JCRETURN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCRETURN))
#define JAGP_IS_JCRETURN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCRETURN))
#define JAGP_JCRETURN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCRETURN, JagPJCReturnClass))


typedef struct _JagPJCReturn               JagPJCReturn;
typedef struct _JagPJCReturnClass          JagPJCReturnClass;


struct _JagPJCReturn {
	JagPJCStatement parent;
	JagPJCExpression *expr;
};

struct _JagPJCReturnClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcreturn_get_type();

JagPJCReturn *jagp_jcreturn_new(JagPJCExpression *expr);

G_END_DECLS

#endif /* TREE_JAGPJCRETURN_H_ */
