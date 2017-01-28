//
///*
//   File:    gropurecactionsexporter.c
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jun 8, 2016
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
//#include "gropurecactionsexporter.h"
//#include "../model/gromprodsymbolpart.h"
//#include <gio/gio.h>
//
//
//static void l_export_c(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname, CatStringWo *actionhname);
//static void l_export_header(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname, CatStringWo *actionhname);
//
//void gro_pure_c_export_actions(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname) {
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
//
//
//static void l_make_declaration(CatStringWo *buf, GroClassName *cname, GroBnfString *label_string, CatStringWo *stack_type, int offset) {
//	CatStringWo *label = gro_bnf_string_get_text(label_string);
//	/* Put in the left/right value labels */
//	cat_string_wo_format(buf, "			GroToken *cup_%O = context_class->getFromTop(parser_context, %d);\n", label, offset);
////		if (true) {
////			out.println("				int cup_" + declaration.label + "left = cup_"+declaration.label+"->left;");
////			out.println("				int cup_" + declaration.label + "row = cup_"+declaration.label+"->row;");
////			out.println("				int cup_" + declaration.label + "right = cup_"+declaration.label+"->right;");
////		}
//
//		/* otherwise, just declare label. */
//	const CatStringWo *a_stack_type = stack_type==NULL ? cat_string_wo_new_data("void") : cat_ref_ptr(stack_type);
//	cat_string_wo_format(buf, "			%O *%O = (%O *) (cup_%O->value);\n", a_stack_type, label, a_stack_type, label);
//
////		String stackType = declaration.stackType==null ? "void" : declaration.stackType;
////		out.println("				" + stackType + " *" + declaration.label + " = (" + stackType + " *) (cup_"+declaration.label+"->value);");
//
//
//
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
//		cat_string_wo_format(buf, "#include <grossglibcruntime.h>\n");
//		cat_string_wo_format(buf, "\n");
//
//		cat_string_wo_format(buf, "GroToken *%O_parser_actions_run_action(GroParserContext *parser_context, int cup_action_id);\n", cname->prefix_lower);
//
//
//		cat_string_wo_format(buf, "\n");
//		cat_string_wo_format(buf, "#endif /* %O%O_H_ */\n", cname->prefix_up, cname->name_up);
//
//		const void *data = cat_string_wo_getchars(buf);
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
//
//		cat_string_wo_format(buf, "GroToken *%O_parser_actions_run_action(GroParserContext *parser_context, int cup_action_id) {\n", cname->prefix_lower, cname->prefix_hyp);
//		cat_string_wo_format(buf, "	GroParserContextClass *context_class = GRO_PARSER_CONTEXT_GET_CLASS(parser_context);\n");
//		cat_string_wo_format(buf, "	/* Symbol object for return from actions */\n");
//		cat_string_wo_format(buf, "	GroToken *cup_result = NULL;\n");
//		cat_string_wo_format(buf, "\n");
//		/* switch top */
//		cat_string_wo_format(buf, "	/* select the action based on the action number */\n");
//		cat_string_wo_format(buf, "	switch(cup_action_id) {\n");
//
//		CatIIterator *prod_iter = gro_model_production_iterator(model);
//		while(cat_iiterator_has_next(prod_iter)) {
//			GroMProduction *production = (GroMProduction *) cat_iiterator_next(prod_iter);
//			/* case label */
//			cat_string_wo_format(buf, "		case %d : { // %O\n", gro_m_production_get_index(production), gro_m_production_to_simple_string(production));
////			out.println("		case " + prod.index() + ": { // " + prod.toSimpleString());
//
//			GroMSymbol *lhs_sym = gro_m_production_get_lhs_symbol(production);
//			const CatStringWo *stack_type = gro_m_symbol_get_stack_type(lhs_sym);
//			if (stack_type==NULL) {
//				stack_type = cat_string_wo_new_data("void");
//			} else {
//				cat_ref_ptr(stack_type);
//			}
//
//			cat_string_wo_format(buf, "			%O *RESULT = NULL;\n", stack_type);
//
//
//			int rhs_count = gro_m_production_rhs_count(production);
//			int rhs_idx;
//			for(rhs_idx=0; rhs_idx<rhs_count; rhs_idx++) {
//				GObject *rhs_at = gro_m_production_rhs_at(production, rhs_idx);
//				if (GRO_IS_M_PROD_SYMBOL_PART(rhs_at)) {
//					GroMProdSymbolPart *sym_part = (GroMProdSymbolPart *) rhs_at;
//					GroBnfString *label_string = gro_m_prod_symbol_part_get_label(sym_part);
//					if (label_string!=NULL) {
//						GroMSymbol *symbol = gro_m_prod_symbol_part_get_symbol(sym_part);
//						l_make_declaration(buf, cname, label_string, gro_m_symbol_get_stack_type(symbol), rhs_count-rhs_idx-1);
//					}
//				}
//			}
//
//
//			cat_string_wo_format(buf, "			cup_result = gro_token_new_minimal(%d /*%O*/, RESULT);\n", gro_m_symbol_get_index(lhs_sym), gro_m_symbol_get_name(lhs_sym));
//
//
//
//			/* if this was the start production, do action for accept */
//			if (gro_m_production_get_index(production)==0) {
////			if (production == start_prod) {
////				out.println("		/* ACCEPT */");
////				out.println("		"+nsPreLow+"_parser_context_done_parsing(parser_context);");
//				cat_string_wo_format(buf, "			/* ACCEPT */\n");
//				cat_string_wo_format(buf, "			parser_context->done_parsing = TRUE;\n");
//			}
//
//
//			cat_string_wo_format(buf, "			return cup_result;\n");
//			cat_string_wo_format(buf, "		}\n");
//
//
//		}
//		cat_unref_ptr(prod_iter);
//		cat_string_wo_format(buf, "	}\n");
//		cat_string_wo_format(buf, "}\n");
//
//		const void *data = cat_string_wo_getchars(buf);
//		int data_len = cat_string_wo_length(buf);
//
//		g_output_stream_write((GOutputStream *) out_stream, data, data_len, NULL, NULL);
//		g_output_stream_close((GOutputStream *) out_stream, NULL, NULL);
//	}
//
//}
