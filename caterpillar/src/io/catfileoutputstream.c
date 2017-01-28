/*
   File:    catfileoutputstream.c
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

#include "catfileoutputstream.h"
#include "catioutputstream.h"
#include "../catlib.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatFileOuputStream"
#include "../logging/catlog.h"

static void _stream_close(CatIOutputStream *self) {
	CatFileOutputStream *stream = CAT_FILE_OUTPUT_STREAM(self);
	cat_file_output_stream_close(stream);
}

static void _stream_write(CatIOutputStream *self, char data) {
	CatFileOutputStream *stream = CAT_FILE_OUTPUT_STREAM(self);
	cat_file_output_stream_write(stream, data);
}

static int _stream_write_length(CatIOutputStream *self, const char *data, int length) {
	CatFileOutputStream *stream = CAT_FILE_OUTPUT_STREAM(self);
	return cat_file_output_stream_write_length(stream, data, length);
}


static void _output_stream_iface_init(CatIOutputStreamInterface *_iface) {
	_iface->close = _stream_close;
	_iface->write = _stream_write;
	_iface->write_length = _stream_write_length;
}

G_DEFINE_TYPE_WITH_CODE(CatFileOutputStream, cat_file_output_stream, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IOUTPUT_STREAM, _output_stream_iface_init)
})


static void _dispose(GObject *object);

static void cat_file_output_stream_class_init(CatFileOutputStreamClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_file_output_stream_init(CatFileOutputStream *node) {
}

static void _dispose(GObject *object) {
	CatFileOutputStream *instance = CAT_FILE_OUTPUT_STREAM(object);
	cat_file_output_stream_close(instance);
	cat_unref_ptr(instance->a_path);
}

CatFileOutputStream *cat_file_output_stream_new(CatStringWo *a_path) {
	CatFileOutputStream *result = g_object_new(CAT_TYPE_FILE_OUTPUT_STREAM, NULL);
	cat_ref_anounce(result);
	result->a_path = cat_ref_ptr(a_path);
	result->state = STREAM_INIT;
#ifdef G_OS_UNIX
	result->file_handle = -1;
#else
	result->file_handle = NULL;
#endif
	return result;
}

void cat_file_output_stream_close(CatFileOutputStream *stream) {
#ifdef G_OS_UNIX
	if (stream->file_handle!=-1) {
		close(stream->file_handle);
		stream->file_handle = -1;
	}
#else
	if (stream->file_handle!=NULL) {
		CloseHandle(stream->file_handle);
		stream->file_handle = NULL;
	}
#endif
	stream->state = STREAM_CLOSED;
}


static void _open_stream(CatFileOutputStream *stream) {
	const gchar *fname = cat_string_wo_getchars(stream->a_path);
	cat_log_debug("opening fname= %s", fname);
#ifdef G_OS_WIN32
	stream->file_handle = CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	cat_log_debug("stream->file_handle=%d", stream->file_handle!=NULL);
	if (stream->file_handle!=NULL) {
		stream->state = STREAM_OPENED;
	} else {
		cat_log_error("stream->file_handle=%d, errono=%d", stream->file_handle!=NULL, errno);
		stream->state = STREAM_CLOSED;
	}
#else
	stream->file_handle = open(fname, O_CREAT|O_WRONLY|O_TRUNC, DEFFILEMODE);
	cat_log_debug("stream->file_handle=%d", stream->file_handle);
	if (stream->file_handle!=-1) {
		stream->state = STREAM_OPENED;
	} else {
		cat_log_error("stream->file_handle=%d, errono=%d", stream->file_handle, errno);
		stream->state = STREAM_CLOSED;
	}
#endif
}


void cat_file_output_stream_write(CatFileOutputStream *stream, char data) {
	cat_log_trace("stream_write");
	switch(stream->state) {
		case STREAM_INIT : {
			cat_log_trace("STREAM_INIT");
			_open_stream(stream);
			if (stream->state!=STREAM_OPENED) {
				break;
			}
		} // NO BREAK HERE !!!
		case STREAM_OPENED : {
			cat_log_trace("STREAM_OPENED");
#ifdef G_OS_UNIX
			int result = write(stream->file_handle, &data, 1);
			cat_log_detail("write=%d", result);
#else
			DWORD result = 0;
			WriteFile(stream->file_handle, &data, 1, &result, NULL);
			cat_log_detail("write=%ld", result);
#endif
			if (result<=0) {
				cat_file_output_stream_close(stream);
			}
		} break;
		case STREAM_CLOSED : break;
	}
}


int cat_file_output_stream_write_length(CatFileOutputStream *stream, const char *data, int length) {
	cat_log_trace("stream_write_length");
	int result = -1;
	switch(stream->state) {
		case STREAM_INIT : {
			cat_log_trace("STREAM_INIT");
			_open_stream(stream);
			if (stream->state!=STREAM_OPENED) {
				break;
			}
		} // NO BREAK HERE !!!
		case STREAM_OPENED : {
			cat_log_trace("STREAM_OPENED");
			if (length==0) {
				result = 0;
			} else {
#ifdef G_OS_UNIX
			int wresult = write(stream->file_handle, data, length);
			cat_log_detail("write=%d", wresult);
#else
			DWORD wresult = 0;
			WriteFile(stream->file_handle, data, length, &wresult, NULL);
			cat_log_detail("write=%ld", wresult);
#endif
			if (wresult<=0) {
				cat_file_output_stream_close(stream);
				result = -1;
			}
			}
		} break;
		case STREAM_CLOSED : break;
	}
	return result;
}
