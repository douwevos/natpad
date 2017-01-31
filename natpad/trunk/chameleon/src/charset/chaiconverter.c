/*
   File:    chaiconverter.c
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

#include "chaiconverter.h"

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaIConverter"
#include "logging/catlog.h"

G_DEFINE_INTERFACE(ChaIConverter, cha_iconverter, G_TYPE_OBJECT);

static void cha_iconverter_default_init(ChaIConverterInterface *iface) {
}


gboolean cha_iconverter_for_charset(ChaIConverter *self, const char *charset_name) {
	g_return_val_if_fail(CHA_IS_ICONVERTER(self), FALSE);
	return CHA_ICONVERTER_GET_INTERFACE(self)->forCharset(self, charset_name);
}

void cha_iconverter_convert(ChaIConverter *self, ChaConvertRequest *request) {
	g_return_if_fail(CHA_IS_ICONVERTER(self));
	CHA_ICONVERTER_GET_INTERFACE(self)->convert(self, request);
}

CatStringWo *cha_iconverter_get_name(ChaIConverter *self) {
	g_return_val_if_fail(CHA_IS_ICONVERTER(self), NULL);
	return CHA_ICONVERTER_GET_INTERFACE(self)->getName(self);

}

