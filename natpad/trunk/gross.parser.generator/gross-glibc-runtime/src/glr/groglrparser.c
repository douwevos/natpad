
/*
   File:    groglrparser.c
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 22, 2016
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

#include "groglrparser.h"
#include "../gro2dintarray.h"

struct _GroGlrParserPrivate {
	GroParserConfig config;
	GroGlrRunActionCB run_action_cb;
	Gro2DIntArray *production_tab;
	Gro2DIntArray *action_tab;
	Gro2DIntArray *reduce_tab;
	Gro2DIntArray *fork_tab;
	int error_sync_cnt;
};

G_DEFINE_TYPE_WITH_CODE(GroGlrParser, gro_glr_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroGlrParser)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_glr_parser_class_init(GroGlrParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_glr_parser_init(GroGlrParser *instance) {
}

static void l_dispose(GObject *object) {
	gro_log("dispose:%p", object);
//	GroGlrParser *instance = GRO_GLR_PARSER(object);
//	GroGlrParserPrivate *priv = gro_glr_parser_get_instance_private(instance);
	G_OBJECT_CLASS(gro_glr_parser_parent_class)->dispose(object);
	gro_log("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	gro_log("finalize:%p", object);
	G_OBJECT_CLASS(gro_glr_parser_parent_class)->finalize(object);
	gro_log("finalized:%p", object);
}


GroGlrParser *gro_glr_parser_new(GroParserConfig *parser_config, GroGlrRunActionCB run_action_cb) {
	GroGlrParser *result = g_object_new(GRO_TYPE_GLR_PARSER, NULL);
	GroGlrParserPrivate *priv = gro_glr_parser_get_instance_private(result);
	priv->config = *parser_config;
	priv->run_action_cb = run_action_cb;
	priv->production_tab = gro_2d_int_array_new(parser_config->production_table);
	priv->reduce_tab = gro_2d_int_array_new(parser_config->reduce_table);
	priv->action_tab = gro_2d_int_array_new(parser_config->action_table);

	if (parser_config->fork_table) {
		priv->fork_tab = gro_2d_int_array_new(parser_config->fork_table);
	} else {
		priv->fork_tab = NULL;
	}
	priv->error_sync_cnt = 6;
	return result;
}


static void l_dump_fork(GroForkEntry *fork_entry, int fork_idx) {
#ifdef GROSS_DEBUG
	GroStateAndSymbol *sas = gro_fork_entry_get_state_and_symbol(fork_entry);
	printf("FORK[%d, status=%d]\n", fork_idx, gro_fork_entry_get_status(fork_entry));
	printf("  state		symbol\n");
	while(sas) {
		printf("  %d		%d\n", gro_state_and_symbol_get_state(sas), gro_state_and_symbol_get_symbol(sas));
		sas = gro_state_and_symbol_get_linked(sas);
	}


	GroContent *content = gro_fork_entry_content_at(fork_entry, 0);
	printf("  value\n");
	while(content) {
		GObject *val = gro_content_get_value(content);

		printf("  %s / %p\n", g_type_name_from_instance((GTypeInstance *) val), val);
		content = gro_content_get_linked(content);
	}
#endif
}

static void l_dump_stacks(GroGlrParserContext *context, GroToken *lookahead_token) {
#ifdef GROSS_DEBUG
	int fork_count = gro_glr_parser_context_fork_entry_count(context);
	int fork_idx;
	printf("LOOKAHEAD=%d\n", lookahead_token->sym);
	for(fork_idx=0; fork_idx<fork_count; fork_idx++) {
		GroForkEntry *fork_entry = gro_glr_parser_context_fork_entry_at(context, fork_idx);
		l_dump_fork(fork_entry, fork_idx);
	}
	printf("------------------------------\n");
#endif
}



/* Pop the state/symbols until a shift to the error symbol is allowed. If a shift is possible then shift a dummy error-symbol.
 */
static gboolean l_pop_to_recovery_config(GroGlrParser *parser, GroForkEntry *fork_entry) {
	GroForkEntryState status = gro_fork_entry_get_status(fork_entry);
	if (status!=GRO_FORK_ENTRY_NORMAL) {
		return FALSE;
	}

	GroGlrParserPrivate *priv = gro_glr_parser_get_instance_private(parser);
	int error_sym = priv->config.error_symbol;
	int skip = 0;
	int act;
	GroStateAndSymbol *sas = gro_fork_entry_get_state_and_symbol(fork_entry);
	while(sas) {
		act = gro_glr_parser_parse_get_action(parser, gro_state_and_symbol_get_state(sas), error_sym);
		gro_log("action %d", act);
		if (act>0 && act<10000) {
			/* found a shift on the error symbol */
			break;
		} else if (act>=10000) {
			// TODO
			gro_log("action is forking on error !!!");
		}

		skip++;
		sas = gro_state_and_symbol_get_linked(sas);
	}

	gboolean result = FALSE;
	if (sas) {
		result = TRUE;
		int new_state = act-1;
		GroStateAndSymbol *new_sas = gro_state_and_symbol_new(sas, new_state, error_sym, TRUE);
		gro_fork_entry_set_error_config(fork_entry, new_sas, skip);
	}
	return result;
}



void gro_glr_parser_parse(GroGlrParser *parser, GroGlrParserContext *context) {
	GroGlrParserPrivate *priv = gro_glr_parser_get_instance_private(parser);

	gro_glr_parser_context_init_state(context, priv->config.start_state);

	while(!gro_glr_parser_context_parsing_done(context)) {

		GroToken *lookahead_token = gro_glr_parser_context_scan_next(context);


		l_dump_stacks(context, lookahead_token);


		gboolean has_recovery_forks = FALSE;
		gboolean all_accepted = TRUE;
		int fork_count = gro_glr_parser_context_fork_entry_count(context);
		int fork_idx;
		for(fork_idx=0; fork_idx<fork_count; fork_idx++) {
			GroForkEntry *fork_entry = gro_glr_parser_context_fork_entry_at(context, fork_idx);
			GroForkEntryState status = gro_fork_entry_get_status(fork_entry);
			GroStateAndSymbol *sas = gro_fork_entry_get_state_and_symbol(fork_entry);
			if (status==GRO_FORK_ENTRY_ERROR_PARENT) {
				GroForkEntry *sub_fe = gro_fork_entry_create_fork(fork_entry, sas, 0);
				gro_glr_parser_context_insert_fork_entry(context, sub_fe, fork_idx+1);
				fork_count++;
				has_recovery_forks = TRUE;
				continue;
			}


			/* handle fork_entry till it wants to shift */
			gboolean fork_entry_done = FALSE;
			while(!(gro_fork_entry_is_accepted(fork_entry) || fork_entry_done)) {
				l_dump_fork(fork_entry, fork_idx);
				int act = gro_fork_entry_get_and_reset_pending_action(fork_entry);
				if (act==0) {
					act = gro_glr_parser_parse_get_action(parser, gro_state_and_symbol_get_state(sas), lookahead_token->sym);
				}
				gro_log("handling action=%d", act);

				if (act>=10000) {
					/* FORK */
					int fork_tab_idx = act-10000;
					int fork_sub_cnt = 0;
					int const *fork_sub_row = gro_2d_int_array_get_row(priv->fork_tab, fork_tab_idx, &fork_sub_cnt);
					for(fork_sub_cnt--; fork_sub_cnt>=0; fork_sub_cnt--) {
						int sub_act = fork_sub_row[fork_sub_cnt];
						gro_log("sub-act=%d", sub_act);
						if (fork_sub_cnt==0) {
							gro_fork_entry_set_pending_action(fork_entry, sub_act);
						} else {
							GroForkEntry *sub_fe = gro_fork_entry_create_fork(fork_entry, sas, sub_act);
							gro_glr_parser_context_insert_fork_entry(context, sub_fe, fork_idx+1);
							fork_count++;
						}
					}

				} else if (act>0) {
					/* SHIFT */
					fork_entry_done = TRUE;
					int new_state = act-1;
					gro_log("shift new_state=%d", new_state);
					GroStateAndSymbol *new_sas = gro_state_and_symbol_new(sas, new_state, lookahead_token->sym, TRUE);
					gro_fork_entry_set_state_and_symbol(fork_entry, new_sas);
					sas = new_sas;


					gro_fork_entry_shift(fork_entry, lookahead_token->value, lookahead_token->left_row, lookahead_token->left, lookahead_token->right_row, lookahead_token->right);

//					int content_count = gro_fork_entry_content_count(fork_entry);
//					GPtrArray *shifted_content_list = g_ptr_array_sized_new(content_count);
//					int content_idx;
//					for(content_idx=0; content_idx<content_count; content_idx++) {
//						GroContent *content = gro_fork_entry_content_at(fork_entry, content_idx);
//						GroContent *shifted_content = gro_content_new(content, lookahead_token->value, lookahead_token->left_row, lookahead_token->left, lookahead_token->right_row, lookahead_token->right);
//						g_ptr_array_add(shifted_content_list, shifted_content);
//					}
//
//					gro_fork_entry_set_content_list(fork_entry, shifted_content_list);

				} else if (act<0) {
					/* REDUCE */
					int reduce_action = ((-act) - 1);


					/* look up information about the production */
					int lhs = gro_2d_int_array_get(priv->production_tab, reduce_action, 0);
					int rhs_count = gro_2d_int_array_get(priv->production_tab, reduce_action, 1);

					gro_log("reduce action=%d, lhs=%d, rhs_count=%d", reduce_action, lhs, rhs_count);

					/* reduce on values level */
					int content_count = gro_fork_entry_content_count(fork_entry);
					GPtrArray *reduced_content_list = g_ptr_array_sized_new(content_count);
					int content_idx;
					for(content_idx=0; content_idx<content_count; content_idx++) {
						GroContent *content = gro_fork_entry_content_at(fork_entry, content_idx);
						GroContent *reduced_content = priv->run_action_cb(context, reduce_action, content, lhs, rhs_count);
						g_ptr_array_add(reduced_content_list, reduced_content);
					}

					gro_fork_entry_set_content_list(fork_entry, reduced_content_list);

					/* reduce on state and symbol level*/
					int rc = rhs_count;
					GroStateAndSymbol *super_sas = sas;
					while(rc>0) {
						super_sas = gro_state_and_symbol_get_linked(super_sas);
						rc--;
					}

					int super_state = gro_state_and_symbol_get_state(super_sas);
					int new_state = gro_glr_parser_parse_get_reduce(parser, super_state, lhs);
					GroStateAndSymbol *new_sas = gro_state_and_symbol_new(super_sas, new_state, lhs, FALSE);
					gro_fork_entry_set_state_and_symbol(fork_entry, new_sas);
					sas = new_sas;

					if (reduce_action == priv->config.start_production) {
						gro_log("ACCEPT");
//						gro_glr_parser_context_set_parsing_done(context, TRUE);
						gro_fork_entry_mark_accepted(fork_entry);
						fork_entry_done = TRUE;
					}

				} else {
					/* error */
					gro_fork_entry_add_error_message(fork_entry, lookahead_token->value, lookahead_token->left_row, lookahead_token->left, lookahead_token->right_row, lookahead_token->right);
					if (l_pop_to_recovery_config(parser, fork_entry)) {
						gro_log("popped to recovery config");
						l_dump_fork(fork_entry, fork_idx);
					} else {


						if (lookahead_token->sym==priv->config.eof_symbol && (status!=GRO_FORK_ENTRY_ERROR_PARENT)) {
							gro_fork_entry_mark_accepted(fork_entry);
							fork_idx++;
						} else {
							gro_log("removing fork:fork_idx=%d", fork_idx);
//							gro_glr_parser_context_remove_fork_entry(context, fork_idx);
//							fork_count--;
							gro_fork_entry_mark_accepted(fork_entry);
							fork_idx++;
						}
					}
					fork_entry_done = TRUE;
					fork_idx--;
				}
			}


			if (!gro_fork_entry_is_accepted(fork_entry)) {
				all_accepted = FALSE;
			}

			/* The fork_entry shifted to the next symbol. Lets see if this fork_entry now is in the same branch
			 * as one of the fork_entries lower in the list.
			 */
			int fork_chk_idx;
			for(fork_chk_idx=fork_idx-1; fork_chk_idx>=0; fork_chk_idx--) {
				GroForkEntry *fork_chk_entry = gro_glr_parser_context_fork_entry_at(context, fork_chk_idx);
				GroStateAndSymbol *chk_sas = gro_fork_entry_get_state_and_symbol(fork_chk_entry);
				if (gro_state_and_symbol_mergable(chk_sas, sas)) {
					// TODO merge !!!
					break;
				}

			}
		}


		if (has_recovery_forks) {
			/* One or more recovery forks are pending.
			 * For each GroForkEntry with the status GRO_FORK_ENTRY_ERROR_PARENT run through the list of fork_entries
			 * to analyze the child (status GRO_FORK_ENTRY_SYNC). If the child syncing ForkEntries contain an entry
			 * that has the status Accept then remove all ForkEntries belonging to the parent-error group including the
			 * parent itself. Please note that forking during error-syncing is not forbidden and multiple routes may
			 * have the status ACCEPT. In such a case all syncing forks that have the status ACCEPT and the same
			 * error_sync count are kept */

			fork_count = gro_glr_parser_context_fork_entry_count(context);
			for(fork_idx=0; fork_idx<fork_count; fork_idx++) {
				GroForkEntry *fork_entry = gro_glr_parser_context_fork_entry_at(context, fork_idx);
				GroForkEntryState state = gro_fork_entry_get_status(fork_entry);
				if (state==GRO_FORK_ENTRY_ERROR_PARENT) {
					/* Collect all children for this fork_entry and increase their error-sync value . */
					int fork_sub_idx;
					int best_error_sync = -1;
					gboolean has_accepted_sub = FALSE;
					gboolean has_children = FALSE;
					for(fork_sub_idx=0; fork_sub_idx<fork_count; fork_sub_idx++) {
						GroForkEntry *fork_sub_entry = gro_glr_parser_context_fork_entry_at(context, fork_sub_idx);
						if (!gro_fork_entry_is_child_of(fork_sub_entry, fork_entry)) {
							continue;
						}
						has_children = TRUE;
						int error_sync = gro_fork_entry_increase_and_get_sync_count(fork_sub_entry);
						if (error_sync>=0) {
							if (gro_fork_entry_get_status(fork_sub_entry)==GRO_FORK_ENTRY_ACCEPTED) {
								has_accepted_sub = TRUE;
								if (error_sync<best_error_sync || best_error_sync==-1) {
									best_error_sync = error_sync;
								}
							} else if (error_sync>priv->error_sync_cnt) {
								if (error_sync<best_error_sync || best_error_sync==-1) {
									best_error_sync = error_sync;
								}
							}
						}
					}


					/* If the best_error_sync is set (thus not -1) then there is at least one child which was accepted
					 * or is considered to be far enough recovered. */
					if (best_error_sync!=-1) {
						for(fork_sub_idx=0; fork_sub_idx<fork_count; fork_sub_idx++) {
							GroForkEntry *fork_sub_entry = gro_glr_parser_context_fork_entry_at(context, fork_sub_idx);
							if (!gro_fork_entry_is_child_of(fork_sub_entry, fork_entry)) {
								continue;
							}

							gboolean remove_sub = FALSE;
							if (best_error_sync==gro_fork_entry_get_sync_count(fork_sub_entry)) {
								if (has_accepted_sub) {
									if (gro_fork_entry_get_status(fork_sub_entry)==GRO_FORK_ENTRY_ACCEPTED) {
										gro_fork_entry_set_error_parent(fork_sub_entry, NULL);
									} else {
										remove_sub = TRUE;
									}
								} else {
									gro_fork_entry_mark_as_recovered(fork_sub_entry);
								}

							} else {
								remove_sub = TRUE;
							}

							if (remove_sub) {
								gro_glr_parser_context_remove_fork_entry(context, fork_sub_idx);
								fork_sub_idx--;
								fork_count--;
							}
						}

						/* Remove the current ForkEntry which was the base of the error recovery. It seems it has
						 * either recovered or one of the recovery configurations was accepted.
						 */
						gro_glr_parser_context_remove_fork_entry(context, fork_idx);
						fork_idx--;
						fork_count--;
					} else if (!has_children && (lookahead_token->sym==priv->config.eof_symbol)) {
						gro_glr_parser_context_remove_fork_entry(context, fork_idx);
						fork_idx--;
						fork_count--;
//						gro_fork_entry_mark_accepted(fork_entry);
					}
				}
			}




		}


		if (all_accepted) {
			gro_log("all were ACCEPTED");
			gro_glr_parser_context_set_parsing_done(context, TRUE);
		}
	}
}



int gro_glr_parser_parse_get_action(GroGlrParser *parser, int state, int sym) {
	GroGlrParserPrivate *priv = gro_glr_parser_get_instance_private(parser);
	int tag;
	int first, last, probe;
	int row_length = 0;
	int const *row = gro_2d_int_array_get_row(priv->action_tab, state, &row_length);
	gro_log("get_action:row[ %d ]=%p, sym=%d, length=%d", state, row, sym, row_length);

	/* linear search if we are < 10 entries */
	if (row_length < 20) {
		for (probe = 0; probe < row_length; probe++) {
			/* is this entry labeled with our Symbol or the default? */
			tag = row[probe++];
			if (tag == sym) {
				/* return the next entry */
				return row[probe];
			}
		}
	/* otherwise binary search */
	} else {
		first = 0;
		last = (row_length - 1) / 2 ; /* leave out trailing default entry */
		while (first <= last) {
			probe = (first + last) / 2;
//			gro_log("probe=%d, row[probe*2]=%d", probe, row[probe * 2]);
			if (sym == row[probe * 2]) {
				return row[probe * 2 + 1];
			} else if (sym > row[probe * 2]) {
				first = probe + 1;
			} else {
				last = probe - 1;
			}
		}
	}

	/*
	 * shouldn't happened, but if we run off the end we return the default
	 */
	return 0;
}




/**
 * Fetch a state from the reduce-goto table. The table is broken up into
 * rows, one per state (rows are indexed directly by state number). Within
 * each row, a list of index, value pairs are given (as sequential entries
 * in the table), and the list is terminated by a default entry (denoted
 * with a Symbol index of -1). To find the proper entry in a row we do a
 * linear search.
 *
 * @param state the state index of the entry being accessed.
 * @param sym   the Symbol index of the entry being accessed.
 */
int gro_glr_parser_parse_get_reduce(GroGlrParser *parser, int state, int sym) {
	GroGlrParserPrivate *priv = gro_glr_parser_get_instance_private(parser);
	int tag;
	int row_length;
	gro_log("get reduce:sym=%d, state=%d", sym, state);
	int const *row = gro_2d_int_array_get_row(priv->reduce_tab, state, &row_length);
	gro_log("get reduce_tab[ %d ]=%p, sym=%d, row_length=%d", state, row, sym, row_length);

	/* if we have a null row we go with the default */
	if (row == NULL)
		return -1;

	int probe;
	for(probe = 0; probe < row_length; probe++) {
		/* is this entry labeled with our Symbol or the default? */
		tag = row[probe++];
//		cat_log_trace("tag=%d", tag);
		if (tag == sym || tag == -1) {
			/* return the next entry */
			return row[probe];
		}
	}
	/* if we run off the end we return the default (error == -1) */
	return -1;
}

