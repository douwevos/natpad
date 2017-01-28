/*
   File:    jagastblock.h
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


#ifndef JAGASTBLOCK_H_
#define JAGASTBLOCK_H_

#include "jagastistatement.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_BLOCK              (jag_ast_block_get_type())
#define JAG_AST_BLOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_block_get_type(), JagAstBlock))
#define JAG_AST_BLOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_BLOCK, JagAstBlockClass))
#define JAG_AST_IS_BLOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_BLOCK))
#define JAG_AST_IS_BLOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_BLOCK))
#define JAG_AST_BLOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_BLOCK, JagAstBlockClass))
#define JAG_AST_BLOCK_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_BLOCK, JagAstBlockPrivate))

typedef struct _JagAstBlock               JagAstBlock;
typedef struct _JagAstBlockPrivate        JagAstBlockPrivate;
typedef struct _JagAstBlockClass          JagAstBlockClass;


struct _JagAstBlock {
	GObject parent;
	JagAstBlockPrivate *priv;
};

struct _JagAstBlockClass {
	GObjectClass parent_class;
};


GType jag_ast_block_get_type();


void jag_ast_block_construct(JagAstBlock *block);

JagAstBlock *jag_ast_block_new();

void jag_ast_block_append_statement(JagAstBlock *block, JagAstIStatement *statement);

void jag_ast_block_remove_last_return(JagAstBlock *block);

int jag_ast_block_get_statement_line_nr(JagAstBlock *block);

JagAstIStatement *jag_ast_block_get_statement_at(JagAstBlock *block, int index);


G_END_DECLS


#endif /* JAGASTBLOCK_H_ */
