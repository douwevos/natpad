/*
   File:    jagastgenericstype.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 23, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef JAGASTCOMPLEXTYPE_H_
#define JAGASTCOMPLEXTYPE_H_

#include "jagastplaintype.h"
#include "jagasttype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_AST_GENERICS_TYPE              (jag_ast_generics_type_get_type())
#define JAG_AST_GENERICS_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_generics_type_get_type(), JagAstGenericsType))
#define JAG_AST_GENERICS_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AST_GENERICS_TYPE, JagAstGenericsTypeClass))
#define JAG_IS_AST_GENERICS_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AST_GENERICS_TYPE))
#define JAG_IS_AST_GENERICS_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AST_GENERICS_TYPE))
#define JAG_AST_GENERICS_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AST_GENERICS_TYPE, JagAstGenericsTypeClass))
#define JAG_AST_GENERICS_TYPE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_AST_GENERICS_TYPE, JagAstGenericsTypePrivate))

typedef struct _JagAstGenericsType               JagAstGenericsType;
typedef struct _JagAstGenericsTypePrivate        JagAstGenericsTypePrivate;
typedef struct _JagAstGenericsTypeClass          JagAstGenericsTypeClass;


struct _JagAstGenericsType {
	JagAstType parent;
	JagAstGenericsTypePrivate *priv;
};

struct _JagAstGenericsTypeClass {
	JagAstTypeClass parent_class;
};


GType jag_ast_generics_type_get_type();

JagAstGenericsType *jag_ast_generics_type_new(JagAstType *base_type, CatArrayWo *a_generics);

G_END_DECLS


#endif /* JAGASTCOMPLEXTYPE_H_ */
