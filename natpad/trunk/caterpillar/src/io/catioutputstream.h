/*
   File:    catiouputstream.h
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

#ifndef CATIOUPUTSTREAM_H_
#define CATIOUPUTSTREAM_H_

#include <glib-object.h>

#define CAT_TYPE_IOUTPUT_STREAM                 (cat_ioutput_stream_get_type())
#define CAT_IOUTPUT_STREAM(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_IOUTPUT_STREAM, CatIOutputStream))
#define CAT_IS_IOUTPUT_STREAM(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_IOUTPUT_STREAM))
#define CAT_IOUTPUT_STREAM_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CAT_TYPE_IOUTPUT_STREAM, CatIOutputStreamInterface))

typedef struct _CatIOutputStream               CatIOutputStream; /* dummy object */
typedef struct _CatIOutputStreamInterface      CatIOutputStreamInterface;

struct _CatIOutputStreamInterface {
	GTypeInterface parent_iface;

	void (*close)(CatIOutputStream *self);
	void (*write)(CatIOutputStream *self, char data);
	int (*write_length)(CatIOutputStream *self, const char *data, int length);
};

GType cat_ioutput_stream_get_type(void);


void cat_ioutput_stream_close(CatIOutputStream *self);
void cat_ioutput_stream_write(CatIOutputStream *self, char data);
int cat_ioutput_stream_write_length(CatIOutputStream *self, const char *data, int length);


#endif /* CATIOUPUTSTREAM_H_ */
