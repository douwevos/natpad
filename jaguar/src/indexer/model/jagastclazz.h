/*
   File:    jagastclazz.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 20, 2012
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

#ifndef JAGASTCLAZZ_H_
#define JAGASTCLAZZ_H_

#include "jagnametype.h"
#include "jagastdeclarator.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_AST_CLAZZ              (jag_ast_clazz_get_type())
#define JAG_AST_CLAZZ(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_clazz_get_type(), JagAstClazz))
#define JAG_AST_CLAZZ_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AST_CLAZZ, JagAstClazzClass))
#define JAG_IS_AST_CLAZZ(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AST_CLAZZ))
#define JAG_IS_AST_CLAZZ_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AST_CLAZZ))
#define JAG_AST_CLAZZ_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AST_CLAZZ, JagAstClazzClass))
#define JAG_AST_CLAZZ_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_AST_CLAZZ, JagAstClazzPrivate))

typedef struct _JagAstClazz               JagAstClazz;
typedef struct _JagAstClazzPrivate        JagAstClazzPrivate;
typedef struct _JagAstClazzClass          JagAstClazzClass;


struct _JagAstClazz {
	GObject parent;
	JagAstClazzPrivate *priv;
};

struct _JagAstClazzClass {
	GObjectClass parent_class;
};


GType jag_ast_clazz_get_type();

void jag_ast_clazz_construct(JagAstClazz *ast_clazz, JagNameType *name_type);

CatStringWo *jag_ast_clazz_get_name(JagAstClazz *ast_clazz);


void jag_ast_clazz_add_field(JagAstClazz *ast_clazz, JagAstDeclarator *field_declarator);

CatArrayWo *jag_ast_clazz_enlist_fields(JagAstClazz *ast_clazz);

G_END_DECLS

#endif /* JAGASTCLAZZ_H_ */
