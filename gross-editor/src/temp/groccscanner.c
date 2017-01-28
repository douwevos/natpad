///*
//   File:    groccscanner.c
//   Project: gross-editor
//   Author:  Douwe Vos
//   Date:    Oct 12, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "groccscanner.h"
//#include <grossruntime.h>
//#include <string.h>
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroCCScanner"
//#include <logging/catlog.h>
//
//
//
//struct _GroCCScannerPrivate {
//	GroRunSymbol *connected_symbols[16];
//	CatIUtf8Scanner *input;
//	CatStreamStatus input_status;
//	GroRunITokenFactory *token_factory;
//	gunichar lookahead[5];
//	gunichar prevLookahead;
//	gunichar nextChar;
//	int row, column;
//	GroRunLocation *location;
//
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//static void l_scanner_iface_init(GroRunIScannerInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroCCScanner, grocc_scanner, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroCCScanner)
//		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void grocc_scanner_class_init(GroCCScannerClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void grocc_scanner_init(GroCCScanner *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	GroCCScanner *instance = GROCC_SCANNER(object);
//	GroCCScannerPrivate *priv = grocc_scanner_get_instance_private(instance);
//	cat_unref_ptr(priv->input);
//	cat_unref_ptr(priv->location);
//	cat_unref_ptr(priv->token_factory);
//	G_OBJECT_CLASS(grocc_scanner_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(grocc_scanner_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//static GroRunSymbol *l_extract_symbol(GroRunISymbolProvider *symbol_provider, const char *name) {
//	CatStringWo *cwname = cat_string_wo_new_with(name);
//	GroRunSymbol *result = grorun_isymbol_provider_get_symbol_by_name(symbol_provider, cwname);
//	cat_unref_ptr(cwname);
//	return result;
//}
//
//static gunichar l_advance(GroCCScannerPrivate *priv);
//
//GroCCScanner *grocc_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, GroRunISymbolProvider *symbol_provider) {
//	GroCCScanner *result = g_object_new(GROCC_TYPE_SCANNER, NULL);
//	cat_ref_anounce(result);
//	GroCCScannerPrivate *priv = grocc_scanner_get_instance_private(result);
//
//	priv->connected_symbols[SYM_END_OF_INPUT] = l_extract_symbol(symbol_provider, "⟧");
//	priv->connected_symbols[SYM_NUMBER] = l_extract_symbol(symbol_provider, "NUMBER");
//	priv->connected_symbols[SYM_RULE_SEPERATOR] = l_extract_symbol(symbol_provider, "RIGHT_DOUBLE_ARROW");
//	priv->connected_symbols[SYM_INDEX_MARK] = l_extract_symbol(symbol_provider, "INDEX_MARK");
//	priv->connected_symbols[SYM_COMMA] = l_extract_symbol(symbol_provider, "COMMA");
//	priv->connected_symbols[SYM_COLON] = l_extract_symbol(symbol_provider, "COLON");
//	priv->connected_symbols[SYM_SEMI] = l_extract_symbol(symbol_provider, "SEMI");
//	priv->connected_symbols[SYM_BAR] = l_extract_symbol(symbol_provider, "BAR");
//	priv->connected_symbols[SYM_EXCL] = l_extract_symbol(symbol_provider, "EXCL");
//	priv->connected_symbols[SYM_STAR] = l_extract_symbol(symbol_provider, "STAR");
//	priv->connected_symbols[SYM_PLUS] = l_extract_symbol(symbol_provider, "PLUS");
//	priv->connected_symbols[SYM_LPAREN] = l_extract_symbol(symbol_provider, "LPAREN");
//	priv->connected_symbols[SYM_RPAREN] = l_extract_symbol(symbol_provider, "RPAREN");
//	priv->connected_symbols[SYM_LSQBRACK] = l_extract_symbol(symbol_provider, "LSQBRACK");
//	priv->connected_symbols[SYM_RSQBRACK] = l_extract_symbol(symbol_provider, "RSQBRACK");
//	priv->connected_symbols[SYM_LCUBRACK] = l_extract_symbol(symbol_provider, "LCUBRACK");
//	priv->connected_symbols[SYM_RCUBRACK] = l_extract_symbol(symbol_provider, "RCUBRACK");
//	priv->connected_symbols[SYM_KW_TERMINAL] = l_extract_symbol(symbol_provider, "TERMINAL");
//	priv->connected_symbols[SYM_KW_CODE] = l_extract_symbol(symbol_provider, "CODE");
//	priv->connected_symbols[SYM_KW_TYPE] = l_extract_symbol(symbol_provider, "TYPE");
//	priv->connected_symbols[SYM_KW_IMPORTS] = l_extract_symbol(symbol_provider, "IMPORTS");
//	priv->connected_symbols[SYM_IDENTIFIER] = l_extract_symbol(symbol_provider, "ID");
//	priv->connected_symbols[SYM_MULTI_LINE_TEXT] = l_extract_symbol(symbol_provider, "CODE_TEXT");
//
//	priv->input_status = CAT_STREAM_OK;
//	priv->input = cat_ref_ptr(scanner);
//	priv->token_factory = cat_ref_ptr(token_factory);
//
//	int idx;
//	for(idx=1; idx<5; idx++) {
//		priv->lookahead[idx] = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
//	}
//	priv->nextChar = 0;
//	priv->prevLookahead = 0;
//	priv->row = 0;
//	priv->column = -1;
//	priv->location = NULL;
//	l_advance(priv);
//	return result;
//}
//
//gboolean grocc_scanner_is_eof(GroCCScanner *scanner, GroRunIToken *token) {
//	if (token==NULL) {
//		return TRUE;
//	}
//	GroCCScannerPrivate *priv = grocc_scanner_get_instance_private(scanner);
//	if (grorun_itoken_get_symbol(token) == priv->connected_symbols[SYM_END_OF_INPUT]) {
//		return TRUE;
//	}
//	return FALSE;
//}
//
//int grocc_scanner_get_symbol_index(GroCCScanner *scanner, GroRunSymbol *sym) {
//	GroCCScannerPrivate *priv = grocc_scanner_get_instance_private(scanner);
//	int idx = 0;
//	for(idx=0; idx<16; idx++) {
//		if (priv->connected_symbols[idx]==sym) {
//			return idx;
//		}
//	}
//	return -1;
//}
//
//
//static gunichar l_next_lookahead(GroCCScannerPrivate *priv){
//	int idx;
//	for(idx=1; idx<5; idx++) {
//		priv->lookahead[idx-1] = priv->lookahead[idx];
//	}
//	priv->lookahead[4] = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
//	gunichar result = priv->lookahead[0];
//	if (result==0xa) {
//		if (priv->prevLookahead==0xd) {
//			priv->prevLookahead = 0;
//		} else {
//			priv->prevLookahead = result;
//			priv->row++;
//		}
//		priv->column = -1;
//	} else if (result==0xd) {
//		if (priv->prevLookahead==0xa) {
//			priv->prevLookahead = 0;
//		} else {
//			priv->prevLookahead = result;
//			priv->row++;
//		}
//		priv->column = -1;
//	} else if (result==-1) {
//		return -1;
//	} else {
//		if (result < 0x80) {
//			priv->column++;
//		} else if (result< (1<<12)) {
//			priv->column+= 2;
//		} else if (result< (1<<18)) {
//			priv->column+= 3;
//		} else if (result< (1<<24)) {
//			priv->column+= 4;
//		} else if (result< (1<<30)) {
//			priv->column+= 5;
//		} else {
//			priv->column+= 6;
//		}
//		priv->prevLookahead = result;
//	}
//	return result;
//}
//
//
//static gunichar l_advance(GroCCScannerPrivate *priv) {
//	while(TRUE) {
//		priv->nextChar = l_next_lookahead(priv);
//		if (priv->nextChar==0xa || priv->nextChar==0xd) {
//			continue;
//		} else if (priv->nextChar==-1) {
//			return -1;
//		}
//		break;
//	}
//	return priv->nextChar;
//}
//
//
//static GroRunIToken *l_create_token(GroCCScannerPrivate *priv, int conn_sym_id, GroRunLocation *c_location, void *c_value) {
//	GroRunSymbol *symbol = priv->connected_symbols[conn_sym_id];
//	GroRunIToken *result = grorun_itoken_factory_create_token(priv->token_factory, symbol, (GObject *) c_value, c_location, NULL);
//	if (GRORUN_IS_FULL_TOKEN(result)) {
//		grorun_full_token_set_user_index((GroRunFullToken *) result, conn_sym_id);
//	}
//	cat_unref_ptr(c_location);
//	cat_unref_ptr(c_value);
//	return result;
//}
//
//static GroRunIToken *l_scan_for_number(GroCCScannerPrivate *priv) {
//	int lastCol = priv->column+1;
//	long lastRow = priv->row;
//	long result = 0;
//	while(priv->nextChar>='0' && priv->nextChar<='9') {
//		result = result*10 + (priv->nextChar-'0');
//		lastCol = priv->column+1;
//		lastRow = priv->row;
//		l_advance(priv);
//		if (priv->row!=lastRow) {
//			break;
//		}
//	}
//	return l_create_token(priv, SYM_NUMBER, grorun_location_set_end(priv->location, lastCol, lastRow), cat_long_new(result));
//}
//
//static GroRunIToken *l_scan_for_code_text(GroCCScannerPrivate *priv) {
//	int lastCol = priv->column;
//	long lastRow = priv->row;
//	gunichar next_lah = l_next_lookahead(priv);
//	CatStringWo *buf = cat_string_wo_new();
//	while(next_lah!=-1 && next_lah!=0xFF60) {	// '｠'
//		cat_string_wo_append_unichar(buf, next_lah);
//		lastCol = priv->column+1;
//		lastRow = priv->row;
//		next_lah = l_next_lookahead(priv);
//	}
//	if (next_lah==0xFF60) {	// '｠'
//		lastCol++;
//	}
//	l_advance(priv);
//	return l_create_token(priv, SYM_MULTI_LINE_TEXT, grorun_location_set_end(priv->location, lastCol, lastRow), buf);
//}
//
//static GroRunIToken *l_scan_for_id(GroCCScannerPrivate *priv) {
//	int lastCol = priv->column+1;
//	long lastRow = priv->row;
//	CatStringWo *buf = cat_string_wo_new();
//	while(g_unichar_isalnum(priv->nextChar) || priv->nextChar=='_' || g_unichar_isdigit(priv->nextChar)) {
//		cat_string_wo_append_unichar(buf, priv->nextChar);
//		lastCol = priv->column+1;
//		lastRow = priv->row;
//		l_advance(priv);
//		if (priv->row!=lastRow) {
//			break;
//		}
//	}
//	const char *idText = cat_string_wo_getchars(buf);
//	int connSymId = SYM_IDENTIFIER;
//	if (strcmp("terminal", idText)==0) {
//		connSymId = SYM_KW_TERMINAL;
//	} else if (strcmp("code", idText)==0) {
//		connSymId = SYM_KW_CODE;
//	} else if (strcmp("type", idText)==0) {
//		connSymId = SYM_KW_TYPE;
//	} else if (strcmp("imports", idText)==0) {
//		connSymId = SYM_KW_IMPORTS;
//	}
//
//	return l_create_token(priv, connSymId, grorun_location_set_end(priv->location, lastCol, lastRow), buf);
//}
//
//
//static CatS l_s_eof = CAT_S_DEF("⟧");
//static CatS l_s_rule_seperator = CAT_S_DEF("⟶");
//static CatS l_s_left_square = CAT_S_DEF("[");
//static CatS l_s_right_square = CAT_S_DEF("]");
//static CatS l_s_index_mark = CAT_S_DEF("≣");
//static CatS l_s_comma = CAT_S_DEF(",");
//static CatS l_s_colon = CAT_S_DEF(":");
//static CatS l_s_semi = CAT_S_DEF(";");
//static CatS l_s_bar = CAT_S_DEF("|");
//
//static GroRunIToken *l_next(GroCCScannerPrivate *priv) {
//	GroRunIToken *result = NULL;
//	while(result == NULL) {
//		cat_unref_ptr(priv->location);
//		priv->location = grorun_location_new(priv->column, priv->row, priv->column+1, priv->row);
//		cat_log_debug("priv->nextChar=%x", priv->nextChar);
//		switch(priv->nextChar) {
//
//			case -1 :
//				result = l_create_token(priv, SYM_END_OF_INPUT, cat_ref_ptr(priv->location), CAT_S(l_s_eof));
//				break;
//			case 0x27F6 : // '⟶'
//			case 0x21D2 : { // '⇒'
//				result = l_create_token(priv, SYM_RULE_SEPERATOR, cat_ref_ptr(priv->location), CAT_S(l_s_rule_seperator));
//				l_advance(priv);
//			} break;
//
//			case '[' : {
//				result = l_create_token(priv, SYM_LSQBRACK, cat_ref_ptr(priv->location), CAT_S(l_s_left_square));
//				l_advance(priv);
//			} break;
//			case ']' : {
//				result = l_create_token(priv, SYM_RSQBRACK, cat_ref_ptr(priv->location), CAT_S(l_s_right_square));
//				l_advance(priv);
//			} break;
//
//			case 0xFF5F : { // '｟'
//				result = l_scan_for_code_text(priv);
//			} break;
//
//
//			case 0x2263 : // '≣'
//			case '#' : {
//				result = l_create_token(priv, SYM_INDEX_MARK, cat_ref_ptr(priv->location), CAT_S(l_s_index_mark));
//				l_advance(priv);
//			} break;
//			case ',' : {
//				result = l_create_token(priv, SYM_COMMA, cat_ref_ptr(priv->location), CAT_S(l_s_comma));
//				l_advance(priv);
//			} break;
//			case ':' : {
//				l_advance(priv);
////						if (lookahead[0]==':' && lookahead[1]=='=') {
////							l_advance(priv);
////							l_advance(priv);
////							result = tokenFactory.createToken(symbolProvider.getTerminalByName("COLON_COLON_EQUALS"), NULL);
////						} else {
//				result = l_create_token(priv, SYM_COLON, cat_ref_ptr(priv->location), CAT_S(l_s_colon));
////						}
//			} break;
//			case ';' : {
//				result = l_create_token(priv, SYM_SEMI, cat_ref_ptr(priv->location), CAT_S(l_s_semi));
//				l_advance(priv);
//			} break;
//			case '|' : {
//				result = l_create_token(priv, SYM_BAR, cat_ref_ptr(priv->location), CAT_S(l_s_bar));
//				l_advance(priv);
//			} break;
//			default : {
//				if (g_unichar_isalpha(priv->nextChar)) {
//					result = l_scan_for_id(priv);
//				} else if (priv->nextChar>='0' && priv->nextChar<='9') {
//					result = l_scan_for_number(priv);
//				} else {
//					l_advance(priv);
//				}
//			}
//		}
//	}
//	return result;
//}
//
//
///********************* start GroRunIScanner implementation *********************/
//
//
//static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
//	GroCCScanner *scanner = GROCC_SCANNER(self);
//	GroCCScannerPrivate *priv = grocc_scanner_get_instance_private(scanner);
//	return l_next(priv);
//}
//
//
//static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
//	iface->next = l_scanner_next;
//}
//
///********************* end GroRunIScanner implementation *********************/
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroCCScanner *instance = GROCC_SCANNER(self);
//	GroCCScannerPrivate *priv = grocc_scanner_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%p]", iname, self);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
//
