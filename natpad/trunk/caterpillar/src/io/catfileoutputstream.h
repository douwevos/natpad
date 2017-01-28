/*
   File:    catfileoutputstream.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 24, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef CATFILEOUTPUTSTREAM_H_
#define CATFILEOUTPUTSTREAM_H_

#include <fcntl.h>
#include <unistd.h>
#include <glib-object.h>
#include "../woo/catstringwo.h"
#include "catstream.h"

#ifdef G_OS_WIN32
#include <windows.h>
#endif

G_BEGIN_DECLS

#define CAT_TYPE_FILE_OUTPUT_STREAM            (cat_file_output_stream_get_type())
#define CAT_FILE_OUTPUT_STREAM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_file_output_stream_get_type(), CatFileOutputStream))
#define CAT_FILE_OUTPUT_STREAM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_FILE_OUTPUT_STREAM, CatFileOutputStreamClass))
#define CAT_IS_FILE_OUTPUT_STREAM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_FILE_OUTPUT_STREAM))
#define CAT_IS_FILE_OUTPUT_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_FILE_OUTPUT_STREAM))
#define CAT_FILE_OUTPUT_STREAM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_FILE_OUTPUT_STREAM, CatFileOutputStreamClass))


typedef struct _CatFileOutputStream             CatFileOutputStream;

typedef struct _CatFileOutputStreamClass        CatFileOutputStreamClass;

struct _CatFileOutputStream {
	GObject parent;
#ifdef G_OS_WIN32
	HANDLE file_handle;
#else
	int file_handle;
#endif
	CatStreamState state;
	CatStringWo *a_path;
};

struct _CatFileOutputStreamClass {
	GObjectClass parent_class;
};


GType cat_file_output_stream_get_type(void);

CatFileOutputStream *cat_file_output_stream_new(CatStringWo *a_path);

void cat_file_output_stream_close(CatFileOutputStream *stream);

void cat_file_output_stream_write(CatFileOutputStream *stream, char data);

int cat_file_output_stream_write_length(CatFileOutputStream *stream, const char *data, int length);

G_END_DECLS


#endif /* CATFILEOUTPUTSTREAM_H_ */
