/*
   File:    jagastquotedstring.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 16, 2012
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

#ifndef JAGASTQUOTEDSTRING_H_
#define JAGASTQUOTEDSTRING_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_QUOTED_STRING              (jag_ast_quoted_string_get_type())
#define JAG_AST_QUOTED_STRING(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_quoted_string_get_type(), JagAstQuotedString))
#define JAG_AST_QUOTED_STRING_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_QUOTED_STRING, JagAstQuotedStringClass))
#define JAG_AST_IS_QUOTED_STRING(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_QUOTED_STRING))
#define JAG_AST_IS_QUOTED_STRING_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_QUOTED_STRING))
#define JAG_AST_QUOTED_STRING_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_QUOTED_STRING, JagAstQuotedStringClass))

typedef struct _JagAstQuotedString               JagAstQuotedString;
typedef struct _JagAstQuotedStringPrivate        JagAstQuotedStringPrivate;
typedef struct _JagAstQuotedStringClass          JagAstQuotedStringClass;


struct _JagAstQuotedString {
	GObject parent;
};

struct _JagAstQuotedStringClass {
	GObjectClass parent_class;
};


GType jag_ast_quoted_string_get_type();

JagAstQuotedString *jag_ast_quoted_string_new(CatStringWo *a_raw_text);

G_END_DECLS

#endif /* JAGASTQUOTEDSTRING_H_ */
