/*
   File:    catstringinputstream.c
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

#include "catstringinputstream.h"
#include "catiinputstream.h"
#include "../catlib.h"
#include <string.h>

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatStringInputStream"
#include "../logging/catlog.h"

struct _CatStringInputStreamPrivate {
	const CatStringWo *e_text;
	int index;
};

static void l_input_stream_iface_init(CatIInputStreamInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatStringInputStream, cat_string_input_stream, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IINPUT_STREAM, l_input_stream_iface_init)
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void cat_string_input_stream_class_init(CatStringInputStreamClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(CatStringInputStreamPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void cat_string_input_stream_init(CatStringInputStream *instance) {
	CatStringInputStreamPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_STRING_INPUT_STREAM, CatStringInputStreamPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatStringInputStream *instance = CAT_STRING_INPUT_STREAM(object);
	CatStringInputStreamPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_text);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatStringInputStream *cat_string_input_stream_new(const CatStringWo *e_text) {
	CatStringInputStream *result = g_object_new(CAT_TYPE_STRING_INPUT_STREAM, NULL);
	cat_ref_anounce(result);
	CatStringInputStreamPrivate *priv = result->priv;
	priv->e_text = cat_ref_ptr((CatStringWo *) e_text);
	priv->index = 0;
	return result;
}




static int l_read(CatIInputStream *self) {
	CatStringInputStream *stream = CAT_STRING_INPUT_STREAM(self);
	CatStringInputStreamPrivate *priv = stream->priv;
	int result = -1;
	if (priv->index<cat_string_wo_length(priv->e_text)) {
		result = cat_string_wo_char_at(priv->e_text, priv->index++);
	}
	return result;
}

static int l_read_length(CatIInputStream *self, char *data, int length) {
	CatStringInputStream *stream = CAT_STRING_INPUT_STREAM(self);
	CatStringInputStreamPrivate *priv = stream->priv;
	int avail = cat_string_wo_length(priv->e_text) - priv->index;
	if (length > avail) {
		length = avail;
	}
	int result = length;
	if (result>0) {
		const char *array_data = cat_string_wo_getchars(priv->e_text);
		memcpy(data, array_data+priv->index, result);
		priv->index += result;
	} else {
		result = -1;
	}
	return result;
}

static long long l_seek(CatIInputStream *self, CatSeekType seek_type, long long seek_offset) {
	CatStringInputStream *stream = CAT_STRING_INPUT_STREAM(self);
	CatStringInputStreamPrivate *priv = stream->priv;
	switch(seek_type) {
		case CAT_SEEK_SET : {
		} break;
		case CAT_SEEK_FORWARD : {
			seek_offset += priv->index;
		} break;
		case CAT_SEEK_SET_FROM_END : {
			seek_offset = cat_string_wo_length(priv->e_text)-seek_offset;
		} break;
	}
	if (seek_offset<0) {
		seek_offset = 0;
	} else if (seek_offset>cat_string_wo_length(priv->e_text)) {
		seek_offset = cat_string_wo_length(priv->e_text);
	}
	priv->index = seek_offset;
	return seek_offset;
}



static void l_input_stream_iface_init(CatIInputStreamInterface *iface) {
	iface->read = l_read;
	iface->readLength = l_read_length;
	iface->seek = l_seek;
}
