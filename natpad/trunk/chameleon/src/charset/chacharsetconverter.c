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

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaCharsetConverter"
#include <logging/catlog.h>


//static gunichar tab_cp1257[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
//			,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63
//			,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95
//			,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127
//			,8364,-1,8218,-1,8222,8230,8224,8225,-1,8240,-1,8249,-1,168,711,184,-1,8216,8217,8220,8221,8226,8211,8212,-1,8482,-1,8250,-1,175,731,-1
//			,160,-1,162,163,164,-1,166,167,216,169,342,171,172,173,174,198,176,177,178,179,180,181,182,183,248,185,343,187,188,189,190,230
//			,260,302,256,262,196,197,280,274,268,201,377,278,290,310,298,315,352,323,325,211,332,213,214,215,370,321,346,362,220,379,381,223
//			,261,303,257,263,228,229,281,275,269,233,378,279,291,311,299,316,353,324,326,243,333,245,246,247,371,322,347,363,252,380,382,729 };

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
//	ChaCharsetConverter *instance = CHA_CHARSET_CONVERTER(object);
//	ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(instance);
	G_OBJECT_CLASS(cha_charset_converter_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_charset_converter_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaCharsetConverter *cha_charset_converter_new() {
	ChaCharsetConverter *result = g_object_new(CHA_TYPE_CHARSET_CONVERTER, NULL);
	cat_ref_anounce(result);
	ChaCharsetConverterPrivate *priv = cha_charset_converter_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
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
	goffset file_length = g_file_info_get_attribute_uint64(info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
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
		CatStringWo *in = cat_string_wo_new_data_len(text, request->text_length);
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




//.
//./harness.c
//./testbench
//./testbench/hw
//./testbench/hw/iso-8859-8.test
//./testbench/hw/utf-8.test
//./testbench/hw/cp1255.test
//./testbench/hw/iso-8859-8.out
//./testbench/hw/cp1255.out
//./testbench/hw/utf-8.out
//./testbench/tr
//./testbench/tr/cp1254.out
//./testbench/tr/utf-8.test
//./testbench/tr/cp1254.test
//./testbench/tr/iso-8859-9.out
//./testbench/tr/utf-8.out
//./testbench/tr/iso-8859-9.test
//./testbench/pl
//./testbench/pl/cp1250.out
//./testbench/pl/utf-8.test
//./testbench/pl/iso8859-2.test
//./testbench/pl/iso8859-2.out
//./testbench/pl/cp1250.test
//./testbench/pl/utf-8.out
//./testbench/gr
//./testbench/gr/iso-8859-7.out
//./testbench/gr/cp1253.out
//./testbench/gr/utf-8.test
//./testbench/gr/iso-8859-7.test
//./testbench/gr/cp1253.test
//./testbench/gr/utf-8.out
//./testbench/jp
//./testbench/jp/euc-jp.test
//./testbench/jp/euc-jp.out
//./testbench/jp/utf-8.test
//./testbench/jp/sjis.out
//./testbench/jp/sjis.test
//./testbench/jp/utf-8.out
//./testbench/bl
//./testbench/bl/iso-8859-13.out
//./testbench/bl/cp1257.out
//./testbench/bl/iso-8859-13.test
//./testbench/bl/utf-8.test
//./testbench/bl/cp1257.test
//./testbench/bl/utf-8.out
//./testbench/ar
//./testbench/ar/iso-8859-6.out
//./testbench/ar/iso-8859-6.test
//./testbench/ar/cp1256.out
//./testbench/ar/utf-8.test
//./testbench/ar/cp1256.test
//./testbench/ar/utf-8.out
//./testbench/ru
//./testbench/ru/koi8-u.test
//./testbench/ru/iso8859-5.test
//./testbench/ru/utf-8.test
//./testbench/ru/koi8-r.out
//./testbench/ru/cp866.out
//./testbench/ru/cp866.test
//./testbench/ru/koi8-r.test
//./testbench/ru/koi8-u.out
//./testbench/ru/cp1251.test
//./testbench/ru/cp1251.out
//./testbench/ru/iso8859-5.out
//./testbench/ru/utf-8.out
//./testbench/Makefile
//./Makefile
