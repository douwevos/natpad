/*
   File:    mulphpscannerfactory.c
   Project: mule
   Author:  Douwe Vos
   Date:    Dec 11, 2016
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

#include "mulphpscannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulPhpScannerFactory"
#include <logging/catlog.h>

struct _MulPhpScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulPhpScannerFactory, mul_php_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulPhpScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_php_scanner_factory_class_init(MulPhpScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_php_scanner_factory_init(MulPhpScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulPhpScannerFactory *instance = MUL_PHP_SCANNER_FACTORY(object);
	MulPhpScannerFactoryPrivate *priv = mul_php_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->connected_symbols);
	G_OBJECT_CLASS(mul_php_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_php_scanner_factory_parent_class)->finalize(object);
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


MulPhpScannerFactory *mul_php_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	MulPhpScannerFactory *result = g_object_new(MUL_PHP_TYPE_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	MulPhpScannerFactoryPrivate *priv = mul_php_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, MUL_PHP_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_ERROR, symbol_provider, "⊗");

	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INCLUDE, symbol_provider, "T_INCLUDE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INCLUDE_ONCE, symbol_provider, "T_INCLUDE_ONCE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_EVAL, symbol_provider, "T_EVAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_REQUIRE, symbol_provider, "T_REQUIRE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_REQUIRE_ONCE, symbol_provider, "T_REQUIRE_ONCE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_LOGICAL_OR, symbol_provider, "T_LOGICAL_OR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_LOGICAL_XOR, symbol_provider, "T_LOGICAL_XOR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_LOGICAL_AND, symbol_provider, "T_LOGICAL_AND");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INSTANCEOF, symbol_provider, "T_INSTANCEOF");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_NEW, symbol_provider, "T_NEW");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CLONE, symbol_provider, "T_CLONE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_EXIT, symbol_provider, "T_EXIT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IF, symbol_provider, "T_IF");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ELSEIF, symbol_provider, "T_ELSEIF");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ELSE, symbol_provider, "T_ELSE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ENDIF, symbol_provider, "T_ENDIF");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ECHO, symbol_provider, "T_ECHO");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DO, symbol_provider, "T_DO");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_WHILE, symbol_provider, "T_WHILE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ENDWHILE, symbol_provider, "T_ENDWHILE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_FOR, symbol_provider, "T_FOR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ENDFOR, symbol_provider, "T_ENDFOR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_FOREACH, symbol_provider, "T_FOREACH");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ENDFOREACH, symbol_provider, "T_ENDFOREACH");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DECLARE, symbol_provider, "T_DECLARE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ENDDECLARE, symbol_provider, "T_ENDDECLARE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_AS, symbol_provider, "T_AS");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_TRY, symbol_provider, "T_TRY");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CATCH, symbol_provider, "T_CATCH");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_FINALLY, symbol_provider, "T_FINALLY");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_THROW, symbol_provider, "T_THROW");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_USE, symbol_provider, "T_USE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INSTEADOF, symbol_provider, "T_INSTEADOF");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_GLOBAL, symbol_provider, "T_GLOBAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_VAR, symbol_provider, "T_VAR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_UNSET, symbol_provider, "T_UNSET");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ISSET, symbol_provider, "T_ISSET");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_EMPTY, symbol_provider, "T_EMPTY");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CONTINUE, symbol_provider, "T_CONTINUE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_GOTO, symbol_provider, "T_GOTO");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_FUNCTION, symbol_provider, "T_FUNCTION");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CONST, symbol_provider, "T_CONST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_RETURN, symbol_provider, "T_RETURN");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_PRINT, symbol_provider, "T_PRINT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_YIELD, symbol_provider, "T_YIELD");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_LIST, symbol_provider, "T_LIST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_SWITCH, symbol_provider, "T_SWITCH");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ENDSWITCH, symbol_provider, "T_ENDSWITCH");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CASE, symbol_provider, "T_CASE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DEFAULT, symbol_provider, "T_DEFAULT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_BREAK, symbol_provider, "T_BREAK");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ARRAY, symbol_provider, "T_ARRAY");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CALLABLE, symbol_provider, "T_CALLABLE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_EXTENDS, symbol_provider, "T_EXTENDS");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IMPLEMENTS, symbol_provider, "T_IMPLEMENTS");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_NAMESPACE, symbol_provider, "T_NAMESPACE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_TRAIT, symbol_provider, "T_TRAIT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INTERFACE, symbol_provider, "T_INTERFACE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CLASS, symbol_provider, "T_CLASS");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CLASS_C, symbol_provider, "T_CLASS_C");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_TRAIT_C, symbol_provider, "T_TRAIT_C");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_FUNC_C, symbol_provider, "T_FUNC_C");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_METHOD_C, symbol_provider, "T_METHOD_C");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_LINE, symbol_provider, "T_LINE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_FILE, symbol_provider, "T_FILE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DIR, symbol_provider, "T_DIR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_NS_C, symbol_provider, "T_NS_C");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_HALT_COMPILER, symbol_provider, "T_HALT_COMPILER");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_STATIC, symbol_provider, "T_STATIC");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ABSTRACT, symbol_provider, "T_ABSTRACT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_FINAL, symbol_provider, "T_FINAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_PRIVATE, symbol_provider, "T_PRIVATE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_PROTECTED, symbol_provider, "T_PROTECTED");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_PUBLIC, symbol_provider, "T_PUBLIC");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_STRING, symbol_provider, "T_STRING");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_NS_SEPARATOR, symbol_provider, "T_NS_SEPARATOR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_SEMI, symbol_provider, "SEMI");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_LCUBRACK, symbol_provider, "LCUBRACK");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_RCUBRACK, symbol_provider, "RCUBRACK");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_COMMA, symbol_provider, "COMMA");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_LPAREN, symbol_provider, "LPAREN");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_RPAREN, symbol_provider, "RPAREN");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_COLON, symbol_provider, "COLON");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_AMPERSAND, symbol_provider, "AMPERSAND");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_PLUS, symbol_provider, "OP_PLUS");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_MINUS, symbol_provider, "OP_MINUS");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INLINE_HTML, symbol_provider, "T_INLINE_HTML");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DOUBLE_ARROW, symbol_provider, "T_DOUBLE_ARROW");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_VARIABLE, symbol_provider, "T_VARIABLE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ELLIPSIS, symbol_provider, "T_ELLIPSIS");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_PAAMAYIM_NEKUDOTAYIM, symbol_provider, "T_PAAMAYIM_NEKUDOTAYIM");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_PLUS_EQUAL, symbol_provider, "T_PLUS_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_MINUS_EQUAL, symbol_provider, "T_MINUS_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_MUL_EQUAL, symbol_provider, "T_MUL_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DIV_EQUAL, symbol_provider, "T_DIV_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CONCAT_EQUAL, symbol_provider, "T_CONCAT_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_MOD_EQUAL, symbol_provider, "T_MOD_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_AND_EQUAL, symbol_provider, "T_AND_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_OR_EQUAL, symbol_provider, "T_OR_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_XOR_EQUAL, symbol_provider, "T_XOR_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_SL_EQUAL, symbol_provider, "T_SL_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_SR_EQUAL, symbol_provider, "T_SR_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_POW_EQUAL, symbol_provider, "T_POW_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INC, symbol_provider, "T_INC");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DEC, symbol_provider, "T_DEC");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_BOOLEAN_OR, symbol_provider, "T_BOOLEAN_OR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_BOOLEAN_AND, symbol_provider, "T_BOOLEAN_AND");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_SL, symbol_provider, "T_SL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ENCAPSED_AND_WHITESPACE, symbol_provider, "T_ENCAPSED_AND_WHITESPACE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_SR, symbol_provider, "T_SR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_POW, symbol_provider, "T_POW");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IS_IDENTICAL, symbol_provider, "T_IS_IDENTICAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_QUESTION_MARK, symbol_provider, "QUESTION_MARK");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_LSQBRACK, symbol_provider, "LSQBRACK");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_RSQBRACK, symbol_provider, "RSQBRACK");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DOLLAR_OPEN_CURLY_BRACES, symbol_provider, "T_DOLLAR_OPEN_CURLY_BRACES");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_OBJECT_OPERATOR, symbol_provider, "T_OBJECT_OPERATOR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CURLY_OPEN, symbol_provider, "T_CURLY_OPEN");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_NUM_STRING, symbol_provider, "T_NUM_STRING");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_STRING_VARNAME, symbol_provider, "T_STRING_VARNAME");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_START_HEREDOC, symbol_provider, "T_START_HEREDOC");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_END_HEREDOC, symbol_provider, "T_END_HEREDOC");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_CONSTANT_ENCAPSED_STRING, symbol_provider, "T_CONSTANT_ENCAPSED_STRING");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_LNUMBER, symbol_provider, "T_LNUMBER");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DNUMBER, symbol_provider, "T_DNUMBER");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IS_NOT_IDENTICAL, symbol_provider, "T_IS_NOT_IDENTICAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IS_EQUAL, symbol_provider, "T_IS_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IS_NOT_EQUAL, symbol_provider, "T_IS_NOT_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_SPACESHIP, symbol_provider, "T_SPACESHIP");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IS_SMALLER_OR_EQUAL, symbol_provider, "T_IS_SMALLER_OR_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_IS_GREATER_OR_EQUAL, symbol_provider, "T_IS_GREATER_OR_EQUAL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_COALESCE, symbol_provider, "T_COALESCE");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_INT_CAST, symbol_provider, "T_INT_CAST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_DOUBLE_CAST, symbol_provider, "T_DOUBLE_CAST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_STRING_CAST, symbol_provider, "T_STRING_CAST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_ARRAY_CAST, symbol_provider, "T_ARRAY_CAST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_OBJECT_CAST, symbol_provider, "T_OBJECT_CAST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_BOOL_CAST, symbol_provider, "T_BOOL_CAST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_UNSET_CAST, symbol_provider, "T_UNSET_CAST");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_T_YIELD_FROM, symbol_provider, "T_YIELD_FROM");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_DOLLAR, symbol_provider, "DOLLAR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_ASSIGN, symbol_provider, "OP_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_OR, symbol_provider, "OP_OR");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_POW, symbol_provider, "OP_POW");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_DOT, symbol_provider, "DOT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_MUL, symbol_provider, "OP_MUL");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_DIV, symbol_provider, "OP_DIV");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_MOD, symbol_provider, "OP_MOD");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_NOT, symbol_provider, "OP_NOT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_NEG, symbol_provider, "OP_NEG");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_LT, symbol_provider, "OP_LT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_OP_GT, symbol_provider, "OP_GT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_AT, symbol_provider, "AT");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_APASTOPH_BACKWARD, symbol_provider, "APASTOPH_BACKWARD");
	l_connect_symbol(connected_syms, MUL_PHP_SYM_DOUBLE_QUOTE, symbol_provider, "DOUBLE_QUOTE");


	CatStringWo *t_comment = cat_string_wo_new_with("COMMENT");
	GroRunSymbol *sym_eol = grorun_symbol_new(FALSE, t_comment, 1000);
	cat_array_wo_append(connected_syms, (GObject *) sym_eol);
	cat_unref_ptr(sym_eol);
	cat_unref_ptr(t_comment);

	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	l_add_kw_mapping(priv->keyword_mapping, "abstract", MUL_PHP_SYM_T_ABSTRACT);
	l_add_kw_mapping(priv->keyword_mapping, "array", MUL_PHP_SYM_T_ARRAY);
	l_add_kw_mapping(priv->keyword_mapping, "as", MUL_PHP_SYM_T_AS);
	l_add_kw_mapping(priv->keyword_mapping, "break", MUL_PHP_SYM_T_BREAK);
	l_add_kw_mapping(priv->keyword_mapping, "callable", MUL_PHP_SYM_T_CALLABLE);
	l_add_kw_mapping(priv->keyword_mapping, "case", MUL_PHP_SYM_T_CASE);
	l_add_kw_mapping(priv->keyword_mapping, "catch", MUL_PHP_SYM_T_CATCH);
	l_add_kw_mapping(priv->keyword_mapping, "class", MUL_PHP_SYM_T_CLASS);
	l_add_kw_mapping(priv->keyword_mapping, "__CLASS__", MUL_PHP_SYM_T_CLASS_C);
	l_add_kw_mapping(priv->keyword_mapping, "clone", MUL_PHP_SYM_T_CLONE);
	l_add_kw_mapping(priv->keyword_mapping, "const", MUL_PHP_SYM_T_CONST);
	l_add_kw_mapping(priv->keyword_mapping, "continue", MUL_PHP_SYM_T_CONTINUE);
	l_add_kw_mapping(priv->keyword_mapping, "declare", MUL_PHP_SYM_T_DECLARE);
	l_add_kw_mapping(priv->keyword_mapping, "default", MUL_PHP_SYM_T_DEFAULT);
	l_add_kw_mapping(priv->keyword_mapping, "__DIR__", MUL_PHP_SYM_T_DIR);
	l_add_kw_mapping(priv->keyword_mapping, "do", MUL_PHP_SYM_T_DO);
	l_add_kw_mapping(priv->keyword_mapping, "echo", MUL_PHP_SYM_T_ECHO);
	l_add_kw_mapping(priv->keyword_mapping, "else", MUL_PHP_SYM_T_ELSE);
	l_add_kw_mapping(priv->keyword_mapping, "elseif", MUL_PHP_SYM_T_ELSEIF);
	l_add_kw_mapping(priv->keyword_mapping, "empty", MUL_PHP_SYM_T_EMPTY);
	l_add_kw_mapping(priv->keyword_mapping, "enddeclare", MUL_PHP_SYM_T_ENDDECLARE);
	l_add_kw_mapping(priv->keyword_mapping, "endfor", MUL_PHP_SYM_T_ENDFOR);
	l_add_kw_mapping(priv->keyword_mapping, "endforeach", MUL_PHP_SYM_T_ENDFOREACH);
	l_add_kw_mapping(priv->keyword_mapping, "endif", MUL_PHP_SYM_T_ENDIF);
	l_add_kw_mapping(priv->keyword_mapping, "endswitch", MUL_PHP_SYM_T_ENDSWITCH);
	l_add_kw_mapping(priv->keyword_mapping, "endwhile", MUL_PHP_SYM_T_ENDWHILE);
	l_add_kw_mapping(priv->keyword_mapping, "eval", MUL_PHP_SYM_T_EVAL);
	l_add_kw_mapping(priv->keyword_mapping, "exit", MUL_PHP_SYM_T_EXIT);
	l_add_kw_mapping(priv->keyword_mapping, "die", MUL_PHP_SYM_T_EXIT);
	l_add_kw_mapping(priv->keyword_mapping, "extends", MUL_PHP_SYM_T_EXTENDS);
	l_add_kw_mapping(priv->keyword_mapping, "__FILE__", MUL_PHP_SYM_T_FILE);
	l_add_kw_mapping(priv->keyword_mapping, "final", MUL_PHP_SYM_T_FINAL);
	l_add_kw_mapping(priv->keyword_mapping, "finally", MUL_PHP_SYM_T_FINALLY);
	l_add_kw_mapping(priv->keyword_mapping, "for", MUL_PHP_SYM_T_FOR);
	l_add_kw_mapping(priv->keyword_mapping, "foreach", MUL_PHP_SYM_T_FOREACH);
	l_add_kw_mapping(priv->keyword_mapping, "function", MUL_PHP_SYM_T_FUNCTION);	// TODO cfunction ?
	l_add_kw_mapping(priv->keyword_mapping, "__FUNCTION__", MUL_PHP_SYM_T_FUNC_C);
	l_add_kw_mapping(priv->keyword_mapping, "global", MUL_PHP_SYM_T_GLOBAL);
	l_add_kw_mapping(priv->keyword_mapping, "goto", MUL_PHP_SYM_T_GOTO);
	l_add_kw_mapping(priv->keyword_mapping, "__halt_compiler", MUL_PHP_SYM_T_HALT_COMPILER);
	l_add_kw_mapping(priv->keyword_mapping, "if", MUL_PHP_SYM_T_IF);
	l_add_kw_mapping(priv->keyword_mapping, "implements", MUL_PHP_SYM_T_IMPLEMENTS);
	l_add_kw_mapping(priv->keyword_mapping, "include", MUL_PHP_SYM_T_INCLUDE);
	l_add_kw_mapping(priv->keyword_mapping, "include_once", MUL_PHP_SYM_T_INCLUDE_ONCE);
	l_add_kw_mapping(priv->keyword_mapping, "instanceof", MUL_PHP_SYM_T_INSTANCEOF);
	l_add_kw_mapping(priv->keyword_mapping, "insteadof", MUL_PHP_SYM_T_INSTEADOF);
	l_add_kw_mapping(priv->keyword_mapping, "interface", MUL_PHP_SYM_T_INTERFACE);
	l_add_kw_mapping(priv->keyword_mapping, "isset", MUL_PHP_SYM_T_ISSET);
	l_add_kw_mapping(priv->keyword_mapping, "__LINE__", MUL_PHP_SYM_T_LINE);
	l_add_kw_mapping(priv->keyword_mapping, "list", MUL_PHP_SYM_T_LIST);
	l_add_kw_mapping(priv->keyword_mapping, "and", MUL_PHP_SYM_T_LOGICAL_AND);
	l_add_kw_mapping(priv->keyword_mapping, "or", MUL_PHP_SYM_T_LOGICAL_OR);
	l_add_kw_mapping(priv->keyword_mapping, "xor", MUL_PHP_SYM_T_LOGICAL_XOR);
	l_add_kw_mapping(priv->keyword_mapping, "__METHOD__", MUL_PHP_SYM_T_METHOD_C);
	l_add_kw_mapping(priv->keyword_mapping, "namespace", MUL_PHP_SYM_T_NAMESPACE);
	l_add_kw_mapping(priv->keyword_mapping, "__NAMESPACE__", MUL_PHP_SYM_T_NS_C);
	l_add_kw_mapping(priv->keyword_mapping, "new", MUL_PHP_SYM_T_NEW);
	l_add_kw_mapping(priv->keyword_mapping, "print", MUL_PHP_SYM_T_PRINT);
	l_add_kw_mapping(priv->keyword_mapping, "private", MUL_PHP_SYM_T_PRIVATE);
	l_add_kw_mapping(priv->keyword_mapping, "public", MUL_PHP_SYM_T_PUBLIC);
	l_add_kw_mapping(priv->keyword_mapping, "protected", MUL_PHP_SYM_T_PROTECTED);
	l_add_kw_mapping(priv->keyword_mapping, "require", MUL_PHP_SYM_T_REQUIRE);
	l_add_kw_mapping(priv->keyword_mapping, "require_once", MUL_PHP_SYM_T_REQUIRE_ONCE);
	l_add_kw_mapping(priv->keyword_mapping, "return", MUL_PHP_SYM_T_RETURN);
	l_add_kw_mapping(priv->keyword_mapping, "static", MUL_PHP_SYM_T_STATIC);
	l_add_kw_mapping(priv->keyword_mapping, "switch", MUL_PHP_SYM_T_SWITCH);
	l_add_kw_mapping(priv->keyword_mapping, "throw", MUL_PHP_SYM_T_THROW);
	l_add_kw_mapping(priv->keyword_mapping, "trait", MUL_PHP_SYM_T_TRAIT);
	l_add_kw_mapping(priv->keyword_mapping, "__TRAIT__", MUL_PHP_SYM_T_TRAIT_C);
	l_add_kw_mapping(priv->keyword_mapping, "try", MUL_PHP_SYM_T_TRY);
	l_add_kw_mapping(priv->keyword_mapping, "unset", MUL_PHP_SYM_T_UNSET);
	l_add_kw_mapping(priv->keyword_mapping, "use", MUL_PHP_SYM_T_USE);
	l_add_kw_mapping(priv->keyword_mapping, "var", MUL_PHP_SYM_T_VAR);
	l_add_kw_mapping(priv->keyword_mapping, "while", MUL_PHP_SYM_T_WHILE);
	l_add_kw_mapping(priv->keyword_mapping, "yield", MUL_PHP_SYM_T_YIELD);



	l_add_kw_mapping(priv->keyword_mapping, "string", MUL_PHP_SYM_T_STRING);
	l_add_kw_mapping(priv->keyword_mapping, "inline_html", MUL_PHP_SYM_T_INLINE_HTML);
	l_add_kw_mapping(priv->keyword_mapping, "ellipsis", MUL_PHP_SYM_T_ELLIPSIS);
	l_add_kw_mapping(priv->keyword_mapping, "paamayim_nekudotayim", MUL_PHP_SYM_T_PAAMAYIM_NEKUDOTAYIM);
	l_add_kw_mapping(priv->keyword_mapping, "object_operator", MUL_PHP_SYM_T_OBJECT_OPERATOR);
	l_add_kw_mapping(priv->keyword_mapping, "string_varname", MUL_PHP_SYM_T_STRING_VARNAME);
	l_add_kw_mapping(priv->keyword_mapping, "start_heredoc", MUL_PHP_SYM_T_START_HEREDOC);
	l_add_kw_mapping(priv->keyword_mapping, "end_heredoc", MUL_PHP_SYM_T_END_HEREDOC);
	l_add_kw_mapping(priv->keyword_mapping, "constant_encapsed_string", MUL_PHP_SYM_T_CONSTANT_ENCAPSED_STRING);
	l_add_kw_mapping(priv->keyword_mapping, "is_not_identical", MUL_PHP_SYM_T_IS_NOT_IDENTICAL);
	l_add_kw_mapping(priv->keyword_mapping, "is_equal", MUL_PHP_SYM_T_IS_EQUAL);
	l_add_kw_mapping(priv->keyword_mapping, "is_not_equal", MUL_PHP_SYM_T_IS_NOT_EQUAL);
	l_add_kw_mapping(priv->keyword_mapping, "spaceship", MUL_PHP_SYM_T_SPACESHIP);
	l_add_kw_mapping(priv->keyword_mapping, "is_smaller_or_equal", MUL_PHP_SYM_T_IS_SMALLER_OR_EQUAL);
	l_add_kw_mapping(priv->keyword_mapping, "is_greater_or_equal", MUL_PHP_SYM_T_IS_GREATER_OR_EQUAL);
	l_add_kw_mapping(priv->keyword_mapping, "coalesce", MUL_PHP_SYM_T_COALESCE);
	l_add_kw_mapping(priv->keyword_mapping, "yield_from", MUL_PHP_SYM_T_YIELD_FROM);

	return result;
}


MulPhpScanner *mul_php_scanner_factory_create_scanner(MulPhpScannerFactory *factory, CatIUtf8Scanner *scanner) {
	MulPhpScannerFactoryPrivate *priv = mul_php_scanner_factory_get_instance_private(factory);
	return mul_php_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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
