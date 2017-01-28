/*
   File:    catunichar.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 2, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "catunichar.h"
#include "catlib.h"

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatUnichar"
#include "logging/catlog.h"


G_DEFINE_TYPE(CatUnichar, cat_unichar, G_TYPE_OBJECT)

static void cat_unichar_class_init(CatUnicharClass *clazz) {
}

static void cat_unichar_init(CatUnichar *synax) {
}

CatUnichar *cat_unichar_new(gunichar value) {
	CatUnichar *result = g_object_new(CAT_TYPE_UNICHAR, NULL);
	cat_ref_anounce(result);
	result->value = value;
	return result;
}

int cat_unichar_hash(const CatUnichar *unichar) {
	return (int) unichar->value;
}

gboolean cat_unichar_equal(const CatUnichar *unichar_a, const CatUnichar *unichar_b) {
	if (unichar_a==unichar_b) {
		return TRUE;
	}
	if (unichar_a==NULL || unichar_b==NULL) {
		return FALSE;
	}
	return  unichar_a->value==unichar_b->value;
}
