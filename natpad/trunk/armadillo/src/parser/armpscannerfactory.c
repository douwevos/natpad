/*
   File:    armpscannerfactory.c
   Project: armadillo
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

#include "armpscannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ArmPScannerFactory"
#include <logging/catlog.h>

struct _ArmPScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ArmPScannerFactory, armp_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ArmPScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void armp_scanner_factory_class_init(ArmPScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void armp_scanner_factory_init(ArmPScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ArmPScannerFactory *instance = ARMP_SCANNER_FACTORY(object);
	ArmPScannerFactoryPrivate *priv = armp_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->connected_symbols);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(armp_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(armp_scanner_factory_parent_class)->finalize(object);
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

ArmPScannerFactory *armp_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	ArmPScannerFactory *result = g_object_new(ARMP_TYPE_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	ArmPScannerFactoryPrivate *priv = armp_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, ARMP_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, ARMP_SYM_ERROR, symbol_provider, "⊗");
	l_connect_symbol(connected_syms, ARMP_SYM_IDENTIFIER, symbol_provider, "IDENTIFIER");
	l_connect_symbol(connected_syms, ARMP_SYM_I_CONSTANT, symbol_provider, "I_CONSTANT");
	l_connect_symbol(connected_syms, ARMP_SYM_F_CONSTANT, symbol_provider, "F_CONSTANT");
	l_connect_symbol(connected_syms, ARMP_SYM_STRING_LITERAL, symbol_provider, "STRING_LITERAL");
	l_connect_symbol(connected_syms, ARMP_SYM_FUNC_NAME, symbol_provider, "FUNC_NAME");
	l_connect_symbol(connected_syms, ARMP_SYM_SIZEOF, symbol_provider, "SIZEOF");
	l_connect_symbol(connected_syms, ARMP_SYM_PTR_OP, symbol_provider, "PTR_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_INC_OP, symbol_provider, "INC_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_DEC_OP, symbol_provider, "DEC_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_LEFT_OP, symbol_provider, "LEFT_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_RIGHT_OP, symbol_provider, "RIGHT_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_LE_OP, symbol_provider, "LE_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_GE_OP, symbol_provider, "GE_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_EQ_OP, symbol_provider, "EQ_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_NE_OP, symbol_provider, "NE_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_AND_OP, symbol_provider, "AND_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_OR_OP, symbol_provider, "OR_OP");
	l_connect_symbol(connected_syms, ARMP_SYM_MUL_ASSIGN, symbol_provider, "MUL_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_DIV_ASSIGN, symbol_provider, "DIV_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_MOD_ASSIGN, symbol_provider, "MOD_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_ADD_ASSIGN, symbol_provider, "ADD_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_SUB_ASSIGN, symbol_provider, "SUB_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_LEFT_ASSIGN, symbol_provider, "LEFT_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_RIGHT_ASSIGN, symbol_provider, "RIGHT_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_AND_ASSIGN, symbol_provider, "AND_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_XOR_ASSIGN, symbol_provider, "XOR_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_OR_ASSIGN, symbol_provider, "OR_ASSIGN");
	l_connect_symbol(connected_syms, ARMP_SYM_TYPEDEF_NAME, symbol_provider, "TYPEDEF_NAME");
	l_connect_symbol(connected_syms, ARMP_SYM_ENUMERATION_CONSTANT, symbol_provider, "ENUMERATION_CONSTANT");
	l_connect_symbol(connected_syms, ARMP_SYM_TYPEDEF, symbol_provider, "TYPEDEF");
	l_connect_symbol(connected_syms, ARMP_SYM_EXTERN, symbol_provider, "EXTERN");
	l_connect_symbol(connected_syms, ARMP_SYM_STATIC, symbol_provider, "STATIC");
	l_connect_symbol(connected_syms, ARMP_SYM_AUTO, symbol_provider, "AUTO");
	l_connect_symbol(connected_syms, ARMP_SYM_REGISTER, symbol_provider, "REGISTER");
	l_connect_symbol(connected_syms, ARMP_SYM_INLINE, symbol_provider, "INLINE");
	l_connect_symbol(connected_syms, ARMP_SYM_CONST, symbol_provider, "CONST");
	l_connect_symbol(connected_syms, ARMP_SYM_RESTRICT, symbol_provider, "RESTRICT");
	l_connect_symbol(connected_syms, ARMP_SYM_VOLATILE, symbol_provider, "VOLATILE");
	l_connect_symbol(connected_syms, ARMP_SYM_BOOL, symbol_provider, "BOOL");
	l_connect_symbol(connected_syms, ARMP_SYM_CHAR, symbol_provider, "CHAR");
	l_connect_symbol(connected_syms, ARMP_SYM_SHORT, symbol_provider, "SHORT");
	l_connect_symbol(connected_syms, ARMP_SYM_INT, symbol_provider, "INT");
	l_connect_symbol(connected_syms, ARMP_SYM_LONG, symbol_provider, "LONG");
	l_connect_symbol(connected_syms, ARMP_SYM_SIGNED, symbol_provider, "SIGNED");
	l_connect_symbol(connected_syms, ARMP_SYM_UNSIGNED, symbol_provider, "UNSIGNED");
	l_connect_symbol(connected_syms, ARMP_SYM_FLOAT, symbol_provider, "FLOAT");
	l_connect_symbol(connected_syms, ARMP_SYM_DOUBLE, symbol_provider, "DOUBLE");
	l_connect_symbol(connected_syms, ARMP_SYM_VOID, symbol_provider, "VOID");
	l_connect_symbol(connected_syms, ARMP_SYM_COMPLEX, symbol_provider, "COMPLEX");
	l_connect_symbol(connected_syms, ARMP_SYM_IMAGINARY, symbol_provider, "IMAGINARY");
	l_connect_symbol(connected_syms, ARMP_SYM_STRUCT, symbol_provider, "STRUCT");
	l_connect_symbol(connected_syms, ARMP_SYM_UNION, symbol_provider, "UNION");
	l_connect_symbol(connected_syms, ARMP_SYM_ENUM, symbol_provider, "ENUM");
	l_connect_symbol(connected_syms, ARMP_SYM_ELLIPSIS, symbol_provider, "ELLIPSIS");
	l_connect_symbol(connected_syms, ARMP_SYM_CASE, symbol_provider, "CASE");
	l_connect_symbol(connected_syms, ARMP_SYM_DEFAULT, symbol_provider, "DEFAULT");
	l_connect_symbol(connected_syms, ARMP_SYM_IF, symbol_provider, "IF");
	l_connect_symbol(connected_syms, ARMP_SYM_ELSE, symbol_provider, "ELSE");
	l_connect_symbol(connected_syms, ARMP_SYM_SWITCH, symbol_provider, "SWITCH");
	l_connect_symbol(connected_syms, ARMP_SYM_WHILE, symbol_provider, "WHILE");
	l_connect_symbol(connected_syms, ARMP_SYM_DO, symbol_provider, "DO");
	l_connect_symbol(connected_syms, ARMP_SYM_FOR, symbol_provider, "FOR");
	l_connect_symbol(connected_syms, ARMP_SYM_GOTO, symbol_provider, "GOTO");
	l_connect_symbol(connected_syms, ARMP_SYM_CONTINUE, symbol_provider, "CONTINUE");
	l_connect_symbol(connected_syms, ARMP_SYM_BREAK, symbol_provider, "BREAK");
	l_connect_symbol(connected_syms, ARMP_SYM_RETURN, symbol_provider, "RETURN");
	l_connect_symbol(connected_syms, ARMP_SYM_ALIGNAS, symbol_provider, "ALIGNAS");
	l_connect_symbol(connected_syms, ARMP_SYM_ALIGNOF, symbol_provider, "ALIGNOF");
	l_connect_symbol(connected_syms, ARMP_SYM_ATOMIC, symbol_provider, "ATOMIC");
	l_connect_symbol(connected_syms, ARMP_SYM_GENERIC, symbol_provider, "GENERIC");
	l_connect_symbol(connected_syms, ARMP_SYM_NORETURN, symbol_provider, "NORETURN");
	l_connect_symbol(connected_syms, ARMP_SYM_STATIC_ASSERT, symbol_provider, "STATIC_ASSERT");
	l_connect_symbol(connected_syms, ARMP_SYM_THREAD_LOCAL, symbol_provider, "THREAD_LOCAL");
	l_connect_symbol(connected_syms, ARMP_SYM_L_BAR, symbol_provider, "L_BAR");
	l_connect_symbol(connected_syms, ARMP_SYM_L_SEMI, symbol_provider, "L_SEMI");
	l_connect_symbol(connected_syms, ARMP_SYM_L_COMMA, symbol_provider, "L_COMMA");
	l_connect_symbol(connected_syms, ARMP_SYM_L_DOT, symbol_provider, "L_DOT");
	l_connect_symbol(connected_syms, ARMP_SYM_L_COLON, symbol_provider, "L_COLON");
	l_connect_symbol(connected_syms, ARMP_SYM_L_QUESTIONMARK, symbol_provider, "L_QUESTIONMARK");
	l_connect_symbol(connected_syms, ARMP_SYM_L_IS, symbol_provider, "L_IS");
	l_connect_symbol(connected_syms, ARMP_SYM_L_AND, symbol_provider, "L_AND");
	l_connect_symbol(connected_syms, ARMP_SYM_L_MUL, symbol_provider, "L_MUL");
	l_connect_symbol(connected_syms, ARMP_SYM_L_PLUS, symbol_provider, "L_PLUS");
	l_connect_symbol(connected_syms, ARMP_SYM_L_MINUS, symbol_provider, "L_MINUS");
	l_connect_symbol(connected_syms, ARMP_SYM_L_LT, symbol_provider, "L_LT");
	l_connect_symbol(connected_syms, ARMP_SYM_L_GT, symbol_provider, "L_GT");
	l_connect_symbol(connected_syms, ARMP_SYM_L_NEG, symbol_provider, "L_NEG");
	l_connect_symbol(connected_syms, ARMP_SYM_L_NOT, symbol_provider, "L_NOT");
	l_connect_symbol(connected_syms, ARMP_SYM_L_DIV, symbol_provider, "L_DIV");
	l_connect_symbol(connected_syms, ARMP_SYM_L_MOD, symbol_provider, "L_MOD");
	l_connect_symbol(connected_syms, ARMP_SYM_L_XOR, symbol_provider, "L_XOR");
	l_connect_symbol(connected_syms, ARMP_SYM_L_LCUBRACE, symbol_provider, "L_LCUBRACE");
	l_connect_symbol(connected_syms, ARMP_SYM_L_RCUBRACE, symbol_provider, "L_RCUBRACE");
	l_connect_symbol(connected_syms, ARMP_SYM_L_LSQBRACE, symbol_provider, "L_LSQBRACE");
	l_connect_symbol(connected_syms, ARMP_SYM_L_RSQBRACE, symbol_provider, "L_RSQBRACE");
	l_connect_symbol(connected_syms, ARMP_SYM_L_LPAREN, symbol_provider, "L_LPAREN");
	l_connect_symbol(connected_syms, ARMP_SYM_L_RPAREN, symbol_provider, "L_RPAREN");



	CatStringWo *t_eol_comment = cat_string_wo_new_with("EOL_COMMENT");
	GroRunSymbol *sym_eol = grorun_symbol_new(FALSE, t_eol_comment, ARMP_SYM_EOL_COMMENT);
	cat_array_wo_append(connected_syms, (GObject *) sym_eol);
	cat_unref_ptr(sym_eol);
	cat_unref_ptr(t_eol_comment);

	CatStringWo *t_full_comment = cat_string_wo_new_with("FULL_COMMENT");
	GroRunSymbol *sym_full_comment = grorun_symbol_new(FALSE, t_full_comment, ARMP_SYM_FULL_COMMENT);
	cat_array_wo_append(connected_syms, (GObject *) sym_full_comment);
	cat_unref_ptr(sym_full_comment);
	cat_unref_ptr(t_full_comment);


	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	l_add_kw_mapping(priv->keyword_mapping, "string_literal", ARMP_SYM_STRING_LITERAL);
	l_add_kw_mapping(priv->keyword_mapping, "func_name", ARMP_SYM_FUNC_NAME);
	l_add_kw_mapping(priv->keyword_mapping, "sizeof", ARMP_SYM_SIZEOF);
	l_add_kw_mapping(priv->keyword_mapping, "typedef_name", ARMP_SYM_TYPEDEF_NAME);
	l_add_kw_mapping(priv->keyword_mapping, "enumeration_constant", ARMP_SYM_ENUMERATION_CONSTANT);
	l_add_kw_mapping(priv->keyword_mapping, "typedef", ARMP_SYM_TYPEDEF);
	l_add_kw_mapping(priv->keyword_mapping, "extern", ARMP_SYM_EXTERN);
	l_add_kw_mapping(priv->keyword_mapping, "static", ARMP_SYM_STATIC);
	l_add_kw_mapping(priv->keyword_mapping, "auto", ARMP_SYM_AUTO);
	l_add_kw_mapping(priv->keyword_mapping, "register", ARMP_SYM_REGISTER);
	l_add_kw_mapping(priv->keyword_mapping, "inline", ARMP_SYM_INLINE);
	l_add_kw_mapping(priv->keyword_mapping, "const", ARMP_SYM_CONST);
	l_add_kw_mapping(priv->keyword_mapping, "restrict", ARMP_SYM_RESTRICT);
	l_add_kw_mapping(priv->keyword_mapping, "volatile", ARMP_SYM_VOLATILE);
	l_add_kw_mapping(priv->keyword_mapping, "bool", ARMP_SYM_BOOL);
	l_add_kw_mapping(priv->keyword_mapping, "char", ARMP_SYM_CHAR);
	l_add_kw_mapping(priv->keyword_mapping, "short", ARMP_SYM_SHORT);
	l_add_kw_mapping(priv->keyword_mapping, "int", ARMP_SYM_INT);
	l_add_kw_mapping(priv->keyword_mapping, "long", ARMP_SYM_LONG);
	l_add_kw_mapping(priv->keyword_mapping, "signed", ARMP_SYM_SIGNED);
	l_add_kw_mapping(priv->keyword_mapping, "unsigned", ARMP_SYM_UNSIGNED);
	l_add_kw_mapping(priv->keyword_mapping, "float", ARMP_SYM_FLOAT);
	l_add_kw_mapping(priv->keyword_mapping, "double", ARMP_SYM_DOUBLE);
	l_add_kw_mapping(priv->keyword_mapping, "void", ARMP_SYM_VOID);
	l_add_kw_mapping(priv->keyword_mapping, "complex", ARMP_SYM_COMPLEX);
	l_add_kw_mapping(priv->keyword_mapping, "imaginary", ARMP_SYM_IMAGINARY);
	l_add_kw_mapping(priv->keyword_mapping, "struct", ARMP_SYM_STRUCT);
	l_add_kw_mapping(priv->keyword_mapping, "union", ARMP_SYM_UNION);
	l_add_kw_mapping(priv->keyword_mapping, "enum", ARMP_SYM_ENUM);
	l_add_kw_mapping(priv->keyword_mapping, "case", ARMP_SYM_CASE);
	l_add_kw_mapping(priv->keyword_mapping, "default", ARMP_SYM_DEFAULT);
	l_add_kw_mapping(priv->keyword_mapping, "if", ARMP_SYM_IF);
	l_add_kw_mapping(priv->keyword_mapping, "else", ARMP_SYM_ELSE);
	l_add_kw_mapping(priv->keyword_mapping, "switch", ARMP_SYM_SWITCH);
	l_add_kw_mapping(priv->keyword_mapping, "while", ARMP_SYM_WHILE);
	l_add_kw_mapping(priv->keyword_mapping, "do", ARMP_SYM_DO);
	l_add_kw_mapping(priv->keyword_mapping, "for", ARMP_SYM_FOR);
	l_add_kw_mapping(priv->keyword_mapping, "goto", ARMP_SYM_GOTO);
	l_add_kw_mapping(priv->keyword_mapping, "continue", ARMP_SYM_CONTINUE);
	l_add_kw_mapping(priv->keyword_mapping, "break", ARMP_SYM_BREAK);
	l_add_kw_mapping(priv->keyword_mapping, "return", ARMP_SYM_RETURN);
	l_add_kw_mapping(priv->keyword_mapping, "alignas", ARMP_SYM_ALIGNAS);
	l_add_kw_mapping(priv->keyword_mapping, "alignof", ARMP_SYM_ALIGNOF);
	l_add_kw_mapping(priv->keyword_mapping, "atomic", ARMP_SYM_ATOMIC);
	l_add_kw_mapping(priv->keyword_mapping, "generic", ARMP_SYM_GENERIC);
	l_add_kw_mapping(priv->keyword_mapping, "noreturn", ARMP_SYM_NORETURN);
	l_add_kw_mapping(priv->keyword_mapping, "static_assert", ARMP_SYM_STATIC_ASSERT);
	l_add_kw_mapping(priv->keyword_mapping, "thread_local", ARMP_SYM_THREAD_LOCAL);

	return result;
}

ArmPScanner *armp_scanner_factory_create_scanner(ArmPScannerFactory *factory, CatIUtf8Scanner *scanner) {
	ArmPScannerFactoryPrivate *priv = armp_scanner_factory_get_instance_private(factory);
	return armp_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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

