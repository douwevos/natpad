/*
   File:    elkiresourcehandler.c
   Project: elk
   Author:  Douwe Vos
   Date:    Nov 25, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "elkiresourcehandler.h"

G_DEFINE_INTERFACE(ElkIResourceHandler, elk_iresource_handler, G_TYPE_OBJECT);

static void elk_iresource_handler_default_init (ElkIResourceHandlerInterface *clazz) {
	ElkIResourceHandlerInterface *iface = (ElkIResourceHandlerInterface*) clazz;
	iface->matchEditorFactories = NULL;
	iface->emptyEditorFactories = NULL;
}


//
//
//GType elk_iresource_handler_get_type (void) {
//	static volatile gsize g_define_type_id__volatile = 0;
//	if (g_once_init_enter (&g_define_type_id__volatile)) {
//		static const GTypeInfo info = {
//			sizeof (ElkIResourceHandlerInterface),
//			elk_iresource_handler_interface_init,   /* base_init */
//			NULL,   /* base_finalize */
//		};
//
//		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE, g_intern_static_string ("ElkIResourceHandler"), &info,0);
//		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
//	}
//	return g_define_type_id__volatile;
//}

