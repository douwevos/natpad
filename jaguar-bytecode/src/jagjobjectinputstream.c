/*
   File:    jagjobjectinputstream.c
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

#include "jagjobjectinputstream.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJObjectInputStream"
#include <logging/catlog.h>

struct _JagJObjectInputStreamPrivate {
	CatIInputStream *istream;
	CatIInputStreamInterface *istream_iface;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagJObjectInputStream, jag_jobject_input_stream, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_jobject_input_stream_class_init(JagJObjectInputStreamClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_jobject_input_stream_init(JagJObjectInputStream *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagJObjectInputStream *instance = JAG_JOBJECT_INPUT_STREAM(object);
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(instance);
	cat_unref_ptr(priv->istream);
	G_OBJECT_CLASS(jag_jobject_input_stream_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_jobject_input_stream_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagJObjectInputStream *jag_jobject_input_stream_new_from_array(CatStringWo *e_buf) {
	JagJObjectInputStream *result = g_object_new(JAG_TYPE_JOBJECT_INPUT_STREAM, NULL);
	cat_ref_anounce(result);
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(result);
	CatIInputStream *input_stream = (CatIInputStream *) cat_string_input_stream_new(e_buf);
	priv->istream = input_stream;
	priv->istream_iface = CAT_IINPUT_STREAM_GET_INTERFACE(input_stream);
	return result;
}


JagJObjectInputStream *jag_jobject_input_stream_new(CatIInputStream *input_stream) {
	JagJObjectInputStream *result = g_object_new(JAG_TYPE_JOBJECT_INPUT_STREAM, NULL);
	cat_ref_anounce(result);
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(result);
	priv->istream = cat_ref_ptr(input_stream);
	priv->istream_iface = CAT_IINPUT_STREAM_GET_INTERFACE(input_stream);
	return result;
}

int8_t jag_jobject_input_stream_read_byte(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	return (int8_t) priv->istream_iface->read(priv->istream);
}

void jag_jobject_input_stream_read_bytes(JagJObjectInputStream *stream, char *data, int length) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	priv->istream_iface->readLength(priv->istream, data, length);
}


int16_t jag_jobject_input_stream_read_short(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	int msb = priv->istream_iface->read(priv->istream);
	int lsb = priv->istream_iface->read(priv->istream);
	return (int16_t) (((msb<<8) & 0xFF00) + (lsb & 0xFF));
}

int32_t jag_jobject_input_stream_read_int(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	char data[4];
	priv->istream_iface->readLength(priv->istream, data, 4);
	return ((((int32_t) data[0]) & 0xFF) << 24)
			+ ((((int32_t) data[1]) & 0xFF) << 16)
			+ ((((int32_t) data[2]) & 0xFF) << 8)
			+ ((((int32_t) data[3]) & 0xFF));
}

int64_t jag_jobject_input_stream_read_long(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	char data[8];
	priv->istream_iface->readLength(priv->istream, data, 8);
	return ((((int64_t) data[0]) & 0xFF) << 56)
			+ ((((int64_t) data[1]) & 0xFF) << 48)
			+ ((((int64_t) data[2]) & 0xFF) << 40)
			+ ((((int64_t) data[3]) & 0xFF) << 32)
			+ ((((int64_t) data[4]) & 0xFF) << 24)
			+ ((((int64_t) data[5]) & 0xFF) << 16)
			+ ((((int64_t) data[6]) & 0xFF) << 8)
			+ ((((int64_t) data[7]) & 0xFF));
}


uint8_t jag_jobject_input_stream_read_unsigned_byte(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	uint8_t result = 0xFF & (uint8_t) priv->istream_iface->read(priv->istream);
	return result;
}

uint16_t jag_jobject_input_stream_read_unsigned_short(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	uint16_t result = 0xFF & (uint16_t) priv->istream_iface->read(priv->istream);
	result = (result<<8) + (0xFF & (uint16_t) priv->istream_iface->read(priv->istream));
	return result;
}

uint32_t jag_jobject_input_stream_read_unsigned_int(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	char data[4];
	priv->istream_iface->readLength(priv->istream, data, 4);
	uint32_t result = 0xFF & (uint32_t) data[0];
	result = (result<<8) + (0xFF & (uint32_t) data[1]);
	result = (result<<8) + (0xFF & (uint32_t) data[2]);
	result = (result<<8) + (0xFF & (uint32_t) data[3]);
	return result;
}

uint64_t jag_jobject_input_stream_read_unsigned_long(JagJObjectInputStream *stream) {
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	char data[8];
	priv->istream_iface->readLength(priv->istream, data, 8);
	uint64_t result = 0xFF & (uint64_t) data[0];
	result = (result<<8) + (0xFF & (uint64_t) data[1]);
	result = (result<<8) + (0xFF & (uint64_t) data[2]);
	result = (result<<8) + (0xFF & (uint64_t) data[3]);
	result = (result<<8) + (0xFF & (uint64_t) data[4]);
	result = (result<<8) + (0xFF & (uint64_t) data[5]);
	result = (result<<8) + (0xFF & (uint64_t) data[6]);
	result = (result<<8) + (0xFF & (uint64_t) data[7]);
	return result;
}

CatStringWo *jag_jobject_input_stream_read_string(JagJObjectInputStream *stream, int byte_length) {
	if (byte_length==0) {
		return cat_string_wo_new_with_len(NULL, 0);
	}
	char *buffer = malloc(byte_length);
	JagJObjectInputStreamPrivate *priv = jag_jobject_input_stream_get_instance_private(stream);
	priv->istream_iface->readLength(priv->istream, buffer, byte_length);
	CatStringWo *a_result = cat_string_wo_new_with_len(buffer, byte_length);
	free(buffer);
	return a_result;
}
