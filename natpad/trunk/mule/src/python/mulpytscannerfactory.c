/*
   File:    mulpytscannerfactory.c
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 14, 2016
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

#include <logging/catlogdefs.h>

#include "mulpytscannerfactory.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulPytScannerFactory"
#include <logging/catlog.h>

struct _MulPytScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};


static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulPytScannerFactory, mul_pyt_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulPytScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_pyt_scanner_factory_class_init(MulPytScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_pyt_scanner_factory_init(MulPytScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulPytScannerFactory *instance = MUL_PYT_SCANNER_FACTORY(object);
	MulPytScannerFactoryPrivate *priv = mul_pyt_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->connected_symbols);
	G_OBJECT_CLASS(mul_pyt_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_pyt_scanner_factory_parent_class)->finalize(object);
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

MulPytScannerFactory *mul_pyt_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	MulPytScannerFactory *result = g_object_new(MUL_TYPE_PYT_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	MulPytScannerFactoryPrivate *priv = mul_pyt_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, MUL_PYT_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_ERROR, symbol_provider, "⊗");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_NEWLINE, symbol_provider, "NEWLINE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_ENDMARKER, symbol_provider, "ENDMARKER");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_DOUBLEASTERISK, symbol_provider, "DOUBLEASTERISK");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_NAME, symbol_provider, "NAME");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_COMMA, symbol_provider, "COMMA");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_CHARAT, symbol_provider, "CHARAT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_LPARREN, symbol_provider, "LPARREN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_RPARREN, symbol_provider, "RPARREN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWDEF, symbol_provider, "KWDEF");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_COLON, symbol_provider, "COLON");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_EQUALS, symbol_provider, "EQUALS");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_SEMI, symbol_provider, "SEMI");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_ADD_ASSIGN, symbol_provider, "ADD_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_SUB_ASSIGN, symbol_provider, "SUB_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_MUL_ASSIGN, symbol_provider, "MUL_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_DIV_ASSIGN, symbol_provider, "DIV_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_MOD_ASSIGN, symbol_provider, "MOD_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_AND_ASSIGN, symbol_provider, "AND_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OR_ASSIGN, symbol_provider, "OR_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_XOR_ASSIGN, symbol_provider, "XOR_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_SHL_ASSIGN, symbol_provider, "SHL_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_SHR_ASSIGN, symbol_provider, "SHR_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_MULMUL_ASSIGN, symbol_provider, "MULMUL_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_DIVDIV_ASSIGN, symbol_provider, "DIVDIV_ASSIGN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWPRINT, symbol_provider, "KWPRINT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_SHIFTRIGHT, symbol_provider, "SHIFTRIGHT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_SHIFTLEFT, symbol_provider, "SHIFTLEFT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWDEL, symbol_provider, "KWDEL");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWPASS, symbol_provider, "KWPASS");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWBREAK, symbol_provider, "KWBREAK");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWCONTINUE, symbol_provider, "KWCONTINUE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWRETURN, symbol_provider, "KWRETURN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWRAISE, symbol_provider, "KWRAISE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWIMPORT, symbol_provider, "KWIMPORT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWFROM, symbol_provider, "KWFROM");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_DOT, symbol_provider, "DOT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWAS, symbol_provider, "KWAS");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWGLOBAL, symbol_provider, "KWGLOBAL");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWEXEC, symbol_provider, "KWEXEC");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWASSERT, symbol_provider, "KWASSERT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWIN, symbol_provider, "KWIN");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWIF, symbol_provider, "KWIF");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWELSE, symbol_provider, "KWELSE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWELIF, symbol_provider, "KWELIF");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWWHILE, symbol_provider, "KWWHILE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWFOR, symbol_provider, "KWFOR");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWTRY, symbol_provider, "KWTRY");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWFINALLY, symbol_provider, "KWFINALLY");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWWITH, symbol_provider, "KWWITH");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWEXCEPT, symbol_provider, "KWEXCEPT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_INDENT, symbol_provider, "INDENT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_DEDENT, symbol_provider, "DEDENT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWLAMBDA, symbol_provider, "KWLAMBDA");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWOR, symbol_provider, "KWOR");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWAND, symbol_provider, "KWAND");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWNOT, symbol_provider, "KWNOT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWIS, symbol_provider, "KWIS");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWCLASS, symbol_provider, "KWCLASS");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWYIELD, symbol_provider, "KWYIELD");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPLT, symbol_provider, "OPLT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPGT, symbol_provider, "OPGT");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPLE, symbol_provider, "OPLE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPGE, symbol_provider, "OPGE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPEQ, symbol_provider, "OPEQ");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPNE, symbol_provider, "OPNE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPDIF, symbol_provider, "OPDIF");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPOR, symbol_provider, "OPOR");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPAND, symbol_provider, "OPAND");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPXOR, symbol_provider, "OPXOR");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPADD, symbol_provider, "OPADD");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPSUB, symbol_provider, "OPSUB");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPDIV, symbol_provider, "OPDIV");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPMUL, symbol_provider, "OPMUL");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPMOD, symbol_provider, "OPMOD");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPDIVDIV, symbol_provider, "OPDIVDIV");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_OPNEG, symbol_provider, "OPNEG");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_LSQBR, symbol_provider, "LSQBR");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_RSQBR, symbol_provider, "RSQBR");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_LACCOL, symbol_provider, "LACCOL");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_RACCOL, symbol_provider, "RACCOL");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_SINGLEQUOTE, symbol_provider, "SINGLEQUOTE");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_NUMBER, symbol_provider, "NUMBER");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_STRING, symbol_provider, "STRING");
	l_connect_symbol(connected_syms, MUL_PYT_SYM_KWASSIGN, symbol_provider, "KWASSIGN");


	CatStringWo *t_eol_comment = cat_string_wo_new_with("EOL_COMMENT");
	GroRunSymbol *sym_eol = grorun_symbol_new(FALSE, t_eol_comment, 1000);
	cat_array_wo_append(connected_syms, (GObject *) sym_eol);
//	cat_unref_ptr(sym_eol);
//	cat_unref_ptr(t_eol_comment);

	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	l_add_kw_mapping(priv->keyword_mapping, "def", MUL_PYT_SYM_KWDEF);
	l_add_kw_mapping(priv->keyword_mapping, "print", MUL_PYT_SYM_KWPRINT);
	l_add_kw_mapping(priv->keyword_mapping, "del", MUL_PYT_SYM_KWDEL);
	l_add_kw_mapping(priv->keyword_mapping, "pass", MUL_PYT_SYM_KWPASS);
	l_add_kw_mapping(priv->keyword_mapping, "break", MUL_PYT_SYM_KWBREAK);
	l_add_kw_mapping(priv->keyword_mapping, "continue", MUL_PYT_SYM_KWCONTINUE);
	l_add_kw_mapping(priv->keyword_mapping, "return", MUL_PYT_SYM_KWRETURN);
	l_add_kw_mapping(priv->keyword_mapping, "raise", MUL_PYT_SYM_KWRAISE);
	l_add_kw_mapping(priv->keyword_mapping, "import", MUL_PYT_SYM_KWIMPORT);
	l_add_kw_mapping(priv->keyword_mapping, "from", MUL_PYT_SYM_KWFROM);
	l_add_kw_mapping(priv->keyword_mapping, "as", MUL_PYT_SYM_KWAS);
	l_add_kw_mapping(priv->keyword_mapping, "global", MUL_PYT_SYM_KWGLOBAL);
	l_add_kw_mapping(priv->keyword_mapping, "exec", MUL_PYT_SYM_KWEXEC);
	l_add_kw_mapping(priv->keyword_mapping, "assert", MUL_PYT_SYM_KWASSERT);
	l_add_kw_mapping(priv->keyword_mapping, "in", MUL_PYT_SYM_KWIN);
	l_add_kw_mapping(priv->keyword_mapping, "if", MUL_PYT_SYM_KWIF);
	l_add_kw_mapping(priv->keyword_mapping, "else", MUL_PYT_SYM_KWELSE);
	l_add_kw_mapping(priv->keyword_mapping, "elif", MUL_PYT_SYM_KWELIF);
	l_add_kw_mapping(priv->keyword_mapping, "while", MUL_PYT_SYM_KWWHILE);
	l_add_kw_mapping(priv->keyword_mapping, "for", MUL_PYT_SYM_KWFOR);
	l_add_kw_mapping(priv->keyword_mapping, "try", MUL_PYT_SYM_KWTRY);
	l_add_kw_mapping(priv->keyword_mapping, "finally", MUL_PYT_SYM_KWFINALLY);
	l_add_kw_mapping(priv->keyword_mapping, "with", MUL_PYT_SYM_KWWITH);
	l_add_kw_mapping(priv->keyword_mapping, "except", MUL_PYT_SYM_KWEXCEPT);
	l_add_kw_mapping(priv->keyword_mapping, "lambda", MUL_PYT_SYM_KWLAMBDA);
	l_add_kw_mapping(priv->keyword_mapping, "or", MUL_PYT_SYM_KWOR);
	l_add_kw_mapping(priv->keyword_mapping, "and", MUL_PYT_SYM_KWAND);
	l_add_kw_mapping(priv->keyword_mapping, "not", MUL_PYT_SYM_KWNOT);
	l_add_kw_mapping(priv->keyword_mapping, "is", MUL_PYT_SYM_KWIS);
	l_add_kw_mapping(priv->keyword_mapping, "class", MUL_PYT_SYM_KWCLASS);
	l_add_kw_mapping(priv->keyword_mapping, "yield", MUL_PYT_SYM_KWYIELD);
	l_add_kw_mapping(priv->keyword_mapping, "assign", MUL_PYT_SYM_KWASSIGN);

	return result;
}

MulPytScanner *mul_pyt_scanner_factory_create_scanner(MulPytScannerFactory *factory, CatIUtf8Scanner *scanner) {
	MulPytScannerFactoryPrivate *priv = mul_pyt_scanner_factory_get_instance_private(factory);
	return mul_pyt_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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
