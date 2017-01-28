/*
   File:    jagjobjectinputstream.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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

#ifndef JAGJOBJECTINPUTSTREAM_H_
#define JAGJOBJECTINPUTSTREAM_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_JOBJECT_INPUT_STREAM              (jag_jobject_input_stream_get_type())
#define JAG_JOBJECT_INPUT_STREAM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jobject_input_stream_get_type(), JagJObjectInputStream))
#define JAG_JOBJECT_INPUT_STREAM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JOBJECT_INPUT_STREAM, JagJObjectInputStreamClass))
#define JAG_IS_JOBJECT_INPUT_STREAM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JOBJECT_INPUT_STREAM))
#define JAG_IS_JOBJECT_INPUT_STREAM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JOBJECT_INPUT_STREAM))
#define JAG_JOBJECT_INPUT_STREAM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JOBJECT_INPUT_STREAM, JagJObjectInputStreamClass))
#define JAG_JOBJECT_INPUT_STREAM_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_JOBJECT_INPUT_STREAM, JagJObjectInputStreamPrivate))

typedef struct _JagJObjectInputStream               JagJObjectInputStream;
typedef struct _JagJObjectInputStreamPrivate        JagJObjectInputStreamPrivate;
typedef struct _JagJObjectInputStreamClass          JagJObjectInputStreamClass;


struct _JagJObjectInputStream {
	GObject parent;
	JagJObjectInputStreamPrivate *priv;
};

struct _JagJObjectInputStreamClass {
	GObjectClass parent_class;
};


GType jag_jobject_input_stream_get_type();

JagJObjectInputStream *jag_jobject_input_stream_new_from_array(CatStringWo *e_buf);
JagJObjectInputStream *jag_jobject_input_stream_new(CatIInputStream *input_stream);


int8_t jag_jobject_input_stream_read_byte(JagJObjectInputStream *stream);
void jag_jobject_input_stream_read_bytes(JagJObjectInputStream *stream, char *data, int length);
int16_t jag_jobject_input_stream_read_short(JagJObjectInputStream *stream);
int32_t jag_jobject_input_stream_read_int(JagJObjectInputStream *stream);
int64_t jag_jobject_input_stream_read_long(JagJObjectInputStream *stream);


uint8_t jag_jobject_input_stream_read_unsigned_byte(JagJObjectInputStream *stream);
uint16_t jag_jobject_input_stream_read_unsigned_short(JagJObjectInputStream *stream);
uint32_t jag_jobject_input_stream_read_unsigned_int(JagJObjectInputStream *stream);
uint64_t jag_jobject_input_stream_read_unsigned_long(JagJObjectInputStream *stream);

CatStringWo *jag_jobject_input_stream_read_string(JagJObjectInputStream *stream, int byte_length);

G_END_DECLS

#endif /* JAGJOBJECTINPUTSTREAM_H_ */
