/*
   File:    jagpjcclassdecl.h
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

#ifndef TREE_JAGPJCCLASSDECL_H_
#define TREE_JAGPJCCLASSDECL_H_

#include "jagpjcstatement.h"
#include "jagpjcexpression.h"
#include "jagpjcmodifiers.h"
#include "../code/jagpname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCCLASS_DECL              (jagp_jcclass_decl_get_type())
#define JAGP_JCCLASS_DECL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcclass_decl_get_type(), JagPJCClassDecl))
#define JAGP_JCCLASS_DECL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCCLASS_DECL, JagPJCClassDeclClass))
#define JAGP_IS_JCCLASS_DECL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCCLASS_DECL))
#define JAGP_IS_JCCLASS_DECL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCCLASS_DECL))
#define JAGP_JCCLASS_DECL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCCLASS_DECL, JagPJCClassDeclClass))


typedef struct _JagPJCClassDecl               JagPJCClassDecl;
typedef struct _JagPJCClassDeclClass          JagPJCClassDeclClass;


struct _JagPJCClassDecl {
	JagPJCStatement parent;
	JagPJCModifiers *modifiers;
	JagPName *name;
	CatArrayWo *typarams;
	JagPJCExpression *extending;
	CatArrayWo *implementing;
	CatArrayWo *defs;
};

struct _JagPJCClassDeclClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcclass_decl_get_type();

JagPJCClassDecl *jagp_jcclass_decl_new(JagPJCModifiers *modifiers, JagPName *name, CatArrayWo *typarams, JagPJCExpression *extending, CatArrayWo *implementing, CatArrayWo *defs /*, ClassSymbol*/);

G_END_DECLS

#endif /* TREE_JAGPJCCLASSDECL_H_ */
