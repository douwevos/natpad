/*
   File:    catfileinputstream.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 4, 2009
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

#ifndef CATFILEINPUTSTREAM_H_
#define CATFILEINPUTSTREAM_H_

#include <fcntl.h>
#include <unistd.h>
#include <glib-object.h>
#include "../woo/catstringwo.h"
#include "catstream.h"

#ifdef G_OS_WIN32
#include <windows.h>
#endif

G_BEGIN_DECLS

#define CAT_TYPE_FILE_INPUT_STREAM            (cat_file_input_stream_get_type())
#define CAT_FILE_INPUT_STREAM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_file_input_stream_get_type(), CatFileInputStream))
#define CAT_FILE_INPUT_STREAM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_FILE_INPUT_STREAM, CatFileInputStreamClass))
#define CAT_IS_FILE_INPUT_STREAM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_FILE_INPUT_STREAM))
#define CAT_IS_FILE_INPUT_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_FILE_INPUT_STREAM))
#define CAT_FILE_INPUT_STREAM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_FILE_INPUT_STREAM, CatFileInputStreamClass))


typedef struct _CatFileInputStream             CatFileInputStream;

typedef struct _CatFileInputStreamClass        CatFileInputStreamClass;

struct _CatFileInputStream {
	GObject parent;
#ifdef G_OS_UNIX
	int file_handle;
#else
	HANDLE file_handle;
#endif
	CatStreamState state;
	CatStringWo *a_path;
};

struct _CatFileInputStreamClass {
	GObjectClass parent_class;
};


GType cat_file_input_stream_get_type(void);

CatFileInputStream *cat_file_input_stream_new(CatStringWo *a_path);

void cat_file_input_stream_close(CatFileInputStream *stream);

#ifdef G_OS_UNIX
CatFileInputStream *cat_file_input_stream_new_opened(CatStringWo *a_path, int file_handle);
#else
CatFileInputStream *cat_file_input_stream_new_opened(CatStringWo *a_path, HANDLE file_handle);
#endif

int cat_file_input_stream_read(CatFileInputStream *stream);

int cat_file_input_stream_read_length(CatFileInputStream *stream, char *data, int length);

CatStreamState cat_file_input_stream_get_status(CatFileInputStream *stream);

G_END_DECLS


#endif /* CATFILEINPUTSTREAM_H_ */
