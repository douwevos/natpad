/*
   File:    catstringinputstream.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 2, 2012
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

#ifndef CATSTRINGINPUTSTREAM_H_
#define CATSTRINGINPUTSTREAM_H_

#include "../woo/catstringwo.h"
#include "catstream.h"

G_BEGIN_DECLS

#define CAT_TYPE_STRING_INPUT_STREAM              (cat_string_input_stream_get_type())
#define CAT_STRING_INPUT_STREAM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_string_input_stream_get_type(), CatStringInputStream))
#define CAT_STRING_INPUT_STREAM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_STRING_INPUT_STREAM, CatStringInputStreamClass))
#define CAT_IS_STRING_INPUT_STREAM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_STRING_INPUT_STREAM))
#define CAT_IS_STRING_INPUT_STREAM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_STRING_INPUT_STREAM))
#define CAT_STRING_INPUT_STREAM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_STRING_INPUT_STREAM, CatStringInputStreamClass))
#define CAT_STRING_INPUT_STREAM_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_STRING_INPUT_STREAM, CatStringInputStreamPrivate))

typedef struct _CatStringInputStream               CatStringInputStream;
typedef struct _CatStringInputStreamPrivate        CatStringInputStreamPrivate;
typedef struct _CatStringInputStreamClass          CatStringInputStreamClass;


struct _CatStringInputStream {
	GObject parent;
	CatStringInputStreamPrivate *priv;
};

struct _CatStringInputStreamClass {
	GObjectClass parent_class;
};


GType cat_string_input_stream_get_type();

CatStringInputStream *cat_string_input_stream_new(const CatStringWo *e_text);

G_END_DECLS

#endif /* CATSTRINGINPUTSTREAM_H_ */
