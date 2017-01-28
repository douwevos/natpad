/*
 * catutf8inputstreamscanner.h
 *
 *  Created on: Dec 3, 2009
 */

#ifndef CATUTF8INPUTSTREAMSCANNER_H_
#define CATUTF8INPUTSTREAMSCANNER_H_

#include <glib-object.h>
#include "../catiinputstream.h"
#include "../catstream.h"

G_BEGIN_DECLS

#define CAT_TYPE_UTF8_INPUT_STREAM_SCANNER            (cat_utf8_input_stream_scanner_get_type())
#define CAT_UTF8_INPUT_STREAM_SCANNER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_utf8_input_stream_scanner_get_type(), CatUtf8InputStreamScanner))
#define CAT_UTF8_INPUT_STREAM_SCANNER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_UTF8_INPUT_STREAM_SCANNER, CatUtf8InputStreamScannerClass))
#define CAT_IS_UTF8_INPUT_STREAM_SCANNER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_UTF8_INPUT_STREAM_SCANNER))
#define CAT_IS_UTF8_INPUT_STREAM_SCANNER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_UTF8_INPUT_STREAM_SCANNER))
#define CAT_UTF8_INPUT_STREAM_SCANNER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_UTF8_INPUT_STREAM_SCANNER, CatUtf8InputStreamScannerClass))


typedef struct _CatUtf8InputStreamScanner             CatUtf8InputStreamScanner;

typedef struct _CatUtf8InputStreamScannerClass        CatUtf8InputStreamScannerClass;

struct _CatUtf8InputStreamScanner {
	GObject parent;
	CatIInputStream *stream;
	unsigned char buf[10];
	int buf_size;
	int buf_mark;
};

struct _CatUtf8InputStreamScannerClass {
	GObjectClass parent_class;
};


GType cat_utf8_input_stream_scanner_get_type(void);

CatUtf8InputStreamScanner *cat_utf8_input_stream_scanner_new(CatIInputStream *stream);

gunichar cat_utf8_input_stream_scanner_read(CatUtf8InputStreamScanner *stream, CatStreamStatus *stream_status);
int cat_utf8_input_stream_scanner_read_length(CatUtf8InputStreamScanner *stream, char *data, int length, CatStreamStatus *stream_status);


G_END_DECLS

#endif /* CATUTF8INPUTSTREAMSCANNER_H_ */
