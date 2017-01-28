/*
   File:    jagastmethod.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 8, 2012
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


#ifndef JAGASTMETHOD_H_
#define JAGASTMETHOD_H_

#include "jagastmodifiers.h"
#include "jagastmethodheader.h"
#include "jagastblock.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_METHOD              (jag_ast_method_get_type())
#define JAG_AST_METHOD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_method_get_type(), JagAstMethod))
#define JAG_AST_METHOD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_METHOD, JagAstMethodClass))
#define JAG_AST_IS_METHOD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_METHOD))
#define JAG_AST_IS_METHOD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_METHOD))
#define JAG_AST_METHOD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_METHOD, JagAstMethodClass))
#define JAG_AST_METHOD_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_METHOD, JagAstMethodPrivate))

typedef struct _JagAstMethod               JagAstMethod;
typedef struct _JagAstMethodPrivate        JagAstMethodPrivate;
typedef struct _JagAstMethodClass          JagAstMethodClass;


struct _JagAstMethod {
	GObject parent;
	JagAstMethodPrivate *priv;
};

struct _JagAstMethodClass {
	GObjectClass parent_class;
};


GType jag_ast_method_get_type();

JagAstMethod *jag_ast_method_new(JagAstModifiers *modifiers, JagAstMethodHeader *method_header, JagAstBlock *statement_block);

G_END_DECLS


#endif /* JAGASTMETHOD_H_ */
