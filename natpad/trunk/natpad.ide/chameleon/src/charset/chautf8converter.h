/*
   File:    chautf8converter.h
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

#ifndef CHARSET_CHAUTF8CONVERTER_H_
#define CHARSET_CHAUTF8CONVERTER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UTF8_CONVERTER              (cha_utf8_converter_get_type())
#define CHA_UTF8_CONVERTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_utf8_converter_get_type(), ChaUtf8Converter))
#define CHA_UTF8_CONVERTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UTF8_CONVERTER, ChaUtf8ConverterClass))
#define CHA_IS_UTF8_CONVERTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UTF8_CONVERTER))
#define CHA_IS_UTF8_CONVERTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UTF8_CONVERTER))
#define CHA_UTF8_CONVERTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UTF8_CONVERTER, ChaUtf8ConverterClass))


typedef struct _ChaUtf8Converter               ChaUtf8Converter;
typedef struct _ChaUtf8ConverterPrivate        ChaUtf8ConverterPrivate;
typedef struct _ChaUtf8ConverterClass          ChaUtf8ConverterClass;


struct _ChaUtf8Converter {
	GObject parent;
};

struct _ChaUtf8ConverterClass {
	GObjectClass parent_class;
};


GType cha_utf8_converter_get_type();

ChaUtf8Converter *cha_utf8_converter_new();

G_END_DECLS

#endif /* CHARSET_CHAUTF8CONVERTER_H_ */
