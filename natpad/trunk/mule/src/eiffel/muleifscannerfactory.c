/*
   File:    muleifscannerfactory.c
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

#include "muleifscannerfactory.h"

#include <logging/catlogdefs.h>

#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulEifScannerFactory"
#include <logging/catlog.h>

struct _MulEifScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};


static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulEifScannerFactory, mul_eif_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulEifScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_eif_scanner_factory_class_init(MulEifScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_eif_scanner_factory_init(MulEifScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulEifScannerFactory *instance = MUL_EIF_SCANNER_FACTORY(object);
	MulEifScannerFactoryPrivate *priv = mul_eif_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->connected_symbols);
	G_OBJECT_CLASS(mul_eif_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_eif_scanner_factory_parent_class)->finalize(object);
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

MulEifScannerFactory *mul_eif_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	MulEifScannerFactory *result = g_object_new(MUL_TYPE_EIF_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	MulEifScannerFactoryPrivate *priv = mul_eif_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, MUL_EIF_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_ERROR, symbol_provider, "⊗");

	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWDEFERRED, symbol_provider, "KWDEFERRED");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWEXPANDED, symbol_provider, "KWEXPANDED");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWSEPARATE, symbol_provider, "KWSEPARATE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWCLASS, symbol_provider, "KWCLASS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_IDENTIFIER, symbol_provider, "IDENTIFIER");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWEND, symbol_provider, "KWEND");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWINDEXING, symbol_provider, "KWINDEXING");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_SEMICOLON, symbol_provider, "SEMICOLON");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_COLON, symbol_provider, "COLON");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_COMMA, symbol_provider, "COMMA");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_ARROW, symbol_provider, "ARROW");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWOBSOLETE, symbol_provider, "KWOBSOLETE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWINHERIT, symbol_provider, "KWINHERIT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWRENAME, symbol_provider, "KWRENAME");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWAS, symbol_provider, "KWAS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWLIKE, symbol_provider, "KWLIKE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWCURRENT, symbol_provider, "KWCURRENT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWBIT, symbol_provider, "KWBIT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_INTEGER_CONSTANT, symbol_provider, "INTEGER_CONSTANT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWUNDEFINE, symbol_provider, "KWUNDEFINE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWREDEFINE, symbol_provider, "KWREDEFINE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWEXPORT, symbol_provider, "KWEXPORT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWSELECT, symbol_provider, "KWSELECT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWALL, symbol_provider, "KWALL");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWCREATE, symbol_provider, "KWCREATE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWFEATURE, symbol_provider, "KWFEATURE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWFROZEN, symbol_provider, "KWFROZEN");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWPREFIX, symbol_provider, "KWPREFIX");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWINFIX, symbol_provider, "KWINFIX");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_QUOTED_UNARY, symbol_provider, "QUOTED_UNARY");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_QUOTED_BINARY, symbol_provider, "QUOTED_BINARY");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_QUOTED_FREE_OPERATOR, symbol_provider, "QUOTED_FREE_OPERATOR");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_QUESTION_MARK, symbol_provider, "QUESTION_MARK");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_LPAREN, symbol_provider, "LPAREN");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_RPAREN, symbol_provider, "RPAREN");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWIS, symbol_provider, "KWIS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWUNIQUE, symbol_provider, "KWUNIQUE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWDO, symbol_provider, "KWDO");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWONCE, symbol_provider, "KWONCE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWEXTERNAL, symbol_provider, "KWEXTERNAL");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_MANIFEST_STRING, symbol_provider, "MANIFEST_STRING");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWALIAS, symbol_provider, "KWALIAS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWLOCAL, symbol_provider, "KWLOCAL");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWREQUIRE, symbol_provider, "KWREQUIRE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWELSE, symbol_provider, "KWELSE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWENSURE, symbol_provider, "KWENSURE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWTHEN, symbol_provider, "KWTHEN");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWINVARIANT, symbol_provider, "KWINVARIANT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWVARIANT, symbol_provider, "KWVARIANT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_COMMENT, symbol_provider, "COMMENT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWRESCUE, symbol_provider, "KWRESCUE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_EXCLAMATION, symbol_provider, "EXCLAMATION");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_DOT, symbol_provider, "DOT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_COLON_IS, symbol_provider, "COLON_IS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_QUESTION_MARK_IS, symbol_provider, "QUESTION_MARK_IS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWIF, symbol_provider, "KWIF");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWELSEIF, symbol_provider, "KWELSEIF");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWINSPECT, symbol_provider, "KWINSPECT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWWHEN, symbol_provider, "KWWHEN");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_DOUBLE_DOT, symbol_provider, "DOUBLE_DOT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_CHARACTER_CONSTANT, symbol_provider, "CHARACTER_CONSTANT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWFROM, symbol_provider, "KWFROM");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWLOOP, symbol_provider, "KWLOOP");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWUNTIL, symbol_provider, "KWUNTIL");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWDEBUG, symbol_provider, "KWDEBUG");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWCHECK, symbol_provider, "KWCHECK");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWRETRY, symbol_provider, "KWRETRY");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWRESULT, symbol_provider, "KWRESULT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWPRECURSOR, symbol_provider, "KWPRECURSOR");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_DOLLAR, symbol_provider, "DOLLAR");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_BIT_CONSTANT, symbol_provider, "BIT_CONSTANT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_INTEGER, symbol_provider, "INTEGER");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_REAL, symbol_provider, "REAL");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_HEXADECIMAL_CONSTANT, symbol_provider, "HEXADECIMAL_CONSTANT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_PREFIX_OPERATOR, symbol_provider, "PREFIX_OPERATOR");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_INFIX_OPERATOR, symbol_provider, "INFIX_OPERATOR");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_OP_IS, symbol_provider, "OP_IS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_OP_FWSLASH_IS, symbol_provider, "OP_FWSLASH_IS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWTRUE, symbol_provider, "KWTRUE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWFALSE, symbol_provider, "KWFALSE");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_OP_PLUS, symbol_provider, "OP_PLUS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_OP_MINUS, symbol_provider, "OP_MINUS");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_LTLT, symbol_provider, "LTLT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_GTGT, symbol_provider, "GTGT");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWOLD, symbol_provider, "KWOLD");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWSTRIP, symbol_provider, "KWSTRIP");
	l_connect_symbol(connected_syms, MUL_EIF_SYM_KWNOTE, symbol_provider, "KWNOTE");

	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	l_add_kw_mapping(priv->keyword_mapping, "deferred", MUL_EIF_SYM_KWDEFERRED);
	l_add_kw_mapping(priv->keyword_mapping, "expanded", MUL_EIF_SYM_KWEXPANDED);
	l_add_kw_mapping(priv->keyword_mapping, "separate", MUL_EIF_SYM_KWSEPARATE);
	l_add_kw_mapping(priv->keyword_mapping, "class", MUL_EIF_SYM_KWCLASS);
	l_add_kw_mapping(priv->keyword_mapping, "end", MUL_EIF_SYM_KWEND);
	l_add_kw_mapping(priv->keyword_mapping, "indexing", MUL_EIF_SYM_KWINDEXING);
	l_add_kw_mapping(priv->keyword_mapping, "obsolete", MUL_EIF_SYM_KWOBSOLETE);
	l_add_kw_mapping(priv->keyword_mapping, "inherit", MUL_EIF_SYM_KWINHERIT);
	l_add_kw_mapping(priv->keyword_mapping, "rename", MUL_EIF_SYM_KWRENAME);
	l_add_kw_mapping(priv->keyword_mapping, "as", MUL_EIF_SYM_KWAS);
	l_add_kw_mapping(priv->keyword_mapping, "like", MUL_EIF_SYM_KWLIKE);
	l_add_kw_mapping(priv->keyword_mapping, "current", MUL_EIF_SYM_KWCURRENT);
	l_add_kw_mapping(priv->keyword_mapping, "bit", MUL_EIF_SYM_KWBIT);
	l_add_kw_mapping(priv->keyword_mapping, "undefine", MUL_EIF_SYM_KWUNDEFINE);
	l_add_kw_mapping(priv->keyword_mapping, "redefine", MUL_EIF_SYM_KWREDEFINE);
	l_add_kw_mapping(priv->keyword_mapping, "export", MUL_EIF_SYM_KWEXPORT);
	l_add_kw_mapping(priv->keyword_mapping, "select", MUL_EIF_SYM_KWSELECT);
	l_add_kw_mapping(priv->keyword_mapping, "all", MUL_EIF_SYM_KWALL);
	l_add_kw_mapping(priv->keyword_mapping, "create", MUL_EIF_SYM_KWCREATE);
	l_add_kw_mapping(priv->keyword_mapping, "feature", MUL_EIF_SYM_KWFEATURE);
	l_add_kw_mapping(priv->keyword_mapping, "frozen", MUL_EIF_SYM_KWFROZEN);
	l_add_kw_mapping(priv->keyword_mapping, "prefix", MUL_EIF_SYM_KWPREFIX);
	l_add_kw_mapping(priv->keyword_mapping, "infix", MUL_EIF_SYM_KWINFIX);
	l_add_kw_mapping(priv->keyword_mapping, "is", MUL_EIF_SYM_KWIS);
	l_add_kw_mapping(priv->keyword_mapping, "unique", MUL_EIF_SYM_KWUNIQUE);
	l_add_kw_mapping(priv->keyword_mapping, "do", MUL_EIF_SYM_KWDO);
	l_add_kw_mapping(priv->keyword_mapping, "once", MUL_EIF_SYM_KWONCE);
	l_add_kw_mapping(priv->keyword_mapping, "external", MUL_EIF_SYM_KWEXTERNAL);
	l_add_kw_mapping(priv->keyword_mapping, "alias", MUL_EIF_SYM_KWALIAS);
	l_add_kw_mapping(priv->keyword_mapping, "local", MUL_EIF_SYM_KWLOCAL);
	l_add_kw_mapping(priv->keyword_mapping, "require", MUL_EIF_SYM_KWREQUIRE);
	l_add_kw_mapping(priv->keyword_mapping, "else", MUL_EIF_SYM_KWELSE);
	l_add_kw_mapping(priv->keyword_mapping, "ensure", MUL_EIF_SYM_KWENSURE);
	l_add_kw_mapping(priv->keyword_mapping, "then", MUL_EIF_SYM_KWTHEN);
	l_add_kw_mapping(priv->keyword_mapping, "invariant", MUL_EIF_SYM_KWINVARIANT);
	l_add_kw_mapping(priv->keyword_mapping, "variant", MUL_EIF_SYM_KWVARIANT);
	l_add_kw_mapping(priv->keyword_mapping, "rescue", MUL_EIF_SYM_KWRESCUE);
	l_add_kw_mapping(priv->keyword_mapping, "if", MUL_EIF_SYM_KWIF);
	l_add_kw_mapping(priv->keyword_mapping, "elseif", MUL_EIF_SYM_KWELSEIF);
	l_add_kw_mapping(priv->keyword_mapping, "inspect", MUL_EIF_SYM_KWINSPECT);
	l_add_kw_mapping(priv->keyword_mapping, "when", MUL_EIF_SYM_KWWHEN);
	l_add_kw_mapping(priv->keyword_mapping, "from", MUL_EIF_SYM_KWFROM);
	l_add_kw_mapping(priv->keyword_mapping, "loop", MUL_EIF_SYM_KWLOOP);
	l_add_kw_mapping(priv->keyword_mapping, "until", MUL_EIF_SYM_KWUNTIL);
	l_add_kw_mapping(priv->keyword_mapping, "debug", MUL_EIF_SYM_KWDEBUG);
	l_add_kw_mapping(priv->keyword_mapping, "check", MUL_EIF_SYM_KWCHECK);
	l_add_kw_mapping(priv->keyword_mapping, "retry", MUL_EIF_SYM_KWRETRY);
	l_add_kw_mapping(priv->keyword_mapping, "result", MUL_EIF_SYM_KWRESULT);
	l_add_kw_mapping(priv->keyword_mapping, "precursor", MUL_EIF_SYM_KWPRECURSOR);
	l_add_kw_mapping(priv->keyword_mapping, "true", MUL_EIF_SYM_KWTRUE);
	l_add_kw_mapping(priv->keyword_mapping, "false", MUL_EIF_SYM_KWFALSE);
	l_add_kw_mapping(priv->keyword_mapping, "old", MUL_EIF_SYM_KWOLD);
	l_add_kw_mapping(priv->keyword_mapping, "strip", MUL_EIF_SYM_KWSTRIP);
	l_add_kw_mapping(priv->keyword_mapping, "note", MUL_EIF_SYM_KWNOTE);

	return result;
}

MulEifScanner *mul_eif_scanner_factory_create_scanner(MulEifScannerFactory *factory, CatIUtf8Scanner *scanner) {
	MulEifScannerFactoryPrivate *priv = mul_eif_scanner_factory_get_instance_private(factory);
	return mul_eif_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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
