/*
   File:    catiinputstream.c
   Project: natpad
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

#include "catiinputstream.h"

static void cat_iinput_stream_interface_init(gpointer klass) {
//	CatIInputStreamInterface *iface = (CatIInputStreamInterface*) klass;
}

GType cat_iinput_stream_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (CatIInputStreamInterface),
				cat_iinput_stream_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};


		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("CatIInputStream"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}

int cat_iinput_stream_read(CatIInputStream *self) {
	g_return_val_if_fail(CAT_IS_IINPUT_STREAM(self), -1);
	return CAT_IINPUT_STREAM_GET_INTERFACE(self)->read(self);
}

int cat_iinput_stream_read_length(CatIInputStream *self, char *data, int length) {
	g_return_val_if_fail(CAT_IS_IINPUT_STREAM(self), -1);
	return CAT_IINPUT_STREAM_GET_INTERFACE(self)->readLength(self, data, length);
}

long long cat_iinput_stream_seek(CatIInputStream *self, CatSeekType seek_type, long long seek_offset) {
	g_return_val_if_fail(CAT_IS_IINPUT_STREAM(self), -1);
	return CAT_IINPUT_STREAM_GET_INTERFACE(self)->seek(self, seek_type, seek_offset);
}
