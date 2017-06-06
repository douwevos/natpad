/*
   File:    jagpjcpackagedecl.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 11, 2017
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

#ifndef TREE_JAGPJCPACKAGEDECL_H_
#define TREE_JAGPJCPACKAGEDECL_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCPACKAGE_DECL              (jagp_jcpackage_decl_get_type())
#define JAGP_JCPACKAGE_DECL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcpackage_decl_get_type(), JagPJCPackageDecl))
#define JAGP_JCPACKAGE_DECL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCPACKAGE_DECL, JagPJCPackageDeclClass))
#define JAGP_IS_JCPACKAGE_DECL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCPACKAGE_DECL))
#define JAGP_IS_JCPACKAGE_DECL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCPACKAGE_DECL))
#define JAGP_JCPACKAGE_DECL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCPACKAGE_DECL, JagPJCPackageDeclClass))


typedef struct _JagPJCPackageDecl               JagPJCPackageDecl;
typedef struct _JagPJCPackageDeclClass          JagPJCPackageDeclClass;


struct _JagPJCPackageDecl {
	JagPJCTree parent;
	CatArrayWo /*<JagPJCAnnotation>*/ *annotations;
	JagPJCExpression *pid;
};

struct _JagPJCPackageDeclClass {
	JagPJCTreeClass parent_class;
};


GType jagp_jcpackage_decl_get_type();

JagPJCPackageDecl *jagp_jcpackage_decl_new(CatArrayWo /*<JagPJCAnnotation>*/ *annotations, JagPJCExpression *pid);

G_END_DECLS

#endif /* TREE_JAGPJCPACKAGEDECL_H_ */
