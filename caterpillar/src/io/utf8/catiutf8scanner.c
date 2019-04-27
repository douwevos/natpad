/*
   File:    catiutf8scanner.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 2, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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


#include "catiutf8scanner.h"

static void cat_iutf8_scanner_interface_init(gpointer klass) {
//	CatIUtf8ScannerInterface *iface = (CatIUtf8ScannerInterface*) klass;
}


GType cat_iutf8_scanner_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (CatIUtf8ScannerInterface),
				cat_iutf8_scanner_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};


		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("CatIUtf8Scanner"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}


gunichar cat_iutf8_scanner_next_char(CatIUtf8Scanner *self, CatStreamStatus *status) {
	g_return_val_if_fail(CAT_IS_IUTF8_SCANNER(self), -1);
	return CAT_IUTF8_SCANNER_GET_INTERFACE(self)->scan_utf8_char(self, status);
}
