#include "catstdoutstream.h"
#include "catioutputstream.h"
#include <stdio.h>

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatStdoutStream"
#include "../logging/catlog.h"

static void _output_stream_close(CatIOutputStream *self) {
//	CatStdoutStream *stream = CAT_STDOUT_STREAM(self);
	fflush(stdout);
}


static void _output_stream_write(CatIOutputStream *self, char data) {
	CatStdoutStream *this = CAT_STDOUT_STREAM(self);
	CatStreamStatus status;
	cat_stdout_stream_write_length(this, &data, 1, &status);
}

static int _output_stream_write_length(CatIOutputStream *self, const char *data, int length) {
	CatStdoutStream *this = CAT_STDOUT_STREAM(self);
	CatStreamStatus status;
	int result = cat_stdout_stream_write_length(this, data, length, &status);
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

G_DEFINE_TYPE_WITH_CODE(CatStdoutStream, cat_stdout_stream, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IOUTPUT_STREAM, _output_stream_iface_init)
})

static void _dispose(GObject *object);

static void cat_stdout_stream_class_init(CatStdoutStreamClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_stdout_stream_init(CatStdoutStream *instance) {
}

static void _dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	CatStdoutStream *instance = CAT_STDOUT_STREAM(object);
//	cat_log_detail("end-dispose:%p", object);
}


CatStdoutStream *cat_stdout_stream_new() {
	CatStdoutStream *result = g_object_new(CAT_TYPE_STDOUT_STREAM, NULL);
	cat_ref_anounce(result);
	return result;
}

int cat_stdout_stream_write_length(CatStdoutStream *self, const char *data, int length, CatStreamStatus *stream_status) {
	int result = fwrite(data, 1, length, stdout);
	if (result==-1) {
		*stream_status = CAT_STREAM_CLOSED;
	} else {
		*stream_status = CAT_STREAM_OK;
	}
	return result;
}
