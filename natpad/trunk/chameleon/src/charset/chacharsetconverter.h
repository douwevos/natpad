/*
   File:    chacharsetconverter.h
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

#ifndef CHARSET_CHACHARSETCONVERTER_H_
#define CHARSET_CHACHARSETCONVERTER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_CHARSET_CONVERTER              (cha_charset_converter_get_type())
#define CHA_CHARSET_CONVERTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_charset_converter_get_type(), ChaCharsetConverter))
#define CHA_CHARSET_CONVERTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_CHARSET_CONVERTER, ChaCharsetConverterClass))
#define CHA_IS_CHARSET_CONVERTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_CHARSET_CONVERTER))
#define CHA_IS_CHARSET_CONVERTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_CHARSET_CONVERTER))
#define CHA_CHARSET_CONVERTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_CHARSET_CONVERTER, ChaCharsetConverterClass))


typedef struct _ChaCharsetConverter               ChaCharsetConverter;
typedef struct _ChaCharsetConverterPrivate        ChaCharsetConverterPrivate;
typedef struct _ChaCharsetConverterClass          ChaCharsetConverterClass;


struct _ChaCharsetConverter {
	GObject parent;
};

struct _ChaCharsetConverterClass {
	GObjectClass parent_class;
};


GType cha_charset_converter_get_type();

ChaCharsetConverter *cha_charset_converter_new();

// dit is een test text. Als het goed is wordt dit juist herkent. Het gaat om een mooie tekst. Over vogeltjes en bijen. Waarom het daar over gaat is onbekend.

ChaCharsetConverter *cha_charset_converter_open(const char *name, const char *filename);


G_END_DECLS

#endif /* CHARSET_CHACHARSETCONVERTER_H_ */
