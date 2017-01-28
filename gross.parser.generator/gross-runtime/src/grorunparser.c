/*
   File:    grorunparser.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
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

#include "grorunparser.h"
#include "grorunshifttransition.h"
#include "grorunreducetransition.h"
#include "grorunaccepttransition.h"
#include "full/grorunfulltoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunParser"
#include <logging/catlog.h>

struct _GroRunParserPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunParser, grorun_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunParser)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_parser_class_init(GroRunParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_parser_init(GroRunParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroRunParser *instance = GRORUN_PARSER(object);
//	GroRunParserPrivate *priv = grorun_parser_get_instance_private(instance);
	G_OBJECT_CLASS(grorun_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunParser *grorun_parser_new() {
	GroRunParser *result = g_object_new(GRORUN_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
//	GroRunParserPrivate *priv = grorun_parser_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}

static void l_try_error_recovery(GroRunContext *context, GroRunIScanner *scanner);


GObject *grorun_parser_do_parse(GroRunContext *context, GroRunIScanner *scanner) {

	GroRunIToken *next_lah = grorun_iscanner_next(scanner);
	grorun_context_create_start_fork(context, next_lah);
	cat_unref_ptr(next_lah);

	while(grorun_context_fork_count(context)>0) {

		grorun_context_error_forks_clear(context);

		cat_unref_ptr(next_lah);
		next_lah = grorun_iscanner_next(scanner);

		cat_log_debug("###################### nextLah=%O, fork-count=%d ######################", next_lah, grorun_context_fork_count(context));

		int fork_idx;
		for(fork_idx=0; fork_idx<grorun_context_fork_count(context); fork_idx++) {
			GroRunFork *fork = grorun_context_fork_at(context, fork_idx);

			while(TRUE) {
				if (grorun_fork_has_pushback(fork)) {
					grorun_fork_dump(fork);
				}
				cat_log_on_debug({
					grorun_fork_dump(fork);
				});

				GroRunStackEntry *top_entry = grorun_fork_get_top_entry(fork);
				GroRunTransition *transition = grorun_fork_get_pending_transition(fork);
				if (transition==NULL) {
					GroRunTransitionsForSymbol *transitions_for_symbol = grorun_stack_entry_get_transitions_for_symbol(top_entry);
					if (transitions_for_symbol==NULL) {
						CatStringWo *s = cat_string_wo_new_with("syntax error");
						GroRunLocation *location = NULL;
						GroRunIToken *lah = grorun_stack_entry_get_lookahead(top_entry);
						if (GRORUN_IS_FULL_TOKEN(lah)) {
							GroRunFullToken *ft = (GroRunFullToken *) lah;
							location = grorun_full_token_get_location(ft);
						}
						grorun_fork_append_message(fork, s, location);
						cat_unref_ptr(s);
						grorun_context_mark_fork_as_error(context, fork_idx);
//						context.markForkAsError(fork_idx, "no transitions for symbol:"+top_entry.lookahead+" for state:"+top_entry.state);
						fork_idx--;
						break;
					}
					int tfs_count = grorun_transitions_for_symbol_count(transitions_for_symbol);
					if (tfs_count>1) {
						int tfs_idx;
						for(tfs_idx=1; tfs_idx<tfs_count; tfs_idx++) {
							GroRunTransition *pending_transition = grorun_transitions_for_symbol_get(transitions_for_symbol, tfs_idx);
							grorun_context_fork(context, fork_idx, pending_transition);
							cat_log_debug("pendingTransition=%O", pending_transition);
						}
					}
					transition = grorun_transitions_for_symbol_get(transitions_for_symbol, 0);
					cat_ref_ptr(transition);
				}

				cat_log_debug("transition=%O", transition);
				if (GRORUN_IS_REDUCE_TRANSITION(transition)) {
					grorun_fork_set_postponed(fork, grorun_stack_entry_get_lookahead(top_entry));
					GroRunReduceTransition *red_trans = (GroRunReduceTransition *) transition;
					int rhs_count = grorun_reduce_transition_get_rhs_count(red_trans);
					int action_id = grorun_reduce_transition_get_action_id(red_trans);
					GroRunReduceLayout *reduce_layout = grorun_reduce_transition_get_reduce_layout(red_trans);
//					cat_log_debug("removing %d from stack and setting %O as replacement", rhs_count, lhs);

					GroRunIToken *reducedToken = grorun_context_reduce(context, fork, reduce_layout, action_id);

					int idx;
					for(idx=0; idx<rhs_count; idx++) {
						top_entry = grorun_stack_entry_get_parent(top_entry);
					}

					GroRunStackEntry *new_top = grorun_stack_entry_new(grorun_stack_entry_get_parent(top_entry), grorun_stack_entry_get_state(top_entry), reducedToken);
					grorun_fork_set_top_entry(fork, new_top);
					cat_unref_ptr(new_top);
					cat_unref_ptr(reducedToken);

				} else if (GRORUN_IS_SHIFT_TRANSITION(transition)) {
					GroRunShiftTransition *shift_trans = (GroRunShiftTransition *) transition;

					gboolean consumed_next_lookahead = FALSE;
					GroRunIToken *te_sym = NULL;
					CatStringWo *rb = grorun_fork_get_push_back_token(fork);
					if (rb!=NULL) {
						GroRunLeftRight lr;
						te_sym = grorun_context_create_for_pushback(context, lr, NULL, rb);
						cat_log_error("got push_back=%O", te_sym);
					} else {
						te_sym = grorun_fork_get_postponed(fork);
						if (te_sym!=NULL) {
							cat_ref_ptr(te_sym);
							grorun_fork_set_postponed(fork, NULL);
						} else {
							te_sym = cat_ref_ptr(next_lah);
							consumed_next_lookahead = TRUE;
						}
					}


					GroRunState *forward_state = grorun_shift_transition_get_forward_state(shift_trans);
					GroRunStackEntry *new_top = grorun_stack_entry_new(top_entry, forward_state, te_sym);
					grorun_fork_set_top_entry(fork, new_top);
					cat_unref(new_top);
					cat_unref(te_sym);

					cat_log_debug("shifting to state %O new lookahead %O", forward_state, te_sym);
					if (consumed_next_lookahead) {
						cat_unref_ptr(transition);
						break;
					}
				} else if (GRORUN_IS_ACCEPT_TRANSITION(transition)) {
					grorun_context_mark_fork_as_accepted(context, fork_idx);
					fork_idx--;
					cat_unref_ptr(transition);
					break;
				} else {
					cat_log_error("no transition....");
					cat_unref_ptr(transition);
					break;
				}
				cat_unref_ptr(transition);
			}
		}



		if (grorun_context_fork_count(context)==0 && !grorun_context_fork_has_accepted(context)) {
			if (grorun_context_error_fork_count(context)>0) {
				l_try_error_recovery(context, scanner);
			}
		}

	}
	cat_unref_ptr(next_lah);

	return grorun_context_get_first_accepted(context);
}


static void l_try_error_recovery(GroRunContext *context, GroRunIScanner *scanner) {


	CatArrayWo *recoverable = cat_array_wo_new();
	GroRunSymbol *error_symbol = grorun_context_get_error_symbol(context);

	int fork_idx;
	for(fork_idx=grorun_context_error_fork_count(context)-1; fork_idx>=0; fork_idx--) {
		GroRunFork *fork = grorun_context_error_fork_at(context, fork_idx);
		GroRunStackEntry *top_entry = grorun_fork_get_top_entry(fork);
		while(top_entry!=NULL) {
			GroRunTransitionsForSymbol *trans_for_sym = grorun_stack_entry_get_transitions_for_symbols(top_entry, error_symbol);
			if (trans_for_sym!=NULL) {
				int tfs_count = grorun_transitions_for_symbol_count(trans_for_sym);
				if (tfs_count>0) {
					GroRunTransition *transition = grorun_transitions_for_symbol_get(trans_for_sym, 0);
					if (GRORUN_IS_SHIFT_TRANSITION(transition)) {
						GroRunIToken *lookahead = grorun_context_create_error_toklen(context);
						GroRunStackEntry *te_parent = grorun_stack_entry_get_parent(top_entry);
						GroRunState *te_state = grorun_stack_entry_get_state(top_entry);

						GroRunStackEntry *stackEntry = grorun_stack_entry_new(te_parent, te_state, lookahead);
						grorun_fork_set_top_entry(fork, stackEntry);
						cat_unref_ptr(stackEntry);
						cat_unref_ptr(lookahead);
						break;
					}
				}
			}
			top_entry = grorun_stack_entry_get_parent(top_entry);
		}

		if (top_entry) {
			cat_array_wo_append(recoverable, (GObject *) fork);
		}
	}

	int recSymCount = 5;
	if (cat_array_wo_size(recoverable)>0) {
		CatArrayWo *spawned = cat_array_wo_new();

		gboolean keep_recovering = TRUE;
		GroRunIToken *next_lah = NULL;
		while(keep_recovering) {
			cat_unref_ptr(next_lah);
			next_lah = grorun_iscanner_next(scanner);
			cat_log_debug("###################### error-recovery: nextLah=%O ######################", next_lah);


			/* each round we spawn a new batch of forks with the error-forks as base and the recovery-count initialized at the beginning */
			if (grorun_itoken_get_symbol(next_lah)!=grorun_context_get_eof_symbol(context)) {
				int idx;
				for(idx=cat_array_wo_size(recoverable)-1; idx>=0; idx--) {
					GroRunFork *error_fork = (GroRunFork *) cat_array_wo_get(recoverable, idx);
					GroRunFork *duplicate = grorun_fork_duplicate(error_fork, recSymCount);
					cat_array_wo_append(spawned, (GObject *) duplicate);
					cat_unref_ptr(duplicate);
				}
			}

			if (cat_array_wo_size(spawned)==0) {
				break;
			}

			int fork_idx;
			for(fork_idx=0; fork_idx<cat_array_wo_size(spawned); fork_idx++) {
				GroRunFork *fork = (GroRunFork *) cat_array_wo_get(spawned, fork_idx);

				while(TRUE) {
					cat_log_on_debug({
						grorun_fork_dump(fork);
					});

					GroRunStackEntry *top_entry = grorun_fork_get_top_entry(fork);
					GroRunTransition *transition = grorun_fork_get_pending_transition(fork);
					if (transition==NULL) {
						GroRunTransitionsForSymbol *transitions_for_symbol = grorun_stack_entry_get_transitions_for_symbol(top_entry);
						if (transitions_for_symbol==NULL) {
							cat_array_wo_remove(spawned, fork_idx, NULL);
							fork_idx--;
							fork = NULL;
							break;
						}

						int tfs_count = grorun_transitions_for_symbol_count(transitions_for_symbol);
						if (tfs_count>1) {
							int tfs_idx;
							int rec_cnt_fork = grorun_fork_get_recovery_count(fork);
							for(tfs_idx=1; tfs_idx<tfs_count; tfs_idx++) {
								GroRunTransition *pending_transition = grorun_transitions_for_symbol_get(transitions_for_symbol, tfs_idx);

								GroRunFork *new_spawned = grorun_fork_duplicate(fork, rec_cnt_fork);
								grorun_fork_set_pending_transition(new_spawned, pending_transition);
								cat_array_wo_append(spawned, (GObject *) new_spawned);
								cat_log_debug("pendingTransition=%O", pending_transition);
								cat_unref_ptr(new_spawned);
							}
						}
						transition = grorun_transitions_for_symbol_get(transitions_for_symbol, 0);
						cat_ref_ptr(transition);
					}

					cat_log_debug("transition=%O", transition);
					if (GRORUN_IS_REDUCE_TRANSITION(transition)) {
						grorun_fork_set_postponed(fork, grorun_stack_entry_get_lookahead(top_entry));
						GroRunReduceTransition *redTrans = (GroRunReduceTransition *) transition;
						int rhsCount = grorun_reduce_transition_get_rhs_count(redTrans);
						GroRunReduceLayout *reduce_layout = grorun_reduce_transition_get_reduce_layout(redTrans);
						cat_log_debug("removing %d from stack",rhsCount);


						GroRunIToken *reducedToken = grorun_context_reduce(context, fork, reduce_layout, grorun_reduce_transition_get_action_id(redTrans));

						int idx;
						for(idx=0; idx<rhsCount; idx++) {
							top_entry = grorun_stack_entry_get_parent(top_entry);
						}

						GroRunStackEntry *te_parent = grorun_stack_entry_get_parent(top_entry);
						GroRunState *te_state = grorun_stack_entry_get_state(top_entry);
						GroRunStackEntry *new_top = grorun_stack_entry_new(te_parent, te_state, reducedToken);
						grorun_fork_set_top_entry(fork, new_top);
						cat_unref_ptr(new_top);
						cat_unref_ptr(reducedToken);

					} else if (GRORUN_IS_SHIFT_TRANSITION(transition)) {
						GroRunShiftTransition *shiftTrans = (GroRunShiftTransition *) transition;

						gboolean consumed_next_lookahead = FALSE;
						GroRunIToken *te_sym = NULL;
						CatStringWo *rb = grorun_fork_get_push_back_token(fork);
						if (rb!=NULL) {
							GroRunLeftRight lr;
							te_sym = grorun_context_create_for_pushback(context, lr, NULL, rb);
							cat_log_error("got push_back=%O", te_sym);
						} else {
							te_sym = grorun_fork_get_postponed(fork);
							if (te_sym!=NULL) {
								cat_ref_ptr(te_sym);
								grorun_fork_set_postponed(fork, NULL);
							} else {
								te_sym = cat_ref_ptr(next_lah);
								consumed_next_lookahead = TRUE;
							}
						}

						GroRunState *fwd_state = grorun_shift_transition_get_forward_state(shiftTrans);

						GroRunStackEntry *new_top = grorun_stack_entry_new(top_entry, fwd_state, te_sym);
						grorun_fork_set_top_entry(fork, new_top);

						cat_log_debug("shifting to new_top=%O", new_top);
						cat_unref_ptr(new_top);
						cat_unref_ptr(te_sym);
						if (consumed_next_lookahead) {
							cat_unref_ptr(transition);
							break;
						}
					} else if (GRORUN_IS_ACCEPT_TRANSITION(transition)) {
						grorun_context_add_accepted(context, fork);
						cat_array_wo_remove(spawned, fork_idx, NULL);
						fork = NULL;
						fork_idx--;
						keep_recovering = FALSE;
						cat_unref_ptr(transition);
						break;
					} else {
						cat_log_warn("no transition....");
						cat_unref_ptr(transition);
						break;
					}
					cat_unref_ptr(transition);
				}


				if (fork!=NULL) {
					if (grorun_fork_decrease_recovery(fork)) {
						keep_recovering = FALSE;
						grorun_context_add_fork(context, fork);
						cat_array_wo_remove(spawned, fork_idx, NULL);
						fork_idx--;
						cat_log_debug("recovery successful");
					}
				}
			}
		}

		cat_unref_ptr(next_lah);
		cat_unref_ptr(spawned);
	}
	cat_unref_ptr(recoverable);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
