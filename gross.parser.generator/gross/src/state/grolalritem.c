
/*
   File:    grolalritem.c
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

#include "grolalritem.h"
#include "../model/gromprodsymbolpart.h"
#include "../model/gromprodactionpart.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroLalrItem"
#include <logging/catlog.h>

struct _GroLalrItemPrivate {
	GroMProduction *production;
	int dot_index;
	int hash_code;
	GroTerminalSet *lookahead;
	GroMSymbol *symbol_after_dot;
	CatArrayWo *propagate_items;	/* <GroLalrItem> */
	gboolean needs_propagation;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroLalrItem, gro_lalr_item, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroLalrItem)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_lalr_item_class_init(GroLalrItemClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_lalr_item_init(GroLalrItem *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroLalrItem *instance = GRO_LALR_ITEM(object);
//	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(instance);
	G_OBJECT_CLASS(gro_lalr_item_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_lalr_item_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_finish_construct(GroLalrItemPrivate *priv) {
	priv->propagate_items = cat_array_wo_new();
	priv->needs_propagation = TRUE;
	priv->hash_code = gro_m_production_hash(priv->production)*111+priv->dot_index;
	priv->symbol_after_dot = NULL;
	if (priv->dot_index<gro_m_production_rhs_count(priv->production)) {
		GroMProdPart *nsym = gro_m_production_rhs_at(priv->production, priv->dot_index);
		if (!GRO_IS_M_PROD_ACTION_PART(nsym)) {
			priv->symbol_after_dot = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) nsym);
		}
	}
}

GroLalrItem *gro_lalr_item_new(GroMProduction *production) {
	GroLalrItem *result = g_object_new(GRO_TYPE_LALR_ITEM, NULL);
	cat_ref_anounce(result);
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(result);
	priv->production = cat_ref_ptr(production);
	priv->dot_index = 0;
	priv->lookahead = gro_terminal_set_new();
	l_finish_construct(priv);
	return result;
}

GroLalrItem *gro_lalr_item_new_full(GroMProduction *production, int dot_index, GroTerminalSet *lookahead) {
	GroLalrItem *result = g_object_new(GRO_TYPE_LALR_ITEM, NULL);
	cat_ref_anounce(result);
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(result);
	priv->production = cat_ref_ptr(production);
	priv->dot_index = dot_index;
	priv->lookahead = cat_ref_ptr(lookahead);
	l_finish_construct(priv);
	return result;
}

int gro_lalr_item_get_dot_index(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private((GroLalrItem *) item);
	return priv->dot_index;
}

gboolean gro_lalr_item_is_dot_at_end(const GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private((GroLalrItem *) item);
	return priv->dot_index>=gro_m_production_rhs_count(priv->production);
}


GroMSymbol *gro_lalr_item_get_symbol_after_dot(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	return priv->symbol_after_dot;
}

GroMSymbol *gro_lalr_item_get_first_symbol(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	GroMProdPart *nsym = gro_m_production_rhs_at(priv->production, 0);
	return gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) nsym);
}


GroMProduction *gro_lalr_item_get_production(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	return priv->production;
}


void gro_lalr_item_add_propagate(GroLalrItem *item, GroLalrItem *prop_to) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	cat_array_wo_append(priv->propagate_items, (GObject *) prop_to);
	priv->needs_propagation = TRUE;
}

CatArrayWo *gro_lalr_item_get_propagate_items(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	return priv->propagate_items;
}


GroTerminalSet *gro_lalr_item_get_lookahead_set(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	return priv->lookahead;
}


gboolean gro_lalr_item_is_lookahead_visible(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);

	/*
	 * if the dot is at the end, we have a problem, but the cleanest thing
	 * to do is just return true.
	 */
	int rhs_count = gro_m_production_rhs_count(priv->production);
	if (priv->dot_index>=rhs_count) {
		return TRUE;
	}


	/* walk down the rhs and bail if we get a non-nullable symbol */
	int pos;
	for (pos = priv->dot_index + 1; pos < rhs_count; pos++) {
		GroMProdPart *part = gro_m_production_rhs_at(priv->production, pos);

		/* only consider symbols, skip actions */
		if (GRO_IS_M_PROD_SYMBOL_PART(part)) {
			GroMSymbol *sym = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) part);

			/* if its a terminal we fail */
			if (GRO_IS_M_TERMINAL(sym)) {
				return FALSE;
			}

			/* if its not nullable we fail */
			if (!gro_m_non_terminal_is_nullable((GroMNonTerminal *) sym)) {
				return FALSE;
			}
		}
	}

	/* if we get here its all nullable */
	return TRUE;
}


GroTerminalSet *gro_lalr_item_calculate_lookahead(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	GroTerminalSet *result = gro_terminal_set_new();

//	/* sanity check */
//	if (dotAtEnd()) {
//		throw new FatalCupException("Attempt to calculate a lookahead set with a completed item");
//	}

	/* start with an empty result */

	cat_log_debug("calc-lah for:%O", priv->production);
	/* consider all nullable symbols after the one to the right of the dot */
	int pos;
	int rhs_count = gro_m_production_rhs_count(priv->production);
	for (pos = priv->dot_index + 1; pos < rhs_count; pos++) {
		GroMProdPart *part = gro_m_production_rhs_at(priv->production, pos);

		/* consider what kind of production part it is -- skip actions */
		if (GRO_IS_M_PROD_SYMBOL_PART(part)) {
			GroMSymbol *sym = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) part);

			/* if its a terminal add it in and we are done */
			if (!GRO_IS_M_NON_TERMINAL(sym)) {
				gro_terminal_set_add(result, (GroMTerminal *) sym);
				return result;
			} else {
				/* otherwise add in first set of the non terminal */
				GroMNonTerminal *non_term = (GroMNonTerminal *) sym;
				GroTerminalSet *first_set = gro_m_non_terminal_get_first_set(non_term);
//				cat_log_debug("look:non_term=%o, first_set=%o", non_term, first_set);
				gro_terminal_set_add_all(result, first_set);

				cat_log_debug("nt_first_set=%O, sym=%O", first_set, sym);


				/* if its nullable we continue adding, if not, we are done */
				if (!gro_m_non_terminal_is_nullable(non_term)) {
					return result;
				}
			}
		}
	}

	/*
	 * if we get here everything past the dot was nullable we add in the
	 * lookahead for after the production and we are done
	 */
	gro_terminal_set_add_all(result, priv->lookahead);
	return result;
}

void gro_lalr_item_propagate_lookaheads(GroLalrItem *item, GroTerminalSet *incoming) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	gboolean change = FALSE;

	/* if we don't need to propagate, then bail out now */
	if (!priv->needs_propagation && (incoming==NULL  || gro_terminal_set_is_empty(incoming)))
		return;

	/* if we have null incoming, treat as an empty set */
	if (incoming != NULL) {
		/* add the incoming to the lookahead of this item */
		change = gro_terminal_set_add_all(priv->lookahead, incoming);
	}

	/* if we changed or need it anyway, propagate across our links */
	if (change || priv->needs_propagation) {
		/* don't need to propagate again */
		priv->needs_propagation = FALSE;

		/* propagate our lookahead into each item we are linked to */
		CatIIterator *iter = cat_array_wo_iterator(priv->propagate_items);
		while(cat_iiterator_has_next(iter)) {
			GroLalrItem *pitem = (GroLalrItem *) cat_iiterator_next(iter);
			gro_lalr_item_propagate_lookaheads(pitem, priv->lookahead);
		}
		cat_unref_ptr(iter);
	}
}



GroLalrItem *gro_lalr_item_shift(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);

	/* can't shift if we have dot already at the end */
// TODO
//	if (dotAtEnd()) {
//		throw new FatalCupException("Attempt to shift past end of an lalr_item");
//	}

	GroTerminalSet *nlas = gro_terminal_set_clone(priv->lookahead);
	GroLalrItem *result = gro_lalr_item_new_full(priv->production, priv->dot_index+1, nlas);
	cat_unref_ptr(nlas);

	/* change in our lookahead needs to be propagated to this item */
	gro_lalr_item_add_propagate(item, result);

	return result;

}


gboolean gro_lalr_item_equal(const GroLalrItem *item_a, const GroLalrItem *item_b) {
	if (item_a==item_b) {
		return TRUE;
	}
	if (item_a==NULL || item_b==NULL) {
		return TRUE;
	}

	GroLalrItemPrivate *priv_a = gro_lalr_item_get_instance_private((GroLalrItem *) item_a);
	GroLalrItemPrivate *priv_b = gro_lalr_item_get_instance_private((GroLalrItem *) item_b);
	return (priv_a->dot_index == priv_b->dot_index)
			&& gro_m_production_equal(priv_a->production, priv_b->production);
}

int gro_lalr_item_hash(GroLalrItem *item) {
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(item);
	return priv->hash_code;
}


/********************* start CatIStringable implementation *********************/



static void l_print_base(GroLalrItem *self, CatStringWo *append_to) {
	GroLalrItem *instance = GRO_LALR_ITEM(self);
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[", iname);


	GroMSymbol *lhs_sym = gro_m_production_get_lhs_symbol(priv->production);
	CatStringWo *name = lhs_sym==NULL ? NULL : gro_m_symbol_get_name(lhs_sym);
	if (name) {
		cat_string_wo_append(append_to, name);
	} else {
		cat_string_wo_append_chars(append_to, "<nil>");
	}

	cat_string_wo_append_chars(append_to, " ::= ");


	int pos;
	int rhs_count = gro_m_production_rhs_count(priv->production);
	for (pos=0; pos < rhs_count; pos++) {
		if (pos==priv->dot_index) {
			cat_string_wo_append_chars(append_to, "(*) ");
		}
		GroMProdPart *part = gro_m_production_rhs_at(priv->production, pos);
		if (GRO_IS_M_PROD_SYMBOL_PART(part)) {
			GroMSymbol *sym = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) part);
			cat_string_wo_format(append_to, "%O ", gro_m_symbol_get_name(sym));
		} else if (part!=NULL) {
			cat_string_wo_append_chars(append_to, "{action} ");
		} else  {
			cat_string_wo_append_chars(append_to, "<nil> ");
		}
	}
	if (priv->dot_index>=rhs_count) {
		cat_string_wo_append_chars(append_to, "(*) ");
	}
	cat_string_wo_append_chars(append_to, "]");
}

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroLalrItem *instance = GRO_LALR_ITEM(self);
	GroLalrItemPrivate *priv = gro_lalr_item_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[", iname);


	GroMSymbol *lhs_sym = gro_m_production_get_lhs_symbol(priv->production);
	CatStringWo *name = lhs_sym==NULL ? NULL : gro_m_symbol_get_name(lhs_sym);
	if (name) {
		cat_string_wo_append(append_to, name);
	} else {
		cat_string_wo_append_chars(append_to, "<nil>");
	}

	cat_string_wo_append_chars(append_to, " ::= ");


	int pos;
	int rhs_count = gro_m_production_rhs_count(priv->production);
	for (pos=0; pos < rhs_count; pos++) {
		if (pos==priv->dot_index) {
			cat_string_wo_append_chars(append_to, "(*) ");
		}
		GroMProdPart *part = gro_m_production_rhs_at(priv->production, pos);
		if (GRO_IS_M_PROD_SYMBOL_PART(part)) {
			GroMSymbol *sym = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) part);
			cat_string_wo_format(append_to, "%O ", gro_m_symbol_get_name(sym));
		} else if (part!=NULL) {
			cat_string_wo_append_chars(append_to, "{action} ");
		} else  {
			cat_string_wo_append_chars(append_to, "<nil> ");
		}
	}
	if (priv->dot_index>=rhs_count) {
		cat_string_wo_append_chars(append_to, "(*) ");
	}
	cat_string_wo_append_chars(append_to, ", LAH={ ");

	if (priv->lookahead) {
		GroMTerminalIterator *term_iter = gro_terminal_set_iterator(priv->lookahead);
		while(gro_mterminal_iterator_has_next(term_iter)) {
			GroMTerminal *term = gro_mterminal_iterator_next(term_iter);
			cat_string_wo_format(append_to, "%O ", gro_m_symbol_get_name((GroMSymbol *) term));
		}
		cat_unref_ptr(term_iter);
	} else {
		cat_string_wo_append_chars(append_to, "<nil> ");
	}

	if (priv->propagate_items) {
		cat_string_wo_append_chars(append_to, "}, PROP={");
		CatIIterator *prp_iter = cat_array_wo_iterator(priv->propagate_items);
		while(cat_iiterator_has_next(prp_iter)) {
			GroLalrItem *prop_item = (GroLalrItem *) cat_iiterator_next(prp_iter);
			l_print_base(prop_item, append_to);
			cat_string_wo_append_chars(append_to, " ");
		}
		cat_unref_ptr(prp_iter);
	}

	cat_string_wo_append_chars(append_to, "}]");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

