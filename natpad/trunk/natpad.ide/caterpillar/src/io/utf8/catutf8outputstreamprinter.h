/*
 * catutf8outputstreamprinter.h
 *
 *  Created on: Dec 17, 2009
 */

#ifndef CATUTF8OUTPUTSTREAMPRINTER_H_
#define CATUTF8OUTPUTSTREAMPRINTER_H_

#include <glib-object.h>
#include "../catioutputstream.h"
#include "catiutf8printer.h"

G_BEGIN_DECLS

#define CAT_TYPE_UTF8_OUTPUT_STREAM_PRINTER            (cat_utf8_output_stream_printer_get_type())
#define CAT_UTF8_OUTPUT_STREAM_PRINTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_utf8_output_stream_printer_get_type(), CatUtf8OutputStreamPrinter))
#define CAT_UTF8_OUTPUT_STREAM_PRINTER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_UTF8_OUTPUT_STREAM_PRINTER, CatUtf8OutputStreamPrinterClass))
#define CAT_IS_UTF8_OUTPUT_STREAM_PRINTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_UTF8_OUTPUT_STREAM_PRINTER))
#define CAT_IS_UTF8_OUTPUT_STREAM_PRINTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_UTF8_OUTPUT_STREAM_PRINTER))
#define CAT_UTF8_OUTPUT_STREAM_PRINTER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_UTF8_OUTPUT_STREAM_PRINTER, CatUtf8OutputStreamPrinterClass))


typedef struct _CatUtf8OutputStreamPrinter             CatUtf8OutputStreamPrinter;

typedef struct _CatUtf8OutputStreamPrinterClass        CatUtf8OutputStreamPrinterClass;

struct _CatUtf8OutputStreamPrinter {
	GObject parent;
	CatStringWo *e_local_array;
	CatIOutputStream *delegate;
};

struct _CatUtf8OutputStreamPrinterClass {
	GObjectClass parent_class;
};


GType cat_utf8_output_stream_printer_get_type(void);

CatUtf8OutputStreamPrinter *cat_utf8_output_stream_printer_new(CatIOutputStream *delegate);

void cat_utf8_output_stream_printer_close(CatUtf8OutputStreamPrinter *printer);

int cat_utf8_output_stream_printer_print_char(CatUtf8OutputStreamPrinter *printer, gunichar ch, CatStreamStatus *status);

G_END_DECLS

#endif /* CATUTF8OUTPUTSTREAMPRINTER_H_ */
