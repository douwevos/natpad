/*
   File:    muleifscanner.c
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

#include "muleifscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulEifScanner"
#include <logging/catlog.h>

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulEifScanner, mul_eif_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_eif_scanner_class_init(MulEifScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

}

static void mul_eif_scanner_init(MulEifScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(mul_eif_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_eif_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulEifScanner *mul_eif_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	MulEifScanner *result = g_object_new(MUL_EIF_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 5);
	return result;
}


gboolean mul_eif_scanner_is_eof(MulEifScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, MUL_EIF_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}






static GroRunIToken *l_scan_for_manifest_string(MulEifScanner *scanner);


static GroRunIToken *l_next_token(MulEifScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(result==NULL) {


		base_class->markLocation((GroRunScannerBase *) scanner);
		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, MUL_EIF_SYM_END_OF_INPUT, -1, -1, NULL);
				break;


			case '(':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_LPAREN, 1);
				break;

			case ')':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_RPAREN, 1);
				break;

			case ',':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_COMMA, 1);
				break;

			case ';':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_SEMICOLON, 1);
				break;

			case ':':
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_COLON_IS, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_COLON, 1);
				}
				break;

			case '.':
				if (lookahead[1]=='.') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_DOUBLE_DOT, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_DOT, 1);
				}
				break;

			case '-' : {
				if (lookahead[1]=='>') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_ARROW, 2);
				} else if (lookahead[1]=='-') {
					result = base_class->scanForEndOfLine((GroRunScannerBase *) scanner, MUL_EIF_SYM_COMMENT, TRUE);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_OP_MINUS, 1);
				}
			} break;

			case '+' : {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_OP_PLUS, 1);
			} break;

			case '!':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_EXCLAMATION, 1);
				break;

			case '?' : {
				if (lookahead[1]=='=') {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_QUESTION_MARK_IS, 2);
				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_QUESTION_MARK, 1);
				}
			} break;

			case '$':
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_DOLLAR, 1);
				break;


			case '=': {
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, MUL_EIF_SYM_OP_IS, 1);
			} break;

			case '"' :
				result = l_scan_for_manifest_string(scanner);
				break;

			default: {
				if (g_unichar_isalpha(lookahead[0])) {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, MUL_EIF_SYM_IDENTIFIER, TRUE);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, 0);
				}
			} break;



		}
	}
	return result;
}

#define CREATE_VALUES FALSE

static GroRunIToken *l_scan_for_manifest_string(MulEifScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	base_class->markLocation((GroRunScannerBase *) scanner);

	gunichar mark_char = lookahead[0];
	CatStringWo *buf = NULL;
	if (CREATE_VALUES) {
		buf = cat_string_wo_new();
		cat_string_wo_append_unichar(buf, mark_char);
	}

	CatStringWo *mark_stack = cat_string_wo_new();

	GroRunIToken *result = NULL;
	while(TRUE) {
		gunichar lch = lookahead[1];
		if (lch==mark_char) {
			int mstop = cat_string_wo_length(mark_stack);
			if (mstop==0) {
				base_class->advance((GroRunScannerBase *) scanner, 0);
				break;
			}
			mark_char = cat_string_wo_char_at(mark_stack, mstop-1);
			cat_string_wo_remove_range(mark_stack, mstop-1, 1);
		} else if (lch=='[') {
			cat_string_wo_append_char(mark_stack, mark_char);
			mark_char = ']';
		} else if (lch=='{') {
			cat_string_wo_append_char(mark_stack, mark_char);
			mark_char = '}';
		} else if (lookahead[0]==-1) {
			// TODO mark token as erroneous
			break;
		}
		base_class->advance((GroRunScannerBase *) scanner, 0);
	}

	cat_unref_ptr(mark_stack);

	long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
	int end_column = base_class->getColumn((GroRunScannerBase *) scanner);
	base_class->advance((GroRunScannerBase *) scanner, 0);
	result = base_class->createToken((GroRunScannerBase *) scanner, MUL_EIF_SYM_MANIFEST_STRING, end_row, end_column+1, buf);
	return result;
}


/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulEifScanner *scanner = MUL_EIF_SCANNER(self);
	GroRunIToken *result = l_next_token(scanner);
	cat_log_error("next:%O", result);
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
