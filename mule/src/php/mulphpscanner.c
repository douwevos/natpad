/*
   File:    mulphpscanner.c
   Project: mule
   Author:  Douwe Vos
   Date:    Dec 8, 2016
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

#include "mulphpscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulPhpScanner"
#include <logging/catlog.h>

struct _MulPhpScannerPrivate {
	gboolean is_inline;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulPhpScanner, mul_php_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_ADD_PRIVATE(MulPhpScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_php_scanner_class_init(MulPhpScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_php_scanner_init(MulPhpScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	MulPhpScanner *instance = MUL_PHP_SCANNER(object);
//	MulPhpScannerPrivate *priv = mul_php_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(mul_php_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_php_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulPhpScanner *mul_php_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	MulPhpScanner *result = g_object_new(MUL_PHP_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	MulPhpScannerPrivate *priv = mul_php_scanner_get_instance_private(result);
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 8);
	priv->is_inline = TRUE;
	return result;
}



gboolean mul_php_scanner_is_eof(MulPhpScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, MUL_PHP_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}




static GroRunIToken *l_skip_inline(MulPhpScanner *scanner) {

	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	base_class->markLocation((GroRunScannerBase *) scanner);

	while(TRUE) {

		if (lookahead[0]=='<' && lookahead[1]=='?') {
			int k = 2;
			/* start of php*/
			if ((lookahead[2]=='p') && (lookahead[3]=='h') && (lookahead[4]=='p')) {
				k=5;
			}

			return base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_INLINE_HTML, k);
		} else if (lookahead[0]==-1) {
			break;
		}

	}
	return base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_INLINE_HTML, 0);
}

static GroRunIToken *l_scan_for_comment(MulPhpScanner *scanner);
static GroRunIToken *l_scan_for_variable(MulPhpScanner *scanner);


//#define MUL_PHP_SYM_T_INLINE_HTML                                                                     88
//#define MUL_PHP_SYM_T_ENCAPSED_AND_WHITESPACE                                                         110
//#define MUL_PHP_SYM_T_CURLY_OPEN                                                                      119
//#define MUL_PHP_SYM_T_NUM_STRING                                                                      120
//#define MUL_PHP_SYM_T_STRING_VARNAME                                                                  121
//#define MUL_PHP_SYM_T_START_HEREDOC                                                                   122
//#define MUL_PHP_SYM_T_END_HEREDOC                                                                     123
//#define MUL_PHP_SYM_T_LNUMBER                                                                         125
//#define MUL_PHP_SYM_T_DNUMBER                                                                         126
//#define MUL_PHP_SYM_T_IS_NOT_IDENTICAL                                                                127
//#define MUL_PHP_SYM_T_IS_NOT_EQUAL                                                                    129
//#define MUL_PHP_SYM_T_IS_GREATER_OR_EQUAL                                                             132
//#define MUL_PHP_SYM_T_COALESCE                                                                        133
//#define MUL_PHP_SYM_T_INT_CAST                                                                        134
//#define MUL_PHP_SYM_T_DOUBLE_CAST                                                                     135
//#define MUL_PHP_SYM_T_STRING_CAST                                                                     136
//#define MUL_PHP_SYM_T_ARRAY_CAST                                                                      137
//#define MUL_PHP_SYM_T_OBJECT_CAST                                                                     138
//#define MUL_PHP_SYM_T_BOOL_CAST                                                                       139
//#define MUL_PHP_SYM_T_UNSET_CAST                                                                      140
//#define MUL_PHP_SYM_T_YIELD_FROM                                                                      141
//#define MUL_PHP_SYM_OP_NOT                                                                            150
//#define MUL_PHP_SYM_OP_NEG                                                                            151
//#define MUL_PHP_SYM_AT                                                                                154
//#define MUL_PHP_SYM_APASTOPH_BACKWARD                                                                 155
//#define MUL_PHP_SYM_DOUBLE_QUOTE                                                                      156


static GroRunIToken *l_scan_for_heredoc(MulPhpScanner *scanner);
static GroRunIToken *l_scan_number(MulPhpScanner *scanner);

static GroRunIToken *l_next_token(MulPhpScanner *scanner) {

	MulPhpScannerPrivate *priv = mul_php_scanner_get_instance_private(scanner);
	if (priv->is_inline) {
		priv->is_inline = FALSE;
		return l_skip_inline(scanner);
	}


	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(result==NULL) {
		base_class->markLocation((GroRunScannerBase *) scanner);
		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, MUL_PHP_SYM_END_OF_INPUT, -1, -1, NULL);
				break;

			case '.' : {
				if (lookahead[1]=='.' && lookahead[2]=='.') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_ELLIPSIS, 3);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_CONCAT_EQUAL, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_DOT, 1);
				}
				break;
			}
			case '<' : {
				if (lookahead[1]=='=' && lookahead[2]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_SPACESHIP, 3);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_IS_SMALLER_OR_EQUAL, 2);
				} else if (lookahead[1]=='<' && lookahead[2]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_SL_EQUAL, 3);
				} else if (lookahead[1]=='<' && lookahead[2]=='<') {
					result = l_scan_for_heredoc(scanner);
				} else if (lookahead[1]=='<') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_SL, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_LT, 1);
				}
				break;
			}
			case '>' : {
				if (lookahead[1]=='>' && lookahead[2]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_SR_EQUAL, 3);
				} else if (lookahead[1]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_SR, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_GT, 1);
				}
				break;
			}

			case '^' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_XOR_EQUAL, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_POW, 1);
				}
				break;
			}


			case '(' : {
				if (lookahead[1]=='i' && lookahead[2]=='n' && lookahead[3]=='t' && lookahead[4]==')') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_INT_CAST, 5);
				} else if (lookahead[1]=='b' && lookahead[2]=='o' && lookahead[3]=='o' && lookahead[4]=='l' && lookahead[5]==')') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_BOOL_CAST, 6);
				} else if (lookahead[1]=='d' && lookahead[2]=='o' && lookahead[3]=='u' && lookahead[4]=='b' && lookahead[5]=='l' && lookahead[6]=='e' && lookahead[7]==')') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_DOUBLE_CAST, 8);
				} else if (lookahead[1]=='s' && lookahead[2]=='t' && lookahead[3]=='r' && lookahead[4]=='i' && lookahead[5]=='n' && lookahead[6]=='g' && lookahead[7]==')') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_STRING_CAST, 8);
				} else if (lookahead[1]=='o' && lookahead[2]=='b' && lookahead[3]=='j' && lookahead[4]=='e' && lookahead[5]=='c' && lookahead[6]=='t' && lookahead[7]==')') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_OBJECT_CAST, 8);
				} else if (lookahead[1]=='a' && lookahead[2]=='r' && lookahead[3]=='r' && lookahead[4]=='a' && lookahead[5]=='y' && lookahead[6]==')') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_OBJECT_CAST, 7);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_LPAREN, 1);
				}
				break;
			}
			case ')' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_RPAREN, 1);
				break;
			}
			case '[' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_LSQBRACK, 1);
				break;
			}
			case ']' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_RSQBRACK, 1);
				break;
			}
			case '{' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_LCUBRACK, 1);
				break;
			}
			case '}' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_RCUBRACK, 1);
				break;
			}

			case ';' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_SEMI, 1);
				break;
			}
			case '\\' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_NS_SEPARATOR, 1);
				break;
			}

			case '?' : {
				if (lookahead[1]=='>') {
					base_class->advance((GroRunScannerBase *) scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES);
					base_class->advance((GroRunScannerBase *) scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES);
					return l_skip_inline(scanner);
				}
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_QUESTION_MARK, 1);
				break;
			}
			case '%' : {
				if (lookahead[1]=='>') {
					base_class->advance((GroRunScannerBase *) scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES);
					base_class->advance((GroRunScannerBase *) scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES);
					return l_skip_inline(scanner);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_MOD_EQUAL, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_MOD, 1);
				}
				break;
			}
			case ',' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_COMMA, 1);
				break;
			}
			case '=' : {
				if (lookahead[1]=='=' && lookahead[2]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_IS_IDENTICAL, 3);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_IS_EQUAL, 2);
				} else if (lookahead[1]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_DOUBLE_ARROW, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_ASSIGN, 1);
				}
				break;
			}

			case '|' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_OR_EQUAL, 2);
				} else if (lookahead[1]=='|') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_BOOLEAN_OR, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_OR, 1);
				}
				break;
			}


			case '&' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_AND_EQUAL, 2);
				} else if (lookahead[1]=='&') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_BOOLEAN_AND, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_AMPERSAND, 1);
				}
				break;
			}


			case ':' : {
				if (lookahead[1]==':') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_PAAMAYIM_NEKUDOTAYIM, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_COLON, 1);
				}
				break;
			}


			case '$' : {
				if (lookahead[1]=='{') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_DOLLAR_OPEN_CURLY_BRACES, 2);
				} else if (g_unichar_isalpha(lookahead[1]) || lookahead[1]=='_') {
					result = l_scan_for_variable(scanner);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_DOLLAR, 1);
				}
				break;
			}

			case '+' : {
				if (lookahead[1]=='+') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_INC, 2);
				} else if (lookahead[1]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_PLUS_EQUAL, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_PLUS, 1);
				}
				break;
			}

			case '-' : {
				if (lookahead[1]=='-') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_DEC, 2);
				} else if (lookahead[1]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_MINUS_EQUAL, 2);
				} else if (lookahead[1]=='>') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_OBJECT_OPERATOR, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_MINUS, 1);
				}
				break;
			}

			case '*' : {
				if (lookahead[1]=='*' && lookahead[2]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_POW_EQUAL, 3);
				} else if (lookahead[1]=='*') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_POW, 2);
				} else if (lookahead[1]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_MUL_EQUAL, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_MUL, 1);
				}
				break;
			}

			case '/' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_DIV_EQUAL, 2);
				} else if (lookahead[1]=='/') {
					result = base_class->scanForEndOfLine((GroRunScannerBase *) scanner, MUL_PHP_SYM_COMMENT, TRUE);
				} else if (lookahead[1]=='*') {
					result = l_scan_for_comment(scanner);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_OP_DIV, 1);
				}
				break;
			}


			case '\'' : {
				result = base_class->scanForQuotedString((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_CONSTANT_ENCAPSED_STRING);
			} break;


			case '0' : case '1' : case '2' : case '3' : case '4' : case '5' :
			case '6' : case '7' : case '8' : case '9' : {
				result = l_scan_number(scanner);
				break;
			}

			default: {
				if (g_unichar_isalpha(lookahead[0]) || lookahead[0]=='_') {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_STRING, TRUE);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES);
				}
			} break;

		}
	}
	return result;
}


static GroRunIToken *l_scan_for_comment(MulPhpScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
//	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(TRUE) {
		if ((lookahead[0]=='*') && (lookahead[1]=='/')) {
			return base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_COMMENT, 2);
		} else if (lookahead[0]==-1) {
			break;
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	return base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PHP_SYM_COMMENT, 0);
}

static GroRunIToken *l_scan_for_variable(MulPhpScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
//	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	CatStringWo *buf = cat_string_wo_new();
	cat_string_wo_append_unichar(buf, lookahead[0]);
	long long start_row = base_class->getRow((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	while(g_unichar_isalpha(lookahead[0]) || lookahead[0]=='_' || g_unichar_isdigit(lookahead[0])) {
		cat_string_wo_append_unichar(buf, lookahead[0]);
		base_class->advance((GroRunScannerBase *) scanner, 0);
		long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
		if (end_row!=start_row) {
			break;
		}
	}

	int last_col = base_class->getColumn((GroRunScannerBase *) scanner);

	return base_class->createToken((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_VARIABLE, start_row, last_col, buf);
}


static GroRunIToken *l_scan_for_heredoc(MulPhpScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
//	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	CatStringWo *buf = cat_string_wo_new();

	base_class->advance((GroRunScannerBase *) scanner, 0);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	long long start_row = base_class->getRow((GroRunScannerBase *) scanner);
	while(TRUE) {
		if (lookahead[0]==-1) {
			break;
		}
		long long row = base_class->getRow((GroRunScannerBase *) scanner);
		if (row!=start_row) {
			break;
		}
		cat_string_wo_append_unichar(buf, lookahead[0]);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}

	int heredoc_len = cat_string_wo_length(buf);
	CatStringWo *buf2 = cat_string_wo_new();
	long long old_row = start_row;
	while(TRUE) {
		if (lookahead[0]==-1) {
			break;
		}
		long long row = base_class->getRow((GroRunScannerBase *) scanner);
		if (old_row!=start_row) {
			cat_string_wo_clear(buf2);
		} else {
			cat_string_wo_append_unichar(buf2, lookahead[0]);
			if (cat_string_wo_length(buf2)>heredoc_len) {
				cat_string_wo_remove_range(buf2, 0, cat_string_wo_length(buf2)-heredoc_len);
			}
			if (cat_string_wo_equal(buf,buf2)) {
				break;
			}
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
		old_row = row;
	}

	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, MUL_PHP_SYM_T_VARIABLE, row, column, NULL);
}



//static gboolean l_scan_digits(MulPhpScanner *scanner) {
//	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
//	gboolean got_digit = FALSE;
//	while(TRUE) {
//		if (priv->nextChar<'0' || priv->nextChar>'9') {
//			break;
//		}
//		got_digit = TRUE;
//		*lastValid = priv->column+1;
//		_next_char(lua_scanner);
//		if (priv->input_status != CAT_STREAM_OK) {
//			break;
//		}
//	}
//	return got_digit;
//}
//
//
//static gboolean l_scan_extended_digits(MulPhpScanner *scanner) {
//	MulLuaScannerPrivate *priv = mul_lua_scanner_get_instance_private(lua_scanner);
//	gboolean got_digit = FALSE;
//	while((priv->nextChar>='0' && priv->nextChar<='9')
//				|| (priv->nextChar>='A' && priv->nextChar<='F')
//				|| (priv->nextChar>='a' && priv->nextChar<='f')) {
//		got_digit = TRUE;
//		*lastValid = priv->column+1;
//		_next_char(lua_scanner);
//		if (priv->input_status != CAT_STREAM_OK) {
//			break;
//		}
//	}
//	return got_digit;
//}

//    {integer}(\.{integer})?({exponent})?
// |  base HASH based_integer (DOT based_integer)? HASH ([eE][-+]?{integer})?
// |  (base COLON based_integer (DOT based_integer)? COLON ([eE][-+]?{integer})?)


static gboolean l_scan_digits(MulPhpScanner *scanner, gunichar *lookahead) {
	gboolean result = FALSE;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	while(TRUE) {
		if (lookahead[0]>='0' && lookahead[0]<='9') {
			base_class->advance((GroRunScannerBase *) scanner, 0);
			result = TRUE;
			continue;
		}
		break;
	}
	return result;
}


static GroRunIToken *l_scan_number(MulPhpScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
//	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	gboolean has_dot = FALSE;
	l_scan_digits(scanner, lookahead);
	if (lookahead[0]=='.') {
		has_dot = TRUE;
		base_class->advance((GroRunScannerBase *) scanner, 0);
		l_scan_digits(scanner, lookahead);
	}

	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, has_dot ? MUL_PHP_SYM_T_DNUMBER : MUL_PHP_SYM_T_LNUMBER, row, column, NULL);
}

/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulPhpScanner *scanner = MUL_PHP_SCANNER(self);
	GroRunIToken *result = l_next_token(scanner);
	cat_log_debug("next=%O", result);
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
