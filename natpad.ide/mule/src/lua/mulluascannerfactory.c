/*
   File:    mulluascannerfactory.c
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 9, 2016
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

#include "mulluascannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulLuaScannerFactory"
#include <logging/catlog.h>

struct _MulLuaScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	GroRunSymbol *connected_symbols[60];
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulLuaScannerFactory, mul_lua_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulLuaScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_lua_scanner_factory_class_init(MulLuaScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_lua_scanner_factory_init(MulLuaScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulLuaScannerFactory *instance = MUL_LUA_SCANNER_FACTORY(object);
	MulLuaScannerFactoryPrivate *priv = mul_lua_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
//	cat_unref_ptr(priv->connected_symbols[MUL_LUA_SYM_LINE_COMMENT]);
//	cat_unref_ptr(priv->connected_symbols[MUL_LUA_SYM_FULL_COMMENT]);
	G_OBJECT_CLASS(mul_lua_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_lua_scanner_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static GroRunSymbol *l_extract_symbol(GroRunISymbolProvider *symbol_provider, const char *name) {
	CatStringWo *cwname = cat_string_wo_new_with(name);
	GroRunSymbol *result = grorun_isymbol_provider_get_symbol_by_name(symbol_provider, cwname);
	cat_unref_ptr(cwname);
	return result;
}

static void l_add_kw_mapping(CatHashMapWo *map, const char *text, int code) {
	CatStringWo *k = cat_string_wo_new_with(text);
	CatInteger *v = cat_integer_new(code);
	cat_hash_map_wo_put(map, (GObject *) k, (GObject *) v);
	cat_unref_ptr(k);
	cat_unref_ptr(v);
}

MulLuaScannerFactory *mul_lua_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	MulLuaScannerFactory *result = g_object_new(MUL_TYPE_LUA_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	MulLuaScannerFactoryPrivate *priv = mul_lua_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);

	priv->connected_symbols[MUL_LUA_SYM_END_OF_INPUT] = l_extract_symbol(symbol_provider, "⟧");
	priv->connected_symbols[MUL_LUA_SYM_ERROR] = l_extract_symbol(symbol_provider, "⊗");
	priv->connected_symbols[MUL_LUA_SYM_LBRACK] = l_extract_symbol(symbol_provider, "LBRACK");
	priv->connected_symbols[MUL_LUA_SYM_RBRACK] = l_extract_symbol(symbol_provider, "RBRACK");
	priv->connected_symbols[MUL_LUA_SYM_LBRACE] = l_extract_symbol(symbol_provider, "LBRACE");
	priv->connected_symbols[MUL_LUA_SYM_RBRACE] = l_extract_symbol(symbol_provider, "RBRACE");
	priv->connected_symbols[MUL_LUA_SYM_KWAND] = l_extract_symbol(symbol_provider, "KWAND");
	priv->connected_symbols[MUL_LUA_SYM_KWBREAK] = l_extract_symbol(symbol_provider, "KWBREAK");
	priv->connected_symbols[MUL_LUA_SYM_KWDO] = l_extract_symbol(symbol_provider, "KWDO");
	priv->connected_symbols[MUL_LUA_SYM_KWELSE] = l_extract_symbol(symbol_provider, "KWELSE");
	priv->connected_symbols[MUL_LUA_SYM_KWELSEIF] = l_extract_symbol(symbol_provider, "KWELSEIF");
	priv->connected_symbols[MUL_LUA_SYM_KWEND] = l_extract_symbol(symbol_provider, "KWEND");
	priv->connected_symbols[MUL_LUA_SYM_KWFALSE] = l_extract_symbol(symbol_provider, "KWFALSE");
	priv->connected_symbols[MUL_LUA_SYM_KWFOR] = l_extract_symbol(symbol_provider, "KWFOR");
	priv->connected_symbols[MUL_LUA_SYM_KWFUNCTION] = l_extract_symbol(symbol_provider, "KWFUNCTION");
	priv->connected_symbols[MUL_LUA_SYM_KWIF] = l_extract_symbol(symbol_provider, "KWIF");
	priv->connected_symbols[MUL_LUA_SYM_KWIN] = l_extract_symbol(symbol_provider, "KWIN");
	priv->connected_symbols[MUL_LUA_SYM_KWLOCAL] = l_extract_symbol(symbol_provider, "KWLOCAL");
	priv->connected_symbols[MUL_LUA_SYM_KWNIL] = l_extract_symbol(symbol_provider, "KWNIL");
	priv->connected_symbols[MUL_LUA_SYM_KWNOT] = l_extract_symbol(symbol_provider, "KWNOT");
	priv->connected_symbols[MUL_LUA_SYM_KWOR] = l_extract_symbol(symbol_provider, "KWOR");
	priv->connected_symbols[MUL_LUA_SYM_KWREPEAT] = l_extract_symbol(symbol_provider, "KWREPEAT");
	priv->connected_symbols[MUL_LUA_SYM_KWRETURN] = l_extract_symbol(symbol_provider, "KWRETURN");
	priv->connected_symbols[MUL_LUA_SYM_KWTHEN] = l_extract_symbol(symbol_provider, "KWTHEN");
	priv->connected_symbols[MUL_LUA_SYM_KWTRUE] = l_extract_symbol(symbol_provider, "KWTRUE");
	priv->connected_symbols[MUL_LUA_SYM_KWUNTIL] = l_extract_symbol(symbol_provider, "KWUNTIL");
	priv->connected_symbols[MUL_LUA_SYM_KWWHILE] = l_extract_symbol(symbol_provider, "KWWHILE");
	priv->connected_symbols[MUL_LUA_SYM_KWCONCAT] = l_extract_symbol(symbol_provider, "KWCONCAT");
	priv->connected_symbols[MUL_LUA_SYM_ELLIPSES] = l_extract_symbol(symbol_provider, "ELLIPSES");
	priv->connected_symbols[MUL_LUA_SYM_EQ] = l_extract_symbol(symbol_provider, "EQ");
	priv->connected_symbols[MUL_LUA_SYM_GE] = l_extract_symbol(symbol_provider, "GE");
	priv->connected_symbols[MUL_LUA_SYM_LE] = l_extract_symbol(symbol_provider, "LE");
	priv->connected_symbols[MUL_LUA_SYM_NE] = l_extract_symbol(symbol_provider, "NE");
	priv->connected_symbols[MUL_LUA_SYM_OP_PLUS] = l_extract_symbol(symbol_provider, "OP_PLUS");
	priv->connected_symbols[MUL_LUA_SYM_OP_MINUS] = l_extract_symbol(symbol_provider, "OP_MINUS");
	priv->connected_symbols[MUL_LUA_SYM_OP_MUL] = l_extract_symbol(symbol_provider, "OP_MUL");
	priv->connected_symbols[MUL_LUA_SYM_OP_DIV] = l_extract_symbol(symbol_provider, "OP_DIV");
	priv->connected_symbols[MUL_LUA_SYM_OP_MOD] = l_extract_symbol(symbol_provider, "OP_MOD");
	priv->connected_symbols[MUL_LUA_SYM_OP_HASH] = l_extract_symbol(symbol_provider, "OP_HASH");
	priv->connected_symbols[MUL_LUA_SYM_OP_NEG] = l_extract_symbol(symbol_provider, "OP_NEG");
	priv->connected_symbols[MUL_LUA_SYM_SPECIAL] = l_extract_symbol(symbol_provider, "SPECIAL");
	priv->connected_symbols[MUL_LUA_SYM_SPECIAL_CONST] = l_extract_symbol(symbol_provider, "SPECIAL_CONST");
	priv->connected_symbols[MUL_LUA_SYM_SPECIAL_NUMBER] = l_extract_symbol(symbol_provider, "SPECIAL_NUMBER");
	priv->connected_symbols[MUL_LUA_SYM_OP_LT] = l_extract_symbol(symbol_provider, "OP_LT");
	priv->connected_symbols[MUL_LUA_SYM_OP_GT] = l_extract_symbol(symbol_provider, "OP_GT");
	priv->connected_symbols[MUL_LUA_SYM_NUMBER] = l_extract_symbol(symbol_provider, "NUMBER");
	priv->connected_symbols[MUL_LUA_SYM_STRING] = l_extract_symbol(symbol_provider, "STRING");
	priv->connected_symbols[MUL_LUA_SYM_IDENTIFIER] = l_extract_symbol(symbol_provider, "IDENTIFIER");
	priv->connected_symbols[MUL_LUA_SYM_UNARY_OPERATOR] = l_extract_symbol(symbol_provider, "UNARY_OPERATOR");
	priv->connected_symbols[MUL_LUA_SYM_SEMICOLON] = l_extract_symbol(symbol_provider, "SEMICOLON");
	priv->connected_symbols[MUL_LUA_SYM_OP_ASSIGN] = l_extract_symbol(symbol_provider, "OP_ASSIGN");
	priv->connected_symbols[MUL_LUA_SYM_COMMA] = l_extract_symbol(symbol_provider, "COMMA");
	priv->connected_symbols[MUL_LUA_SYM_COLON] = l_extract_symbol(symbol_provider, "COLON");
	priv->connected_symbols[MUL_LUA_SYM_DOT] = l_extract_symbol(symbol_provider, "DOT");
	priv->connected_symbols[MUL_LUA_SYM_LPAREN] = l_extract_symbol(symbol_provider, "LPAREN");
	priv->connected_symbols[MUL_LUA_SYM_RPAREN] = l_extract_symbol(symbol_provider, "RPAREN");


	CatStringWo *t_eol_comment = cat_string_wo_new_with("EOL_COMMENT");
	priv->connected_symbols[MUL_LUA_SYM_EOL_COMMENT] = grorun_symbol_new(FALSE, t_eol_comment, 1000);
	cat_unref_ptr(t_eol_comment);
//
//	CatStringWo *t_full_comment = cat_string_wo_new_with("FULL_COMMENT");
//	priv->connected_symbols[SPI_SYM_FULL_COMMENT] = grorun_symbol_new(FALSE, t_full_comment, 1001);
//	cat_unref_ptr(t_full_comment);



	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	l_add_kw_mapping(priv->keyword_mapping, "and", MUL_LUA_SYM_KWAND);
	l_add_kw_mapping(priv->keyword_mapping, "break", MUL_LUA_SYM_KWBREAK);
	l_add_kw_mapping(priv->keyword_mapping, "do", MUL_LUA_SYM_KWDO);
	l_add_kw_mapping(priv->keyword_mapping, "else", MUL_LUA_SYM_KWELSE);
	l_add_kw_mapping(priv->keyword_mapping, "elseif", MUL_LUA_SYM_KWELSEIF);
	l_add_kw_mapping(priv->keyword_mapping, "end", MUL_LUA_SYM_KWEND);
	l_add_kw_mapping(priv->keyword_mapping, "false", MUL_LUA_SYM_KWFALSE);
	l_add_kw_mapping(priv->keyword_mapping, "for", MUL_LUA_SYM_KWFOR);
	l_add_kw_mapping(priv->keyword_mapping, "function", MUL_LUA_SYM_KWFUNCTION);
	l_add_kw_mapping(priv->keyword_mapping, "if", MUL_LUA_SYM_KWIF);
	l_add_kw_mapping(priv->keyword_mapping, "in", MUL_LUA_SYM_KWIN);
	l_add_kw_mapping(priv->keyword_mapping, "local", MUL_LUA_SYM_KWLOCAL);
	l_add_kw_mapping(priv->keyword_mapping, "nil", MUL_LUA_SYM_KWNIL);
	l_add_kw_mapping(priv->keyword_mapping, "not", MUL_LUA_SYM_KWNOT);
	l_add_kw_mapping(priv->keyword_mapping, "or", MUL_LUA_SYM_KWOR);
	l_add_kw_mapping(priv->keyword_mapping, "repeat", MUL_LUA_SYM_KWREPEAT);
	l_add_kw_mapping(priv->keyword_mapping, "return", MUL_LUA_SYM_KWRETURN);
	l_add_kw_mapping(priv->keyword_mapping, "then", MUL_LUA_SYM_KWTHEN);
	l_add_kw_mapping(priv->keyword_mapping, "true", MUL_LUA_SYM_KWTRUE);
	l_add_kw_mapping(priv->keyword_mapping, "until", MUL_LUA_SYM_KWUNTIL);
	l_add_kw_mapping(priv->keyword_mapping, "while", MUL_LUA_SYM_KWWHILE);
//	l_add_kw_mapping(priv->keyword_mapping, "concat", MUL_LUA_SYM_KWCONCAT);

	return result;
}

MulLuaScanner *mul_lua_scanner_factory_create_scanner(MulLuaScannerFactory *factory, CatIUtf8Scanner *scanner) {
	MulLuaScannerFactoryPrivate *priv = mul_lua_scanner_factory_get_instance_private(factory);
	return mul_lua_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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
