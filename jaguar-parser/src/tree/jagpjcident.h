/*
   File:    jagpjcident.h
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

#ifndef TREE_JAGPJCIDENT_H_
#define TREE_JAGPJCIDENT_H_

#include "../code/jagpname.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCIDENT              (jagp_jcident_get_type())
#define JAGP_JCIDENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcident_get_type(), JagPJCIdent))
#define JAGP_JCIDENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCIDENT, JagPJCIdentClass))
#define JAGP_IS_JCIDENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCIDENT))
#define JAGP_IS_JCIDENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCIDENT))
#define JAGP_JCIDENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCIDENT, JagPJCIdentClass))


typedef struct _JagPJCIdent               JagPJCIdent;
typedef struct _JagPJCIdentClass          JagPJCIdentClass;


struct _JagPJCIdent {
	JagPJCExpression parent;
	JagPName *name;
};

struct _JagPJCIdentClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcident_get_type();

JagPJCIdent *jagp_jcident_new(JagPName *name);

G_END_DECLS

#endif /* TREE_JAGPJCIDENT_H_ */
