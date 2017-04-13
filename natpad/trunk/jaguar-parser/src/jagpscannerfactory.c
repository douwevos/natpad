/*
   File:    jagpscannerfactory.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Mar 11, 2017
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

#include "jagpscannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPScannerFactory"
#include <logging/catlog.h>

struct _JagPScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPScannerFactory, jagp_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_scanner_factory_class_init(JagPScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_scanner_factory_init(JagPScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPScannerFactory *instance = JAGP_SCANNER_FACTORY(object);
	JagPScannerFactoryPrivate *priv = jagp_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->connected_symbols);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(jagp_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_scanner_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_connect_symbol(CatArrayWo *connected_syms, int expected_conn_id, GroRunISymbolProvider *symbol_provider, const char *name) {
	CatStringWo *cwname = cat_string_wo_new_with(name);
	GroRunSymbol *symbol = grorun_isymbol_provider_get_symbol_by_name(symbol_provider, cwname);
	cat_unref_ptr(cwname);
	if (symbol==NULL) {
		cat_log_fatal("symbol with name not found name:%s", name)
		return;
	}
	if (cat_array_wo_size(connected_syms)!=expected_conn_id) {
		cat_log_fatal("incorrect offset for symbol with name:%s", name);
	}
	cat_array_wo_append(connected_syms, (GObject *) symbol);
}


static void l_add_kw_mapping(CatHashMapWo *map, const char *text, int code) {
	CatStringWo *k = cat_string_wo_new_with(text);
	CatInteger *v = cat_integer_new(code);
	cat_hash_map_wo_put(map, (GObject *) k, (GObject *) v);
	cat_unref_ptr(k);
	cat_unref_ptr(v);
}

JagPScannerFactory *jagp_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	JagPScannerFactory *result = g_object_new(JAGP_TYPE_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	JagPScannerFactoryPrivate *priv = jagp_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, JAGP_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, JAGP_SYM_ERROR, symbol_provider, "⊗");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LSQBRACE, symbol_provider, "L_LSQBRACE");
	l_connect_symbol(connected_syms, JAGP_SYM_L_RSQBRACE, symbol_provider, "L_RSQBRACE");
	l_connect_symbol(connected_syms, JAGP_SYM_L_ARROW, symbol_provider, "L_ARROW");
	l_connect_symbol(connected_syms, JAGP_SYM_L_AND, symbol_provider, "L_AND");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LT, symbol_provider, "L_LT");
	l_connect_symbol(connected_syms, JAGP_SYM_L_GT, symbol_provider, "L_GT");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LE, symbol_provider, "L_LE");
	l_connect_symbol(connected_syms, JAGP_SYM_L_GE, symbol_provider, "L_GE");
	l_connect_symbol(connected_syms, JAGP_SYM_L_EQ, symbol_provider, "L_EQ");
	l_connect_symbol(connected_syms, JAGP_SYM_L_NE, symbol_provider, "L_NE");
	l_connect_symbol(connected_syms, JAGP_SYM_L_COMMA, symbol_provider, "L_COMMA");
	l_connect_symbol(connected_syms, JAGP_SYM_L_QUESTION_MARK, symbol_provider, "L_QUESTION_MARK");
	l_connect_symbol(connected_syms, JAGP_SYM_L_DOT, symbol_provider, "L_DOT");
	l_connect_symbol(connected_syms, JAGP_SYM_L_XOR, symbol_provider, "L_XOR");
	l_connect_symbol(connected_syms, JAGP_SYM_L_MUL, symbol_provider, "L_MUL");
	l_connect_symbol(connected_syms, JAGP_SYM_L_DIV, symbol_provider, "L_DIV");
	l_connect_symbol(connected_syms, JAGP_SYM_L_MOD, symbol_provider, "L_MOD");
	l_connect_symbol(connected_syms, JAGP_SYM_L_SUB, symbol_provider, "L_SUB");
	l_connect_symbol(connected_syms, JAGP_SYM_L_ADD, symbol_provider, "L_ADD");
	l_connect_symbol(connected_syms, JAGP_SYM_L_IS, symbol_provider, "L_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_NOT, symbol_provider, "L_NOT");
	l_connect_symbol(connected_syms, JAGP_SYM_L_NEG, symbol_provider, "L_NEG");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LPAREN, symbol_provider, "L_LPAREN");
	l_connect_symbol(connected_syms, JAGP_SYM_L_RPAREN, symbol_provider, "L_RPAREN");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LCUBRACE, symbol_provider, "L_LCUBRACE");
	l_connect_symbol(connected_syms, JAGP_SYM_L_RCUBRACE, symbol_provider, "L_RCUBRACE");
	l_connect_symbol(connected_syms, JAGP_SYM_L_VARARGS, symbol_provider, "L_VARARGS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_AT, symbol_provider, "L_AT");
	l_connect_symbol(connected_syms, JAGP_SYM_L_INC, symbol_provider, "L_INC");
	l_connect_symbol(connected_syms, JAGP_SYM_L_DEC, symbol_provider, "L_DEC");
	l_connect_symbol(connected_syms, JAGP_SYM_L_COLON, symbol_provider, "L_COLON");
	l_connect_symbol(connected_syms, JAGP_SYM_L_SEMI, symbol_provider, "L_SEMI");
	l_connect_symbol(connected_syms, JAGP_SYM_L_COLON_COLON, symbol_provider, "L_COLON_COLON");
	l_connect_symbol(connected_syms, JAGP_SYM_L_BAR, symbol_provider, "L_BAR");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LOR, symbol_provider, "L_LOR");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LAND, symbol_provider, "L_LAND");
	l_connect_symbol(connected_syms, JAGP_SYM_L_MUL_IS, symbol_provider, "L_MUL_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_DIV_IS, symbol_provider, "L_DIV_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_MOD_IS, symbol_provider, "L_MOD_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_ADD_IS, symbol_provider, "L_ADD_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_SUB_IS, symbol_provider, "L_SUB_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_SHL_IS, symbol_provider, "L_SHL_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_SHR_IS, symbol_provider, "L_SHR_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_SHRR_IS, symbol_provider, "L_SHRR_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_AND_IS, symbol_provider, "L_AND_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_XOR_IS, symbol_provider, "L_XOR_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_L_OR_IS, symbol_provider, "L_OR_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_K_BOOLEAN, symbol_provider, "K_BOOLEAN");
	l_connect_symbol(connected_syms, JAGP_SYM_K_BYTE, symbol_provider, "K_BYTE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_SHORT, symbol_provider, "K_SHORT");
	l_connect_symbol(connected_syms, JAGP_SYM_K_INT, symbol_provider, "K_INT");
	l_connect_symbol(connected_syms, JAGP_SYM_K_LONG, symbol_provider, "K_LONG");
	l_connect_symbol(connected_syms, JAGP_SYM_K_CHAR, symbol_provider, "K_CHAR");
	l_connect_symbol(connected_syms, JAGP_SYM_K_DOUBLE, symbol_provider, "K_DOUBLE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_FLOAT, symbol_provider, "K_FLOAT");
	l_connect_symbol(connected_syms, JAGP_SYM_IDENTIFIER, symbol_provider, "IDENTIFIER");
	l_connect_symbol(connected_syms, JAGP_SYM_LITERAL, symbol_provider, "LITERAL");
	l_connect_symbol(connected_syms, JAGP_SYM_K_EXTENDS, symbol_provider, "K_EXTENDS");
	l_connect_symbol(connected_syms, JAGP_SYM_K_SUPER, symbol_provider, "K_SUPER");
	l_connect_symbol(connected_syms, JAGP_SYM_K_THIS, symbol_provider, "K_THIS");
	l_connect_symbol(connected_syms, JAGP_SYM_K_PACKAGE, symbol_provider, "K_PACKAGE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_PUBLIC, symbol_provider, "K_PUBLIC");
	l_connect_symbol(connected_syms, JAGP_SYM_K_PRIVATE, symbol_provider, "K_PRIVATE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_PROTECTED, symbol_provider, "K_PROTECTED");
	l_connect_symbol(connected_syms, JAGP_SYM_K_IMPORT, symbol_provider, "K_IMPORT");
	l_connect_symbol(connected_syms, JAGP_SYM_K_CLASS, symbol_provider, "K_CLASS");
	l_connect_symbol(connected_syms, JAGP_SYM_K_INTERFACE, symbol_provider, "K_INTERFACE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_ABSTRACT, symbol_provider, "K_ABSTRACT");
	l_connect_symbol(connected_syms, JAGP_SYM_K_STATIC, symbol_provider, "K_STATIC");
	l_connect_symbol(connected_syms, JAGP_SYM_K_FINAL, symbol_provider, "K_FINAL");
	l_connect_symbol(connected_syms, JAGP_SYM_K_STRICTFP, symbol_provider, "K_STRICTFP");
	l_connect_symbol(connected_syms, JAGP_SYM_K_IMPLEMENTS, symbol_provider, "K_IMPLEMENTS");
	l_connect_symbol(connected_syms, JAGP_SYM_K_TRANSIENT, symbol_provider, "K_TRANSIENT");
	l_connect_symbol(connected_syms, JAGP_SYM_K_VOLATILE, symbol_provider, "K_VOLATILE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_SYNCHRONIZED, symbol_provider, "K_SYNCHRONIZED");
	l_connect_symbol(connected_syms, JAGP_SYM_K_NATIVE, symbol_provider, "K_NATIVE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_VOID, symbol_provider, "K_VOID");
	l_connect_symbol(connected_syms, JAGP_SYM_K_THROWS, symbol_provider, "K_THROWS");
	l_connect_symbol(connected_syms, JAGP_SYM_K_ENUM, symbol_provider, "K_ENUM");
	l_connect_symbol(connected_syms, JAGP_SYM_K_DEFAULT, symbol_provider, "K_DEFAULT");
	l_connect_symbol(connected_syms, JAGP_SYM_K_IF, symbol_provider, "K_IF");
	l_connect_symbol(connected_syms, JAGP_SYM_K_ASSERT, symbol_provider, "K_ASSERT");
	l_connect_symbol(connected_syms, JAGP_SYM_K_SWITCH, symbol_provider, "K_SWITCH");
	l_connect_symbol(connected_syms, JAGP_SYM_K_CASE, symbol_provider, "K_CASE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_BREAK, symbol_provider, "K_BREAK");
	l_connect_symbol(connected_syms, JAGP_SYM_K_ELSE, symbol_provider, "K_ELSE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_DO, symbol_provider, "K_DO");
	l_connect_symbol(connected_syms, JAGP_SYM_K_WHILE, symbol_provider, "K_WHILE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_FOR, symbol_provider, "K_FOR");
	l_connect_symbol(connected_syms, JAGP_SYM_K_NEW, symbol_provider, "K_NEW");
	l_connect_symbol(connected_syms, JAGP_SYM_K_INSTANCEOF, symbol_provider, "K_INSTANCEOF");
	l_connect_symbol(connected_syms, JAGP_SYM_K_TRY, symbol_provider, "K_TRY");
	l_connect_symbol(connected_syms, JAGP_SYM_K_CATCH, symbol_provider, "K_CATCH");
	l_connect_symbol(connected_syms, JAGP_SYM_K_FINALLY, symbol_provider, "K_FINALLY");
	l_connect_symbol(connected_syms, JAGP_SYM_K_CONTINUE, symbol_provider, "K_CONTINUE");
	l_connect_symbol(connected_syms, JAGP_SYM_K_THROW, symbol_provider, "K_THROW");
	l_connect_symbol(connected_syms, JAGP_SYM_L_LPARREN2, symbol_provider, "L_LPARREN2");
	l_connect_symbol(connected_syms, JAGP_SYM_K_RETURN, symbol_provider, "K_RETURN");


	CatStringWo *t_eol_comment = cat_string_wo_new_with("EOL_COMMENT");
	GroRunSymbol *sym_eol = grorun_symbol_new(FALSE, t_eol_comment, JAGP_SYM_EOL_COMMENT);
	cat_array_wo_append(connected_syms, (GObject *) sym_eol);
	cat_unref_ptr(sym_eol);
	cat_unref_ptr(t_eol_comment);

	CatStringWo *t_full_comment = cat_string_wo_new_with("FULL_COMMENT");
	GroRunSymbol *sym_full_comment = grorun_symbol_new(FALSE, t_full_comment, JAGP_SYM_FULL_COMMENT);
	cat_array_wo_append(connected_syms, (GObject *) sym_full_comment);
	cat_unref_ptr(sym_full_comment);
	cat_unref_ptr(t_full_comment);


	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	l_add_kw_mapping(priv->keyword_mapping, "boolean", JAGP_SYM_K_BOOLEAN);
	l_add_kw_mapping(priv->keyword_mapping, "byte", JAGP_SYM_K_BYTE);
	l_add_kw_mapping(priv->keyword_mapping, "short", JAGP_SYM_K_SHORT);
	l_add_kw_mapping(priv->keyword_mapping, "int", JAGP_SYM_K_INT);
	l_add_kw_mapping(priv->keyword_mapping, "long", JAGP_SYM_K_LONG);
	l_add_kw_mapping(priv->keyword_mapping, "char", JAGP_SYM_K_CHAR);
	l_add_kw_mapping(priv->keyword_mapping, "double", JAGP_SYM_K_DOUBLE);
	l_add_kw_mapping(priv->keyword_mapping, "float", JAGP_SYM_K_FLOAT);
	l_add_kw_mapping(priv->keyword_mapping, "extends", JAGP_SYM_K_EXTENDS);
	l_add_kw_mapping(priv->keyword_mapping, "super", JAGP_SYM_K_SUPER);
	l_add_kw_mapping(priv->keyword_mapping, "this", JAGP_SYM_K_THIS);
	l_add_kw_mapping(priv->keyword_mapping, "package", JAGP_SYM_K_PACKAGE);
	l_add_kw_mapping(priv->keyword_mapping, "public", JAGP_SYM_K_PUBLIC);
	l_add_kw_mapping(priv->keyword_mapping, "private", JAGP_SYM_K_PRIVATE);
	l_add_kw_mapping(priv->keyword_mapping, "protected", JAGP_SYM_K_PROTECTED);
	l_add_kw_mapping(priv->keyword_mapping, "import", JAGP_SYM_K_IMPORT);
	l_add_kw_mapping(priv->keyword_mapping, "class", JAGP_SYM_K_CLASS);
	l_add_kw_mapping(priv->keyword_mapping, "interface", JAGP_SYM_K_INTERFACE);
	l_add_kw_mapping(priv->keyword_mapping, "abstract", JAGP_SYM_K_ABSTRACT);
	l_add_kw_mapping(priv->keyword_mapping, "static", JAGP_SYM_K_STATIC);
	l_add_kw_mapping(priv->keyword_mapping, "final", JAGP_SYM_K_FINAL);
	l_add_kw_mapping(priv->keyword_mapping, "strictfp", JAGP_SYM_K_STRICTFP);
	l_add_kw_mapping(priv->keyword_mapping, "implements", JAGP_SYM_K_IMPLEMENTS);
	l_add_kw_mapping(priv->keyword_mapping, "transient", JAGP_SYM_K_TRANSIENT);
	l_add_kw_mapping(priv->keyword_mapping, "volatile", JAGP_SYM_K_VOLATILE);
	l_add_kw_mapping(priv->keyword_mapping, "synchronized", JAGP_SYM_K_SYNCHRONIZED);
	l_add_kw_mapping(priv->keyword_mapping, "native", JAGP_SYM_K_NATIVE);
	l_add_kw_mapping(priv->keyword_mapping, "void", JAGP_SYM_K_VOID);
	l_add_kw_mapping(priv->keyword_mapping, "throws", JAGP_SYM_K_THROWS);
	l_add_kw_mapping(priv->keyword_mapping, "enum", JAGP_SYM_K_ENUM);
	l_add_kw_mapping(priv->keyword_mapping, "default", JAGP_SYM_K_DEFAULT);
	l_add_kw_mapping(priv->keyword_mapping, "if", JAGP_SYM_K_IF);
	l_add_kw_mapping(priv->keyword_mapping, "assert", JAGP_SYM_K_ASSERT);
	l_add_kw_mapping(priv->keyword_mapping, "switch", JAGP_SYM_K_SWITCH);
	l_add_kw_mapping(priv->keyword_mapping, "case", JAGP_SYM_K_CASE);
	l_add_kw_mapping(priv->keyword_mapping, "break", JAGP_SYM_K_BREAK);
	l_add_kw_mapping(priv->keyword_mapping, "else", JAGP_SYM_K_ELSE);
	l_add_kw_mapping(priv->keyword_mapping, "do", JAGP_SYM_K_DO);
	l_add_kw_mapping(priv->keyword_mapping, "while", JAGP_SYM_K_WHILE);
	l_add_kw_mapping(priv->keyword_mapping, "for", JAGP_SYM_K_FOR);
	l_add_kw_mapping(priv->keyword_mapping, "new", JAGP_SYM_K_NEW);
	l_add_kw_mapping(priv->keyword_mapping, "instanceof", JAGP_SYM_K_INSTANCEOF);
	l_add_kw_mapping(priv->keyword_mapping, "try", JAGP_SYM_K_TRY);
	l_add_kw_mapping(priv->keyword_mapping, "catch", JAGP_SYM_K_CATCH);
	l_add_kw_mapping(priv->keyword_mapping, "finally", JAGP_SYM_K_FINALLY);
	l_add_kw_mapping(priv->keyword_mapping, "continue", JAGP_SYM_K_CONTINUE);
	l_add_kw_mapping(priv->keyword_mapping, "throw", JAGP_SYM_K_THROW);
	l_add_kw_mapping(priv->keyword_mapping, "return", JAGP_SYM_K_RETURN);

	return result;
}

JagPScanner *jagp_scanner_factory_create_scanner(JagPScannerFactory *factory, CatIUtf8Scanner *scanner) {
	JagPScannerFactoryPrivate *priv = jagp_scanner_factory_get_instance_private(factory);
	return jagp_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

