/*
   File:    catchararrayoutputstream.h
   Project: natpad
   Author:  Douwe Vos
   Date:    Jan 10, 2010
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


#ifndef CATSTRINGOUTPUTSTREAM_H_
#define CATSTRINGOUTPUTSTREAM_H_


#include "../woo/catstringwo.h"
#include "catstream.h"

G_BEGIN_DECLS

#define CAT_TYPE_STRING_OUTPUT_STREAM            (cat_string_output_stream_get_type())
#define CAT_STRING_OUTPUT_STREAM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_string_output_stream_get_type(), CatStringOutputStream))
#define CAT_STRING_OUTPUT_STREAM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_STRING_OUTPUT_STREAM, CatStringOutputStreamClass))
#define CAT_IS_STRING_OUTPUT_STREAM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_STRING_OUTPUT_STREAM))
#define CAT_IS_STRING_OUTPUT_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_STRING_OUTPUT_STREAM))
#define CAT_STRING_OUTPUT_STREAM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_STRING_OUTPUT_STREAM, CatStringOutputStreamClass))


typedef struct _CatStringOutputStream	   		CatStringOutputStream;
typedef struct _CatStringOutputStreamClass	  	CatStringOutputStreamClass;


struct _CatStringOutputStream {
	GObject parent;
	CatStringWo *e_char_array;
};

struct _CatStringOutputStreamClass {
	GObjectClass parent_class;
};


GType cat_string_output_stream_get_type();

CatStringOutputStream *cat_string_output_stream_new();

int cat_string_output_stream_write_length(CatStringOutputStream *self, const char *data, int length, CatStreamStatus *stream_status);

G_END_DECLS

#endif /* CATSTRINGOUTPUTSTREAM_H_ */
