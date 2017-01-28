/*
   File:    catiinputstream.h
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

#ifndef CATIINPUTSTREAM_H_
#define CATIINPUTSTREAM_H_

#include "catstream.h"
#include <glib-object.h>

#define CAT_TYPE_IINPUT_STREAM                 (cat_iinput_stream_get_type())
#define CAT_IINPUT_STREAM(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_IINPUT_STREAM, CatIInputStream))
#define CAT_IS_IINPUT_STREAM(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_IINPUT_STREAM))
#define CAT_IINPUT_STREAM_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CAT_TYPE_IINPUT_STREAM, CatIInputStreamInterface))


typedef struct _CatIInputStream               CatIInputStream; /* dummy object */
typedef struct _CatIInputStreamInterface      CatIInputStreamInterface;


struct _CatIInputStreamInterface {
	GTypeInterface parent_iface;

	int (*read)(CatIInputStream *self);
	int (*readLength)(CatIInputStream *self, char *data, int length);
	long long (*seek)(CatIInputStream *self, CatSeekType seek_type, long long seek_offset);
};

GType cat_iinput_stream_get_type(void);

int cat_iinput_stream_read(CatIInputStream *self);
int cat_iinput_stream_read_length(CatIInputStream *self, char *data, int length);
long long cat_iinput_stream_seek(CatIInputStream *self, CatSeekType seek_type, long long seek_offset);

#endif /* CATIINPUTSTREAM_H_ */
