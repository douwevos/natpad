//
///*
//   File:    gross.c
//   Project: gross
//   Author:  Douwe Vos
//   Date:    May 22, 2016
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
//#include <stdio.h>
//#include <string.h>
//#include <gio/gio.h>
//#include "state/grostatemachine.h"
//#include "parser/groimessagehandler.h"
//#include "parser/grostdoutmessagehandler.h"
//#include "parser/groscanner.h"
//#include "model/gromodel.h"
//#include "export/gropurecexporter.h"
//#include "export/groglibcexporter.h"
//#include "parser/file/grobnfspec.h"
//
//#include "parser/gross/groglrparsertables.h"
//#include "parser/gross/groglrparseractions.h"
//#include "parser/generated/runtime/groiscanner.h"
//#include "parser/generated/runtime/groparsercontext.h"
//#include "parser/generated/grolrparser.h"
//#include "parser/generated/runtime/grotoken.h"
//
//
//static gboolean do_verbose = FALSE;
//static CatStringWo *grammar_file = NULL;
//static CatStringWo *output_path = NULL;
//static CatStringWo *prefix = NULL;
//static GroTypeOfParser type_of_parser = GRO_PARSER_TYPE_AUTO;
//
//static gboolean parse_args(int argc, char **argv) {
//	gboolean result = TRUE;
//	int arg_idx;
//	for(arg_idx=1; arg_idx<argc; arg_idx++) {
//		const char *arg = argv[arg_idx];
//		int arg_len = strlen(arg);
//		if (arg_len==0) {
//			continue;
//		}
//		if (arg[0]=='-') {
//			if (arg_len>1) {
//				if (arg[1]=='-') {
//					if (strcmp(arg, "--output-path")==0) {
//						arg_idx++;
//						if (arg_idx<argc) {
//							output_path = cat_string_wo_new_with(argv[arg_idx]);
//						} else {
//							printf("Expected path argument following --output-path\n");
//							result = FALSE;
//						}
//					} else if (strcmp(arg, "--prefix")==0) {
//						arg_idx++;
//						if (arg_idx<argc) {
//							prefix = cat_string_wo_new_with(argv[arg_idx]);
//						} else {
//							printf("Expected prefix argument following --prefix\n");
//							result = FALSE;
//						}
//					}
//				} else {
//					int f_idx;
//					for(f_idx=1; f_idx<arg_len; f_idx++) {
//						switch(arg[f_idx]) {
//							case 'v' : do_verbose = TRUE; ; break;
//							case 'o' : {
//								arg_idx++;
//								if (arg_idx<argc) {
//									output_path = cat_string_wo_new_with(argv[arg_idx]);
//								} else {
//									printf("Expected path argument following --output-path\n");
//									result = FALSE;
//								}
//							} break;
//							case 'p' : {
//								arg_idx++;
//								if (arg_idx<argc) {
//									prefix = cat_string_wo_new_with(argv[arg_idx]);
//								} else {
//									printf("Expected prefix argument following --prefix\n");
//									result = FALSE;
//								}
//							} break;
//
//						}
//					}
//				}
//			} else {
//				printf("Ignoring '-' argument.\n");
//			}
//		} else {
//			if (grammar_file==NULL) {
//				grammar_file = cat_string_wo_new_with(arg);
//			} else {
//				printf("Grammar was already defined. Invalid argument:%s\n", arg);
//				result = FALSE;
//			}
//		}
//	}
//	return result && grammar_file!=NULL;
//}
//
//static void l_show_usage() {
//	printf("gross [options] -o <output-path> <grammar-file>\n");
//	printf("\n");
//	printf(" Options:\n");
//	printf("   -v                         verbose\n");
//	printf("   -o,--output-path <path>    output generated files in given path\n");
//	printf("   -p,--prefix <prefix>       the prefix of the generated files in CamelCase\n");
//	printf("   -l,--force-lrar            force generating an LRAR parser. By default Gross automatically\n"
//		   "                              generates either a GLR or a LRAR parser depending on whether there\n"
//		   "                              are collisions or not. This allows");
//}
//
//int main(int argc, char **argv) {
//
//	if (!parse_args(argc, argv)) {
//		l_show_usage();
//		return -1;
//	}
//	printf("G.R.O.S.S\n");
//	g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);
////	const CatStringWo *grammar_file = cat_string_wo_new_data("/home/superman/cpp-workspace/trunk/gross/etc/test.cup");
//
//	if (output_path==NULL) {
//		printf("output-path is a mandatory argument\n");
//		return -1;
//	}
//
//	GError *error = NULL;
//	GFile *file = g_file_new_for_path(cat_string_wo_getchars(grammar_file));
//	GFileInputStream *inp_stream = g_file_read(file, NULL, &error);
//	if (error) {
//		printf("Could not open given grammar file for reading. Does it exists and is readable?\n");
//		printf("error:%s\n", error->message);
//		return -1;
//	}
//	g_input_stream_close((GInputStream *) inp_stream, NULL, NULL);
//
//	GroIMessageHandler *msg_handler = (GroIMessageHandler *) gro_std_out_message_handler_new();
//
//
//	CatFileInputStream *in_stream = cat_file_input_stream_new((CatStringWo *) grammar_file);
//	CatUtf8InputStreamScanner *utf8_scanner = cat_utf8_input_stream_scanner_new((CatIInputStream *) in_stream);
//
//
//	GroIScanner *scanner = (GroIScanner *) gro_scanner_new((CatIUtf8Scanner *) utf8_scanner);
//
//
////	GroGlrParser *parser = gro_glr_parser_new(&gro_glr_parser_tables_definitions, gro_glr_parser_actions_run_action);
////	GroGlrParserContext *context = gro_glr_parser_context_new(scanner);
////	gro_glr_parser_parse(parser, context);
////	GroForkEntry *fork_entry = gro_glr_parser_context_get_best_parse_result(context, FALSE);
////	if (fork_entry) {
////		GroContent *content = gro_fork_entry_content_at(fork_entry, 0);
////		GroBnfSpec *spec = (GroBnfSpec *) gro_content_get_value(content);
//
//
//
//	GroLrParser *lr_parser = gro_lr_parser_new(scanner);
//	GroParserContext *parser_context = gro_parser_context_new(scanner);
//	GroToken *token = gro_parser_base_parse((GroParserBase *) lr_parser, parser_context);
//	if (token!=NULL) {
//		GroBnfSpec *spec = (GroBnfSpec *) token->value;
//
//
//
//		GroModel *model = gro_bnf_spec_create_model(msg_handler, spec);
//		if (model!=NULL) {
//
//			gro_model_compute_nullability(model);
//			gro_model_compute_first_sets(model);
//
//			GroStateMachine *machine = gro_state_machine_new(model, type_of_parser);
//			gro_state_machine_build(machine);
//
//			gro_model_dump_terminals(model);
//			gro_model_dump_non_terminals(model);
//
//			gro_state_machine_dump(machine);
//
////			GroPureCExporter *exporter = gro_pure_c_exporter_new(output_path);
////			gro_pure_c_exporter_do_export(exporter, model, machine);
//
//			GroGlibCExporter *gexporter = gro_glibc_exporter_new(output_path, prefix);
//			gro_glibc_exporter_do_export(gexporter, model, machine, spec);
//
//		}
//	}
//	return 0;
//}
