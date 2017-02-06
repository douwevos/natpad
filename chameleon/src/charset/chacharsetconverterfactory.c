/*
   File:    chacharsetconverterfactory.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 16, 2017
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

#include "chacharsetconverterfactory.h"
#include "chautf8converter.h"
#include "chacharsetconverter.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaCharsetConverterFactory"
#include <logging/catlog.h>

struct _ChaCharsetConverterFactoryPrivate {
	CatLock *lock;
	ChaUtf8Converter *default_converter;
	CatArrayWo *converters;
};

typedef struct _CreatorInfo CreatorInfo;
typedef ChaIConverter *(CreatorCB)(CreatorInfo *info);

struct _CreatorInfo {
	const char *name;
	CreatorCB *creatorCB;
};

static ChaIConverter *l_creator_utf8(CreatorInfo *info) {
	return (ChaIConverter *) cha_utf8_converter_new();
}

static ChaIConverter *l_create_basic(CreatorInfo *info) {
	const char *name = info->name;
	return (ChaIConverter *) cha_charset_converter_open(name, name);
}

static CreatorInfo charset_config[] = {
		{"UTF8", l_creator_utf8},
		{"KOI8-U", l_create_basic},
		{"KOI8-R", l_create_basic},
		{"ISO-8859-5", l_create_basic},
		{NULL, NULL}
};


static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaCharsetConverterFactory, cha_charset_converter_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaCharsetConverterFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_charset_converter_factory_class_init(ChaCharsetConverterFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_charset_converter_factory_init(ChaCharsetConverterFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaCharsetConverterFactory *instance = CHA_CHARSET_CONVERTER_FACTORY(object);
	ChaCharsetConverterFactoryPrivate *priv = cha_charset_converter_factory_get_instance_private(instance);
	cat_unref_ptr(priv->default_converter);
	cat_unref_ptr(priv->converters);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(cha_charset_converter_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_charset_converter_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaCharsetConverterFactory *cha_charset_converter_factory_new() {
	ChaCharsetConverterFactory *result = g_object_new(CHA_TYPE_CHARSET_CONVERTER_FACTORY, NULL);
	cat_ref_anounce(result);
	ChaCharsetConverterFactoryPrivate *priv = cha_charset_converter_factory_get_instance_private(result);
	priv->lock = cat_lock_new();
	priv->converters = cat_array_wo_new();
//	GObject *ob = (GObject *) cha_charset_converter_new();
//	cat_array_wo_append(priv->converters, ob);
//	cat_unref_ptr(ob);

//	ChaCharsetConverter *cs = cha_charset_converter_open("ISO-8859-5", "ISO-8859-5");
//	if (cs!=NULL) {
//		cat_array_wo_append(priv->converters, cs);
//		cat_unref_ptr(cs);
//	}


	priv->default_converter = cha_utf8_converter_new();
	return result;
}

ChaIConverter *cha_charset_converter_factory_get(ChaCharsetConverterFactory *factory, const char *charset) {
	ChaCharsetConverterFactoryPrivate *priv = cha_charset_converter_factory_get_instance_private(factory);
	ChaIConverter *result = NULL;
	cat_lock_lock(priv->lock);

	if (charset==NULL) {
		charset = "UTF8";
	}

	CatIIterator *iter = cat_array_wo_iterator(priv->converters);
	while(cat_iiterator_has_next(iter)) {
		ChaIConverter *converter = cat_iiterator_next(iter);
		if (cha_iconverter_for_charset(converter, charset)) {
			result = converter;
			break;
		}
	}
	cat_unref_ptr(iter);

	if (result==NULL) {

		int idx=0;
		while(TRUE) {
			CreatorInfo info = charset_config[idx];
			if (info.name==NULL) {
				break;
			}
			cat_log_debug("name = %s, charset = %s", info.name, charset);
			if (strcmp(info.name, charset)==0) {
				result = info.creatorCB(&info);
				break;
			}
			idx++;
		}

		if (result!=NULL) {
			cat_log_debug("adding converter:%O", result);
			cat_array_wo_append(priv->converters, (GObject *) result);
			cat_unref(result);
		} else {
			result = (ChaIConverter *) priv->default_converter;
		}
	}

	cat_lock_unlock(priv->lock);
	return result;
}

CatArrayWo *cha_charset_converter_factory_enlist_names(ChaCharsetConverterFactory *factory) {
	ChaCharsetConverterFactoryPrivate *priv = cha_charset_converter_factory_get_instance_private(factory);
	CatArrayWo *result = cat_array_wo_new();

	int idx=0;
	while(TRUE) {
		CreatorInfo info = charset_config[idx];
		if (info.name==NULL) {
			break;
		}
		CatStringWo *name = cat_string_wo_new_with(info.name);
		cat_array_wo_append(result, (GObject *) name);
		cat_unref_ptr(name);
		idx++;
	}

	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaCharsetConverterFactory *instance = CHA_CHARSET_CONVERTER_FACTORY(self);
	ChaCharsetConverterFactoryPrivate *priv = cha_charset_converter_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
