
/*
   File:    grotablebuilder.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 23, 2016
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

#include "grotablebuilder.h"
#include "groactionreduce.h"
#include "groactionshift.h"
#include "groactionnonassoc.h"
#include "groactionfork.h"
#include "groassoc.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroTableBuilder"
#include <logging/catlog.h>

struct _GroTableBuilderPrivate {
	GroModel *model;
	GroTypeOfParser type_of_parser;
	gboolean create_glr;
	GroParseActionTable *action_table;
	GroParseReduceTable *reduce_table;
	int fork_index;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroTableBuilder, gro_table_builder, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroTableBuilder)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_table_builder_class_init(GroTableBuilderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_table_builder_init(GroTableBuilder *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroTableBuilder *instance = GRO_TABLE_BUILDER(object);
	GroTableBuilderPrivate *priv = gro_table_builder_get_instance_private(instance);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->action_table);
	cat_unref_ptr(priv->reduce_table);
	G_OBJECT_CLASS(gro_table_builder_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_table_builder_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroTableBuilder *gro_table_builder_new(GroModel *model, GroTypeOfParser top) {
	GroTableBuilder *result = g_object_new(GRO_TYPE_TABLE_BUILDER, NULL);
	cat_ref_anounce(result);
	GroTableBuilderPrivate *priv = gro_table_builder_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->type_of_parser = top;
	priv->action_table = NULL;
	priv->reduce_table = NULL;
	priv->create_glr = top!=GRO_PARSER_TYPE_LRAR;
	priv->fork_index = 0;
	return result;
}

void gro_table_builder_configure(GroTableBuilder *table_builder, int state_count) {
	GroTableBuilderPrivate *priv = gro_table_builder_get_instance_private(table_builder);
	cat_unref_ptr(priv->action_table);
	cat_unref_ptr(priv->reduce_table);
	priv->action_table = gro_parse_action_table_new(priv->model, state_count);
	priv->reduce_table = gro_parse_reduce_table_new(priv->model, state_count);
}

GroParseActionTable *gro_table_builder_get_action_table(GroTableBuilder *table_builder) {
	GroTableBuilderPrivate *priv = gro_table_builder_get_instance_private(table_builder);
	return priv->action_table;
}

GroParseReduceTable *gro_table_builder_get_reduce_table(GroTableBuilder *table_builder) {
	GroTableBuilderPrivate *priv = gro_table_builder_get_instance_private(table_builder);
	return priv->reduce_table;
}

static gboolean l_fix_with_precedence(GroModel *model, GroMProduction *p, int term_index, GroParseActionRow *table_row, GroIParseAction *act);


void gro_table_builder_create_for_state(GroTableBuilder *table_builder, GroLalrState *state) {
	GroTableBuilderPrivate *priv = gro_table_builder_get_instance_private(table_builder);

	GroTerminalSet *conflict_set = gro_terminal_set_new();

	/* pull out our rows from the tables */
	int state_index = gro_lalr_state_get_index(state);
	GroParseActionRow *our_action_row = gro_parse_action_table_get_row(priv->action_table, state_index);
	GroParseReduceRow *our_reduce_row = gro_parse_reduce_table_get_row(priv->reduce_table, state_index);


	cat_log_debug("###### state=%d", state_index);


	/* consider each item in our state */
	GroLalrItemSet *state_item_set = gro_lalr_state_get_item_set(state);
	GroLalrItemIterator *iter = gro_lalr_item_set_iterator(state_item_set);
	while(gro_lalr_item_iterator_has_next(iter)) {
		GroLalrItem *item = gro_lalr_item_iterator_next(iter);


		GroMProduction *item_prod = gro_lalr_item_get_production(item);
		cat_log_debug("item_prod=%o", item_prod);
		gboolean has_trans= FALSE;
		GroMSymbol *lhs = gro_m_production_get_lhs_symbol(item_prod);
		GroLalrTransition *trans = gro_lalr_state_get_transitions(state);


		if (gro_m_production_rhs_count(item_prod)>0) {
			has_trans = TRUE;
		} else {

			for(; trans != NULL; trans = gro_lalr_transition_next(trans)) {
				if (gro_lalr_transition_get_key_symbol(trans)==lhs) {
					if (gro_lalr_transition_get_to_state(trans)!=state) {
						has_trans = TRUE;
					}
				}
			}
		}


		cat_log_debug("at-end=%s, has_trans=%s, item=%o", gro_lalr_item_is_dot_at_end(item) ? "true" : "false", has_trans ? "true" : "false", item);


		/* if its completed (dot at end) then reduce under the lookahead */
		if (has_trans && gro_lalr_item_is_dot_at_end(item)) {
			GroIParseAction *act = (GroIParseAction *) gro_action_reduce_new(item_prod);

			int t;
			/* consider each lookahead symbol */
			GroMTerminalIterator *term_iter = gro_model_terminal_iterator(priv->model);
			while(gro_mterminal_iterator_has_next(term_iter)) {
				GroMTerminal *term = gro_mterminal_iterator_next(term_iter);
				t = gro_m_symbol_get_index((GroMSymbol *) term);


				/* skip over the ones not in the lookahead */
				GroTerminalSet *term_las = gro_lalr_item_get_lookahead_set(item);
				cat_log_debug("term=%o, term_las=%o", term, term_las);
				if (!gro_terminal_set_contains(term_las, term)) {
//					cat_log_debug("lookahead set contains term=%o", term);
					continue;
				}

				/* if we don't already have an action put this one in */
				GroIParseAction *other_act = gro_parse_action_row_get_action(our_action_row, t);
				GroActionKind other_kind = gro_iparse_action_kind(other_act);
				cat_log_debug("other_kind=%d", other_kind);
				if (other_kind==GRO_ACTION_KIND_ERROR) {
					/* We are fine. No other action was entered here before. */
					gro_parse_action_row_set_action(our_action_row, t, act);
				} else if (priv->create_glr) {
					cat_log_error("possible conflict but creating GLR parser");
					GroActionFork *fork_act = NULL;
					if (other_kind==GRO_ACTION_KIND_FORK) {
						fork_act = GRO_ACTION_FORK(other_act);
					} else if (!gro_action_fork_actions_equal(act, other_act)) {
						/* replace existing action with a fork */

						fork_act = gro_action_fork_new(priv->fork_index++, other_act);
						gro_parse_action_row_set_action(our_action_row, t, (GroIParseAction *) fork_act);
						cat_unref(fork_act);
						cat_log_error("new fork action created:%d", (priv->fork_index-1));
					}
					if (fork_act) {
						gro_action_fork_add_action(fork_act, act);
					}
					CatIIterator *ii = gro_action_fork_action_iterator(fork_act);
					while(cat_iiterator_has_next(ii)) {
						GObject *oo = cat_iiterator_next(ii);
						cat_log_error("oooooo=%O", oo);
					}
					cat_unref_ptr(ii);
				} else {
	//				cat_log_debug("other_kind=%d", other_kind);
					switch(other_kind) {
						case GRO_ACTION_KIND_REDUCE : {
							/* shift/reduce error */
							/* if we have lower index hence priority, replace it */
							GroActionReduce *other_reduce = GRO_ACTION_REDUCE(other_act);
							GroMProduction *other_prod = gro_action_reduce_get_production(other_reduce);
							GroMProduction *item_prod = gro_lalr_item_get_production(item);
							if (gro_m_production_get_index(item_prod) < gro_m_production_get_index(other_prod)) {
								/* replace the action */
								gro_parse_action_row_set_action(our_action_row, t, act);
							}
//							conflict_set.add(term);
						} break;
						case GRO_ACTION_KIND_SHIFT :
						case GRO_ACTION_KIND_NONASSOC : {
							/* Check precedences,see if problem is correctable */
							GroMProduction *item_prod = gro_lalr_item_get_production(item);
							if (l_fix_with_precedence(priv->model, item_prod, t, our_action_row, act)) {
								term = NULL;
							}

	//						if (term != null) {
	//							conflict_set.add(term);
	//						}
						} break;
						default :
						case GRO_ACTION_KIND_FORK : {
							// TODO break program ?
							cat_log_error("ERROR:Should not happen");
						} break;
					}
				}
			}
		}
	}

	/* consider each outgoing transition */
	GroLalrTransition *trans = gro_lalr_state_get_transitions(state);
	for(; trans != NULL; trans = gro_lalr_transition_next(trans)) {
		/* if its on an terminal add a shift entry */
		GroMSymbol *sym = gro_lalr_transition_get_key_symbol(trans);
		int sym_index= gro_m_symbol_get_index(sym);
		if (!GRO_IS_M_NON_TERMINAL(sym)) {
			GroIParseAction *act = (GroIParseAction *) gro_action_shift_new(gro_lalr_transition_get_to_state(trans));

			/* if we don't already have an action put this one in */
			GroIParseAction *other_act = gro_parse_action_row_get_action(our_action_row, sym_index);
			GroActionKind other_kind = gro_iparse_action_kind(other_act);
			cat_log_debug("sym:%O, other_kind=%d", sym, other_kind);
			if (other_kind == GRO_ACTION_KIND_ERROR) {
				gro_parse_action_row_set_action(our_action_row, sym_index, act);
			} else if (priv->create_glr) {
				cat_log_error("possible conflict but creating GLR parser");
				GroActionFork *fork_act = NULL;
				if (other_kind==GRO_ACTION_KIND_FORK) {
					fork_act = GRO_ACTION_FORK(other_act);
				} else if (!gro_action_fork_actions_equal(act, other_act)) {
					/* replace existing action with a fork */

					fork_act = gro_action_fork_new(priv->fork_index++, other_act);
					gro_parse_action_row_set_action(our_action_row, sym_index, fork_act);
					cat_unref(fork_act);
					cat_log_error("new fork action created:%d", (priv->fork_index-1));
				}
				if (fork_act) {
					gro_action_fork_add_action(fork_act, act);
				}
				CatIIterator *ii = gro_action_fork_action_iterator(fork_act);
				while(cat_iiterator_has_next(ii)) {
					GObject *oo = cat_iiterator_next(ii);
					cat_log_error("oooooo=%O", oo);
				}
				cat_unref_ptr(ii);
			} else {
				/* we now have at least one conflict */
				GroActionReduce *other_reduce = GRO_ACTION_REDUCE(other_act);
				GroMProduction *other_prod = gro_action_reduce_get_production(other_reduce);

				/* shift always wins */
				if (!l_fix_with_precedence(priv->model, other_prod, sym_index, our_action_row, act)) {
					gro_parse_action_row_set_action(our_action_row, sym_index, act);
//					conflict_set.add(model.terminals.get(sym.index()));
				}
				cat_log_warn("conflict:%O", sym);
			}
		} else {
			/* for non terminals add an entry to the reduce-goto table */
			GroLalrState *to_state = gro_lalr_transition_get_to_state(trans);
			gro_parse_reduce_row_set_to_state(our_reduce_row, sym_index, to_state);
		}
	}

	/* if we end up with conflict(s), report them */
//	if (!conflict_set.empty()) {
//		report_conflicts(model, conflict_set);
//	}

}



/*
 * given two actions, and an action type, return the action of that action
 * type. give an error if they are of the same action, because that should
 * never have tried to be fixed
 */
static GroIParseAction *l_insert_action(GroIParseAction *a1, GroIParseAction *a2, int act_type) {
	GroActionKind kind1 = gro_iparse_action_kind(a1);
	GroActionKind kind2 = gro_iparse_action_kind(a2);
	if ((kind1 == act_type) && (kind2 == act_type)) {
		// TODO
		// throw new FatalCupException("Conflict resolution of bogus actions");
		return NULL;
	} else if (kind1 == act_type) {
		return a1;
	} else if (kind2 == act_type) {
		return a2;
	} else {
		// TODO
//		throw new FatalCupException("Conflict resolution of bogus actions");
	}
	return NULL;
}

/* find the shift in the two actions */
static GroIParseAction *l_insert_shift(GroIParseAction *a1, GroIParseAction *a2) {
	return l_insert_action(a1, a2, GRO_ACTION_KIND_SHIFT);
}

/* find the reduce in the two actions */
static GroIParseAction *l_insert_reduce(GroIParseAction *a1, GroIParseAction *a2) {
	return l_insert_action(a1, a2, GRO_ACTION_KIND_REDUCE);
}


/**
 * Procedure that attempts to fix a shift/reduce error by using precedences.
 * --frankf 6/26/96
 *
 * if a production (also called rule) or the lookahead terminal has a
 * precedence, then the table can be fixed. if the rule has greater
 * precedence than the terminal, a reduce by that rule in inserted in the
 * table. If the terminal has a higher precedence, it is shifted. if they
 * have equal precedence, then the associativity of the precedence is used
 * to determine what to put in the table: if the precedence is left
 * associative, the action is to reduce. if the precedence is right
 * associative, the action is to shift. if the precedence is non
 * associative, then it is a syntax error.
 *
 * @param p               the production
 * @param term_index      the index of the lokahead terminal
 * @param ParseActionRow  a row of the action table
 * @param act             the rule in conflict with the table entry
 */
static gboolean l_fix_with_precedence(GroModel *model, GroMProduction *p, int term_index, GroParseActionRow *table_row, GroIParseAction *act) {

	GroMTerminal *term = gro_model_get_terminal_by_index(model, term_index);

	int prod_prec_num = gro_m_production_get_precedence_num(p);
	int term_prec_num = gro_m_terminal_get_precedence_num(term);
	GroIParseAction *cur_act = gro_parse_action_row_get_action(table_row, term_index);

	/* if the production has a precedence number, it can be fixed */
	if (prod_prec_num > GRO_ASSOC_NO_PREC) {

		/* if production precedes terminal, put reduce in table */
		if (prod_prec_num > term_prec_num) {
			GroIParseAction *act = l_insert_reduce(cur_act, act);
			gro_parse_action_row_set_action(table_row, term_index, act);
			cat_unref_ptr(act);
			return TRUE;
		} else if (prod_prec_num < term_prec_num) {
			/* if terminal precedes rule, put shift in table */
			GroIParseAction *act = l_insert_shift(cur_act, act);
			gro_parse_action_row_set_action(table_row, term_index, act);
			cat_unref_ptr(act);
			return TRUE;
		} else { /* they are == precedence */
			/*
			 * equal precedences have equal sides, so only need to look at
			 * one: if it is right, put shift in table
			 */

			int prec_side = gro_m_terminal_get_precedence_side(term);


			if (prec_side == GRO_ASSOC_RIGHT) {
				GroIParseAction *act = l_insert_shift(cur_act, act);
				gro_parse_action_row_set_action(table_row, term_index, act);
				cat_unref_ptr(act);
				return TRUE;
			} else if (prec_side == GRO_ASSOC_LEFT) {
				/* if it is left, put reduce in table */
				GroIParseAction *act = l_insert_reduce(cur_act, act);
				gro_parse_action_row_set_action(table_row, term_index, act);
				cat_unref_ptr(act);
				return TRUE;
			} else if (prec_side == GRO_ASSOC_NONASSOC) {
				/*
				 * if it is nonassoc, we're not allowed to have two nonassocs of
				 * equal precedence in a row, so put in NONASSOC
				 */
				GroIParseAction *act = (GroIParseAction *) gro_action_non_assoc_new();
				gro_parse_action_row_set_action(table_row, term_index, act);
				cat_unref_ptr(act);
				return TRUE;
			} else {
				/* something really went wrong */
				cat_log_fatal("not implemented yet");
//				throw new FatalCupException("Unable to resolve conflict correctly");
			}
		}
	} else if (term_prec_num > GRO_ASSOC_NO_PREC) {
		/*
		 * check if terminal has precedence, if so, shift, since rule does not
		 * have precedence
		 */
		GroIParseAction *act = l_insert_shift(cur_act, act);
		gro_parse_action_row_set_action(table_row, term_index, act);
		cat_unref_ptr(act);
		return TRUE;
	}

	/*
	 * otherwise, neither the rule nor the terminal has a precedence, so it
	 * can't be fixed.
	 */
	return FALSE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroTableBuilder *instance = GRO_TABLE_BUILDER(self);
//	GroTableBuilderPrivate *priv = gro_table_builder_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
