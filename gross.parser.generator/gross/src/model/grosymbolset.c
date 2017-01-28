
/*
   File:    grosymbolset.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 2, 2016
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

#include "grosymbolset.h"
#include "gromnonterminal.h"
#include "gromterminal.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroSymbolSet"
#include <logging/catlog.h>

struct _GroSymbolSetPrivate {
	CatHashMapWo *map;		/* <CatStringWo, GroMSymbol> */
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroSymbolSet, gro_symbol_set, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroSymbolSet)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_symbol_set_class_init(GroSymbolSetClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_symbol_set_init(GroSymbolSet *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroSymbolSet *instance = GRO_SYMBOL_SET(object);
//	GroSymbolSetPrivate *priv = gro_symbol_set_get_instance_private(instance);
	G_OBJECT_CLASS(gro_symbol_set_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_symbol_set_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroSymbolSet *gro_symbol_set_new() {
	GroSymbolSet *result = g_object_new(GRO_TYPE_SYMBOL_SET, NULL);
	cat_ref_anounce(result);
	GroSymbolSetPrivate *priv = gro_symbol_set_get_instance_private(result);
	priv->map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	return result;
}


gboolean gro_symbol_set_add(GroSymbolSet *symbol_set, GroMSymbol *symbol) {
	GroSymbolSetPrivate *priv = gro_symbol_set_get_instance_private(symbol_set);
	CatStringWo *name = gro_m_symbol_get_name(symbol);
	return cat_hash_map_wo_put(priv->map, (GObject *) name, (GObject *) symbol);
}

static gint l_gro_sym_comp(gconstpointer a, gconstpointer b) {
	gboolean a_is_nt = GRO_IS_M_NON_TERMINAL(a);
	gboolean b_is_nt = GRO_IS_M_NON_TERMINAL(b);
	if (a_is_nt && !b_is_nt) {
		return -1;
	} else if (!a_is_nt && b_is_nt) {
		return 1;
	}

	int idx_a = gro_m_symbol_get_index((GroMSymbol *) a);
	int idx_b = gro_m_symbol_get_index((GroMSymbol *) b);
	return idx_a<idx_b ? -1 : (idx_a>idx_b ? 1 : 0);
}

CatIIterator *gro_symbol_set_value_iterator(GroSymbolSet *symbol_set) {
	GroSymbolSetPrivate *priv = gro_symbol_set_get_instance_private(symbol_set);
	CatArrayWo *enlisted = cat_hash_map_wo_enlist_values(priv->map, NULL);

	// TODO : DEBUGONLY
	cat_array_wo_sort(enlisted, (GCompareFunc) l_gro_sym_comp);

	CatIIterator *result = cat_array_wo_iterator(enlisted);
	cat_unref_ptr(enlisted);
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroSymbolSet *instance = GRO_SYMBOL_SET(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p,", iname, self);
	CatIIterator *iter = gro_symbol_set_value_iterator(instance);
	while(cat_iiterator_has_next(iter)) {
		GroMSymbol *sym = (GroMSymbol *) cat_iiterator_next(iter);
		cat_string_wo_format(append_to, ",%O", gro_m_symbol_get_name(sym));
	}
	cat_string_wo_append_chars(append_to, "]");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
