/*
   File:    mulluascanner.c
   Project: mule
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

#include "mulluascanner.h"
#include "mulluasymbolext.h"
#include <grossruntime.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulLuaScanner"
#include <logging/catlog.h>


struct _MulLuaScannerPrivate {
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
	int column, mark_column;
	long long row, mark_row;
	GroRunLocation *location;

	gboolean inMultiLine;
	int multi_line_level, multi_line_symbol;


	CatUnicharArray *buffer;
	CatUnicharArray *look_a_head;
	int look_a_head_index;
	int nextColumn;

	int index_sequence;
};


static void l_scanner_iface_init(GroRunIScannerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulLuaScanner, mul_lua_scanner, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(MulLuaScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
});

static void l_dispose(GObject *object);

static void mul_lua_scanner_class_init(MulLuaScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;


}

static void mul_lua_scanner_init(MulLuaScanner *obj) {
}

static void l_dispose(GObject *object) {
	MulLuaScanner *instance = MUL_LUA_SCANNER(object);
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(instance);
	cat_unref_ptr(priv->input);
	cat_unref_ptr(priv->buffer);
	cat_unref_ptr(priv->look_a_head);
}


static void _next_char(MulLuaScanner *lua_scanner) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	if (cat_unichar_array_length(priv->look_a_head)>0) {
		gunichar nchar = cat_unichar_array_char_at(priv->look_a_head, 0);
		if (priv->look_a_head_index>0) {
			priv->look_a_head_index--;
		}

		cat_unichar_array_remove(priv->look_a_head, 0, 1);
		priv->nextChar = nchar;
		priv->column = priv->nextColumn;
		if (nchar==0xa || nchar==0xd || nchar==0xc) {
			priv->row++;
			priv->nextColumn = 0;
		} else {
			priv->nextColumn++;
		}
		return;
	}
	if (priv->input_status==CAT_STREAM_OK) {
		gunichar nchar = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
		cat_log_detail("nchar='%c', %d", nchar, nchar);
		priv->nextChar = nchar;
		priv->column = priv->nextColumn;
		if (nchar==0xa || nchar==0xd || nchar==0xc) {
			priv->row++;
			priv->nextColumn = 0;
		} else {
			priv->nextColumn++;
		}
	}
}


static void l_look_a_head(MulLuaScanner *lua_scanner) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	if (priv->look_a_head_index>=cat_unichar_array_length(priv->look_a_head)) {
		if (priv->input_status==CAT_STREAM_OK) {
			gunichar nchar = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
			cat_unichar_array_append_uni_char(priv->look_a_head, nchar);
			cat_log_detail("nchar='%c', %d", nchar, nchar);
			priv->nextChar = nchar;
			priv->look_a_head_index++;
		} else {
			priv->nextChar = 0;
		}
		return;
	}

	priv->nextChar = cat_unichar_array_char_at(priv->look_a_head, priv->look_a_head_index++);
}



MulLuaScanner *mul_lua_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, GroRunSymbol **connected_symbols) {
	MulLuaScanner *result = g_object_new(MUL_TYPE_LUA_SCANNER, NULL);
	cat_ref_anounce(result);
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(result);
	priv->connected_symbols = connected_symbols;
	priv->keyword_mapping = cat_ref_ptr(keyword_mapping);

	priv->input_status = CAT_STREAM_OK;
	priv->input = cat_ref_ptr(scanner);
	priv->token_factory = cat_ref_ptr(token_factory);

	priv->mark_column = 0;
	priv->mark_row = 0;
	priv->row = 0;
	priv->column = 0;
	priv->nextColumn = 0;
	priv->nextChar = 0;
	priv->input_status = CAT_STREAM_OK;
	priv->buffer = cat_unichar_array_new();
	priv->look_a_head = cat_unichar_array_new();
	priv->look_a_head_index = 0;
	priv->inMultiLine = FALSE;

	priv->index_sequence = 0;

	_next_char(result);
	return result;
}

gboolean mul_lua_scanner_is_eof(MulLuaScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(scanner);
	if (grorun_itoken_get_symbol(token) == priv->connected_symbols[MUL_LUA_SYM_END_OF_INPUT]) {
		return TRUE;
	}
	return FALSE;
}




static GroRunIToken *l_create_token(MulLuaScannerPrivate *priv, int connSymId, long long end_row, int end_column, void *c_value) {
	GroRunLocation *c_location = grorun_location_new(priv->mark_column, priv->mark_row, end_column, end_row);
	GroRunSymbol *symbol = priv->connected_symbols[connSymId];
	GroRunLeftRight lr;
	lr.left = priv->index_sequence++;
	lr.right = lr.left;
	GroRunIToken *result = grorun_itoken_factory_create_token(priv->token_factory, lr, symbol, (GObject *) c_value, c_location, NULL);
	if (GRORUN_IS_FULL_TOKEN(result)) {
		grorun_full_token_set_user_index((GroRunFullToken *) result, connSymId);
	}
	cat_log_debug("location=%O, sym=%O", c_location, grorun_symbol_get_name(symbol));

	cat_unref_ptr(c_location);
	cat_unref_ptr(c_value);
	return result;
}



#define MUL_LUA_SYM_DECIMAL MUL_LUA_SYM_NUMBER
#define MUL_LUA_SYM_OCTAL MUL_LUA_SYM_NUMBER
#define MUL_LUA_SYM_HEX MUL_LUA_SYM_NUMBER


static gboolean l_scan_digits(MulLuaScanner *lua_scanner, int *lastValid) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	gboolean got_digit = FALSE;
	while(TRUE) {
		if (priv->nextChar<'0' || priv->nextChar>'9') {
			break;
		}
		got_digit = TRUE;
		*lastValid = priv->column+1;
		_next_char(lua_scanner);
		if (priv->input_status != CAT_STREAM_OK) {
			break;
		}
	}
	return got_digit;
}


static gboolean l_scan_extended_digits(MulLuaScanner *lua_scanner, int *lastValid) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	gboolean got_digit = FALSE;
	while((priv->nextChar>='0' && priv->nextChar<='9')
				|| (priv->nextChar>='A' && priv->nextChar<='F')
				|| (priv->nextChar>='a' && priv->nextChar<='f')) {
		got_digit = TRUE;
		*lastValid = priv->column+1;
		_next_char(lua_scanner);
		if (priv->input_status != CAT_STREAM_OK) {
			break;
		}
	}
	return got_digit;
}

//    {integer}(\.{integer})?({exponent})?
// |  base HASH based_integer (DOT based_integer)? HASH ([eE][-+]?{integer})?
// |  (base COLON based_integer (DOT based_integer)? COLON ([eE][-+]?{integer})?)


static GroRunIToken *l_scan_number(MulLuaScanner *lua_scanner, gboolean skip_first_digits) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	priv->mark_column = priv->column;
	priv->mark_row = priv->row;
	int end = priv->column;
	int state = 0;
	if (!skip_first_digits) {
		l_scan_digits(lua_scanner, &end);
		if (priv->input_status != CAT_STREAM_OK) {
			return l_create_token(priv, MUL_LUA_SYM_NUMBER, priv->mark_row, end, NULL);
		}

		switch(priv->nextChar) {
			case '.' : {
				end = priv->column+1;
				_next_char(lua_scanner);
				state = 1;
			} break;
			case '#' :
				end = priv->column+1;
				_next_char(lua_scanner);
				state = 2;
				break;
			case ':' :
				end = priv->column+1;
				_next_char(lua_scanner);
				state = 3;
				break;
			case 'e' :
			case 'E' : {
				end = priv->column+1;
				_next_char(lua_scanner);
				state = 4;
			} break;
			case 'x' : {
				end = priv->column+1;
				_next_char(lua_scanner);
				state = 6;
			} break;
			default : {
				return l_create_token(priv, MUL_LUA_SYM_NUMBER, priv->mark_row, end, NULL);
			}
		}
	} else {
		state = 1;
	}

	if (state==1) {
		l_scan_digits(lua_scanner, &end);
		state = 5;
	}

	if (state==2 || state==3) {
		l_scan_extended_digits(lua_scanner, &end);
		if (priv->nextChar=='.') {
			_next_char(lua_scanner);
			l_scan_extended_digits(lua_scanner, &end);
		}
	}

	if (state==2) {
		if (priv->nextChar=='#') {
			_next_char(lua_scanner);
			state = 5;
		} else {
			end = priv->column;
			return l_create_token(priv, MUL_LUA_SYM_NUMBER, priv->mark_row, end, NULL);
		}
	}

	if (state==3) {
		if (priv->nextChar==':') {
			_next_char(lua_scanner);
			state = 5;
		} else {
			end = priv->column;
			return l_create_token(priv, MUL_LUA_SYM_NUMBER, priv->mark_row, end, NULL);
		}
	}



	if (state==5) {
		if ((priv->nextChar=='e') || (priv->nextChar=='E')) {
			_next_char(lua_scanner);
			state = 4;
		} else {
			end = priv->column;
			return l_create_token(priv, MUL_LUA_SYM_NUMBER, priv->mark_row, end, NULL);
		}
	}

	if (state==4) {
		if ((priv->nextChar=='-') || (priv->nextChar=='+')) {
			_next_char(lua_scanner);
		}
		end = priv->column;
		l_scan_digits(lua_scanner, &end);
	}

	if (state==6) {
		while(TRUE) {
			if ((priv->nextChar>='0' && priv->nextChar<='9')
					|| (priv->nextChar>='a' && priv->nextChar<='f')
					|| (priv->nextChar>='A' && priv->nextChar<='F')) {
				_next_char(lua_scanner);
				continue;
			}
			break;
		}
		end = priv->column;
	}


	return l_create_token(priv, MUL_LUA_SYM_NUMBER, priv->mark_row, end, NULL);
}



static GroRunIToken *l_scan_read_string(MulLuaScanner *lua_scanner) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	int row = priv->row;
	priv->mark_column = priv->column;
	priv->mark_row = priv->row;

	gboolean escaped = FALSE;
	gunichar mark_char = priv->nextChar;

	int end;

	GroRunIToken *result = NULL;
	while(TRUE) {
		end  = priv->column+1;
		_next_char(lua_scanner);
		if (priv->input_status != CAT_STREAM_OK) {
			result = l_create_token(priv, MUL_LUA_SYM_STRING, priv->mark_row, end, NULL);
			break;
		}
		if (row != priv->row) {
			result = l_create_token(priv, MUL_LUA_SYM_STRING, priv->mark_row, end, NULL);		/* TODO: mark as unterminated token */
			break;
		}

		if ((priv->nextChar==mark_char) && !escaped) {
			result = l_create_token(priv, MUL_LUA_SYM_STRING, priv->mark_row, priv->column+1, NULL);
			_next_char(lua_scanner);
			break;
		}
		if (priv->nextChar=='\\') {
			escaped = !escaped;
		} else {
			escaped = FALSE;
		}
	}
	return result;
}

static gboolean _is_lua_letter_start(gunichar unichar) {
	return ((unichar>='a' && unichar<='z')
			|| (unichar>='A' && unichar<='Z')
			|| (unichar=='_')) ? TRUE : FALSE;
}

static gboolean _is_lua_letter(gunichar unichar) {
	return (_is_lua_letter_start(unichar)
			|| (unichar>='0' && unichar<='9')) ? TRUE : FALSE;
}


static GroRunIToken *l_scan_read_identifier_continue(MulLuaScanner *lua_scanner, int row, int end) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	while(TRUE) {
		if (row!=priv->row) {
			break;
		}
		if (_is_lua_letter(priv->nextChar)) {
			cat_unichar_array_append_uni_char(priv->buffer, priv->nextChar);
			end = priv->column+1;
		} else {
			break;
		}
		_next_char(lua_scanner);
	}
	CatStringWo *e_parsedId = cat_unichar_array_to_string(priv->buffer);
	cat_string_wo_to_lowercase(e_parsedId);


	int connSymId = MUL_LUA_SYM_IDENTIFIER;
	CatInteger *ci = (CatInteger *) cat_hash_map_wo_get(priv->keyword_mapping, e_parsedId);
	cat_unref_ptr(e_parsedId);
	if (ci!=NULL) {
		connSymId = cat_integer_value(ci);
	}

	return l_create_token(priv, connSymId, priv->mark_row, end, NULL);
}


static GroRunIToken *_scan_read_identifier(MulLuaScanner *lua_scanner) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	cat_unichar_array_set_length(priv->buffer, 0);
	int row = priv->row;
	int end = priv->column;
	return l_scan_read_identifier_continue(lua_scanner, row, end);
}


static GroRunIToken *l_scan_quoted_digits(MulLuaScanner *lua_scanner) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	gunichar mark_char = priv->nextChar;
	int row = priv->row;
	int end = priv->column;

	while(TRUE) {
		_next_char(lua_scanner);
		gunichar ch = priv->nextChar;
		if ((priv->input_status != CAT_STREAM_OK) || (row != priv->row)) {
			return l_create_token(priv, MUL_LUA_SYM_DECIMAL, priv->mark_row, end, NULL);	// TODO with error
		}
		if (mark_char == priv->nextChar) {
			end = priv->column+1;
			_next_char(lua_scanner);
			return l_create_token(priv, MUL_LUA_SYM_DECIMAL, priv->mark_row, end, NULL);
		}

		if ((ch>='0' && ch <='9') || (ch>='A' && ch <='F') || (ch>='a' && ch <='f')) {
			continue;
		}
		return l_create_token(priv, MUL_LUA_SYM_DECIMAL, priv->mark_row, end, NULL);	// TODO with error
	}
}





static GroRunIToken *l_scan_next_multi_line(MulLuaScanner *lua_scanner, int row) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	cat_log_debug("level=%d, mark=%d, priv->row=%d, row=%d", priv->multi_line_level, priv->mark_column, priv->row, row);

	int sym = priv->multi_line_symbol;
	if (sym==MUL_LUA_SYM_STRING) {
		priv->multi_line_symbol = MUL_LUA_SYM_STRING_CONTINUE;
	}

	if (row!=priv->row) {
		return l_create_token(priv, sym, priv->mark_row, priv->column, NULL);
	}

	while(TRUE) {
		if ((priv->input_status != CAT_STREAM_OK) || (row!=priv->row)) {
			int end_column = priv->column;
			_next_char(lua_scanner);
			return l_create_token(priv, sym, priv->mark_row, end_column, NULL);
		}


		if (priv->nextChar==']') {
			priv->look_a_head_index = 0;
			l_look_a_head(lua_scanner);
			int lt;
			for(lt=0; lt<priv->multi_line_level; lt++) {
				if (priv->nextChar!='=') {
					lt = -1;
					break;
				}
				l_look_a_head(lua_scanner);
			}
			if (lt!=-1) {
				if (priv->nextChar==']') {
					priv->column += priv->look_a_head_index;
					priv->nextColumn += priv->look_a_head_index;
					cat_unichar_array_remove(priv->look_a_head, 0, priv->look_a_head_index);
					priv->look_a_head_index = 0;
					_next_char(lua_scanner);
					priv->inMultiLine = FALSE;
					return l_create_token(priv, sym, priv->mark_row, priv->column, NULL);
				}
			}
		}
		_next_char(lua_scanner);

	}

}


static GroRunIToken *l_scan_multi_line(MulLuaScanner *lua_scanner, int symbol, int level) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	int row = priv->row;
	if (level==-1) {
		level = 0;
		while(TRUE) {
			if ((priv->input_status != CAT_STREAM_OK) || (row != priv->row)) {
				return l_create_token(priv, MUL_LUA_SYM_ERROR, priv->mark_row, priv->column, NULL);
			}
			if (priv->nextChar=='=') {
				level++;
				_next_char(lua_scanner);
			} else if (priv->nextChar=='[') {
				break;
			}
		}
	}

	priv->inMultiLine = TRUE;
	priv->multi_line_level = level;
	priv->multi_line_symbol = symbol;
	return l_scan_next_multi_line(lua_scanner, row);
}


static GroRunIToken *l_scan_eol_comment(MulLuaScanner *lua_scanner) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	int row = priv->row;

	CatStringWo *buffer = cat_string_wo_new();
	cat_string_wo_append_chars(buffer, "--");

	l_look_a_head(lua_scanner);
	if (priv->nextChar=='[') {
		int level = 0;
		while(TRUE) {
			l_look_a_head(lua_scanner);
			if (priv->nextChar=='[') {
				l_look_a_head(lua_scanner);
				break;
			} else if (priv->nextChar=='=') {
				level++;
			} else {
				level = -1;
				break;
			}
		}
		cat_log_debug("eol-level=%d", level);
		if (level!=-1) {
			_next_char(lua_scanner);
			cat_unref_ptr(buffer);
			return l_scan_multi_line(lua_scanner, MUL_LUA_SYM_EOL_COMMENT, level);
		}
	}

	cat_log_debug("normal eol");


	while(TRUE) {
		_next_char(lua_scanner);
		if ((priv->input_status != CAT_STREAM_OK) || (row != priv->row)) {
			GroRunIToken *result = l_create_token(priv, MUL_LUA_SYM_EOL_COMMENT, priv->mark_row, priv->column, buffer);
//			cat_unref_ptr(buffer);
			return result;
		}
		cat_string_wo_append_unichar(buffer, priv->nextChar);
	}
}


static GroRunIToken *real_scanner_next_token(MulLuaScanner *lua_scanner) {
	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
	GroRunIToken *result = NULL;
	cat_log_detail("scanning next");
	if (priv->input_status != CAT_STREAM_OK) {
		cat_log_debug("EOF");
		return l_create_token(priv, MUL_LUA_SYM_END_OF_INPUT, -1, -1, NULL);
	}




	gboolean advance = FALSE;
	while(TRUE) {



		if (advance) {
			_next_char(lua_scanner);
			if (priv->input_status != CAT_STREAM_OK) {
				cat_log_debug("EOF");
				return l_create_token(priv, MUL_LUA_SYM_END_OF_INPUT, -1, -1, NULL);
			}
			advance = FALSE;
		}

		cat_log_detail("column=%d, row=%d, last_char=%d", priv->column, priv->row, priv->nextChar);


		if (priv->column==0) {
			while(TRUE) {
				if (priv->input_status != CAT_STREAM_OK) {
					cat_log_debug("EOF");
					return l_create_token(priv, MUL_LUA_SYM_END_OF_INPUT, -1, -1, NULL);
				}
				switch(priv->nextChar) {
					case ' ' :
					case '\t' :
					case 0xa :
					case 0xc :
					case 0xd : {
						_next_char(lua_scanner);
						continue;
					}
				}
				break;
			}
		}


		int row = priv->row;
		priv->mark_column = priv->column;
		priv->mark_row = priv->row;


		if (priv->inMultiLine) {
			return l_scan_next_multi_line(lua_scanner, row);
		}
		switch(priv->nextChar) {

			case 0xa :
			case 0xc :
			case 0xd : {
				row = priv->row;
				priv->mark_column = priv->column;
				priv->mark_row = priv->row;
				_next_char(lua_scanner);
			} break;


			case 'b':
			case 'B':
			case 'o':
			case 'O':
			case 'x':
			case 'X': {
				cat_unichar_array_set_length(priv->buffer, 0);
				cat_unichar_array_append_uni_char(priv->buffer, priv->nextChar);
				int row = priv->row;
				int end = priv->column+1;

				_next_char(lua_scanner);
				if ((priv->nextChar=='"') ||  (priv->nextChar=='%')) {
					return l_scan_quoted_digits(lua_scanner);
				}
				return l_scan_read_identifier_continue(lua_scanner, row, end);
			}


			case '(': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_LPAREN, priv->mark_row, priv->column, NULL);
			}
			case ')': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_RPAREN, priv->mark_row, priv->column, NULL);
			}
			case '{': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_LBRACK, priv->mark_row, priv->column, NULL);
			}
			case '}': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_RBRACK, priv->mark_row, priv->column, NULL);
			}

			case '[': {
				_next_char(lua_scanner);
				if (priv->nextChar == '[' || priv->nextChar == '=') {
					return l_scan_multi_line(lua_scanner, MUL_LUA_SYM_STRING, -1);
				}
				return l_create_token(priv, MUL_LUA_SYM_LBRACE, priv->mark_row, priv->column, NULL);
			}
			case ']': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_RBRACE, priv->mark_row, priv->column, NULL);
			}

			case '.': {
				_next_char(lua_scanner);
				if (priv->nextChar == '.') {
					_next_char(lua_scanner);
					if (priv->nextChar == '.') {
						_next_char(lua_scanner);
						return l_create_token(priv, MUL_LUA_SYM_ELLIPSES, priv->mark_row, priv->column, NULL);
					} else {
						return l_create_token(priv, MUL_LUA_SYM_KWCONCAT, priv->mark_row, priv->column, NULL);
					}
				} else if (priv->nextChar>='0' && priv->nextChar<='9') {
					return l_scan_number(lua_scanner, TRUE);
				}
				return l_create_token(priv, MUL_LUA_SYM_DOT, priv->mark_row, priv->column, NULL);
			}


			case ':' : {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_COLON, priv->mark_row, priv->column, NULL);
			}
			case ';': _next_char(lua_scanner); return l_create_token(priv, MUL_LUA_SYM_SEMICOLON, priv->mark_row, priv->column, NULL);
			case ',': _next_char(lua_scanner); return l_create_token(priv, MUL_LUA_SYM_COMMA, priv->mark_row, priv->column, NULL);

			case '/': {
				_next_char(lua_scanner);
				if (priv->nextChar=='=') {
					_next_char(lua_scanner);
					return l_create_token(priv, MUL_LUA_SYM_NE, priv->mark_row, priv->column, NULL);
				}
				return l_create_token(priv, MUL_LUA_SYM_OP_DIV, priv->mark_row, priv->column, NULL);
			} break;


			case '=': {
				_next_char(lua_scanner);
				if (priv->nextChar=='=') {
					_next_char(lua_scanner);
					return l_create_token(priv, MUL_LUA_SYM_EQ, priv->mark_row, priv->column, NULL);
				}
				return l_create_token(priv, MUL_LUA_SYM_OP_ASSIGN, priv->mark_row, priv->column, NULL);
			} break;


			case '+': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_OP_PLUS, priv->mark_row, priv->column, NULL);
			}

			case '-': {
				_next_char(lua_scanner);
				if (priv->nextChar=='-') {
					return l_scan_eol_comment(lua_scanner);
				}
				return l_create_token(priv, MUL_LUA_SYM_OP_MINUS, priv->mark_row, priv->column, NULL);
			}

			case '*': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_OP_MUL, priv->mark_row, priv->column, NULL);
			}

			case '#': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_OP_HASH, priv->mark_row, priv->column, NULL);
			}

			case '%': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_OP_MOD, priv->mark_row, priv->column, NULL);
			}
			case '^': {
				_next_char(lua_scanner);
				return l_create_token(priv, MUL_LUA_SYM_OP_NEG, priv->mark_row, priv->column, NULL);
			}
			case '~': {
				_next_char(lua_scanner);
				if (priv->nextChar=='=') {
					_next_char(lua_scanner);
					return l_create_token(priv, MUL_LUA_SYM_NE, priv->mark_row, priv->column, NULL);
				}
			} break;
			case '<': {
				_next_char(lua_scanner);
				if (priv->nextChar=='=') {
					_next_char(lua_scanner);
					return l_create_token(priv, MUL_LUA_SYM_LE, priv->mark_row, priv->column, NULL);
				}
				return l_create_token(priv, MUL_LUA_SYM_OP_LT, priv->mark_row, priv->column, NULL);
			}
			case '>': {
				_next_char(lua_scanner);
				if (priv->nextChar=='=') {
					_next_char(lua_scanner);
					return l_create_token(priv, MUL_LUA_SYM_GE, priv->mark_row, priv->column, NULL);
				}
				return l_create_token(priv, MUL_LUA_SYM_OP_GT, priv->mark_row, priv->column, NULL);
			}
			case '0' : case '1' : case '2' : case '3' : case '4' :
			case '5' : case '6' : case '7' : case '8' : case '9' : {
				return l_scan_number(lua_scanner, FALSE);
			}
			case '\'' :
			case '"' :  {
				return l_scan_read_string(lua_scanner);
			}



			default: {
				if (_is_lua_letter_start(priv->nextChar)) {
					return _scan_read_identifier(lua_scanner);
				}
				advance = TRUE;
			} break;
		}
	}
	return result;
}

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulLuaScanner *lua_scanner = MUL_LUA_SCANNER(self);
	GroRunIToken *result = real_scanner_next_token(lua_scanner);
	return result;
}

static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
	iface->next = l_scanner_next;
}

