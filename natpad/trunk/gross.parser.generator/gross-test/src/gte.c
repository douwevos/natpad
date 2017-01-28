
/*
   File:    gte.c
   Project: gross-test
   Author:  Douwe Vos
   Date:    Jun 14, 2016
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

#include "basic/generated/grotbaglrparsertables.h"
#include "basic/generated/grotbaglrparseractions.h"
#include "basic/grosimplescanner.h"
#include "reduceonempty/generated/groroeglrparsertables.h"
#include "reduceonempty/generated/groroeglrparseractions.h"
#include "reduceonempty/groroescanner.h"
#include "empty/generated/groempglrparsertables.h"
#include "empty/generated/groempglrparseractions.h"
#include "empty/groempscanner.h"
#include <caterpillar.h>
#include <stdlib.h>


static void l_gro_bta() {


	CatStringWo *path = cat_string_wo_new_data("/home/superman/cpp-workspace/trunk/gross.parser.generator/gross-test/test/test.txt");
	CatFileInputStream *in_stream = cat_file_input_stream_new(path);
	CatUtf8InputStreamScanner *utf8_scanner = cat_utf8_input_stream_scanner_new((CatIInputStream *) in_stream);
	GroIScanner *scanner = (GroIScanner *) gro_simple_scanner_new(utf8_scanner);

	GroGlrParser *parser = gro_glr_parser_new(&grotba_glr_parser_tables_definitions, grotba_glr_parser_actions_run_action);
	GroGlrParserContext *context = gro_glr_parser_context_new(scanner);
	gro_glr_parser_parse(parser, context);

}


static void l_gro_roe() {
	CatStringWo *path = cat_string_wo_new_data("/home/superman/cpp-workspace/trunk/gross.parser.generator/gross-test/test/reduceonempty.txt");
	CatFileInputStream *in_stream = cat_file_input_stream_new(path);
	CatUtf8InputStreamScanner *utf8_scanner = cat_utf8_input_stream_scanner_new((CatIInputStream *) in_stream);
	GroIScanner *scanner = (GroIScanner *) gro_roe_scanner_new(utf8_scanner);

	GroGlrParser *parser = gro_glr_parser_new(&groroe_glr_parser_tables_definitions, groroe_glr_parser_actions_run_action);
	GroGlrParserContext *context = gro_glr_parser_context_new(scanner);
	gro_glr_parser_parse(parser, context);
}


static void l_gro_emp() {
	CatStringWo *path = cat_string_wo_new_data("/home/superman/cpp-workspace/trunk/gross.parser.generator/gross-test/test/empty.txt");
	CatFileInputStream *in_stream = cat_file_input_stream_new(path);
	CatUtf8InputStreamScanner *utf8_scanner = cat_utf8_input_stream_scanner_new((CatIInputStream *) in_stream);
	GroIScanner *scanner = (GroIScanner *) gro_emp_scanner_new(utf8_scanner);

	GroGlrParser *parser = gro_glr_parser_new(&groemp_glr_parser_tables_definitions, groemp_glr_parser_actions_run_action);
	GroGlrParserContext *context = gro_glr_parser_context_new(scanner);
	gro_glr_parser_parse(parser, context);
}


int main(int argc, char **argv) {
	printf("Gross TEST\n");
	g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);

//	CatStringWo *path = cat_string_wo_new_data("/home/superman/cpp-workspace/trunk/gross.parser.generator/gross-test/etc/test-mini.cup");
//	CatFileInputStream *in_stream = cat_file_input_stream_new(path);
//	CatUtf8InputStreamScanner *utf8_scanner = cat_utf8_input_stream_scanner_new((CatIInputStream *) in_stream);
//	GroIScanner *scanner = (GroIScanner *) gro_scanner_new(utf8_scanner);
//
//	GroParserBase *parser = gro_parser_base_new(&gte_parser_tables_definitions, gte_parser_actions_run_action);
//	GroParserContext *context = gro_parser_context_new(scanner);
//	gro_parser_base_parse(parser, context);
//

	l_gro_roe();


	return 0;
}
