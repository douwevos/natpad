///*
//   File:    groedplainparser.c
//   Project: gross-editor
//   Author:  Douwe Vos
//   Date:    Oct 12, 2016
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
//#include "groedplainparser.h"
//#include <grossruntime.h>
//#include <grossparser.h>
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroEdPlainParser"
//#include <logging/catlog.h>
//
//#define MARKED_TERMINAL 100
//#define MARKED_NON_TERMINAL 101
//
//struct _GroEdPlainParserPrivate {
//	GroScanner *scanner;
//	CatArrayWo *token_cash;
//	GroRunITokenFactory *token_factory;
//	GroRunModel *model;
//	CatArrayWo *message_list;
//	DraKeywordPrinter *token_printer;
//	DraKeywordPrinter *line_tag_printer;
//};
//
//static void l_scanner_iface_init(GroRunIScannerInterface *iface);
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroEdPlainParser, groed_plain_parser, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroEdPlainParser)
//		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void groed_plain_parser_class_init(GroEdPlainParserClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void groed_plain_parser_init(GroEdPlainParser *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	GroEdPlainParser *instance = GROED_PLAIN_PARSER(object);
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(instance);
//	cat_unref_ptr(priv->scanner);
//	cat_unref_ptr(priv->token_factory);
//	cat_unref_ptr(priv->token_printer);
//	cat_unref_ptr(priv->line_tag_printer);
//	cat_unref_ptr(priv->model);
//	cat_unref_ptr(priv->token_cash);
//	G_OBJECT_CLASS(groed_plain_parser_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(groed_plain_parser_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//GroEdPlainParser *groed_plain_parser_new(GroScanner *scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer) {
//	GroEdPlainParser *result = g_object_new(GROED_TYPE_PLAIN_PARSER, NULL);
//	cat_ref_anounce(result);
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(result);
//	priv->scanner = cat_ref_ptr(scanner);
//	priv->token_factory = cat_ref_ptr(token_factory);
//	priv->model = cat_ref_ptr(model);
//	priv->token_cash = cat_array_wo_new();
//	priv->token_printer = cat_ref_ptr(token_printer);
//	priv->line_tag_printer = cat_ref_ptr(line_tag_printer);
//	return result;
//}
//
//
//
//static int l_map_symbol(GroEdPlainParserPrivate *priv, GroRunIToken *token);
//
//static void l_analyze_specification(GroEdPlainParser *parser, GroBnfSpec *spec, CatArrayWo *errors_list);
//static void l_analyze_spec(GroEdPlainParser *parser, GroAstSpec *spec, CatArrayWo *errors_list);
//
//
//static gint l_message_compare(gconstpointer a, gconstpointer b) {
//	GroRunLocation *loc_a = grorun_message_get_location((GroRunMessage *) a);
//	GroRunLocation *loc_b = grorun_message_get_location((GroRunMessage *) b);
//	int col_a, col_b;
//	long row_a, row_b;
//	grorun_location_get_begin(loc_a, &col_a, &row_a);
//	grorun_location_get_begin(loc_b, &col_b, &row_b);
//
//	if (row_a<row_b) {
//		return -1;
//	} else if (row_a>row_b) {
//		return 1;
//	} else if (col_a<col_b) {
//		return -1;
//	} else if (col_a>col_b) {
//		return 1;
//	}
//	return 0;
//}
//
//void groed_plain_parser_run(GroEdPlainParser *parser) {
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(parser);
//
//	GroRunIScanner *scanner = GRORUN_ISCANNER(parser);
//
////	GroActions *actions = gro_actions_new();
//
//	GroAstActions *actions = gro_ast_actions_new();
//
//	GroRunContext *context = grorun_context_new(priv->token_factory, priv->model, GRORUN_IACTION_HANDLER(actions));
//	grorun_parser_do_parse(context, scanner);
//
//	GroRunFork *fork = grorun_context_accepted_fork_first(context);
//	if (fork==NULL) {
//		if (grorun_context_error_fork_count(context)>0) {
//			fork = grorun_context_error_fork_at(context, 0);
//		}
//	}
//	cat_log_debug("#############################################################++++ fork=%O", fork);
//	if (fork) {
//
//		CatArrayWo *errors_list = cat_array_wo_new();
//
//		GroRunStackEntry *entry = grorun_fork_get_entry(fork, 1);
//		if (entry) {
//			GroRunIToken *main_lah = grorun_stack_entry_get_lookahead(entry);
//			cat_log_error("*********** %O", main_lah);
//			GObject *val = grorun_itoken_get_value(main_lah);
//			cat_log_error("*********** %O", val);
//			if (GRO_IS_BNF_SPEC(val)) {
//				GroBnfSpec *spec = (GroBnfSpec *) val;
//				l_analyze_specification(parser, spec, errors_list);
//			}
//			if (GROAST_IS_SPEC(val)) {
//				GroAstSpec *spec = (GroAstSpec *) val;
//				l_analyze_spec(parser, spec, errors_list);
//			}
//
//		}
//
//		GroRunMessage *messages = grorun_fork_get_messages(fork);
//		cat_log_debug("messages=%O", messages);
//		if (messages) {
//			while(messages) {
//				cat_array_wo_append(errors_list, (GObject *) messages);
//				messages = grorun_message_get_parent_message(messages);
//			}
//			cat_array_wo_reverse(errors_list);
//		}
//
//
//		if (cat_array_wo_size(errors_list)>0) {
//			cat_array_wo_sort(errors_list, (GCompareFunc) l_message_compare);
//			long left_row, right_row;
//			int left_column, right_column;
//			CatIIterator *iter = cat_array_wo_iterator(errors_list);
//			while(cat_iiterator_has_next(iter)) {
//				GroRunMessage *msg = cat_iiterator_next(iter);
//
//				GroRunLocation *location = grorun_message_get_location(msg);
//				grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);
//
//				DraLineTagWo *line_tag = dra_line_tag_wo_new(left_row);
//				dra_line_tag_wo_set_color(line_tag, 0.9,0,0.7);
//				dra_line_tag_wo_set_start_and_end_index(line_tag, left_column, right_column-1);
//				dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
//			}
//			cat_unref_ptr(iter);
//			cat_unref_ptr(errors_list);
//		}
//	}
//
//
//	dra_keyword_printer_flush_line_tags(priv->line_tag_printer);
//	dra_keyword_printer_reset(priv->line_tag_printer);
//
//
//
//
//
//	while(TRUE) {
//		GroRunIToken *token = grorun_iscanner_next((GroRunIScanner *) priv->scanner);
//
//		if (token) {
//			if (gro_scanner_is_eof(priv->scanner, token)) {
//				cat_unref_ptr(token);
//				break;
//			}
//			cat_array_wo_append(priv->token_cash, (GObject *) token);
//			cat_unref_ptr(token);
//		} else {
//			break;
//		}
//	}
//
//
//	CatIIterator *iter = cat_array_wo_iterator(priv->token_cash);
//	while(cat_iiterator_has_next(iter)) {
//		GroRunFullToken *token = (GroRunFullToken *) cat_iiterator_next(iter);
//
//		cat_log_debug("token=%O", token);
//
//		GroRunLocation *location = grorun_full_token_get_location(token);
//
//		int left_column, right_column;
//		long left_row, right_row;
//		grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);
//
//		dra_keyword_printer_print_fg_color(priv->token_printer, left_row, left_column, right_row, right_column, l_map_symbol(priv, (GroRunIToken *) token));
////
////		if (token->is_error) {
////			cat_log_error("error !!! row=%d, token->left=%d, token->right=%d", token->left_row, token->left, token->right);
////			DraLineTagWo *line_tag = dra_line_tag_wo_new(token->left_row);
////			dra_line_tag_wo_set_color(line_tag, 1,0,0);
////			dra_line_tag_wo_set_start_and_end_index(line_tag, token->left, token->right-1);
////			dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
////		}
//	}
//	cat_unref_ptr(iter);
//	cat_unref_ptr(context);
//
//}
//
//static GroRunIToken *l_find_token_by_start_location(GroEdPlainParser *parser, GroLocation *start_loc) {
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(parser);
//	GroRunIToken *result = NULL;
//	CatIIterator *iter = cat_array_wo_iterator(priv->token_cash);
//	int scol = gro_location_get_column(start_loc);
//	long long srow = gro_location_get_row(start_loc);
//	int col_a;
//	long long row_a;
//	while(cat_iiterator_has_next(iter)) {
//		GroRunIToken *token = (GroRunIToken *) cat_iiterator_next(iter);
//		GroRunLocation *rloc = grorun_full_token_get_location(token);
//		grorun_location_get_begin(rloc, &col_a, &row_a);
//		if (col_a==scol && row_a==srow) {
//			result = token;
//			break;
//		}
//	}
//	cat_unref_ptr(iter);
//	return result;
//}
//
//
//static void l_symbol_marker(GroEdPlainParser *parser, CatArrayWo *errors_list, GroIBnfRhsRulePart *part, CatHashMapWo *term_names, CatHashMapWo *non_term_names) {
//	if (GRO_IS_IBNF_RHS_ENLISTABLE(part)) {
//		CatArrayWo *sub_parts = gro_ibnf_rhs_enlistable_enlist((GroIBnfRhsEnlistable *) part);
//		cat_log_error("sub_parts=%O", sub_parts);
//
//		CatIIterator *iter = cat_array_wo_iterator(sub_parts);
//		cat_unref_ptr(sub_parts);
//		while(cat_iiterator_has_next(iter)) {
//			GroIBnfRhsRulePart *sub_part = (GroIBnfRhsRulePart *) cat_iiterator_next(iter);
//			l_symbol_marker(parser, errors_list, sub_part, term_names, non_term_names);
//		}
//		cat_unref_ptr(iter);
//	} else if (GRO_IS_BNF_RHS_UNARY_PART(part)) {
//		GroIBnfRhsRulePart *sub = gro_bnf_rhs_unary_part_get_part((GroBnfRhsUnaryPart *) part);
//		l_symbol_marker(parser, errors_list, sub, term_names, non_term_names);
//	} else if (GRO_IS_BNF_RHS_SYMBOL_PART(part)) {
//		GroBnfRhsSymbolPart *sym_part = (GroBnfRhsSymbolPart *) part;
//		GroBnfString *sym_id = gro_bnf_rhs_symbol_part_get_symbol_id(sym_part);
//		CatStringWo *sym_text = gro_bnf_string_get_text(sym_id);
//		if (sym_text==NULL) {
//			return;
//		}
//		GroLocation *start_loc = gro_bnf_string_get_start(sym_id);
//		GroRunIToken *token = l_find_token_by_start_location(parser, start_loc);
//		cat_log_error("sym_id=%O, token=%p", sym_id, token);
//		if (token!=NULL) {
//			GObject *t = cat_hash_map_wo_get(term_names, sym_text);
//			if (t!=NULL) {
//				grorun_full_token_set_user_index((GroRunFullToken *) token, MARKED_TERMINAL);
//			} else {
//				t = cat_hash_map_wo_get(non_term_names, sym_text);
//				if (t!=NULL) {
//					grorun_full_token_set_user_index((GroRunFullToken *) token, MARKED_NON_TERMINAL);
//				} else {
//					GroLocation *start_location = gro_bnf_string_get_start(sym_id);
//					GroLocation *end_location = gro_bnf_string_get_end(sym_id);
//
//					GroRunLocation *run_loc = grorun_location_new(gro_location_get_column(start_location), gro_location_get_row(start_location),
//							gro_location_get_column(end_location), gro_location_get_row(end_location));
//					GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new("Symbol undefinied"), run_loc);
//					cat_array_wo_append(errors_list, (GObject *) message);
//					cat_unref_ptr(message);
//					cat_unref_ptr(run_loc);
//
//				}
//
//			}
//		}
//	} else {
//		cat_log_error("part=%O", part);
//	}
//}
//
//static void l_analyze_specification(GroEdPlainParser *parser, GroBnfSpec *spec, CatArrayWo *errors_list) {
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(parser);
//
//
//	CatHashMapWo *term_names = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
//
//	GroBnfTerminalDeclarationList *decl_list = gro_bnf_spec_get_terminal_decl_list(spec);
//	CatArrayWo *decl_enlisted = gro_bnf_terminal_declaration_list_enlist(decl_list);
//	CatIIterator *iter = cat_array_wo_iterator(decl_enlisted);
//	while(cat_iiterator_has_next(iter)) {
//		GroBnfTerminalDeclaration *term_decl = (GroBnfTerminalDeclaration*) cat_iiterator_next(iter);
//		CatArrayWo *term_enlisted = gro_bnf_terminal_declaration_enlist(term_decl);
//		CatIIterator *titer = cat_array_wo_iterator(term_enlisted);
//		while(cat_iiterator_has_next(titer)) {
//			GroBnfString *term_name = cat_iiterator_next(titer);
//			cat_log_error("term_name=%O", term_name);
//			CatStringWo *term_name_text = gro_bnf_string_get_text(term_name);
//			GroBnfString *first_decl = cat_hash_map_wo_get(term_names, term_name_text);
//			if (first_decl!=NULL) {
////				GroRunIToken *token = l_find_token_with_value(term_name);
////				if (token) {
////					gro
////				}
//				GroLocation *start_location = gro_bnf_string_get_start(term_name);
//				GroLocation *end_location = gro_bnf_string_get_end(term_name);
//
//				GroRunLocation *run_loc = grorun_location_new(gro_location_get_column(start_location), gro_location_get_row(start_location),
//						gro_location_get_column(end_location), gro_location_get_row(end_location));
//				GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new("Symbol already definied"), run_loc);
//				cat_log_error("message=%O", message);
//				cat_array_wo_append(errors_list, (GObject *) message);
//				cat_unref_ptr(message);
//				cat_unref_ptr(run_loc);
//			} else {
//				cat_hash_map_wo_put(term_names, (GObject *) term_name_text, (GObject *) term_name);
//			}
//		}
//		cat_unref_ptr(titer);
//	}
//	cat_unref_ptr(iter);
//
//
//	CatHashMapWo *non_term_names = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
//
//	GroBnfProductionDefinitionList *prod_def_list = gro_bnf_spec_get_production_def_list(spec);
//	CatArrayWo *prod_def_array = gro_bnf_production_definition_list_enlist(prod_def_list);
//	iter = cat_array_wo_iterator(prod_def_array);
//	while(cat_iiterator_has_next(iter)) {
//		GroBnfProductionDefinition *prod_def = (GroBnfProductionDefinition *) cat_iiterator_next(iter);
//		GroBnfString *lhs = gro_bnf_production_definition_get_lhs_id(prod_def);
//		CatStringWo *lhs_name_text = gro_bnf_string_get_text(lhs);
//		cat_log_error("non_term_name=%O", lhs_name_text);
//		GroBnfString *first_decl = cat_hash_map_wo_get(non_term_names, lhs_name_text);
//		GroBnfString *term_decl = cat_hash_map_wo_get(term_names, lhs_name_text);
//		if (first_decl!=NULL || term_decl!=NULL) {
////				GroRunIToken *token = l_find_token_with_value(term_name);
////				if (token) {
////					gro
////				}
//			GroLocation *start_location = gro_bnf_string_get_start(lhs);
//			GroLocation *end_location = gro_bnf_string_get_end(lhs);
//
//			GroRunLocation *run_loc = grorun_location_new(gro_location_get_column(start_location), gro_location_get_row(start_location),
//					gro_location_get_column(end_location), gro_location_get_row(end_location));
//			GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new("Symbol already definied"), run_loc);
//			cat_array_wo_append(errors_list, (GObject *) message);
//			cat_unref_ptr(message);
//			cat_unref_ptr(run_loc);
//		} else {
//			cat_hash_map_wo_put(non_term_names, (GObject *) lhs_name_text, (GObject *) lhs);
//		}
//	}
//	cat_unref_ptr(iter);
//
//
//	iter = cat_array_wo_iterator(prod_def_array);
//	while(cat_iiterator_has_next(iter)) {
//		GroBnfProductionDefinition *prod_def = (GroBnfProductionDefinition *) cat_iiterator_next(iter);
//		GroBnfRhsRuleList *rhs_rule_list = gro_bnf_production_definition_get_rhs_rule_list(prod_def);
//		l_symbol_marker(parser, errors_list, (GroIBnfRhsRulePart *) rhs_rule_list, term_names, non_term_names);
//	}
//	cat_unref_ptr(iter);
//
//
//}
//
//
//
//static void l_analyze_spec(GroEdPlainParser *parser, GroAstSpec *spec, CatArrayWo *errors_list) {
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(parser);
//
//
//	CatHashMapWo *term_names = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
//
//
//
//	GroAstTerminalDeclarationList *decl_list = groast_spec_get_td(spec);
//	cat_log_error("decl_list=%O", decl_list);
//	CatArrayWo *decl_enlisted = groast_terminal_declaration_list_enlist(decl_list);
//
//	CatIIterator *iter = cat_array_wo_iterator(decl_enlisted);
//	while(cat_iiterator_has_next(iter)) {
//		GroRunIToken *tk_term_decl = (GroRunIToken *) cat_iiterator_next(iter);
//		if (tk_term_decl==NULL) {
//			cat_log_error("tk_term_decl==NULL");
//			continue;
//		}
//		GroAstTerminalDeclaration *term_decl = (GroAstTerminalDeclaration *) grorun_itoken_get_value(tk_term_decl);
//		cat_log_error("term_decl=%O", term_decl);
//		CatArrayWo *term_enlisted = groast_terminal_declaration_enlist(term_decl);
//		CatIIterator *titer = cat_array_wo_iterator(term_enlisted);
//		while(cat_iiterator_has_next(titer)) {
//			GroRunIToken *tk_term_name = (GroRunIToken *) cat_iiterator_next(titer);
//			cat_log_error("tk_term_name=%O", tk_term_name);
//			CatStringWo *term_name = grorun_itoken_get_value(tk_term_name);
//			cat_log_error("term_name=%O", term_name);
//			GroRunIToken *tk_first_decl = cat_hash_map_wo_get(term_names, term_name);
//			if (tk_first_decl!=NULL) {
//////				GroRunIToken *token = l_find_token_with_value(term_name);
//////				if (token) {
//////					gro
//////				}
//				GroRunLocation *location = grorun_full_token_get_location((GroRunFullToken *) tk_term_name);
////				GroLocation *start_location = gro_bnf_string_get_start(term_name);
////				GroLocation *end_location = gro_bnf_string_get_end(term_name);
////
////				GroRunLocation *run_loc = grorun_location_new(gro_location_get_column(start_location), gro_location_get_row(start_location),
////						gro_location_get_column(end_location), gro_location_get_row(end_location));
//				GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new("Symbol already defined"), location);
//				cat_array_wo_append(errors_list, (GObject *) message);
//				cat_unref_ptr(message);
////				cat_unref_ptr(run_loc);
//			} else {
//				cat_hash_map_wo_put(term_names, (GObject *) term_name, (GObject *) tk_term_name);
//			}
//		}
//		cat_unref_ptr(titer);
//	}
//	cat_unref_ptr(iter);
////
////
////	CatHashMapWo *non_term_names = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
////
////	GroBnfProductionDefinitionList *prod_def_list = gro_bnf_spec_get_production_def_list(spec);
////	CatArrayWo *prod_def_array = gro_bnf_production_definition_list_enlist(prod_def_list);
////	iter = cat_array_wo_iterator(prod_def_array);
////	while(cat_iiterator_has_next(iter)) {
////		GroBnfProductionDefinition *prod_def = (GroBnfProductionDefinition *) cat_iiterator_next(iter);
////		GroBnfString *lhs = gro_bnf_production_definition_get_lhs_id(prod_def);
////		CatStringWo *lhs_name_text = gro_bnf_string_get_text(lhs);
////		cat_log_error("non_term_name=%O", lhs_name_text);
////		GroBnfString *first_decl = cat_hash_map_wo_get(non_term_names, lhs_name_text);
////		GroBnfString *term_decl = cat_hash_map_wo_get(term_names, lhs_name_text);
////		if (first_decl!=NULL || term_decl!=NULL) {
//////				GroRunIToken *token = l_find_token_with_value(term_name);
//////				if (token) {
//////					gro
//////				}
////			GroLocation *start_location = gro_bnf_string_get_start(lhs);
////			GroLocation *end_location = gro_bnf_string_get_end(lhs);
////
////			GroRunLocation *run_loc = grorun_location_new(gro_location_get_column(start_location), gro_location_get_row(start_location),
////					gro_location_get_column(end_location), gro_location_get_row(end_location));
////			GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new("Symbol already definied"), run_loc);
////			cat_array_wo_append(errors_list, (GObject *) message);
////			cat_unref_ptr(message);
////			cat_unref_ptr(run_loc);
////		} else {
////			cat_hash_map_wo_put(non_term_names, (GObject *) lhs_name_text, (GObject *) lhs);
////		}
////	}
////	cat_unref_ptr(iter);
////
////
////	iter = cat_array_wo_iterator(prod_def_array);
////	while(cat_iiterator_has_next(iter)) {
////		GroBnfProductionDefinition *prod_def = (GroBnfProductionDefinition *) cat_iiterator_next(iter);
////		GroBnfRhsRuleList *rhs_rule_list = gro_bnf_production_definition_get_rhs_rule_list(prod_def);
////		l_symbol_marker(parser, errors_list, (GroIBnfRhsRulePart *) rhs_rule_list, term_names, non_term_names);
////	}
////	cat_unref_ptr(iter);
//
//
//}
//
//
//static int l_map_symbol(GroEdPlainParserPrivate *priv, GroRunIToken *token) {
//	GroRunSymbol *sym = grorun_itoken_get_symbol(token);
//	GroRunFullToken *full_token = GRORUN_FULL_TOKEN(token);
//	int sym_index = grorun_full_token_get_user_index(full_token);
//	switch(sym_index) {
//		case GRO_SYM_END_OF_INPUT : return 3;
//		case GRO_SYM_BAR : return 4;
//		case GRO_SYM_COLON : return 4;
//		case GRO_SYM_COMMA : return 4;
//		case GRO_SYM_IDENTIFIER : return 1;
//		case GRO_SYM_INDEX_MARK : return 11;
//
//		case GRO_SYM_KW_CODE : return 9;
//		case GRO_SYM_KW_IMPORTS : return 9;
//		case GRO_SYM_KW_TERMINAL : return 9;
//		case GRO_SYM_KW_TYPE : return 9;
//
//		case GRO_SYM_LSQBRACK: return 4;
//		case GRO_SYM_RSQBRACK: return 4;
//
//		case GRO_SYM_CODE_TEXT : return 2;
//		case GRO_SYM_NUMBER : return 16;
//		case GRO_SYM_RULE_SEPERATOR : return 4;
//		case GRO_SYM_SEMI : return 13;
//		case GRO_SYM_EOL_COMMENT : return 15;
//
//		case MARKED_TERMINAL : return 11;
//		case MARKED_NON_TERMINAL : return 2;
//
//		default : {} break;
//	}
//	return 1;
//}
//
//
//
//
//
///********************* start GroRunIScanner implementation *********************/
//
//
//static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
//	GroEdPlainParser *plain_parser = GROED_PLAIN_PARSER(self);
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(plain_parser);
//
//	GroRunIToken *token = NULL;
//	while(TRUE) {
//		token = grorun_iscanner_next((GroRunIScanner *) priv->scanner);
//
//		if (token!=NULL) {
//			cat_log_debug("token=%O", token);
//			if (gro_scanner_is_eof(priv->scanner, token)) {
//				break;
//			}
//
//			cat_array_wo_append(priv->token_cash, (GObject *) token);
//
//			if (grorun_full_token_get_user_index((GroRunFullToken *) token)==GRO_SYM_EOL_COMMENT) {
//				cat_unref_ptr(token);
//				continue;
//			}
//		}
//		break;
//	}
//	return token;
//}
//
//
//static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
//	iface->next = l_scanner_next;
//}
//
///********************* end GroRunIScanner implementation *********************/
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroEdPlainParser *instance = GROED_PLAIN_PARSER(self);
//	GroEdPlainParserPrivate *priv = groed_plain_parser_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%p]", iname, self);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
