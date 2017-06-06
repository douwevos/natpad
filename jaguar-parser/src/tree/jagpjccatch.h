/*
   File:    jagpjccatch.h
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

#ifndef TREE_JAGPJCCATCH_H_
#define TREE_JAGPJCCATCH_H_

#include "jagpjctree.h"
#include "jagpjcvariabledecl.h"
#include "jagpjcblock.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCCATCH              (jagp_jccatch_get_type())
#define JAGP_JCCATCH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jccatch_get_type(), JagPJCCatch))
#define JAGP_JCCATCH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCCATCH, JagPJCCatchClass))
#define JAGP_IS_JCCATCH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCCATCH))
#define JAGP_IS_JCCATCH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCCATCH))
#define JAGP_JCCATCH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCCATCH, JagPJCCatchClass))


typedef struct _JagPJCCatch               JagPJCCatch;
typedef struct _JagPJCCatchClass          JagPJCCatchClass;


struct _JagPJCCatch {
	JagPJCTree parent;
	JagPJCVariableDecl *param;
	JagPJCBlock *body;
};

struct _JagPJCCatchClass {
	JagPJCTreeClass parent_class;
};


GType jagp_jccatch_get_type();

JagPJCCatch *jagp_jccatch_new(JagPJCVariableDecl *param, JagPJCBlock *body);

G_END_DECLS

#endif /* TREE_JAGPJCCATCH_H_ */
