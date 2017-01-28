/*
   File:    grorunsymbol.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 10, 2016
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

#include "grorunsymbol.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunSymbol"
#include <logging/catlog.h>

struct _GroRunSymbolPrivate {
	gboolean is_non_terminal;
	const CatStringWo *name;
	int id;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunSymbol, grorun_symbol, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunSymbol)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_symbol_class_init(GroRunSymbolClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_symbol_init(GroRunSymbol *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunSymbol *instance = GRORUN_SYMBOL(object);
	GroRunSymbolPrivate *priv = grorun_symbol_get_instance_private(instance);
	cat_unref_ptr(priv->name);
	G_OBJECT_CLASS(grorun_symbol_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_symbol_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunSymbol *grorun_symbol_new(gboolean is_non_terminal, const CatStringWo *name, int id) {
	GroRunSymbol *result = g_object_new(GRORUN_TYPE_SYMBOL, NULL);
	cat_ref_anounce(result);
	GroRunSymbolPrivate *priv = grorun_symbol_get_instance_private(result);
	priv->is_non_terminal = is_non_terminal;
	priv->name = cat_ref_ptr(name);
	priv->id = id;
	return result;
}

const CatStringWo *grorun_symbol_get_name(GroRunSymbol *symbol) {
	GroRunSymbolPrivate *priv = grorun_symbol_get_instance_private(symbol);
	return priv->name;
}

int grorun_symbol_hash(GroRunSymbol *symbol) {
	GroRunSymbolPrivate *priv = grorun_symbol_get_instance_private(symbol);
	return priv->id;

}

gboolean grorun_symbol_equal(const GroRunSymbol *symbol_a, const GroRunSymbol *symbol_b) {
	return symbol_a == symbol_b;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunSymbol *instance = GRORUN_SYMBOL(self);
	GroRunSymbolPrivate *priv = grorun_symbol_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p name=%O, id=%d]", iname, self, priv->name, priv->id);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
