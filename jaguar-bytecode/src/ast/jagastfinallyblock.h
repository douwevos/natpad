/*
   File:    jagastfinallyblock.h
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

#ifndef JAGASTFINALLYBLOCK_H_
#define JAGASTFINALLYBLOCK_H_

#include "jagastblock.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_FINALLY_BLOCK              (jag_ast_finally_block_get_type())
#define JAG_AST_FINALLY_BLOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_finally_block_get_type(), JagAstFinallyBlock))
#define JAG_AST_FINALLY_BLOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_FINALLY_BLOCK, JagAstFinallyBlockClass))
#define JAG_AST_IS_FINALLY_BLOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_FINALLY_BLOCK))
#define JAG_AST_IS_FINALLY_BLOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_FINALLY_BLOCK))
#define JAG_AST_FINALLY_BLOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_FINALLY_BLOCK, JagAstFinallyBlockClass))

typedef struct _JagAstFinallyBlock               JagAstFinallyBlock;
typedef struct _JagAstFinallyBlockClass          JagAstFinallyBlockClass;


struct _JagAstFinallyBlock {
	JagAstBlock parent;
};

struct _JagAstFinallyBlockClass {
	JagAstBlockClass parent_class;
};


GType jag_ast_finally_block_get_type();

JagAstFinallyBlock *jag_ast_finally_block_new();

G_END_DECLS

#endif /* JAGASTFINALLYBLOCK_H_ */
