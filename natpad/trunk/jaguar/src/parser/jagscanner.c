/*
   File:    jagscanner.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 16, 2010
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

#include "jagscanner.h"
#include "jagasttoken.h"
#include "generated/runtime/jagiscanner.h"
#include "generated/runtime/jagtoken.h"
#include "jagsymbolext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagScanner"
#include <logging/catlog.h>



static JagToken *i_scanner_next_token(JagIScanner *self);
static JagToken *i_scanner_create_token(JagIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);

static void i_scanner_iface_init(JagIScannerInterface *_iface) {
	_iface->next_token = i_scanner_next_token;
	_iface->createToken = i_scanner_create_token;
}

G_DEFINE_TYPE_WITH_CODE(JagScanner, jag_scanner, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_TYPE_ISCANNER, i_scanner_iface_init)
});

static void _dispose(GObject *object);

static CatHashMapWo *l_create_keyword_tokens() {
	CatHashMapWo *e_fixedMap = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("true"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_BOOLEAN_LITERAL));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("false"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_BOOLEAN_LITERAL));

	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("abstract"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_ABSTRACT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("do"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_DO));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("implements"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_IMPLEMENTS));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("package"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_PACKAGE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("throw"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_THROW));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("boolean"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_BOOLEAN));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("double"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_DOUBLE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("import"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_IMPORT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("private"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_PRIVATE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("throws"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_THROWS));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("break"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_BREAK));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("else"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_ELSE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("protected"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_PROTECTED));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("transient"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_TRANSIENT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("byte"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_BYTE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("extends"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_EXTENDS));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("instanceof"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_INSTANCEOF));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("public"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_PUBLIC));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("try"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_TRY));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("case"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_CASE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("final"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_FINAL));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("int"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_INT));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("cast"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_CAST));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("finally"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_FINALLY));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("interface"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_INTERFACE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("return"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_RETURN));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("void"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_VOID));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("catch"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_CATCH));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("float"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_FLOAT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("long"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_LONG));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("short"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_SHORT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("volatile"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_VOLATILE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("char"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_CHAR));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("for"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_FOR));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("native"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_NATIVE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("static"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_STATIC));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("while"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_WHILE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("class"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_CLASS));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("future"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_FUTURE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("new"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_NEW));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("super"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_SUPER));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("const"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_CONST));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("generic"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_GENERIC));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("null"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_NULL));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("switch"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_SWITCH));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("continue"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_CONTINUE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("goto"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_GOTO));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("operator"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_OPERATOR));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("synchronized"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_SYNCHRONIZED));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("default"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_DEFAULT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("if"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_IF));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("this"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_THIS));

	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("assert"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_ASSERT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("strictfp"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_STRICTFP));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("enum"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_ENUM));
	//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("inner"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_INNER));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("rest"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_REST));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("var"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_VAR));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("outer"), (GObject *) jag_token_new_symbol(JAG_SYMBOL_TERM_OUTER));

	CatArrayWo *e_keys = cat_hash_map_wo_enlist_keys(e_fixedMap, NULL);
	CatIIterator *iter = cat_array_wo_iterator(e_keys);
	cat_unref_ptr(e_keys);
	while(cat_iiterator_has_next(iter)) {
		GObject *key = cat_iiterator_next(iter);
		GObject *val = cat_hash_map_wo_get(e_fixedMap, key);
		cat_unref_ptr(key);
		cat_unref_ptr(val);
	}
	cat_unref_ptr(iter);

	return e_fixedMap;
}


static void jag_scanner_class_init(JagScannerClass *clazz) {

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
#ifndef NATPAD_TESTING
	clazz->a_keywordMap = cat_ref_sink_ptr(l_create_keyword_tokens());
#endif
}

static void jag_scanner_init(JagScanner *obj) {
}

static void _dispose(GObject *object) {
	JagScanner *instance = JAG_SCANNER(object);
	cat_unref_ptr(instance->utf8Scanner);
	cat_unref_ptr(instance->buffer);

#ifdef NATPAD_TESTING
	cat_unref_ptr(instance->a_keyword_map);
#endif

}


static void _next_char(JagScanner *scanner) {
	if (scanner->streamStatus==CAT_STREAM_OK) {
		gunichar nchar = cat_iutf8_scanner_next_char(scanner->utf8Scanner, &(scanner->streamStatus));
		scanner->lastChar = nchar;
		scanner->column = scanner->nextColumn;
		if (nchar==0xa || nchar==0xd) {
			scanner->row++;
			scanner->nextColumn = 0;
		} else {
			scanner->nextColumn++;
		}
	}
}

static JagToken *_create_token_with_pos(JagScanner *scanner, int terminal_symbol, int left, int left_row, int right, int right_row) {
	return i_scanner_create_token(JAG_ISCANNER(scanner), terminal_symbol, -1, TRUE, FALSE, FALSE, left, left_row, right, right_row, NULL);
}

static JagToken *_create_token_with_pos_val(JagScanner *scanner, int terminal_symbol, int left, int left_row, int right, int right_row, GObject *val) {
	return i_scanner_create_token(JAG_ISCANNER(scanner), terminal_symbol, -1, TRUE, FALSE, FALSE, left, left_row, right, right_row, val);
}


static JagToken *i_scanner_create_token(JagIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	JagScanner *scanner = JAG_SCANNER(self);
	JagToken *result = NULL;
	if (scanner->create_ast_tokens) {
		result = (JagToken *) jag_ast_token_new(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
	} else {
		result = jag_token_new_full_ext(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
	}
	return result;
}


JagScanner *jag_scanner_new(CatIUtf8Scanner *scanner) {
	JagScanner *result = g_object_new(JAG_TYPE_SCANNER, NULL);
	result->utf8Scanner = cat_ref_ptr(scanner);
	result->mark = 0;
	result->row = 0;
	result->column = 0;
	result->nextColumn = 0;
	result->lastChar = 0;
	result->streamStatus = CAT_STREAM_OK;
	result->buffer = cat_ref_sink_ptr(cat_unichar_array_new());
	result->inFullComment = FALSE;
	result->filter_non_parsable = FALSE;
	result->create_ast_tokens = FALSE;
#ifdef NATPAD_TESTING
	result->a_keyword_map = l_create_keyword_tokens();
#endif

	_next_char(result);
	return result;
}

#define JAG_SYMBOL_TERM_LONG_LITERAL JAG_SYMBOL_TERM_STRING_LITERAL
#define JAG_SYMBOL_TERM_FLOAT_LITERAL JAG_SYMBOL_TERM_STRING_LITERAL
//define JAG_SYMBOL_TERM_INTEGER_LITERAL JAG_SYMBOL_TERM_STRING_LITERAL
#define JAG_SYMBOL_TERM_DOUBLE_LITERAL JAG_SYMBOL_TERM_STRING_LITERAL
#define JAG_SYMBOL_TERM_OCTAL JAG_SYMBOL_TERM_STRING_LITERAL
#define JAG_SYMBOL_TERM_HEX JAG_SYMBOL_TERM_STRING_LITERAL
//define JAG_SYMBOL_TERM_CHARACTER_LITERAL JAG_SYMBOL_TERM_STRING_LITERAL

static gboolean _scan_digits(JagScanner *scanner, int *lastValid) {
	gboolean got_digit = FALSE;
	while(TRUE) {
		if (scanner->lastChar<'0' || scanner->lastChar>'9') {
			break;
		}
		got_digit = TRUE;
		*lastValid = scanner->column+1;
		_next_char(scanner);
		if (scanner->streamStatus != CAT_STREAM_OK) {
			break;
		}
	}
	return got_digit;
}


static JagToken *_scan_hex_octal(JagScanner *scanner) {
	int row = scanner->row;
	int end = scanner->column+1;
	gboolean isHex = FALSE;
	if ((scanner->lastChar=='x') || (scanner->lastChar=='X')) {
		/* '0x' */
		isHex = TRUE;
		end = scanner->column+1;
		_next_char(scanner);
	} else if ((scanner->lastChar>='0') && (scanner->lastChar<'8')) {
		/* '02' */
		end = scanner->column+1;
		_next_char(scanner);
	} else {
		/* '0' */
		return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_INTEGER_LITERAL, scanner->mark, scanner->mark_row, end, row);
	}

	while(TRUE) {
		switch(scanner->lastChar) {
			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': {
			} break;
			case '8': case '9' : case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
			case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' : {
				if (!isHex) {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end, row);
				}
			} break;
			case 'l' :
			case 'L' : {
				end = scanner->column+1;
				_next_char(scanner);
				return _create_token_with_pos(scanner, isHex ? JAG_SYMBOL_TERM_HEX : JAG_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end , row);
			} break;
			default : {
				return _create_token_with_pos(scanner, isHex ? JAG_SYMBOL_TERM_HEX : JAG_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end , row);
			}
		}

		end = scanner->column+1;
		_next_char(scanner);
		if (scanner->streamStatus != CAT_STREAM_OK) {
			return _create_token_with_pos(scanner, isHex ? JAG_SYMBOL_TERM_HEX : JAG_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end, row);
		}
	}
	return NULL;
}

static JagToken *_scan_number(JagScanner *scanner) {
	int row = scanner->row;
	scanner->mark = scanner->column;
	scanner->mark_row = scanner->row;
	int end = scanner->column;
	int state = 0;
//	gboolean gd;
//	=
			_scan_digits(scanner, &end);
	if (scanner->streamStatus != CAT_STREAM_OK) {
		return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_INTEGER_LITERAL, scanner->mark, scanner->mark_row, end, row);
	}

	switch(scanner->lastChar) {
		case '.' : {
			end = scanner->column+1;
			_next_char(scanner);
			state = 1;
		} break;
		case 'x' :
		case 'X' :
			return _scan_hex_octal(scanner);
		case 'e' :
		case 'E' : {
			end = scanner->column+1;
			_next_char(scanner);
			state = 2;
		} break;
		case 'l' :
		case 'L' : {
			end = scanner->column+1;
			_next_char(scanner);
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_LONG_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		case 'd' :
		case 'D' : {
			end = scanner->column+1;
			_next_char(scanner);
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_DOUBLE_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		case 'f' :
		case 'F' : {
			end = scanner->column+1;
			_next_char(scanner);
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		default : {
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_INTEGER_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
	}

	if (state==1) {
//		gd =
				_scan_digits(scanner, &end);
		if (scanner->streamStatus != CAT_STREAM_OK) {
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
	}


	switch(scanner->lastChar) {
		case 'e' :
		case 'E' : {
			end = scanner->column+1;
			_next_char(scanner);
			state = 3;
		} break;
		case 'd' :
		case 'D' : {
			end = scanner->column+1;
			_next_char(scanner);
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_DOUBLE_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		case 'f' :
		case 'F' : {
			end = scanner->column+1;
			_next_char(scanner);
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		default : {
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
	}


	if ((scanner->lastChar=='+') || (scanner->lastChar=='-')) {
		end = scanner->column+1;
		_next_char(scanner);
	}

//	gd =
			_scan_digits(scanner, &end);


	switch(scanner->lastChar) {
		case 'd' :
		case 'D' : {
			end = scanner->column+1;
			_next_char(scanner);
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_DOUBLE_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		case 'f' :
		case 'F' : {
			end = scanner->column+1;
			_next_char(scanner);
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		default : {
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
	}
}


static JagToken *_scan_read_string(JagScanner *scanner) {
	int row = scanner->row;
	scanner->mark = scanner->column;
	scanner->mark_row = scanner->row;
//	_next_char(scanner);

	gboolean escaped = FALSE;

	int end;

	while(TRUE) {
		end  = scanner->column+1;
		_next_char(scanner);
		if (scanner->streamStatus != CAT_STREAM_OK) {
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_STRING_LITERAL, scanner->mark, scanner->mark_row, end, row);
		}
		if (row != scanner->row) {
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_STRING_LITERAL, scanner->mark, scanner->mark_row, end, row);		/* TODO: mark as unterminated token */
		}

		if ((scanner->lastChar=='"') && !escaped) {
			JagToken *result = _create_token_with_pos(scanner, JAG_SYMBOL_TERM_STRING_LITERAL, scanner->mark, scanner->mark_row, scanner->column+1, row);
			_next_char(scanner);
			return result;
		}
		if (scanner->lastChar=='\\') {
			escaped = !escaped;
		} else {
			escaped = FALSE;
		}
	}
}

static gboolean _is_java_letter_start(gunichar unichar) {
	return ((unichar>='a' && unichar<='z')
			|| (unichar>='A' && unichar<='Z')
			|| (unichar=='_')) ? TRUE : FALSE;
}

static gboolean _is_java_letter(gunichar unichar) {
	return (_is_java_letter_start(unichar)
			|| (unichar>='0' && unichar<='9') || unichar=='$') ? TRUE : FALSE;
}


static JagToken *_scan_read_identifier(JagScanner *scanner) {
	cat_unichar_array_set_length(scanner->buffer, 0);
	int row = scanner->row;
	int end = scanner->column;
	while(TRUE) {
		if (row!=scanner->row) {
			break;
		}
		if (_is_java_letter(scanner->lastChar)) {
			cat_unichar_array_append_uni_char(scanner->buffer, scanner->lastChar);
			end = scanner->column+1;
		} else {
			break;
		}
		_next_char(scanner);
	}
	JagToken *result = NULL;
	CatStringWo *a_parsedId = cat_unichar_array_to_string(scanner->buffer);
	cat_log_debug("parsedId=%s", cat_string_wo_getchars(a_parsedId));
#ifdef NATPAD_TESTING
	JagToken *symbolHolder = (JagToken *) cat_hash_map_wo_get(scanner->a_keyword_map, a_parsedId);
#else
	JagScannerClass *scannerClass = JAG_SCANNER_GET_CLASS(scanner);
	JagToken *symbolHolder = (JagToken *) cat_hash_map_wo_get(scannerClass->a_keywordMap, a_parsedId);
#endif
//	cat_unref_ptr(parsedId);
	cat_log_debug("symbolHolder=%p", symbolHolder);
	if (symbolHolder!=NULL) {
		result = _create_token_with_pos_val(scanner, symbolHolder->sym, scanner->mark, scanner->mark_row, end, row, (GObject *) a_parsedId);
	} else {
		result = _create_token_with_pos_val(scanner, JAG_SYMBOL_TERM_IDENTIFIER, scanner->mark, scanner->mark_row, end, row, (GObject *) a_parsedId);
	}
	cat_unref_ptr(a_parsedId);
	return result;
}

static JagToken *l_scan_read_char_literal(JagScanner *scanner) {
	int row = scanner->row;
	if (scanner->lastChar=='\\') {
		_next_char(scanner);
		switch(scanner->lastChar) {
			case 'b' : {
				_next_char(scanner);
			} break;
			case 'f' : {
				_next_char(scanner);
			} break;
			case 'n' : {
				_next_char(scanner);
			} break;
			case 'r' : {
				_next_char(scanner);
			} break;
			case 't' : {
				_next_char(scanner);
			} break;
			case '\\' : {
				_next_char(scanner);
			} break;
			case '"' : {
				_next_char(scanner);
			} break;
			case '\'' : {
				_next_char(scanner);
			} break;
			case 'u' : {
				_next_char(scanner);
				_next_char(scanner);
				_next_char(scanner);
				_next_char(scanner);
			} break;
			case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : {
				_next_char(scanner);
				if (scanner->lastChar=='\'') {
					break;
				}
				_next_char(scanner);
				_next_char(scanner);
			} break;
			default : {
				// TODO ERROR
			} break;
		}
	} else  {
		_next_char(scanner);
	}
	if (scanner->lastChar!='\'') {
		cat_log_error("lexer error with literal char");		// TODO
	}
	_next_char(scanner);
	return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_CHARACTER_LITERAL, scanner->mark, scanner->mark_row, scanner->column, row);
}

static JagToken *_scan_eol_comment(JagScanner *scanner) {
	int row = scanner->row;

	while(TRUE) {
		_next_char(scanner);
		if ((scanner->streamStatus != CAT_STREAM_OK) || (row != scanner->row)) {
			return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_EOL_COMMENT, scanner->mark, scanner->mark_row, scanner->column, row);
		}
	}
}

static JagToken *_scan_full_comment(JagScanner *scanner) {


	CatStringWo *buffer = cat_string_wo_new();
	cat_string_wo_append_chars(buffer, "/*");
	int step = 0;

	while(TRUE) {
		cat_string_wo_append_unichar(buffer, scanner->lastChar);
		if ((scanner->streamStatus != CAT_STREAM_OK)) {
			if (scanner->mark==scanner->column) {
				cat_unref_ptr(buffer);
				return jag_token_new_symbol(JAG_SYMBOL_TERM_EOF);
			}
			JagToken *result = _create_token_with_pos_val(scanner, JAG_SYMBOL_TERM_FULL_COMMENT, scanner->mark, scanner->mark_row, scanner->column, scanner->mark_row, buffer);
			cat_unref_ptr(buffer);
			_next_char(scanner);
			return result;
		}
		if (step==0) {
			if (scanner->lastChar=='*') {
				step++;
			}
		} else if (step==1) {
			if (scanner->lastChar=='/') {
				_next_char(scanner);
				scanner->inFullComment = FALSE;
				JagToken *result = _create_token_with_pos_val(scanner, JAG_SYMBOL_TERM_FULL_COMMENT, scanner->mark, scanner->mark_row, scanner->column, scanner->row, buffer);
				cat_unref_ptr(buffer);
				return result;
			} else if (scanner->lastChar!='*') {
				step = 0;
			}
		}
		_next_char(scanner);
	}
}


static JagToken *real_scanner_next_token(JagScanner *scanner) {
	JagToken *result = NULL;
	cat_log_debug("scanning next");
	if (scanner->streamStatus != CAT_STREAM_OK) {
		cat_log_debug("EOF");
		return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_EOF, 0, scanner->row+1, 0, scanner->row+1);
	}




	gboolean advance = FALSE;
	while(TRUE) {
		if (advance) {
			_next_char(scanner);
			if (scanner->streamStatus != CAT_STREAM_OK) {
				cat_log_debug("EOF");
				return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_EOF, 0, scanner->row+1,0, scanner->row+1);
			}
		}
		int row = scanner->row;
		scanner->mark = scanner->column;
		scanner->mark_row = scanner->row;
		if (scanner->inFullComment) {
			return _scan_full_comment(scanner);
		}
		switch(scanner->lastChar) {

			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': case '8': case '9': {
				return _scan_number(scanner);
			}
			case '"' :  {
				return _scan_read_string(scanner);
			}

			case '\'' :  {
				_next_char(scanner);
				return l_scan_read_char_literal(scanner);
			}


			case '<' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					result = _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LE, scanner->mark, scanner->mark_row, scanner->column+1, row);
					_next_char(scanner);
					return result;
				} else if (scanner->lastChar=='<') {
					_next_char(scanner);
					if (scanner->lastChar=='=') {
						result = _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SHL, scanner->mark, scanner->mark_row, scanner->column+1, row);
						_next_char(scanner);
						return result;
					} else {
						return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SHL, scanner->mark, scanner->mark_row, scanner->column, row);
					}
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LT, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}



			case '>' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_GE, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='>') {
					_next_char(scanner);
					if (scanner->lastChar=='>') {
						_next_char(scanner);
						if (scanner->lastChar=='=') {
							_next_char(scanner);
							return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SHRR, scanner->mark, scanner->mark_row, scanner->column, row);
						} else {
							return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SHRR, scanner->mark, scanner->mark_row, scanner->column, row);
						}
					} else if (scanner->lastChar=='=') {
						_next_char(scanner);
						return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SHR, scanner->mark, scanner->mark_row, scanner->column, row);
					} else {
						return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SHR, scanner->mark, scanner->mark_row, scanner->column, row);
					}
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_GT, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}


			case '=' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_EQ, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_IS, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '!' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_NE, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_NOT, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '^' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_XOR, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_XOR, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '%' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_MOD, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_MOD, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}


			case '|' : {
				_next_char(scanner);
				if (scanner->lastChar=='|') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LOR, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_OR, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_OR, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '&' : {
				_next_char(scanner);
				if (scanner->lastChar=='&') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LAND, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_AND, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_AND, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '+' : {
				_next_char(scanner);
				if (scanner->lastChar=='+') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_INC, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_ADD, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_ADD, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '-' : {
				_next_char(scanner);
				if (scanner->lastChar=='-') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_DEC, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SUB, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SUB, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '*' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_MUL, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_MUL, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '/' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_DIV, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='/') {
					return _scan_eol_comment(scanner);
					break;
				} else if (scanner->lastChar=='*') {
					scanner->inFullComment = TRUE;
					_next_char(scanner);
					return _scan_full_comment(scanner);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_DIV, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}




			case '~': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_NEG, scanner->mark, scanner->mark_row, scanner->column, row);

			case ';': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_SEMICOLON, scanner->mark, scanner->mark_row, scanner->column, row);
			case '(': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_LPAREN, scanner->mark, scanner->mark_row, scanner->column, row);
			case ')': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_RPAREN, scanner->mark, scanner->mark_row, scanner->column, row);
			case '?': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_QUESTION, scanner->mark, scanner->mark_row, scanner->column, row);
			case '{': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_LBRACE, scanner->mark, scanner->mark_row, scanner->column, row);
			case '}': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_RBRACE, scanner->mark, scanner->mark_row, scanner->column, row);
			case '[':
			{
				_next_char(scanner);
				return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_LBRACK, scanner->mark, scanner->mark_row, scanner->column, row);
			}
			case ']': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_RBRACK, scanner->mark, scanner->mark_row, scanner->column, row);
			case '@': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_AT, scanner->mark, scanner->mark_row, scanner->column, row);
			case ':': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_COLON, scanner->mark, scanner->mark_row, scanner->column, row);
			case '.': {
				_next_char(scanner);
				gboolean is_oke = TRUE;
//				if (Character.isDigit(nchar))
//				{
//					item.setLength(0);
//					item.append('.');
//					status=SCAN_READPREEXP;
//				}
//				else
//				{
				if (scanner->lastChar=='.') {
					_next_char(scanner);
					is_oke = FALSE;
					if (scanner->lastChar=='.') {
						_next_char(scanner);
						return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ELLIPSIS, scanner->mark, scanner->mark_row, scanner->column, row);
					}
				}
				JagToken *tok = _create_token_with_pos(scanner, JAG_SYMBOL_TERM_DOT, scanner->mark, scanner->mark_row, scanner->column, row);
				tok->is_error = !is_oke;
				return tok;
//				}
			}
			case ',': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_COMMA, scanner->mark, scanner->mark_row, scanner->column, row);



			default: {
				if (_is_java_letter_start(scanner->lastChar)) {
					return _scan_read_identifier(scanner);
				}
				advance = TRUE;
			} break;
		}
	}
	return result;
}

static JagToken *i_scanner_next_token(JagIScanner *self) {
	JagScanner *scanner = JAG_SCANNER(self);
	JagToken *result = real_scanner_next_token(scanner);
	if (scanner->filter_non_parsable) {
		if (result->sym==JAG_SYMBOL_TERM_FULL_COMMENT  || result->sym == JAG_SYMBOL_TERM_EOL_COMMENT) {
			cat_unref_ptr(result);
			return i_scanner_next_token(self);	// TODO remove recursion :(
		}
	}
	cat_log_debug("JagToken[sym=%d, left=%d:%d, right=%d:%d, sym-text=%s]", result->sym, result->left, result->left_row, result->right, result->right_row, result->symbol_text==NULL ? "<nil>" : result->symbol_text);
	return result;
}
