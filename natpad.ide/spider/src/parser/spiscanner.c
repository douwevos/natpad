///*
//   File:    spiscanner.c
//   Project: spider
//   Author:  Douwe Vos
//   Date:    May 22, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include <logging/catlogdefs.h>
//
//#include "spisymbolext.h"
//#include "spiscanner.h"
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "SpiScanner"
//#include <logging/catlog.h>
//
//
//
//static GroToken *i_scanner_next_token(GroIScanner *self);
//static GroToken *i_scanner_create_token(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);
//
//static void i_scanner_iface_init(GroIScannerInterface *iface) {
//	iface->next_token = i_scanner_next_token;
//	iface->createToken = i_scanner_create_token;
//}
//
//G_DEFINE_TYPE_WITH_CODE(SpiScanner, spi_scanner, G_TYPE_OBJECT, {
//		G_IMPLEMENT_INTERFACE(GRO_TYPE_ISCANNER, i_scanner_iface_init)
//});
//
//static void l_dispose(GObject *object);
//
//static CatHashMapWo *l_create_keyword_tokens() {
//	CatHashMapWo *e_fixedMap = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
//
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("function"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_FUNCTION));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("var"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_VAR));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("return"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_RETURN));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("if"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_IF));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("else"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_ELSE));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("this"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_THIS));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("for"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_FOR));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("null"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_NULL));// [CSA]
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("true"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_TRUE));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("false"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_FALSE));
//
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("delete"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_DELETE));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("void"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_VOID));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("typeof"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_TYPEOF));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("instanceof"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_INSTANCE_OF));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("in"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_IN));
//	cat_hash_map_wo_put(e_fixedMap, (GObject *) cat_string_wo_new_with("new"), (GObject *) gro_token_new_symbol(SPI_SYMBOL_TERM_NEW));
//
//	CatArrayWo *e_keys = cat_hash_map_wo_enlist_keys(e_fixedMap, NULL);
//	CatIIterator *iter = cat_array_wo_iterator(e_keys);
//	cat_unref_ptr(e_keys);
//	while(cat_iiterator_has_next(iter)) {
//		GObject *key = cat_iiterator_next(iter);
//		GObject *val = cat_hash_map_wo_get(e_fixedMap, key);
//		cat_unref_ptr(key);
//		cat_unref_ptr(val);
//	}
//	cat_unref_ptr(iter);
//
//	return e_fixedMap;
//}
//
//
//static void spi_scanner_class_init(SpiScannerClass *clazz) {
//
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	clazz->a_keywordMap = cat_ref_sink_ptr(l_create_keyword_tokens());
//}
//
//static void spi_scanner_init(SpiScanner *obj) {
//}
//
//static void l_dispose(GObject *object) {
//	SpiScanner *instance = SPI_SCANNER(object);
//	cat_unref_ptr(instance->utf8Scanner);
//	cat_unref_ptr(instance->buffer);
//
//}
//
//
//static void l_next_char(SpiScanner *scanner) {
//	if (scanner->streamStatus==CAT_STREAM_OK) {
//		gunichar nchar = cat_iutf8_scanner_next_char(scanner->utf8Scanner, &(scanner->streamStatus));
//		scanner->lastChar = nchar;
//		scanner->column = scanner->nextColumn;
//		if (nchar==0xa || nchar==0xd) {
//			scanner->row++;
//			scanner->nextColumn = 0;
//		} else {
//			scanner->nextColumn++;
//		}
//	}
//}
//
//static GroToken *l_create_token_with_pos(SpiScanner *scanner, int terminal_symbol, int left, int left_row, int right, int right_row) {
//	return i_scanner_create_token(GRO_ISCANNER(scanner), terminal_symbol, -1, TRUE, FALSE, FALSE, left, left_row, right, right_row, NULL);
//}
//
//static GroToken *l_create_token_with_pos_val(SpiScanner *scanner, int terminal_symbol, int left, int left_row, int right, int right_row, GObject *val) {
//	return i_scanner_create_token(GRO_ISCANNER(scanner), terminal_symbol, -1, TRUE, FALSE, FALSE, left, left_row, right, right_row, val);
//}
//
//
//static GroToken *i_scanner_create_token(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
//	GroToken *result = NULL;
//	result = gro_token_new_full_ext(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
//	return result;
//}
//
//
//SpiScanner *spi_scanner_new(CatIUtf8Scanner *scanner) {
//	SpiScanner *result = g_object_new(SPI_TYPE_SCANNER, NULL);
//
//	result->utf8Scanner = cat_ref_ptr(scanner);
//	result->buffered_token = NULL;
//	result->mark = 0;
//	result->row = 0;
//	result->column = 0;
//	result->nextColumn = 0;
//	result->lastChar = 0;
//	result->streamStatus = CAT_STREAM_OK;
//	result->buffer = cat_ref_sink_ptr(cat_unichar_array_new());
//	result->inFullComment = FALSE;
//	result->filter_non_parsable = FALSE;
//	result->create_ast_tokens = FALSE;
//	l_next_char(result);
//	return result;
//}
//
//static GroToken *l_scan_read_string(SpiScanner *scanner) {
//	int row = scanner->row;
//	scanner->mark = scanner->column;
//	scanner->mark_row = scanner->row;
////	l_next_char(scanner);
//
//	gboolean escaped = FALSE;
//
//	int end;
//
//	while(TRUE) {
//		end  = scanner->column+1;
//		l_next_char(scanner);
//		if (scanner->streamStatus != CAT_STREAM_OK) {
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_STRING, scanner->mark, scanner->mark_row, end, row);
//		}
//		if (row != scanner->row) {
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_STRING, scanner->mark, scanner->mark_row, end, row);		/* TODO: mark as unterminated token */
//		}
//
//		if ((scanner->lastChar=='"') && !escaped) {
//			GroToken *result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_STRING, scanner->mark, scanner->mark_row, scanner->column+1, row);
//			l_next_char(scanner);
//			return result;
//		}
//		if (scanner->lastChar=='\\') {
//			escaped = !escaped;
//		} else {
//			escaped = FALSE;
//		}
//	}
//}
//
//
//static gboolean l_is_java_letter_start(gunichar unichar) {
//	return ((unichar>='a' && unichar<='z')
//			|| (unichar>='A' && unichar<='Z')
//			|| (unichar=='_') || (unichar=='$')) ? TRUE : FALSE;
//}
//
//static gboolean l_is_java_letter(gunichar unichar) {
//	return (l_is_java_letter_start(unichar)
//			|| (unichar>='0' && unichar<='9')) ? TRUE : FALSE;
//}
//
//
//#define SPI_SYMBOL_TERM_LONG_LITERAL SPI_SYMBOL_TERM_NUMBER
//#define SPI_SYMBOL_TERM_FLOAT_LITERAL SPI_SYMBOL_TERM_NUMBER
//#define SPI_SYMBOL_TERM_INTEGER_LITERAL SPI_SYMBOL_TERM_NUMBER
//#define SPI_SYMBOL_TERM_DOUBLE_LITERAL SPI_SYMBOL_TERM_NUMBER
//#define SPI_SYMBOL_TERM_OCTAL SPI_SYMBOL_TERM_NUMBER
//#define SPI_SYMBOL_TERM_HEX SPI_SYMBOL_TERM_NUMBER
////define SPI_SYMBOL_TERM_CHARACTER_LITERAL SPI_SYMBOL_TERM_NUMBER
//
//
//static gboolean l_scan_digits(SpiScanner *scanner, int *lastValid) {
//	gboolean got_digit = FALSE;
//	while(TRUE) {
//		if (scanner->lastChar<'0' || scanner->lastChar>'9') {
//			break;
//		}
//		got_digit = TRUE;
//		*lastValid = scanner->column+1;
//		l_next_char(scanner);
//		if (scanner->streamStatus != CAT_STREAM_OK) {
//			break;
//		}
//	}
//	return got_digit;
//}
//
//
//static GroToken *l_scan_hex_octal(SpiScanner *scanner) {
//	int row = scanner->row;
//	int end = scanner->column+1;
//	gboolean isHex = FALSE;
//	if ((scanner->lastChar=='x') || (scanner->lastChar=='X')) {
//		/* '0x' */
//		isHex = TRUE;
//		end = scanner->column+1;
//		l_next_char(scanner);
//	} else if ((scanner->lastChar>='0') && (scanner->lastChar<'8')) {
//		/* '02' */
//		end = scanner->column+1;
//		l_next_char(scanner);
//	} else {
//		/* '0' */
//		return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_INTEGER_LITERAL, scanner->mark, scanner->mark_row, end, row);
//	}
//
//	while(TRUE) {
//		switch(scanner->lastChar) {
//			case '0' : case '1': case '2': case '3': case '4':
//			case '5' : case '6': case '7': {
//			} break;
//			case '8': case '9' : case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
//			case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' : {
//				if (!isHex) {
//					return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end, row);
//				}
//			} break;
//			case 'l' :
//			case 'L' : {
//				end = scanner->column+1;
//				l_next_char(scanner);
//				return l_create_token_with_pos(scanner, isHex ? SPI_SYMBOL_TERM_HEX : SPI_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end , row);
//			} break;
//			default : {
//				return l_create_token_with_pos(scanner, isHex ? SPI_SYMBOL_TERM_HEX : SPI_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end , row);
//			}
//		}
//
//		end = scanner->column+1;
//		l_next_char(scanner);
//		if (scanner->streamStatus != CAT_STREAM_OK) {
//			return l_create_token_with_pos(scanner, isHex ? SPI_SYMBOL_TERM_HEX : SPI_SYMBOL_TERM_OCTAL, scanner->mark, scanner->mark_row, end, row);
//		}
//	}
//	return NULL;
//}
//
//static GroToken *l_scan_number(SpiScanner *scanner) {
//	int row = scanner->row;
//	scanner->mark = scanner->column;
//	scanner->mark_row = scanner->row;
//	int end = scanner->column;
//	int state = 0;
////	gboolean gd;
////	=
//	l_scan_digits(scanner, &end);
//	if (scanner->streamStatus != CAT_STREAM_OK) {
//		return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_INTEGER_LITERAL, scanner->mark, scanner->mark_row, end, row);
//	}
//
//	switch(scanner->lastChar) {
//		case '.' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			state = 1;
//		} break;
//		case 'x' :
//		case 'X' :
//			return l_scan_hex_octal(scanner);
//		case 'e' :
//		case 'E' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			state = 2;
//		} break;
//		case 'l' :
//		case 'L' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_LONG_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//		case 'd' :
//		case 'D' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_DOUBLE_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//		case 'f' :
//		case 'F' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//		default : {
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_INTEGER_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//	}
//
//	if (state==1) {
////		gd =
//		l_scan_digits(scanner, &end);
//		if (scanner->streamStatus != CAT_STREAM_OK) {
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//	}
//
//
//	switch(scanner->lastChar) {
//		case 'e' :
//		case 'E' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			state = 3;
//		} break;
//		case 'd' :
//		case 'D' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_DOUBLE_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//		case 'f' :
//		case 'F' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//		default : {
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//	}
//
//
//	if ((scanner->lastChar=='+') || (scanner->lastChar=='-')) {
//		end = scanner->column+1;
//		l_next_char(scanner);
//	}
//
////	gd =
//	l_scan_digits(scanner, &end);
//
//
//	switch(scanner->lastChar) {
//		case 'd' :
//		case 'D' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_DOUBLE_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//		case 'f' :
//		case 'F' : {
//			end = scanner->column+1;
//			l_next_char(scanner);
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//		default : {
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FLOAT_LITERAL, scanner->mark, scanner->mark_row, end, row);
//		}
//	}
//}
//
//static GroToken *l_scan_read_identifier(SpiScanner *scanner) {
//
//	int end = scanner->column;
//	while(TRUE) {
//		if (scanner->mark_row!=scanner->row) {
//			break;
//		}
//		if (l_is_java_letter(scanner->lastChar)) {
//			cat_unichar_array_append_uni_char(scanner->buffer, scanner->lastChar);
//			end = scanner->column+1;
//		} else {
//			break;
//		}
//		l_next_char(scanner);
//	}
//	GroToken *result = NULL;
//	CatStringWo *a_parsedId = cat_unichar_array_to_string(scanner->buffer);
//	cat_log_debug("parsedId=%s", cat_string_wo_getchars(a_parsedId));
//	SpiScannerClass *scannerClass = SPI_SCANNER_GET_CLASS(scanner);
//	GroToken *symbolHolder = (GroToken *) cat_hash_map_wo_get(scannerClass->a_keywordMap, a_parsedId);
////	cat_unref_ptr(parsedId);
//	cat_log_debug("symbolHolder=%p", symbolHolder);
//	if (symbolHolder!=NULL) {
//		result = l_create_token_with_pos_val(scanner, symbolHolder->sym, scanner->mark, scanner->mark_row, end, scanner->mark_row, (GObject *) a_parsedId);
//	} else {
//		result = l_create_token_with_pos_val(scanner, SPI_SYMBOL_TERM_ID, scanner->mark, scanner->mark_row, end, scanner->mark_row, (GObject *) a_parsedId);
//	}
//	cat_unref_ptr(a_parsedId);
//	cat_unichar_array_set_length(scanner->buffer, 0);
//	return result;
//}
//
//static GroToken *l_scan_eol_comment(SpiScanner *scanner) {
//	int row = scanner->row;
//
//	while(TRUE) {
//		l_next_char(scanner);
//		if ((scanner->streamStatus != CAT_STREAM_OK) || (row != scanner->row)) {
//			return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_LINE_COMMENT, scanner->mark, scanner->mark_row, scanner->column, row);
//		}
//	}
//}
//
//static GroToken *l_scan_full_comment(SpiScanner *scanner) {
//
//	int step = 0;
//
//	while(TRUE) {
//		if ((scanner->streamStatus != CAT_STREAM_OK)) {
//			if (scanner->mark==scanner->column) {
//				return gro_token_new_symbol(SPI_SYMBOL_TERM_EOF);
//			}
//			GroToken *result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FULL_COMMENT, scanner->mark, scanner->mark_row, scanner->column, scanner->mark_row);
//			l_next_char(scanner);
//			return result;
//		}
//		if (step==0) {
//			if (scanner->lastChar=='*') {
//				step++;
//			}
//		} else if (step==1) {
//			if (scanner->lastChar=='/') {
//				l_next_char(scanner);
//				scanner->inFullComment = FALSE;
//				return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_FULL_COMMENT, scanner->mark, scanner->mark_row, scanner->column, scanner->row);
//			} else if (scanner->lastChar!='*') {
//				step = 0;
//			}
//		}
//		l_next_char(scanner);
//	}
//}
//
//static GroToken *l_scan_regular_expression(SpiScanner *scanner) {
//
//	int step = 0;
//
//	int row = scanner->row;
//	int col = scanner->column;
//
//	while(TRUE) {
//		if (row!=scanner->row) {
//			GroToken *result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_REGULAR_EXPRESSION, scanner->mark, scanner->mark_row, col, scanner->mark_row);
//			return result;
//		}
//		if ((scanner->streamStatus != CAT_STREAM_OK)) {
//			if (scanner->mark==scanner->column) {
//				return gro_token_new_symbol(SPI_SYMBOL_TERM_EOF);
//			}
//			GroToken *result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_REGULAR_EXPRESSION, scanner->mark, scanner->mark_row, scanner->column, scanner->mark_row);
//			l_next_char(scanner);
//			return result;
//		}
//		if (step==0) {
//			if (scanner->lastChar=='/') {
//				step++;
//			}
//		} else if (step==1) {
//			if (scanner->lastChar=='/') {
//				l_next_char(scanner);
//				return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_REGULAR_EXPRESSION, scanner->mark, scanner->mark_row, scanner->column, scanner->row);
//			} else if (scanner->lastChar!='*') {
//				step = 0;
//			}
//		}
//		col = scanner->column;
//		l_next_char(scanner);
//	}
//}
//
//
//static GroToken *real_scanner_next_token(SpiScanner *scanner) {
//	GroToken *result = NULL;
//	if (scanner->buffered_token!=NULL) {
//		result = scanner->buffered_token;
//		scanner->buffered_token = NULL;
//		return result;
//	}
//	cat_log_debug("scanning next");
//	if (scanner->streamStatus != CAT_STREAM_OK) {
//		cat_log_debug("EOF");
//		return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_EOF, 0, scanner->row+1, 0, scanner->row+1);
//	}
//
//
//
//
//	gboolean advance = FALSE;
//	while(TRUE) {
//		if (advance) {
//			l_next_char(scanner);
//			if (scanner->streamStatus != CAT_STREAM_OK) {
//				cat_log_debug("EOF");
//				return l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_EOF, 0, scanner->row+1,0, scanner->row+1);
//			}
//		}
//		int row = scanner->row;
//		scanner->mark = scanner->column;
//		scanner->mark_row = scanner->row;
////		if (scanner->inFullComment) {
////			return l_scan_full_comment(scanner);
////		}
//		switch(scanner->lastChar) {
//
//			case '0' : case '1': case '2': case '3': case '4':
//			case '5' : case '6': case '7': case '8': case '9': {
//				return l_scan_number(scanner);
//			}
//			case '"' :  {
//				return l_scan_read_string(scanner);
//			}
//
//
//			case '.' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_DOT, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//			case '?' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_QUESTION, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//			case ':' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_COLON, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//			case ';' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_SEMI, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//			case ',' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_COMMA, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//
//			case '*' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_MUL, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_MUL, scanner->mark, scanner->mark_row, lcol+1, row);
//				}
//				return result;
//			}
//
//			case '%' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_MOD, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_MOD, scanner->mark, scanner->mark_row, lcol+1, row);
//				}
//				return result;
//			}
//
//			case '=' : {
//				int lcol = scanner->column;
//				int lrow = scanner->row;
//				l_next_char(scanner);
//				if (scanner->lastChar=='=') {
//					lcol = scanner->column;
//					l_next_char(scanner);
//					if (scanner->lastChar=='=') {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_TEQ, scanner->mark, scanner->mark_row, scanner->column+1, row);
//						l_next_char(scanner);
//					} else {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_EQ, scanner->mark, scanner->mark_row, lcol+1, row);
//					}
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_ASS_IS, scanner->mark, scanner->mark_row, lcol+1, row);
//				}
//				return result;
//			}
//
//
//			case '+' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='+') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_INC, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_ADD, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ADD, scanner->mark, scanner->mark_row, scanner->mark+1, row);
//				}
//				return result;
//			}
//
//			case '-' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='-') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_DEC, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_SUB, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_SUB, scanner->mark, scanner->mark_row, scanner->mark+1, row);
//				}
//				return result;
//			}
//
//
//			case '<' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='<') {
//					lcol = scanner->column;
//					l_next_char(scanner);
//					if (scanner->lastChar=='=') {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_SHL, scanner->mark, scanner->mark_row, scanner->column+1, row);
//						l_next_char(scanner);
//					} else {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_SHL, scanner->mark, scanner->mark_row, lcol+1, row);
//					}
//				} else if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_LE, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_LT, scanner->mark, scanner->mark_row, scanner->mark+1, row);
//				}
//				return result;
//			}
//
//			case '>' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='>') {
//					lcol = scanner->column;
//					l_next_char(scanner);
//					if (scanner->lastChar=='>') {
//						lcol = scanner->column;
//						l_next_char(scanner);
//						if (scanner->lastChar=='=') {
//							result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_SHRR, scanner->mark, scanner->mark_row, scanner->column+1, row);
//							l_next_char(scanner);
//						} else {
//							result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_SHRR, scanner->mark, scanner->mark_row, lcol+1, row);
//						}
//					} else if (scanner->lastChar=='=') {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_SHR, scanner->mark, scanner->mark_row, scanner->column+1, row);
//						l_next_char(scanner);
//					} else {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_SHR, scanner->mark, scanner->mark_row, lcol+1, row);
//					}
//				} else if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_GE, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_GT, scanner->mark, scanner->mark_row, scanner->mark+1, row);
//				}
//				return result;
//			}
//
//			case '&' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_AND, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else if (scanner->lastChar=='&') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_LAND, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_BAND, scanner->mark, scanner->mark_row, lcol+1, row);
//				}
//				return result;
//			}
//
//			case '|' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_OR, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else if (scanner->lastChar=='|') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_LOR, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_BOR, scanner->mark, scanner->mark_row, lcol+1, row);
//				}
//				return result;
//			}
//
//			case '^' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_XOR, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_BXOR, scanner->mark, scanner->mark_row, lcol+1, row);
//				}
//				return result;
//			}
//
//			case '(' : {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_LPAREN, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//					return result;
//			}
//			case ')' : {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_RPAREN, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//					return result;
//			}
//
//			case '{' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_LBRACE, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//			case '}' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_RBRACE, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//
//			case '[' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_LBRACK, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//			case ']' : {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_RBRACK, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//
//
//			case '~': {
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_NEG, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				l_next_char(scanner);
//				return result;
//			}
//
//			case '!': {
//				int lcol = scanner->column;
//				int lrow = scanner->row;
//				l_next_char(scanner);
//				if (scanner->lastChar=='=') {
//					lcol = scanner->column;
//					l_next_char(scanner);
//					if (scanner->lastChar=='=') {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_TNE, scanner->mark, scanner->mark_row, scanner->column+1, row);
//						l_next_char(scanner);
//					} else {
//						result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_NE, scanner->mark, scanner->mark_row, lcol+1, row);
//					}
//					return result;
//				}
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_NOT, scanner->mark, scanner->mark_row, lcol+1, row);
//				return result;
//			}
//
//			case '/' : {
//				int lcol = scanner->column;
//				l_next_char(scanner);
//				if (scanner->lastChar=='/') {
//					result = l_scan_eol_comment(scanner);
//				} else if (scanner->lastChar=='*') {
//					scanner->inFullComment = TRUE;
//					l_next_char(scanner);
//					result = l_scan_full_comment(scanner);
//				} else if (scanner->lastChar=='=') {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_ASS_DIV, scanner->mark, scanner->mark_row, scanner->column+1, row);
//					l_next_char(scanner);
//				} else if (row==scanner->row) {
//					result = l_scan_regular_expression(scanner);
//				} else {
//					result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_OP_DIV, scanner->mark, scanner->mark_row, lcol+1, row);
//				}
//				return result;
//			} break;
//
//			case 0x0 :
//			case 0xa :
//			case 0xd :
//			case '	' :
//			case ' ' : {
//				advance = TRUE;
//			} break;
//
//			default: {
//				if (l_is_java_letter_start(scanner->lastChar)) {
//					return l_scan_read_identifier(scanner);
//				}
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_ERROR, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				result->is_error = TRUE;
//				l_next_char(scanner);
//				return result;
//
//			} break;
//		}
//	}
//	return result;
//}
//
//static GroToken *i_scanner_next_token(GroIScanner *self) {
//	SpiScanner *scanner = SPI_SCANNER(self);
//	GroToken *result = real_scanner_next_token(scanner);
////	if (result->sym==SPI_SYMBOL_TERM_FULL_COMMENT  || result->sym == SPI_SYMBOL_TERM_LINE_COMMENT) {
////		cat_unref_ptr(result);
////		return i_scanner_next_token(self);	// TODO remove recursion :(
////	}
//	cat_log_debug("SpiToken[sym=%d, left=%d:%d, right=%d:%d, value=%O]", result->sym, result->left, result->left_row, result->right, result->right_row, result->value);
//	return result;
//}
