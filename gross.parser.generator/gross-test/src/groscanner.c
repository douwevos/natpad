/*
   File:    groscanner.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 22, 2016
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

#include "groscanner.h"
#include <logging/catlogdefs.h>

#include "grosymbolext.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroScanner"
#include <logging/catlog.h>



static GroToken *i_scanner_next_token(GroIScanner *self);
static GroToken *i_scanner_create_token(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);

static void i_scanner_iface_init(GroIScannerInterface *iface) {
	iface->next_token = i_scanner_next_token;
	iface->createToken = i_scanner_create_token;
}

G_DEFINE_TYPE_WITH_CODE(GroScanner, gro_scanner, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(GRO_TYPE_ISCANNER, i_scanner_iface_init)
});

static void _dispose(GObject *object);

static CatHashMapWo *l_create_keyword_tokens() {
	CatHashMapWo *e_fixedMap = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("package"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_PACKAGE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("import"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_IMPORT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("code"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_CODE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("action"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_ACTION));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("parser"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_PARSER));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("terminal"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_TERMINAL));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("non"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_NON));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("nonterminal"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_NONTERMINAL));// [CSA]
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("init"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_INIT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("scan"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_SCAN));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("with"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_WITH));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("start"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_START));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("precedence"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_PRECEDENCE));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("left"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_LEFT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("right"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_RIGHT));
	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("nonassoc"), (GObject *) gro_token_new_symbol(GTE_SYMBOL_TERM_NONASSOC));


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


static void gro_scanner_class_init(GroScannerClass *clazz) {

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	clazz->a_keywordMap = cat_ref_sink_ptr(l_create_keyword_tokens());
}

static void gro_scanner_init(GroScanner *obj) {
}

static void _dispose(GObject *object) {
	GroScanner *instance = GRO_SCANNER(object);
	cat_unref_ptr(instance->utf8Scanner);
	cat_unref_ptr(instance->buffer);

}


static void l_next_char(GroScanner *scanner) {
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

static GroToken *_create_token_with_pos(GroScanner *scanner, int terminal_symbol, int left, int left_row, int right, int right_row) {
	return i_scanner_create_token(GRO_ISCANNER(scanner), terminal_symbol, -1, TRUE, FALSE, FALSE, left, left_row, right, right_row, NULL);
}

static GroToken *_create_token_with_pos_val(GroScanner *scanner, int terminal_symbol, int left, int left_row, int right, int right_row, GObject *val) {
	return i_scanner_create_token(GRO_ISCANNER(scanner), terminal_symbol, -1, TRUE, FALSE, FALSE, left, left_row, right, right_row, val);
}


static GroToken *i_scanner_create_token(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	GroScanner *scanner = GRO_SCANNER(self);
	GroToken *result = NULL;
	result = gro_token_new_full_ext(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
	return result;
}


GroScanner *gro_scanner_new(CatIUtf8Scanner *scanner) {
	GroScanner *result = g_object_new(GRO_TYPE_SCANNER, NULL);
	result->utf8Scanner = cat_ref_ptr(scanner);
	result->buffered_token = NULL;
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
	l_next_char(result);
	return result;
}

static gboolean l_is_java_letter_start(gunichar unichar) {
	return ((unichar>='a' && unichar<='z')
			|| (unichar>='A' && unichar<='Z')
			|| (unichar=='_')) ? TRUE : FALSE;
}

static gboolean l_is_java_letter(gunichar unichar) {
	return (l_is_java_letter_start(unichar)
			|| (unichar>='0' && unichar<='9') || unichar=='$') ? TRUE : FALSE;
}

//

static GroToken *l_scan_code(GroScanner *scanner) {
	gboolean last_was_colon = FALSE;
	while(TRUE) {
		if (scanner->lastChar==':') {
			last_was_colon = TRUE;
		} else if (scanner->lastChar=='}') {
			if (last_was_colon) {
				GroToken *result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_CODE_STRING, scanner->mark, scanner->mark_row, scanner->column+1, scanner->row);
				l_next_char(scanner);
				return result;
			}
			last_was_colon = FALSE;
		} else {
			last_was_colon = FALSE;
		}

		l_next_char(scanner);
		if (scanner->streamStatus != CAT_STREAM_OK) {
			cat_log_debug("EOF");
			// TODO mark as unclosed code
			return _create_token_with_pos(scanner, GTE_SYMBOL_TERM_CODE_STRING, scanner->mark, scanner->mark_row, scanner->column, scanner->row);
		}
	}

	return _create_token_with_pos(scanner, GTE_SYMBOL_TERM_CODE_STRING, scanner->mark, scanner->mark_row, scanner->column, scanner->row);
}


static GroToken *l_scan_read_identifier(GroScanner *scanner) {

	int end = scanner->column;
	while(TRUE) {
		if (scanner->mark_row!=scanner->row) {
			break;
		}
		if (l_is_java_letter(scanner->lastChar)) {
			cat_unichar_array_append_uni_char(scanner->buffer, scanner->lastChar);
			end = scanner->column+1;
		} else {
			break;
		}
		l_next_char(scanner);
	}
	GroToken *result = NULL;
	CatStringWo *a_parsedId = cat_unichar_array_to_string(scanner->buffer);
	cat_log_debug("parsedId=%s", cat_string_wo_getchars(a_parsedId));
	GroScannerClass *scannerClass = GRO_SCANNER_GET_CLASS(scanner);
	GroToken *symbolHolder = (GroToken *) cat_hash_map_wo_get(scannerClass->a_keywordMap, a_parsedId);
//	cat_unref_ptr(parsedId);
	cat_log_debug("symbolHolder=%p", symbolHolder);
	if (symbolHolder!=NULL) {
		result = _create_token_with_pos_val(scanner, symbolHolder->sym, scanner->mark, scanner->mark_row, end, scanner->mark_row, (GObject *) a_parsedId);
	} else {
		result = _create_token_with_pos_val(scanner, GTE_SYMBOL_TERM_ID, scanner->mark, scanner->mark_row, end, scanner->mark_row, (GObject *) a_parsedId);
	}
	cat_unref_ptr(a_parsedId);
	cat_unichar_array_set_length(scanner->buffer, 0);
	return result;
}

static GroToken *l_scan_eol_comment(GroScanner *scanner) {
	int row = scanner->row;

	while(TRUE) {
		l_next_char(scanner);
		if ((scanner->streamStatus != CAT_STREAM_OK) || (row != scanner->row)) {
			return _create_token_with_pos(scanner, GTE_SYMBOL_TERM_LINE_COMMENT, scanner->mark, scanner->mark_row, scanner->column, row);
		}
	}
}

static GroToken *l_scan_full_comment(GroScanner *scanner) {

	int step = 0;

	while(TRUE) {
		if ((scanner->streamStatus != CAT_STREAM_OK)) {
			if (scanner->mark==scanner->column) {
				return gro_token_new_symbol(GTE_SYMBOL_TERM_EOF);
			}
			GroToken *result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_FULL_COMMENT, scanner->mark, scanner->mark_row, scanner->column, scanner->mark_row);
			l_next_char(scanner);
			return result;
		}
		if (step==0) {
			if (scanner->lastChar=='*') {
				step++;
			}
		} else if (step==1) {
			if (scanner->lastChar=='/') {
				l_next_char(scanner);
				scanner->inFullComment = FALSE;
				return _create_token_with_pos(scanner, GTE_SYMBOL_TERM_FULL_COMMENT, scanner->mark, scanner->mark_row, scanner->column, scanner->row);
			} else if (scanner->lastChar!='*') {
				step = 0;
			}
		}
		l_next_char(scanner);
	}
}


static GroToken *real_scanner_next_token(GroScanner *scanner) {
	GroToken *result = NULL;
	if (scanner->buffered_token!=NULL) {
		result = scanner->buffered_token;
		scanner->buffered_token = NULL;
		return result;
	}
	cat_log_debug("scanning next");
	if (scanner->streamStatus != CAT_STREAM_OK) {
		cat_log_debug("EOF");
		return _create_token_with_pos(scanner, GTE_SYMBOL_TERM_EOF, 0, scanner->row+1, 0, scanner->row+1);
	}




	gboolean advance = FALSE;
	while(TRUE) {
		if (advance) {
			l_next_char(scanner);
			if (scanner->streamStatus != CAT_STREAM_OK) {
				cat_log_debug("EOF");
				return _create_token_with_pos(scanner, GTE_SYMBOL_TERM_EOF, 0, scanner->row+1,0, scanner->row+1);
			}
		}
		int row = scanner->row;
		scanner->mark = scanner->column;
		scanner->mark_row = scanner->row;
		if (scanner->inFullComment) {
			return l_scan_full_comment(scanner);
		}
		switch(scanner->lastChar) {
			case ';' : {
					result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_SEMI, scanner->mark, scanner->mark_row, scanner->column+1, row);
					l_next_char(scanner);
					return result;
			}
			case ',' : {
					result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_COMMA, scanner->mark, scanner->mark_row, scanner->column+1, row);
					l_next_char(scanner);
					return result;
			}

			case '*' : {
					result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_STAR, scanner->mark, scanner->mark_row, scanner->column+1, row);
					l_next_char(scanner);
					return result;
			}

			case '.' : {
					result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_DOT, scanner->mark, scanner->mark_row, scanner->column+1, row);
					l_next_char(scanner);
					return result;
			}

			case '|' : {
					result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_BAR, scanner->mark, scanner->mark_row, scanner->column+1, row);
					l_next_char(scanner);
					return result;
			}

			case '[' : {
					result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_LBRACK, scanner->mark, scanner->mark_row, scanner->column+1, row);
					l_next_char(scanner);
					return result;
			}

			case ']' : {
					result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_RBRACK, scanner->mark, scanner->mark_row, scanner->column+1, row);
					l_next_char(scanner);
					return result;
			}

			case ':' : {
				l_next_char(scanner);
				if (scanner->lastChar==':') {
					l_next_char(scanner);
					if (scanner->lastChar=='=') {
						result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_COLON_COLON_EQUALS, scanner->mark, scanner->mark_row, scanner->column+1, row);
						l_next_char(scanner);
						return result;
					} else {
						scanner->buffered_token = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_COLON, scanner->mark, scanner->mark_row, scanner->mark+1, row);
						result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_COLON, scanner->mark+1, scanner->mark_row, scanner->mark+2, row);
						return result;
					}

				}
				result = _create_token_with_pos(scanner, GTE_SYMBOL_TERM_COLON, scanner->mark, scanner->mark_row, scanner->mark+1, row);
				return result;
			}

			case '%' : {
				cat_unichar_array_append_uni_char(scanner->buffer, scanner->lastChar);
				return l_scan_read_identifier(scanner);
			}

			case '{' : {
				l_next_char(scanner);
				if (scanner->lastChar==':') {
					l_next_char(scanner);
					return l_scan_code(scanner);
				} else {
					// TODO error
				}
			} break;


			case '/' : {
				l_next_char(scanner);
				if (scanner->lastChar=='/') {
					return l_scan_eol_comment(scanner);
					break;
				} else if (scanner->lastChar=='*') {
					scanner->inFullComment = TRUE;
					l_next_char(scanner);
					return l_scan_full_comment(scanner);
				}
			} break;

			default: {
				if (l_is_java_letter_start(scanner->lastChar)) {
					return l_scan_read_identifier(scanner);
				}
				advance = TRUE;
			} break;
		}
	}
	return result;
}

static GroToken *i_scanner_next_token(GroIScanner *self) {
	GroScanner *scanner = GRO_SCANNER(self);
	GroToken *result = real_scanner_next_token(scanner);
//	if (scanner->filter_non_parsable) {
		if (result->sym==GTE_SYMBOL_TERM_FULL_COMMENT  || result->sym == GTE_SYMBOL_TERM_LINE_COMMENT) {
			cat_unref_ptr(result);
			return i_scanner_next_token(self);	// TODO remove recursion :(
		}
//	}
	cat_log_debug("JagToken[sym=%d, left=%d:%d, right=%d:%d]", result->sym, result->left, result->left_row, result->right, result->right_row);
	return result;
}
