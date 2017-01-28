/*
   File:    spiscannernew.c
   Project: spider
   Author:  Douwe Vos
   Date:    Oct 18, 2016
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

#include "spiscannernew.h"
#include <grossruntime.h>
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SpiScannerNew"
#include <logging/catlog.h>



struct _SpiScannerNewPrivate {
	CatHashMapWo *keyword_mapping;
	GroRunSymbol **connected_symbols;
	CatIUtf8Scanner *input;
	CatStreamStatus input_status;
	GroRunITokenFactory *token_factory;
	CatUnicharArray *lookahead_sec;
	int lookahead_sec_offset;
	gunichar lookahead[5];
	gunichar prevLookahead;
	gunichar nextChar;
	int row, column;
	GroRunLocation *location;

	int index_sequence;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_scanner_iface_init(GroRunIScannerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(SpiScannerNew, spi_scanner_new, G_TYPE_OBJECT,
		G_ADD_PRIVATE(SpiScannerNew)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void spi_scanner_new_class_init(SpiScannerNewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void spi_scanner_new_init(SpiScannerNew *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	SpiScannerNew *instance = SPI_SCANNER_NEW(object);
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(instance);
	cat_unref_ptr(priv->input);
	cat_unref_ptr(priv->location);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->lookahead_sec);
	G_OBJECT_CLASS(spi_scanner_new_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(spi_scanner_new_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static gunichar l_advance(SpiScannerNewPrivate *priv);

SpiScannerNew *spi_scanner_new_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, GroRunSymbol **connected_symbols) {
	SpiScannerNew *result = g_object_new(SPI_TYPE_SCANNER_NEW, NULL);
	cat_ref_anounce(result);
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(result);

	priv->connected_symbols = connected_symbols;
	priv->keyword_mapping = cat_ref_ptr(keyword_mapping);

	priv->input_status = CAT_STREAM_OK;
	priv->input = cat_ref_ptr(scanner);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->lookahead_sec = NULL;

	int idx;
	for(idx=1; idx<5; idx++) {
		priv->lookahead[idx] = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
	}
	priv->nextChar = 0;
	priv->prevLookahead = 0;
	priv->row = 0;
	priv->column = -1;
	priv->location = NULL;
	priv->index_sequence = 0;
	l_advance(priv);
	return result;
}

gboolean spi_scanner_new_is_eof(SpiScannerNew *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(scanner);
	if (grorun_itoken_get_symbol(token) == priv->connected_symbols[SPI_SYM_END_OF_INPUT]) {
		return TRUE;
	}
	return FALSE;
}

int spi_scanner_new_get_symbol_index(SpiScannerNew *scanner, GroRunSymbol *sym) {
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(scanner);
	int idx = 0;
	for(idx=0; idx<16; idx++) {
		if (priv->connected_symbols[idx]==sym) {
			return idx;
		}
	}
	return -1;
}


static gunichar l_next_lookahead(SpiScannerNewPrivate *priv){
	int idx;
	for(idx=1; idx<5; idx++) {
		priv->lookahead[idx-1] = priv->lookahead[idx];
	}
	if (priv->lookahead_sec!=NULL) {
		priv->lookahead[4] = cat_unichar_array_char_at(priv->lookahead_sec, priv->lookahead_sec_offset++);
		if (priv->lookahead_sec_offset>cat_unichar_array_length(priv->lookahead_sec)) {
			cat_unref_ptr(priv->lookahead_sec);
			priv->lookahead_sec_offset = 0;
		}
	} else {
		priv->lookahead[4] = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
	}
	gunichar result = priv->lookahead[0];
	if (result==0xa) {
		if (priv->prevLookahead==0xd) {
			priv->prevLookahead = 0;
		} else {
			priv->prevLookahead = result;
			priv->row++;
		}
		priv->column = -1;
	} else if (result==0xd) {
		if (priv->prevLookahead==0xa) {
			priv->prevLookahead = 0;
		} else {
			priv->prevLookahead = result;
			priv->row++;
		}
		priv->column = -1;
	} else if (result==-1) {
		return -1;
	} else {
		if (result < 0x80) {
			priv->column++;
		} else if (result< (1<<12)) {
			priv->column+= 2;
		} else if (result< (1<<18)) {
			priv->column+= 3;
		} else if (result< (1<<24)) {
			priv->column+= 4;
		} else if (result< (1<<30)) {
			priv->column+= 5;
		} else {
			priv->column+= 6;
		}
		priv->prevLookahead = result;
	}
	return result;
}


static gunichar l_advance(SpiScannerNewPrivate *priv) {
	while(TRUE) {
		priv->nextChar = l_next_lookahead(priv);
		if (priv->nextChar==0xa || priv->nextChar==0xd) {
			continue;
		} else if (priv->nextChar==-1) {
			return -1;
		}
		break;
	}
	return priv->nextChar;
}


static GroRunIToken *l_create_token(SpiScannerNewPrivate *priv, int connSymId, GroRunLocation *c_location, void *c_value) {
	GroRunSymbol *symbol = priv->connected_symbols[connSymId];
	GroRunLeftRight lr;
	lr.left = priv->index_sequence++;
	lr.right = lr.left;
	GroRunIToken *result = grorun_itoken_factory_create_token(priv->token_factory, lr, symbol, (GObject *) c_value, c_location, NULL);
	if (GRORUN_IS_FULL_TOKEN(result)) {
		grorun_full_token_set_user_index((GroRunFullToken *) result, connSymId);
	}
	cat_unref_ptr(c_location);
	cat_unref_ptr(c_value);
	return result;
}

#define SPI_SYM_INTEGER_LITERAL SPI_SYM_NUMBER
#define SPI_SYM_LONG_LITERAL SPI_SYM_NUMBER
#define SPI_SYM_DOUBLE_LITERAL SPI_SYM_NUMBER
#define SPI_SYM_FLOAT_LITERAL SPI_SYM_NUMBER
#define SPI_SYM_HEX SPI_SYM_NUMBER
#define SPI_SYM_OCTAL SPI_SYM_NUMBER

static gboolean l_scan_digits(SpiScannerNewPrivate *priv, int *lastColumn) {
	gboolean got_digit = FALSE;
	while(TRUE) {
		if (priv->nextChar<'0' || priv->nextChar>'9') {
			break;
		}
		got_digit = TRUE;
		*lastColumn = priv->column;
		l_advance(priv);
		if (priv->input_status != CAT_STREAM_OK) {
			break;
		}
	}
	return got_digit;
}

static GroRunIToken *l_scan_hex_octal(SpiScannerNewPrivate *priv) {
	int row = priv->row;
	int end = priv->column+1;
	gboolean isHex = FALSE;
	if ((priv->nextChar=='x') || (priv->nextChar=='X')) {
		/* '0x' */
		isHex = TRUE;
		end = priv->column+1;
		l_advance(priv);
	} else if ((priv->nextChar>='0') && (priv->nextChar<'8')) {
		/* '02' */
		end = priv->column+1;
		l_advance(priv);
	} else {
		/* '0' */
		GroRunLocation *loc = grorun_location_set_end(priv->location, end, row);
		return l_create_token(priv, SPI_SYM_INTEGER_LITERAL, loc, NULL);
	}

	while(TRUE) {
		switch(priv->nextChar) {
			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': {
			} break;
			case '8': case '9' : case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
			case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' : {
				if (!isHex) {
					GroRunLocation *loc = grorun_location_set_end(priv->location, end, row);
					return l_create_token(priv, SPI_SYM_OCTAL, loc, NULL);
				}
			} break;
			case 'l' :
			case 'L' : {
				end = priv->column+1;
				l_advance(priv);
				GroRunLocation *loc = grorun_location_set_end(priv->location, end, row);
				return l_create_token(priv, isHex ? SPI_SYM_HEX : SPI_SYM_OCTAL, loc, NULL);
			} break;
			default : {
				GroRunLocation *loc = grorun_location_set_end(priv->location, end, row);
				return l_create_token(priv, isHex ? SPI_SYM_HEX : SPI_SYM_OCTAL, loc, NULL);
			}
		}

		end = priv->column+1;
		l_advance(priv);
		if (priv->input_status != CAT_STREAM_OK) {
			GroRunLocation *loc = grorun_location_set_end(priv->location, end, row);
			return l_create_token(priv, isHex ? SPI_SYM_HEX : SPI_SYM_OCTAL, loc, NULL);
		}
	}
	return NULL;
}


static GroRunIToken *l_scan_for_number(SpiScannerNewPrivate *priv) {
	int lastCol = priv->column;
	l_scan_digits(priv, &lastCol);
	long lastRow = priv->row;
	if (priv->input_status != CAT_STREAM_OK || lastRow!=priv->row) {
		GroRunLocation *loc = grorun_location_set_end(priv->location, lastCol+1, lastRow);
		return l_create_token(priv, SPI_SYM_INTEGER_LITERAL, loc, NULL);
	}

	int state = 0;
	switch(priv->nextChar) {
		case '.' : {
			l_advance(priv);
			state = 1;
		} break;
		case 'x' :
		case 'X' :
			return l_scan_hex_octal(priv);
		case 'e' :
		case 'E' : {
			l_advance(priv);
			state = 2;
		} break;
		case 'l' :
		case 'L' : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, lastRow);
			l_advance(priv);
			return l_create_token(priv, SPI_SYM_LONG_LITERAL, loc, NULL);
		}
		case 'd' :
		case 'D' : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, lastRow);
			l_advance(priv);
			return l_create_token(priv, SPI_SYM_DOUBLE_LITERAL, loc, NULL);
		}
		case 'f' :
		case 'F' : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, lastRow);
			l_advance(priv);
			return l_create_token(priv, SPI_SYM_FLOAT_LITERAL, loc, NULL);
		}
		default : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, lastCol+1, lastRow);
			return l_create_token(priv, SPI_SYM_INTEGER_LITERAL, loc, NULL);
		}
	}

	lastCol = priv->column;
	if (state==1) {
//		gd =
		l_scan_digits(priv, &lastCol);
		if (priv->input_status != CAT_STREAM_OK) {
			GroRunLocation *loc = grorun_location_set_end(priv->location, lastCol+1, lastRow);
			return l_create_token(priv, SPI_SYM_DOUBLE_LITERAL, loc, NULL);
		}
	}


	switch(priv->nextChar) {
		case 'e' :
		case 'E' : {
			lastCol = priv->column;
			l_advance(priv);
			state = 3;
		} break;
		case 'd' :
		case 'D' : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, lastRow);
			l_advance(priv);
			return l_create_token(priv, SPI_SYM_DOUBLE_LITERAL, loc, NULL);
		}
		case 'f' :
		case 'F' : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, lastRow);
			l_advance(priv);
			return l_create_token(priv, SPI_SYM_FLOAT_LITERAL, loc, NULL);
		}
		default : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, lastCol+1, lastRow);
			return l_create_token(priv, SPI_SYM_FLOAT_LITERAL, loc, NULL);
		}
	}


	if ((priv->nextChar=='+') || (priv->nextChar=='-')) {
		lastCol = priv->column;
		l_advance(priv);
	}

	l_scan_digits(priv, &lastCol);


	switch(priv->nextChar) {
		case 'd' :
		case 'D' : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, lastRow);
			l_advance(priv);
			return l_create_token(priv, SPI_SYM_DOUBLE_LITERAL, loc, NULL);
		}
		case 'f' :
		case 'F' : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, lastRow);
			l_advance(priv);
			return l_create_token(priv, SPI_SYM_FLOAT_LITERAL, loc, NULL);
		}
		default : {
			GroRunLocation *loc = grorun_location_set_end(priv->location, lastCol+1, lastRow);
			return l_create_token(priv, SPI_SYM_FLOAT_LITERAL, loc, NULL);
		}
	}
}





static gboolean l_is_java_letter_start(gunichar unichar) {
	return ((unichar>='a' && unichar<='z')
			|| (unichar>='A' && unichar<='Z')
			|| (unichar=='_') || (unichar=='$')) ? TRUE : FALSE;
}

static gboolean l_is_java_letter(gunichar unichar) {
	return (l_is_java_letter_start(unichar)
			|| (unichar>='0' && unichar<='9')) ? TRUE : FALSE;
}


static GroRunIToken *l_scan_for_id_or_keyword(SpiScannerNew *scanner) {
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(scanner);

	int lastCol = priv->column+1;
	long lastRow = priv->row;
	CatStringWo *buf = cat_string_wo_new();
	while(l_is_java_letter(priv->nextChar) || priv->nextChar=='_' || g_unichar_isdigit(priv->nextChar)) {
		cat_string_wo_append_unichar(buf, priv->nextChar);
		lastCol = priv->column+1;
		lastRow = priv->row;
		l_advance(priv);
		if (priv->row!=lastRow) {
			break;
		}
	}

	int connSymId = SPI_SYM_ID;
	CatInteger *ci = (CatInteger *) cat_hash_map_wo_get(priv->keyword_mapping, buf);
	if (ci!=NULL) {
		connSymId = cat_integer_value(ci);
	}

	return l_create_token(priv, connSymId, grorun_location_set_end(priv->location, lastCol, lastRow), buf);
}

static GroRunIToken *l_scan_for_eol_comment(SpiScannerNewPrivate *priv) {
	int lastCol = priv->column;
	long lastRow = priv->row;
	gunichar next_lah = l_next_lookahead(priv);
	CatStringWo *buf = cat_string_wo_new();
	while(next_lah!=-1) {
		cat_string_wo_append_unichar(buf, next_lah);
		lastCol = priv->column;
		lastRow = priv->row;
		next_lah = l_next_lookahead(priv);
		if (priv->row!=lastRow) {
			break;
		}
	}
	l_advance(priv);

	return l_create_token(priv, SPI_SYM_LINE_COMMENT, grorun_location_set_end(priv->location, lastCol+1, lastRow), buf);
}

static GroRunIToken *l_scan_for_full_comment(SpiScannerNewPrivate *priv) {

	GroRunIToken *result = NULL;
	int lastCol = priv->column;
	long lastRow = priv->row;
	int step = 0;

	while(TRUE) {
		if ((priv->input_status != CAT_STREAM_OK)) {
//			if (scanner->mark==scanner->column) {
//				return gro_token_new_symbol(SPI_SYMBOL_TERM_EOF);
//			}
			GroRunLocation *loc = grorun_location_set_end(priv->location, lastCol+1, lastRow);
			result = l_create_token(priv, SPI_SYM_FULL_COMMENT, loc, NULL);
			break;
		} else if (step==0) {
			if (priv->nextChar=='*') {
				step = 1;
			}
		} else if (step==1) {
			if (priv->nextChar=='/') {
				GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
				result = l_create_token(priv, SPI_SYM_FULL_COMMENT, loc, NULL);
				l_advance(priv);
				break;
			} else if (priv->nextChar!='*') {
				step = 0;
			}
		}
		lastCol = priv->column;
		lastRow = priv->row;
		l_advance(priv);
	}
	return result;
}


static GroRunIToken *l_scan_for_string(SpiScannerNewPrivate *priv) {
	int row = priv->row;

	gboolean escaped = FALSE;
	GroRunIToken *result = NULL;
	int last_col;

	while(result == NULL) {
		last_col  = priv->column;
		l_advance(priv);
		if ((priv->input_status != CAT_STREAM_OK) || (row!=priv->row)) {
			GroRunLocation *loc = grorun_location_set_end(priv->location, last_col+1, row);
			result = l_create_token(priv, SPI_SYM_STRING, loc, NULL);
		} else  if ((priv->nextChar=='"') && !escaped) {
			GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, row);
			result = l_create_token(priv, SPI_SYM_STRING, loc, NULL);
			l_advance(priv);
		} else if (priv->nextChar=='\\') {
			escaped = !escaped;
		} else {
			escaped = FALSE;
		}
	}
	return result;
}


#define L_REGEX_DOT_FWSL_BODY_FWSL_FLAGS                        0
#define L_REGEX_FWSL_DOT_BODY_FWSL_FLAGS                        1
#define L_REGEX_BWSL_CHARS_DOT                                  2
#define L_REGEX_LBRACK_DOT_CLASS_CHARS_RBRACK                   3
#define L_REGEX_FWSL_BODY_FWSL_DOT_FLAGS                        4


static GroRunIToken *l_scan_regular_expression(SpiScannerNew *scanner) {
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(scanner);

	/* create a mark buffer */
	CatUnicharArray *mark_buffer = cat_unichar_array_new();
	cat_unichar_array_append_uni_char(mark_buffer, priv->nextChar);
	long mark_row = priv->row;
	int mark_column = priv->column;

	int last_column = priv->column;

	CatIntArrayWo *state_stack = cat_int_array_wo_new();

	GroRunIToken *result = NULL;
	int state = L_REGEX_DOT_FWSL_BODY_FWSL_FLAGS;
	gboolean keep_scanning = TRUE;
	gboolean scan_next = FALSE;
	while(keep_scanning) {
		if (scan_next) {
			scan_next = FALSE;
			last_column = priv->column;
			priv->nextChar = l_next_lookahead(priv);
			cat_unichar_array_append_uni_char(mark_buffer, priv->nextChar);
		}

		switch(state) {
			case L_REGEX_DOT_FWSL_BODY_FWSL_FLAGS : {
				if (priv->nextChar=='/') {
					state = L_REGEX_FWSL_DOT_BODY_FWSL_FLAGS;
					scan_next = TRUE;
				} else {
					keep_scanning = FALSE;
				}
			} break;
			case L_REGEX_FWSL_DOT_BODY_FWSL_FLAGS : {
				if (priv->nextChar=='\\') {
					cat_int_array_wo_append(state_stack, state);
					state = L_REGEX_BWSL_CHARS_DOT;
					scan_next = TRUE;
				} else if (priv->nextChar=='[') {
					cat_int_array_wo_append(state_stack, state);
					state = L_REGEX_LBRACK_DOT_CLASS_CHARS_RBRACK;
				} else if (priv->nextChar=='/') {
					state = L_REGEX_FWSL_BODY_FWSL_DOT_FLAGS;
					scan_next = TRUE;
				} else if (priv->row!=mark_row) {
					keep_scanning = FALSE;
				} else {
					scan_next = TRUE;
				}
			} break;
			case L_REGEX_BWSL_CHARS_DOT : {
				if (priv->row!=mark_row) {
					keep_scanning = FALSE;
				}
				cat_int_array_wo_remove_last(state_stack, &state);
				scan_next = TRUE;
			} break;

			case L_REGEX_LBRACK_DOT_CLASS_CHARS_RBRACK : {
				if (priv->nextChar==']') {
					cat_int_array_wo_remove_last(state_stack, &state);
					scan_next = TRUE;
				} else if (priv->row!=mark_row) {
					keep_scanning = FALSE;
				} else {
					scan_next = TRUE;
				}
			} break;

			case L_REGEX_FWSL_BODY_FWSL_DOT_FLAGS : {
				if (priv->nextChar=='g' || priv->nextChar=='i' || priv->nextChar=='m' || priv->nextChar=='u' || priv->nextChar=='y') {
					scan_next = TRUE;
				} else {
					keep_scanning = FALSE;
					GroRunLocation *loc = grorun_location_set_end(priv->location, last_column+1, mark_row);
					result = l_create_token(priv, SPI_SYM_REGULAR_EXPRESSION, loc, NULL);
				}

			} break;

		}
	}


	cat_unref_ptr(state_stack);

	if (result==NULL) {
		/* rewind */
		int x;
		for(x=1; x<5; x++) {
			cat_unichar_array_append_uni_char(mark_buffer, priv->lookahead[x]);
		}
		if (priv->lookahead_sec) {
			int cnt = cat_unichar_array_length(priv->lookahead_sec);
			while(priv->lookahead_sec_offset<cnt) {
				gunichar ch = cat_unichar_array_char_at(priv->lookahead_sec, priv->lookahead_sec_offset++);
				cat_unichar_array_append_uni_char(mark_buffer, ch);
			}
			cat_unref_ptr(priv->lookahead_sec);
		}
		for(x=0; x<5; x++) {
			priv->lookahead[x] = cat_unichar_array_char_at(mark_buffer, x);
		}
		priv->lookahead_sec = mark_buffer;
		priv->lookahead_sec_offset = 5;
		if (priv->lookahead_sec_offset>=cat_unichar_array_length(priv->lookahead_sec)) {
			cat_unref_ptr(priv->lookahead_sec);
		}
		priv->nextChar = priv->lookahead[0];
		priv->row = mark_row;
		priv->column = mark_column;
	} else {
		cat_unref_ptr(mark_buffer);
	}

	return result;
}


static CatS l_s_eof = CAT_S_DEF("⟧");

static GroRunIToken *l_next(SpiScannerNew *scanner) {
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(scanner);
	GroRunIToken *result = NULL;
	while(result == NULL) {
		cat_unref_ptr(priv->location);
		priv->location = grorun_location_new(priv->column, priv->row, priv->column+1, priv->row);
		cat_log_debug("priv->nextChar=%x", priv->nextChar);
		switch(priv->nextChar) {

			case -1 :
				result = l_create_token(priv, SPI_SYM_END_OF_INPUT, cat_ref_ptr(priv->location), CAT_S(l_s_eof));
				break;

			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': case '8': case '9':
				result = l_scan_for_number(priv);
				break;

			case '"' :
				result = l_scan_for_string(priv);
				break;



			case '.' :
				result = l_create_token(priv, SPI_SYM_DOT, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case '?' :
				result = l_create_token(priv, SPI_SYM_QUESTION, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case ':' :
				result = l_create_token(priv, SPI_SYM_COLON, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case ';' :
				result = l_create_token(priv, SPI_SYM_SEMI, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case ',' :
				result = l_create_token(priv, SPI_SYM_COMMA, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;

			case '*' :
				if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_MUL, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_MUL, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;


			case '%' :
				if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_MOD, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_MOD, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;



			case '=' :
				if (priv->lookahead[1]=='=') {
					l_advance(priv);
					if (priv->lookahead[1]=='=') {
						l_advance(priv);
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_TEQ, loc, NULL);
					} else {
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_EQ, loc, NULL);
					}
				} else {
					result = l_create_token(priv, SPI_SYM_ASS_IS, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;


			case '+' :
				if (priv->lookahead[1]=='+') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_INC, loc, NULL);
				} else if (priv->lookahead[1]=='-') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_ADD, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_ADD, cat_ref_ptr(priv->location), NULL);
				}

				l_advance(priv);
				break;


			case '-' :
				if (priv->lookahead[1]=='-') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_DEC, loc, NULL);
				} else if (priv->lookahead[1]=='-') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_SUB, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_SUB, cat_ref_ptr(priv->location), NULL);
				}

				l_advance(priv);
				break;

			case '<' :
				if (priv->lookahead[1]=='<') {
					l_advance(priv);
					if (priv->lookahead[1]=='=') {
						l_advance(priv);
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_ASS_SHL, loc, NULL);
					} else {
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_SHL, loc, NULL);
					}
				} else if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_LE, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_LT, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;


			case '>' :
				if (priv->lookahead[1]=='>') {
					l_advance(priv);
					if (priv->lookahead[1]=='>') {
						l_advance(priv);
						if (priv->lookahead[1]=='=') {
							l_advance(priv);
							GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
							result = l_create_token(priv, SPI_SYM_OP_ASS_SHRR, loc, NULL);
						} else {
							GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
							result = l_create_token(priv, SPI_SYM_OP_SHRR, loc, NULL);
						}

					} else if (priv->lookahead[1]=='=') {
						l_advance(priv);
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_ASS_SHR, loc, NULL);
					} else {
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_SHR, loc, NULL);
					}
				} else if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_GE, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_GT, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;


			case '&' :
				if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_AND, loc, NULL);
				} else if (priv->lookahead[1]=='&') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_LAND, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_BAND, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;


			case '|' :
				if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_OR, loc, NULL);
				} else if (priv->lookahead[1]=='|') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_LOR, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_BOR, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;


			case '^' :
				if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_XOR, loc, NULL);
				} else {
					result = l_create_token(priv, SPI_SYM_OP_BXOR, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;


			case '(' :
				result = l_create_token(priv, SPI_SYM_LPAREN, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case ')' :
				result = l_create_token(priv, SPI_SYM_RPAREN, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case '{' :
				result = l_create_token(priv, SPI_SYM_LBRACE, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case '}' :
				result = l_create_token(priv, SPI_SYM_RBRACE, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case '[' :
				result = l_create_token(priv, SPI_SYM_LBRACK, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;
			case ']' :
				result = l_create_token(priv, SPI_SYM_RBRACK, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;

			case '~':
				result = l_create_token(priv, SPI_SYM_OP_NEG, cat_ref_ptr(priv->location), NULL);
				l_advance(priv);
				break;

			case '!':
				if (priv->lookahead[1]=='=') {
					l_advance(priv);
					if (priv->lookahead[1]=='=') {
						l_advance(priv);
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_TNE, loc, NULL);
					} else {
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_NE, loc, NULL);
					}
				} else {
					result = l_create_token(priv, SPI_SYM_OP_NOT, cat_ref_ptr(priv->location), NULL);
				}
				l_advance(priv);
				break;

			case '/':
				if (priv->lookahead[1]=='/') {
					result = l_scan_for_eol_comment(priv);
				} else if (priv->lookahead[1]=='*') {
					result = l_scan_for_full_comment(priv);
				} else if (priv->lookahead[1]=='=') {
					l_advance(priv);
					GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
					result = l_create_token(priv, SPI_SYM_OP_ASS_DIV, loc, NULL);
				} else {
					result = l_scan_regular_expression(scanner);
					if (result==NULL) {
						GroRunLocation *loc = grorun_location_set_end(priv->location, priv->column+1, priv->row);
						result = l_create_token(priv, SPI_SYM_OP_DIV, loc, NULL);
						l_advance(priv);
					}
				}
				break;


			default : {
				if (l_is_java_letter_start(priv->nextChar)) {
					result = l_scan_for_id_or_keyword(scanner);
				} else {
					l_advance(priv);
				}
//				result = l_create_token_with_pos(scanner, SPI_SYMBOL_TERM_ERROR, scanner->mark, scanner->mark_row, scanner->column+1, row);
//				result->is_error = TRUE;
//				l_advance(priv);
//				return result;

			} break;
		}
	}
	return result;
}


/********************* start GroRunIScanner implementation *********************/


static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	SpiScannerNew *scanner = SPI_SCANNER_NEW(self);
	return l_next(scanner);
}


static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
	iface->next = l_scanner_next;
}

/********************* end GroRunIScanner implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	SpiScannerNew *instance = SPI_SCANNER_NEW(self);
	SpiScannerNewPrivate *priv = spi_scanner_new_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, index_sequence=%d]", iname, self, priv->index_sequence);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

