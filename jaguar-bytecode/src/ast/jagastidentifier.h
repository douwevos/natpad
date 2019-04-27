/*
   File:    jagastidentifier.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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

#ifndef JAGASTIDENTIFIER_H_
#define JAGASTIDENTIFIER_H_

#include "../classfile/jagbytname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_IDENTIFIER              (jag_ast_identifier_get_type())
#define JAG_AST_IDENTIFIER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_identifier_get_type(), JagAstIdentifier))
#define JAG_AST_IDENTIFIER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_IDENTIFIER, JagAstIdentifierClass))
#define JAG_AST_IS_IDENTIFIER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_IDENTIFIER))
#define JAG_AST_IS_IDENTIFIER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_IDENTIFIER))
#define JAG_AST_IDENTIFIER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_IDENTIFIER, JagAstIdentifierClass))

typedef struct _JagAstIdentifier               JagAstIdentifier;
typedef struct _JagAstIdentifierPrivate        JagAstIdentifierPrivate;
typedef struct _JagAstIdentifierClass          JagAstIdentifierClass;


struct _JagAstIdentifier {
	GObject parent;
};

struct _JagAstIdentifierClass {
	GObjectClass parent_class;
};


GType jag_ast_identifier_get_type();

JagAstIdentifier *jag_ast_identifier_new(CatStringWo *a_id);

JagAstIdentifier *jag_ast_identifier_new_from_name(JagBytName *name);

CatStringWo *jag_ast_identifier_get_text(JagAstIdentifier *identifier);

G_END_DECLS

#endif /* JAGASTIDENTIFIER_H_ */
