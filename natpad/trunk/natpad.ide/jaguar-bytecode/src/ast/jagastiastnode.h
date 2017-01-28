/*
   File:    jagastiastnode.h
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


#ifndef JAGASTIASTNODE_H_
#define JAGASTIASTNODE_H_

#include "formater/jagastwriter.h"
#include <glib-object.h>

#define JAG_AST_TYPE_IAST_NODE                 (jag_ast_iast_node_get_type())
#define JAG_AST_IAST_NODE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_AST_TYPE_IAST_NODE, JagAstIAstNode))
#define JAG_AST_IS_IAST_NODE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_AST_TYPE_IAST_NODE))
#define JAG_AST_IAST_NODE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_AST_TYPE_IAST_NODE, JagAstIAstNodeInterface))


typedef struct _JagAstIAstNode               JagAstIAstNode; /* dummy object */
typedef struct _JagAstIAstNodeInterface      JagAstIAstNodeInterface;

struct _JagAstIAstNodeInterface {
	GTypeInterface parent_iface;

	void (*write)(JagAstIAstNode *self, JagAstWriter *out);
};

GType jag_ast_iast_node_get_type(void);

void jag_ast_iast_node_write(JagAstIAstNode *self, JagAstWriter *out);

#endif /* JAGASTIASTNODE_H_ */
