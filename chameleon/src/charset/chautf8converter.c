/*
   File:    chautf8converter.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 17, 2017
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

#include "chautf8converter.h"
#include "chaiconverter.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUtf8Converter"
#include <logging/catlog.h>

struct _ChaUtf8ConverterPrivate {
	CatStringWo *name;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_converter_iface_init(ChaIConverterInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUtf8Converter, cha_utf8_converter, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaUtf8Converter)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_ICONVERTER, l_converter_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_utf8_converter_class_init(ChaUtf8ConverterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_utf8_converter_init(ChaUtf8Converter *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaUtf8Converter *instance = CHA_UTF8_CONVERTER(object);
	ChaUtf8ConverterPrivate *priv = cha_utf8_converter_get_instance_private(instance);
	cat_unref_ptr(priv->name);
	G_OBJECT_CLASS(cha_utf8_converter_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_utf8_converter_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUtf8Converter *cha_utf8_converter_new() {
	ChaUtf8Converter *result = g_object_new(CHA_TYPE_UTF8_CONVERTER, NULL);
	cat_ref_anounce(result);
	ChaUtf8ConverterPrivate *priv = cha_utf8_converter_get_instance_private(result);
	priv->name = cat_string_wo_new_with("UTF-8");
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUtf8Converter *instance = CHA_UTF8_CONVERTER(self);
	ChaUtf8ConverterPrivate *priv = cha_utf8_converter_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* start ChaIConverter implementation *********************/

static void l_convert(ChaIConverter *self, ChaConvertRequest *request) {

	request->error_count = 0;

	if (request->output==NULL) {
		request->output = cat_string_wo_new_with_len(NULL, request->text_length+2);
	} else {
		cat_string_wo_clear(request->output);
	}

	cat_string_wo_set_capacity(request->output, request->text_length+2);

	const char *text = request->text;
	const char *text_end = text+request->text_length;

	char *base = (char *) cat_string_wo_getchars(request->output);

	char *out = base;

	int cp_len = 0;
	while(text<text_end) {


		int fb = 0xff & *text++;
		if ((fb & 0x80)==0) {
			cp_len = 0;
		} else if (fb>=0xc2 && fb<=0xDF) {
			cp_len = 1;
		} else if (fb>=0xE0 && fb<=0xEF) {
			cp_len = 2;
		} else if (fb >= 0xF0 && fb<=0xF4) {
			cp_len = 3;
//		} else if ((fb & 0xFC) == 0xF8) {
//		} else if ((fb & 0xFC) == 0xF8) {
//			cp_len = 4;
//		} else if ((fb & 0xFE) == 0xFC) {
//			cp_len = 5;
		} else {
			request->error_count++;
			*out = '.';
			out++;
			continue;
		}
		if (text+cp_len>text_end) {
			request->error_count++;
			*out = '.';
			out++;
			break;
		}
		*out = (char) fb;
		out++;
		while(cp_len>0) {
			fb = (int) *text++;
			char ob = (char) ((fb & 0x3f) | (0x80));
			if (ob!=fb) {
				request->error_count++;
			}
			*out = ob;
			out++;
			cp_len--;
		}
	}


	cat_log_debug("length=%d", out-base);

	cat_string_wo_append_chars_len(request->output, base, out-base);
}

static CatStringWo *l_get_name(ChaIConverter *self) {
	ChaUtf8ConverterPrivate *priv = cha_utf8_converter_get_instance_private(CHA_UTF8_CONVERTER(self));
	return priv->name;
}


static gboolean l_for_charset(ChaIConverter *self, const char *charset_name) {
	if (charset_name==NULL) {
		return FALSE;
	}

	return strcasecmp("UTF8", charset_name)==0;
}


static void l_converter_iface_init(ChaIConverterInterface *iface) {
	iface->convert = l_convert;
	iface->getName = l_get_name;
	iface->forCharset = l_for_charset;
}


/********************* end ChaIConverter implementation *********************/
