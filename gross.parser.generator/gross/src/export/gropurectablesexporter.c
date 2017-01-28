///*
//   File:    gropurectablesexporter.c
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jun 12, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "gropurectablesexporter.h"
//#include "../model/gromprodsymbolpart.h"
//#include "../state/groparseactiontable.h"
//#include "../state/groparseactionrow.h"
//#include "../state/groparsereducetable.h"
//#include "../state/groparsereducerow.h"
//#include "../state/groiparseaction.h"
//#include "../state/groactionreduce.h"
//#include "../state/groactionshift.h"
//#include "../state/groactionfork.h"
//#include <gio/gio.h>
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroPureCExporter"
//#include <logging/catlog.h>
//
//
//static void l_export_c(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname, CatStringWo *actionhname);
//static void l_export_header(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname, CatStringWo *actionhname);
//static void l_emit_production_table(CatStringWo *out, GroModel *model, GroClassName *cname);
//static gboolean l_emit_action_table(CatStringWo *out, GroModel *model, GroStateMachine *state_machine, GroClassName *cname);
//static void l_emit_reduce_table(CatStringWo *out, GroModel *model, GroStateMachine *state_machine, GroClassName *cname);
//
//void gro_pure_c_export_tables(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname) {
//	CatStringWo *actionhname = cat_string_wo_new();
//	CatStringWo *compname = cat_string_wo_clone(cname->name_hyp, CAT_CLONE_DEPTH_FULL);
//	cat_string_wo_to_lowercase(compname);
//	cat_string_wo_format(actionhname, "%O%O.h", cname->prefix_lower, compname);
//
//	l_export_header(model, state_machine, output_path, cname, actionhname);
//	l_export_c(model, state_machine, output_path, cname, actionhname);
//}
//
//
//static void l_export_header(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname, CatStringWo *actionhname) {
//	GFile *file = g_file_get_child(output_path, cat_string_wo_getchars(actionhname));
//	GFileOutputStream *out_stream = g_file_replace(file, NULL, FALSE, 0, NULL, NULL);
//	if (out_stream) {
//
//		CatStringWo *buf = cat_string_wo_new();
//
//		cat_string_wo_format(buf, "#ifndef %O%O_H_\n", cname->prefix_up, cname->name_up);
//		cat_string_wo_format(buf, "#define %O%O_H_\n", cname->prefix_up, cname->name_up);
//		cat_string_wo_format(buf, "\n");
//
//		cat_string_wo_format(buf, "#include <grossglibcruntime.h>\n", actionhname);
//		cat_string_wo_format(buf, "\n");
//		cat_string_wo_format(buf, "GroTableDefs %O_%O_definitions;\n", cname->prefix_lower, cname->name_lower);
//		cat_string_wo_format(buf, "\n");
//		cat_string_wo_format(buf, "#endif /* %O%O_H_ */\n", cname->prefix_up, cname->name_up);
//
//		const char *data = cat_string_wo_getchars(buf);
//		int data_len = cat_string_wo_length(buf);
//		g_output_stream_write((GOutputStream *) out_stream, data, data_len, NULL, NULL);
//		g_output_stream_close((GOutputStream *) out_stream, NULL, NULL);
//	}
//}
//
//
//static void l_export_c(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname, CatStringWo *actionhname) {
//	CatStringWo *actioncname = cat_string_wo_new_sub(actionhname, 0, cat_string_wo_length(actionhname)-2);
//	cat_string_wo_append_chars_len(actioncname, ".c", 2);
//
//	GFile *file = g_file_get_child(output_path, cat_string_wo_getchars(actioncname));
//	GFileOutputStream *out_stream = g_file_replace(file, NULL, FALSE, 0, NULL, NULL);
//	if (out_stream) {
//
//		CatStringWo *buf = cat_string_wo_new();
//
//		cat_string_wo_format(buf, "#include \"%O\"\n", actionhname);
//		cat_string_wo_format(buf, "\n");
//
//		l_emit_production_table(buf, model, cname);
//		gboolean has_fork_table  = l_emit_action_table(buf, model, state_machine, cname);
//		l_emit_reduce_table(buf, model, state_machine, cname);
//
//		cat_string_wo_format(buf, "\nGroTableDefs %O_%O_definitions = {\n", cname->prefix_lower, cname->name_lower);
//		cat_string_wo_format(buf, "	%O_%O_production_table,\n", cname->prefix_lower, cname->name_lower);
//		cat_string_wo_format(buf, "	%O_%O_action_table,\n", cname->prefix_lower, cname->name_lower);
//		cat_string_wo_format(buf, "	%O_%O_reduce_table,\n", cname->prefix_lower, cname->name_lower);
//		if (has_fork_table) {
//			cat_string_wo_format(buf, "	%O_%O_fork_table,\n", cname->prefix_lower, cname->name_lower);
//		} else {
//			cat_string_wo_format(buf, "	NULL,\n");
//		}
//
//		GroLalrState *start_state = gro_state_machine_get_start_state(state_machine);
//		cat_string_wo_format(buf, "	%d,	/* start-state */\n", gro_lalr_state_get_index(start_state));
//
//		GroMProduction *start_production = gro_model_get_start_production(model);
//		cat_string_wo_format(buf, "	%d,	/* start-production */\n", gro_m_production_get_index(start_production));
//
//		GroMTerminal *eof_term = gro_model_get_terminal_eof(model);
//		cat_string_wo_format(buf, "	%d,	/* EOF-terminal */\n", gro_m_symbol_get_index((GroMSymbol *) eof_term));
//
//		GroMTerminal *error_term = gro_model_get_terminal_error(model);
//		cat_string_wo_format(buf, "	%d	/* error-terminal */\n", gro_m_symbol_get_index((GroMSymbol *) error_term));
//
//		cat_string_wo_format(buf, "};\n");
//
//
//		const char *data = cat_string_wo_getchars(buf);
//		int data_len = cat_string_wo_length(buf);
//
//		g_output_stream_write((GOutputStream *) out_stream, data, data_len, NULL, NULL);
//		g_output_stream_close((GOutputStream *) out_stream, NULL, NULL);
//	}
//}
//
//static void l_emit_table(CatStringWo *out, int *col_size, int *table, int row_size, int row_count, int default_value);
//
//
//
//
//static void l_emit_production_table(CatStringWo *out, GroModel *model, GroClassName *cname) {
//	int prod_count = gro_model_production_count(model);
//	int *prod_table = (int *) g_malloc0(sizeof(int)*prod_count*2);
//
//
//	CatIIterator *iter = gro_model_production_iterator(model);
//	while(cat_iiterator_has_next(iter)) {
//		GroMProduction *prod = (GroMProduction *) cat_iiterator_next(iter);
//		int row = gro_m_production_get_index(prod)*2;
//		GroMSymbol *lhs_sym = gro_m_production_get_lhs_symbol(prod);
//		prod_table[row] = gro_m_symbol_get_index(lhs_sym);
//		prod_table[row+1] = gro_m_production_rhs_count(prod);
//	}
//	cat_unref_ptr(iter);
//
//	cat_string_wo_append_chars(out, "/** Production table. */\n");
//	cat_string_wo_format(out, "const short %O_%O_production_table[] = {\n", cname->prefix_lower, cname->name_lower);
//	l_emit_table(out, NULL, prod_table, 2, prod_count, -1);
//	cat_string_wo_append_chars(out, "};\n\n");
//
//	g_free(prod_table);
//}
//
//
//
//static gboolean l_emit_action_table(CatStringWo *out, GroModel *model, GroStateMachine *state_machine, GroClassName *cname) {
//	GroParseActionTable *act_tab = gro_state_machine_get_action_table(state_machine);
//	gboolean result = FALSE;
////	ParseActionRow row;
////	ParseAction act;
////	int red;
////
//
//	/* collect values for the action table */
//	int num_rows = gro_parse_action_table_row_count(act_tab);
//	int num_terms = gro_model_terminal_count(model);
//	int *act_table = g_malloc0(sizeof(int) * num_rows * num_terms*2);
//	int *act_row_size = g_malloc0(sizeof(int) * num_rows);
//
//	CatArrayWo *fork_actions = cat_array_wo_new();
//	int max_fork_action_count = 0;
//
//	/* do for each state (row) of the action table */
//	int row_idx;
//	for(row_idx=0; row_idx<num_rows; row_idx++) {
//		GroParseActionRow *row = gro_parse_action_table_get_row(act_tab, row_idx);
//		int entry_idx = 0;
//		int col_idx;
//		int *act_table_row = act_table+num_terms*row_idx*2;
//		for(col_idx=0; col_idx<num_terms; col_idx++) {
//			GroIParseAction *action = gro_parse_action_row_get_action(row, col_idx);
//			GroActionKind kind = gro_iparse_action_kind(action);
//			if (kind!=GRO_ACTION_KIND_ERROR) {
//				cat_log_debug("row=%d, col_idx=%d, kind=%d, action=%p", row_idx, col_idx, kind, action);
//			}
//			switch(kind) {
//				case GRO_ACTION_KIND_ERROR : {
//				} break;
//				case GRO_ACTION_KIND_NONASSOC : {
//				} break;
//				case GRO_ACTION_KIND_REDUCE : {
//					/* reduce actions get negated entries of production# + 1 */
//					GroMProduction *production = gro_action_reduce_get_production((GroActionReduce *) action);
//					cat_log_debug("production=%o", production);
//					int index = gro_m_production_get_index(production);
//					act_table_row[entry_idx++] = col_idx;
//					act_table_row[entry_idx++] = -(index+1);
//				} break;
//				case GRO_ACTION_KIND_SHIFT : {
//					GroLalrState *state = gro_action_shift_get_shift_to((GroActionShift *) action);
//					int index = gro_lalr_state_get_index(state);
//					act_table_row[entry_idx++] = col_idx;
//					act_table_row[entry_idx++] = index+1;
//				} break;
//				case GRO_ACTION_KIND_FORK : {
//					cat_array_wo_append(fork_actions, (GObject *) action);
//					GroActionFork *fork_action = (GroActionFork *) action;
//					int fork_index = gro_action_fork_get_index(fork_action);
//					int facnt = gro_action_fork_action_count(fork_action);
//					if (facnt>max_fork_action_count) {
//						max_fork_action_count = facnt;
//					}
//					act_table_row[entry_idx++] = col_idx;
//					act_table_row[entry_idx++] = 10000+fork_index;
//				} break;
//			}
//		}
//		act_row_size[row_idx] = entry_idx;
//	}
//
//	cat_string_wo_append_chars(out, "/** action table. */\n");
//	cat_string_wo_format(out, "const short %O_%O_action_table[] = {\n", cname->prefix_lower, cname->name_lower);
//	l_emit_table(out, act_row_size, act_table, num_terms*2, num_rows, 1);
//	cat_string_wo_append_chars(out, "};\n\n");
//
//
//
//	if (cat_array_wo_size(fork_actions)) {
//		cat_array_wo_sort(fork_actions, (GCompareFunc) gro_action_fork_compare);
//
//		GroActionFork *action_fork = (GroActionFork *) cat_array_wo_get_last(fork_actions);
//		int fork_action_row_cnt = gro_action_fork_get_index(action_fork)+1;
//
//		int *fork_table = g_malloc0(sizeof(int) * fork_action_row_cnt * max_fork_action_count);
//		int *fork_row_size = g_malloc0(sizeof(int) * fork_action_row_cnt);
//
//		CatIIterator *fa_iter = cat_array_wo_iterator(fork_actions);
//		while(cat_iiterator_has_next(fa_iter)) {
//			action_fork = (GroActionFork *) cat_iiterator_next(fa_iter);
//			int fork_index = gro_action_fork_get_index(action_fork);
//			int *fork_table_row = fork_table+max_fork_action_count*fork_index;
//			fork_row_size[fork_index] = gro_action_fork_action_count(action_fork);
//			CatIIterator *act_iter = gro_action_fork_action_iterator(action_fork);
//			int entry_idx=0;
//			while(cat_iiterator_has_next(act_iter)) {
//				GroIParseAction *act_sub = (GroIParseAction *) cat_iiterator_next(act_iter);
//				switch(gro_iparse_action_kind(act_sub)) {
//					case GRO_ACTION_KIND_ERROR :
//					case GRO_ACTION_KIND_FORK :
//					case GRO_ACTION_KIND_NONASSOC : {
//					} break;
//					case GRO_ACTION_KIND_REDUCE : {
//						/* reduce actions get negated entries of production# + 1 */
//						GroMProduction *production = gro_action_reduce_get_production((GroActionReduce *) act_sub);
//						cat_log_debug("production=%o", production);
//						int index = gro_m_production_get_index(production);
//						fork_table_row[entry_idx++] = -(index+1);
//					} break;
//					case GRO_ACTION_KIND_SHIFT : {
//						GroLalrState *state = gro_action_shift_get_shift_to((GroActionShift *) act_sub);
//						int index = gro_lalr_state_get_index(state);
//						fork_table_row[entry_idx++] = index+1;
//					} break;
//				}
//			}
//			cat_unref_ptr(act_iter);
//		}
//		cat_unref_ptr(fa_iter);
//
//
//		cat_string_wo_append_chars(out, "/** fork table. */\n");
//		cat_string_wo_format(out, "const short %O_%O_fork_table[] = {\n", cname->prefix_lower, cname->name_lower);
//		l_emit_table(out, act_row_size, act_table, max_fork_action_count, fork_action_row_cnt, 0);
//		cat_string_wo_append_chars(out, "};\n\n");
//		result = TRUE;
//	}
//	return result;
//}
//
//
//static void l_emit_reduce_table(CatStringWo *out, GroModel *model, GroStateMachine *state_machine, GroClassName *cname) {
//	GroParseReduceTable *reduce_table = gro_state_machine_get_reduce_table(state_machine);
//	int num_rows = gro_parse_reduce_table_row_count(reduce_table);
//	int num_non_terms = gro_model_non_terminal_count(model);
//	int *red_table = g_malloc0(sizeof(int) * num_rows * num_non_terms*2);
//	int *red_row_size = g_malloc0(sizeof(int) * num_rows);
//
//	int row_idx;
//	for(row_idx=0; row_idx<num_rows; row_idx++) {
//		GroParseReduceRow *row = gro_parse_reduce_table_get_row(reduce_table, row_idx);
//		int entry_idx = 0;
//		int col_idx;
//		int *act_table_row = red_table+num_non_terms*row_idx*2;
//		for(col_idx=0; col_idx<num_non_terms; col_idx++) {
//			GroLalrState *to_state = gro_parse_reduce_row_get_to_state(row, col_idx);
//			if (to_state == NULL) {
//				continue;
//			}
//			act_table_row[entry_idx++] = col_idx;
//			act_table_row[entry_idx++] = gro_lalr_state_get_index(to_state);
//		}
//		red_row_size[row_idx] = entry_idx;
//	}
//
//	cat_string_wo_append_chars(out, "/** reduce table. */\n");
//	cat_string_wo_format(out, "const short %O_%O_reduce_table[] = {\n", cname->prefix_lower, cname->name_lower);
//	l_emit_table(out, red_row_size, red_table, num_non_terms*2, num_rows, 1);
//	cat_string_wo_append_chars(out, "};\n\n");
//}
//
//
//
//
//#define TAB_PREFIX "\t"
//
//	// print a string array encoding the given short[][] array.
//static void l_emit_table(CatStringWo *out, int *cols_per_row, int *table, int row_size, int row_count, int default_value) {
//
//	cat_string_wo_format(out, TAB_PREFIX "/* the number of rows, default_value */\n");
//	cat_string_wo_format(out, TAB_PREFIX "%d, %d,\n", row_count, default_value);
//	cat_string_wo_format(out, TAB_PREFIX "/* the number of columns for each row */");
//
//	int *columns_per_row = cols_per_row;
//
//	if (cols_per_row==NULL) {
//		columns_per_row = g_malloc0(row_count*sizeof(int));
//
//		int row;
//		for(row=0; row<row_count; row++) {
//			int *crow = table+row_size*row;
//			int col_size = 0;
//			int col;
//			for(col=0; col<row_size; col++) {
//				if (crow[col]!=default_value) {
//					col_size = col+1;
//				}
//			}
//			columns_per_row[row] = col_size;
//		}
//	}
//
//	int row;
//	int idx = 0;
//	for(row=0; row<row_count; row++) {
//		if (row%24==0) {
//			if (row!=0) {
//				cat_string_wo_append_chars(out, ",");
//			}
//			cat_string_wo_append_chars(out, "\n" TAB_PREFIX);
//		} else {
//			cat_string_wo_append_chars(out, ",");
//		}
//		cat_string_wo_format(out, "%d", columns_per_row[row]);
//	}
//	cat_string_wo_append_chars(out, ",\n");
//
//
//	cat_string_wo_format(out, TAB_PREFIX "/* the raw table data */");
//	for(row=0; row<row_count; row++) {
//		int *crow = table+row_size*row;
//		int col_size = columns_per_row[row];
//		int col;
//		cat_string_wo_format(out, "\n/* %d */" TAB_PREFIX, row);
//		for(col=0; col<col_size; col++) {
////			if (idx%24==0) {
//				if (idx!=0) {
//					cat_string_wo_append_chars(out, ",");
//				}
////				cat_string_wo_append_chars(out, "\n" TAB_PREFIX);
////			} else {
////				cat_string_wo_append_chars(out, ",");
////			}
//			cat_string_wo_format(out, "%d", crow[col]);
//			idx++;
//		}
//	}
//	cat_string_wo_append_chars(out, "\n");
//
//
////String prefix = "\t";
////
////	out.print(prefix+"/* the raw table data */");
////	int idx = 0;
////	for(int row=0; row<sa.length; row++) {
////		if (configuration.symbolInfo) {
////			out.print(prefix+"/* start row "+row+" */");
////		}
////		for(int col=0; col<sa[row].length; col++) {
////			if (idx%24==0) {
////				if (idx!=0) {
////					out.print(",");
////				}
////				out.print("\n"+prefix);
////			} else {
////				out.print(", ");
////			}
////			out.print(sa[row][col]);
////			idx++;
////		}
////	}
////	out.println();
//}
//
//
