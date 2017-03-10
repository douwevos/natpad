/*
   File:    jagpscanner.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Mar 9, 2017
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

#include "jagpscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPScanner"
#include <logging/catlog.h>

struct _JagPScannerPrivate {
	void *dummy;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPScanner, jagp_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_ADD_PRIVATE(JagPScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_scanner_class_init(JagPScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_scanner_init(JagPScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPScanner *instance = JAGP_SCANNER(object);
//	JagPScannerPrivate *priv = jagp_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPScanner *jagp_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	JagPScanner *result = g_object_new(JAGP_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	JagPScannerPrivate *priv = jagp_scanner_get_instance_private(result);
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 7);
	return result;
}


gboolean jagp_scanner_is_eof(JagPScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, JAGP_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}

static GroRunIToken *l_scan_number(JagPScanner *scanner);
static GroRunIToken *l_scan_read_char_literal(JagPScanner *scanner);
static GroRunIToken *l_scan_full_comment(JagPScanner *scanner);

static GroRunIToken *l_next_token(JagPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	GroRunScannerBase *gro_scanner = (GroRunScannerBase *) scanner;

	while(result==NULL) {
		base_class->markLocation((GroRunScannerBase *) scanner);
//		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_END_OF_INPUT, -1, -1, NULL);
				break;

			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': case '8': case '9':
				result = l_scan_number(scanner);
				break;

			case '"' :
				result = base_class->scanForQuotedString(gro_scanner, JAGP_SYM_STRING_LITERAL);
				break;

			case '\'' :
				result = l_scan_read_char_literal(scanner);
				break;

			case '<' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_LE, 2);
				} else if (lookahead[1]=='<') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_SHL, 3);
					} else {
						result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_SHL, 2);
					}
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_LT, 1);
				}
			} break;



			case '>' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_GE, 2);
				} else if (lookahead[1]=='>') {
					if (lookahead[2]=='>') {
						if (lookahead[3]=='=') {
							result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_SHRR, 4);
						} else {
							result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_SHRR, 3);
						}
					} else if (lookahead[2]=='=') {
						result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_SHR, 3);
					} else {
						result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_SHR, 2);
					}
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_GT, 1);
				}
			} break;


			case '=' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_EQ, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_IS, 1);
				}
			} break;

			case '!' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_NE, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_NOT, 1);
				}
			} break;

			case '^' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_XOR, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_XOR, 1);
				}
			} break;

			case '%' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_MOD, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_MOD, 1);
				}
			} break;


			case '|' : {
				if (lookahead[1]=='|') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_LOR, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_OR, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_OR, 1);
				}
			} break;

			case '&' : {
				if (lookahead[1]=='&') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_LAND, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_AND, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_AND, 1);
				}
			} break;

			case '+' : {
				if (lookahead[1]=='+') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_INC, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_ADD, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_ADD, 1);
				}
			} break;

			case '-' : {
				if (lookahead[1]=='-') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_DEC, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_SUB, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_SUB, 1);
				}
			} break;

			case '*' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_MUL, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_MUL, 1);
				}
			} break;

			case '/' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ASS_DIV, 2);
				} else if (lookahead[1]=='/') {
					result = base_class->scanForEndOfLine(gro_scanner, JAGP_SYM_EOL_COMMENT, TRUE);
				} else if (lookahead[1]=='*') {
					result = l_scan_full_comment(scanner);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_DIV, 1);
				}
			} break;



			case '~': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_OP_NEG, 1); break;
			case ';': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_SEMICOLON, 1); break;
			case '(': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_LPAREN, 1); break;
			case ')': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_RPAREN, 1); break;
			case '?': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_QUESTION, 1); break;
			case '{': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_LBRACE, 1); break;
			case '}': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_RBRACE, 1); break;
			case '[': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_LBRACK, 1); break;
			case ']': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_RBRACK, 1); break;
			case '@': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_AT, 1); break;
			case ':': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_COLON, 1); break;
			case ',': result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_COMMA, 1); break;
			case '.': {
				gboolean is_oke = TRUE;
//				if (Character.isDigit(nchar))
//				{
//					item.setLength(0);
//					item.append('.');
//					status=SCAN_READPREEXP;
//				}
//				else
//				{
				if (lookahead[1]=='.' && lookahead[2]=='.') {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_ELLIPSIS, 1);
				} else {
					result = base_class->createTokenBasic(gro_scanner, JAGP_SYM_DOT, 1);
				}
			} break;



			default: {
				if (g_unichar_isalpha(lookahead[0]) || lookahead[0]=='_') {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, JAGP_SYM_IDENTIFIER, TRUE);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, 0);
				}
			} break;
		}
	}
	return result;
}


static gboolean l_scan_digits(JagPScanner *scanner, int *lastValid) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	gboolean got_digit = FALSE;
	while(TRUE) {
		if (lookahead[0]<'0' || lookahead[0]>'9') {
			break;
		}
		got_digit = TRUE;
		*lastValid = base_class->getColumn((GroRunScannerBase *) scanner);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	return got_digit;
}

#define JAGP_SYM_DOUBLE_LITERAL JAGP_SYM_INTEGER_LITERAL
#define JAGP_SYM_FLOAT_LITERAL JAGP_SYM_INTEGER_LITERAL
#define JAGP_SYM_HEX JAGP_SYM_INTEGER_LITERAL
#define JAGP_SYM_OCTAL JAGP_SYM_INTEGER_LITERAL

static GroRunIToken *l_scan_hex_octal(JagPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	int row = base_class->getRow((GroRunScannerBase *) scanner);
	int end = base_class->getColumn((GroRunScannerBase *) scanner);
	gboolean isHex = FALSE;
	if ((lookahead[0]=='x') || (lookahead[0]=='X')) {
		/* '0x' */
		isHex = TRUE;
		end = base_class->getColumn((GroRunScannerBase *) scanner);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	} else if ((lookahead[0]>='0') && (lookahead[0]<'8')) {
		/* '02' */
		end = base_class->getColumn((GroRunScannerBase *) scanner);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	} else {
		/* '0' */
		return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_INTEGER_LITERAL, row, end, NULL);
	}

	while(TRUE) {
		switch(lookahead[0]) {
			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': {
			} break;
			case '8': case '9' : case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
			case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' : {
				if (!isHex) {
					return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_OCTAL, row, end, NULL);
				}
			} break;
			case 'l' :
			case 'L' : {
				end = base_class->getColumn((GroRunScannerBase *) scanner);
				base_class->advance((GroRunScannerBase *) scanner, 0);
				return base_class->createToken((GroRunScannerBase *) scanner, isHex ? JAGP_SYM_HEX : JAGP_SYM_OCTAL, row, end, NULL);
			} break;
			default : {
				return base_class->createToken((GroRunScannerBase *) scanner, isHex ? JAGP_SYM_HEX : JAGP_SYM_OCTAL, row, end, NULL);
			}
		}

		end = base_class->getColumn((GroRunScannerBase *) scanner);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	return NULL;
}


static GroRunIToken *l_scan_number(JagPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	int row = base_class->getLeftRow((GroRunScannerBase *) scanner);
	int end = base_class->getLeftColumn((GroRunScannerBase *) scanner);
	int state = 0;
	l_scan_digits(scanner, &end);
	if (lookahead[0]==-1) {
		return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_INTEGER_LITERAL, row, end, NULL);
	}

	switch(lookahead[0]) {
		case '.' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			state = 1;
		} break;
		case 'x' :
		case 'X' :
			return l_scan_hex_octal(scanner);
		case 'e' :
		case 'E' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			state = 2;
		} break;
		case 'l' :
		case 'L' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_INTEGER_LITERAL, row, end, NULL);
		}
		case 'd' :
		case 'D' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_DOUBLE_LITERAL, row, end, NULL);
		}
		case 'f' :
		case 'F' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
		default : {
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_INTEGER_LITERAL, row, end, NULL);
		}
	}

	if (state==1) {
		l_scan_digits(scanner, &end);
		if (lookahead[0]==-1) {
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
	}


	switch(lookahead[0]) {
		case 'e' :
		case 'E' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			state = 3;
		} break;
		case 'd' :
		case 'D' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_DOUBLE_LITERAL, row, end, NULL);
		}
		case 'f' :
		case 'F' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
		default : {
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
	}


	if ((lookahead[0]=='+') || (lookahead[0]=='-')) {
		end = base_class->getColumn((GroRunScannerBase *) scanner);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}

	l_scan_digits(scanner, &end);

	switch(lookahead[0]) {
		case 'd' :
		case 'D' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_DOUBLE_LITERAL, row, end, NULL);
		}
		case 'f' :
		case 'F' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
		default : {
			return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
	}
}


static GroRunIToken *l_scan_read_char_literal(JagPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	int row = base_class->getLeftRow((GroRunScannerBase *) scanner);
	int end = base_class->getLeftColumn((GroRunScannerBase *) scanner);

	base_class->advance((GroRunScannerBase *) scanner, 0);

	if (lookahead[0]=='\\') {
		base_class->advance((GroRunScannerBase *) scanner, 0);
		switch(lookahead[0]) {
			case 'b' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case 'f' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case 'n' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case 'r' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case 't' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case '\\' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case '"' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case '\'' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case 'u' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
				base_class->advance((GroRunScannerBase *) scanner, 0);
				base_class->advance((GroRunScannerBase *) scanner, 0);
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : {
				base_class->advance((GroRunScannerBase *) scanner, 0);
				if (lookahead[0]=='\'') {
					break;
				}
				base_class->advance((GroRunScannerBase *) scanner, 0);
				base_class->advance((GroRunScannerBase *) scanner, 0);
			} break;
			default : {
				// TODO ERROR
			} break;
		}
	} else  {
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}

	gboolean ok = TRUE;
	if (lookahead[0]!='\'') {
//		cat_log_error("lexer error with literal char");		// TODO
		ok = FALSE;

	}

	row = base_class->getRow((GroRunScannerBase *) scanner);
	end = base_class->getColumn((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	GroRunIToken *result = base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_CHARACTER_LITERAL, row, end, NULL);
	if (!ok) {
		grorun_itoken_set_state(result, GRORUN_STATE_SCANNER_ERROR);
	}
	return result;
}



static GroRunIToken *l_scan_full_comment(JagPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	int level = 1;
	while(TRUE) {
		if ((lookahead[0]=='*') && (lookahead[1]=='/')) {
			level--;
			base_class->advance((GroRunScannerBase *) scanner, 0);
			break;
		} else if (lookahead[0]==-1) {
			// TODO error
			break;
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	return base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_FULL_COMMENT, row, column, NULL);
}



/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	JagPScanner *scanner = JAGP_SCANNER(self);
	GroRunIToken *result = l_next_token(scanner);
	cat_log_debug("next:%O", result);
	return result;
}


static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
	iface->next = l_scanner_next;
}

/********************* end GroRunIScanner implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPScanner *instance = JAGP_SCANNER(self);
	JagPScannerPrivate *priv = jagp_scanner_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
