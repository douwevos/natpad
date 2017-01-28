
/*
   File:    gromodel.c
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

#include "gromodel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroModel"
#include <logging/catlog.h>

struct _GroModelPrivate {
	CatHashMapWo *symbol_map;

	CatArrayWo *terminal_list;
	CatArrayWo *non_terminal_list;
	GroMTerminal *terminal_eof;
	GroMTerminal *terminal_error;
	CatArrayWo *productions;
	int sub_prod_seq;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroModel, gro_model, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroModel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_model_class_init(GroModelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_model_init(GroModel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroModel *instance = GRO_MODEL(object);
//	GroModelPrivate *priv = gro_model_get_instance_private(instance);
	G_OBJECT_CLASS(gro_model_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_model_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroModel *gro_model_new() {
	GroModel *result = g_object_new(GRO_TYPE_MODEL, NULL);
	cat_ref_anounce(result);
	GroModelPrivate *priv = gro_model_get_instance_private(result);
	priv->symbol_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	priv->non_terminal_list = cat_array_wo_new();
	priv->terminal_list = cat_array_wo_new();
	priv->productions = cat_array_wo_new();

	GroBnfString *eof_string = gro_bnf_string_new(cat_string_wo_new_data("EOF"), NULL, NULL);
	priv->terminal_eof = gro_model_add_terminal(result, eof_string, NULL, NULL);
	GroBnfString *error_string = gro_bnf_string_new(cat_string_wo_new_data("error"), NULL, NULL);
	priv->terminal_error = gro_model_add_terminal(result, error_string, NULL, NULL);
	GroBnfString *start_string = gro_bnf_string_new(cat_string_wo_new_data("$START"), NULL, NULL);
	gro_model_add_non_terminal(result, start_string, NULL);

	priv->sub_prod_seq = 0;
	return result;
}

void gro_model_compute_nullability(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	gboolean change = TRUE;

	/* repeat this process until there is no change */
	while(change) {
		/* look for a new change */
		change = FALSE;

		/* consider each non-terminal */
		CatIIterator *iter = cat_array_wo_iterator(priv->non_terminal_list);
		while(cat_iiterator_has_next(iter)) {
			GroMNonTerminal *non_term = (GroMNonTerminal *) cat_iiterator_next(iter);
			/* only look at things that aren't already marked nullable */
			if (!gro_m_non_terminal_is_nullable(non_term)) {
				if (gro_m_non_terminal_compute_nullable(non_term)) {
					change = TRUE;
				}
			}
		}
		cat_unref_ptr(iter);
	}

	/* do one last pass over the productions to finalize all of them */
	CatIIterator *iter = cat_array_wo_iterator(priv->productions);
	while(cat_iiterator_has_next(iter)) {
		GroMProduction *production = (GroMProduction *) cat_iiterator_next(iter);
		gro_m_production_check_nullable(production);
	}
	cat_unref_ptr(iter);

}

void gro_model_compute_first_sets(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	gboolean change = TRUE;

	/* repeat this process until we have no change */
	while(change) {
		/* look for a new change */
		change = FALSE;

		/* consider each non-terminal */
		CatIIterator *iter = cat_array_wo_iterator(priv->non_terminal_list);
		while(cat_iiterator_has_next(iter)) {
			GroMNonTerminal *non_term = (GroMNonTerminal *) cat_iiterator_next(iter);
			GroTerminalSet *nt_first_set = gro_m_non_terminal_get_first_set(non_term);

			/* consider every production of that non terminal */
			CatIIterator *prod_iter = gro_m_non_terminal_production_iterator(non_term);
			while(cat_iiterator_has_next(prod_iter)) {
				GroMProduction *production = (GroMProduction *) cat_iiterator_next(prod_iter);

				/* get the updated first of that production */
				GroTerminalSet *prod_first = gro_m_production_check_first_set(production);
//				cat_log_debug("non_term=%o, production=%o, prod_first=%o", non_term, production, prod_first);

				if (gro_terminal_set_add_all(nt_first_set, prod_first)) {
					change = TRUE;
				}
			}
			cat_unref_ptr(prod_iter);

		}
		cat_unref_ptr(iter);
	}
}


GroMSymbol *gro_model_get_symbol_by_name(GroModel *model, CatStringWo *name) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return (GroMSymbol *) cat_hash_map_wo_get(priv->symbol_map, name);
}

GroMTerminal *gro_model_add_terminal(GroModel *model, GroBnfString *term_string, CatStringWo *stack_type, gboolean *created_new) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	CatStringWo *term_name = gro_bnf_string_get_text(term_string);
	GroMSymbol *esym = (GroMSymbol *) cat_hash_map_wo_get(priv->symbol_map, term_name);
	if (esym!=NULL) {
		if (GRO_IS_M_TERMINAL(esym)) {
			if (created_new) {
				*created_new = FALSE;
			}
			return (GroMTerminal *) esym;
		}
		return NULL;
	}

	int index = cat_array_wo_size(priv->terminal_list);
	GroMTerminal *result = gro_m_terminal_new(index, term_string);
	gro_m_symbol_set_stack_type((GroMSymbol *) result, stack_type);
	cat_hash_map_wo_put(priv->symbol_map, (GObject *) term_name, (GObject *) result);
	cat_array_wo_append(priv->terminal_list, (GObject *) result);
	cat_unref(result);
	if (created_new) {
		*created_new = TRUE;
	}
	return result;
}

int gro_model_terminal_count(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return cat_array_wo_size(priv->terminal_list);
}


GroMTerminalIterator *gro_model_terminal_iterator(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return (GroMTerminalIterator *) cat_array_wo_iterator(priv->terminal_list);
}


CatArrayWo *gro_model_enlist_terminals(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return priv->terminal_list;
}

void gro_model_dump_terminals(GroModel *model) {
	GroMTerminalIterator *term_iter = gro_model_terminal_iterator(model);
	while(gro_mterminal_iterator_has_next(term_iter)) {
		GroMSymbol *term = (GroMSymbol *) gro_mterminal_iterator_next(term_iter);
		cat_log_debug("term[%d]: name='%O'", gro_m_symbol_get_index(term), gro_m_symbol_get_name(term));
	}
	cat_unref_ptr(term_iter);
}


GroMTerminal *gro_model_get_terminal_eof(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return priv->terminal_eof;
}

GroMTerminal *gro_model_get_terminal_error(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return priv->terminal_error;
}

GroMTerminal *gro_model_get_terminal_by_index(GroModel *model, int index) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return (GroMTerminal *) cat_array_wo_get(priv->terminal_list, index);
}

GroMTerminal *gro_model_get_terminal_by_name(GroModel *model, CatStringWo *name) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	GroMSymbol *sym = (GroMSymbol *) cat_hash_map_wo_get(priv->symbol_map, name);
	if (GRO_IS_M_TERMINAL(sym)) {
		return (GroMTerminal *) sym;
	}
	return NULL;
}



int gro_model_non_terminal_count(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return cat_array_wo_size(priv->non_terminal_list);
}

CatArrayWo *gro_model_enlist_non_terminals(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return priv->non_terminal_list;
}

GroMNonTerminal *gro_model_add_non_terminal(GroModel *model, GroBnfString *nterm_string, CatStringWo *stack_type) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	CatStringWo *nterm_name = gro_bnf_string_get_text(nterm_string);
	GroMSymbol *esym = (GroMSymbol *) cat_hash_map_wo_get(priv->symbol_map, nterm_name);
	if (esym!=NULL) {
		if (GRO_IS_M_NON_TERMINAL(esym)) {
			return (GroMNonTerminal *) esym;
		}
		return NULL;
	}

	int index = cat_array_wo_size(priv->non_terminal_list);
	GroMNonTerminal *result = gro_m_non_terminal_new(index, nterm_string);
	gro_m_symbol_set_stack_type((GroMSymbol *) result, stack_type);
	cat_hash_map_wo_put(priv->symbol_map, (GObject *) nterm_name, (GObject *) result);
	cat_array_wo_append(priv->non_terminal_list, (GObject *) result);
	cat_unref(result);
	return result;
}

GroMNonTerminal *gro_model_get_non_terminal_dollar_start(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return (GroMNonTerminal *) cat_array_wo_get_first(priv->non_terminal_list);
}

GroMNonTerminal *gro_model_get_non_terminal_by_name(GroModel *model, CatStringWo *name) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	GroMSymbol *sym = (GroMSymbol *) cat_hash_map_wo_get(priv->symbol_map, name);
	if (GRO_IS_M_NON_TERMINAL(sym)) {
		return (GroMNonTerminal *) sym;
	}
	return NULL;
}

void gro_model_dump_non_terminals(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	CatIIterator *nterm_iter = cat_array_wo_iterator(priv->non_terminal_list);
	while(cat_iiterator_has_next(nterm_iter)) {
		GroMNonTerminal *non_term = cat_iiterator_next(nterm_iter);
		cat_log_debug("non-term[%d]: name='%O', %s, first-set=%o", gro_m_symbol_get_index((GroMSymbol *) non_term), gro_m_symbol_get_name((GroMSymbol *) non_term), gro_m_non_terminal_is_nullable(non_term) ? "nullable" : "not-nlbl", gro_m_non_terminal_get_first_set(non_term));
	}
	cat_unref_ptr(nterm_iter);
}


GroMProduction *gro_model_add_production(GroModel *model, GroMNonTerminal *lhs_non_term, CatArrayWo *production_parts, int prec_num, int prec_side) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	int idx = cat_array_wo_size(priv->productions);
	GroMProduction *result = gro_m_production_new(idx, lhs_non_term, production_parts, prec_num, prec_side);
	cat_array_wo_append(priv->productions, (GObject *) result);
	return result;
}

GroMNonTerminal *gro_model_create_sub_non_terminal(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);

	CatStringWo *sl_nterm_name = cat_string_wo_new();
	cat_string_wo_format(sl_nterm_name, "sub_list$%d", priv->sub_prod_seq++);
	GroBnfString *sl_nterm_string = gro_bnf_string_new(sl_nterm_name, NULL, NULL);
	return gro_model_add_non_terminal(model, sl_nterm_string, NULL);
}

// TODO provide locations as argument
GroMNonTerminal *gro_model_add_sub_productions(GroModel *model, CatArrayWo *sub_prod_parts_list) {
	GroMNonTerminal *lhs_non_term = gro_model_create_sub_non_terminal(model);

	CatIIterator *iter = cat_array_wo_iterator(sub_prod_parts_list);
	while(cat_iiterator_has_next(iter)) {
		CatArrayWo *sub_prod_parts = (CatArrayWo *) cat_iiterator_next(iter);
		gro_model_add_production(model, lhs_non_term, sub_prod_parts, -1,-1);
	}
	return lhs_non_term;
}


GroMProduction *gro_model_get_start_production(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return (GroMProduction *) cat_array_wo_get_first(priv->productions);
}

CatIIterator *gro_model_production_iterator(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return cat_array_wo_iterator(priv->productions);
}

int gro_model_production_count(GroModel *model) {
	GroModelPrivate *priv = gro_model_get_instance_private(model);
	return cat_array_wo_size(priv->productions);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroModel *instance = GRO_MODEL(self);
	GroModelPrivate *priv = gro_model_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, productions=%p]", iname, self, priv->productions);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
