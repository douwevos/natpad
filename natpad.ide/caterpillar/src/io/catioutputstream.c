/*
   File:    catioutputstream.c
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

#include "catioutputstream.h"

#include "../catlib.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatIOuputStream"
#include "../logging/catlog.h"


static void _close_warn(CatIOutputStream *self) {
	cat_log_warn("CatIOutputStream.close not implemented for class: %s", g_type_name_from_instance((GTypeInstance *) self));
}

static void cat_ioutput_stream_interface_init(gpointer klass) {
	CatIOutputStreamInterface *iface = (CatIOutputStreamInterface*) klass;
	iface->close = _close_warn;
}


GType cat_ioutput_stream_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (CatIOutputStreamInterface),
				cat_ioutput_stream_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};


		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("CatIOutputStream"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}

void cat_ioutput_stream_close(CatIOutputStream *self)  {
	g_return_if_fail(CAT_IS_IOUTPUT_STREAM(self));
	CAT_IOUTPUT_STREAM_GET_INTERFACE(self)->close(self);
}

void cat_ioutput_stream_write(CatIOutputStream *self, char data) {
	g_return_if_fail(CAT_IS_IOUTPUT_STREAM(self));
	CAT_IOUTPUT_STREAM_GET_INTERFACE(self)->write(self, data);
}

int cat_ioutput_stream_write_length(CatIOutputStream *self, const char *data, int length) {
	g_return_val_if_fail(CAT_IS_IOUTPUT_STREAM(self), -1);
	return CAT_IOUTPUT_STREAM_GET_INTERFACE(self)->write_length(self, data, length);
}
