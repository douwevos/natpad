/*
   File:    jortoken.c
   Project: jordanella
   Author:  Douwe Vos
   Date:    Feb 6, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "jortoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JorToken"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JorToken, jor_token, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jor_token_class_init(JorTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jor_token_init(JorToken *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jor_token_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jor_token_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JorToken *jor_token_new(int code, int start_column, long long start_row, int end_column, long long end_row) {
	JorToken *result = g_object_new(JOR_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	result->code = code;
	result->start_column = start_column;
	result->start_row = start_row;
	result->end_column = end_column;
	result->end_row = end_row;
	return result;
}


const char *jor_token_code_as_text(int code) {
	switch(code) {
	case JOR_SYMBOL_EOF : return "EOF";
	case JOR_SYMBOL_LIST_OPEN : return "LIST_OPEN";
	case JOR_SYMBOL_LIST_CLOSE : return "LIST_CLOSE";
	case JOR_SYMBOL_MAP_OPEN : return "MAP_OPEN";
	case JOR_SYMBOL_MAP_CLOSE : return "MAP_CLOSE";
	case JOR_SYMBOL_STRING : return "STRING";
	case JOR_SYMBOL_COMMA : return "COMMA";
	case JOR_SYMBOL_COLON : return "COLON";
	case JOR_SYMBOL_NULL : return "NULL";
	case JOR_SYMBOL_TRUE : return "TRUE";
	case JOR_SYMBOL_FALSE : return "FALSE";
	case JOR_SYMBOL_NUMBER : return "NUMBER";
	case JOR_SYMBOL_KEY : return "KEY";
	case JOR_SYMBOL_INVALID : return "INVALID";
	case JOR_NTERM_VALUE : return "VALUE";
	}
	return "???";
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JorToken *instance = JOR_TOKEN(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p code=%d]", iname, self, instance->code);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
