/*
   File:    jagptokenizer.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 29, 2017
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

#include "jagptokenizer.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPTokenizer"
#include <logging/catlog.h>

typedef struct _CharInfo CharInfo;


struct _CharInfo {
	gunichar value;
	int left_column, left_row, left_offset;
	int right_column, right_row, right_offset;
};

struct _JagPTokenizerPrivate {
	CatIUtf8Scanner *input;
	CatStreamStatus input_status;

	int lookahead_max;
	int lookahead_size;
	int lookahead_head;
	CharInfo *lookahead;

	//
//	/** Allow binary literals.
//	 */
//	gboolean allowBinaryLiterals;
//
//	/** Allow underscores in literals.
//	 */
//	gboolean allowUnderscoresInLiterals;
//
//	/** The source language setting.
//	 */
//	Source source;
//
//	/** The log to be used for error reporting.
//	 */
//	Log log;
//
//	/** The token factory. */
//	Tokens tokens;
//
//	/** The token kind, set by nextToken().
//	 */
//	JagPTokenKind tk;
//
//	/** The token's radix, set by nextToken().
//	 */
//	int radix;
//
//	/** The token's name, set by nextToken().
//	 */
//	JagPName *name;
//
//	/** The position where a lexical error occurred;
//	 */
//	int errPos = -1;
//
//	/** The Unicode reader (low-level stream reader).
//	 */
//	UnicodeReader reader;
//
//	ScannerFactory fac;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPTokenizer, jagp_tokenizer, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPTokenizer)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
//
static void jagp_tokenizer_class_init(JagPTokenizerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_tokenizer_init(JagPTokenizer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPTokenizer *instance = JAGP_TOKENIZER(object);
//	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_tokenizer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_tokenizer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_scan_one_unichar_raw(JagPTokenizerPrivate *priv, CharInfo *old_ci, CharInfo *next_ci);

JagPTokenizer *jagp_tokenizer_new(CatIUtf8Scanner *input) {
	JagPTokenizer *result = g_object_new(JAGP_TYPE_TOKENIZER, NULL);
	cat_ref_anounce(result);
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(result);
	priv->input = cat_ref_ptr(input);
	priv->input_status = CAT_STREAM_OK;
	priv->lookahead_size = 9;
	priv->lookahead_max = 6;
	priv->lookahead_head = 0;
	priv->lookahead = g_new(CharInfo, priv->lookahead_size+10);
	cat_log_debug("priv->lookahead=%p", priv->lookahead);
	int idx;
	CharInfo fake = { 0 };
	fake.left_offset = 0;
	fake.right_offset = 0;

	CharInfo *old = &fake;
	for(idx=0; idx<priv->lookahead_max; idx++) {
		CharInfo *cur = priv->lookahead+idx;
		cat_log_debug("cur[%d]=%p", idx, cur);
		l_scan_one_unichar_raw(priv, old, cur);
		old = cur;
	}
	return result;
}



static void l_scan_one_unichar_raw(JagPTokenizerPrivate *priv, CharInfo *old_ci, CharInfo *next_ci) {
	cat_log_debug("old_ci=%p, next_ci=%p", old_ci, next_ci);
	gunichar result = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
	next_ci->value = result;
	next_ci->left_column = old_ci->right_column;
	next_ci->left_row = old_ci->right_row;
	next_ci->right_column = old_ci->right_column+1;
	next_ci->right_row = old_ci->right_row;
	next_ci->left_offset = old_ci->right_offset;
	next_ci->right_offset = next_ci->left_offset+1;

	if (result==0xa) {
		if ((old_ci->value != 0xd) || (old_ci->left_row==old_ci->right_row)) {
			next_ci->right_row++;
		}
		next_ci->right_column = 0;
		next_ci->right_offset = 0;
	} else if (result==0xd) {
		if ((old_ci->value != 0xa) || (old_ci->left_row==old_ci->right_row)) {
			next_ci->right_row++;
		}
		next_ci->right_column = 0;
		next_ci->right_offset = 0;
	} else if (result==-1) {
		next_ci->value = -1;
		next_ci->right_offset--;
	} else {
		if (result < 0x80) {
		} else if (result< (1<<12)) {
			next_ci->right_offset++;
		} else if (result< (1<<18)) {
			next_ci->right_offset+= 2;
		} else if (result< (1<<24)) {
			next_ci->right_offset+= 3;
		} else if (result< (1<<30)) {
			next_ci->right_offset+= 4;
		} else {
			next_ci->right_offset+= 5;
		}
	}
}

static CharInfo *l_scan_one_unichar(JagPTokenizerPrivate *priv) {
	int next_idx = (priv->lookahead_head+priv->lookahead_max) % priv->lookahead_size;
	priv->lookahead_head++;
	int old_idx = (next_idx-1+priv->lookahead_size) % priv->lookahead_size;
	CharInfo *cur = priv->lookahead+next_idx;
	CharInfo *old = priv->lookahead+old_idx;
	l_scan_one_unichar_raw(priv, old, cur);
	int lah_idx = priv->lookahead_head % priv->lookahead_size;
	return priv->lookahead+lah_idx;
}

static CharInfo *l_lookahead_at(JagPTokenizerPrivate *priv, int lookahead) {
	int lah_idx = (priv->lookahead_head+lookahead+priv->lookahead_size) % priv->lookahead_size;
	cat_log_debug("lah_idx=%d", lah_idx);
	return priv->lookahead+lah_idx;
}


static void l_dump_charinfo(const char *t, CharInfo *ci0) {
	cat_log_debug("%sch=%c:%x, left-off=%d:col=%d:row=%d, right-off=%d:col=%d:row=%d", t, (ci0->value<32 ? '.' : ci0->value), ci0->value, ci0->left_offset, ci0->left_column, ci0->left_row, ci0->right_offset, ci0->right_column, ci0->right_row);
}




static JagPToken *l_scan_full_comment(JagPTokenizer *tokenizer);
static JagPToken *l_scan_eol_comment(JagPTokenizer *tokenizer);
static JagPToken *l_scan_ident(JagPTokenizer *tokenizer);
static JagPToken *l_create_basic(JagPTokenizerPrivate *priv, JagPTokenKind kind, int cnt);
static JagPToken *l_scan_char_literal(JagPTokenizer *tokenizer);
static JagPToken *l_scan_string_literal(JagPTokenizer *tokenizer);
static JagPToken *l_scan_number(JagPTokenizer *tokenizer, int radix, int skip_cnt);


JagPToken *jagp_tokenizer_next(JagPTokenizer *tokenizer) {
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(tokenizer);
	JagPToken *result = NULL;

	while(result==NULL) {
		CharInfo *ci0 = l_lookahead_at(priv, 0);
		cat_log_debug("ch=%c:%x, left-off=%d:col=%d:row=%d, right-off=%d:col=%d:row=%d", (ci0->value<32 ? '.' : ci0->value), ci0->value, ci0->left_offset, ci0->left_column, ci0->left_row, ci0->right_offset, ci0->right_column, ci0->right_row);

		switch(ci0->value) {
			case -1 :
				return NULL;
//				break;

			case '[' : result = l_create_basic(priv, JAGP_KIND_LBRACKET, 1); break;
			case ']' : result = l_create_basic(priv, JAGP_KIND_RBRACKET, 1); break;
			case ',' : result = l_create_basic(priv, JAGP_KIND_COMMA, 1); break;
			case '?' : result = l_create_basic(priv, JAGP_KIND_QUES, 1); break;
			case '~' : result = l_create_basic(priv, JAGP_KIND_TILDE, 1); break;
			case '@' : result = l_create_basic(priv, JAGP_KIND_MONKEYS_AT, 1); break;
			case ';' : result = l_create_basic(priv, JAGP_KIND_SEMI, 1); break;

			case '{' : result = l_create_basic(priv, JAGP_KIND_LBRACE, 1); break;
			case '}' : result = l_create_basic(priv, JAGP_KIND_RBRACE, 1); break;
			case '(' : result = l_create_basic(priv, JAGP_KIND_LPAREN, 1); break;
			case ')' : result = l_create_basic(priv, JAGP_KIND_RPAREN, 1); break;


            case '0' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
                if (ci1->value == 'x' || ci1->value == 'X') {
//                    reader.scanChar();
//                    skipIllegalUnderscores();
                    l_scan_number(tokenizer, 16, 2);
                } else if (ci1->value == 'b' || ci1->value == 'B') {
//                    if (!allowBinaryLiterals) {
//                        lexError(pos, "unsupported.binary.lit", source.name);
//                        allowBinaryLiterals = true;
//                    }
//                    skipIllegalUnderscores();
                    result = l_scan_number(tokenizer, 2, 2);
                } else {
//                    if (ci1->value == '_') {
//                        int savePos = reader.bp;
//                        do {
//                            reader.scanChar();
//                        } while (reader.ch == '_');
//                        if (reader.digit(pos, 10) < 0) {
//                            lexError(savePos, "illegal.underscore");
//                        }
//                    }
                    result = l_scan_number(tokenizer, 8, 1);
                }
            } break;

            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            	result = l_scan_number(tokenizer, 10, 0);
                break;

			case '.': {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
//				if (Character.isDigit(nchar))
//				{
//					item.setLength(0);
//					item.append('.');
//					status=SCAN_READPREEXP;
//				}
//				else
//				{
				CharInfo *ci2 = l_lookahead_at(priv, 2);
				if (ci1->value=='.' && ci2->value=='.') {
					result = l_create_basic(priv, JAGP_KIND_ELLIPSIS, 3);
				} else {
					result = l_create_basic(priv, JAGP_KIND_DOT, 1);
				}
			} break;


			case '-' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);

				if (ci1->value=='-') {
					result = l_create_basic(priv, JAGP_KIND_SUBSUB, 2);
				} else if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_SUBEQ, 2);
				} else if (ci1->value=='>') {
					result = l_create_basic(priv, JAGP_KIND_ARROW, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_SUB, 1);
				}
			} break;

			case '&' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='&') {
					result = l_create_basic(priv, JAGP_KIND_AMPAMP, 2);
				} else if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_AMPEQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_AMP, 1);
				}
			} break;

			case '<' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_LTEQ, 2);
				} else if (ci1->value=='<') {
					CharInfo *ci2 = l_lookahead_at(priv, 2);
					if (ci2->value=='=') {
						result = l_create_basic(priv, JAGP_KIND_LTLTEQ, 3);
					} else {
						result = l_create_basic(priv, JAGP_KIND_LTLT , 2);
					}
				} else {
					result = l_create_basic(priv, JAGP_KIND_LT, 1);
				}
			} break;

			case '>' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_GTEQ, 2);
				} else if (ci1->value=='>') {
					CharInfo *ci2 = l_lookahead_at(priv, 2);
					if (ci2->value=='>') {
						CharInfo *ci3 = l_lookahead_at(priv, 3);
						if (ci3->value=='=') {
							result = l_create_basic(priv, JAGP_KIND_GTGTGTEQ, 4);
						} else {
							result = l_create_basic(priv, JAGP_KIND_GTGTGT, 3);
						}
					} else if (ci2->value=='=') {
						result = l_create_basic(priv, JAGP_KIND_GTGTEQ, 3);
					} else {
						result = l_create_basic(priv, JAGP_KIND_GTGT, 1);
					}
				} else {
					result = l_create_basic(priv, JAGP_KIND_GT, 1);
				}
			} break;

			case '|' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='|') {
					result = l_create_basic(priv, JAGP_KIND_BARBAR, 2);
				} else if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_BAREQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_BAR, 1);
				}
			} break;

			case '=' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_EQEQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_EQ, 1);
				}
			} break;

			case ':' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value==':') {
					result = l_create_basic(priv, JAGP_KIND_COLCOL, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_COLON, 1);
				}
			} break;

			case '!' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_BANGEQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_BANG, 1);
				}
			} break;

			case '^' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_CARETEQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_CARET, 1);
				}
			} break;

			case '*' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_STAREQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_STAR, 1);
				}
			} break;

			case '%' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_PERCENTEQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_PERCENT, 1);
				}
			} break;

			case '/' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_SLASHEQ, 2);
				} else if (ci1->value=='/') {
					result = l_scan_eol_comment(tokenizer);
				} else if (ci1->value=='*') {
					result = l_scan_full_comment(tokenizer);
				} else {
					result = l_create_basic(priv, JAGP_KIND_SLASH, 1);
				}
			} break;

			case '+' : {
				CharInfo *ci1 = l_lookahead_at(priv, 1);
				if (ci1->value=='+') {
					result = l_create_basic(priv, JAGP_KIND_PLUSPLUS, 2);
				} else if (ci1->value=='=') {
					result = l_create_basic(priv, JAGP_KIND_PLUSEQ, 2);
				} else {
					result = l_create_basic(priv, JAGP_KIND_PLUS, 1);
				}
			} break;

			case '\'' : {
				result = l_scan_char_literal(tokenizer);
			} break;

			case '"' : {
				result = l_scan_string_literal(tokenizer);
			} break;

			default : {

				if ((ci0->value>='a' && ci0->value<='z')
						|| (ci0->value>='A' && ci0->value<='Z')) {
					result = l_scan_ident(tokenizer);
					break;
				}

				l_scan_one_unichar(priv);
			} break;
		}
	}

	return result;

}

static JagPToken *l_create_basic(JagPTokenizerPrivate *priv, JagPTokenKind kind, int cnt) {
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	CharInfo start = *ci0;

	while(cnt>1) {
		cnt--;
		l_scan_one_unichar(priv);
	}
	ci0 = l_lookahead_at(priv, 0);
	CharInfo end = *ci0;
	l_scan_one_unichar(priv);
	JagPToken *result = jagp_token_new();
	result->kind = kind;
	result->cur_start = jagp_cursor_new(start.left_row, start.left_column);
	result->cur_end = jagp_cursor_new(end.right_row, end.right_column);
	result->value = NULL;
	return result;
}

static gboolean l_is_java_identifier(gunichar ch) {
	return ((ch>='a') && (ch<='z'))
			|| ((ch>='A') && (ch<='Z'))
			|| ((ch>='0') && (ch<='9'))
			|| (ch=='_');
}


static void l_apply_char(JagPTokenizerPrivate *priv, CatStringWo *e_out, const char ch, CharInfo *end, CharInfo *asend) {
	cat_string_wo_append_char(e_out, ch);
	*end=*asend;
	l_scan_one_unichar(priv);
	l_scan_one_unichar(priv);
}

typedef enum _SingleChar SingleChar;
enum _SingleChar {
	SINGLE_CHAR_NORMAL,
	SINGLE_CHAR_ESCAPED,
	SINGLE_CHAR_EOT,
	SINGLE_CHAR_ERROR
};

   /** Read next character in character or string literal and copy into sbuf.
     */
static SingleChar l_scan_single_char(JagPTokenizerPrivate *priv, CatStringWo *e_out, CharInfo *end) {
	SingleChar result = SINGLE_CHAR_NORMAL;
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	if (ci0->value=='\\') {
		result = SINGLE_CHAR_ESCAPED;
		CharInfo *ci1 = l_lookahead_at(priv, 1);
		if (ci1->value == '\\' /*&& !reader.isUnicode() */) {
			l_apply_char(priv, e_out, '\'', end, ci1);
		} else {
			switch(ci1->value) {
				case '0': case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					ci0 = l_scan_one_unichar(priv);
					char leadch = ci0->value;
					int oct = (int) (ci0->value-'0');
					*end=*ci0;
					ci0 = l_scan_one_unichar(priv);
					if (ci0->value >= '0' && ci0->value <= '7') {
						oct = oct * 8 + (int) (ci0->value-'0');
						*end=*ci0;
						ci0 = l_scan_one_unichar(priv);
						if (leadch <= '3' && (ci0->value >= '0' && ci0->value <= '7')) {
							oct = oct * 8 + (int) (ci0->value-'0');
							*end=*ci0;
							l_scan_one_unichar(priv);
						}
					}
					cat_string_wo_append_unichar(e_out, oct);
					break;
				case 'b': l_apply_char(priv, e_out, '\b', end, ci1); break;
				case 't': l_apply_char(priv, e_out, '\t', end, ci1); break;
				case 'n': l_apply_char(priv, e_out, '\n', end, ci1); break;
				case 'f': l_apply_char(priv, e_out, '\f', end, ci1); break;
				case 'r': l_apply_char(priv, e_out, '\r', end, ci1); break;
				case '\'': l_apply_char(priv, e_out, '\'', end, ci1); break;
				case '\"': l_apply_char(priv, e_out, '\"', end, ci1); break;
				case '\\': l_apply_char(priv, e_out, '\\', end, ci1); break;
				default:
					result = SINGLE_CHAR_ERROR;
			}
		}
	} else if (ci0->value==-1) {
		result = SINGLE_CHAR_EOT;
	} else {
		cat_string_wo_append_unichar(e_out, ci0->value);
		*end=*ci0;
		l_scan_one_unichar(priv);
	}
	return result;
}


static JagPToken *l_scan_char_literal(JagPTokenizer *tokenizer) {
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(tokenizer);
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	CharInfo start = *ci0;
	CharInfo end = *ci0;

	JagPToken *result = jagp_token_new();
	result->kind = JAGP_KIND_CHARLITERAL;

	ci0 = l_scan_one_unichar(priv);
	CatStringWo *buf = cat_string_wo_new();
	SingleChar scres = l_scan_single_char(priv, buf, &end);
	ci0 = l_lookahead_at(priv, 0);
	result->value = (GObject *) buf;
	switch(scres) {
		case SINGLE_CHAR_NORMAL : {
			if (end.value=='\'') {
				// TODO mark error
//                lexError(pos, "empty.char.lit");
			} else if (ci0->value!='\'') {
				// TODO mark error
//                lexError(pos, "unclosed.char.lit");
			} else {
				end = *ci0;
				l_scan_one_unichar(priv);
			}
		} break;

		case SINGLE_CHAR_EOT : {
			// TODO mark error
//                lexError(pos, "unclosed.char.lit");
		} break;
		case SINGLE_CHAR_ERROR : {
			// TODO mark error
//                lexError(pos, "illegal.esc.char");
		} break;
		case SINGLE_CHAR_ESCAPED : {
			if (ci0->value!='\'') {
				// TODO mark error
	//                lexError(pos, "unclosed.char.lit");
			} else {
				end = *ci0;
				l_scan_one_unichar(priv);
			}
		} break;

	}
	result->cur_start = jagp_cursor_new(start.left_row, start.left_column);
	result->cur_end = jagp_cursor_new(end.right_row, end.right_column);
	return result;
}



static JagPToken *l_scan_string_literal(JagPTokenizer *tokenizer) {
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(tokenizer);
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	CharInfo start = *ci0;
	CharInfo end = *ci0;

	JagPToken *result = jagp_token_new();
	result->kind = JAGP_KIND_STRINGLITERAL;

	ci0 = l_scan_one_unichar(priv);
	CatStringWo *buf = cat_string_wo_new();
	result->value = (GObject *) buf;
	gboolean keep_scanning = TRUE;
	while(keep_scanning) {
		if (ci0->value=='"') {
			keep_scanning = FALSE;
			l_scan_one_unichar(priv);
			break;
		}
		SingleChar scres = l_scan_single_char(priv, buf, &end);
		ci0 = l_lookahead_at(priv, 0);
		if (scres==SINGLE_CHAR_EOT) {
			keep_scanning = FALSE;
			//				// TODO mark error
			//	//                lexError(pos, "unclosed.string.lit");
		} else if (scres==SINGLE_CHAR_ERROR) {
			l_scan_one_unichar(priv);
			//				// TODO mark error
			//	//                lexError(pos, "illegal.esc.char");
		}

	}

//		switch(scres) {
//			case SINGLE_CHAR_NORMAL : {
//				if (ci0->value=='"') {
//					keep_scanning = FALSE;
//					l_scan_one_unichar(priv);
//				} else {
//					end = *ci0;
////					ci0 = l_scan_one_unichar(priv);
//				}
//			} break;
//
//			case SINGLE_CHAR_EOT : {
//				// TODO mark error
//	//                lexError(pos, "unclosed.string.lit");
//				keep_scanning = FALSE;
//			} break;
//			case SINGLE_CHAR_ERROR : {
//				// TODO mark error
//	//                lexError(pos, "illegal.esc.char");
////				l_scan_one_unichar(priv);
//			} break;
//			case SINGLE_CHAR_ESCAPED : {
//				end = *ci0;
////				l_scan_one_unichar(priv);
//			} break;
//		}

//	}
	CharInfo *ci_last = l_lookahead_at(priv, -1);
	result->cur_start = jagp_cursor_new(start.left_row, start.left_column);
	result->cur_end = jagp_cursor_new(ci_last->right_row, ci_last->right_column);
	return result;
}

static JagPToken *l_scan_full_comment(JagPTokenizer *tokenizer) {
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(tokenizer);
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	CharInfo start = *ci0;
	JagPToken *result = jagp_token_new();
	result->kind = JAGP_KIND_COMMENT_FULL;
	CatStringWo *e_buf = cat_string_wo_new();
	cat_string_wo_append_unichar(e_buf, ci0->value);

	CharInfo *ci1 = l_scan_one_unichar(priv);

	while(TRUE) {
		if (ci1->value==-1) {
//			jagp_token_add_error(result, "");
			break;
		} else if (ci0->value=='*' && ci1->value=='/') {
			cat_string_wo_append_unichar(e_buf, '/');
			l_scan_one_unichar(priv);
			break;
		}
		ci0=ci1;
		cat_string_wo_append_unichar(e_buf, ci0->value);
		ci1 = l_scan_one_unichar(priv);
	}

	result->value = (GObject *) cat_string_wo_anchor(e_buf, 0);
	CharInfo *ci_last = l_lookahead_at(priv, -1);
	result->cur_start = jagp_cursor_new(start.left_row, start.left_column);
	result->cur_end = jagp_cursor_new(ci_last->right_row, ci_last->right_column);
	return result;
}

static JagPToken *l_scan_eol_comment(JagPTokenizer *tokenizer) {
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(tokenizer);
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	CharInfo start = *ci0;
	JagPToken *result = jagp_token_new();
	result->kind = JAGP_KIND_COMMENT_EOL;
	CatStringWo *e_buf = cat_string_wo_new();
	cat_string_wo_append_unichar(e_buf, ci0->value);

	CharInfo *ci1 = l_scan_one_unichar(priv);

	while(TRUE) {
		if (ci1->value==-1) {
//			jagp_token_add_error(result, "");
			break;
		} else if (ci1->left_row!=ci1->right_row || ci1->left_row!=ci0->right_row) {
			l_scan_one_unichar(priv);
			break;
		}
		ci0=ci1;
		cat_string_wo_append_unichar(e_buf, ci0->value);
		ci1 = l_scan_one_unichar(priv);
	}

	result->value = (GObject *) cat_string_wo_anchor(e_buf, 0);
	CharInfo *ci_last = l_lookahead_at(priv, -1);
	result->cur_start = jagp_cursor_new(start.left_row, start.left_column);
	result->cur_end = jagp_cursor_new(ci_last->right_row, ci_last->right_column);
	return result;
}


static JagPToken *l_scan_ident(JagPTokenizer *tokenizer) {
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(tokenizer);
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	CharInfo start = *ci0;
	CharInfo *cilast = ci0;
	CatStringWo *buf = cat_string_wo_new();
	cat_string_wo_append_unichar(buf, ci0->value);
	CharInfo *cin = l_scan_one_unichar(priv);
	while(l_is_java_identifier(cin->value)) {
		cilast = cin;
		l_dump_charinfo("ident_next=", cin);
		cat_string_wo_append_unichar(buf, cin->value);
		cin = l_scan_one_unichar(priv);
	}

	l_dump_charinfo("ci-end=", cilast);

	cat_log_debug("buf=%O", buf);

	JagPToken *result = jagp_token_new();
	result->kind = JAGP_KIND_IDENTIFIER;
	char fch = cat_string_wo_char_at(buf, 0);
	const char *bufchrs = cat_string_wo_getchars(buf);
	switch(fch) {
		case 'a' : {
			if (strcmp("abstract", bufchrs)==0) {
				result->kind = JAGP_KIND_ABSTRACT;
			} else if (strcmp("assert", bufchrs)==0) {
				result->kind = JAGP_KIND_ASSERT;
			}
		} break;
		case 'b' : {
			if (strcmp("boolean", bufchrs)==0) {
				result->kind = JAGP_KIND_BOOLEAN;
			} else if (strcmp("break", bufchrs)==0) {
				result->kind = JAGP_KIND_BREAK;
			} else if (strcmp("byte", bufchrs)==0) {
				result->kind = JAGP_KIND_BYTE;
			}
		} break;
		case 'c' : {
			if (strcmp("char", bufchrs)==0) {
				result->kind = JAGP_KIND_CHAR;
			} else if (strcmp("class", bufchrs)==0) {
				result->kind = JAGP_KIND_CLASS;
			} else if (strcmp("case", bufchrs)==0) {
				result->kind = JAGP_KIND_CASE;
			} else if (strcmp("catch", bufchrs)==0) {
				result->kind = JAGP_KIND_CATCH;
			} else if (strcmp("continue", bufchrs)==0) {
				result->kind = JAGP_KIND_CONTINUE;
			}
		} break;
		case 'd' : {
			if (strcmp("double", bufchrs)==0) {
				result->kind = JAGP_KIND_DOUBLE;
			} else if (strcmp("default", bufchrs)==0) {
				result->kind = JAGP_KIND_DEFAULT;
			} else if (strcmp("do", bufchrs)==0) {
				result->kind = JAGP_KIND_DO;
			}
		} break;
		case 'e' : {
			if (strcmp("extends", bufchrs)==0) {
				result->kind = JAGP_KIND_EXTENDS;
			} else if (strcmp("else", bufchrs)==0) {
				result->kind = JAGP_KIND_ELSE;
			} else if (strcmp("enum", bufchrs)==0) {
				result->kind = JAGP_KIND_ENUM;
			}
		} break;
		case 'f' : {
			if (strcmp("float", bufchrs)==0) {
				result->kind = JAGP_KIND_FLOAT;
			} else if (strcmp("for", bufchrs)==0) {
				result->kind = JAGP_KIND_FOR;
			} else if (strcmp("final", bufchrs)==0) {
				result->kind = JAGP_KIND_FINAL;
			} else if (strcmp("finally", bufchrs)==0) {
				result->kind = JAGP_KIND_FINALLY;
			}
		} break;
		case 'i' : {
			if (strcmp("int", bufchrs)==0) {
				result->kind = JAGP_KIND_INT;
			} else if (strcmp("if", bufchrs)==0) {
				result->kind = JAGP_KIND_IF;
			} else if (strcmp("instanceof", bufchrs)==0) {
				result->kind = JAGP_KIND_INSTANCEOF;
			} else if (strcmp("import", bufchrs)==0) {
				result->kind = JAGP_KIND_IMPORT;
			} else if (strcmp("implements", bufchrs)==0) {
				result->kind = JAGP_KIND_IMPLEMENTS;
			} else if (strcmp("interface", bufchrs)==0) {
				result->kind = JAGP_KIND_INTERFACE;
			}
		} break;
		case 'l' : {
			if (strcmp("long", bufchrs)==0) {
				result->kind = JAGP_KIND_LONG;
			}
		} break;
		case 'n' : {
			if (strcmp("new", bufchrs)==0) {
				result->kind = JAGP_KIND_NEW;
			} else if (strcmp("null", bufchrs)==0) {
				result->kind = JAGP_KIND_NULL;
			} else if (strcmp("native", bufchrs)==0) {
				result->kind = JAGP_KIND_NATIVE;
			}
		} break;
		case 'p' : {
			if (strcmp("public", bufchrs)==0) {
				result->kind = JAGP_KIND_PUBLIC;
			} else if (strcmp("private", bufchrs)==0) {
				result->kind = JAGP_KIND_PRIVATE;
			} else if (strcmp("protected", bufchrs)==0) {
				result->kind = JAGP_KIND_PROTECTED;
			} else if (strcmp("package", bufchrs)==0) {
				result->kind = JAGP_KIND_PACKAGE;
			}
		} break;
		case 'r' : {
			if (strcmp("return", bufchrs)==0) {
				result->kind = JAGP_KIND_RETURN;
			}
		} break;
		case 's' : {
			if (strcmp("short", bufchrs)==0) {
				result->kind = JAGP_KIND_SHORT;
			} else if (strcmp("switch", bufchrs)==0) {
				result->kind = JAGP_KIND_SWITCH;
			} else if (strcmp("static", bufchrs)==0) {
				result->kind = JAGP_KIND_STATIC;
			} else if (strcmp("super", bufchrs)==0) {
				result->kind = JAGP_KIND_SUPER;
			} else if (strcmp("synchronized", bufchrs)==0) {
				result->kind = JAGP_KIND_SYNCHRONIZED;
			} else if (strcmp("strictfp", bufchrs)==0) {
				result->kind = JAGP_KIND_STRICTFP;
			}
		} break;
		case 't' : {
			if (strcmp("this", bufchrs)==0) {
				result->kind = JAGP_KIND_THIS;
			} else if (strcmp("try", bufchrs)==0) {
				result->kind = JAGP_KIND_TRY;
			} else if (strcmp("throw", bufchrs)==0) {
				result->kind = JAGP_KIND_THROW;
			} else if (strcmp("throws", bufchrs)==0) {
				result->kind = JAGP_KIND_THROWS;
			} else if (strcmp("transient", bufchrs)==0) {
				result->kind = JAGP_KIND_TRANSIENT;
			}
		} break;
		case 'v' : {
			if (strcmp("void", bufchrs)==0) {
				result->kind = JAGP_KIND_VOID;
			} else if (strcmp("volatile", bufchrs)==0) {
				result->kind = JAGP_KIND_VOLATILE;
			}
		} break;
		case 'w' : {
			if (strcmp("while", bufchrs)==0) {
				result->kind = JAGP_KIND_WHILE;
			}
		} break;

		default :
			break;
	}
	if (result->kind == JAGP_KIND_IDENTIFIER) {
		result->value = (GObject *) buf;
	}
	result->cur_start = jagp_cursor_new(start.left_row, start.left_column);
	result->cur_end = jagp_cursor_new(cilast->right_row, cilast->right_column);
	return result;
}


static int l_digit(JagPTokenizerPrivate *priv, gunichar uch, int base) {
	cat_log_debug("digit:ch=%d, base=%d", uch, base);
	int result = -1;
	if (uch>='0' && uch<='9') {
		result = (int) uch-'0';
	} else if (uch>='A' && uch<='Z') {
		result = 10+(int) uch-'A';
	} else if (uch>='a' && uch<='z') {
		result = 10+(int) uch-'a';
	}
	cat_log_debug("digit:result=%d", result);
	return result<base ? result : -1;
}

static void l_scan_digits(JagPTokenizerPrivate *priv, CatStringWo *e_buf, int digitRadix) {
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	l_dump_charinfo("scan_digits:", ci0);

	gunichar save_ch = 0;
	while (l_digit(priv, ci0->value,digitRadix)>=0 || ci0->value=='_') {
		save_ch = ci0->value;
		cat_string_wo_append_unichar(e_buf, save_ch);
		ci0 = l_scan_one_unichar(priv);
		l_dump_charinfo("scan_digits:next ", ci0);
	};
	//	char saveCh;
	//	int savePos;
	//	do {
	//		if (reader.ch != '_') {
	//			reader.putChar(false);
	//		} else {
	//			if (!allowUnderscoresInLiterals) {
	//				lexError(pos, "unsupported.underscore.lit", source.name);
	//				allowUnderscoresInLiterals = true;
	//			}
	//		}
	//		saveCh = reader.ch;
	//		savePos = reader.bp;
	//		reader.scanChar();
	//	} while (reader.digit(pos, digitRadix) >= 0 || reader.ch == '_');
	//	if (saveCh == '_')
	//		lexError(savePos, "illegal.underscore");

}

static JagPToken *l_scan_number(JagPTokenizer *tokenizer, int radix, int skip_cnt) {
	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(tokenizer);
	JagPTokenKind tk;
	CatStringWo *buf = cat_string_wo_new();
	CharInfo *ci0 = l_lookahead_at(priv, 0);
	CharInfo start = *ci0;
	while(skip_cnt>0 || ci0->value=='_') {
		skip_cnt--;
		cat_string_wo_append_unichar(buf, ci0->value);
		ci0 = l_scan_one_unichar(priv);
	}
	// for octal, allow base-10 digit in case it's a float literal
	int digitRadix = (radix == 8 ? 10 : radix);
	int firstDigit = l_digit(priv, ci0->value, digitRadix<10 ? 10 : digitRadix);
	gboolean seendigit = firstDigit >= 0;
	gboolean seenValidDigit = firstDigit >= 0 && firstDigit < digitRadix;
	cat_log_debug("seenValidDigit=%d, firstDigit=%d, digitRadix=%d", seenValidDigit, firstDigit, digitRadix);
	l_dump_charinfo("first:", ci0);
	if (seendigit) {
		l_scan_digits(priv, buf, digitRadix);
		ci0 = l_lookahead_at(priv, 0);
	}
//	if (radix == 16 && ci0->value == '.') {
//		scanHexFractionAndSuffix(pos, seendigit);
//	} else if (seendigit && radix == 16 && (ci0->value == 'p' || ci0->value == 'P')) {
//		scanHexExponentAndSuffix(pos);
//	} else if (digitRadix == 10 && ci0->value == '.') {
//		reader.putChar(true);
//		scanFractionAndSuffix(pos);
//	} else if (digitRadix == 10 &&
//			   (ci0->value == 'e' || ci0->value == 'E' ||
//					   ci0->value == 'f' || ci0->value == 'F' ||
//					   ci0->value == 'd' || ci0->value == 'D')) {
//		scanFractionAndSuffix(pos);
//	} else {
		if (!seenValidDigit) {
//			switch (radix) {
//				case 2:
//					lexError(pos, "invalid.binary.number");
//					break;
//				case 16:
//					lexError(pos, "invalid.hex.number");
//					break;
//			}
		}
		if (ci0->value == 'l' || ci0->value == 'L') {
			cat_string_wo_append_unichar(buf, ci0->value);
			l_scan_one_unichar(priv);
			tk = JAGP_KIND_LONGLITERAL;
		} else {
			tk = JAGP_KIND_INTLITERAL;
		}
//	}

	CharInfo *ci_last = l_lookahead_at(priv, -1);
	JagPToken *result = jagp_token_new();
	result->kind = tk;
	result->cur_start = jagp_cursor_new(start.left_row, start.left_column);
	result->cur_end = jagp_cursor_new(ci_last->right_row, ci_last->right_column);
	result->value = (GObject *) buf;
	return result;
}



// JAGP_KIND_EOF,
// JAGP_KIND_ERROR,
// JAGP_KIND_IDENTIFIER,
// JAGP_KIND_CONST,
// JAGP_KIND_GOTO,
// JAGP_KIND_INTLITERAL,
// JAGP_KIND_LONGLITERAL,
// JAGP_KIND_FLOATLITERAL,
// JAGP_KIND_DOUBLELITERAL,
// JAGP_KIND_STRINGLITERAL,
// JAGP_KIND_TRUE,
// JAGP_KIND_FALSE,
// JAGP_KIND_UNDERSCORE,
// JAGP_KIND_CUSTOM

//
//
//
//#define SCANNER_DEBUG  FALSE
//
//
//
//static boolean hexFloatsWork = hexFloatsWork();
//
//static boolean hexFloatsWork() {
//	try {
//		Float.valueOf("0x1.0p1");
//		return true;
//	} catch (NumberFormatException ex) {
//		return false;
//	}
//}
//
//
// JavaTokenizer(ScannerFactory fac, UnicodeReader reader) {
//	this.fac = fac;
//	this.log = fac.log;
//	this.tokens = fac.tokens;
//	this.source = fac.source;
//	this.reader = reader;
//	this.allowBinaryLiterals = source.allowBinaryLiterals();
//	this.allowUnderscoresInLiterals = source.allowUnderscoresInLiterals();
//}
//
///** Report an error at the given position using the provided arguments.
// */
//void lexError(int pos, String key, Object... args) {
//	log.error(pos, key, args);
//	tk = TokenKind.ERROR;
//	errPos = pos;
//}
//
///** Read next character in character or string literal and copy into sbuf.
// */
//void scanLitChar(JagPTokenizer *tokenizer, int pos) {
//	if (reader.ch == '\\') {
//		if (reader.peekChar() == '\\' && !reader.isUnicode()) {
//			reader.skipChar();
//			reader.putChar('\\', true);
//		} else {
//			reader.scanChar();
//			switch (reader.ch) {
//			case '0': case '1': case '2': case '3':
//			case '4': case '5': case '6': case '7':
//				char leadch = reader.ch;
//				int oct = reader.digit(pos, 8);
//				reader.scanChar();
//				if ('0' <= reader.ch && reader.ch <= '7') {
//					oct = oct * 8 + reader.digit(pos, 8);
//					reader.scanChar();
//					if (leadch <= '3' && '0' <= reader.ch && reader.ch <= '7') {
//						oct = oct * 8 + reader.digit(pos, 8);
//						reader.scanChar();
//					}
//				}
//				reader.putChar((char)oct);
//				break;
//			case 'b':
//				reader.putChar('\b', true); break;
//			case 't':
//				reader.putChar('\t', true); break;
//			case 'n':
//				reader.putChar('\n', true); break;
//			case 'f':
//				reader.putChar('\f', true); break;
//			case 'r':
//				reader.putChar('\r', true); break;
//			case '\'':
//				reader.putChar('\'', true); break;
//			case '\"':
//				reader.putChar('\"', true); break;
//			case '\\':
//				reader.putChar('\\', true); break;
//			default:
//				lexError(reader.bp, "illegal.esc.char");
//			}
//		}
//	} else if (reader.bp != reader.buflen) {
//		reader.putChar(true);
//	}
//}
//
//void scanDigits(JagPTokenizer *tokenizer, int pos, int digitRadix) {
//	char saveCh;
//	int savePos;
//	do {
//		if (reader.ch != '_') {
//			reader.putChar(false);
//		} else {
//			if (!allowUnderscoresInLiterals) {
//				lexError(pos, "unsupported.underscore.lit", source.name);
//				allowUnderscoresInLiterals = true;
//			}
//		}
//		saveCh = reader.ch;
//		savePos = reader.bp;
//		reader.scanChar();
//	} while (reader.digit(pos, digitRadix) >= 0 || reader.ch == '_');
//	if (saveCh == '_')
//		lexError(savePos, "illegal.underscore");
//}
//
///** Read fractional part of hexadecimal floating point number.
// */
//void scanHexExponentAndSuffix(JagPTokenizer *tokenizer, int pos) {
//	if (reader.ch == 'p' || reader.ch == 'P') {
//		reader.putChar(true);
//		skipIllegalUnderscores();
//		if (reader.ch == '+' || reader.ch == '-') {
//			reader.putChar(true);
//		}
//		skipIllegalUnderscores();
//		if (reader.digit(pos, 10) >= 0) {
//			scanDigits(pos, 10);
//			if (!hexFloatsWork)
//				lexError(pos, "unsupported.cross.fp.lit");
//		} else
//			lexError(pos, "malformed.fp.lit");
//	} else {
//		lexError(pos, "malformed.fp.lit");
//	}
//	if (reader.ch == 'f' || reader.ch == 'F') {
//		reader.putChar(true);
//		tk = TokenKind.FLOATLITERAL;
//		radix = 16;
//	} else {
//		if (reader.ch == 'd' || reader.ch == 'D') {
//			reader.putChar(true);
//		}
//		tk = TokenKind.DOUBLELITERAL;
//		radix = 16;
//	}
//}
//
///** Read fractional part of floating point number.
// */
//void scanFraction(JagPTokenizer *tokenizer, int pos) {
//	skipIllegalUnderscores();
//	if (reader.digit(pos, 10) >= 0) {
//		scanDigits(pos, 10);
//	}
//	int sp1 = reader.sp;
//	if (reader.ch == 'e' || reader.ch == 'E') {
//		reader.putChar(true);
//		skipIllegalUnderscores();
//		if (reader.ch == '+' || reader.ch == '-') {
//			reader.putChar(true);
//		}
//		skipIllegalUnderscores();
//		if (reader.digit(pos, 10) >= 0) {
//			scanDigits(pos, 10);
//			return;
//		}
//		lexError(pos, "malformed.fp.lit");
//		reader.sp = sp1;
//	}
//}
//
///** Read fractional part and 'd' or 'f' suffix of floating point number.
// */
//void scanFractionAndSuffix(JagPTokenizer *tokenizer, int pos) {
//	radix = 10;
//	scanFraction(pos);
//	if (reader.ch == 'f' || reader.ch == 'F') {
//		reader.putChar(true);
//		tk = TokenKind.FLOATLITERAL;
//	} else {
//		if (reader.ch == 'd' || reader.ch == 'D') {
//			reader.putChar(true);
//		}
//		tk = TokenKind.DOUBLELITERAL;
//	}
//}
//
///** Read fractional part and 'd' or 'f' suffix of floating point number.
// */
//void scanHexFractionAndSuffix(JagPTokenizer *tokenizer, int pos, boolean seendigit) {
//	radix = 16;
//	Assert.check(reader.ch == '.');
//	reader.putChar(true);
//	skipIllegalUnderscores();
//	if (reader.digit(pos, 16) >= 0) {
//		seendigit = true;
//		scanDigits(pos, 16);
//	}
//	if (!seendigit)
//		lexError(pos, "invalid.hex.number");
//	else
//		scanHexExponentAndSuffix(pos);
//}
//
//void skipIllegalUnderscores(JagPTokenizer *tokenizer) {
//	if (reader.ch == '_') {
//		lexError(reader.bp, "illegal.underscore");
//		while (reader.ch == '_')
//			reader.scanChar();
//	}
//}
//
///** Read a number.
// *  @param radix  The radix of the number; one of 2, 8, 10, 16.
// */
//void scanNumber(JagPTokenizer *tokenizer, int pos, int radix) {
//	// for octal, allow base-10 digit in case it's a float literal
//	this.radix = radix;
//	int digitRadix = (radix == 8 ? 10 : radix);
//	int firstDigit = reader.digit(pos, Math.max(10, digitRadix));
//	boolean seendigit = firstDigit >= 0;
//	boolean seenValidDigit = firstDigit >= 0 && firstDigit < digitRadix;
//	if (seendigit) {
//		scanDigits(pos, digitRadix);
//	}
//	if (radix == 16 && reader.ch == '.') {
//		scanHexFractionAndSuffix(pos, seendigit);
//	} else if (seendigit && radix == 16 && (reader.ch == 'p' || reader.ch == 'P')) {
//		scanHexExponentAndSuffix(pos);
//	} else if (digitRadix == 10 && reader.ch == '.') {
//		reader.putChar(true);
//		scanFractionAndSuffix(pos);
//	} else if (digitRadix == 10 &&
//			   (reader.ch == 'e' || reader.ch == 'E' ||
//				reader.ch == 'f' || reader.ch == 'F' ||
//				reader.ch == 'd' || reader.ch == 'D')) {
//		scanFractionAndSuffix(pos);
//	} else {
//		if (!seenValidDigit) {
//			switch (radix) {
//			case 2:
//				lexError(pos, "invalid.binary.number");
//				break;
//			case 16:
//				lexError(pos, "invalid.hex.number");
//				break;
//			}
//		}
//		if (reader.ch == 'l' || reader.ch == 'L') {
//			reader.scanChar();
//			tk = TokenKind.LONGLITERAL;
//		} else {
//			tk = TokenKind.INTLITERAL;
//		}
//	}
//}
//
///** Read an identifier.
// */
//void scanIdent(JagPTokenizer *tokenizer) {
//	boolean isJavaIdentifierPart;
//	char high;
//	reader.putChar(true);
//	do {
//		switch (reader.ch) {
//		case 'A': case 'B': case 'C': case 'D': case 'E':
//		case 'F': case 'G': case 'H': case 'I': case 'J':
//		case 'K': case 'L': case 'M': case 'N': case 'O':
//		case 'P': case 'Q': case 'R': case 'S': case 'T':
//		case 'U': case 'V': case 'W': case 'X': case 'Y':
//		case 'Z':
//		case 'a': case 'b': case 'c': case 'd': case 'e':
//		case 'f': case 'g': case 'h': case 'i': case 'j':
//		case 'k': case 'l': case 'm': case 'n': case 'o':
//		case 'p': case 'q': case 'r': case 's': case 't':
//		case 'u': case 'v': case 'w': case 'x': case 'y':
//		case 'z':
//		case '$': case '_':
//		case '0': case '1': case '2': case '3': case '4':
//		case '5': case '6': case '7': case '8': case '9':
//			break;
//		case '\u0000': case '\u0001': case '\u0002': case '\u0003':
//		case '\u0004': case '\u0005': case '\u0006': case '\u0007':
//		case '\u0008': case '\u000E': case '\u000F': case '\u0010':
//		case '\u0011': case '\u0012': case '\u0013': case '\u0014':
//		case '\u0015': case '\u0016': case '\u0017':
//		case '\u0018': case '\u0019': case '\u001B':
//		case '\u007F':
//			reader.scanChar();
//			continue;
//		case '\u001A': // EOI is also a legal identifier part
//			if (reader.bp >= reader.buflen) {
//				name = reader.name();
//				tk = tokens.lookupKind(name);
//				return;
//			}
//			reader.scanChar();
//			continue;
//		default:
//			if (reader.ch < '\u0080') {
//				// all ASCII range chars already handled, above
//				isJavaIdentifierPart = false;
//			} else {
//				if (Character.isIdentifierIgnorable(reader.ch)) {
//					reader.scanChar();
//					continue;
//				} else {
//					int codePoint = reader.peekSurrogates();
//					if (codePoint >= 0) {
//						if (isJavaIdentifierPart = Character.isJavaIdentifierPart(codePoint)) {
//							reader.putChar(true);
//						}
//					} else {
//						isJavaIdentifierPart = Character.isJavaIdentifierPart(reader.ch);
//					}
//				}
//			}
//			if (!isJavaIdentifierPart) {
//				name = reader.name();
//				tk = tokens.lookupKind(name);
//				return;
//			}
//		}
//		reader.putChar(true);
//	} while (true);
//}
//
///** Return true if reader.ch can be part of an operator.
// */
//gboolean isSpecial(JagPTokenizer *tokenizer, char ch) {
//	switch (ch) {
//	case '!': case '%': case '&': case '*': case '?':
//	case '+': case '-': case ':': case '<': case '=':
//	case '>': case '^': case '|': case '~':
//	case '@':
//		return true;
//	default:
//		return false;
//	}
//}
//
///** Read longest possible sequence of special characters and convert
// *  to token.
// */
//void scanOperator(JagPTokenizer *tokenizer) {
//	while (true) {
//		reader.putChar(false);
//		Name newname = reader.name();
//		TokenKind tk1 = tokens.lookupKind(newname);
//		if (tk1 == TokenKind.IDENTIFIER) {
//			reader.sp--;
//			break;
//		}
//		tk = tk1;
//		reader.scanChar();
//		if (!isSpecial(reader.ch)) break;
//	}
//}
//
///** Read token.
// */
//Token readToken(JagPTokenizer *tokenizer) {
//
//	reader.sp = 0;
//	name = null;
//	radix = 0;
//
//	int pos = 0;
//	int endPos = 0;
//	List<Comment> comments = null;
//
//	try {
//		loop: while (true) {
//			pos = reader.bp;
//			switch (reader.ch) {
//			case ' ': // (Spec 3.6)
//			case '\t': // (Spec 3.6)
//			case FF: // (Spec 3.6)
//				do {
//					reader.scanChar();
//				} while (reader.ch == ' ' || reader.ch == '\t' || reader.ch == FF);
//				processWhiteSpace(pos, reader.bp);
//				break;
//			case LF: // (Spec 3.4)
//				reader.scanChar();
//				processLineTerminator(pos, reader.bp);
//				break;
//			case CR: // (Spec 3.4)
//				reader.scanChar();
//				if (reader.ch == LF) {
//					reader.scanChar();
//				}
//				processLineTerminator(pos, reader.bp);
//				break;
//			case 'A': case 'B': case 'C': case 'D': case 'E':
//			case 'F': case 'G': case 'H': case 'I': case 'J':
//			case 'K': case 'L': case 'M': case 'N': case 'O':
//			case 'P': case 'Q': case 'R': case 'S': case 'T':
//			case 'U': case 'V': case 'W': case 'X': case 'Y':
//			case 'Z':
//			case 'a': case 'b': case 'c': case 'd': case 'e':
//			case 'f': case 'g': case 'h': case 'i': case 'j':
//			case 'k': case 'l': case 'm': case 'n': case 'o':
//			case 'p': case 'q': case 'r': case 's': case 't':
//			case 'u': case 'v': case 'w': case 'x': case 'y':
//			case 'z':
//			case '$': case '_':
//				scanIdent();
//				break loop;
//			case '0':
//				reader.scanChar();
//				if (reader.ch == 'x' || reader.ch == 'X') {
//					reader.scanChar();
//					skipIllegalUnderscores();
//					scanNumber(pos, 16);
//				} else if (reader.ch == 'b' || reader.ch == 'B') {
//					if (!allowBinaryLiterals) {
//						lexError(pos, "unsupported.binary.lit", source.name);
//						allowBinaryLiterals = true;
//					}
//					reader.scanChar();
//					skipIllegalUnderscores();
//					scanNumber(pos, 2);
//				} else {
//					reader.putChar('0');
//					if (reader.ch == '_') {
//						int savePos = reader.bp;
//						do {
//							reader.scanChar();
//						} while (reader.ch == '_');
//						if (reader.digit(pos, 10) < 0) {
//							lexError(savePos, "illegal.underscore");
//						}
//					}
//					scanNumber(pos, 8);
//				}
//				break loop;
//			case '1': case '2': case '3': case '4':
//			case '5': case '6': case '7': case '8': case '9':
//				scanNumber(pos, 10);
//				break loop;
//			case '.':
//				reader.scanChar();
//				if (reader.digit(pos, 10) >= 0) {
//					reader.putChar('.');
//					scanFractionAndSuffix(pos);
//				} else if (reader.ch == '.') {
//					int savePos = reader.bp;
//					reader.putChar('.'); reader.putChar('.', true);
//					if (reader.ch == '.') {
//						reader.scanChar();
//						reader.putChar('.');
//						tk = TokenKind.ELLIPSIS;
//					} else {
//						lexError(savePos, "illegal.dot");
//					}
//				} else {
//					tk = TokenKind.DOT;
//				}
//				break loop;
//			case ',':
//				reader.scanChar(); tk = TokenKind.COMMA; break loop;
//			case ';':
//				reader.scanChar(); tk = TokenKind.SEMI; break loop;
//			case '(':
//				reader.scanChar(); tk = TokenKind.LPAREN; break loop;
//			case ')':
//				reader.scanChar(); tk = TokenKind.RPAREN; break loop;
//			case '[':
//				reader.scanChar(); tk = TokenKind.LBRACKET; break loop;
//			case ']':
//				reader.scanChar(); tk = TokenKind.RBRACKET; break loop;
//			case '{':
//				reader.scanChar(); tk = TokenKind.LBRACE; break loop;
//			case '}':
//				reader.scanChar(); tk = TokenKind.RBRACE; break loop;
//			case '/':
//				reader.scanChar();
//				if (reader.ch == '/') {
//					do {
//						reader.scanCommentChar();
//					} while (reader.ch != CR && reader.ch != LF && reader.bp < reader.buflen);
//					if (reader.bp < reader.buflen) {
//						comments = addComment(comments, processComment(pos, reader.bp, CommentStyle.LINE));
//					}
//					break;
//				} else if (reader.ch == '*') {
//					boolean isEmpty = false;
//					reader.scanChar();
//					CommentStyle style;
//					if (reader.ch == '*') {
//						style = CommentStyle.JAVADOC;
//						reader.scanCommentChar();
//						if (reader.ch == '/') {
//							isEmpty = true;
//						}
//					} else {
//						style = CommentStyle.BLOCK;
//					}
//					while (!isEmpty && reader.bp < reader.buflen) {
//						if (reader.ch == '*') {
//							reader.scanChar();
//							if (reader.ch == '/') break;
//						} else {
//							reader.scanCommentChar();
//						}
//					}
//					if (reader.ch == '/') {
//						reader.scanChar();
//						comments = addComment(comments, processComment(pos, reader.bp, style));
//						break;
//					} else {
//						lexError(pos, "unclosed.comment");
//						break loop;
//					}
//				} else if (reader.ch == '=') {
//					tk = TokenKind.SLASHEQ;
//					reader.scanChar();
//				} else {
//					tk = TokenKind.SLASH;
//				}
//				break loop;
//			case '\'':
//				reader.scanChar();
//				if (reader.ch == '\'') {
//					lexError(pos, "empty.char.lit");
//					reader.scanChar();
//				} else {
//					if (reader.ch == CR || reader.ch == LF)
//						lexError(pos, "illegal.line.end.in.char.lit");
//					scanLitChar(pos);
//					if (reader.ch == '\'') {
//						reader.scanChar();
//						tk = TokenKind.CHARLITERAL;
//					} else {
//						lexError(pos, "unclosed.char.lit");
//					}
//				}
//				break loop;
//			case '\"':
//				reader.scanChar();
//				while (reader.ch != '\"' && reader.ch != CR && reader.ch != LF && reader.bp < reader.buflen)
//					scanLitChar(pos);
//				if (reader.ch == '\"') {
//					tk = TokenKind.STRINGLITERAL;
//					reader.scanChar();
//				} else {
//					lexError(pos, "unclosed.str.lit");
//				}
//				break loop;
//			default:
//				if (isSpecial(reader.ch)) {
//					scanOperator();
//				} else {
//					boolean isJavaIdentifierStart;
//					int codePoint = -1;
//					if (reader.ch < '\u0080') {
//						// all ASCII range chars already handled, above
//						isJavaIdentifierStart = false;
//					} else {
//						codePoint = reader.peekSurrogates();
//						if (codePoint >= 0) {
//							if (isJavaIdentifierStart = Character.isJavaIdentifierStart(codePoint)) {
//								reader.putChar(true);
//							}
//						} else {
//							isJavaIdentifierStart = Character.isJavaIdentifierStart(reader.ch);
//						}
//					}
//					if (isJavaIdentifierStart) {
//						scanIdent();
//					} else if (reader.digit(pos, 10) >= 0) {
//						scanNumber(pos, 10);
//					} else if (reader.bp == reader.buflen || reader.ch == EOI && reader.bp + 1 == reader.buflen) { // JLS 3.5
//						tk = TokenKind.EOF;
//						pos = reader.buflen;
//					} else {
//						String arg;
//
//						if (codePoint >= 0) {
//							char high = reader.ch;
//							reader.scanChar();
//							arg = String.format("\\u%04x\\u%04x", (int) high, (int)reader.ch);
//						} else {
//							arg = (32 < reader.ch && reader.ch < 127) ?
//											String.format("%s", reader.ch) :
//											String.format("\\u%04x", (int)reader.ch);
//						}
//						lexError(pos, "illegal.char", arg);
//						reader.scanChar();
//					}
//				}
//				break loop;
//			}
//		}
//		endPos = reader.bp;
//		switch (tk.tag) {
//			case DEFAULT: return new Token(tk, pos, endPos, comments);
//			case NAMED: return new NamedToken(tk, pos, endPos, name, comments);
//			case STRING: return new StringToken(tk, pos, endPos, reader.chars(), comments);
//			case NUMERIC: return new NumericToken(tk, pos, endPos, reader.chars(), radix, comments);
//			default: throw new AssertionError();
//		}
//	}
//	finally {
//		if (SCANNER_DEBUG) {
//				System.out.println("nextToken(" + pos
//								   + "," + endPos + ")=|" +
//								   new String(reader.getRawCharacters(pos, endPos))
//								   + "|");
//		}
//	}
//}
////where
//	List<Comment> addComment(List<Comment> comments, Comment comment) {
//		return comments == null ?
//				List.of(comment) :
//				comments.prepend(comment);
//	}
//
///** Return the position where a lexical error occurred;
// */
//int errPos(JagPTokenizer *tokenizer) {
//	return errPos;
//}
//
///** Set the position where a lexical error occurred;
// */
//void errPos(JagPTokenizer *tokenizer, int pos) {
//	errPos = pos;
//}
//
///**
// * Called when a complete comment has been scanned. pos and endPos
// * will mark the comment boundary.
// */
//Tokens.Comment processComment(JagPTokenizer *tokenizer, int pos, int endPos, CommentStyle style) {
//	if (SCANNER_DEBUG)
//		System.out.println("processComment(" + pos
//						   + "," + endPos + "," + style + ")=|"
//						   + new String(reader.getRawCharacters(pos, endPos))
//						   + "|");
//	char[] buf = reader.getRawCharacters(pos, endPos);
//	return new BasicComment<>(new UnicodeReader(fac, buf, buf.length), style);
//}
//
///**
// * Called when a complete whitespace run has been scanned. pos and endPos
// * will mark the whitespace boundary.
// */
//void processWhiteSpace(JagPTokenizer *tokenizer, int pos, int endPos) {
//	if (SCANNER_DEBUG)
//		System.out.println("processWhitespace(" + pos
//						   + "," + endPos + ")=|" +
//						   new String(reader.getRawCharacters(pos, endPos))
//						   + "|");
//}
//
///**
// * Called when a line terminator has been processed.
// */
//void processLineTerminator(JagPTokenizer *tokenizer, int pos, int endPos) {
//	if (SCANNER_DEBUG)
//		System.out.println("processTerminator(" + pos
//						   + "," + endPos + ")=|" +
//						   new String(reader.getRawCharacters(pos, endPos))
//						   + "|");
//}
//
///** Build a map for translating between line numbers and
// * positions in the input.
// *
// * @return a LineMap */
//Position.LineMap getLineMap(JagPTokenizer *tokenizer) {
//	return Position.makeLineMap(reader.getRawCharacters(), reader.buflen, false);
//}
//
//
/////**
////* Scan a documentation comment; determine if a deprecated tag is present.
////* Called once the initial /, * have been skipped, positioned at the second *
////* (which is treated as the beginning of the first line).
////* Stops positioned at the closing '/'.
////*/
////static class BasicComment<U extends UnicodeReader> implements Comment {
////
////	CommentStyle cs;
////	U comment_reader;
////
////	boolean deprecatedFlag = false;
////	boolean scanned = false;
////
////	BasicComment(U comment_reader, CommentStyle cs) {
////		this.comment_reader = comment_reader;
////		this.cs = cs;
////	}
////
////	String getText() {
////		return null;
////	}
////
////	int getSourcePos(int pos) {
////		return -1;
////	}
////
////	CommentStyle getStyle() {
////		return cs;
////	}
////
////	boolean isDeprecated() {
////		if (!scanned && cs == CommentStyle.JAVADOC) {
////			scanDocComment();
////		}
////		return deprecatedFlag;
////	}
////
////	@SuppressWarnings("fallthrough")
////	void scanDocComment() {
////		try {
////			boolean deprecatedPrefix = false;
////
////			comment_reader.bp += 3; // '/**'
////			comment_reader.ch = comment_reader.buf[comment_reader.bp];
////
////			forEachLine:
////			while (comment_reader.bp < comment_reader.buflen) {
////
////				// Skip optional WhiteSpace at beginning of line
////				while (comment_reader.bp < comment_reader.buflen && (comment_reader.ch == ' ' || comment_reader.ch == '\t' || comment_reader.ch == FF)) {
////					comment_reader.scanCommentChar();
////				}
////
////				// Skip optional consecutive Stars
////				while (comment_reader.bp < comment_reader.buflen && comment_reader.ch == '*') {
////					comment_reader.scanCommentChar();
////					if (comment_reader.ch == '/') {
////						return;
////					}
////				}
////
////				// Skip optional WhiteSpace after Stars
////				while (comment_reader.bp < comment_reader.buflen && (comment_reader.ch == ' ' || comment_reader.ch == '\t' || comment_reader.ch == FF)) {
////					comment_reader.scanCommentChar();
////				}
////
////				deprecatedPrefix = false;
////				// At beginning of line in the JavaDoc sense.
////				if (!deprecatedFlag) {
////					String deprecated = "@deprecated";
////					int i = 0;
////					while (comment_reader.bp < comment_reader.buflen && comment_reader.ch == deprecated.charAt(i)) {
////						comment_reader.scanCommentChar();
////						i++;
////						if (i == deprecated.length()) {
////							deprecatedPrefix = true;
////							break;
////						}
////					}
////				}
////
////				if (deprecatedPrefix && comment_reader.bp < comment_reader.buflen) {
////					if (Character.isWhitespace(comment_reader.ch)) {
////						deprecatedFlag = true;
////					} else if (comment_reader.ch == '*') {
////						comment_reader.scanCommentChar();
////						if (comment_reader.ch == '/') {
////							deprecatedFlag = true;
////							return;
////						}
////					}
////				}
////
////				// Skip rest of line
////				while (comment_reader.bp < comment_reader.buflen) {
////					switch (comment_reader.ch) {
////						case '*':
////							comment_reader.scanCommentChar();
////							if (comment_reader.ch == '/') {
////								return;
////							}
////							break;
////						case CR: // (Spec 3.4)
////							comment_reader.scanCommentChar();
////							if (comment_reader.ch != LF) {
////								continue forEachLine;
////							}
////						/* fall through to LF case */
////						case LF: // (Spec 3.4)
////							comment_reader.scanCommentChar();
////							continue forEachLine;
////						default:
////							comment_reader.scanCommentChar();
////					}
////				} // rest of line
////			} // forEachLine
////			return;
////		} finally {
////			scanned = true;
////		}
////	}
////}
////




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	JagPTokenizer *instance = JAGP_TOKENIZER(self);
//	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

