/*
   File:    catstdoutstream.h
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 15, 2009
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

#include <glib-object.h>
#include "catstream.h"

#ifndef CATSTDOUTSTREAM_H_
#define CATSTDOUTSTREAM_H_

G_BEGIN_DECLS

#define CAT_TYPE_STDOUT_STREAM            (cat_stdout_stream_get_type())
#define CAT_STDOUT_STREAM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_stdout_stream_get_type(), CatStdoutStream))
#define CAT_STDOUT_STREAM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_STDOUT_STREAM, CatStdoutStreamClass))
#define CAT_IS_STDOUT_STREAM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_STDOUT_STREAM))
#define CAT_IS_STDOUT_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_STDOUT_STREAM))
#define CAT_STDOUT_STREAM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_STDOUT_STREAM, CatStdoutStreamClass))


typedef struct _CatStdoutStream	   		CatStdoutStream;
typedef struct _CatStdoutStreamClass  	CatStdoutStreamClass;


struct _CatStdoutStream {
	GObject parent;
};

struct _CatStdoutStreamClass {
	GObjectClass parent_class;
};


GType cat_stdout_stream_get_type();

CatStdoutStream *cat_stdout_stream_new();

int cat_stdout_stream_write_length(CatStdoutStream *stream, const char *data, int length, CatStreamStatus *status);

G_END_DECLS

#endif /* CATSTDOUTSTREAM_H_ */
