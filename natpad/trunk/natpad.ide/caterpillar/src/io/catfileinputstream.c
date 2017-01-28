/*
   File:    catfileinputstream.c
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

#include "catfileinputstream.h"
#include "catiinputstream.h"
#include "../catlib.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatFileInputStream"
#include "../logging/catlog.h"


static void l_input_stream_iface_init(CatIInputStreamInterface *iface);


G_DEFINE_TYPE_WITH_CODE(CatFileInputStream, cat_file_input_stream, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IINPUT_STREAM, l_input_stream_iface_init)
})

static void _dispose(GObject *object);

static void cat_file_input_stream_class_init(CatFileInputStreamClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_file_input_stream_init(CatFileInputStream *node) {
}

static void _dispose(GObject *object) {
	CatFileInputStream *instance = CAT_FILE_INPUT_STREAM(object);
	cat_file_input_stream_close(instance);
	cat_unref_ptr(instance->a_path);
}

CatFileInputStream *cat_file_input_stream_new(CatStringWo *a_path) {
	CatFileInputStream *result = g_object_new(CAT_TYPE_FILE_INPUT_STREAM, NULL);
	cat_ref_anounce(result);
	cat_log_detail("path=%s", cat_string_wo_getchars(a_path));
	result->a_path = cat_ref_ptr(a_path);
	result->state = STREAM_INIT;
	return result;
}


#ifdef G_OS_UNIX
CatFileInputStream *cat_file_input_stream_new_opened(CatStringWo *a_path, int file_handle) {
#else
CatFileInputStream *cat_file_input_stream_new_opened(CatStringWo *a_path, HANDLE file_handle) {
#endif
	CatFileInputStream *result = g_object_new(CAT_TYPE_FILE_INPUT_STREAM, NULL);
	cat_ref_anounce(result);
	result->a_path = cat_ref_ptr(a_path);
	result->state = STREAM_OPENED;
	result->file_handle = file_handle;
	return result;
}


void cat_file_input_stream_close(CatFileInputStream *stream) {
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


int cat_file_input_stream_read(CatFileInputStream *stream) {
	switch(stream->state) {
		case STREAM_INIT : {
			cat_log_trace("STREAM_INIT");
			const gchar *fname = cat_string_wo_getchars(stream->a_path);
			cat_log_debug("opening fname=%s", fname);
#ifdef G_OS_UNIX
			stream->file_handle = open(fname, O_RDONLY);
			cat_log_debug("stream->file_handle=%d", stream->file_handle);
			if (stream->file_handle!=-1) {
#else
			stream->file_handle = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			cat_log_debug("stream->file_handle=%d", stream->file_handle!=NULL);
			if (stream->file_handle!=NULL) {
#endif
				stream->state = STREAM_OPENED;
			} else {
				stream->state = STREAM_CLOSED;
				break;
			}
		} // NO BREAK HERE !!!
		case STREAM_OPENED : {
			cat_log_trace("STREAM_OPENED");
#ifdef G_OS_UNIX
			unsigned char data;
			int result = read(stream->file_handle, &data, 1);
			cat_log_detail("read=%d", result);
			if (result<=0) {
				close(stream->file_handle);
				stream->file_handle = -1;
				stream->state = STREAM_CLOSED;
				result = -1;
			} else {
				result = data;
#else
			unsigned char data[5];
			cat_log_detail("stream->file_handle=%d", stream->file_handle!=NULL);
			unsigned long read_size;
			BOOL did_read = ReadFile(stream->file_handle, data, 1, &read_size, NULL);
			int result = -1;
			cat_log_detail("read=%ld, did_read=%d", read_size, did_read);
			if (!did_read || read_size<=0L) {
				CloseHandle(stream->file_handle);
				stream->file_handle = NULL;
				stream->state = STREAM_CLOSED;
			} else {
				result = data[0];
#endif
			}
			return result;
		} break;
		case STREAM_CLOSED : {
			return -1;
		} break;
	}
	return -1;
}


int cat_file_input_stream_read_length(CatFileInputStream *stream, char *data, int length) {
	cat_log_trace("read_length");
	int result = -1;
	switch(stream->state) {
		case STREAM_INIT : {
			const gchar *fname = cat_string_wo_getchars(stream->a_path);
			cat_log_debug("opening fname=%s", fname);
#ifdef G_OS_UNIX
			stream->file_handle = open(fname, O_RDONLY);
			if (stream->file_handle!=-1) {
#else
			stream->file_handle = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (stream->file_handle!=NULL) {
#endif
				stream->state = STREAM_OPENED;
			} else {
				stream->state = STREAM_CLOSED;
				break;
			}
		} // NO BREAK HERE !!!
		case STREAM_OPENED : {
			if (length==0) {
				result = 0;
				break;
			}

#ifdef G_OS_UNIX
			result = read(stream->file_handle, data, length);
			cat_log_detail("read=%d", result);
			if (result<=0) {
				close(stream->file_handle);
				stream->file_handle = -1;
				stream->state = STREAM_CLOSED;
				result = -1;
			}
#else
			unsigned long read_size;
			BOOL did_read = ReadFile(stream->file_handle, data, length, &read_size, NULL);
			result = read_size;
			cat_log_detail("read=%d, did_read=%d", result, did_read);
			if (!did_read) {
				CloseHandle(stream->file_handle);
				stream->file_handle = NULL;
				stream->state = STREAM_CLOSED;
				result = -1;
			}
#endif
		} break;
		case STREAM_CLOSED : break;
	}
	return result;
}



long long cat_file_input_stream_seek(CatFileInputStream *stream, CatSeekType seek_type, long long seekf_offset) {
	cat_log_trace("seek");
	long long result = -1;
	switch(stream->state) {
		case STREAM_INIT : {
			const gchar *fname = cat_string_wo_getchars(stream->a_path);
			cat_log_debug("opening fname=%s", fname);
#ifdef G_OS_UNIX
			stream->file_handle = open(fname, O_RDONLY);
			if (stream->file_handle!=-1) {
#else
			stream->file_handle = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (stream->file_handle!=NULL) {
#endif
				stream->state = STREAM_OPENED;
			} else {
				stream->state = STREAM_CLOSED;
				break;
			}
		} // NO BREAK HERE !!!
		case STREAM_OPENED : {
#ifdef G_OS_UNIX
			int whence = 0;
			switch(seek_type) {
				case CAT_SEEK_SET : {
					whence = SEEK_SET;
				} break;
				case CAT_SEEK_SET_FROM_END : {
					whence = SEEK_END;
				} break;
				case CAT_SEEK_FORWARD : {
					whence = SEEK_CUR;
				} break;
			}
			result = lseek(stream->file_handle, seekf_offset, whence);

#else
			unsigned int whence = 0;
			switch(seek_type) {
				case CAT_SEEK_SET : {
					whence = FILE_BEGIN;
				} break;
				case CAT_SEEK_SET_FROM_END : {
					whence = FILE_END;
				} break;
				case CAT_SEEK_FORWARD : {
					whence = FILE_CURRENT;
				} break;
			}

			unsigned long distance_to_move_low = (unsigned long) (seekf_offset);
			unsigned long distance_to_move_high = (unsigned long) (seekf_offset>>32);
			DWORD did_set = SetFilePointer(stream->file_handle, distance_to_move_low, &distance_to_move_high, NULL);
			if (did_set != INVALID_SET_FILE_POINTER) {
				result = ((long long) distance_to_move_high)<<32 + ((long long) distance_to_move_low & 0xFFFFFFFFL);
			} else {
				result = -1;
			}
#endif
		} break;
		case STREAM_CLOSED : break;
	}
	return result;
}



CatStreamState cat_file_input_stream_get_status(CatFileInputStream *stream) {
	return stream->state;
}




static int _iface_read(CatIInputStream *self) {
	CatFileInputStream *this = CAT_FILE_INPUT_STREAM(self);
	return cat_file_input_stream_read(this);
}

static int _iface_read_length(CatIInputStream *self, char *data, int length) {
	CatFileInputStream *this = CAT_FILE_INPUT_STREAM(self);
	return cat_file_input_stream_read_length(this, data, length);
}

static long long l_seek(CatIInputStream *self, CatSeekType seek_type, long long seek_offset) {
	CatFileInputStream *stream = CAT_FILE_INPUT_STREAM(self);
	return cat_file_input_stream_seek(stream, seek_type, seek_offset);
}


static void l_input_stream_iface_init(CatIInputStreamInterface *iface) {
	iface->read = _iface_read;
	iface->readLength = _iface_read_length;
	iface->seek = l_seek;

}
