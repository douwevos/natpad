/*
   File:    groscanner.c
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Nov 18, 2016
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

#include "groscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroScanner"
#include <logging/catlog.h>

struct _GroScannerPrivate {
	void *dummy;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroScanner, gro_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_ADD_PRIVATE(GroScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_scanner_class_init(GroScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_scanner_init(GroScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroScanner *instance = GRO_SCANNER(object);
//	GroScannerPrivate *priv = gro_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(gro_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroScanner *gro_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols) {
	GroScanner *result = g_object_new(GRO_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, scanner, keyword_mapping, connected_symbols, 3);
	return result;
}


gboolean gro_scanner_is_eof(GroScanner *scanner, GroRunIToken *token) {
	if (token==NULL) {
		return TRUE;
	}
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunSymbol *eof_symbol = base_class->getConnectedSymbol((GroRunScannerBase *) scanner, GRO_SYM_END_OF_INPUT);
	if (grorun_itoken_get_symbol(token) == eof_symbol) {
		return TRUE;
	}
	return FALSE;
}

static GroRunIToken *l_scan_for_code_text(GroScanner *scanner);
static GroRunIToken *l_scan_for_number(GroScanner *scanner);

static GroRunIToken *l_next_token(GroScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);

	while(result==NULL) {
		base_class->markLocation((GroRunScannerBase *) scanner);
		cat_log_debug("lookahead[0]=%c", lookahead[0]);
		switch(lookahead[0]) {
			case -1 :
				result = base_class->createToken((GroRunScannerBase *) scanner, GRO_SYM_END_OF_INPUT, -1, -1, NULL);
				break;

			case 0x27F6 : // '⟶'
			case 0x21D2 : // '⇒'
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_RULE_SEPERATOR, 1);
				break;

			case 0x2263 : // '≣'
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_INDEX_MARK, 1);
				break;

			case ',' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_COMMA, 1);
				break;

			case ':' :
//				if (lookahead[1]=='.') {
//					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_COLON_DOT, 2);
//				} else {
					result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_COLON, 1);
//				}
				break;

			case ';' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_SEMI, 1);
				break;

			case '+' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_PLUS, 1);
				break;

			case '*' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_STAR, 1);
				break;

			case '?' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_EXCL, 1);
				break;

			case '[' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_LSQBRACK, 1);
				break;
			case ']' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_RSQBRACK, 1);
				break;

			case '{' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_LCUBRACK, 1);
				break;
			case '}' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_RCUBRACK, 1);
				break;

			case '(' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_LPAREN, 1);
				break;
			case ')' :
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_RPAREN, 1);
				break;

			case 0x2297 : // '⊗'
				result = base_class->createTokenBasic((GroRunScannerBase *) scanner, GRO_SYM_IDENTIFIER, 1);
				break;

			case 0xFF5F : // '｟'
				result = l_scan_for_code_text(scanner);
				break;

			case '#' :
				result = base_class->scanForEndOfLine((GroRunScannerBase *) scanner, GRO_SYM_EOL_COMMENT, TRUE);
				break;

			default : {
				gunichar ch = lookahead[0];
				if (g_unichar_isalpha(ch)) {
					result = base_class->scanForIdOrKeyword((GroRunScannerBase *) scanner, GRO_SYM_IDENTIFIER, TRUE);
				} else if (ch>='0' && ch<='9') {
					result = l_scan_for_number(scanner);
				} else {
					base_class->advance((GroRunScannerBase *) scanner, 0);
				}
			} break;
		}
	}
	return result;
}

static GroRunIToken *l_scan_for_code_text(GroScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	gunichar ch = lookahead[0];

	CatStringWo *buf = cat_string_wo_new();
	while((ch!=-1) && (ch!=0xFF60)) {	// '｠'
		cat_log_debug("ct-ch=%d",ch);
		cat_string_wo_append_unichar(buf, ch);
		ch = base_class->advance((GroRunScannerBase *) scanner, 0);
	}


	cat_log_debug("ct done");

	long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
	int end_column = base_class->getColumn((GroRunScannerBase *) scanner);

	base_class->advance((GroRunScannerBase *) scanner, 0);
	return base_class->createToken((GroRunScannerBase *) scanner, GRO_SYM_CODE_TEXT, end_row, end_column, buf);
}

static GroRunIToken *l_scan_for_number(GroScanner *scanner) {
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer((GroRunScannerBase *) scanner);
	gunichar ch = lookahead[1];

	int32_t val = 0;

	val = lookahead[0]-'0';
	while(ch!=-1 && ch>='0' && ch<='9') {
		val = val*10 + (ch-'0');
		base_class->advance((GroRunScannerBase *) scanner, 0);
		ch = lookahead[1];
	}

	long long end_row = base_class->getRow((GroRunScannerBase *) scanner);
	int end_column = base_class->getColumn((GroRunScannerBase *) scanner);

	base_class->advance((GroRunScannerBase *) scanner, 0);
	return base_class->createToken((GroRunScannerBase *) scanner, GRO_SYM_NUMBER, end_row, end_column, cat_integer_new(val));
}


/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	GroScanner *scanner = GRO_SCANNER(self);
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
