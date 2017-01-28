/*
   File:    groruntransitionsforsymbol.c
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

#include "groruntransitionsforsymbol.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunTransitionsForSymbol"
#include <logging/catlog.h>

struct _GroRunTransitionsForSymbolPrivate {
	GroRunSymbol *symbol;
	CatArrayWo *transitions;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunTransitionsForSymbol, grorun_transitions_for_symbol, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunTransitionsForSymbol)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_transitions_for_symbol_class_init(GroRunTransitionsForSymbolClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_transitions_for_symbol_init(GroRunTransitionsForSymbol *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunTransitionsForSymbol *instance = GRORUN_TRANSITIONS_FOR_SYMBOL(object);
	GroRunTransitionsForSymbolPrivate *priv = grorun_transitions_for_symbol_get_instance_private(instance);
	cat_unref_ptr(priv->symbol);
	cat_unref_ptr(priv->transitions);
	G_OBJECT_CLASS(grorun_transitions_for_symbol_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_transitions_for_symbol_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunTransitionsForSymbol *grorun_transitions_for_symbol_new(GroRunSymbol *symbol) {
	GroRunTransitionsForSymbol *result = g_object_new(GRORUN_TYPE_TRANSITIONS_FOR_SYMBOL, NULL);
	cat_ref_anounce(result);
	GroRunTransitionsForSymbolPrivate *priv = grorun_transitions_for_symbol_get_instance_private(result);
	priv->symbol = cat_ref_ptr(symbol);
	priv->transitions = cat_array_wo_new();
	return result;
}

void grorun_transitions_for_symbol_add(GroRunTransitionsForSymbol *trans_for_sym, GroRunTransition *transition) {
	GroRunTransitionsForSymbolPrivate *priv = grorun_transitions_for_symbol_get_instance_private(trans_for_sym);
	cat_array_wo_append(priv->transitions, (GObject *) transition);
}

int grorun_transitions_for_symbol_count(GroRunTransitionsForSymbol *trans_for_sym) {
	GroRunTransitionsForSymbolPrivate *priv = grorun_transitions_for_symbol_get_instance_private(trans_for_sym);
	return cat_array_wo_size(priv->transitions);
}

GroRunTransition *grorun_transitions_for_symbol_get(GroRunTransitionsForSymbol *trans_for_sym, int index) {
	GroRunTransitionsForSymbolPrivate *priv = grorun_transitions_for_symbol_get_instance_private(trans_for_sym);
	return (GroRunTransition *) cat_array_wo_get(priv->transitions, index);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunTransitionsForSymbol *instance = GRORUN_TRANSITIONS_FOR_SYMBOL(self);
	GroRunTransitionsForSymbolPrivate *priv = grorun_transitions_for_symbol_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, symbol=%O, transitions=%O]", iname, self, priv->symbol, priv->transitions);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
