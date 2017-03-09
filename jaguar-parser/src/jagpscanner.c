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

static GroRunIToken *l_next_token(JagPScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(result==NULL) {
		base_class->markLocation((GroRunScannerBase *) scanner);
//		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_END_OF_INPUT, -1, -1, NULL);
				break;

			case '0' : case '1': case '2': case '3': case '4':
			case '5' : case '6': case '7': case '8': case '9': {
				result = l_scan_number(scanner);
				break;

			case '"' :
				result = base_class->scanForQuotedString(scanner, JAGP_SYM_STRING_LITERAL);
				break;

			case '\'' :
				result = l_scan_read_char_literal(scanner);
				break;

			case '<' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					result = _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LE, scanner->mark, scanner->mark_row, scanner->column+1, row);
					_next_char(scanner);
					return result;
				} else if (scanner->lastChar=='<') {
					_next_char(scanner);
					if (scanner->lastChar=='=') {
						result = _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SHL, scanner->mark, scanner->mark_row, scanner->column+1, row);
						_next_char(scanner);
						return result;
					} else {
						return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SHL, scanner->mark, scanner->mark_row, scanner->column, row);
					}
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LT, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}



			case '>' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_GE, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='>') {
					_next_char(scanner);
					if (scanner->lastChar=='>') {
						_next_char(scanner);
						if (scanner->lastChar=='=') {
							_next_char(scanner);
							return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SHRR, scanner->mark, scanner->mark_row, scanner->column, row);
						} else {
							return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SHRR, scanner->mark, scanner->mark_row, scanner->column, row);
						}
					} else if (scanner->lastChar=='=') {
						_next_char(scanner);
						return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SHR, scanner->mark, scanner->mark_row, scanner->column, row);
					} else {
						return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SHR, scanner->mark, scanner->mark_row, scanner->column, row);
					}
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_GT, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}


			case '=' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_EQ, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_IS, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '!' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_NE, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_NOT, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '^' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_XOR, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_XOR, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '%' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_MOD, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_MOD, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}


			case '|' : {
				_next_char(scanner);
				if (scanner->lastChar=='|') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LOR, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_OR, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_OR, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '&' : {
				_next_char(scanner);
				if (scanner->lastChar=='&') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_LAND, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_AND, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_AND, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '+' : {
				_next_char(scanner);
				if (scanner->lastChar=='+') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_INC, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_ADD, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_ADD, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '-' : {
				_next_char(scanner);
				if (scanner->lastChar=='-') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_DEC, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_SUB, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_SUB, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '*' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_MUL, scanner->mark, scanner->mark_row, scanner->column, row);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_MUL, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}

			case '/' : {
				_next_char(scanner);
				if (scanner->lastChar=='=') {
					_next_char(scanner);
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_ASS_DIV, scanner->mark, scanner->mark_row, scanner->column, row);
				} else if (scanner->lastChar=='/') {
					return _scan_eol_comment(scanner);
					break;
				} else if (scanner->lastChar=='*') {
					scanner->inFullComment = TRUE;
					_next_char(scanner);
					return _scan_full_comment(scanner);
				} else {
					return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_DIV, scanner->mark, scanner->mark_row, scanner->column, row);
				}
			}




			case '~': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_OP_NEG, scanner->mark, scanner->mark_row, scanner->column, row);

			case ';': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_SEMICOLON, scanner->mark, scanner->mark_row, scanner->column, row);
			case '(': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_LPAREN, scanner->mark, scanner->mark_row, scanner->column, row);
			case ')': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_RPAREN, scanner->mark, scanner->mark_row, scanner->column, row);
			case '?': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_QUESTION, scanner->mark, scanner->mark_row, scanner->column, row);
			case '{': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_LBRACE, scanner->mark, scanner->mark_row, scanner->column, row);
			case '}': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_RBRACE, scanner->mark, scanner->mark_row, scanner->column, row);
			case '[':
			{
				_next_char(scanner);
				return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_LBRACK, scanner->mark, scanner->mark_row, scanner->column, row);
			}
			case ']': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_RBRACK, scanner->mark, scanner->mark_row, scanner->column, row);
			case '@': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_AT, scanner->mark, scanner->mark_row, scanner->column, row);
			case ':': _next_char(scanner); return _create_token_with_pos(scanner, JAG_SYMBOL_TERM_COLON, scanner->mark, scanner->mark_row, scanner->column, row);

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
			return _scan_hex_octal(scanner);
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

	int row = base_class->getLeftRow((GroRunScannerBase *) scanner);
	int end = base_class->getLeftColumn((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	GroRunIToken *result = base_class->createToken((GroRunScannerBase *) scanner, JAGP_SYM_CHARACTER_LITERAL, row, end, NULL);
	if (!ok) {
		grorun_itoken_set_state(result, GRORUN_STATE_SCANNER_ERROR);
	}
	return result;
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
