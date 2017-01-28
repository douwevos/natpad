/*
   File:    jagasttype.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 24, 2012
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


#ifndef JAGASTTYPE_H_
#define JAGASTTYPE_H_


#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_AST_TYPE              (jag_ast_type_get_type())
#define JAG_AST_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_type_get_type(), JagAstType))
#define JAG_AST_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AST_TYPE, JagAstTypeClass))
#define JAG_IS_AST_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AST_TYPE))
#define JAG_IS_AST_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AST_TYPE))
#define JAG_AST_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AST_TYPE, JagAstTypeClass))
#define JAG_AST_TYPE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_AST_TYPE, JagAstTypePrivate))

typedef struct _JagAstType               JagAstType;
typedef struct _JagAstTypePrivate        JagAstTypePrivate;
typedef struct _JagAstTypeClass          JagAstTypeClass;


struct _JagAstType {
	GObject parent;
	JagAstTypePrivate *priv;
};

struct _JagAstTypeClass {
	GObjectClass parent_class;
	CatStringWo *(*toString)(const JagAstType *ast_type);
};


GType jag_ast_type_get_type();

void jag_ast_type_construct(JagAstType *ast_type);


G_END_DECLS


#endif /* JAGASTTYPE_H_ */
