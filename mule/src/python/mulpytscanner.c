/*
   File:    mulpytscanner.c
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 13, 2016
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

#include "mulpytscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulPytScanner"
#include <logging/catlog.h>

struct _MulPytScannerPrivate {
	int prevColumn;
	int indentLevel;
	int parrenLevel;
	gboolean last_was_newline;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulPytScanner, mul_pyt_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_ADD_PRIVATE(MulPytScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gunichar l_scanner_advance(GroRunScannerBase *scanner, int advance_flags);

static void mul_pyt_scanner_class_init(MulPytScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GroRunScannerBaseClass *scanner_base_class = GRORUN_SCANNER_BASE_CLASS(clazz);
	scanner_base_class->advance = l_scanner_advance;
}

static void mul_pyt_scanner_init(MulPytScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	MulPytScanner *instance = MUL_PYT_SCANNER(object);
//	MulPytScannerPrivate *priv = mul_pyt_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(mul_pyt_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_pyt_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulPytScanner *mul_pyt_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	MulPytScanner *result = g_object_new(MUL_PYT_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	MulPytScannerPrivate *priv = mul_pyt_scanner_get_instance_private(result);
	priv->indentLevel = 0;
	priv->parrenLevel = 0;
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 5);
	return result;
}


gboolean mul_pyt_scanner_is_eof(MulPytScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, MUL_PYT_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}


static gunichar l_scanner_advance(GroRunScannerBase *scanner, int advance_flags) {
	MulPytScannerPrivate *priv = mul_pyt_scanner_get_instance_private((MulPytScanner *) scanner);
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	priv->prevColumn = base_class->getColumn((GroRunScannerBase *) scanner);
	return GRORUN_SCANNER_BASE_CLASS(mul_pyt_scanner_parent_class)->advance(scanner, advance_flags);
}


static gboolean l_is_python_letter_start(gunichar unichar);
static GroRunIToken *l_scan_for_string(MulPytScanner *scanner);
static GroRunIToken *l_scan_for_number(MulPytScanner *scanner);
static GroRunIToken *l_scan_for_octal_or_hex_number(MulPytScanner *scanner);


static GroRunIToken *l_next_token(MulPytScanner *scanner) {
	MulPytScannerPrivate *priv = mul_pyt_scanner_get_instance_private(scanner);
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	gboolean last_was_newline = priv->last_was_newline;
	priv->last_was_newline = FALSE;

	while(result==NULL) {


		int column = base_class->getColumn((GroRunScannerBase *) scanner);
		cat_log_debug("column=%d, parrenLevel=%d", column, priv->parrenLevel);

		if (last_was_newline && priv->parrenLevel==0) {
			int indent = 0;

			long long row = base_class->getRow((GroRunScannerBase *) scanner);
			/* skip the current return/linefeed character */

			while(TRUE) {
				cat_log_debug("Dedenting or indenting lookahead=%d, indent=%d", lookahead[0], indent);
				switch(lookahead[0]) {
					case -1 :
						break;
//						return base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_END_OF_INPUT, -1, -1, NULL);
//						goto out;

					case ' ' : {
						indent++;
						base_class->advance((GroRunScannerBase *) scanner, 0);
						continue;
					}
					case '\t' : {
						indent = (indent+8);
						indent = indent - indent%8;
						base_class->advance((GroRunScannerBase *) scanner, 0);
						continue;
					}
//					case 0xa :
//					case 0xc :
//					case 0xd : {
//						if (last_was_newline) {
//							indent=0;
//							base_class->advance((GroRunScannerBase *) scanner, 0);
//							column = base_class->getColumn((GroRunScannerBase *) scanner);
//	//						return base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_NEWLINE, row, column+1, NULL);
//							continue;
//						}
//					}
//					default :
//						emptyLineTillNow = FALSE;
				}
				break;
			}
			column = base_class->getColumn((GroRunScannerBase *) scanner);
			cat_log_debug("priv->indentLevel=%d, indent=%d", priv->indentLevel, indent);
				if (indent<priv->indentLevel) {
					priv->indentLevel = indent;
					return base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_DEDENT, row, column+1, NULL);
				} else if (indent>priv->indentLevel) {
					priv->indentLevel = indent;
					return base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_INDENT, row, column+1, NULL);
	//			} else {
	//				return base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_NEWLINE, row, column+1, NULL);
				}
		}


		base_class->markLocation((GroRunScannerBase *) scanner);
		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_END_OF_INPUT, -1, -1, NULL);
				break;

			case 0xa :
			case 0xd : {
				if (priv->parrenLevel>0) {
					base_class->advance((GroRunScannerBase *) scanner, 0);
				} else {
					priv->last_was_newline = TRUE;
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_NEWLINE, 1);
				}
			} break;


			case '(':
				priv->parrenLevel++;
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_LPARREN, 1);
				break;

			case ')':
				priv->parrenLevel--;
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_RPARREN, 1);
				break;

			case '{':
				priv->parrenLevel++;
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_LACCOL, 1);
				break;

			case '}':
				priv->parrenLevel--;
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_RACCOL, 1);
				break;

			case '[':
				priv->parrenLevel++;
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_LSQBR, 1);
				break;

			case ']':
				priv->parrenLevel--;
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_RSQBR, 1);
				break;

			case '.':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_DOT, 1);
				break;

			case ':':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_COLON, 1);
				break;

			case ';':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_SEMI, 1);
				break;

			case ',':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_COMMA, 1);
				break;

			case '@':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_CHARAT, 1);
				break;


			case '=': {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPEQ, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_EQUALS, 1);
				}
			} break;


			case '+' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_ADD_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPADD, 1);
				}
			} break;

			case '-' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_SUB_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPSUB, 1);
				}
			} break;

			case '*' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_MUL_ASSIGN, 2);
				} else if (lookahead[1]=='*') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_MULMUL_ASSIGN, 3);
					} else {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_DOUBLEASTERISK, 2);
					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPMUL, 1);
				}
			} break;

			case '/' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_DIV_ASSIGN, 2);
				} else if (lookahead[1]=='/') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_DIVDIV_ASSIGN, 3);
					} else {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPDIVDIV, 2);
					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPDIV, 1);
				}
			} break;

			case '%' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_MOD_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPMOD, 1);
				}
			} break;

			case '&' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_AND_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPAND, 1);
				}
			} break;

			case '|' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OR_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPOR, 1);
				}
			} break;

			case '^' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_XOR_ASSIGN, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPXOR, 1);
				}
			} break;


			case '!' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPNE, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPNEG, 1);
				}
			} break;


			case '<' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPLE, 2);
				} else if (lookahead[1]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPDIF, 2);
				} else if (lookahead[1]=='<') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_SHL_ASSIGN, 3);
					} else {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_SHIFTLEFT, 2);
					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPLT, 1);
				}
			} break;

			case '>' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPGE, 2);
				} else if (lookahead[1]=='>') {
					if (lookahead[2]=='=') {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_SHR_ASSIGN, 3);
					} else {
						result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_SHIFTRIGHT, 2);
					}
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_OPGT, 1);
				}
			} break;


			case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				result = l_scan_for_number(scanner);
				break;

			case '0' :
				result = l_scan_for_octal_or_hex_number(scanner);
				break;

			case '"' :
				result = l_scan_for_string(scanner);
				break;

			case '\'' :
				if (lookahead[2]=='\'') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_STRING, 3);
				} else if (lookahead[1]=='\\' && lookahead[3]=='\'') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_STRING, 4);
				}

				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_PYT_SYM_SINGLEQUOTE, 1);
				break;

				//
//			case '~': _next_char(python_scanner); return mul_token_new_symbol_pos(MUL_PYT_SYMBOL_TERM_OP_NOT, python_scanner->mark, python_scanner->column, row);
//
//			case '?': _next_char(python_scanner); return mul_token_new_symbol_pos(MUL_PYT_SYMBOL_TERM_OP_COND, python_scanner->mark, python_scanner->column, row);


			case '#' :
				result = base_class->scanForEndOfLine((GroRunScannerBase *) scanner, MUL_PYT_SYM_EOL_COMMENT, TRUE);
				break;

			default: {
				if (l_is_python_letter_start(lookahead[0])) {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, MUL_PYT_SYM_NAME, TRUE);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, 0);
				}
			} break;



		}
	}
	return result;
}

static gboolean l_is_python_letter_start(gunichar unichar) {
	return ((unichar>='a' && unichar<='z')
			|| (unichar>='A' && unichar<='Z')
			|| (unichar=='_')) ? TRUE : FALSE;
}

//static gboolean l_is_python_letter(gunichar unichar) {
//	return (l_is_python_letter_start(unichar)
//			|| (unichar>='0' && unichar<='9')) ? TRUE : FALSE;
//}

static GroRunIToken *l_scan_for_string(MulPytScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	gboolean in_triple_mode = FALSE;

	if (lookahead[0]=='"' && lookahead[1]=='"' && lookahead[2]=='"') {
		in_triple_mode = TRUE;
		base_class->advance((GroRunScannerBase *) scanner, 0);
		base_class->advance((GroRunScannerBase *) scanner, 0);
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}


	gboolean escaped = FALSE;

	while(result==NULL) {
		gunichar ch = base_class->advance((GroRunScannerBase *) scanner, 0);
		if (ch==-1) {
			int end_column = base_class->getColumn((GroRunScannerBase *) scanner);
			long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
			result = base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_STRING, end_row, end_column+1, NULL);		/* TODO: mark as unterminated token */
		} else if (in_triple_mode) {
			if (lookahead[0]=='"' && lookahead[1]=='"' && lookahead[2]=='"') {
				base_class->advance((GroRunScannerBase *) scanner, 0);
				base_class->advance((GroRunScannerBase *) scanner, 0);
				int end_column = base_class->getColumn((GroRunScannerBase *) scanner);
				long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
				base_class->advance((GroRunScannerBase *) scanner, 0);
				result = base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_STRING, end_row, end_column+1, NULL);
			}
		} else {
			if (!escaped && lookahead[0]=='"') {
				int end_column = base_class->getColumn((GroRunScannerBase *) scanner);
				long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
				base_class->advance((GroRunScannerBase *) scanner, 0);
				result = base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_STRING, end_row, end_column+1, NULL);
			}
		}

		if (lookahead[0]=='\\') {
			escaped = !escaped;
		} else {
			escaped = FALSE;
		}
	}
	return result;
}

static GroRunIToken *l_scan_for_number(MulPytScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(TRUE) {
		gunichar ch = lookahead[1];
		if (ch<'0' || ch>'9') {
			break;
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}
	int end_column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
	result = base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_NUMBER, end_row, end_column+1, NULL);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	return result;
}

static GroRunIToken *l_scan_for_octal_or_hex_number(MulPytScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	gunichar ch = base_class->advance((GroRunScannerBase *) scanner, 0);
	gboolean isHex = FALSE;
	gboolean keep_looping = TRUE;
	if ((ch=='x') || (ch=='X')) {
		/* '0x' */
		isHex = TRUE;
	} else if ((ch>='0') && (ch<'8')) {
		/* '02' */
	} else {
		/* '0' */
		keep_looping = FALSE;
	}

	while(keep_looping) {
		gunichar ch = lookahead[1];
		switch(ch) {
			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': {
			} break;
			case '8': case '9' : case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
			case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' : {
				if (!isHex) {
					keep_looping = FALSE;
				}
			} break;
			default : {
				keep_looping = FALSE;
			}
		}
	}

	int end_column = base_class->getColumn((GroRunScannerBase *) scanner);
	long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
	result = base_class->createToken((GroRunScannerBase *) scanner, MUL_PYT_SYM_NUMBER, end_row, end_column+1, NULL);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	return result;
}


/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulPytScanner *scanner = MUL_PYT_SCANNER(self);
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
