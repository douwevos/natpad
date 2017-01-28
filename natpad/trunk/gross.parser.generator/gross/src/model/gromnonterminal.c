
/*
   File:    gromnonterminal.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 25, 2016
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

#include "gromnonterminal.h"
#include "gromproduction.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroMNonTerminal"
#include <logging/catlog.h>

struct _GroMNonTerminalPrivate {
	GroTerminalSet *first_set;
	CatHashSet *productions;
	gboolean nullable_set;
	gboolean nullable;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroMNonTerminal, gro_m_non_terminal, GRO_TYPE_M_SYMBOL,
		G_ADD_PRIVATE(GroMNonTerminal)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_m_non_terminal_class_init(GroMNonTerminalClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_m_non_terminal_init(GroMNonTerminal *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroMNonTerminal *instance = GRO_M_NON_TERMINAL(object);
//	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(instance);
	G_OBJECT_CLASS(gro_m_non_terminal_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_m_non_terminal_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroMNonTerminal *gro_m_non_terminal_new(int index, GroBnfString *name) {
	GroMNonTerminal *result = g_object_new(GRO_TYPE_M_NON_TERMINAL, NULL);
	cat_ref_anounce(result);
	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(result);
	gro_m_symbol_construct((GroMSymbol *) result, index, name);
	priv->first_set = gro_terminal_set_new();
	priv->nullable = FALSE;
	priv->nullable_set = FALSE;
	priv->productions = cat_hash_set_new((GHashFunc) gro_m_production_hash, (GEqualFunc) gro_m_production_equal);
	return result;
}

void gro_m_non_terminal_add_production(GroMNonTerminal *non_term, GroMProduction *production) {
	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(non_term);
	cat_hash_set_add(priv->productions, (GObject *) production);
}

CatIIterator *gro_m_non_terminal_production_iterator(GroMNonTerminal *non_term) {
	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(non_term);
	return cat_hash_set_iterator(priv->productions);
}


GroTerminalSet *gro_m_non_terminal_get_first_set(GroMNonTerminal *non_term) {
	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(non_term);
	return priv->first_set;
}

gboolean gro_m_non_terminal_is_nullable(GroMNonTerminal *non_term) {
	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(non_term);
	return priv->nullable;
}

gboolean gro_m_non_terminal_compute_nullable(GroMNonTerminal *non_term) {
	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(non_term);
	gboolean computed = FALSE;
	CatIIterator *iter = cat_hash_set_iterator(priv->productions);
	while(cat_iiterator_has_next(iter)) {
		GroMProduction *prod = (GroMProduction *) cat_iiterator_next(iter);
		if (gro_m_production_check_nullable(prod)) {
			computed = TRUE;
		}
	}
	cat_unref_ptr(iter);
	priv->nullable_set = TRUE;
	if (priv->nullable!=computed) {
		priv->nullable = computed;
		return TRUE;
	}
	return FALSE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroMNonTerminal *instance = GRO_M_NON_TERMINAL(self);
	GroMNonTerminalPrivate *priv = gro_m_non_terminal_get_instance_private(instance);

	const char *nlt;
	if (!priv->nullable_set) {
		nlt = "nb-not-set";
	} else if (priv->nullable) {
		nlt = "is-nb";
	} else {
		nlt = "not-nb";
	}
	cat_string_wo_format(append_to, "NonTerm[id=%d, '%O', %s]", gro_m_symbol_get_index((GroMSymbol *) instance), gro_m_symbol_get_name((GroMSymbol *) self), nlt);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
