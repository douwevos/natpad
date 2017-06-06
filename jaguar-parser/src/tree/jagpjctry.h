/*
   File:    jagpjctry.h
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

#ifndef TREE_JAGPJCTRY_H_
#define TREE_JAGPJCTRY_H_

#include "jagpjcstatement.h"
#include "jagpjcblock.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCTRY              (jagp_jctry_get_type())
#define JAGP_JCTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jctry_get_type(), JagPJCTry))
#define JAGP_JCTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTRY, JagPJCTryClass))
#define JAGP_IS_JCTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTRY))
#define JAGP_IS_JCTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTRY))
#define JAGP_JCTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTRY, JagPJCTryClass))


typedef struct _JagPJCTry               JagPJCTry;
typedef struct _JagPJCTryClass          JagPJCTryClass;


struct _JagPJCTry {
	JagPJCStatement parent;
	CatArrayWo /*<JagPJCTree>*/ *resources;
	JagPJCBlock *body;
	CatArrayWo /*<JCCatch>*/ *catchers;
	JagPJCBlock *finalizer;
};

struct _JagPJCTryClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jctry_get_type();

JagPJCTry *jagp_jctry_new(CatArrayWo /*<JagPJCTree>*/ *resources, JagPJCBlock *body, CatArrayWo /*<JCCatch>*/ *catchers, JagPJCBlock *finalizer);

G_END_DECLS

#endif /* TREE_JAGPJCTRY_H_ */
