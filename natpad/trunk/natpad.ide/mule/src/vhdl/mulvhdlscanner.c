/*
   File:    mulvhdlscanner.c
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 11, 2016
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

#include "mulvhdlscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulVhdlScanner"
#include <logging/catlog.h>

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulVhdlScanner, mul_vhdl_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_vhdl_scanner_class_init(MulVhdlScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_vhdl_scanner_init(MulVhdlScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(mul_vhdl_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_vhdl_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MulVhdlScanner *mul_vhdl_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	MulVhdlScanner *result = g_object_new(MUL_VHDL_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 5);
	return result;
}

gboolean mul_vhdl_scanner_is_eof(MulVhdlScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, MUL_VHDL_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}


static GroRunIToken *l_scan_for_quoted_digits(MulVhdlScanner *scanner);
static GroRunIToken *l_scan_for_number(MulVhdlScanner *scanner);
static gboolean l_is_identifier_start_char(gunichar unichar);

static GroRunIToken *l_next_token(MulVhdlScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(result==NULL) {
		base_class->markLocation((GroRunScannerBase *) scanner);
		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, MUL_VHDL_SYM_END_OF_INPUT, -1, -1, NULL);
				break;

			case 'b':
			case 'B':
			case 'o':
			case 'O':
			case 'x':
			case 'X': {

				if ((lookahead[1]=='"') ||  (lookahead[1]=='%')) {
					result = l_scan_for_quoted_digits(scanner);
				} else {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, MUL_VHDL_SYM_IDENTIFIER, FALSE);
				}
			} break;

			case ':' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_VARASSIGN, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_COLON, 1);
				}
				break;
			}

			case '(':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_LEFTPAREN, 1);
				break;

			case ')':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_RIGHTPAREN, 1);
				break;

			case '.':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_DOT, 1);
				break;

			case ';':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_SEMICOLON, 1);
				break;

			case ',':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_COMMA, 1);
				break;

			case '+':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_PLUS, 1);
				break;

			case '&':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_AMPERSAND, 1);
				break;

			case '!' :
			case '|' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_BAR, 1);
				break;


			case '\'': {
				gunichar ch = lookahead[1];
				if ((lookahead[2]=='\'') && (ch=='%' || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9') || ch=='#' || ch=='&' || ch=='\'' || ch=='(' || ch==')' || ch=='*' || ch=='+' || ch==',' || ch=='-'
						 || ch=='.' || ch=='/' || ch==':' || ch==';' || ch=='<' || ch=='=' || ch=='>' || ch=='_' || ch=='|' || ch==' ' || ch=='\t' || (ch>='a' && ch<='z')
						 || ch=='!' || ch=='$' || ch=='@' || ch=='?' || ch=='[' || ch=='\\' || ch==']' || ch=='^' || ch=='`' || ch=='{' || ch=='}' || ch=='~')) {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_CHARACTERLITERAL, 3);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_APOSTROPHE, 1);
				}
			} break;

			case '/': {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_OPPNE, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_SLASH, 1);
				}
			} break;

			case '=': {
				if (lookahead[1]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_ARROW, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_OPPEQ, 1);
				}
			} break;


			case '-': {
				if (lookahead[1]=='-') {
					result = base_class->scanForEndOfLine((GroRunScannerBase *) scanner, MUL_VHDL_SYM_EOL_COMMENT, TRUE);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_MINUS, 1);
				}
			} break;

			case '*': {
				if (lookahead[1]=='*') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_DOUBLESTAR, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_STAR, 1);
				}
			} break;

			case '<': {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_OPPLE, 2);
				} else if (lookahead[1]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_BOX, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_OPPLT, 1);
				}
			} break;

			case '>': {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_OPPGE, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_VHDL_SYM_OPPGT, 1);
				}
			} break;

			case '0' : case '1' : case '2' : case '3' : case '4' :
			case '5' : case '6' : case '7' : case '8' : case '9' : {
				result = l_scan_for_number(scanner);
			} break;

			case '"' :
				result = base_class->scanForQuotedString((GroRunScannerBase *) scanner, MUL_VHDL_SYM_STRINGLITERAL);
				break;

			default: {
				if (l_is_identifier_start_char(lookahead[0])) {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, MUL_VHDL_SYM_IDENTIFIER, FALSE);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES);
				}
			} break;

		}
	}
	return result;
}




static gboolean l_is_identifier_start_char(gunichar unichar) {
	return ((unichar>='a' && unichar<='z')
			|| (unichar>='A' && unichar<='Z')
			|| (unichar=='_')) ? TRUE : FALSE;
}

static GroRunIToken *l_scan_for_quoted_digits(MulVhdlScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	base_class->markLocation((GroRunScannerBase *) scanner);

	gunichar mark_char = lookahead[0];
	CatStringWo *buf = cat_string_wo_new();
	cat_string_wo_append_unichar(buf, mark_char);
	int end_column = 0;

	GroRunIToken *result = NULL;
	while(result!=NULL) {
		end_column = base_class->getColumn((GroRunScannerBase *) scanner);
		gunichar ch = base_class->advance((GroRunScannerBase *) scanner, 0);
		if ((ch>='0' && ch <='9') || (ch>='A' && ch <='F') || (ch>='a' && ch <='f')) {
			cat_string_wo_append_unichar(buf, ch);
		} else {
			if (mark_char == ch) {
				end_column = base_class->getColumn((GroRunScannerBase *) scanner);
				base_class->advance((GroRunScannerBase *) scanner, 0);
			}
			long long end_row = base_class->getMarkedRow((GroRunScannerBase *) scanner);
			result = base_class->createToken((GroRunScannerBase *) scanner, MUL_VHDL_SYM_NUMBERLITERAL, end_row, end_column+1, buf);
		}
	}
	return result;
}


static gboolean l_scan_digits(MulVhdlScanner *scanner, int *lastValid) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	gunichar ch = lookahead[0];
	gboolean got_digit = FALSE;
	while(ch>='0' && ch<='9') {
		got_digit = TRUE;
		*lastValid = base_class->getColumn((GroRunScannerBase *) scanner);
		ch = base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	return got_digit;
}


static gboolean l_scan_extended_digits(MulVhdlScanner *scanner, int *lastValid) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	gunichar ch = lookahead[0];
	gboolean got_digit = FALSE;
	while((ch>='0' && ch<='9')
				|| (ch>='A' && ch<='F')
				|| (ch>='a' && ch<='f')) {
		got_digit = TRUE;
		*lastValid = base_class->getColumn((GroRunScannerBase *) scanner);
		ch = base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	return got_digit;
}

//    {integer}(\.{integer})?({exponent})?
// |  base HASH based_integer (DOT based_integer)? HASH ([eE][-+]?{integer})?
// |  (base COLON based_integer (DOT based_integer)? COLON ([eE][-+]?{integer})?)
static GroRunIToken *l_scan_for_number(MulVhdlScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	int row = base_class->getMarkedRow((GroRunScannerBase *) scanner);
	int end = base_class->getMarkedColumn((GroRunScannerBase *) scanner);
	int state = 0;
	l_scan_digits(scanner, &end);

	gunichar ch = lookahead[0];

	switch(ch) {
		case '.' :
			state = 1;
			break;
		case '#' :
			state = 2;
			break;
		case ':' :
			state = 3;
			break;
		case 'e' :
		case 'E' :
			state = 4;
			break;
		default : {
			return base_class->createToken((GroRunScannerBase *) scanner, MUL_VHDL_SYM_NUMBERLITERAL, row, end+1, NULL);
		}
	}

	end = base_class->getColumn((GroRunScannerBase *) scanner);
	ch = base_class->advance((GroRunScannerBase *) scanner, 0);


	if (state==1) {
		l_scan_digits(scanner, &end);
		state = 5;
	} else if (state==2 || state==3) {
		l_scan_extended_digits(scanner, &end);
		ch = lookahead[0];
		if (ch=='.') {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			ch = base_class->advance((GroRunScannerBase *) scanner, 0);
			l_scan_extended_digits(scanner, &end);
		}
	}
	ch = lookahead[0];

	if (state==2) {
		if (ch=='#') {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			ch = base_class->advance((GroRunScannerBase *) scanner, 0);
			state = 5;
		} else {
			return base_class->createToken((GroRunScannerBase *) scanner, MUL_VHDL_SYM_NUMBERLITERAL, row, end+1, NULL);
		}
	} else if (state==3) {
		if (ch==':') {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			ch = base_class->advance((GroRunScannerBase *) scanner, 0);
			state = 5;
		} else {
			return base_class->createToken((GroRunScannerBase *) scanner, MUL_VHDL_SYM_NUMBERLITERAL, row, end+1, NULL);
		}
	}



	if (state==5) {
		if ((ch=='e') || (ch=='E')) {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			ch = base_class->advance((GroRunScannerBase *) scanner, 0);
			state = 4;
		} else {
			return base_class->createToken((GroRunScannerBase *) scanner, MUL_VHDL_SYM_NUMBERLITERAL, row, end+1, NULL);
		}
	}

	if (state==4) {
		if ((ch=='-') || (ch=='+')) {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			ch = base_class->advance((GroRunScannerBase *) scanner, 0);
		}
		l_scan_digits(scanner, &end);
	}

	return base_class->createToken((GroRunScannerBase *) scanner, MUL_VHDL_SYM_NUMBERLITERAL, row, end+1, NULL);
}


/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulVhdlScanner *scanner = MUL_VHDL_SCANNER(self);
	GroRunIToken *result = l_next_token(scanner);
	cat_log_on_debug({
		cat_log_debug("next=%O", result);
	});
	return result;
}


static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
	iface->next = l_scanner_next;
}

/********************* end GroRunIScanner implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
