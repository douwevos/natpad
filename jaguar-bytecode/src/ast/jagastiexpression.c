/*
   File:    jagastiexpression.c
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

#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstIExpression"
#include <logging/catlog.h>

static gboolean l_equal(JagAstIExpression *self, JagAstIExpression *other) {
	cat_log_trace("JagAstIExpressionInterface->equal not implemented by:%s", g_type_name_from_instance((GTypeInstance *) self)); // TODO check all expressions
	return self==other;
}

static void l_dummy_write(JagAstIExpression *self, JagAstWriter *out) {
	cat_log_fatal("dummy_write:self=%s", g_type_name_from_instance((GTypeInstance *) l_dummy_write));
}


static void jag_ast_iexpression_interface_init(gpointer klass) {
	JagAstIExpressionInterface *iface = (JagAstIExpressionInterface *) klass;
	iface->equal = l_equal;
	iface->write = l_dummy_write;
}


GType jag_ast_iexpression_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (JagAstIExpressionInterface),
				jag_ast_iexpression_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};


		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("JagAstIExpression"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}


void jag_ast_iexpression_write(JagAstIExpression *self, JagAstWriter *out) {
	g_return_if_fail(JAG_AST_IS_IEXPRESSION(self));
	cat_log_trace("priv->identifier=%s", g_type_name_from_instance((GTypeInstance *) self));

	JAG_AST_IEXPRESSION_GET_INTERFACE(self)->write(self, out);
}


gboolean jag_ast_iexpression_equal(JagAstIExpression *self, JagAstIExpression *other) {
	if (self==other) {
		return TRUE;
	}
	if (self==NULL || other==NULL) {
		return FALSE;
	}
	if (((GTypeInstance *) self)->g_class->g_type != ((GTypeInstance *) other)->g_class->g_type) {
		return FALSE;
	}
	g_return_val_if_fail(JAG_AST_IS_IEXPRESSION(self), FALSE);
	return JAG_AST_IEXPRESSION_GET_INTERFACE(self)->equal(self, other);
}
