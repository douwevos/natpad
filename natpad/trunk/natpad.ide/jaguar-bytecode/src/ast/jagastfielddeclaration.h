/*
   File:    jagastfielddeclaration.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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


#ifndef JAGASTFIELDDECLARATION_H_
#define JAGASTFIELDDECLARATION_H_

#include "jagastmodifiers.h"
#include "jagastdeclarationtype.h"
#include "jagastidentifier.h"
#include "jagastiexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_FIELD_DECLARATION              (jag_ast_field_declaration_get_type())
#define JAG_AST_FIELD_DECLARATION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_field_declaration_get_type(), JagAstFieldDeclaration))
#define JAG_AST_FIELD_DECLARATION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_FIELD_DECLARATION, JagAstFieldDeclarationClass))
#define JAG_AST_IS_FIELD_DECLARATION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_FIELD_DECLARATION))
#define JAG_AST_IS_FIELD_DECLARATION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_FIELD_DECLARATION))
#define JAG_AST_FIELD_DECLARATION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_FIELD_DECLARATION, JagAstFieldDeclarationClass))
#define JAG_AST_FIELD_DECLARATION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_FIELD_DECLARATION, JagAstFieldDeclarationPrivate))

typedef struct _JagAstFieldDeclaration               JagAstFieldDeclaration;
typedef struct _JagAstFieldDeclarationPrivate        JagAstFieldDeclarationPrivate;
typedef struct _JagAstFieldDeclarationClass          JagAstFieldDeclarationClass;


struct _JagAstFieldDeclaration {
	GObject parent;
	JagAstFieldDeclarationPrivate *priv;
};

struct _JagAstFieldDeclarationClass {
	GObjectClass parent_class;
};


GType jag_ast_field_declaration_get_type();

JagAstFieldDeclaration *jag_ast_field_declaration_new(JagAstModifiers *modifiers, JagAstDeclarationType *fieldDescriptor, JagAstIdentifier *variableName, JagAstIExpression *value);

JagAstIdentifier *jag_ast_field_declaration_get_variable(JagAstFieldDeclaration *field_declaration);

void jag_ast_field_declaration_write(JagAstFieldDeclaration *field_declaration, JagAstWriter *out);


G_END_DECLS


#endif /* JAGASTFIELDDECLARATION_H_ */
