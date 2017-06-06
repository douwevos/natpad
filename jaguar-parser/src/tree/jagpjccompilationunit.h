/*
   File:    jagpjcompilationunit.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 1, 2017
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

#ifndef TREE_JAGPJCCOMPILATIONUNIT_H_
#define TREE_JAGPJCCOMPILATIONUNIT_H_

#include "jagpjctree.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCCOMPILATION_UNIT              (jagp_jccompilation_unit_get_type())
#define JAGP_JCCOMPILATION_UNIT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jccompilation_unit_get_type(), JagPJCCompilationUnit))
#define JAGP_JCCOMPILATION_UNIT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCCOMPILATION_UNIT, JagPJCCompilationUnitClass))
#define JAGP_IS_JCCOMPILATION_UNIT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCCOMPILATION_UNIT))
#define JAGP_IS_JCCOMPILATION_UNIT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCCOMPILATION_UNIT))
#define JAGP_JCCOMPILATION_UNIT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCCOMPILATION_UNIT, JagPJCCompilationUnitClass))


typedef struct _JagPJCCompilationUnit               JagPJCCompilationUnit;
typedef struct _JagPJCCompilationUnitClass          JagPJCCompilationUnitClass;


struct _JagPJCCompilationUnit {
	JagPJCTree parent;
	CatArrayWo /*<JagPJCTree>*/ *defs;
};

struct _JagPJCCompilationUnitClass {
	JagPJCTreeClass parent_class;
};


GType jagp_jccompilation_unit_get_type();

JagPJCCompilationUnit *jagp_jccompilation_unit_new(CatArrayWo /*<JagPJCTree>*/ *defs);

G_END_DECLS

#endif /* TREE_JAGPJCCOMPILATIONUNIT_H_ */
