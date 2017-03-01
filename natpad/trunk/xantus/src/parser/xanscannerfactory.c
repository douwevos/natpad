/*
   File:    xanscannerfactory.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 11, 2017
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

#include "xanscannerfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "XanScannerFactory"
#include <logging/catlog.h>

struct _XanScannerFactoryPrivate {
	CatArrayWo *symbols;
	GroRunITokenFactory *token_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(XanScannerFactory, xan_scanner_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(XanScannerFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void xan_scanner_factory_class_init(XanScannerFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void xan_scanner_factory_init(XanScannerFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	XanScannerFactory *instance = XAN_SCANNER_FACTORY(object);
//	XanScannerFactoryPrivate *priv = xan_scanner_factory_get_instance_private(instance);
	G_OBJECT_CLASS(xan_scanner_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_scanner_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_create_symbol(CatArrayWo *symbols, const char *text, int id) {
	CatStringWo *t = text==NULL ? NULL : cat_string_wo_new_data(text);
	GroRunSymbol *sym = grorun_symbol_new(TRUE, t, id);
	cat_unref_ptr(t);
	cat_array_wo_append(symbols, (GObject *) sym);
	cat_unref_ptr(sym);
}

XanScannerFactory *xan_scanner_factory_new() {
	XanScannerFactory *result = g_object_new(XAN_TYPE_SCANNER_FACTORY, NULL);
	cat_ref_anounce(result);
	XanScannerFactoryPrivate *priv = xan_scanner_factory_get_instance_private(result);
	priv->symbols = cat_array_wo_new();

	l_create_symbol(priv->symbols, "EOT", XAN_SYM_END_OF_INPUT);
	l_create_symbol(priv->symbols, "<", XAN_SYM_ELEMENT_OPEN);
	l_create_symbol(priv->symbols, ">", XAN_SYM_ELEMENT_CLOSE);
	l_create_symbol(priv->symbols, NULL, XAN_SYM_LITERAL);
	l_create_symbol(priv->symbols, "=", XAN_SYM_IS);
	l_create_symbol(priv->symbols, NULL, XAN_SYM_ID);
	l_create_symbol(priv->symbols, NULL, XAN_SYM_TEXT);
	l_create_symbol(priv->symbols, NULL, XAN_SYM_COMMENT);
	l_create_symbol(priv->symbols, "</", XAN_SYM_TAG_OPEN_TERMINATOR);
	l_create_symbol(priv->symbols, "<", XAN_SYM_TAG_OPEN);
	l_create_symbol(priv->symbols, "..", XAN_SYM_TAG);
	l_create_symbol(priv->symbols, NULL, XAN_SYM_NAME);
	l_create_symbol(priv->symbols, ">", XAN_SYM_TAG_CLOSE);
	l_create_symbol(priv->symbols, "/>", XAN_SYM_TAG_TERMINATOR_CLOSE);

	priv->token_factory = (GroRunITokenFactory *) grorun_full_token_factory_new();
	return result;
}




XanScanner *xan_scanner_factory_create_scanner(XanScannerFactory *factory, CatIUtf8Scanner *scanner) {
	XanScannerFactoryPrivate *priv = xan_scanner_factory_get_instance_private(factory);
	return xan_scanner_new(priv->token_factory, scanner, priv->symbols);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	XanScannerFactory *instance = XAN_SCANNER_FACTORY(self);
	XanScannerFactoryPrivate *priv = xan_scanner_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
