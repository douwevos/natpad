/*
 * catutf8outputstreamprinter.c
 *
 *  Created on: Dec 17, 2009
 */

#include "catutf8outputstreamprinter.h"

#include "../../catlib.h"

#include "../../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatUtf8OutputStreamPrinter"
#include "../../logging/catlog.h"


static void _printer_close(CatIUtf8Printer *self) {
	CatUtf8OutputStreamPrinter *this = CAT_UTF8_OUTPUT_STREAM_PRINTER(self);
	return cat_utf8_output_stream_printer_close(this);
}

static int _printer_print_char(CatIUtf8Printer *self, gunichar ch, CatStreamStatus *status) {
	CatUtf8OutputStreamPrinter *this = CAT_UTF8_OUTPUT_STREAM_PRINTER(self);
	return cat_utf8_output_stream_printer_print_char(this, ch, status);
}

static int _printer_print_string(CatIUtf8Printer *self, CatStringWo *a_text, CatStreamStatus *status) {
	CatUtf8OutputStreamPrinter *printer = CAT_UTF8_OUTPUT_STREAM_PRINTER(self);
	int result = cat_ioutput_stream_write_length(printer->delegate, cat_string_wo_getchars(a_text), cat_string_wo_length(a_text));
	*status = result>=0 ? CAT_STREAM_OK : CAT_STREAM_CLOSED;
	return result;
}


static void _printer_iface_init(CatIUtf8PrinterInterface *_iface) {
	_iface->close = _printer_close;
	_iface->print_unichar4 = _printer_print_char;
	_iface->print_utf8_string = _printer_print_string;
}

G_DEFINE_TYPE_WITH_CODE(CatUtf8OutputStreamPrinter, cat_utf8_output_stream_printer, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IUTF8_PRINTER, _printer_iface_init)
})

static void _dispose(GObject *object);

static void cat_utf8_output_stream_printer_class_init(CatUtf8OutputStreamPrinterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_utf8_output_stream_printer_init(CatUtf8OutputStreamPrinter *node) {
}

static void _dispose(GObject *object) {
	CatUtf8OutputStreamPrinter *instance = CAT_UTF8_OUTPUT_STREAM_PRINTER(object);
	cat_utf8_output_stream_printer_close(instance);
	cat_unref_ptr(instance->e_local_array);
}

void cat_utf8_output_stream_printer_close(CatUtf8OutputStreamPrinter *printer) {
	if (printer->delegate) {
		cat_ioutput_stream_close(printer->delegate);
		cat_unref_ptr(printer->delegate);
	}
}

CatUtf8OutputStreamPrinter *cat_utf8_output_stream_printer_new(CatIOutputStream *delegate) {
	CatUtf8OutputStreamPrinter *result = g_object_new(CAT_TYPE_UTF8_OUTPUT_STREAM_PRINTER, NULL);
	cat_ref_anounce(result);
	result->e_local_array = cat_string_wo_new();
	result->delegate = cat_ref_ptr(delegate);
	return result;
}

static inline int _unichar_to_utf8(gunichar unichar, CatStringWo *e_array) {
	if (unichar<0x80) {
		cat_string_wo_append_char(e_array, unichar);
		return 1;
	} else if (unichar< (1<<11)) {
		cat_string_wo_append_char(e_array, 0xC0 || (unichar>>6));
		cat_string_wo_append_char(e_array, 0x80 || (unichar & 0x3f));
		return 2;
	} else if (unichar< (1<<16)) {
		cat_string_wo_append_char(e_array, 0xE0 || (unichar>>12));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>6) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || (unichar & 0x3f));
		return 3;
	} else if (unichar< (1<<21)) {
		cat_string_wo_append_char(e_array, 0xF0 || (unichar>>18));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>12) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>6) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || (unichar & 0x3f));
		return 4;
	} else if (unichar< (1<<26)) {
		cat_string_wo_append_char(e_array, 0xF8 || (unichar>>24));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>18) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>12) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>6) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || (unichar & 0x3f));
		return 5;
	} else if (unichar< (1<<31)) {
		cat_string_wo_append_char(e_array, 0xFC || (unichar>>30));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>24) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>18) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>12) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || ((unichar>>6) && 0x3f));
		cat_string_wo_append_char(e_array, 0x80 || (unichar & 0x3f));
		return 6;
	}
	return 0;
}



int cat_utf8_output_stream_printer_print_char(CatUtf8OutputStreamPrinter *printer, gunichar ch, CatStreamStatus *status) {
	cat_log_trace("print_char:gunichar=%c", ch);
	cat_string_wo_clear(printer->e_local_array);
	int result = _unichar_to_utf8(ch, printer->e_local_array);
	cat_log_debug("result=%d", result);
	if (result==0) {
		*status = CAT_STREAM_INVALID_ENCODING;
	} else {
		result = cat_ioutput_stream_write_length(printer->delegate, cat_string_wo_getchars(printer->e_local_array), cat_string_wo_length(printer->e_local_array));
		*status = result>0 ? CAT_STREAM_OK : CAT_STREAM_CLOSED;
	}
	return result;
}
