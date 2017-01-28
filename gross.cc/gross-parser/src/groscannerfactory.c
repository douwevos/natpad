/*
   File:    groscannerfactory.c
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

#include "groscannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroScannerFactory"
#include <logging/catlog.h>

struct _GroScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroScannerFactory, gro_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_scanner_factory_class_init(GroScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_scanner_factory_init(GroScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroScannerFactory *instance = GRO_SCANNER_FACTORY(object);
	GroScannerFactoryPrivate *priv = gro_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->connected_symbols);
	G_OBJECT_CLASS(gro_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_scanner_factory_parent_class)->finalize(object);
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

GroScannerFactory *gro_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	GroScannerFactory *result = g_object_new(GRO_TYPE_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	GroScannerFactoryPrivate *priv = gro_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, GRO_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, GRO_SYM_ERROR, symbol_provider, "⊗");
	l_connect_symbol(connected_syms, GRO_SYM_NUMBER, symbol_provider, "NUMBER");
	l_connect_symbol(connected_syms, GRO_SYM_RULE_SEPERATOR, symbol_provider, "RULE_SEPERATOR");
	l_connect_symbol(connected_syms, GRO_SYM_INDEX_MARK, symbol_provider, "INDEX_MARK");
	l_connect_symbol(connected_syms, GRO_SYM_COMMA, symbol_provider, "COMMA");
	l_connect_symbol(connected_syms, GRO_SYM_COLON, symbol_provider, "COLON");
	l_connect_symbol(connected_syms, GRO_SYM_SEMI, symbol_provider, "SEMI");
	l_connect_symbol(connected_syms, GRO_SYM_BAR, symbol_provider, "BAR");
	l_connect_symbol(connected_syms, GRO_SYM_EXCL, symbol_provider, "EXCL");
	l_connect_symbol(connected_syms, GRO_SYM_STAR, symbol_provider, "STAR");
	l_connect_symbol(connected_syms, GRO_SYM_PLUS, symbol_provider, "PLUS");
	l_connect_symbol(connected_syms, GRO_SYM_LPAREN, symbol_provider, "LPAREN");
	l_connect_symbol(connected_syms, GRO_SYM_RPAREN, symbol_provider, "RPAREN");
	l_connect_symbol(connected_syms, GRO_SYM_LSQBRACK, symbol_provider, "LSQBRACK");
	l_connect_symbol(connected_syms, GRO_SYM_RSQBRACK, symbol_provider, "RSQBRACK");
	l_connect_symbol(connected_syms, GRO_SYM_LCUBRACK, symbol_provider, "LCUBRACK");
	l_connect_symbol(connected_syms, GRO_SYM_RCUBRACK, symbol_provider, "RCUBRACK");
	l_connect_symbol(connected_syms, GRO_SYM_KW_TERMINAL, symbol_provider, "TERMINAL");
	l_connect_symbol(connected_syms, GRO_SYM_KW_CODE, symbol_provider, "CODE");
	l_connect_symbol(connected_syms, GRO_SYM_KW_TYPE, symbol_provider, "TYPE");
	l_connect_symbol(connected_syms, GRO_SYM_KW_IMPORTS, symbol_provider, "IMPORTS");
	l_connect_symbol(connected_syms, GRO_SYM_IDENTIFIER, symbol_provider, "IDENTIFIER");
	l_connect_symbol(connected_syms, GRO_SYM_CODE_TEXT, symbol_provider, "CODE_TEXT");
//	l_connect_symbol(connected_syms, GRO_SYM_COLON_DOT, symbol_provider, "COLON_DOT");


	CatStringWo *t_eol_comment = cat_string_wo_new_with("EOL_COMMENT");
	GroRunSymbol *sym_eol = grorun_symbol_new(FALSE, t_eol_comment, 1000);
	cat_array_wo_append(connected_syms, (GObject *) sym_eol);
	cat_unref_ptr(sym_eol);
	cat_unref_ptr(t_eol_comment);


	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	l_add_kw_mapping(priv->keyword_mapping, "terminal", GRO_SYM_KW_TERMINAL);
	l_add_kw_mapping(priv->keyword_mapping, "code", GRO_SYM_KW_CODE);
	l_add_kw_mapping(priv->keyword_mapping, "type", GRO_SYM_KW_TYPE);
	l_add_kw_mapping(priv->keyword_mapping, "imports", GRO_SYM_KW_IMPORTS);

	return result;
}

GroScanner *gro_scanner_factory_create_scanner(GroScannerFactory *factory, CatIUtf8Scanner *scanner) {
	GroScannerFactoryPrivate *priv = gro_scanner_factory_get_instance_private(factory);
	return gro_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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
