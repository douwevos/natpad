/*
   File:    catstringoutputstream.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jan 10, 2010
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


#include "catstringoutputstream.h"
#include "catioutputstream.h"
#include "../catlib.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatStringOutputStream"
#include "../logging/catlog.h"

static void _output_stream_close(CatIOutputStream *self) { /* dummy */ }

static void _output_stream_write(CatIOutputStream *self, char data) {
	CatStringOutputStream *this = CAT_STRING_OUTPUT_STREAM(self);
	CatStreamStatus status;
	cat_string_output_stream_write_length(this, &data, 1, &status);
}

static int _output_stream_write_length(CatIOutputStream *self, const char *data, int length) {
	CatStringOutputStream *this = CAT_STRING_OUTPUT_STREAM(self);
	CatStreamStatus status;
	int result = cat_string_output_stream_write_length(this, data, length, &status);
	if (status!=CAT_STREAM_OK) {
		result = -1;
	}
	return result;
}


static void _output_stream_iface_init(CatIOutputStreamInterface *_iface) {
	_iface->close = _output_stream_close;
	_iface->write = _output_stream_write;
	_iface->write_length = _output_stream_write_length;
}

G_DEFINE_TYPE_WITH_CODE(CatStringOutputStream, cat_string_output_stream, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IOUTPUT_STREAM, _output_stream_iface_init)
})

static void _dispose(GObject *object);

static void cat_string_output_stream_class_init(CatStringOutputStreamClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_string_output_stream_init(CatStringOutputStream *instance) {
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatStringOutputStream *instance = CAT_STRING_OUTPUT_STREAM(object);
	cat_unref_ptr(instance->e_char_array);
	cat_log_detail("end-dispose:%p", object);
}


CatStringOutputStream *cat_string_output_stream_new() {
	CatStringOutputStream *result = g_object_new(CAT_TYPE_STRING_OUTPUT_STREAM, NULL);
	cat_ref_anounce(result);
	result->e_char_array = cat_string_wo_new();
	return result;
}



int cat_string_output_stream_write_length(CatStringOutputStream *self, const char *data, int length, CatStreamStatus *stream_status) {
	cat_string_wo_append_chars_len(self->e_char_array, data, length);
	*stream_status = CAT_STREAM_OK;
	return length;
}

