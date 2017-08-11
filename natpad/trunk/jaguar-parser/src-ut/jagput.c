/*
   File:    jagput.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Jul 4, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "../src/jaguarparser.h"
#include "../src/jagptokenizer.h"
#include <gio/gio.h>
#include <caterpillar.h>


static void doTest(const char *fname);
static void testTokenizer();

int main(int argc, char **argv) {

	testTokenizer();

	doTest("BasicClass.java");
//	doTest("BasicClass2.java");
//	doTest("Expressions.java");

#ifdef NATPAD_TESTING

#ifdef MONITOR_CLAZZ
	cat_ref_report();
	cat_ref_summary();
	cat_ref_cleanup();
#endif

	cat_ref_entry_report();
#endif



	return 0;
}

static void testTokenizer() {

	CatStringWo *text = cat_string_wo_new_data("public\n\rclass { 's' '\\43' 34  182l");
	CatStringInputStream *sis = cat_string_input_stream_new(text);
	CatUtf8InputStreamScanner *scanner = cat_utf8_input_stream_scanner_new(CAT_IINPUT_STREAM(sis));

	JagPTokenizer *tokenizer = jagp_tokenizer_new(CAT_IUTF8_SCANNER(scanner));
	while(TRUE) {
		JagPToken *token = jagp_tokenizer_next(tokenizer);
		if (token==NULL) {
			break;
		}
		CatStringWo *msg = cat_string_wo_new();
		cat_string_wo_format(msg, "token=%O", token);
		printf("%s\n", cat_string_wo_getchars(msg));
	}
}


static void doTest(const char *fname) {
	char *basename = g_malloc0(1000);
	readlink("/proc/self/exe", basename, 1000);
	GFile *bf = g_file_new_for_path(basename);
	GFile *bf_id = g_file_get_parent(bf);
	GFile *bf_dung = g_file_get_parent(bf_id);
	GFile *bf_module = g_file_get_parent(bf_dung);
	GFile *bf_srcut = g_file_get_child(bf_module, "src-ut");
	GFile *file = g_file_get_child(bf_srcut, fname);


//	GFile *file = g_file_new_for_commandline_arg(name);
//	GFile *file = g_file_new_for_path(fname);


	printf("file-name=%s\n", g_file_get_parse_name(file));


	GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	goffset file_length = g_file_info_get_attribute_uint64(info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
	printf("file-len=%ld\n", file_length);


	gsize contents_length = 0;
	char *contents = NULL;
	GError *error = NULL;
	if (g_file_load_contents(file, NULL, &contents, &contents_length, NULL, &error)) {
		write(1, contents, contents_length);

		CatStringInputStream *sis = cat_string_input_stream_new(cat_string_wo_new_data_len(contents, contents_length));
		CatUtf8InputStreamScanner *os = cat_utf8_input_stream_scanner_new(CAT_IINPUT_STREAM(sis));
		CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(os);

		JagPNames *names = jagp_names_new();
		JagPTokenizer *tokenizer = jagp_tokenizer_new(utf8_scanner, names);
		JagPLexerImpl *lexer_impl = jagp_lexer_impl_new(tokenizer);

		JagPParser *parser = jagp_parser_new((JagPILexer *) lexer_impl, names);
		jagp_parser_run(parser);
		cat_unref_ptr(parser);
		cat_unref_ptr(names);
	}

}
