/*
   File:    chacharsetconverter.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 14, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "chacharsetconverter.h"
#include "chaiconverter.h"
#include "../document/chapagewo.h"
#include <gio/gio.h>
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaCharsetConverter"
#include <logging/catlog.h>

struct _ChaCharsetConverterPrivate {
	gunichar *data;
	const char *name;
	CatStringWo *s_name;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_converter_iface_init(ChaIConverterInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaCharsetConverter, cha_charset_converter, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaCharsetConverter)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_ICONVERTER, l_converter_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_charset_converter_class_init(ChaCharsetConverterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_charset_converter_init(ChaCharsetConverter *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaCharsetConverter *instance = CHA_CHARSET_CONVERTER(object);
	ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(instance);
	cat_free_ptr(priv->data);
	cat_free_ptr(priv->s_name);
	G_OBJECT_CLASS(cha_charset_converter_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_charset_converter_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


struct _re {
	gunichar *data;
	int min_offset, max_offset;
};

typedef struct _re Req;


static long long l_parse_hex(CatStringWo *text, int *at) {
	int idx = *at;
	if ( (cat_string_wo_char_at(text,idx)=='0') && ((cat_string_wo_char_at(text,idx+1)=='x') || (cat_string_wo_char_at(text,idx+1)=='X'))) {
		idx+=2;
	}


	gboolean has_value = FALSE;

	long result = 0;
	long n;
	for(; idx<cat_string_wo_length(text); idx++) {
		char ch = cat_string_wo_char_at(text, idx);
		if ((ch>='0') && (ch<='9')) {
			n = ch - '0';
		} else if ((ch>='a') && (ch<='f')) {
			n = ch - 'a' + 10;
		} else if ((ch>='A') && (ch<='F')) {
			n = ch - 'A' + 10;
		} else {
			break;
		}
		has_value = TRUE;
		result = result*16 + n;
	}

	if (has_value) {
		*at = idx;
	} else {
		*at = -1;
	}

	return result;
}

static int l_skip_spaces(CatStringWo *t, int idx) {
	while(idx<cat_string_wo_length(t)) {
		char ch = cat_string_wo_char_at(t, idx);
		if (ch==9 || ch==' ') {
			idx++;
			continue;
		}
		break;
	}
	return idx;
}

static gboolean l_scanned_line(char *off_line_start, char *off_line_end, ChaLineEnd line_end, void *data) {
	Req *req = (Req *) data;
	CatStringWo *line = cat_string_wo_new_with_len(off_line_start, off_line_end-off_line_start);

	if (cat_string_wo_beginswith_chars_len(line, "#", 1)) {
		return TRUE;
	}


	cat_log_debug("line = %O", line);

	int idx = 0;
	long long offset = l_parse_hex(line, &idx);
	if (idx>=0) {

		idx = l_skip_spaces(line, idx);

		long long value = l_parse_hex(line, &idx);
		if (idx>=0) {
			cat_log_debug("offset = %d, value=%d", (long long) offset, (long long) value);

			if (offset>=0 && offset<65536) {
				*(req->data+offset) = (gunichar) value;
			} else {
				cat_log_warn("charset has offset outside boundaries");
			}

		}
	}

	return TRUE;
}

ChaCharsetConverter *cha_charset_converter_open(const char *name, const char *filename) {
	CatStringWo *fn = cat_string_wo_new();
	cat_string_wo_format(fn, "/usr/share/natpad/charset/%s.TXT", filename);

	GFile *file = g_file_new_for_path(cat_string_wo_getchars(fn));

	GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	cat_log_debug("info=%o", info);
	if (info==NULL) {
		return NULL;
	}
	cat_unref_ptr(info);

	gsize contents_length = 0;
	char *contents = NULL;
	GError *error = NULL;

	if (g_file_load_contents(file, NULL, &contents, &contents_length, NULL, &error)) {

		Req request;
		request.data = g_malloc(sizeof(gunichar)*65536);
		int idx = 0;
		for(idx=0; idx<65546; idx++) {
			request.data[idx] = -1;
		}
		request.max_offset = -1;
		request.min_offset = -1;

		cha_scan_lines(contents, contents+contents_length, l_scanned_line, &request);

		ChaCharsetConverter *result = g_object_new(CHA_TYPE_CHARSET_CONVERTER, NULL);
		cat_ref_anounce(result);
		ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(result);
		priv->data = request.data;
		priv->name = name;
		priv->s_name = cat_string_wo_new_with(name);
		return result;
	}

	return NULL;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaCharsetConverter *instance = CHA_CHARSET_CONVERTER(self);
	ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* start ChaIConverter implementation *********************/

static void l_convert(ChaIConverter *self, ChaConvertRequest *request) {

	ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(CHA_CHARSET_CONVERTER(self));

	request->error_count = 0;

	if (request->output==NULL) {
		request->output = cat_string_wo_new_with_len(NULL, request->text_length+2);
	} else {
		cat_string_wo_clear(request->output);
	}

	cat_string_wo_set_capacity(request->output, request->text_length+2);

	const char *text = request->text;
	const char *text_end = text+request->text_length;


	CatStringWo *output = request->output;

	if (request->forward_conversion) {

		while(text<text_end) {
			unsigned char ch = (unsigned char) *text;
			gunichar s = *(priv->data+ (0xff & ch));
			cat_log_debug("converted:ch=%d to %d", (long long) ch, (long long) s);
			if (s==-1) {
				cat_string_wo_append_char(output, '.');
				request->error_count++;
			} else {
				cat_string_wo_append_unichar(output, s);
			}
			text++;
		}
	} else {
		const CatStringWo *in = cat_string_wo_new_data_len(text, request->text_length);
		int index = 0;
		while(TRUE) {
			gunichar uch = cat_string_wo_unichar_at(in, &index);
			if (uch==-1) {
				break;
			}
			int idx=0;
			int out_idx = -1;
			for(idx=0; idx<65536; idx++) {
				if (priv->data[idx]==uch) {
					out_idx = idx;
					break;
				}
			}
			if (out_idx==-1) {
				request->error_count++;
				cat_string_wo_append_unichar(output, uch);
			} else if (out_idx<256) {
				cat_string_wo_append_char(output, out_idx);
			} else {
				cat_string_wo_append_char(output, out_idx>>8);
				cat_string_wo_append_char(output, out_idx);
			}
		}
		cat_unref_ptr(in);
	}

}

static gboolean l_for_charset(ChaIConverter *self, const char *charset_name) {
	if (charset_name==NULL) {
		return FALSE;
	}
	ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(CHA_CHARSET_CONVERTER(self));

	return strcasecmp(priv->name, charset_name)==0;
}

static CatStringWo *l_get_name(ChaIConverter *self) {
	ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(CHA_CHARSET_CONVERTER(self));
	return priv->s_name;
}


static void l_converter_iface_init(ChaIConverterInterface *iface) {
	iface->convert = l_convert;
	iface->forCharset = l_for_charset;
	iface->getName = l_get_name;
}

/********************* end ChaIConverter implementation *********************/

