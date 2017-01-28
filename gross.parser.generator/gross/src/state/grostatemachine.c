
/*
   File:    grostatemachine.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 23, 2016
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

#include "grostatemachine.h"
#include "grolalritemset.h"
#include "grolalritem.h"
#include "grolalrstatemap.h"
#include "grolalrstate.h"
#include "../model/grosymbolset.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroStateMachine"
#include <logging/catlog.h>

struct _GroStateMachinePrivate {
	GroModel *model;
	GroLalrStateMap *state_map;

	/**
	 * Hash table to find states by their kernels (i.e, the original, unclosed,
	 * set of items -- which uniquely define the state). This table stores state
	 * objects using (a copy of) their kernel item sets as keys.
	 */
	CatHashMapWo *all_kernels; /* <GroLalrItemSet, GroLalrState> */

	GroLalrState *start_state;

	GroTableBuilder *table_builder;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroStateMachine, gro_state_machine, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroStateMachine)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_state_machine_class_init(GroStateMachineClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_state_machine_init(GroStateMachine *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroStateMachine *instance = GRO_STATE_MACHINE(object);
//	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(instance);
	G_OBJECT_CLASS(gro_state_machine_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_state_machine_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroStateMachine *gro_state_machine_new(GroModel *model, GroTypeOfParser top) {
	GroStateMachine *result = g_object_new(GRO_TYPE_STATE_MACHINE, NULL);
	cat_ref_anounce(result);
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->state_map = gro_lalr_state_map_new();
	priv->all_kernels = cat_hash_map_wo_new((GHashFunc) gro_lalr_item_set_hash, (GEqualFunc) gro_lalr_item_set_equal);
	priv->start_state = NULL;
	priv->table_builder = gro_table_builder_new(model, top);
	return result;
}

int gro_state_machine_state_count(GroStateMachine *state_machine) {
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(state_machine);
	return gro_lalr_state_map_count(priv->state_map);
}

GroParseActionTable *gro_state_machine_get_action_table(GroStateMachine *state_machine) {
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(state_machine);
	return gro_table_builder_get_action_table(priv->table_builder);
}

GroParseReduceTable *gro_state_machine_get_reduce_table(GroStateMachine *state_machine) {
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(state_machine);
	return gro_table_builder_get_reduce_table(priv->table_builder);
}

GroLalrState *gro_state_machine_get_start_state(GroStateMachine *state_machine) {
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(state_machine);
	return priv->start_state;
}

void gro_state_machine_build(GroStateMachine *state_machine) {
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(state_machine);
	cat_unref_ptr(priv->start_state);
	priv->start_state = gro_state_machine_create_states(state_machine);

	int state_count = gro_state_machine_state_count(state_machine);
	gro_table_builder_configure(priv->table_builder, state_count);

	GroLalrStateIterator *iter = gro_lalr_state_map_iterator(priv->state_map);
	while(gro_lalr_state_iterator_has_next(iter)) {
		GroLalrState *state = gro_lalr_state_iterator_next(iter);
		gro_table_builder_create_for_state(priv->table_builder, state);
//		warningCount += lst.warningCount;
	}
	cat_unref_ptr(iter);
//
////		table_end = System.currentTimeMillis();
//
//	/* check and warn for non-reduced productions */
////		if (opt_do_debug || print_progress)
////			System.err.println("  Checking for non-reduced productions...");
//	action_table.checkReductions();
//	warningCount += action_table.warningCount;
//
////		reduce_check_end = System.currentTimeMillis();
//
//	/* if we have more conflicts than we expected issue a message and die */
//	if (model.num_conflicts > cupConfiguration.getExpectConflicts()) {
//		System.err.println("*** More conflicts encountered "+model.num_conflicts +" than expected " + cupConfiguration.getExpectConflicts() +  "-- parser generation aborted");
//		errorCount++; // indicate the problem.
//		// we'll die on return, after clean up.
//	}
//
}


void gro_state_machine_dump(GroStateMachine *state_machine) {
//	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(state_machine);
//
//	cat_log_print("DUMP", "####################### state-machine #######################");
//
//
//	CatArrayWo *states = cat_array_wo_new();
//	GroLalrStateIterator *state_iter = gro_lalr_state_map_iterator(priv->state_map);
//	while(gro_lalr_state_iterator_has_next(state_iter)) {
//		GroLalrState *state = gro_lalr_state_iterator_next(state_iter);
//
//		int index = gro_lalr_state_get_index(state);
//		cat_array_wo_pad(states, index+1, NULL);
//		cat_array_wo_set(states, state, index, NULL);
//	}
//
//	CatIIterator *iter = cat_array_wo_iterator(states);
//	while(cat_iiterator_has_next(iter)) {
//		GroLalrState *state = (GroLalrState *) cat_iiterator_next(iter);
//		int idx = gro_lalr_state_get_index(state);
//		cat_log_print("DUMP", "state: %d", idx);
//		GroLalrItemSet *item_set = gro_lalr_state_get_item_set(state);
//		GroLalrItemIterator *item_iter = gro_lalr_item_set_iterator(item_set);
//		while(gro_lalr_item_iterator_has_next(item_iter)) {
//			GroLalrItem *item = gro_lalr_item_iterator_next(item_iter);
//			cat_log_print("DUMP", "  # %O", item);
//		}
//		cat_unref_ptr(item_iter);
//
//		GroLalrTransition *transition = gro_lalr_state_get_transitions(state);
//		while(transition) {
//			GroMSymbol *sym = gro_lalr_transition_get_key_symbol(transition);
//			GroLalrState *to_state = gro_lalr_transition_get_to_state(transition);
//			cat_log_print("DUMP", "  # %O to-state=%d", sym, gro_lalr_state_get_index(to_state));
//			transition = gro_lalr_transition_next(transition);
//		}
//		cat_log_print("DUMP", "");
//
//	}
//	cat_unref_ptr(iter);
}


static GroLalrState *l_create_lalr_state(GroStateMachinePrivate *priv, GroLalrItemSet *item_set) {
	int next_index = gro_lalr_state_map_count(priv->state_map);
	GroLalrState *result = gro_lalr_state_new(item_set, next_index);
	gro_lalr_state_map_add(priv->state_map, result);
	cat_unref(result);
	return result;
}



static void l_propagate_all_lookaheads(GroStateMachinePrivate *priv) {
	GroLalrStateIterator *iter = gro_lalr_state_map_iterator(priv->state_map);
	/* iterate across all states */
	while(gro_lalr_state_iterator_has_next(iter)) {
		GroLalrState *state = gro_lalr_state_iterator_next(iter);
		gro_lalr_state_propagate_lookaheads(state);
	}
	cat_unref_ptr(iter);
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


GroLalrState *gro_state_machine_create_states(GroStateMachine *state_machine) {
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(state_machine);
	GroLalrItemSet *start_items = gro_lalr_item_set_new();

	GroMProduction *production = gro_model_get_start_production(priv->model);

	GroLalrItem *item = gro_lalr_item_new(production);
	GroTerminalSet *lah_set = gro_lalr_item_get_lookahead_set(item);
	gro_terminal_set_add(lah_set, gro_model_get_terminal_eof(priv->model));

	gro_lalr_item_set_add(start_items, item);
	cat_unref_ptr(item);

	/* create copy the item set to form the kernel */
	GroLalrItemSet *kernel = gro_lalr_item_set_clone(start_items);
//	cat_log_debug("start_items=%o", start_items);

	/* create the closure from that item set */
	gro_lalr_item_set_compute_closure(start_items);

	CatArrayWo *work_stack = cat_array_wo_new();

	/* build a state out of that item set and put it in our work set */
	GroLalrState *start_state = l_create_lalr_state(priv, start_items);
	cat_array_wo_append(work_stack, (GObject *) start_state);

	/* enter the state using the kernel as the key */
	cat_hash_map_wo_put(priv->all_kernels, (GObject *) kernel, (GObject *) start_state);
//	cat_log_debug("kernel=%o, start_state=%o", kernel, start_state);
	cat_unref(kernel);

	/* continue looking at new states until we have no more work to do */
	while(cat_array_wo_size(work_stack)>0) {
		/* remove a state from the work set */
		GroLalrState *work_state;
		cat_array_wo_remove_last(work_stack, (GObject **) &work_state);
		cat_log_debug("##################################", work_state);
		cat_log_debug("work_state=%o", work_state);


		/* gather up all the symbols that appear before dots */
		GroSymbolSet *outgoing = gro_symbol_set_new();
		GroLalrItemSet *ws_item_set = gro_lalr_state_get_item_set(work_state);


		GroLalrItemIterator *ws_iter = gro_lalr_item_set_iterator(ws_item_set);
		while(gro_lalr_item_iterator_has_next(ws_iter)) {
			GroLalrItem *preceding_item = gro_lalr_item_iterator_next(ws_iter);

			cat_log_debug("preceding_item=%o", preceding_item);

			/* add the symbol before the dot (if any) to our collection */
			GroMSymbol *sym = gro_lalr_item_get_symbol_after_dot(preceding_item);
//			cat_log_debug("sym=%o", sym);
			if (sym != NULL) {
				gro_symbol_set_add(outgoing, sym);
			}
		}
		cat_unref_ptr(ws_iter);


		/* now create a transition out for each individual symbol */
		cat_log_debug("outgoing=%o", outgoing);
		CatIIterator *out_iter = gro_symbol_set_value_iterator(outgoing);
		while(cat_iiterator_has_next(out_iter)) {
			GroMSymbol *sym = (GroMSymbol *) cat_iiterator_next(out_iter);
			cat_log_debug("outgoing-sym=%o", sym);

			/* will be keeping the set of items with propagate links */
			GroLalrItemSet *linked_items = gro_lalr_item_set_new();

			/*
			 * gather up shifted versions of all the items that have this
			 * symbol before the dot
			 */
			GroLalrItemSet *new_items = gro_lalr_item_set_new();

			GroLalrItemIterator *ws_iter = gro_lalr_item_set_iterator(ws_item_set);
			while(gro_lalr_item_iterator_has_next(ws_iter)) {
				GroLalrItem *shitem = gro_lalr_item_iterator_next(ws_iter);

				/* if this is the symbol we are working on now, add to set */
				GroMSymbol *sym2 = gro_lalr_item_get_symbol_after_dot(shitem);
				if (gro_m_symbol_equal(sym, sym2)) {
					/* add to the kernel of the new state */

					GroLalrItem *shifted_to_item = gro_lalr_item_shift(shitem);
					gro_lalr_item_set_add(new_items, shifted_to_item);

					/* remember that shitem has propagate link to it */
					gro_lalr_item_set_add(linked_items, shitem);
					cat_log_debug("shitem=%O, shifted_to_item=%O", shitem, shifted_to_item);
					cat_unref_ptr(shifted_to_item);
				}
			}
			cat_unref_ptr(ws_iter);

			/* use new items as state kernel */
			kernel = gro_lalr_item_set_clone(new_items);

			/* have we seen this one already? */
			GroLalrState *new_st = (GroLalrState *) cat_hash_map_wo_get(priv->all_kernels, kernel);
//			cat_log_debug("kernel=%o, new_st=%o", kernel, new_st);

			/* if we haven't, build a new state out of the item set */
			if (new_st == NULL) {
//				cat_log_debug("new state kernel=%o", kernel);
				/* compute closure of the kernel for the full item set */
				gro_lalr_item_set_compute_closure(new_items);

				/* build the new state */
				new_st = l_create_lalr_state(priv, new_items);

				/* add the new state to our work set */
				cat_array_wo_append(work_stack, (GObject *) new_st);

				/* put it in our kernel table */
				cat_hash_map_wo_put(priv->all_kernels, (GObject *) kernel, (GObject *) new_st);
			} else {
//				if (new_st == work_state) {
//					continue;
//				}
				/* otherwise relink propagation to items in existing state */



				/* walk through the items that have links to the new state */
				GroLalrItemIterator *iter = gro_lalr_item_set_iterator(linked_items);
				while(gro_lalr_item_iterator_has_next(iter)) {
					GroLalrItem *fix_item = gro_lalr_item_iterator_next(iter);

					/* look at each propagate link out of that item */
					CatArrayWo *prop_items = gro_lalr_item_get_propagate_items(fix_item);

					// DEBUGONLY
					cat_array_wo_sort(prop_items, l_gro_lalr_item_comp);


					GroLalrItemSet *new_st_item_set = gro_lalr_state_get_item_set(new_st);
					int prop_count = cat_array_wo_size(prop_items);
					int l;
					for(l = 0; l < prop_count; l++) {
						/* pull out item linked to in the new state */
						GroLalrItem *new_item = (GroLalrItem *) cat_array_wo_get(prop_items, l);

						/* find corresponding item in the existing state */
						GroLalrItem *existing = gro_lalr_item_set_find(new_st_item_set, new_item);

						/* fix up the item so it points to the existing set */
						if (existing != NULL) {
							cat_array_wo_set(prop_items, (GObject *) existing, l, NULL);
						}
					}
				}
				cat_unref_ptr(iter);
			}

			cat_log_debug("add-trans sym=%O -> new_st=%O", gro_m_symbol_get_name(sym), new_st);

			/* add a transition from current state to that state */
			gro_lalr_state_add_transition(work_state, sym, new_st);
		}
		cat_unref_ptr(out_iter);

	}

	/* all done building states */

	/* propagate complete lookahead sets throughout the states */
	l_propagate_all_lookaheads(priv);

	return start_state;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroStateMachine *instance = GRO_STATE_MACHINE(self);
	GroStateMachinePrivate *priv = gro_state_machine_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, model=%p]", iname, self, priv->model);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
