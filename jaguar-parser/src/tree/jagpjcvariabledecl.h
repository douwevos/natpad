/*
   File:    jagpjcvariabledecl.h
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

#ifndef TREE_JAGPJCVARIABLEDECL_H_
#define TREE_JAGPJCVARIABLEDECL_H_

#include "jagpjcstatement.h"
#include "jagpjcmodifiers.h"
#include "jagpjcexpression.h"
#include "../code/jagpname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCVARIABLE_DECL              (jagp_jcvariable_decl_get_type())
#define JAGP_JCVARIABLE_DECL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcvariable_decl_get_type(), JagPJCVariableDecl))
#define JAGP_JCVARIABLE_DECL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCVARIABLE_DECL, JagPJCVariableDeclClass))
#define JAGP_IS_JCVARIABLE_DECL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCVARIABLE_DECL))
#define JAGP_IS_JCVARIABLE_DECL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCVARIABLE_DECL))
#define JAGP_JCVARIABLE_DECL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCVARIABLE_DECL, JagPJCVariableDeclClass))


typedef struct _JagPJCVariableDecl               JagPJCVariableDecl;
typedef struct _JagPJCVariableDeclClass          JagPJCVariableDeclClass;


struct _JagPJCVariableDecl {
	JagPJCStatement parent;
	JagPJCModifiers *mods;
	JagPName *name;
	JagPJCExpression *nameexpr;
	JagPJCExpression *vartype;
	JagPJCExpression *init;
	/*, VarSymbol sym*/
};

struct _JagPJCVariableDeclClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcvariable_decl_get_type();

JagPJCVariableDecl *jagp_jcvariable_decl_new(JagPJCModifiers *mods, JagPName *name, JagPJCExpression *nameexpr, JagPJCExpression *vartype, JagPJCExpression *init/*, VarSymbol sym*/);

G_END_DECLS

#endif /* TREE_JAGPJCVARIABLEDECL_H_ */
