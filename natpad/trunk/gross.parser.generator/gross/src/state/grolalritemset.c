
/*
   File:    grolalritemset.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 31, 2016
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

#include "grolalritemset.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroLalrItemSet"
#include <logging/catlog.h>

struct _GroLalrItemSetPrivate {
	CatHashSet *all;	/* <GroLalrItem> */
	int hash_code;
	gboolean hash_code_valid;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroLalrItemSet, gro_lalr_item_set, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroLalrItemSet)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_lalr_item_set_class_init(GroLalrItemSetClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_lalr_item_set_init(GroLalrItemSet *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroLalrItemSet *instance = GRO_LALR_ITEM_SET(object);
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(instance);
	cat_unref_ptr(priv->all);
	G_OBJECT_CLASS(gro_lalr_item_set_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_lalr_item_set_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroLalrItemSet *gro_lalr_item_set_new() {
	GroLalrItemSet *result = g_object_new(GRO_TYPE_LALR_ITEM_SET, NULL);
	cat_ref_anounce(result);
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(result);
	priv->all = cat_hash_set_new((GHashFunc) gro_lalr_item_hash, (GEqualFunc) gro_lalr_item_equal);
	priv->hash_code_valid = FALSE;
	return result;
}


GroLalrItemSet *gro_lalr_item_set_clone(GroLalrItemSet *to_clone) {
	GroLalrItemSet *result = g_object_new(GRO_TYPE_LALR_ITEM_SET, NULL);
	cat_ref_anounce(result);
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(result);
	priv->all = cat_hash_set_new((GHashFunc) gro_lalr_item_hash, (GEqualFunc) gro_lalr_item_equal);
	priv->hash_code_valid = FALSE;

	GroLalrItemSetPrivate *priv_src = gro_lalr_item_set_get_instance_private(to_clone);
	cat_hash_set_add_all(priv->all, priv_src->all);
	return result;

}


static gint l_gro_lalr_item_comp(gconstpointer a, gconstpointer b) {
	GroLalrItem *item_a = (GroLalrItem *) a;
	GroLalrItem *item_b = (GroLalrItem *) b;
	int prod_idx_a = gro_m_production_get_index(gro_lalr_item_get_production(item_a));
	int prod_idx_b = gro_m_production_get_index(gro_lalr_item_get_production(item_b));
	int dot_idx_a = gro_lalr_item_get_dot_index(item_a);
	int dot_idx_b = gro_lalr_item_get_dot_index(item_b);
	int val_a = prod_idx_a*1024 + dot_idx_a;
	int val_b = prod_idx_b*1024 + dot_idx_b;
	return val_a<val_b ? -1 : (val_a>val_b ? 1 : 0);
}


void gro_lalr_item_set_compute_closure(GroLalrItemSet *item_set) {
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(item_set);

	cat_log_debug("Compute-closure");


//	cat_log_debug("priv->all=%o", priv->all);

	CatArrayWo *consider = 	cat_hash_set_enlist_all(priv->all);
	// DEBUGONLY
	cat_array_wo_sort(consider, l_gro_lalr_item_comp);

	while(cat_array_wo_size(consider)>0) {
		GroLalrItem *item;
		// DEBUGONLY
		int idx;
		for(idx=0; idx<cat_array_wo_size(consider); idx++) {
			cat_log_debug("[%d] -- %O", idx, cat_array_wo_get(consider, idx));
		}
//		cat_array_wo_remove_last(consider, (GObject **) &item);
		cat_array_wo_remove_first(consider, (GObject **) &item);

		GroMSymbol *sym_after_dot = gro_lalr_item_get_symbol_after_dot(item);
		if (GRO_IS_M_NON_TERMINAL(sym_after_dot)) {
			cat_log_debug("consider=%o, sym_after_dot=%o", item, sym_after_dot);
			GroMNonTerminal *non_term = (GroMNonTerminal *) sym_after_dot;
			GroTerminalSet *new_lookaheads = gro_lalr_item_calculate_lookahead(item);

			/*
			 * are we going to need to propagate our lookahead to new item
			 */
			gboolean need_propagation = gro_lalr_item_is_lookahead_visible(item);
			cat_log_debug("new_lookaheads=%o%s", new_lookaheads, need_propagation ? ", need-propagation" : "");

// this does not work....
//
//			GroMNonTerminal *skip_non_term = NULL;
//			if (gro_lalr_item_get_dot_index(item)==1) {
//				GroMSymbol *start_sym = gro_lalr_item_get_first_symbol(item);
//				if (GRO_IS_M_NON_TERMINAL(start_sym)) {
//					GroMNonTerminal *start_non_term = (GroMNonTerminal *) start_sym;
//					if (gro_m_non_terminal_is_nullable(start_non_term)) {
//						skip_non_term = start_non_term;
//					}
//
//				}
//			}


//			cat_log_error("skip_non_term:%O", skip_non_term);

			CatIIterator *prod_iter = gro_m_non_terminal_production_iterator(non_term);
			while(cat_iiterator_has_next(prod_iter)) {


				GroMProduction *prod = cat_iiterator_next(prod_iter);
//				if (gro_m_production_get_lhs_symbol(prod) == skip_non_term) {
//					cat_log_error("skipping prod:%O", prod);
//					continue;
//				}
				GroTerminalSet *nlook = gro_terminal_set_clone(new_lookaheads);
				GroLalrItem *new_item = gro_lalr_item_new_full(prod, 0, nlook);
				cat_unref_ptr(nlook);

				/* add/merge item into the set */
				GroLalrItem *add_item = gro_lalr_item_set_add(item_set, new_item);
				/* if propagation is needed link to that item */
				if (need_propagation) {
					gro_lalr_item_add_propagate(item, add_item);
				}

				/* was this was a new item */
				if (add_item == new_item) {
					/* that may need further closure, consider it also */
					cat_array_wo_append(consider, (GObject *) new_item);
				}

			}
			cat_unref_ptr(prod_iter);
		}

	}
	cat_unref_ptr(consider);

}


GroLalrItem *gro_lalr_item_set_add(GroLalrItemSet *item_set, GroLalrItem *item) {
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(item_set);
	GroLalrItem *other = gro_lalr_item_set_find(item_set, item);
//	cat_log_debug("item=%o, other=%o", item, other);
	if (other!=NULL) {
		GroTerminalSet *las_other = gro_lalr_item_get_lookahead_set(other);
		GroTerminalSet *las_item = gro_lalr_item_get_lookahead_set(item);
		gro_terminal_set_add_all(las_other, las_item);
		return other;
	} else {
		/* otherwise we just go in the set */
		/* invalidate cached hashcode */
		priv->hash_code_valid = FALSE;

		cat_hash_set_add(priv->all, (GObject *) item);
//		cat_log_debug("priv->all=%o", priv->all);
		return item;
	}
}

GroLalrItem *gro_lalr_item_set_find(GroLalrItemSet *item_set, const GroLalrItem *item) {
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private((GroLalrItemSet *) item_set);
	GroLalrItem *result = NULL;
	CatIIterator *iter = cat_hash_set_iterator(priv->all);
	while(cat_iiterator_has_next(iter)) {
		GroLalrItem *search = cat_iiterator_next(iter);
		if (gro_lalr_item_equal(item, search)) {
			result = search;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}


GroLalrItemIterator *gro_lalr_item_set_iterator(GroLalrItemSet *item_set) {
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(item_set);
	CatArrayWo *enlisted = cat_hash_set_enlist_all(priv->all);
	cat_array_wo_sort(enlisted, l_gro_lalr_item_comp);

	return (GroLalrItemIterator *) cat_array_wo_iterator(enlisted);
//	return (GroLalrItemIterator *) cat_hash_set_iterator(priv->all);
}


int gro_lalr_item_set_hash(GroLalrItemSet *item_set) {
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(item_set);
	if (!priv->hash_code_valid) {
		CatIIterator *iter = cat_hash_set_iterator(priv->all);
		int result = 0;
		while(cat_iiterator_has_next(iter)) {
			GroLalrItem *item = (GroLalrItem *) cat_iiterator_next(iter);
			int ih = gro_lalr_item_hash(item);
			result = result ^ ih;
		}
		cat_unref_ptr(iter);
		priv->hash_code = result;
		priv->hash_code_valid = TRUE;
	}
	return priv->hash_code;
}

gboolean gro_lalr_item_set_equal(const GroLalrItemSet *item_set_a, const GroLalrItemSet *item_set_b) {
	if (item_set_a==item_set_b) {
		return TRUE;
	}
	if (item_set_a==NULL || item_set_b==NULL) {
		return FALSE;
	}
	const GroLalrItemSetPrivate *priv_a = gro_lalr_item_set_get_instance_private((GroLalrItemSet *) item_set_a);
	const GroLalrItemSetPrivate *priv_b = gro_lalr_item_set_get_instance_private((GroLalrItemSet *) item_set_b);

	return cat_hash_set_equal(priv_a->all, priv_b->all);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroLalrItemSet *instance = GRO_LALR_ITEM_SET(self);
	GroLalrItemSetPrivate *priv = gro_lalr_item_set_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, %o]", iname, self, priv->all);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

