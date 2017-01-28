
/*
   File:    gromproduction.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 26, 2016
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

#include "gromproduction.h"
#include "gromprodsymbolpart.h"
#include "gromprodactionpart.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroMProduction"
#include <logging/catlog.h>

struct _GroMProductionPrivate {
	int index;
	GroMNonTerminal *lhs_sym;
	CatArrayWo *rhs_parts;
	gboolean nullable_set;
	gboolean nullable;
	GroTerminalSet *first_set;
	CatStringWo *tail_action;
	int prec_num;
	int prec_side;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroMProduction, gro_m_production, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroMProduction)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_m_production_class_init(GroMProductionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_m_production_init(GroMProduction *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroMProduction *instance = GRO_M_PRODUCTION(object);
//	GroMProductionPrivate *priv = gro_m_production_get_instance_private(instance);
	G_OBJECT_CLASS(gro_m_production_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_m_production_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroMProduction *gro_m_production_new(int index, GroMNonTerminal *lhs_sym, CatArrayWo *rhs_parts, int prec_num, int prec_side) {
	GroMProduction *result = g_object_new(GRO_TYPE_M_PRODUCTION, NULL);
	cat_ref_anounce(result);
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(result);
	priv->index = index;
	priv->lhs_sym = cat_ref_ptr(lhs_sym);
	priv->rhs_parts = cat_ref_ptr(rhs_parts);
	priv->prec_num = prec_num;
	priv->prec_side = prec_side;
	priv->nullable_set = FALSE;
	priv->nullable = FALSE;
	priv->first_set = gro_terminal_set_new();

	priv->tail_action = NULL;
	GroMProdPart *prod_part = cat_array_wo_get_last(rhs_parts);
	cat_log_debug("index=%d, prod_part=%o", index, prod_part);
	if (GRO_IS_M_PROD_ACTION_PART(prod_part)) {
		GroBnfString *action_string = gro_m_prod_action_part_get_code((GroMProdActionPart *) prod_part);
		priv->tail_action = gro_bnf_string_get_text(action_string);
		cat_ref_ptr(priv->tail_action);
		cat_array_wo_remove_last(rhs_parts, NULL);
	}

	/* put us in the production list of the lhs non terminal */
	gro_m_non_terminal_add_production(lhs_sym, result);

	return result;
}

int gro_m_production_get_index(const GroMProduction *prod) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private((GroMProduction *) prod);
	return priv->index;
}

GroMSymbol *gro_m_production_get_lhs_symbol(GroMProduction *production) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(production);
	return (GroMSymbol *) priv->lhs_sym;
}

int gro_m_production_get_precedence_num(GroMProduction *production) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(production);
	return priv->prec_num;
}

CatStringWo *gro_m_production_get_action(GroMProduction *production) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(production);
	return priv->tail_action;
}

int gro_m_production_rhs_count(const GroMProduction *prod) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private((GroMProduction *) prod);
	return cat_array_wo_size(priv->rhs_parts);
}

GroMProdPart *gro_m_production_rhs_at(GroMProduction *prod, int index) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(prod);
	return (GroMProdPart *) cat_array_wo_get(priv->rhs_parts, index);
}

static gboolean l_set_nullable(GroMProductionPrivate *priv, gboolean is_nullable) {
	priv->nullable_set = TRUE;
	priv->nullable = is_nullable;
	return is_nullable;
}

gboolean gro_m_production_check_nullable(GroMProduction *prod) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(prod);

	/* if we already know bail out early */
	if (priv->nullable_set)
		return priv->nullable;

	/* if we have a zero size RHS we are directly nullable */
	int rhs_count = cat_array_wo_size(priv->rhs_parts);
	if (rhs_count == 0) {
		/* stash and return the result */
		return l_set_nullable(priv, TRUE);
	}

	/* otherwise we need to test all of our parts */
	int pos;
	for(pos = 0; pos < rhs_count; pos++) {
		GroMProdPart *part = (GroMProdPart *) cat_array_wo_get(priv->rhs_parts, pos);

		/* only look at non-actions */
		if (GRO_IS_M_PROD_SYMBOL_PART(part)) {
			GroMSymbol *sym = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) part);

			/* if its a terminal we are definitely not nullable */
			if (GRO_IS_M_NON_TERMINAL(sym)) {
				if (!gro_m_non_terminal_is_nullable((GroMNonTerminal *) sym)) {
					/* this one not (yet) nullable, so we aren't */
					return FALSE;
				}
			} else {
				return l_set_nullable(priv, FALSE);
			}
		}
	}

	/* if we make it here all parts are nullable */
	return l_set_nullable(priv, TRUE);
}

GroTerminalSet *gro_m_production_check_first_set(GroMProduction *prod) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(prod);

	int rhs_count = cat_array_wo_size(priv->rhs_parts);

	/* walk down the right hand side till we get past all nullables */
	int pos;
	for(pos = 0; pos < rhs_count; pos++) {
		/* only look at non-actions */
		GroMProdPart *part = (GroMProdPart *) cat_array_wo_get(priv->rhs_parts, pos);

		/* only look at non-actions */
		if (GRO_IS_M_PROD_SYMBOL_PART(part)) {
			GroMSymbol *sym = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) part);

//			cat_log_debug("lhs=%O, pos=%d, sym=%o", priv->lhs_sym, pos, sym);

			/* is it a non-terminal? */
			if (GRO_IS_M_NON_TERMINAL(sym)) {
				/* add in current firsts from that NT */
				GroMNonTerminal *non_term = (GroMNonTerminal *) sym;
				GroTerminalSet *nt_tset = gro_m_non_terminal_get_first_set(non_term);
//				cat_log_debug("lhs=%O, pos=%d, nt_tset=%o", priv->lhs_sym, pos, nt_tset);
				gro_terminal_set_add_all(priv->first_set, nt_tset);

				/* if its not nullable, we are done */
				if (!gro_m_non_terminal_is_nullable(non_term)) {
					break;
				}
			} else {
				/* its a terminal -- add that to the set */
				gro_terminal_set_add(priv->first_set, (GroMTerminal *) sym);
				/* we are done */
				break;
			}
		}
	}
	return priv->first_set;
}


gboolean gro_m_production_equal(const GroMProduction *prod_a, const GroMProduction *prod_b) {
	if (prod_a==prod_b) {
		return TRUE;
	}
	if (prod_a==NULL || prod_b==NULL) {
		return FALSE;
	}
	GroMProductionPrivate *priv_a = gro_m_production_get_instance_private((GroMProduction *) prod_a);
	GroMProductionPrivate *priv_b = gro_m_production_get_instance_private((GroMProduction *) prod_b);
	return priv_a->index==priv_b->index;
}

int gro_m_production_hash(GroMProduction *prod) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(prod);
	return priv->index*13;
}

CatStringWo *gro_m_production_to_simple_string(GroMProduction *prod) {
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(prod);
	CatStringWo *result = cat_string_wo_new();
	if (priv->lhs_sym) {
		cat_string_wo_append(result, gro_m_symbol_get_name((GroMSymbol *) priv->lhs_sym));
	} else {
		cat_string_wo_append_chars(result, "NULL_LHS");
	}
	cat_string_wo_append_chars(result, " ::= ");
	CatIIterator *iter = cat_array_wo_iterator(priv->rhs_parts);
	while(cat_iiterator_has_next(iter)) {
		GroMProdPart *prod_part = cat_iiterator_next(iter);
		if (GRO_IS_M_PROD_SYMBOL_PART(prod_part)) {
			GroMSymbol *ppsym = gro_m_prod_symbol_part_get_symbol((GroMProdSymbolPart *) prod_part);
			cat_string_wo_format(result, "%O ", gro_m_symbol_get_name(ppsym));
		}
	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroMProduction *instance = GRO_M_PRODUCTION(self);
	GroMProductionPrivate *priv = gro_m_production_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	CatStringWo *lhs_name = priv->lhs_sym==NULL ? NULL : gro_m_symbol_get_name((GroMSymbol *) priv->lhs_sym);

	cat_string_wo_format(append_to, "%s[index=%d, sym=%o, rhs", iname, priv->index, lhs_name);

	CatIIterator *iter = cat_array_wo_iterator(priv->rhs_parts);
	while(cat_iiterator_has_next(iter)) {
		GroMProdPart *part = (GroMProdPart *) cat_iiterator_next(iter);
		cat_string_wo_format(append_to, ", %O", part);
	}
	cat_unref_ptr(iter);
	cat_string_wo_append_chars(append_to, "]");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
