/*
   File:    jagpjcstatement.h
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

#ifndef TREE_JAGPJCSTATEMENT_H_
#define TREE_JAGPJCSTATEMENT_H_

#include "jagpjctree.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCSTATEMENT              (jagp_jcstatement_get_type())
#define JAGP_JCSTATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcstatement_get_type(), JagPJCStatement))
#define JAGP_JCSTATEMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCSTATEMENT, JagPJCStatementClass))
#define JAGP_IS_JCSTATEMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCSTATEMENT))
#define JAGP_IS_JCSTATEMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCSTATEMENT))
#define JAGP_JCSTATEMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCSTATEMENT, JagPJCStatementClass))


typedef struct _JagPJCStatement               JagPJCStatement;
typedef struct _JagPJCStatementPrivate        JagPJCStatementPrivate;
typedef struct _JagPJCStatementClass          JagPJCStatementClass;


struct _JagPJCStatement {
	JagPJCTree parent;
};

struct _JagPJCStatementClass {
	JagPJCTreeClass parent_class;
};


GType jagp_jcstatement_get_type();

JagPJCStatement *jagp_jcstatement_new();

G_END_DECLS

#endif /* TREE_JAGPJCSTATEMENT_H_ */
