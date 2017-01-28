/*
   File:    jagastargument.h
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


#ifndef JAGBYTARGUMENT_H_
#define JAGBYTARGUMENT_H_


#include "jagastdeclarationtype.h"
#include "jagastmodifiers.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_ARGUMENT              (jag_ast_argument_get_type())
#define JAG_AST_ARGUMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_argument_get_type(), JagAstArgument))
#define JAG_AST_ARGUMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_ARGUMENT, JagAstArgumentClass))
#define JAG_AST_IS_ARGUMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_ARGUMENT))
#define JAG_AST_IS_ARGUMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_ARGUMENT))
#define JAG_AST_ARGUMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_ARGUMENT, JagAstArgumentClass))
#define JAG_AST_ARGUMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_ARGUMENT, JagAstArgumentPrivate))

typedef struct _JagAstArgument               JagAstArgument;
typedef struct _JagAstArgumentPrivate        JagAstArgumentPrivate;
typedef struct _JagAstArgumentClass          JagAstArgumentClass;


struct _JagAstArgument {
	GObject parent;
	JagAstArgumentPrivate *priv;
};

struct _JagAstArgumentClass {
	GObjectClass parent_class;
};


GType jag_ast_argument_get_type();

JagAstArgument *jag_ast_argument_new(JagAstDeclarationType *declaration_type, CatStringWo *a_name);

CatStringWo *jag_ast_argument_get_name(JagAstArgument *argument);

JagAstDeclarationType *jag_ast_argument_get_declaration_type(JagAstArgument *argument);

JagAstModifiers *jag_ast_argument_get_modifiers(JagAstArgument *argument);

gboolean jag_ast_argument_equal(JagAstArgument *argument_a, JagAstArgument *argument_b);

CatStringWo *jag_ast_argument_as_text(JagAstArgument *argument, CatStringWo *e_dump_buffer);

G_END_DECLS


#endif /* JAGBYTARGUMENT_H_ */
