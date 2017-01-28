/*
   File:    chaiconverter.h
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

#ifndef CHARSET_CHAICONVERTER_H_
#define CHARSET_CHAICONVERTER_H_

#include <caterpillar.h>

typedef struct _ChaConvertRequest ChaConvertRequest;

struct _ChaConvertRequest {
	/* the input text to convert */
	const char *text;

	/* the byte length of the input text */
	int text_length;

	/* the number of conversion errors */
	int error_count;

	/* the output. If set to null a buffer is created */
	CatStringWo *output;

	gboolean forward_conversion;
};

#define CHA_TYPE_ICONVERTER                 (cha_iconverter_get_type())
#define CHA_ICONVERTER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CHA_TYPE_ICONVERTER, ChaIConverter))
#define CHA_IS_ICONVERTER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CHA_TYPE_ICONVERTER))
#define CHA_ICONVERTER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CHA_TYPE_ICONVERTER, ChaIConverterInterface))


typedef struct _ChaIConverter               ChaIConverter; /* dummy object */
typedef struct _ChaIConverterInterface      ChaIConverterInterface;

struct _CatStringWo;

struct _ChaIConverterInterface {
	GTypeInterface parent_iface;

	gboolean (*forCharset)(ChaIConverter *self, const char *charset_name);
	void (*convert)(ChaIConverter *self, ChaConvertRequest *request);
};

GType cha_iconverter_get_type(void);

gboolean cha_iconverter_for_charset(ChaIConverter *self, const char *charset_name);

void cha_iconverter_convert(ChaIConverter *self, ChaConvertRequest *request);

#endif /* CHARSET_CHAICONVERTER_H_ */
