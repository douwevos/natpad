/*
   File:    jagastdeclarationtype.h
   Project: jaguar-bytecode
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


#ifndef JAGASTDECLARATIONTYPE_H_
#define JAGASTDECLARATIONTYPE_H_

#include "../classfile/bytecode/jagbyttype.h"
#include "../classfile/jagbytname.h"
#include "formater/jagastwriter.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_DECLARATION_TYPE              (jag_ast_declaration_type_get_type())
#define JAG_AST_DECLARATION_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_declaration_type_get_type(), JagAstDeclarationType))
#define JAG_AST_DECLARATION_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_DECLARATION_TYPE, JagAstDeclarationTypeClass))
#define JAG_AST_IS_DECLARATION_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_DECLARATION_TYPE))
#define JAG_AST_IS_DECLARATION_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_DECLARATION_TYPE))
#define JAG_AST_DECLARATION_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_DECLARATION_TYPE, JagAstDeclarationTypeClass))
#define JAG_AST_DECLARATION_TYPE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_DECLARATION_TYPE, JagAstDeclarationTypePrivate))

typedef struct _JagAstDeclarationType               JagAstDeclarationType;
typedef struct _JagAstDeclarationTypePrivate        JagAstDeclarationTypePrivate;
typedef struct _JagAstDeclarationTypeClass          JagAstDeclarationTypeClass;


struct _JagAstDeclarationType {
	GObject parent;
	JagAstDeclarationTypePrivate *priv;
};

struct _JagAstDeclarationTypeClass {
	GObjectClass parent_class;
};


GType jag_ast_declaration_type_get_type();


JagAstDeclarationType *jag_ast_declaration_type_new(JagBytType primitive_type, int dimCount);

JagAstDeclarationType *jag_ast_declaration_type_new_reference(JagBytName *type_name, int dimCount);


JagAstDeclarationType *jag_ast_declaration_type_set_dim_count(JagAstDeclarationType *declaration_type, int newDimCount);

int jag_ast_declaration_type_get_dim_count(JagAstDeclarationType *declaration_type);

JagBytName *jag_ast_declaration_type_get_reference_type_name(JagAstDeclarationType *declaration_type);

JagAstDeclarationType *jag_ast_declaration_type_set_reference_type_name(JagAstDeclarationType *declaration_type, JagBytName *new_name);


JagBytType jag_ast_declaration_type_get_primitive_type(JagAstDeclarationType *declaration_type);

gboolean jag_ast_declaration_type_is_reference(JagAstDeclarationType *declaration_type);

gboolean jag_ast_declaration_type_is_void(JagAstDeclarationType *declaration_type);

gboolean jag_ast_declaration_type_is_primitive(JagAstDeclarationType *declaration_type);


gboolean jag_ast_declaration_type_is_category2(JagAstDeclarationType *declaration_type);

int jag_ast_declaration_type_hash_code(JagAstDeclarationType *declaration_type);

gboolean jag_ast_declaration_type_equal(JagAstDeclarationType *declaration_type, JagAstDeclarationType *other);


void jag_ast_declaration_type_write(JagAstDeclarationType *declaration_type, JagAstWriter *out);

CatStringWo *jag_ast_declaration_type_as_text(JagAstDeclarationType *declaration_type, CatStringWo *e_dump_buffer);

G_END_DECLS

#endif /* JAGASTDECLARATIONTYPE_H_ */
