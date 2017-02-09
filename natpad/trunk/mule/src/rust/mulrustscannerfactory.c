/*
   File:    mulrustscannerfactory.c
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#include "mulrustscannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulRustScannerFactory"
#include <logging/catlog.h>

struct _MulRustScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulRustScannerFactory, mul_rust_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulRustScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_rust_scanner_factory_class_init(MulRustScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_rust_scanner_factory_init(MulRustScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulRustScannerFactory *instance = MUL_RUST_SCANNER_FACTORY(object);
	MulRustScannerFactoryPrivate *priv = mul_rust_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->connected_symbols);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(mul_rust_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_rust_scanner_factory_parent_class)->finalize(object);
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

MulRustScannerFactory *mul_rust_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	MulRustScannerFactory *result = g_object_new(MUL_TYPE_RUST_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	MulRustScannerFactoryPrivate *priv = mul_rust_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, MUL_RUST_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_ERROR, symbol_provider, "⊗");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SHL, symbol_provider, "SHL");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SHR, symbol_provider, "SHR");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LE, symbol_provider, "LE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_EQEQ, symbol_provider, "EQEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_NE, symbol_provider, "NE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_GE, symbol_provider, "GE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_ANDAND, symbol_provider, "ANDAND");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_OROR, symbol_provider, "OROR");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SHLEQ, symbol_provider, "SHLEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SHREQ, symbol_provider, "SHREQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_MINUSEQ, symbol_provider, "MINUSEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_ANDEQ, symbol_provider, "ANDEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_OREQ, symbol_provider, "OREQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_PLUSEQ, symbol_provider, "PLUSEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_STAREQ, symbol_provider, "STAREQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SLASHEQ, symbol_provider, "SLASHEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_CARETEQ, symbol_provider, "CARETEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_PERCENTEQ, symbol_provider, "PERCENTEQ");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_DOTDOT, symbol_provider, "DOTDOT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_DOTDOTDOT, symbol_provider, "DOTDOTDOT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_MOD_SEP, symbol_provider, "MOD_SEP");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_RARROW, symbol_provider, "RARROW");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LARROW, symbol_provider, "LARROW");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_FAT_ARROW, symbol_provider, "FAT_ARROW");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_BYTE, symbol_provider, "LIT_BYTE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_CHAR, symbol_provider, "LIT_CHAR");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_INTEGER, symbol_provider, "LIT_INTEGER");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_FLOAT, symbol_provider, "LIT_FLOAT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_STR, symbol_provider, "LIT_STR");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_STR_RAW, symbol_provider, "LIT_STR_RAW");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_BYTE_STR, symbol_provider, "LIT_BYTE_STR");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIT_BYTE_STR_RAW, symbol_provider, "LIT_BYTE_STR_RAW");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_IDENT, symbol_provider, "IDENT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_UNDERSCORE, symbol_provider, "UNDERSCORE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LIFETIME, symbol_provider, "LIFETIME");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SELF, symbol_provider, "SELF");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_STATIC, symbol_provider, "STATIC");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_AS, symbol_provider, "AS");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_BREAK, symbol_provider, "BREAK");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_CRATE, symbol_provider, "CRATE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_ELSE, symbol_provider, "ELSE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_ENUM, symbol_provider, "ENUM");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_EXTERN, symbol_provider, "EXTERN");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_FALSE, symbol_provider, "FALSE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_FN, symbol_provider, "FN");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_FOR, symbol_provider, "FOR");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_IF, symbol_provider, "IF");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_IMPL, symbol_provider, "IMPL");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_IN, symbol_provider, "IN");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LET, symbol_provider, "LET");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_LOOP, symbol_provider, "LOOP");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_MATCH, symbol_provider, "MATCH");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_MOD, symbol_provider, "MOD");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_MOVE, symbol_provider, "MOVE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_MUT, symbol_provider, "MUT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_PRIV, symbol_provider, "PRIV");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_PUB, symbol_provider, "PUB");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_REF, symbol_provider, "REF");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_RETURN, symbol_provider, "RETURN");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_STRUCT, symbol_provider, "STRUCT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_TRUE, symbol_provider, "TRUE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_TRAIT, symbol_provider, "TRAIT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_TYPE, symbol_provider, "TYPE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_UNSAFE, symbol_provider, "UNSAFE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_USE, symbol_provider, "USE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_WHILE, symbol_provider, "WHILE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_CONTINUE, symbol_provider, "CONTINUE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_PROC, symbol_provider, "PROC");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_BOX, symbol_provider, "BOX");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_CONST, symbol_provider, "CONST");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_WHERE, symbol_provider, "WHERE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_TYPEOF, symbol_provider, "TYPEOF");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_INNER_DOC_COMMENT, symbol_provider, "INNER_DOC_COMMENT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_OUTER_DOC_COMMENT, symbol_provider, "OUTER_DOC_COMMENT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SHEBANG, symbol_provider, "SHEBANG");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_SHEBANG_LINE, symbol_provider, "SHEBANG_LINE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_STATIC_LIFETIME, symbol_provider, "STATIC_LIFETIME");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_LSQBRACE, symbol_provider, "L_LSQBRACE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_RSQBRACE, symbol_provider, "L_RSQBRACE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_LPAREN, symbol_provider, "L_LPAREN");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_RPAREN, symbol_provider, "L_RPAREN");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_LCUBRACE, symbol_provider, "L_LCUBRACE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_RCUBRACE, symbol_provider, "L_RCUBRACE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_LT, symbol_provider, "L_LT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_GT, symbol_provider, "L_GT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_COMMA, symbol_provider, "L_COMMA");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_DOT, symbol_provider, "L_DOT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_SEMI, symbol_provider, "L_SEMI");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_AMPERSAND, symbol_provider, "L_AMPERSAND");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_COLON, symbol_provider, "L_COLON");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_HASH, symbol_provider, "L_HASH");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_AT, symbol_provider, "L_AT");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_NEG, symbol_provider, "L_NEG");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_QUESTIONMARK, symbol_provider, "L_QUESTIONMARK");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_EXCLAMATION, symbol_provider, "L_EXCLAMATION");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_VERTICAL_LINE, symbol_provider, "L_VERTICAL_LINE");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_CIRCUMFLEX, symbol_provider, "L_CIRCUMFLEX");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_PLUS, symbol_provider, "L_PLUS");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_MINUS, symbol_provider, "L_MINUS");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_MUL, symbol_provider, "L_MUL");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_DIV, symbol_provider, "L_DIV");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_IS, symbol_provider, "L_IS");
	l_connect_symbol(connected_syms, MUL_RUST_SYM_L_MOD, symbol_provider, "L_MOD");

	CatStringWo *t_block_comment = cat_string_wo_new_with("BLOCK_COMMENT");
	GroRunSymbol *sym_block_comment = grorun_symbol_new(FALSE, t_block_comment, MUL_RUST_SYM_BLOCK_COMMENT);
	cat_array_wo_append(connected_syms, (GObject *) sym_block_comment);
	cat_unref_ptr(sym_block_comment);
	cat_unref_ptr(t_block_comment);


	CatStringWo *t_eol_comment = cat_string_wo_new_with("EOL_COMMENT");
	GroRunSymbol *sym_eol = grorun_symbol_new(FALSE, t_eol_comment, 1000);
	cat_array_wo_append(connected_syms, (GObject *) sym_eol);
	cat_unref_ptr(sym_eol);
	cat_unref_ptr(t_eol_comment);

	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	l_add_kw_mapping(priv->keyword_mapping, "lit_integer", MUL_RUST_SYM_LIT_INTEGER);
	l_add_kw_mapping(priv->keyword_mapping, "lit_float", MUL_RUST_SYM_LIT_FLOAT);
	l_add_kw_mapping(priv->keyword_mapping, "lit_str", MUL_RUST_SYM_LIT_STR);
	l_add_kw_mapping(priv->keyword_mapping, "lit_str_raw", MUL_RUST_SYM_LIT_STR_RAW);
	l_add_kw_mapping(priv->keyword_mapping, "lit_byte_str", MUL_RUST_SYM_LIT_BYTE_STR);
	l_add_kw_mapping(priv->keyword_mapping, "lit_byte_str_raw", MUL_RUST_SYM_LIT_BYTE_STR_RAW);
	l_add_kw_mapping(priv->keyword_mapping, "ident", MUL_RUST_SYM_IDENT);
	l_add_kw_mapping(priv->keyword_mapping, "lifetime", MUL_RUST_SYM_LIFETIME);
	l_add_kw_mapping(priv->keyword_mapping, "self", MUL_RUST_SYM_SELF);
	l_add_kw_mapping(priv->keyword_mapping, "static", MUL_RUST_SYM_STATIC);
	l_add_kw_mapping(priv->keyword_mapping, "as", MUL_RUST_SYM_AS);
	l_add_kw_mapping(priv->keyword_mapping, "break", MUL_RUST_SYM_BREAK);
	l_add_kw_mapping(priv->keyword_mapping, "crate", MUL_RUST_SYM_CRATE);
	l_add_kw_mapping(priv->keyword_mapping, "else", MUL_RUST_SYM_ELSE);
	l_add_kw_mapping(priv->keyword_mapping, "enum", MUL_RUST_SYM_ENUM);
	l_add_kw_mapping(priv->keyword_mapping, "extern", MUL_RUST_SYM_EXTERN);
	l_add_kw_mapping(priv->keyword_mapping, "false", MUL_RUST_SYM_FALSE);
	l_add_kw_mapping(priv->keyword_mapping, "fn", MUL_RUST_SYM_FN);
	l_add_kw_mapping(priv->keyword_mapping, "for", MUL_RUST_SYM_FOR);
	l_add_kw_mapping(priv->keyword_mapping, "if", MUL_RUST_SYM_IF);
	l_add_kw_mapping(priv->keyword_mapping, "impl", MUL_RUST_SYM_IMPL);
	l_add_kw_mapping(priv->keyword_mapping, "in", MUL_RUST_SYM_IN);
	l_add_kw_mapping(priv->keyword_mapping, "let", MUL_RUST_SYM_LET);
	l_add_kw_mapping(priv->keyword_mapping, "loop", MUL_RUST_SYM_LOOP);
	l_add_kw_mapping(priv->keyword_mapping, "match", MUL_RUST_SYM_MATCH);
	l_add_kw_mapping(priv->keyword_mapping, "mod", MUL_RUST_SYM_MOD);
	l_add_kw_mapping(priv->keyword_mapping, "move", MUL_RUST_SYM_MOVE);
	l_add_kw_mapping(priv->keyword_mapping, "mut", MUL_RUST_SYM_MUT);
	l_add_kw_mapping(priv->keyword_mapping, "priv", MUL_RUST_SYM_PRIV);
	l_add_kw_mapping(priv->keyword_mapping, "pub", MUL_RUST_SYM_PUB);
	l_add_kw_mapping(priv->keyword_mapping, "ref", MUL_RUST_SYM_REF);
	l_add_kw_mapping(priv->keyword_mapping, "return", MUL_RUST_SYM_RETURN);
	l_add_kw_mapping(priv->keyword_mapping, "struct", MUL_RUST_SYM_STRUCT);
	l_add_kw_mapping(priv->keyword_mapping, "true", MUL_RUST_SYM_TRUE);
	l_add_kw_mapping(priv->keyword_mapping, "trait", MUL_RUST_SYM_TRAIT);
	l_add_kw_mapping(priv->keyword_mapping, "type", MUL_RUST_SYM_TYPE);
	l_add_kw_mapping(priv->keyword_mapping, "unsafe", MUL_RUST_SYM_UNSAFE);
	l_add_kw_mapping(priv->keyword_mapping, "use", MUL_RUST_SYM_USE);
	l_add_kw_mapping(priv->keyword_mapping, "while", MUL_RUST_SYM_WHILE);
	l_add_kw_mapping(priv->keyword_mapping, "continue", MUL_RUST_SYM_CONTINUE);
	l_add_kw_mapping(priv->keyword_mapping, "proc", MUL_RUST_SYM_PROC);
	l_add_kw_mapping(priv->keyword_mapping, "box", MUL_RUST_SYM_BOX);
	l_add_kw_mapping(priv->keyword_mapping, "const", MUL_RUST_SYM_CONST);
	l_add_kw_mapping(priv->keyword_mapping, "where", MUL_RUST_SYM_WHERE);
	l_add_kw_mapping(priv->keyword_mapping, "typeof", MUL_RUST_SYM_TYPEOF);
	l_add_kw_mapping(priv->keyword_mapping, "inner_doc_comment", MUL_RUST_SYM_INNER_DOC_COMMENT);
	l_add_kw_mapping(priv->keyword_mapping, "outer_doc_comment", MUL_RUST_SYM_OUTER_DOC_COMMENT);
	l_add_kw_mapping(priv->keyword_mapping, "shebang", MUL_RUST_SYM_SHEBANG);
	l_add_kw_mapping(priv->keyword_mapping, "shebang_line", MUL_RUST_SYM_SHEBANG_LINE);
	return result;
}

MulRustScanner *mul_rust_scanner_factory_create_scanner(MulRustScannerFactory *factory, CatIUtf8Scanner *scanner) {
	MulRustScannerFactoryPrivate *priv = mul_rust_scanner_factory_get_instance_private(factory);
	return mul_rust_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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
