/*
   File:    jorscanner.c
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

#include "jorscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JorScanner"
#include <logging/catlog.h>

struct _JorScannerPrivate {
	CatIUtf8Scanner *scanner;
	CatStreamStatus stream_status;
	gunichar last_char;
	long long row;
	int column;
	int next_column;
	int mark_column;
	long long mark_row;
	CatUnicharArray *buffer;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JorScanner, jor_scanner, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JorScanner)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jor_scanner_class_init(JorScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jor_scanner_init(JorScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JorScanner *instance = JOR_SCANNER(object);
	JorScannerPrivate *priv = jor_scanner_get_instance_private(instance);
	cat_unref_ptr(priv->buffer);
	cat_unref_ptr(priv->scanner);
	G_OBJECT_CLASS(jor_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jor_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_advance(JorScanner *scanner) {
	JorScannerPrivate *priv = jor_scanner_get_instance_private(scanner);
	if (priv->stream_status==CAT_STREAM_OK) {
		gunichar nchar = cat_iutf8_scanner_next_char(priv->scanner, &(priv->stream_status));
		priv->last_char = nchar;
		priv->column = priv->next_column;
		if (nchar==0xa || nchar==0xd) {
			priv->row++;
			priv->next_column = 0;
		} else {
			priv->next_column++;
		}
	}
}

JorScanner *jor_scanner_new(CatIUtf8Scanner *scanner) {
	JorScanner *result = g_object_new(JOR_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	JorScannerPrivate *priv = jor_scanner_get_instance_private(result);
	priv->scanner = cat_ref_ptr(scanner);
	priv->stream_status = CAT_STREAM_OK;
	priv->buffer = cat_unichar_array_new();
	priv->column = 0;
	priv->row = 0;
	l_advance(result);
	return result;
}


static JorToken *l_create_token_with_pos(JorScanner *scanner, int code, int start_column, long long start_row, int end_column, long long end_row) {
	return jor_token_new(code, start_column, start_row, end_column, end_row);
}

static CatS l_s_txt_null = CAT_S_DEF("null");
static CatS l_s_txt_true = CAT_S_DEF("true");
static CatS l_s_txt_false = CAT_S_DEF("false");

static JorToken *l_scan_literal(JorScanner *scanner) {
	JorScannerPrivate *priv = jor_scanner_get_instance_private(scanner);
	cat_unichar_array_set_length(priv->buffer, 0);
	int row = priv->row;
	int end = priv->column;
	while(TRUE) {
		if (row!=priv->row) {
			break;
		}
		if (g_unichar_isalnum(priv->last_char)) {
			cat_unichar_array_append_uni_char(priv->buffer, priv->last_char);
			end = priv->column+1;
		} else {
			break;
		}
		l_advance(scanner);
	}
	JorToken *result = NULL;
	CatStringWo *parsed_literal = cat_unichar_array_to_string(priv->buffer);
	if (cat_string_wo_equal(parsed_literal, CAT_S(l_s_txt_null))) {
		result = l_create_token_with_pos(scanner, JOR_SYMBOL_NULL, priv->mark_column, priv->mark_row, end, row);
	} else if (cat_string_wo_equal(parsed_literal, CAT_S(l_s_txt_true))) {
		result = l_create_token_with_pos(scanner, JOR_SYMBOL_TRUE, priv->mark_column, priv->mark_row, end, row);
	} else if (cat_string_wo_equal(parsed_literal, CAT_S(l_s_txt_false))) {
		result = l_create_token_with_pos(scanner, JOR_SYMBOL_FALSE, priv->mark_column, priv->mark_row, end, row);
	} else {
		result = l_create_token_with_pos(scanner, JOR_SYMBOL_INVALID, priv->mark_column, priv->mark_row, end, row);
	}

	cat_unref_ptr(parsed_literal);
	return result;
}


static JorToken *l_scan_value(JorScanner *scanner) {
	JorScannerPrivate *priv = jor_scanner_get_instance_private(scanner);
	cat_unichar_array_set_length(priv->buffer, 0);
	int row = priv->row;
	int end = priv->column;
	while(TRUE) {
		if (row!=priv->row) {
			break;
		}
		if (g_unichar_isdigit(priv->last_char) || priv->last_char=='-' || priv->last_char=='e' || priv->last_char=='E' || priv->last_char=='.') {
			cat_unichar_array_append_uni_char(priv->buffer, priv->last_char);
			end = priv->column+1;
		} else {
			break;
		}
		l_advance(scanner);
	}
	JorToken *result = NULL;
	result = l_create_token_with_pos(scanner, JOR_SYMBOL_NUMBER, priv->mark_column, priv->mark_row, end, row);
	return result;
}


static JorToken *l_scan_string(JorScanner *scanner) {
	JorScannerPrivate *priv = jor_scanner_get_instance_private(scanner);
	int row = priv->row;
	priv->mark_column = priv->column;
	priv->mark_row = priv->row;

	gboolean escaped = FALSE;

	int end;

	while(TRUE) {
		end  = priv->column+1;
		l_advance(scanner);
		if (priv->stream_status != CAT_STREAM_OK) {
			return l_create_token_with_pos(scanner, JOR_SYMBOL_STRING, priv->mark_column, priv->mark_row, end, row);
		}
		if (row != priv->row) {
			return l_create_token_with_pos(scanner, JOR_SYMBOL_STRING, priv->mark_column, priv->mark_row, end, row);		/* TODO: mark as unterminated token */
		}

		if ((priv->last_char=='"') && !escaped) {
			JorToken *result = l_create_token_with_pos(scanner, JOR_SYMBOL_STRING, priv->mark_column, priv->mark_row, end+1, row);
			l_advance(scanner);
			return result;
		}
		if (priv->last_char=='\\') {
			escaped = !escaped;
		} else {
			escaped = FALSE;
		}
	}
}

JorToken *jor_scanner_next(JorScanner *scanner) {
	JorScannerPrivate *priv = jor_scanner_get_instance_private(scanner);
	JorToken *result = NULL;
	cat_log_debug("scanning next");
	if (priv->stream_status != CAT_STREAM_OK) {
		cat_log_debug("EOF");
		return l_create_token_with_pos(scanner, JOR_SYMBOL_EOF, 0, priv->row+1, 0, priv->row+1);
	}


	gboolean advance = FALSE;
	while(TRUE) {
		if (advance) {
			advance = FALSE;
			l_advance(scanner);
			if (priv->stream_status != CAT_STREAM_OK) {
				cat_log_debug("EOF");
				return l_create_token_with_pos(scanner, JOR_SYMBOL_EOF, 0, priv->row+1, 0, priv->row+1);
			}
		}

		priv->mark_column = priv->column;
		priv->mark_row = priv->row;

		cat_log_debug("priv->last_char=%d, %c, row=%d", priv->last_char, priv->last_char, priv->row);
		switch(priv->last_char) {
			case '[' : {
				l_advance(scanner);
				return l_create_token_with_pos(scanner, JOR_SYMBOL_LIST_OPEN, priv->mark_column, priv->mark_row, priv->column, priv->mark_row);
			} break;
			case ']' : {
				l_advance(scanner);
				return l_create_token_with_pos(scanner, JOR_SYMBOL_LIST_CLOSE, priv->mark_column, priv->mark_row, priv->column, priv->mark_row);
			} break;
			case '{' : {
				l_advance(scanner);
				return l_create_token_with_pos(scanner, JOR_SYMBOL_MAP_OPEN, priv->mark_column, priv->mark_row, priv->column, priv->mark_row);
			} break;
			case '}' : {
				l_advance(scanner);
				return l_create_token_with_pos(scanner, JOR_SYMBOL_MAP_CLOSE, priv->mark_column, priv->mark_row, priv->column, priv->mark_row);
			} break;
			case ',' : {
				l_advance(scanner);
				return l_create_token_with_pos(scanner, JOR_SYMBOL_COMMA, priv->mark_column, priv->mark_row, priv->column, priv->mark_row);
			} break;
			case ':' : {
				l_advance(scanner);
				return l_create_token_with_pos(scanner, JOR_SYMBOL_COLON, priv->mark_column, priv->mark_row, priv->column, priv->mark_row);
			} break;
			case '"' :
				return l_scan_string(scanner);

			default : {
				advance = TRUE;
				if (g_unichar_isalpha(priv->last_char)) {
					return l_scan_literal(scanner);
				} else if (g_unichar_isdigit(priv->last_char) || priv->last_char=='-') {
					return l_scan_value(scanner);
				}
			}
		}
	}

	return NULL;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JorScanner *instance = JOR_SCANNER(self);
	JorScannerPrivate *priv = jor_scanner_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
