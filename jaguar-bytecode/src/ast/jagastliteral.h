/*
   File:    jagastliteral.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 9, 2012
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


#ifndef JAGASTLITERAL_H_
#define JAGASTLITERAL_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_LITERAL              (jag_ast_literal_get_type())
#define JAG_AST_LITERAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_literal_get_type(), JagAstLiteral))
#define JAG_AST_LITERAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_LITERAL, JagAstLiteralClass))
#define JAG_AST_IS_LITERAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_LITERAL))
#define JAG_AST_IS_LITERAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_LITERAL))
#define JAG_AST_LITERAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_LITERAL, JagAstLiteralClass))
#define JAG_AST_LITERAL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_LITERAL, JagAstLiteralPrivate))

typedef struct _JagAstLiteral               JagAstLiteral;
typedef struct _JagAstLiteralPrivate        JagAstLiteralPrivate;
typedef struct _JagAstLiteralClass          JagAstLiteralClass;


struct _JagAstLiteral {
	GObject parent;
	JagAstLiteralPrivate *priv;
};

struct _JagAstLiteralClass {
	GObjectClass parent_class;
};


GType jag_ast_literal_get_type();

JagAstLiteral *jag_ast_literal_new(CatIStringable *literal);

CatIStringable *jag_ast_literal_get_value(JagAstLiteral *literal);


G_END_DECLS


#endif /* JAGASTLITERAL_H_ */
