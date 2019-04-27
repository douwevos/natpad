/*
   File:    shoscanner.c
   Project: shoveler
   Author:  Douwe Vos
   Date:    May 7, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "shoscanner.h"
#include "generated/runtime/shoiscanner.h"
#include "generated/shosymbol.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoScanner"
#include <logging/catlog.h>

struct _ShoScannerPrivate {
	CatIInputStream *stream_in;

	int last_char;
	int column, row;
	int mark_column, mark_row;
};

static void l_scanner_iface_init(ShoIScannerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ShoScanner, sho_scanner, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(ShoScanner)
		G_IMPLEMENT_INTERFACE(SHO_TYPE_ISCANNER, l_scanner_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_advance(ShoScanner *scanner);

static void sho_scanner_class_init(ShoScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_scanner_init(ShoScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoScanner *instance = SHO_SCANNER(object);
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(instance);
	cat_unref_ptr(priv->stream_in);
	G_OBJECT_CLASS(sho_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoScanner *sho_scanner_new(CatIInputStream *stream_in) {
	ShoScanner *result = g_object_new(SHO_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(result);
	priv->stream_in = cat_ref_ptr(stream_in);
	priv->column = -1;
	priv->row = 0;
	priv->last_char = 0;
	l_advance(result);
	return result;
}

ShoToken *l_scanner_create_token(ShoIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	return sho_token_new_full_ext(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
}

static void l_advance(ShoScanner *scanner) {
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(scanner);
	if ((priv->last_char=='\r') || (priv->last_char=='\n')) {
		priv->column = -1;
		priv->row++;
	}
	priv->last_char = cat_iinput_stream_read(priv->stream_in);
	priv->column++;
}


static ShoToken * l_read_string(ShoScanner *scanner) {
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(scanner);
	char mark = priv->last_char;
	gboolean escaped = FALSE;
	l_advance(scanner);
	ShoToken *result = NULL;

	CatStringWo *e_buf = cat_string_wo_new();
	while(TRUE) {
		if (priv->last_char==mark) {
			if (!escaped) {
				l_advance(scanner);
				e_buf = cat_string_wo_anchor(e_buf, 0);
				result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_STRING, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
				break;
			}
		} else if (priv->last_char=='\\') {
			if (!escaped) {
				l_advance(scanner);
				escaped = TRUE;
				continue;
			}
		} else if (priv->last_char==-1) {
			e_buf = cat_string_wo_anchor(e_buf, 0);
			result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_STRING, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
			break;
		}
		if (escaped) {
			switch(priv->last_char) {
				case 'e' : { cat_string_wo_append_char(e_buf, (char) 27); } break;
				case '\\' : { cat_string_wo_append_char(e_buf, (char) '\\'); } break;
				case 'n' : { cat_string_wo_append_char(e_buf, (char) '\n'); } break;
				case 'r' : { cat_string_wo_append_char(e_buf, (char) '\r'); } break;
				case 'f' : { cat_string_wo_append_char(e_buf, (char) '\f'); } break;
				case 't' : { cat_string_wo_append_char(e_buf, (char) '\t'); } break;
				case 'v' : { cat_string_wo_append_char(e_buf, (char) '\v'); } break;
				case 'a' : { cat_string_wo_append_char(e_buf, (char) 7); } break;
				case 'b' : { cat_string_wo_append_char(e_buf, (char) 8); } break;
				case '0' : { cat_string_wo_append_char(e_buf, (char) 0); } break;
				default : {
					cat_string_wo_append_char(e_buf, (char) '\\');
					cat_string_wo_append_char(e_buf, (char) priv->last_char);
				}
			}
		} else {
			cat_string_wo_append_char(e_buf, (char) priv->last_char);
		}
		l_advance(scanner);
		escaped = FALSE;
	}
	cat_unref_ptr(e_buf);
	return result;
}




static ShoToken *l_read_asign_value_new_line(ShoScanner *scanner) {
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(scanner);
	l_advance(scanner);
	CatStringWo *e_buf = cat_string_wo_new();
	gboolean keep_running = TRUE;
	while(keep_running) {
		switch(priv->last_char) {
			case -1 :
			case '\n' :
			case '\r' : {
				keep_running = FALSE;
			} break;
			default : {
				cat_string_wo_append_char(e_buf, (char) priv->last_char);
				l_advance(scanner);
			} break;
		}
	}
	e_buf = cat_string_wo_anchor(e_buf, 0);
	ShoToken *result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_ASSIGN_VALUE_EOL, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
	cat_unref_ptr(e_buf);
	l_advance(scanner);
	return result;
}



static gboolean l_read_digits(ShoScanner *scanner, CatStringWo *e_buf) {
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(scanner);
	gboolean result = FALSE;
	while(TRUE) {
		if ((priv->last_char>='0') && (priv->last_char<='9')) {
			cat_string_wo_append_char(e_buf, (char) priv->last_char);
			result = TRUE;
		} else {
			break;
		}
		l_advance(scanner);
	}
	return result;
}

enum NumberScanState {
	DIGITS_DOT,
	DIGITS_EXP,
	DIGITS_DOT_OPTDIGITS_EXP,
};

static ShoToken *l_read_number(ShoScanner *scanner) {
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(scanner);
	CatStringWo *e_buf = cat_string_wo_new();
	l_read_digits(scanner, e_buf);
	enum NumberScanState state;
	switch(priv->last_char) {
		case '.' : state = DIGITS_DOT; break;
		case 'E' :
		case 'e' : state = DIGITS_EXP; break;
		default : {
			e_buf = cat_string_wo_anchor(e_buf, 0);
			ShoToken *result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_NUMBER, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
			cat_unref_ptr(e_buf);
			return result;

		}
	}
	cat_string_wo_append_char(e_buf, (char) priv->last_char);
	l_advance(scanner);


	if (state==DIGITS_DOT) {
		l_read_digits(scanner, e_buf);
		if (priv->last_char=='e' || priv->last_char=='E') {
			cat_string_wo_append_char(e_buf, (char) priv->last_char);
			state = DIGITS_DOT_OPTDIGITS_EXP;
			l_advance(scanner);
		} else {
			e_buf = cat_string_wo_anchor(e_buf, 0);
			ShoToken *result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_NUMBER, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
			cat_unref_ptr(e_buf);
			return result;
		}
	}


	if (priv->last_char=='-') {
		cat_string_wo_append_char(e_buf, (char) priv->last_char);
		l_advance(scanner);
	}
	l_read_digits(scanner, e_buf);
	e_buf = cat_string_wo_anchor(e_buf, 0);
	ShoToken *result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_NUMBER, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
	cat_unref_ptr(e_buf);
	return result;
}


static CatS sho_s_nil = CAT_S_DEF("nil");

static ShoToken *l_read_identifier(ShoScanner *scanner) {
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(scanner);
	CatStringWo *e_buf = cat_string_wo_new();
	cat_string_wo_append_char(e_buf, (char) priv->last_char);
	l_advance(scanner);
	while(TRUE) {
		int lch = priv->last_char;
		if ((lch>='a' && lch<='z')
				|| (lch>='A' && lch<='Z')
				|| (lch>='0' && lch<='9')
				|| lch=='_' || lch=='.' || lch=='-') {
			cat_string_wo_append_char(e_buf, (char) priv->last_char);
		} else {
			break;
		}
		l_advance(scanner);
	}

	ShoToken *result = NULL;
	e_buf = cat_string_wo_anchor(e_buf, 0);
	if (cat_string_wo_equal(CAT_S(sho_s_nil), e_buf)) {
		result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_NIL, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
	} else {
		result = l_scanner_create_token((ShoIScanner *) scanner, SHO_SYMBOL_TERM_ID, -1, TRUE, FALSE, FALSE, priv->mark_column, priv->mark_row, priv->column, priv->row, (GObject *) e_buf);
	}
	cat_unref_ptr(e_buf);
	return result;
}



ShoToken *l_scanner_next_token(ShoIScanner *self) {
	ShoScanner *scanner = SHO_SCANNER(self);
	ShoScannerPrivate *priv = sho_scanner_get_instance_private(scanner);

	ShoToken *result = NULL;

	while(result==NULL) {

		priv->mark_column = priv->column;
		priv->mark_row = priv->row;

		switch(priv->last_char) {
			case -1 : result = l_scanner_create_token(self, SHO_SYMBOL_TERM_EOF, -1, TRUE, FALSE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL); break;
			case '{' : l_advance(scanner); result = l_scanner_create_token(self, SHO_SYMBOL_TERM_LBRACE, -1, TRUE, FALSE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL); break;
			case '}' : l_advance(scanner); result = l_scanner_create_token(self, SHO_SYMBOL_TERM_RBRACE, -1, TRUE, FALSE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL); break;
			case '(' : l_advance(scanner); result = l_scanner_create_token(self, SHO_SYMBOL_TERM_LPARREN, -1, TRUE, FALSE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL); break;
			case ')' : l_advance(scanner); result = l_scanner_create_token(self, SHO_SYMBOL_TERM_RPARREN, -1, TRUE, FALSE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL); break;
			case ';' : l_advance(scanner); result = l_scanner_create_token(self, SHO_SYMBOL_TERM_SEMICOLON, -1, TRUE, FALSE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL); break;
			case ',' : l_advance(scanner); result = l_scanner_create_token(self, SHO_SYMBOL_TERM_COMMA, -1, TRUE, FALSE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL); break;
			case '"' :
			case '\'' : result = l_read_string(scanner); break;
			case '=' : result = l_read_asign_value_new_line(scanner); break;

			case '\r' :
			case '\n' :
			case '\t' :
			case ' ' : {
				l_advance(scanner);
			} break;

			case '0' : case '1' : case '2' : case '3' : case '4' :
			case '5' : case '6' : case '7' : case '8' : case '9' : {
				result = l_read_number(scanner);
			} break;

			default : {
				if ((priv->last_char>='a' && priv->last_char>='z') ||
						(priv->last_char>='A' && priv->last_char>='Z') || priv->last_char=='_') {
					result = l_read_identifier(scanner);
					break;
				}
				result = l_scanner_create_token(self, SHO_SYMBOL_TERM_ERROR, -1, TRUE, TRUE,FALSE,priv->mark_column, priv->mark_row, priv->column, priv->row, NULL);;
				break;
			}
		}
	}


	cat_log_on_detail({
		if (result) {
			cat_log_print("DUMP", "sym=%d, value=%o", result->sym, result->value);
		}
	});

	return result;
}

static void l_scanner_iface_init(ShoIScannerInterface *iface) {
	iface->createToken = l_scanner_create_token;
	iface->next_token = l_scanner_next_token;
}
