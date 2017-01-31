/*
   File:    chacharsetconverterfactory.h
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

#ifndef CHARSET_CHACHARSETCONVERTERFACTORY_H_
#define CHARSET_CHACHARSETCONVERTERFACTORY_H_

#include "chaiconverter.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_CHARSET_CONVERTER_FACTORY              (cha_charset_converter_factory_get_type())
#define CHA_CHARSET_CONVERTER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_charset_converter_factory_get_type(), ChaCharsetConverterFactory))
#define CHA_CHARSET_CONVERTER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_CHARSET_CONVERTER_FACTORY, ChaCharsetConverterFactoryClass))
#define CHA_IS_CHARSET_CONVERTER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_CHARSET_CONVERTER_FACTORY))
#define CHA_IS_CHARSET_CONVERTER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_CHARSET_CONVERTER_FACTORY))
#define CHA_CHARSET_CONVERTER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_CHARSET_CONVERTER_FACTORY, ChaCharsetConverterFactoryClass))


typedef struct _ChaCharsetConverterFactory               ChaCharsetConverterFactory;
typedef struct _ChaCharsetConverterFactoryPrivate        ChaCharsetConverterFactoryPrivate;
typedef struct _ChaCharsetConverterFactoryClass          ChaCharsetConverterFactoryClass;


struct _ChaCharsetConverterFactory {
	GObject parent;
};

struct _ChaCharsetConverterFactoryClass {
	GObjectClass parent_class;
};


GType cha_charset_converter_factory_get_type();

ChaCharsetConverterFactory *cha_charset_converter_factory_new();

ChaIConverter *cha_charset_converter_factory_get(ChaCharsetConverterFactory *factory, const char *charset);

CatArrayWo *cha_charset_converter_factory_enlist_names(ChaCharsetConverterFactory *factory);

G_END_DECLS

#endif /* CHARSET_CHACHARSETCONVERTERFACTORY_H_ */
