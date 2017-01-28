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

#include <logging/catlogdefs.h>

#include "mulvhdlscannerfactory.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulVhdlScannerFactory"
#include <logging/catlog.h>

struct _MulVhdlScannerFactoryPrivate {
	GroRunISymbolProvider *symbol_provider;
	GroRunITokenFactory *token_factory;
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulVhdlScannerFactory, mul_vhdl_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulVhdlScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_vhdl_scanner_factory_class_init(MulVhdlScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_vhdl_scanner_factory_init(MulVhdlScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulVhdlScannerFactory *instance = MUL_VHDL_SCANNER_FACTORY(object);
	MulVhdlScannerFactoryPrivate *priv = mul_vhdl_scanner_factory_get_instance_private(instance);
	cat_unref_ptr(priv->symbol_provider);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->connected_symbols);
//	cat_unref_ptr(priv->connected_symbols[MUL_VHDL_SYM_LINE_COMMENT]);
//	cat_unref_ptr(priv->connected_symbols[MUL_VHDL_SYM_FULL_COMMENT]);
	G_OBJECT_CLASS(mul_vhdl_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_vhdl_scanner_factory_parent_class)->finalize(object);
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
		cat_log_fatal("incorrect offset for symbol with name:%s, expected:%d but is:%d", name, expected_conn_id, cat_array_wo_size(connected_syms));
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

MulVhdlScannerFactory *mul_vhdl_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory) {
	MulVhdlScannerFactory *result = g_object_new(MUL_TYPE_VHDL_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	MulVhdlScannerFactoryPrivate *priv = mul_vhdl_scanner_factory_get_instance_private(result);
	priv->symbol_provider = cat_ref_ptr(symbol_provider);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->connected_symbols = cat_array_wo_new();
	CatArrayWo *connected_syms = priv->connected_symbols;

	l_connect_symbol(connected_syms, MUL_VHDL_SYM_END_OF_INPUT, symbol_provider, "⟧");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_ERROR, symbol_provider, "⊗");

	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWACCESS, symbol_provider, "KWACCESS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWAFTER, symbol_provider, "KWAFTER");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWALIAS, symbol_provider, "KWALIAS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWALL, symbol_provider, "KWALL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWAND, symbol_provider, "KWAND");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWARCHITECTURE, symbol_provider, "KWARCHITECTURE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWARRAY, symbol_provider, "KWARRAY");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWASSERT, symbol_provider, "KWASSERT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWATTRIBUTE, symbol_provider, "KWATTRIBUTE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWBEGIN, symbol_provider, "KWBEGIN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWBLOCK, symbol_provider, "KWBLOCK");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWBODY, symbol_provider, "KWBODY");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWBUFFER, symbol_provider, "KWBUFFER");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWBUS, symbol_provider, "KWBUS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWCASE, symbol_provider, "KWCASE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWCOMPONENT, symbol_provider, "KWCOMPONENT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWCONFIGURATION, symbol_provider, "KWCONFIGURATION");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWCONSTANT, symbol_provider, "KWCONSTANT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWDISCONNECT, symbol_provider, "KWDISCONNECT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWDOWNTO, symbol_provider, "KWDOWNTO");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWELSE, symbol_provider, "KWELSE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWELSIF, symbol_provider, "KWELSIF");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWEND, symbol_provider, "KWEND");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWENTITY, symbol_provider, "KWENTITY");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWEXIT, symbol_provider, "KWEXIT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWFILE, symbol_provider, "KWFILE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWFOR, symbol_provider, "KWFOR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWFUNCTION, symbol_provider, "KWFUNCTION");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWGENERATE, symbol_provider, "KWGENERATE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWGENERIC, symbol_provider, "KWGENERIC");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWGUARDED, symbol_provider, "KWGUARDED");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWIF, symbol_provider, "KWIF");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWIMPURE, symbol_provider, "KWIMPURE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWIN, symbol_provider, "KWIN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWINERTIAL, symbol_provider, "KWINERTIAL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWINOUT, symbol_provider, "KWINOUT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWIS, symbol_provider, "KWIS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWLABEL, symbol_provider, "KWLABEL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWLIBRARY, symbol_provider, "KWLIBRARY");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWLINKAGE, symbol_provider, "KWLINKAGE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWLOOP, symbol_provider, "KWLOOP");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWMAP, symbol_provider, "KWMAP");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWNAND, symbol_provider, "KWNAND");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWNEW, symbol_provider, "KWNEW");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWNEXT, symbol_provider, "KWNEXT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWNOR, symbol_provider, "KWNOR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWNULL, symbol_provider, "KWNULL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWOF, symbol_provider, "KWOF");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWON, symbol_provider, "KWON");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWOPEN, symbol_provider, "KWOPEN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWOR, symbol_provider, "KWOR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWOTHERS, symbol_provider, "KWOTHERS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWOUT, symbol_provider, "KWOUT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWPACKAGE, symbol_provider, "KWPACKAGE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWPORT, symbol_provider, "KWPORT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWPOSTPONED, symbol_provider, "KWPOSTPONED");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWPROCEDURE, symbol_provider, "KWPROCEDURE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWPROCESS, symbol_provider, "KWPROCESS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWPURE, symbol_provider, "KWPURE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWRANGE, symbol_provider, "KWRANGE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWREVERSE_RANGE, symbol_provider, "KWREVERSE_RANGE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWRECORD, symbol_provider, "KWRECORD");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWREGISTER, symbol_provider, "KWREGISTER");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWREJECT, symbol_provider, "KWREJECT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWREPORT, symbol_provider, "KWREPORT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWRETURN, symbol_provider, "KWRETURN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWROL, symbol_provider, "KWROL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWROR, symbol_provider, "KWROR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSELECT, symbol_provider, "KWSELECT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSEVERITY, symbol_provider, "KWSEVERITY");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSIGNAL, symbol_provider, "KWSIGNAL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSLA, symbol_provider, "KWSLA");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSLL, symbol_provider, "KWSLL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSRA, symbol_provider, "KWSRA");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSRL, symbol_provider, "KWSRL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWSUBTYPE, symbol_provider, "KWSUBTYPE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWTHEN, symbol_provider, "KWTHEN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWTO, symbol_provider, "KWTO");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWTRANSPORT, symbol_provider, "KWTRANSPORT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWTYPE, symbol_provider, "KWTYPE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWUNITS, symbol_provider, "KWUNITS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWUNTIL, symbol_provider, "KWUNTIL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWUSE, symbol_provider, "KWUSE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWVARIABLE, symbol_provider, "KWVARIABLE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWWAIT, symbol_provider, "KWWAIT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWWHEN, symbol_provider, "KWWHEN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWWHILE, symbol_provider, "KWWHILE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWWITH, symbol_provider, "KWWITH");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWXNOR, symbol_provider, "KWXNOR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWXOR, symbol_provider, "KWXOR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_OPPEQ, symbol_provider, "OPPEQ");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_OPPNE, symbol_provider, "OPPNE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_OPPLT, symbol_provider, "OPPLT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_OPPLE, symbol_provider, "OPPLE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_OPPGT, symbol_provider, "OPPGT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_OPPGE, symbol_provider, "OPPGE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_PLUS, symbol_provider, "PLUS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_MINUS, symbol_provider, "MINUS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_AMPERSAND, symbol_provider, "AMPERSAND");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_STAR, symbol_provider, "STAR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_SLASH, symbol_provider, "SLASH");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWMOD, symbol_provider, "KWMOD");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWREM, symbol_provider, "KWREM");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_DOUBLESTAR, symbol_provider, "DOUBLESTAR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWABS, symbol_provider, "KWABS");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_KWNOT, symbol_provider, "KWNOT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_APOSTROPHE, symbol_provider, "APOSTROPHE");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_LEFTPAREN, symbol_provider, "LEFTPAREN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_RIGHTPAREN, symbol_provider, "RIGHTPAREN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_COMMA, symbol_provider, "COMMA");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_VARASSIGN, symbol_provider, "VARASSIGN");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_COLON, symbol_provider, "COLON");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_SEMICOLON, symbol_provider, "SEMICOLON");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_ARROW, symbol_provider, "ARROW");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_BOX, symbol_provider, "BOX");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_BAR, symbol_provider, "BAR");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_DOT, symbol_provider, "DOT");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_NUMBERLITERAL, symbol_provider, "NUMBERLITERAL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_CHARACTERLITERAL, symbol_provider, "CHARACTERLITERAL");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_IDENTIFIER, symbol_provider, "IDENTIFIER");
	l_connect_symbol(connected_syms, MUL_VHDL_SYM_STRINGLITERAL, symbol_provider, "STRINGLITERAL");


	CatStringWo *t_eol_comment = cat_string_wo_new_with("EOL_COMMENT");
	GroRunSymbol *sym_eol = grorun_symbol_new(FALSE, t_eol_comment, 1000);
	cat_array_wo_append(connected_syms, (GObject *) sym_eol);
	cat_unref_ptr(sym_eol);
	cat_unref_ptr(t_eol_comment);
//
//	CatStringWo *t_full_comment = cat_string_wo_new_with("FULL_COMMENT");
//	priv->connected_symbols[SPI_SYM_FULL_COMMENT] = grorun_symbol_new(FALSE, t_full_comment, 1001);
//	cat_unref_ptr(t_full_comment);



	priv->keyword_mapping = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	l_add_kw_mapping(priv->keyword_mapping, "access", MUL_VHDL_SYM_KWACCESS);
	l_add_kw_mapping(priv->keyword_mapping, "after", MUL_VHDL_SYM_KWAFTER);
	l_add_kw_mapping(priv->keyword_mapping, "alias", MUL_VHDL_SYM_KWALIAS);
	l_add_kw_mapping(priv->keyword_mapping, "all", MUL_VHDL_SYM_KWALL);
	l_add_kw_mapping(priv->keyword_mapping, "and", MUL_VHDL_SYM_KWAND);
	l_add_kw_mapping(priv->keyword_mapping, "architecture", MUL_VHDL_SYM_KWARCHITECTURE);
	l_add_kw_mapping(priv->keyword_mapping, "array", MUL_VHDL_SYM_KWARRAY);
	l_add_kw_mapping(priv->keyword_mapping, "assert", MUL_VHDL_SYM_KWASSERT);
	l_add_kw_mapping(priv->keyword_mapping, "attribute", MUL_VHDL_SYM_KWATTRIBUTE);
	l_add_kw_mapping(priv->keyword_mapping, "begin", MUL_VHDL_SYM_KWBEGIN);
	l_add_kw_mapping(priv->keyword_mapping, "block", MUL_VHDL_SYM_KWBLOCK);
	l_add_kw_mapping(priv->keyword_mapping, "body", MUL_VHDL_SYM_KWBODY);
	l_add_kw_mapping(priv->keyword_mapping, "buffer", MUL_VHDL_SYM_KWBUFFER);
	l_add_kw_mapping(priv->keyword_mapping, "bus", MUL_VHDL_SYM_KWBUS);
	l_add_kw_mapping(priv->keyword_mapping, "case", MUL_VHDL_SYM_KWCASE);
	l_add_kw_mapping(priv->keyword_mapping, "component", MUL_VHDL_SYM_KWCOMPONENT);
	l_add_kw_mapping(priv->keyword_mapping, "configuration", MUL_VHDL_SYM_KWCONFIGURATION);
	l_add_kw_mapping(priv->keyword_mapping, "constant", MUL_VHDL_SYM_KWCONSTANT);
	l_add_kw_mapping(priv->keyword_mapping, "disconnect", MUL_VHDL_SYM_KWDISCONNECT);
	l_add_kw_mapping(priv->keyword_mapping, "downto", MUL_VHDL_SYM_KWDOWNTO);
	l_add_kw_mapping(priv->keyword_mapping, "else", MUL_VHDL_SYM_KWELSE);
	l_add_kw_mapping(priv->keyword_mapping, "elsif", MUL_VHDL_SYM_KWELSIF);
	l_add_kw_mapping(priv->keyword_mapping, "end", MUL_VHDL_SYM_KWEND);
	l_add_kw_mapping(priv->keyword_mapping, "entity", MUL_VHDL_SYM_KWENTITY);
	l_add_kw_mapping(priv->keyword_mapping, "exit", MUL_VHDL_SYM_KWEXIT);
	l_add_kw_mapping(priv->keyword_mapping, "file", MUL_VHDL_SYM_KWFILE);
	l_add_kw_mapping(priv->keyword_mapping, "for", MUL_VHDL_SYM_KWFOR);
	l_add_kw_mapping(priv->keyword_mapping, "function", MUL_VHDL_SYM_KWFUNCTION);
	l_add_kw_mapping(priv->keyword_mapping, "generate", MUL_VHDL_SYM_KWGENERATE);
	l_add_kw_mapping(priv->keyword_mapping, "generic", MUL_VHDL_SYM_KWGENERIC);
	l_add_kw_mapping(priv->keyword_mapping, "guarded", MUL_VHDL_SYM_KWGUARDED);
	l_add_kw_mapping(priv->keyword_mapping, "if", MUL_VHDL_SYM_KWIF);
	l_add_kw_mapping(priv->keyword_mapping, "impure", MUL_VHDL_SYM_KWIMPURE);
	l_add_kw_mapping(priv->keyword_mapping, "in", MUL_VHDL_SYM_KWIN);
	l_add_kw_mapping(priv->keyword_mapping, "inertial", MUL_VHDL_SYM_KWINERTIAL);
	l_add_kw_mapping(priv->keyword_mapping, "inout", MUL_VHDL_SYM_KWINOUT);
	l_add_kw_mapping(priv->keyword_mapping, "is", MUL_VHDL_SYM_KWIS);
	l_add_kw_mapping(priv->keyword_mapping, "label", MUL_VHDL_SYM_KWLABEL);
	l_add_kw_mapping(priv->keyword_mapping, "library", MUL_VHDL_SYM_KWLIBRARY);
	l_add_kw_mapping(priv->keyword_mapping, "linkage", MUL_VHDL_SYM_KWLINKAGE);
	l_add_kw_mapping(priv->keyword_mapping, "loop", MUL_VHDL_SYM_KWLOOP);
	l_add_kw_mapping(priv->keyword_mapping, "map", MUL_VHDL_SYM_KWMAP);
	l_add_kw_mapping(priv->keyword_mapping, "nand", MUL_VHDL_SYM_KWNAND);
	l_add_kw_mapping(priv->keyword_mapping, "new", MUL_VHDL_SYM_KWNEW);
	l_add_kw_mapping(priv->keyword_mapping, "next", MUL_VHDL_SYM_KWNEXT);
	l_add_kw_mapping(priv->keyword_mapping, "nor", MUL_VHDL_SYM_KWNOR);
	l_add_kw_mapping(priv->keyword_mapping, "null", MUL_VHDL_SYM_KWNULL);
	l_add_kw_mapping(priv->keyword_mapping, "of", MUL_VHDL_SYM_KWOF);
	l_add_kw_mapping(priv->keyword_mapping, "on", MUL_VHDL_SYM_KWON);
	l_add_kw_mapping(priv->keyword_mapping, "open", MUL_VHDL_SYM_KWOPEN);
	l_add_kw_mapping(priv->keyword_mapping, "or", MUL_VHDL_SYM_KWOR);
	l_add_kw_mapping(priv->keyword_mapping, "others", MUL_VHDL_SYM_KWOTHERS);
	l_add_kw_mapping(priv->keyword_mapping, "out", MUL_VHDL_SYM_KWOUT);
	l_add_kw_mapping(priv->keyword_mapping, "package", MUL_VHDL_SYM_KWPACKAGE);
	l_add_kw_mapping(priv->keyword_mapping, "port", MUL_VHDL_SYM_KWPORT);
	l_add_kw_mapping(priv->keyword_mapping, "postponed", MUL_VHDL_SYM_KWPOSTPONED);
	l_add_kw_mapping(priv->keyword_mapping, "procedure", MUL_VHDL_SYM_KWPROCEDURE);
	l_add_kw_mapping(priv->keyword_mapping, "process", MUL_VHDL_SYM_KWPROCESS);
	l_add_kw_mapping(priv->keyword_mapping, "pure", MUL_VHDL_SYM_KWPURE);
	l_add_kw_mapping(priv->keyword_mapping, "range", MUL_VHDL_SYM_KWRANGE);
	l_add_kw_mapping(priv->keyword_mapping, "reverse_range", MUL_VHDL_SYM_KWREVERSE_RANGE);
	l_add_kw_mapping(priv->keyword_mapping, "record", MUL_VHDL_SYM_KWRECORD);
	l_add_kw_mapping(priv->keyword_mapping, "register", MUL_VHDL_SYM_KWREGISTER);
	l_add_kw_mapping(priv->keyword_mapping, "reject", MUL_VHDL_SYM_KWREJECT);
	l_add_kw_mapping(priv->keyword_mapping, "report", MUL_VHDL_SYM_KWREPORT);
	l_add_kw_mapping(priv->keyword_mapping, "return", MUL_VHDL_SYM_KWRETURN);
	l_add_kw_mapping(priv->keyword_mapping, "rol", MUL_VHDL_SYM_KWROL);
	l_add_kw_mapping(priv->keyword_mapping, "ror", MUL_VHDL_SYM_KWROR);
	l_add_kw_mapping(priv->keyword_mapping, "select", MUL_VHDL_SYM_KWSELECT);
	l_add_kw_mapping(priv->keyword_mapping, "severity", MUL_VHDL_SYM_KWSEVERITY);
	l_add_kw_mapping(priv->keyword_mapping, "signal", MUL_VHDL_SYM_KWSIGNAL);
	l_add_kw_mapping(priv->keyword_mapping, "sla", MUL_VHDL_SYM_KWSLA);
	l_add_kw_mapping(priv->keyword_mapping, "sll", MUL_VHDL_SYM_KWSLL);
	l_add_kw_mapping(priv->keyword_mapping, "sra", MUL_VHDL_SYM_KWSRA);
	l_add_kw_mapping(priv->keyword_mapping, "srl", MUL_VHDL_SYM_KWSRL);
	l_add_kw_mapping(priv->keyword_mapping, "subtype", MUL_VHDL_SYM_KWSUBTYPE);
	l_add_kw_mapping(priv->keyword_mapping, "then", MUL_VHDL_SYM_KWTHEN);
	l_add_kw_mapping(priv->keyword_mapping, "to", MUL_VHDL_SYM_KWTO);
	l_add_kw_mapping(priv->keyword_mapping, "transport", MUL_VHDL_SYM_KWTRANSPORT);
	l_add_kw_mapping(priv->keyword_mapping, "type", MUL_VHDL_SYM_KWTYPE);
	l_add_kw_mapping(priv->keyword_mapping, "units", MUL_VHDL_SYM_KWUNITS);
	l_add_kw_mapping(priv->keyword_mapping, "until", MUL_VHDL_SYM_KWUNTIL);
	l_add_kw_mapping(priv->keyword_mapping, "use", MUL_VHDL_SYM_KWUSE);
	l_add_kw_mapping(priv->keyword_mapping, "variable", MUL_VHDL_SYM_KWVARIABLE);
	l_add_kw_mapping(priv->keyword_mapping, "wait", MUL_VHDL_SYM_KWWAIT);
	l_add_kw_mapping(priv->keyword_mapping, "when", MUL_VHDL_SYM_KWWHEN);
	l_add_kw_mapping(priv->keyword_mapping, "while", MUL_VHDL_SYM_KWWHILE);
	l_add_kw_mapping(priv->keyword_mapping, "with", MUL_VHDL_SYM_KWWITH);
	l_add_kw_mapping(priv->keyword_mapping, "xnor", MUL_VHDL_SYM_KWXNOR);
	l_add_kw_mapping(priv->keyword_mapping, "xor", MUL_VHDL_SYM_KWXOR);
	l_add_kw_mapping(priv->keyword_mapping, "mod", MUL_VHDL_SYM_KWMOD);
	l_add_kw_mapping(priv->keyword_mapping, "rem", MUL_VHDL_SYM_KWREM);
	l_add_kw_mapping(priv->keyword_mapping, "abs", MUL_VHDL_SYM_KWABS);
	l_add_kw_mapping(priv->keyword_mapping, "not", MUL_VHDL_SYM_KWNOT);

	return result;
}

MulVhdlScanner *mul_vhdl_scanner_factory_create_scanner(MulVhdlScannerFactory *factory, CatIUtf8Scanner *scanner) {
	MulVhdlScannerFactoryPrivate *priv = mul_vhdl_scanner_factory_get_instance_private(factory);
	return mul_vhdl_scanner_new(priv->token_factory, scanner, priv->keyword_mapping, priv->connected_symbols);
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
