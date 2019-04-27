/*
   File:    jagastiastnode.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Dec 24, 2012
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

#include "jagastiastnode.h"

G_DEFINE_INTERFACE(JagAstIAstNode, jag_ast_iast_node, G_TYPE_OBJECT);

static void jag_ast_iast_node_default_init(JagAstIAstNodeInterface*klass) {
}


void jag_ast_iast_node_write(JagAstIAstNode *self, JagAstWriter *out) {
	g_return_if_fail(JAG_AST_IS_IAST_NODE(self));
	JAG_AST_IAST_NODE_GET_INTERFACE(self)->write(self, out);
}
