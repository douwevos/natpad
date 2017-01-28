/*
   File:    jagastifthenelse.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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


#ifndef JAGASTIFTHENELSE_H_
#define JAGASTIFTHENELSE_H_

#include "jagastblock.h"
#include "jagastiexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_IF_THEN_ELSE              (jag_ast_if_then_else_get_type())
#define JAG_AST_IF_THEN_ELSE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_if_then_else_get_type(), JagAstIfThenElse))
#define JAG_AST_IF_THEN_ELSE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_IF_THEN_ELSE, JagAstIfThenElseClass))
#define JAG_AST_IS_IF_THEN_ELSE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_IF_THEN_ELSE))
#define JAG_AST_IS_IF_THEN_ELSE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_IF_THEN_ELSE))
#define JAG_AST_IF_THEN_ELSE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_IF_THEN_ELSE, JagAstIfThenElseClass))
#define JAG_AST_IF_THEN_ELSE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_IF_THEN_ELSE, JagAstIfThenElsePrivate))

typedef struct _JagAstIfThenElse               JagAstIfThenElse;
typedef struct _JagAstIfThenElsePrivate        JagAstIfThenElsePrivate;
typedef struct _JagAstIfThenElseClass          JagAstIfThenElseClass;


struct _JagAstIfThenElse {
	GObject parent;
	JagAstIfThenElsePrivate *priv;
};

struct _JagAstIfThenElseClass {
	GObjectClass parent_class;
};


GType jag_ast_if_then_else_get_type();

JagAstIfThenElse *jag_ast_if_then_else_new(JagAstIExpression *expression, JagAstBlock *then_block, JagAstBlock *else_block);

G_END_DECLS


#endif /* JAGASTIFTHENELSE_H_ */
