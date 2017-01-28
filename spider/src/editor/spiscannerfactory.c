/*
   File:    spiscannerfactory.c
   Project: spider
   Author:  Douwe Vos
   Date:    Nov 7, 2016
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

#include "spiscannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SpiScannerFactory"
#include <logging/catlog.h>

struct _SpiScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	GroRunSymbol *connected_symbols[86];
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(SpiScannerFactory, spi_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(SpiScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void spi_scanner_factory_class_init(SpiScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void spi_scanner_factory_init(SpiScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	SpiScannerFactory *instance = SPI_SCANNER_FACTORY(object);
	SpiScannerFactoryPrivate *priv = spi_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->connected_symbols[SPI_SYM_LINE_COMMENT]);
	cat_unref_ptr(priv->connected_symbols[SPI_SYM_FULL_COMMENT]);

	G_OBJECT_CLASS(spi_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(spi_scanner_factory_parent_class)->finalize(object);
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


SpiScannerFactory *spi_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	SpiScannerFactory *result = g_object_new(SPI_TYPE_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	SpiScannerFactoryPrivate *priv = spi_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);

	priv->connected_symbols[SPI_SYM_END_OF_INPUT] = l_extract_symbol(symbol_provider, "⟧");

	priv->connected_symbols[SPI_SYM_KW_FUNCTION] = l_extract_symbol(symbol_provider, "FUNCTION");
	priv->connected_symbols[SPI_SYM_KW_VAR] = l_extract_symbol(symbol_provider, "VAR");
	priv->connected_symbols[SPI_SYM_KW_RETURN] = l_extract_symbol(symbol_provider, "RETURN");
	priv->connected_symbols[SPI_SYM_KW_IF] = l_extract_symbol(symbol_provider, "IF");
	priv->connected_symbols[SPI_SYM_KW_ELSE] = l_extract_symbol(symbol_provider, "ELSE");
	priv->connected_symbols[SPI_SYM_KW_THIS] = l_extract_symbol(symbol_provider, "THIS");
	priv->connected_symbols[SPI_SYM_KW_FOR] = l_extract_symbol(symbol_provider, "FOR");
	priv->connected_symbols[SPI_SYM_ID] = l_extract_symbol(symbol_provider, "ID");
	priv->connected_symbols[SPI_SYM_KW_NULL] = l_extract_symbol(symbol_provider, "NULL");
	priv->connected_symbols[SPI_SYM_KW_TRUE] = l_extract_symbol(symbol_provider, "TRUE");
	priv->connected_symbols[SPI_SYM_KW_FALSE] = l_extract_symbol(symbol_provider, "FALSE");
	priv->connected_symbols[SPI_SYM_NUMBER] = l_extract_symbol(symbol_provider, "NUMBER");
	priv->connected_symbols[SPI_SYM_STRING] = l_extract_symbol(symbol_provider, "STRING");
	priv->connected_symbols[SPI_SYM_LPAREN] = l_extract_symbol(symbol_provider, "LPAREN");
	priv->connected_symbols[SPI_SYM_RPAREN] = l_extract_symbol(symbol_provider, "RPAREN");
	priv->connected_symbols[SPI_SYM_LBRACE] = l_extract_symbol(symbol_provider, "LBRACE");
	priv->connected_symbols[SPI_SYM_RBRACE] = l_extract_symbol(symbol_provider, "RBRACE");
	priv->connected_symbols[SPI_SYM_SEMI] = l_extract_symbol(symbol_provider, "SEMI");
	priv->connected_symbols[SPI_SYM_COMMA] = l_extract_symbol(symbol_provider, "COMMA");
	priv->connected_symbols[SPI_SYM_ASS_IS] = l_extract_symbol(symbol_provider, "ASS_IS");
	priv->connected_symbols[SPI_SYM_QUESTION] = l_extract_symbol(symbol_provider, "QUESTION");
	priv->connected_symbols[SPI_SYM_COLON] = l_extract_symbol(symbol_provider, "COLON");
	priv->connected_symbols[SPI_SYM_OP_LOR] = l_extract_symbol(symbol_provider, "OP_LOR");
	priv->connected_symbols[SPI_SYM_OP_LAND] = l_extract_symbol(symbol_provider, "OP_LAND");
	priv->connected_symbols[SPI_SYM_OP_BOR] = l_extract_symbol(symbol_provider, "OP_BOR");
	priv->connected_symbols[SPI_SYM_OP_BXOR] = l_extract_symbol(symbol_provider, "OP_BXOR");
	priv->connected_symbols[SPI_SYM_OP_BAND] = l_extract_symbol(symbol_provider, "OP_BAND");
	priv->connected_symbols[SPI_SYM_KW_DELETE] = l_extract_symbol(symbol_provider, "DELETE");
	priv->connected_symbols[SPI_SYM_KW_VOID] = l_extract_symbol(symbol_provider, "VOID");
	priv->connected_symbols[SPI_SYM_KW_TYPEOF] = l_extract_symbol(symbol_provider, "TYPEOF");
	priv->connected_symbols[SPI_SYM_OP_INC] = l_extract_symbol(symbol_provider, "OP_INC");
	priv->connected_symbols[SPI_SYM_OP_DEC] = l_extract_symbol(symbol_provider, "OP_DEC");
	priv->connected_symbols[SPI_SYM_OP_ADD] = l_extract_symbol(symbol_provider, "OP_ADD");
	priv->connected_symbols[SPI_SYM_OP_SUB] = l_extract_symbol(symbol_provider, "OP_SUB");
	priv->connected_symbols[SPI_SYM_OP_NEG] = l_extract_symbol(symbol_provider, "OP_NEG");
	priv->connected_symbols[SPI_SYM_OP_NOT] = l_extract_symbol(symbol_provider, "OP_NOT");
	priv->connected_symbols[SPI_SYM_OP_MUL] = l_extract_symbol(symbol_provider, "OP_MUL");
	priv->connected_symbols[SPI_SYM_OP_DIV] = l_extract_symbol(symbol_provider, "OP_DIV");
	priv->connected_symbols[SPI_SYM_OP_MOD] = l_extract_symbol(symbol_provider, "OP_MOD");
	priv->connected_symbols[SPI_SYM_OP_SHL] = l_extract_symbol(symbol_provider, "OP_SHL");
	priv->connected_symbols[SPI_SYM_OP_SHR] = l_extract_symbol(symbol_provider, "OP_SHR");
	priv->connected_symbols[SPI_SYM_OP_SHRR] = l_extract_symbol(symbol_provider, "OP_SHRR");
	priv->connected_symbols[SPI_SYM_OP_LT] = l_extract_symbol(symbol_provider, "OP_LT");
	priv->connected_symbols[SPI_SYM_OP_GT] = l_extract_symbol(symbol_provider, "OP_GT");
	priv->connected_symbols[SPI_SYM_OP_LE] = l_extract_symbol(symbol_provider, "OP_LE");
	priv->connected_symbols[SPI_SYM_OP_GE] = l_extract_symbol(symbol_provider, "OP_GE");
	priv->connected_symbols[SPI_SYM_KW_INSTANCE_OF] = l_extract_symbol(symbol_provider, "INSTANCE_OF");
	priv->connected_symbols[SPI_SYM_KW_IN] = l_extract_symbol(symbol_provider, "IN");
	priv->connected_symbols[SPI_SYM_OP_EQ] = l_extract_symbol(symbol_provider, "OP_EQ");
	priv->connected_symbols[SPI_SYM_OP_NE] = l_extract_symbol(symbol_provider, "OP_NE");
	priv->connected_symbols[SPI_SYM_OP_TEQ] = l_extract_symbol(symbol_provider, "OP_TEQ");
	priv->connected_symbols[SPI_SYM_OP_TNE] = l_extract_symbol(symbol_provider, "OP_TNE");
	priv->connected_symbols[SPI_SYM_OP_ASS_MUL] = l_extract_symbol(symbol_provider, "OP_ASS_MUL");
	priv->connected_symbols[SPI_SYM_OP_ASS_DIV] = l_extract_symbol(symbol_provider, "OP_ASS_DIV");
	priv->connected_symbols[SPI_SYM_OP_ASS_MOD] = l_extract_symbol(symbol_provider, "OP_ASS_MOD");
	priv->connected_symbols[SPI_SYM_OP_ASS_ADD] = l_extract_symbol(symbol_provider, "OP_ASS_ADD");
	priv->connected_symbols[SPI_SYM_OP_ASS_SUB] = l_extract_symbol(symbol_provider, "OP_ASS_SUB");
	priv->connected_symbols[SPI_SYM_OP_ASS_SHL] = l_extract_symbol(symbol_provider, "OP_ASS_SHL");
	priv->connected_symbols[SPI_SYM_OP_ASS_SHR] = l_extract_symbol(symbol_provider, "OP_ASS_SHR");
	priv->connected_symbols[SPI_SYM_OP_ASS_SHRR] = l_extract_symbol(symbol_provider, "OP_ASS_SHRR");
	priv->connected_symbols[SPI_SYM_OP_ASS_AND] = l_extract_symbol(symbol_provider, "OP_ASS_AND");
	priv->connected_symbols[SPI_SYM_OP_ASS_XOR] = l_extract_symbol(symbol_provider, "OP_ASS_XOR");
	priv->connected_symbols[SPI_SYM_OP_ASS_OR] = l_extract_symbol(symbol_provider, "OP_ASS_OR");
	priv->connected_symbols[SPI_SYM_KW_NEW] = l_extract_symbol(symbol_provider, "NEW");
	priv->connected_symbols[SPI_SYM_LBRACK] = l_extract_symbol(symbol_provider, "LBRACK");
	priv->connected_symbols[SPI_SYM_RBRACK] = l_extract_symbol(symbol_provider, "RBRACK");
	priv->connected_symbols[SPI_SYM_DOT] = l_extract_symbol(symbol_provider, "DOT");
	priv->connected_symbols[SPI_SYM_REGULAR_EXPRESSION] = l_extract_symbol(symbol_provider, "REGULAR_EXPRESSION");
	priv->connected_symbols[SPI_SYM_KW_SWITCH] = l_extract_symbol(symbol_provider, "SWITCH");
	priv->connected_symbols[SPI_SYM_KW_CASE] = l_extract_symbol(symbol_provider, "CASE");
	priv->connected_symbols[SPI_SYM_KW_DEFAULT] = l_extract_symbol(symbol_provider, "DEFAULT");
	priv->connected_symbols[SPI_SYM_KW_DO] = l_extract_symbol(symbol_provider, "DO");
	priv->connected_symbols[SPI_SYM_KW_WHILE] = l_extract_symbol(symbol_provider, "WHILE");
	priv->connected_symbols[SPI_SYM_KW_WITH] = l_extract_symbol(symbol_provider, "WITH");
	priv->connected_symbols[SPI_SYM_KW_CONTINUE] = l_extract_symbol(symbol_provider, "CONTINUE");
	priv->connected_symbols[SPI_SYM_KW_BREAK] = l_extract_symbol(symbol_provider, "BREAK");
	priv->connected_symbols[SPI_SYM_KW_THROW] = l_extract_symbol(symbol_provider, "THROW");
	priv->connected_symbols[SPI_SYM_KW_TRY] = l_extract_symbol(symbol_provider, "TRY");
	priv->connected_symbols[SPI_SYM_KW_CATCH] = l_extract_symbol(symbol_provider, "CATCH");
	priv->connected_symbols[SPI_SYM_KW_FINALLY] = l_extract_symbol(symbol_provider, "FINALLY");
	CatStringWo *t_line_comment = cat_string_wo_new_with("LINE_COMMENT");
	priv->connected_symbols[SPI_SYM_LINE_COMMENT] = grorun_symbol_new(FALSE, t_line_comment, 1000);
	cat_unref_ptr(t_line_comment);

	CatStringWo *t_full_comment = cat_string_wo_new_with("FULL_COMMENT");
	priv->connected_symbols[SPI_SYM_FULL_COMMENT] = grorun_symbol_new(FALSE, t_full_comment, 1001);
	cat_unref_ptr(t_full_comment);



	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	l_add_kw_mapping(priv->keyword_mapping, "function", SPI_SYM_KW_FUNCTION);
	l_add_kw_mapping(priv->keyword_mapping, "var", SPI_SYM_KW_VAR);
	l_add_kw_mapping(priv->keyword_mapping, "return", SPI_SYM_KW_RETURN);
	l_add_kw_mapping(priv->keyword_mapping, "if", SPI_SYM_KW_IF);
	l_add_kw_mapping(priv->keyword_mapping, "else", SPI_SYM_KW_ELSE);
	l_add_kw_mapping(priv->keyword_mapping, "this", SPI_SYM_KW_THIS);
	l_add_kw_mapping(priv->keyword_mapping, "for", SPI_SYM_KW_FOR);
	l_add_kw_mapping(priv->keyword_mapping, "null", SPI_SYM_KW_NULL);
	l_add_kw_mapping(priv->keyword_mapping, "true", SPI_SYM_KW_TRUE);
	l_add_kw_mapping(priv->keyword_mapping, "false", SPI_SYM_KW_FALSE);
	l_add_kw_mapping(priv->keyword_mapping, "delete", SPI_SYM_KW_DELETE);
	l_add_kw_mapping(priv->keyword_mapping, "void", SPI_SYM_KW_VOID);
	l_add_kw_mapping(priv->keyword_mapping, "typeof", SPI_SYM_KW_TYPEOF);
	l_add_kw_mapping(priv->keyword_mapping, "instanceof", SPI_SYM_KW_INSTANCE_OF);
	l_add_kw_mapping(priv->keyword_mapping, "in", SPI_SYM_KW_IN);
	l_add_kw_mapping(priv->keyword_mapping, "new", SPI_SYM_KW_NEW);
	l_add_kw_mapping(priv->keyword_mapping, "switch", SPI_SYM_KW_SWITCH);
	l_add_kw_mapping(priv->keyword_mapping, "case", SPI_SYM_KW_CASE);
	l_add_kw_mapping(priv->keyword_mapping, "default", SPI_SYM_KW_DEFAULT);
	l_add_kw_mapping(priv->keyword_mapping, "do", SPI_SYM_KW_DO);
	l_add_kw_mapping(priv->keyword_mapping, "while", SPI_SYM_KW_WHILE);
	l_add_kw_mapping(priv->keyword_mapping, "with", SPI_SYM_KW_WITH);
	l_add_kw_mapping(priv->keyword_mapping, "continue", SPI_SYM_KW_CONTINUE);
	l_add_kw_mapping(priv->keyword_mapping, "break", SPI_SYM_KW_BREAK);
	l_add_kw_mapping(priv->keyword_mapping, "throw", SPI_SYM_KW_THROW);
	l_add_kw_mapping(priv->keyword_mapping, "try", SPI_SYM_KW_TRY);
	l_add_kw_mapping(priv->keyword_mapping, "catch", SPI_SYM_KW_CATCH);
	l_add_kw_mapping(priv->keyword_mapping, "finally", SPI_SYM_KW_FINALLY);
	l_add_kw_mapping(priv->keyword_mapping, "null", SPI_SYM_KW_NULL);

	return result;
}


SpiScannerNew *spi_scanner_factory_create_scanner(SpiScannerFactory *factory, CatIUtf8Scanner *scanner) {
	SpiScannerFactoryPrivate *priv = spi_scanner_factory_get_instance_private(factory);
	return spi_scanner_new_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	SpiScannerFactory *instance = SPI_SCANNER_FACTORY(self);
	SpiScannerFactoryPrivate *priv = spi_scanner_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, symbol_provider=%O, token_factory=%O]", iname, self, priv->symbol_provider, priv->token_factory);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
