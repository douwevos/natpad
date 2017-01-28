/*
   File:    catiutf8printer.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 17, 2009
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


#include "catiutf8printer.h"

static void cat_iutf8_printer_interface_init(gpointer klass) {
//	CatIUtf8PrinterInterface *iface = (CatIUtf8PrinterInterface*) klass;
}


GType cat_iutf8_printer_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (CatIUtf8PrinterInterface),
				cat_iutf8_printer_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};


		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("CatIUtf8Printer"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}



void cat_iutf8_printer_close(CatIUtf8Printer *self) {
	g_return_if_fail(CAT_IS_IUTF8_PRINTER(self));
	CAT_IUTF8_PRINTER_GET_INTERFACE(self)->close(self);
}


int cat_iutf8_printer_print_unichar4(CatIUtf8Printer *self, gunichar ch, CatStreamStatus *status) {
	g_return_val_if_fail(CAT_IS_IUTF8_PRINTER(self), -1);
	return CAT_IUTF8_PRINTER_GET_INTERFACE(self)->print_unichar4(self, ch, status);
}

int cat_iutf8_printer_print_utf8_string(CatIUtf8Printer *self, CatStringWo *toprint, CatStreamStatus *status) {
	g_return_val_if_fail(CAT_IS_IUTF8_PRINTER(self), -1);
	return CAT_IUTF8_PRINTER_GET_INTERFACE(self)->print_utf8_string(self, toprint, status);
}
