/*
   File:    jagastdeclarator.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 25, 2012
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


#ifndef JAGASTDECLARATOR_H_
#define JAGASTDECLARATOR_H_

#include "jagasttype.h"
#include "jagastindexermodifiers.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_AST_DECLARATOR              (jag_ast_declarator_get_type())
#define JAG_AST_DECLARATOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_declarator_get_type(), JagAstDeclarator))
#define JAG_AST_DECLARATOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AST_DECLARATOR, JagAstDeclaratorClass))
#define JAG_IS_AST_DECLARATOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AST_DECLARATOR))
#define JAG_IS_AST_DECLARATOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AST_DECLARATOR))
#define JAG_AST_DECLARATOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AST_DECLARATOR, JagAstDeclaratorClass))
#define JAG_AST_DECLARATOR_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_AST_DECLARATOR, JagAstDeclaratorPrivate))

typedef struct _JagAstDeclarator               JagAstDeclarator;
typedef struct _JagAstDeclaratorPrivate        JagAstDeclaratorPrivate;
typedef struct _JagAstDeclaratorClass          JagAstDeclaratorClass;


struct _JagAstDeclarator {
	GObject parent;
	JagAstDeclaratorPrivate *priv;
};

struct _JagAstDeclaratorClass {
	GObjectClass parent_class;
};


GType jag_ast_declarator_get_type();

JagAstDeclarator *jag_ast_declarator_new(CatStringWo *a_id);


void jag_ast_declarator_add_dimension(JagAstDeclarator *declarator);

void jag_ast_declarator_set_base_type(JagAstDeclarator *declarator, JagAstType *base_type);

void jag_ast_declarator_set_modifiers(JagAstDeclarator *declarator, JagAstIndexerModifiers *modifiers);

CatStringWo *jag_ast_declarator_get_id(JagAstDeclarator *declarator);

JagAstIndexerModifiers *jag_ast_declarator_get_modifiers(JagAstDeclarator *declarator);

G_END_DECLS


#endif /* JAGASTDECLARATOR_H_ */
