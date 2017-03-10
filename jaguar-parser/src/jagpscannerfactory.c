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
	l_connect_symbol(connected_syms, JAGP_SYM_ABSTRACT, symbol_provider, "ABSTRACT");
	l_connect_symbol(connected_syms, JAGP_SYM_DO, symbol_provider, "DO");
	l_connect_symbol(connected_syms, JAGP_SYM_IMPLEMENTS, symbol_provider, "IMPLEMENTS");
	l_connect_symbol(connected_syms, JAGP_SYM_PACKAGE, symbol_provider, "PACKAGE");
	l_connect_symbol(connected_syms, JAGP_SYM_THROW, symbol_provider, "THROW");
	l_connect_symbol(connected_syms, JAGP_SYM_BOOLEAN, symbol_provider, "BOOLEAN");
	l_connect_symbol(connected_syms, JAGP_SYM_DOUBLE, symbol_provider, "DOUBLE");
	l_connect_symbol(connected_syms, JAGP_SYM_IMPORT, symbol_provider, "IMPORT");
	l_connect_symbol(connected_syms, JAGP_SYM_PRIVATE, symbol_provider, "PRIVATE");
	l_connect_symbol(connected_syms, JAGP_SYM_THROWS, symbol_provider, "THROWS");
	l_connect_symbol(connected_syms, JAGP_SYM_BREAK, symbol_provider, "BREAK");
	l_connect_symbol(connected_syms, JAGP_SYM_ELSE, symbol_provider, "ELSE");
	l_connect_symbol(connected_syms, JAGP_SYM_PROTECTED, symbol_provider, "PROTECTED");
	l_connect_symbol(connected_syms, JAGP_SYM_TRANSIENT, symbol_provider, "TRANSIENT");
	l_connect_symbol(connected_syms, JAGP_SYM_BYTE, symbol_provider, "BYTE");
	l_connect_symbol(connected_syms, JAGP_SYM_SHORT, symbol_provider, "SHORT");
	l_connect_symbol(connected_syms, JAGP_SYM_INT, symbol_provider, "INT");
	l_connect_symbol(connected_syms, JAGP_SYM_LONG, symbol_provider, "LONG");
	l_connect_symbol(connected_syms, JAGP_SYM_CHAR, symbol_provider, "CHAR");
	l_connect_symbol(connected_syms, JAGP_SYM_EXTENDS, symbol_provider, "EXTENDS");
	l_connect_symbol(connected_syms, JAGP_SYM_INSTANCEOF, symbol_provider, "INSTANCEOF");
	l_connect_symbol(connected_syms, JAGP_SYM_PUBLIC, symbol_provider, "PUBLIC");
	l_connect_symbol(connected_syms, JAGP_SYM_TRY, symbol_provider, "TRY");
	l_connect_symbol(connected_syms, JAGP_SYM_CASE, symbol_provider, "CASE");
	l_connect_symbol(connected_syms, JAGP_SYM_FINAL, symbol_provider, "FINAL");
	l_connect_symbol(connected_syms, JAGP_SYM_FINALLY, symbol_provider, "FINALLY");
	l_connect_symbol(connected_syms, JAGP_SYM_INTERFACE, symbol_provider, "INTERFACE");
	l_connect_symbol(connected_syms, JAGP_SYM_RETURN, symbol_provider, "RETURN");
	l_connect_symbol(connected_syms, JAGP_SYM_VOID, symbol_provider, "VOID");
	l_connect_symbol(connected_syms, JAGP_SYM_CATCH, symbol_provider, "CATCH");
	l_connect_symbol(connected_syms, JAGP_SYM_FLOAT, symbol_provider, "FLOAT");
	l_connect_symbol(connected_syms, JAGP_SYM_VOLATILE, symbol_provider, "VOLATILE");
	l_connect_symbol(connected_syms, JAGP_SYM_FOR, symbol_provider, "FOR");
	l_connect_symbol(connected_syms, JAGP_SYM_NATIVE, symbol_provider, "NATIVE");
	l_connect_symbol(connected_syms, JAGP_SYM_STATIC, symbol_provider, "STATIC");
	l_connect_symbol(connected_syms, JAGP_SYM_WHILE, symbol_provider, "WHILE");
	l_connect_symbol(connected_syms, JAGP_SYM_CLASS, symbol_provider, "CLASS");
	l_connect_symbol(connected_syms, JAGP_SYM_NEW, symbol_provider, "NEW");
	l_connect_symbol(connected_syms, JAGP_SYM_SUPER, symbol_provider, "SUPER");
	l_connect_symbol(connected_syms, JAGP_SYM_CONST, symbol_provider, "CONST");
	l_connect_symbol(connected_syms, JAGP_SYM_NULL, symbol_provider, "NULL");
	l_connect_symbol(connected_syms, JAGP_SYM_SWITCH, symbol_provider, "SWITCH");
	l_connect_symbol(connected_syms, JAGP_SYM_CONTINUE, symbol_provider, "CONTINUE");
	l_connect_symbol(connected_syms, JAGP_SYM_GOTO, symbol_provider, "GOTO");
	l_connect_symbol(connected_syms, JAGP_SYM_SYNCHRONIZED, symbol_provider, "SYNCHRONIZED");
	l_connect_symbol(connected_syms, JAGP_SYM_DEFAULT, symbol_provider, "DEFAULT");
	l_connect_symbol(connected_syms, JAGP_SYM_IF, symbol_provider, "IF");
	l_connect_symbol(connected_syms, JAGP_SYM_THIS, symbol_provider, "THIS");
	l_connect_symbol(connected_syms, JAGP_SYM_LPAREN, symbol_provider, "LPAREN");
	l_connect_symbol(connected_syms, JAGP_SYM_RPAREN, symbol_provider, "RPAREN");
	l_connect_symbol(connected_syms, JAGP_SYM_LBRACE, symbol_provider, "LBRACE");
	l_connect_symbol(connected_syms, JAGP_SYM_RBRACE, symbol_provider, "RBRACE");
	l_connect_symbol(connected_syms, JAGP_SYM_LBRACK, symbol_provider, "LBRACK");
	l_connect_symbol(connected_syms, JAGP_SYM_RBRACK, symbol_provider, "RBRACK");
	l_connect_symbol(connected_syms, JAGP_SYM_IDENTIFIER, symbol_provider, "IDENTIFIER");
	l_connect_symbol(connected_syms, JAGP_SYM_DOT, symbol_provider, "DOT");
	l_connect_symbol(connected_syms, JAGP_SYM_COMMA, symbol_provider, "COMMA");
	l_connect_symbol(connected_syms, JAGP_SYM_COLON, symbol_provider, "COLON");
	l_connect_symbol(connected_syms, JAGP_SYM_SEMICOLON, symbol_provider, "SEMICOLON");
	l_connect_symbol(connected_syms, JAGP_SYM_STRING_LITERAL, symbol_provider, "STRING_LITERAL");
	l_connect_symbol(connected_syms, JAGP_SYM_BOOLEAN_LITERAL, symbol_provider, "BOOLEAN_LITERAL");
	l_connect_symbol(connected_syms, JAGP_SYM_QUESTION, symbol_provider, "QUESTION");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_MUL, symbol_provider, "OP_MUL");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_DIV, symbol_provider, "OP_DIV");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_SUB, symbol_provider, "OP_SUB");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_ADD, symbol_provider, "OP_ADD");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_AND, symbol_provider, "OP_AND");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_OR, symbol_provider, "OP_OR");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_XOR, symbol_provider, "OP_XOR");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_NOT, symbol_provider, "OP_NOT");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_NEG, symbol_provider, "OP_NEG");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_LAND, symbol_provider, "OP_LAND");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_INC, symbol_provider, "OP_INC");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_DEC, symbol_provider, "OP_DEC");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_MOD, symbol_provider, "OP_MOD");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_LOR, symbol_provider, "OP_LOR");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_SHL, symbol_provider, "OP_SHL");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_SHR, symbol_provider, "OP_SHR");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_SHRR, symbol_provider, "OP_SHRR");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_EQ, symbol_provider, "OP_EQ");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_NE, symbol_provider, "OP_NE");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_GE, symbol_provider, "OP_GE");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_GT, symbol_provider, "OP_GT");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_LE, symbol_provider, "OP_LE");
	l_connect_symbol(connected_syms, JAGP_SYM_OP_LT, symbol_provider, "OP_LT");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_MUL, symbol_provider, "ASS_MUL");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_DIV, symbol_provider, "ASS_DIV");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_MOD, symbol_provider, "ASS_MOD");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_ADD, symbol_provider, "ASS_ADD");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_SUB, symbol_provider, "ASS_SUB");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_SHL, symbol_provider, "ASS_SHL");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_SHR, symbol_provider, "ASS_SHR");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_SHRR, symbol_provider, "ASS_SHRR");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_AND, symbol_provider, "ASS_AND");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_XOR, symbol_provider, "ASS_XOR");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_OR, symbol_provider, "ASS_OR");
	l_connect_symbol(connected_syms, JAGP_SYM_ASS_IS, symbol_provider, "ASS_IS");
	l_connect_symbol(connected_syms, JAGP_SYM_AT, symbol_provider, "AT");
	l_connect_symbol(connected_syms, JAGP_SYM_INTEGER_LITERAL, symbol_provider, "INTEGER_LITERAL");
	l_connect_symbol(connected_syms, JAGP_SYM_FLOATING_POINT_LITERAL, symbol_provider, "FLOATING_POINT_LITERAL");
	l_connect_symbol(connected_syms, JAGP_SYM_CHARACTER_LITERAL, symbol_provider, "CHARACTER_LITERAL");
	l_connect_symbol(connected_syms, JAGP_SYM_STRICTFP, symbol_provider, "STRICTFP");
	l_connect_symbol(connected_syms, JAGP_SYM_ASSERT, symbol_provider, "ASSERT");
	l_connect_symbol(connected_syms, JAGP_SYM_ELLIPSIS, symbol_provider, "ELLIPSIS");
	l_connect_symbol(connected_syms, JAGP_SYM_ENUM, symbol_provider, "ENUM");


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
	l_add_kw_mapping(priv->keyword_mapping, "abstract", JAGP_SYM_ABSTRACT);
	l_add_kw_mapping(priv->keyword_mapping, "do", JAGP_SYM_DO);
	l_add_kw_mapping(priv->keyword_mapping, "implements", JAGP_SYM_IMPLEMENTS);
	l_add_kw_mapping(priv->keyword_mapping, "package", JAGP_SYM_PACKAGE);
	l_add_kw_mapping(priv->keyword_mapping, "throw", JAGP_SYM_THROW);
	l_add_kw_mapping(priv->keyword_mapping, "boolean", JAGP_SYM_BOOLEAN);
	l_add_kw_mapping(priv->keyword_mapping, "double", JAGP_SYM_DOUBLE);
	l_add_kw_mapping(priv->keyword_mapping, "import", JAGP_SYM_IMPORT);
	l_add_kw_mapping(priv->keyword_mapping, "private", JAGP_SYM_PRIVATE);
	l_add_kw_mapping(priv->keyword_mapping, "throws", JAGP_SYM_THROWS);
	l_add_kw_mapping(priv->keyword_mapping, "break", JAGP_SYM_BREAK);
	l_add_kw_mapping(priv->keyword_mapping, "else", JAGP_SYM_ELSE);
	l_add_kw_mapping(priv->keyword_mapping, "protected", JAGP_SYM_PROTECTED);
	l_add_kw_mapping(priv->keyword_mapping, "transient", JAGP_SYM_TRANSIENT);
	l_add_kw_mapping(priv->keyword_mapping, "byte", JAGP_SYM_BYTE);
	l_add_kw_mapping(priv->keyword_mapping, "short", JAGP_SYM_SHORT);
	l_add_kw_mapping(priv->keyword_mapping, "int", JAGP_SYM_INT);
	l_add_kw_mapping(priv->keyword_mapping, "long", JAGP_SYM_LONG);
	l_add_kw_mapping(priv->keyword_mapping, "char", JAGP_SYM_CHAR);
	l_add_kw_mapping(priv->keyword_mapping, "extends", JAGP_SYM_EXTENDS);
	l_add_kw_mapping(priv->keyword_mapping, "instanceof", JAGP_SYM_INSTANCEOF);
	l_add_kw_mapping(priv->keyword_mapping, "public", JAGP_SYM_PUBLIC);
	l_add_kw_mapping(priv->keyword_mapping, "try", JAGP_SYM_TRY);
	l_add_kw_mapping(priv->keyword_mapping, "case", JAGP_SYM_CASE);
	l_add_kw_mapping(priv->keyword_mapping, "final", JAGP_SYM_FINAL);
	l_add_kw_mapping(priv->keyword_mapping, "finally", JAGP_SYM_FINALLY);
	l_add_kw_mapping(priv->keyword_mapping, "interface", JAGP_SYM_INTERFACE);
	l_add_kw_mapping(priv->keyword_mapping, "return", JAGP_SYM_RETURN);
	l_add_kw_mapping(priv->keyword_mapping, "void", JAGP_SYM_VOID);
	l_add_kw_mapping(priv->keyword_mapping, "catch", JAGP_SYM_CATCH);
	l_add_kw_mapping(priv->keyword_mapping, "float", JAGP_SYM_FLOAT);
	l_add_kw_mapping(priv->keyword_mapping, "volatile", JAGP_SYM_VOLATILE);
	l_add_kw_mapping(priv->keyword_mapping, "for", JAGP_SYM_FOR);
	l_add_kw_mapping(priv->keyword_mapping, "native", JAGP_SYM_NATIVE);
	l_add_kw_mapping(priv->keyword_mapping, "static", JAGP_SYM_STATIC);
	l_add_kw_mapping(priv->keyword_mapping, "while", JAGP_SYM_WHILE);
	l_add_kw_mapping(priv->keyword_mapping, "class", JAGP_SYM_CLASS);
	l_add_kw_mapping(priv->keyword_mapping, "new", JAGP_SYM_NEW);
	l_add_kw_mapping(priv->keyword_mapping, "super", JAGP_SYM_SUPER);
	l_add_kw_mapping(priv->keyword_mapping, "const", JAGP_SYM_CONST);
	l_add_kw_mapping(priv->keyword_mapping, "null", JAGP_SYM_NULL);
	l_add_kw_mapping(priv->keyword_mapping, "switch", JAGP_SYM_SWITCH);
	l_add_kw_mapping(priv->keyword_mapping, "continue", JAGP_SYM_CONTINUE);
	l_add_kw_mapping(priv->keyword_mapping, "goto", JAGP_SYM_GOTO);
	l_add_kw_mapping(priv->keyword_mapping, "synchronized", JAGP_SYM_SYNCHRONIZED);
	l_add_kw_mapping(priv->keyword_mapping, "default", JAGP_SYM_DEFAULT);
	l_add_kw_mapping(priv->keyword_mapping, "if", JAGP_SYM_IF);
	l_add_kw_mapping(priv->keyword_mapping, "this", JAGP_SYM_THIS);
	l_add_kw_mapping(priv->keyword_mapping, "strictfp", JAGP_SYM_STRICTFP);
	l_add_kw_mapping(priv->keyword_mapping, "assert", JAGP_SYM_ASSERT);
	l_add_kw_mapping(priv->keyword_mapping, "enum", JAGP_SYM_ENUM);

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

