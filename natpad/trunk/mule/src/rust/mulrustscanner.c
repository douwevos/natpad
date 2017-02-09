/*
   File:    mulrustscanner.c
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#include "mulrustscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulRustScanner"
#include <logging/catlog.h>

struct _MulRustScannerPrivate {
	void *dummy;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulRustScanner, mul_rust_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_ADD_PRIVATE(MulRustScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_rust_scanner_class_init(MulRustScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_rust_scanner_init(MulRustScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	MulRustScanner *instance = MUL_RUST_SCANNER(object);
//	MulRustScannerPrivate *priv = mul_rust_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(mul_rust_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_rust_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulRustScanner *mul_rust_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	MulRustScanner *result = g_object_new(MUL_TYPE_RUST_SCANNER, NULL);
	cat_ref_anounce(result);
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 7);
	return result;
}

gboolean mul_rust_scanner_is_eof(MulRustScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, MUL_RUST_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}


// https://github.com/rust-lang/rust/blob/master/src/grammar/lexer.l

//#define MUL_RUST_SYM_END_OF_INPUT                                                                    0
//#define MUL_RUST_SYM_ERROR                                                                           1
//#define MUL_RUST_SYM_LIT_STR                                                                         30
//#define MUL_RUST_SYM_LIT_STR_RAW                                                                     31
//#define MUL_RUST_SYM_LIT_BYTE_STR                                                                    32
//#define MUL_RUST_SYM_LIT_BYTE_STR_RAW                                                                33
//#define MUL_RUST_SYM_IDENT                                                                           34
//#define MUL_RUST_SYM_LIFETIME                                                                        36
//#define MUL_RUST_SYM_SELF                                                                            37
//#define MUL_RUST_SYM_STATIC                                                                          38
//#define MUL_RUST_SYM_AS                                                                              39
//#define MUL_RUST_SYM_BREAK                                                                           40
//#define MUL_RUST_SYM_CRATE                                                                           41
//#define MUL_RUST_SYM_ELSE                                                                            42
//#define MUL_RUST_SYM_ENUM                                                                            43
//#define MUL_RUST_SYM_EXTERN                                                                          44
//#define MUL_RUST_SYM_FALSE                                                                           45
//#define MUL_RUST_SYM_FN                                                                              46
//#define MUL_RUST_SYM_FOR                                                                             47
//#define MUL_RUST_SYM_IF                                                                              48
//#define MUL_RUST_SYM_IMPL                                                                            49
//#define MUL_RUST_SYM_IN                                                                              50
//#define MUL_RUST_SYM_LET                                                                             51
//#define MUL_RUST_SYM_LOOP                                                                            52
//#define MUL_RUST_SYM_MATCH                                                                           53
//#define MUL_RUST_SYM_MOD                                                                             54
//#define MUL_RUST_SYM_MOVE                                                                            55
//#define MUL_RUST_SYM_MUT                                                                             56
//#define MUL_RUST_SYM_PRIV                                                                            57
//#define MUL_RUST_SYM_PUB                                                                             58
//#define MUL_RUST_SYM_REF                                                                             59
//#define MUL_RUST_SYM_RETURN                                                                          60
//#define MUL_RUST_SYM_STRUCT                                                                          61
//#define MUL_RUST_SYM_TRUE                                                                            62
//#define MUL_RUST_SYM_TRAIT                                                                           63
//#define MUL_RUST_SYM_TYPE                                                                            64
//#define MUL_RUST_SYM_UNSAFE                                                                          65
//#define MUL_RUST_SYM_USE                                                                             66
//#define MUL_RUST_SYM_WHILE                                                                           67
//#define MUL_RUST_SYM_CONTINUE                                                                        68
//#define MUL_RUST_SYM_PROC                                                                            69
//#define MUL_RUST_SYM_BOX                                                                             70
//#define MUL_RUST_SYM_CONST                                                                           71
//#define MUL_RUST_SYM_WHERE                                                                           72
//#define MUL_RUST_SYM_TYPEOF                                                                          73
//#define MUL_RUST_SYM_INNER_DOC_COMMENT                                                               74
//#define MUL_RUST_SYM_OUTER_DOC_COMMENT                                                               75
//#define MUL_RUST_SYM_SHEBANG                                                                         76
//#define MUL_RUST_SYM_SHEBANG_LINE                                                                    77


static GroRunIToken *l_scan_lit_single_quoted(MulRustScanner *scanner, int sym_id, int initial_skip, int max_digits, gboolean digits_optional);
static GroRunIToken *l_scan_raw_string(MulRustScanner *scanner);
static GroRunIToken *l_scan_doc_comment(MulRustScanner *scanner, int sym_id);
static GroRunIToken *l_scan_hex_int(MulRustScanner *scanner);
static GroRunIToken *l_scan_oct_int(MulRustScanner *scanner);
static GroRunIToken *l_scan_bin_int(MulRustScanner *scanner);
static GroRunIToken *l_scan_int_or_float(MulRustScanner *scanner);


static GroRunIToken *l_next_token(MulRustScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(result==NULL) {

		base_class->markLocation((GroRunScannerBase *) scanner);
//		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_END_OF_INPUT, -1, -1, NULL);
				break;

			case '(':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_LPAREN, 1);
				break;

			case ')':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_RPAREN, 1);
				break;

			case '{':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_LCUBRACE, 1);
				break;

			case '}':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_RCUBRACE, 1);
				break;

			case '[':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_LSQBRACE, 1);
				break;

			case ']':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_RSQBRACE, 1);
				break;


			case '0' : {
				if (lookahead[1]=='x') { // 0x[0-9a-fA-F_]+
					result = l_scan_hex_int(scanner);
				} else if (lookahead[1]=='o') { // 0o[0-8_]+
					result = l_scan_oct_int(scanner);
				} else if (lookahead[1]=='b') { // 0o[0-8_]+
					result = l_scan_bin_int(scanner);
				} else {
					result = l_scan_int_or_float(scanner);
				}
				break;
			}
			case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' : {
				result = l_scan_int_or_float(scanner);
			} break;

			case '>':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_GE, 2);
				} else if (lookahead[1]=='>') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_SHREQ, 3);
					} else {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_SHR, 2);
					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_GT, 1);
				}
				break;

			case '<':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LE, 2);
				} else if (lookahead[1]=='-') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LARROW, 2);
				} else if (lookahead[1]=='<') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_SHLEQ, 3);
					} else {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_SHL, 2);
					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_LT, 1);
				}
				break;


			case ';':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_SEMI, 1);
				break;

			case ':':
				if (lookahead[1]==':') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_MOD_SEP, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_COLON, 1);
				}
				break;

			case '?':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_QUESTIONMARK, 1);
				break;

			case '!':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_NE, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_EXCLAMATION, 1);
				}
				break;

			case '|':
				if (lookahead[1]=='|') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_OROR, 2);
				} else if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_OREQ, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_VERTICAL_LINE, 1);
				}
				break;

			case ',':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_COMMA, 1);
				break;

			case '"':
				result = base_class->scanForQuotedString((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_STR);
				break;

			case '&':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_ANDEQ, 2);
				} else if (lookahead[1]=='&') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_ANDAND, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_AMPERSAND, 1);
				}
				break;

			case '@':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_AT, 1);
				break;

			case '~':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_NEG, 1);
				break;

			case '^':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_CARETEQ, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_CIRCUMFLEX, 1);
				}
				break;

			case '#':
				if (lookahead[1]=='!') {
//					if (lookahead[2]=='[') {
//					shebang
//					} else {
						result = base_class->scanForEndOfLine((GroRunScannerBase *) scanner, MUL_RUST_SYM_SHEBANG_LINE, 2);
//					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_HASH, 1);
				}
				break;

			case '.':
				if (lookahead[1]=='.') {
					if (lookahead[2]=='.') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_DOTDOTDOT, 3);
					} else {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_DOTDOT, 2);
					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_DOT, 1);
				}
				break;

			case '+':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_PLUSEQ, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_PLUS, 1);
				}
				break;

			case '-':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_MINUSEQ, 2);
				}else if (lookahead[1]=='>') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_RARROW, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_MINUS, 1);
				}
				break;

			case '*':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_STAREQ, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_MUL, 1);
				}
				break;

			case '/':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_SLASHEQ, 2);
				} else if (lookahead[1]=='*') {
					if (lookahead[2]=='!') {
						result = l_scan_doc_comment(scanner, MUL_RUST_SYM_INNER_DOC_COMMENT);
					} else if (lookahead[2]=='*') {
						result = l_scan_doc_comment(scanner, MUL_RUST_SYM_OUTER_DOC_COMMENT);
					} else {
						result = l_scan_doc_comment(scanner, MUL_RUST_SYM_BLOCK_COMMENT);
					}
				} else if (lookahead[1]=='/') {
					// TODO support //! and ///
					result = base_class->scanForEndOfLine((GroRunScannerBase *) scanner, MUL_RUST_SYM_BLOCK_COMMENT, FALSE);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_DIV, 1);
				}
				break;

			case '%':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_PERCENTEQ, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_MOD, 1);
				}
				break;

			case '=':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_EQEQ, 2);
				} else if (lookahead[1]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_FAT_ARROW, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_L_IS, 1);
				}
				break;

			case '\'' :
				if ((lookahead[1]=='s') && (lookahead[2]=='t') && (lookahead[3]=='a') && (lookahead[4]=='t') && (lookahead[5]=='i') && (lookahead[6]=='c')) {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_STATIC_LIFETIME, 7);
				} else if (lookahead[1]=='\\') {


					gunichar la2 = lookahead[2];
					if ((la2=='n') || (la2=='r') || (la2=='t') || (la2=='\\') || (la2=='\'') || (la2=='"') || (la2=='0')) {
						if (lookahead[4]=='\'') { // b'\[nrt\'"0]'
							result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_CHAR, 5);
						} else {
							// TODO error
							result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_CHAR, 4);
						}
					} else if (la2=='x') {
						result = l_scan_lit_single_quoted(scanner, MUL_RUST_SYM_LIT_CHAR, 4, 2, FALSE);
					} else if (la2=='u') {
						result = l_scan_lit_single_quoted(scanner, MUL_RUST_SYM_LIT_CHAR, 4, 6, TRUE);
					}

				} else if (lookahead[2]=='\'') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_CHAR, 3);
				} else if (lookahead[1]=='\'') {
					// TODO ERROR
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_CHAR, 2);
				}
				if (result==NULL) {
					// TODO error
					base_class->advance((GroRunScannerBase *) scanner, 0);
				}

				break;


			default: {
				if ((lookahead[0]=='b') && lookahead[1]=='\'') {
					if (lookahead[2]=='\\') {
						gunichar la3 = lookahead[3];
						if ((la3=='n') || (la3=='r') || (la3=='t') || (la3=='\\') || (la3=='\'') || (la3=='"') || (la3=='0')) {
							if (lookahead[4]=='\'') { // b'\[nrt\'"0]'
								result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_BYTE, 5);
							} else {
								// TODO error
								result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_BYTE, 4);
							}
						} else if (la3=='x') {
							result = l_scan_lit_single_quoted(scanner, MUL_RUST_SYM_LIT_BYTE, 4, 2, FALSE);
						} else if (la3=='u') {
							result = l_scan_lit_single_quoted(scanner, MUL_RUST_SYM_LIT_BYTE, 4, 4, FALSE);
						} else if (la3=='U') {
							result = l_scan_lit_single_quoted(scanner, MUL_RUST_SYM_LIT_BYTE, 4, 8, FALSE);
						}
					}
				} else if (lookahead[0]=='r' && lookahead[1]=='#') {
					result = l_scan_raw_string(scanner);
				} else if (lookahead[0]=='_' && !(g_unichar_isalpha(lookahead[1]) || lookahead[1]=='_')) {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_RUST_SYM_UNDERSCORE, 1);
				}

				if (result) {
					break;
				}

				if (g_unichar_isalpha(lookahead[0]) || lookahead[0]=='_') {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, MUL_RUST_SYM_IDENT, TRUE);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, 0);
				}
			} break;
		}
	}

	return result;
}

static GroRunIToken *l_scan_lit_single_quoted(MulRustScanner *scanner, int sym_id, int initial_skip, int max_digits, gboolean digits_optional) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(initial_skip>0) {
		base_class->advance((GroRunScannerBase *) scanner, 0);
		initial_skip--;
	}

	while(max_digits>0) {
		gunichar la = lookahead[0];
		if (!((la>='0' && la<='9') || (la>='a' && la<='f') || (la>='A' && la<='F'))) {
			break;
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
		max_digits--;
	}

	gboolean err = (!digits_optional) && max_digits>0;
	if (lookahead[0]=='\'') {
		base_class->advance((GroRunScannerBase *) scanner, 0);
	} else {
		err = TRUE;
	}

	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);

// TODO mark error
//	if (err) {
//	}
	return base_class->createToken((GroRunScannerBase *) scanner, sym_id, row, column, NULL);
}


static GroRunIToken *l_scan_raw_string(MulRustScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	int has_count = 0;
	while(TRUE) {
		if (lookahead[0]=='#') {
			has_count++;
			base_class->advance((GroRunScannerBase *) scanner, 0);
		} else {
			break;
		}
	}

	if (lookahead[0]=='"') {
		base_class->advance((GroRunScannerBase *) scanner, 0);
	} else {
		int column = base_class->getColumn((GroRunScannerBase *) scanner);
		long long row = base_class->getRow((GroRunScannerBase *) scanner);
		// TODO mark error
		return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_STR_RAW, row, column, NULL);
	}


	int has_b_cnt = -1;
	while(TRUE) {
		if (lookahead[0]=='"') {
			has_b_cnt = 0;
		} else if (lookahead[0]==-1) {
			// TODO error
			break;
		} else if (has_b_cnt>=0) {
			if (lookahead[0]=='#') {
				has_b_cnt++;
				if (has_b_cnt==has_count) {
					base_class->advance((GroRunScannerBase *) scanner, 0);
					break;
				}
			} else {
				has_b_cnt = -1;
			}
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}

	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_STR_RAW, row, column, NULL);
}

static GroRunIToken *l_scan_doc_comment(MulRustScanner *scanner, int sym_id) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	int level = 1;
	while(TRUE) {
		if ((lookahead[0]=='/') && (lookahead[1]=='*')) {
			level++;
			base_class->advance((GroRunScannerBase *) scanner, 0);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			continue;
		} else if ((lookahead[0]=='*') && (lookahead[1]=='/')) {
			level--;
			base_class->advance((GroRunScannerBase *) scanner, 0);
			base_class->advance((GroRunScannerBase *) scanner, 0);
			if (level <= 0) {
				break;
			}
			continue;
		}
		if (lookahead[0]==-1) {
			// TODO error
			break;
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, sym_id, row, column, NULL);
}

static GroRunIToken *l_scan_hex_int(MulRustScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	// skip 0x
	base_class->advance((GroRunScannerBase *) scanner, 0);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	while(TRUE) {
		gunichar ch = lookahead[0];
		if ((ch>='0' && ch<='9') || (ch>='A' && ch<='F') || (ch>='a' && ch<='f') || (ch=='_')) {
			base_class->advance((GroRunScannerBase *) scanner, 0);
			continue;
		}
		break;
	}
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_INTEGER, row, column, NULL);
}

static GroRunIToken *l_scan_oct_int(MulRustScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	// skip 0x
	base_class->advance((GroRunScannerBase *) scanner, 0);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	while(TRUE) {
		gunichar ch = lookahead[0];
		if ((ch>='0' && ch<='8') || (ch=='_')) {
			base_class->advance((GroRunScannerBase *) scanner, 0);
			continue;
		}
		break;
	}
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_INTEGER, row, column, NULL);
}


static GroRunIToken *l_scan_bin_int(MulRustScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	// skip 0x
	base_class->advance((GroRunScannerBase *) scanner, 0);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	while(TRUE) {
		gunichar ch = lookahead[0];
		if ((ch=='0') || (ch=='1') || (ch=='_')) {
			base_class->advance((GroRunScannerBase *) scanner, 0);
			continue;
		}
		break;
	}
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_INTEGER, row, column, NULL);
}

static gboolean l_scan_digits(MulRustScanner *scanner, gboolean first_underscore) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	gboolean result = FALSE;
	while(TRUE) {
		gunichar ch = lookahead[0];
		if ((ch>='0' && ch<='9') || (ch=='_' && first_underscore)) {
			base_class->advance((GroRunScannerBase *) scanner, 0);
			first_underscore = TRUE;
			result = TRUE;
			continue;
		}
		break;
	}
	return result;
}

static GroRunIToken *l_scan_int_or_float(MulRustScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	l_scan_digits(scanner, FALSE);


	if (lookahead[0]=='e' || lookahead[0]=='E') {
		int p = 1;
		if (lookahead[1]=='+' || lookahead[1]=='-') {
			p++;
		}
		if ((lookahead[p]>='0' && lookahead[p]<='9') || (lookahead[p]=='_')) {
			//	[0-9][0-9_]*[eE][-\+]?[0-9_]+)          { BEGIN(suffix); return LIT_FLOAT; }
			// skip e/E
			base_class->advance((GroRunScannerBase *) scanner, 0);
			if (p>1) {
				// skip +/-
				base_class->advance((GroRunScannerBase *) scanner, 0);
			}
			l_scan_digits(scanner, TRUE);
			int column = base_class->getColumn((GroRunScannerBase *) scanner);
			long long row = base_class->getRow((GroRunScannerBase *) scanner);
			return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_FLOAT, row, column, NULL);
		}
	}

	if (!(lookahead[0]=='.' && ((lookahead[1]>='0' && lookahead[1]<='9') || (lookahead[1]=='_')) )) {
//		[0-9][0-9_]*
		int column = base_class->getColumn((GroRunScannerBase *) scanner);
		long long row = base_class->getRow((GroRunScannerBase *) scanner);
		return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_INTEGER, row, column, NULL);
	}

	/* skip \.[0-9_]* */
	while(TRUE) {
		if (lookahead[0]=='.' && ((lookahead[1]>='0' && lookahead[1]<='9') || (lookahead[1]=='_')) ) {
			base_class->advance((GroRunScannerBase *) scanner, 0);
			l_scan_digits(scanner, TRUE);
		} else {
			break;
		}
	}

//	[0-9][0-9_]*\.[0-9_]*([eE][-\+]?[0-9_]+)?          { BEGIN(suffix); return LIT_FLOAT; }
//	[0-9][0-9_]*(\.[0-9_]*)?[eE][-\+]?[0-9_]+ { BEGIN(suffix); return LIT_FLOAT; }

	l_scan_digits(scanner, TRUE);

	if (lookahead[0]=='e' || lookahead[0]=='E') {
		int p = 1;
		if (lookahead[1]=='+' || lookahead[1]=='-') {
			p++;
		}
		if ((lookahead[p]>='0' && lookahead[p]<='9') || (lookahead[p]=='_')) {
			//	[0-9][0-9_]*[eE][-\+]?[0-9_]+)          { BEGIN(suffix); return LIT_FLOAT; }
			// skip e/E
			base_class->advance((GroRunScannerBase *) scanner, 0);
			if (p>1) {
				// skip +/-
				base_class->advance((GroRunScannerBase *) scanner, 0);
			}
			l_scan_digits(scanner, TRUE);
		}
	}
	int column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long row = base_class->getRow((GroRunScannerBase *) scanner);
	return base_class->createToken((GroRunScannerBase *) scanner, MUL_RUST_SYM_LIT_FLOAT, row, column, NULL);
}


/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulRustScanner *scanner = MUL_RUST_SCANNER(self);
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
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
