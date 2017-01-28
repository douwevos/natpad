/*
 * catutf8inputstreamscanner.c
 *
 *  Created on: Dec 3, 2009
 */


#include "catutf8inputstreamscanner.h"
#include "catiutf8scanner.h"
#include "../../catlib.h"

#include "../../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatUtf8InputStreamScanner"
#include "../../logging/catlog.h"


static gunichar _scanner_scan_utf8_char(CatIUtf8Scanner *self, CatStreamStatus *status) {
	CatUtf8InputStreamScanner *this = CAT_UTF8_INPUT_STREAM_SCANNER(self);
	return cat_utf8_input_stream_scanner_read(this, status);
}

static int _scanner_scan_utf8_char_length(CatIUtf8Scanner *self, char *data, int length, CatStreamStatus *status) {
	CatUtf8InputStreamScanner *this = CAT_UTF8_INPUT_STREAM_SCANNER(self);
	return cat_utf8_input_stream_scanner_read_length(this, data, length, status);
}

static void _scanner_interface_init(CatIUtf8ScannerInterface *_iface) {
	_iface->scan_utf8_char = _scanner_scan_utf8_char;
	_iface->scan_utf8_char_length = _scanner_scan_utf8_char_length;

}

G_DEFINE_TYPE_WITH_CODE(CatUtf8InputStreamScanner, cat_utf8_input_stream_scanner, G_TYPE_OBJECT,{
	G_IMPLEMENT_INTERFACE(CAT_TYPE_IUTF8_SCANNER, _scanner_interface_init)
})

static void _dispose(GObject *object);

static void cat_utf8_input_stream_scanner_class_init(CatUtf8InputStreamScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_utf8_input_stream_scanner_init(CatUtf8InputStreamScanner *node) {
}

static void _dispose(GObject *object) {
	CatUtf8InputStreamScanner *instance = CAT_UTF8_INPUT_STREAM_SCANNER(object);
	cat_unref_ptr(instance->stream);
}

CatUtf8InputStreamScanner *cat_utf8_input_stream_scanner_new(CatIInputStream *stream) {
	CatUtf8InputStreamScanner *result = g_object_new(CAT_TYPE_UTF8_INPUT_STREAM_SCANNER, NULL);
	cat_ref_anounce(result);
	result->stream = cat_ref_sink_ptr(stream);
	result->buf_size = 0;
	result->buf_mark = 0;
	return result;
}

static inline gunichar _buf_next(CatUtf8InputStreamScanner *stream) {
	if (stream->buf_mark<stream->buf_size) {
		return stream->buf[stream->buf_mark++];
	}
//	int ch = cat_iinput_stream_read(stream);
	int ch = cat_iinput_stream_read(stream->stream);
//	printf("reading from file :%2x\n", ch);
	if (ch<0) {
		return (gunichar) ch;
	}

	stream->buf[stream->buf_size++] = ch;
	stream->buf_mark++;
	return ch;
}


static void _flush(CatUtf8InputStreamScanner *stream) {
	if (stream->buf_mark<stream->buf_size) {
		unsigned char *src = stream->buf+stream->buf_mark;
		unsigned char *dst = stream->buf;
		int len;
		for(len=stream->buf_size-stream->buf_mark; len>0; len--) {
			*dst = *src;
			dst++;
			src++;
		}
	}
	stream->buf_mark = 0;
	stream->buf_size = 0;
}

static inline void _rewind(CatUtf8InputStreamScanner *stream) {
	stream->buf_mark = 0;
}




static inline gunichar _next_char(CatUtf8InputStreamScanner *stream, CatStreamStatus *stream_status) {
	guint i, len;
	gunichar wc = _buf_next(stream);

//	printf("\nwc = %2x", wc);
	if ((int) wc < 0) {
		*stream_status = CAT_STREAM_CLOSED;
		return wc;
	} else if (wc < 0x80) {
		*stream_status = CAT_STREAM_OK;
		return wc;
	} else if (wc < 0xc0) {
		*stream_status = CAT_STREAM_INVALID_ENCODING;
		return (gunichar) wc;
	} else if (wc < 0xe0) {
		len = 2;
		wc &= 0x1f;
	} else if (wc < 0xf0) {
		len = 3;
		wc &= 0x0f;
	} else if (wc < 0xf8) {
		len = 4;
		wc &= 0x07;
	} else if (wc < 0xfc) {
		len = 5;
		wc &= 0x03;
	} else if (wc < 0xfe) {
		len = 6;
		wc &= 0x01;
	} else {
		*stream_status = CAT_STREAM_INVALID_ENCODING;
		return (gunichar) wc;
	}

	*stream_status = CAT_STREAM_OK;
	for (i = 1; i < len; ++i) {
		gunichar ch = _buf_next(stream);
		if ((ch & 0xc0) != 0x80) {
			*stream_status = CAT_STREAM_INVALID_ENCODING;
			stream->buf_mark--;
			break;
		}
//			if (ch) {
//				return (gunichar)-1;
//			} else {
//				return (gunichar)-2;
//			}
//		}

		wc <<= 6;
		wc |= (ch & 0x3f);
	}

//  if (UTF8_LENGTH(wc) != len)
//    return (gunichar)-1;
	return wc;
}





gunichar cat_utf8_input_stream_scanner_read(CatUtf8InputStreamScanner *stream, CatStreamStatus *stream_status) {
	_rewind(stream);
	const gunichar unich = _next_char(stream, stream_status);
//	if (*stream_status = CHAR_STATUS_EOS) {
		_flush(stream);
//	}
	return unich;
}


int cat_utf8_input_stream_scanner_read_length(CatUtf8InputStreamScanner *stream, char *data, int length, CatStreamStatus *stream_status) {
	_rewind(stream);
	int write_count = 0;
	char *output = data;
	unsigned char *in;
	int i;
	while(TRUE) {
		_next_char(stream, stream_status);
//		printf("stream_status==%d\n", *stream_status);
		if (*stream_status==CAT_STREAM_CLOSED) {
//			printf("write_count==%d\n", write_count);
			if (write_count>0) {
				*stream_status = CAT_STREAM_OK;
			}
			_flush(stream);
			break;
		} else if (*stream_status==CAT_STREAM_INVALID_ENCODING) {
			break;
		}
		if (write_count+stream->buf_mark > length) {
			break;
		}
		in = (stream->buf);
		for(i=stream->buf_mark; i>0; i--) {
			*output++ = *in++;
		}
		write_count += stream->buf_mark;
		_flush(stream);
	}
	return write_count;
}

