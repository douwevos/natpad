/*
   File:    jagpjclambda.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 9, 2017
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

#ifndef TREE_JAGPJCLAMBDA_H_
#define TREE_JAGPJCLAMBDA_H_

#include "jagpjcfunctionalexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCLAMBDA              (jagp_jclambda_get_type())
#define JAGP_JCLAMBDA(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jclambda_get_type(), JagPJCLambda))
#define JAGP_JCLAMBDA_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCLAMBDA, JagPJCLambdaClass))
#define JAGP_IS_JCLAMBDA(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCLAMBDA))
#define JAGP_IS_JCLAMBDA_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCLAMBDA))
#define JAGP_JCLAMBDA_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCLAMBDA, JagPJCLambdaClass))


typedef struct _JagPJCLambda               JagPJCLambda;
typedef struct _JagPJCLambdaClass          JagPJCLambdaClass;


struct _JagPJCLambda {
	JagPJCFunctionalExpression parent;
	CatArrayWo /*JagPJCVariableDecl*/*params;
	JagPJCTree *body;
};

struct _JagPJCLambdaClass {
	JagPJCFunctionalExpressionClass parent_class;
};


GType jagp_jclambda_get_type();

JagPJCLambda *jagp_jclambda_new(CatArrayWo /*JagPJCVariableDecl*/*params, JagPJCTree *body);

G_END_DECLS

#endif /* TREE_JAGPJCLAMBDA_H_ */
