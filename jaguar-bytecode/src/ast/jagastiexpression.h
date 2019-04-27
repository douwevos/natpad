/*
   File:    jagastiexpression.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 29, 2012
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

#ifndef JAGASTIEXPRESSION_H_
#define JAGASTIEXPRESSION_H_

#include "formater/jagastwriter.h"
#include <glib-object.h>

#define JAG_AST_TYPE_IEXPRESSION                 (jag_ast_iexpression_get_type())
#define JAG_AST_IEXPRESSION(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_AST_TYPE_IEXPRESSION, JagAstIExpression))
#define JAG_AST_IS_IEXPRESSION(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_AST_TYPE_IEXPRESSION))
#define JAG_AST_IEXPRESSION_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_AST_TYPE_IEXPRESSION, JagAstIExpressionInterface))


typedef struct _JagAstIExpression               JagAstIExpression; /* dummy object */
typedef struct _JagAstIExpressionInterface      JagAstIExpressionInterface;

struct _JagAstIExpressionInterface {
	GTypeInterface parent_iface;

	void (*write)(JagAstIExpression *self, JagAstWriter *out);
	gboolean (*equal)(JagAstIExpression *self, JagAstIExpression *other);
};

GType jag_ast_iexpression_get_type(void);


void jag_ast_iexpression_write(JagAstIExpression *self, JagAstWriter *out);

gboolean jag_ast_iexpression_equal(JagAstIExpression *self, JagAstIExpression *other);

#endif /* JAGASTIEXPRESSION_H_ */
