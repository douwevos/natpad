/*
   File:    armpscanner.c
   Project: armadilla-parser
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

#include "armpscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "ArmPScanner"
#include <logging/catlog.h>

struct _ArmPScannerPrivate {
	void *dummy;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ArmPScanner, armp_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_ADD_PRIVATE(ArmPScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void armp_scanner_class_init(ArmPScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void armp_scanner_init(ArmPScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ArmPScanner *instance = ARMP_SCANNER(object);
//	ArmPScannerPrivate *priv = armp_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(armp_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(armp_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ArmPScanner *armp_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	ArmPScanner *result = g_object_new(ARMP_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	ArmPScannerPrivate *priv = armp_scanner_get_instance_private(result);
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 7);
	return result;
}


gboolean armp_scanner_is_eof(ArmPScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, ARMP_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}

static GroRunIToken *l_scan_number(ArmPScanner *scanner);
static GroRunIToken *l_scan_read_char_literal(ArmPScanner *scanner);
static GroRunIToken *l_scan_full_comment(ArmPScanner *scanner);


#define ARMP_SYM_STRING_LITERAL                                                                     5
#define ARMP_SYM_I_CONSTANT                                                                         3
#define ARMP_SYM_F_CONSTANT                                                                         4

#define ARMP_SYM_ERROR                                                                              1

#define ARMP_SYM_FUNC_NAME                                                                          6
#define ARMP_SYM_SIZEOF                                                                             7
#define ARMP_SYM_PTR_OP                                                                             8
#define ARMP_SYM_TYPEDEF_NAME                                                                       29
#define ARMP_SYM_ENUMERATION_CONSTANT                                                               30
#define ARMP_SYM_TYPEDEF                                                                            31
#define ARMP_SYM_EXTERN                                                                             32
#define ARMP_SYM_STATIC                                                                             33
#define ARMP_SYM_AUTO                                                                               34
#define ARMP_SYM_REGISTER                                                                           35
#define ARMP_SYM_INLINE                                                                             36
#define ARMP_SYM_CONST                                                                              37
#define ARMP_SYM_RESTRICT                                                                           38
#define ARMP_SYM_VOLATILE                                                                           39
#define ARMP_SYM_BOOL                                                                               40
#define ARMP_SYM_CHAR                                                                               41
#define ARMP_SYM_SHORT                                                                              42
#define ARMP_SYM_INT                                                                                43
#define ARMP_SYM_LONG                                                                               44
#define ARMP_SYM_SIGNED                                                                             45
#define ARMP_SYM_UNSIGNED                                                                           46
#define ARMP_SYM_FLOAT                                                                              47
#define ARMP_SYM_DOUBLE                                                                             48
#define ARMP_SYM_VOID                                                                               49
#define ARMP_SYM_COMPLEX                                                                            50
#define ARMP_SYM_IMAGINARY                                                                          51
#define ARMP_SYM_STRUCT                                                                             52
#define ARMP_SYM_UNION                                                                              53
#define ARMP_SYM_ENUM                                                                               54
#define ARMP_SYM_CASE                                                                               56
#define ARMP_SYM_DEFAULT                                                                            57
#define ARMP_SYM_IF                                                                                 58
#define ARMP_SYM_ELSE                                                                               59
#define ARMP_SYM_SWITCH                                                                             60
#define ARMP_SYM_WHILE                                                                              61
#define ARMP_SYM_DO                                                                                 62
#define ARMP_SYM_FOR                                                                                63
#define ARMP_SYM_GOTO                                                                               64
#define ARMP_SYM_CONTINUE                                                                           65
#define ARMP_SYM_BREAK                                                                              66
#define ARMP_SYM_RETURN                                                                             67
#define ARMP_SYM_ALIGNAS                                                                            68
#define ARMP_SYM_ALIGNOF                                                                            69
#define ARMP_SYM_ATOMIC                                                                             70
#define ARMP_SYM_GENERIC                                                                            71
#define ARMP_SYM_NORETURN                                                                           72
#define ARMP_SYM_STATIC_ASSERT                                                                      73
#define ARMP_SYM_THREAD_LOCAL                                                                       74
#define ARMP_SYM_FULL_COMMENT                                                                       100


static GroRunIToken *l_next_token(ArmPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	GroRunScannerBase *gro_scanner = (GroRunScannerBase *) scanner;

	while(result==NULL) {
		base_class->markLocation((GroRunScannerBase *) scanner);
//		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_END_OF_INPUT, -1, -1, NULL);
				break;


			case '|' : {
				if (lookahead[1]=='|') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_OR_OP, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_OR_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_BAR, 1);
				}
			} break;

			case ';': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_SEMI, 1); break;
			case ',': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_COMMA, 1); break;
			case ':': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_COLON, 1); break;
			case '~': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_NEG, 1); break;
			case '(': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_LPAREN, 1); break;
			case ')': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_RPAREN, 1); break;
			case '?': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_QUESTIONMARK, 1); break;
			case '{': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_LCUBRACE, 1); break;
			case '}': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_RCUBRACE, 1); break;
			case '[': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_LSQBRACE, 1); break;
			case ']': result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_RSQBRACE, 1); break;

			case '.': {
				if (lookahead[1]=='.' && lookahead[2]=='.') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_ELLIPSIS, 1);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_DOT, 1);
				}
			} break;

			case '<' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_LE_OP, 2);
				} else if (lookahead[1]=='<') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_LEFT_ASSIGN, 3);
					} else {
						result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_LEFT_OP, 1);
					}
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_LT, 1);
				}
			} break;

			case '>' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_GE_OP, 2);
				} else if (lookahead[1]=='>') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_RIGHT_ASSIGN, 3);
					} else {
						result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_RIGHT_OP, 1);
					}
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_GT, 1);
				}
			} break;

			case '=' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_EQ_OP, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_IS, 1);
				}
			} break;

			case '+' : {
				if (lookahead[1]=='+') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_INC_OP, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_ADD_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_PLUS, 1);
				}
			} break;

			case '-' : {
				if (lookahead[1]=='-') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_DEC_OP, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_SUB_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_MINUS, 1);
				}
			} break;

			case '*' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_MUL_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_MUL, 1);
				}
			} break;

			case '/' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_DIV_ASSIGN, 2);
				} else if (lookahead[1]=='/') {
					result = base_class->scanForEndOfLine(gro_scanner, ARMP_SYM_EOL_COMMENT, TRUE);
				} else if (lookahead[1]=='*') {
					result = l_scan_full_comment(scanner);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_DIV, 1);
				}
			} break;

			case '%' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_MOD_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_MOD, 1);
				}
			} break;

			case '^' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_XOR_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_XOR, 1);
				}
			} break;

			case '&' : {
				if (lookahead[1]=='&') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_AND_OP, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_AND_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_AND, 1);
				}
			} break;

			case '!' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_NE_OP, 2);
				} else {
					result = base_class->createTokenBasic(gro_scanner, ARMP_SYM_L_NOT, 1);
				}
			} break;

			case '"' :
				result = base_class->scanForQuotedString(gro_scanner, ARMP_SYM_STRING_LITERAL);
				break;


			case '\'' :
				result = l_scan_read_char_literal(scanner);
				break;






			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': case '8': case '9':
				result = l_scan_number(scanner);
				break;

			default: {
				if (g_unichar_isalpha(lookahead[0]) || lookahead[0]=='_') {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, ARMP_SYM_IDENTIFIER, TRUE);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, 0);
				}
			} break;

		}
	}
	return result;
}


static gboolean l_scan_digits(ArmPScanner *scanner, int *lastValid) {
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

#define ARMP_SYM_DOUBLE_LITERAL ARMP_SYM_I_CONSTANT
#define ARMP_SYM_FLOAT_LITERAL ARMP_SYM_F_CONSTANT
#define ARMP_SYM_HEX ARMP_SYM_I_CONSTANT
#define ARMP_SYM_OCTAL ARMP_SYM_I_CONSTANT

static GroRunIToken *l_scan_hex_octal(ArmPScanner *scanner) {
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
		return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_I_CONSTANT, row, end, NULL);
	}

	while(TRUE) {
		switch(lookahead[0]) {
			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': {
			} break;
			case '8': case '9' : case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
			case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' : {
				if (!isHex) {
					return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_OCTAL, row, end, NULL);
				}
			} break;
			case 'l' :
			case 'L' : {
				end = base_class->getColumn((GroRunScannerBase *) scanner);
				base_class->advance((GroRunScannerBase *) scanner, 0);
				return base_class->createToken((GroRunScannerBase *) scanner, isHex ? ARMP_SYM_HEX : ARMP_SYM_OCTAL, row, end, NULL);
			} break;
			default : {
				return base_class->createToken((GroRunScannerBase *) scanner, isHex ? ARMP_SYM_HEX : ARMP_SYM_OCTAL, row, end, NULL);
			}
		}

		end = base_class->getColumn((GroRunScannerBase *) scanner);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	return NULL;
}


static GroRunIToken *l_scan_number(ArmPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	int row = base_class->getLeftRow((GroRunScannerBase *) scanner);
	int end = base_class->getLeftColumn((GroRunScannerBase *) scanner);
	int state = 0;
	l_scan_digits(scanner, &end);
	if (lookahead[0]==-1) {
		return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_I_CONSTANT, row, end, NULL);
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
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_I_CONSTANT, row, end, NULL);
		}
		case 'd' :
		case 'D' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_DOUBLE_LITERAL, row, end, NULL);
		}
		case 'f' :
		case 'F' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
		default : {
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_I_CONSTANT, row, end, NULL);
		}
	}

	if (state==1) {
		l_scan_digits(scanner, &end);
		if (lookahead[0]==-1) {
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_FLOAT_LITERAL, row, end, NULL);
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
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_DOUBLE_LITERAL, row, end, NULL);
		}
		case 'f' :
		case 'F' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
		default : {
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_FLOAT_LITERAL, row, end, NULL);
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
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_DOUBLE_LITERAL, row, end, NULL);
		}
		case 'f' :
		case 'F' : {
			end = base_class->getColumn((GroRunScannerBase *) scanner);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
		default : {
			return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_FLOAT_LITERAL, row, end, NULL);
		}
	}
}


static GroRunIToken *l_scan_read_char_literal(ArmPScanner *scanner) {
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
				// TODO validate
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
	GroRunIToken *result = base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_I_CONSTANT, row, end, NULL);
	if (!ok) {
		grorun_itoken_set_state(result, GRORUN_STATE_SCANNER_ERROR);
	}
	return result;
}



static GroRunIToken *l_scan_full_comment(ArmPScanner *scanner) {
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
	return base_class->createToken((GroRunScannerBase *) scanner, ARMP_SYM_FULL_COMMENT, row, column, NULL);
}



/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	ArmPScanner *scanner = ARMP_SCANNER(self);
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
	ArmPScanner *instance = ARMP_SCANNER(self);
	ArmPScannerPrivate *priv = armp_scanner_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
