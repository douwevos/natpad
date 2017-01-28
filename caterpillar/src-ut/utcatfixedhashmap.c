///*
// * utcatfixedhashmap.c
// *  Created on: Nov 5, 2009
// */
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "../src/catlib.h"
//#include "../src/catfixedhashmap.h"
//#include "../src/catstring.h"
//
//#include "../src/io/utf8/catiutf8printer.h"
//#include "../src/io/utf8/catutf8inputstreamscanner.h"
//#include "../src/io/utf8/catutf8outputstreamprinter.h"
//#include "../src/io/catfileinputstream.h"
//#include "../src/io/catstdoutstream.h"
//
//
//#include "../src/logging/catlogdefs.h"
//#define CAT_LOG_LEVEL CAT_LOG_FATAL
////#define CAT_LOG_CLAZZ "CatShtParserString"
//#include "../src/logging/catlog.h"
//
//
//typedef struct _Persoon Persoon;
//
//struct _Persoon {
//	CatFixedString *naam;
//};
//
//
//int main2(int argc, char **argv) {
//	CatFixedHashMap *map = cat_fixed_hash_map_new((GHashFunc) cat_fixed_string_hash, (GEqualFunc) cat_fixed_string_equal);
//	printf("1\n");
//	map = cat_fixed_hash_map_put(map, (GObject *) cat_fixed_string_new("douwe"), (GObject *) cat_fixed_string_new("vos"));
//	map = cat_fixed_hash_map_put(map, (GObject *) cat_fixed_string_new("klaas"), (GObject *) cat_fixed_string_new("jan"));
//	map = cat_fixed_hash_map_put(map, (GObject *) cat_fixed_string_new("Christiane"), (GObject *) cat_fixed_string_new("Maier"));
//	CatFixedString *last_name = (CatFixedString *) cat_fixed_hash_map_get(map, cat_fixed_string_new("douwe"));
//	char *txt = cat_fixed_string_to_gchars(last_name);
//	printf("last_name=%s\n", txt);
//
//
//
//
////	char *ptxt = "Persoon { Naam = \"Douwe\";  John = ''; Adres { Street='willemstraat 7'; }  }";
////
////	CatShtParser *parser = cat_sht_parser_new(ptxt, strlen(ptxt));
////	cat_sht_parser_parse(parser);
////	CatShtEntry *entry;
////	while((entry=cat_sht_parser_next(parser))!=NULL) {
////		CatFixedString *dump = cat_sht_entry_dump(entry);
////		cat_ref_sink_ptr(dump);
////		char *txt = cat_fixed_string_to_gchars(dump);
////		printf("entry=%lx, %s\n", (gulong) entry, txt);
////		g_free(txt);
////		cat_unref(dump);
////	}
//
//
//	return 0;
//}
//
//
//int test_stream() {
//	CatFileInputStream *in_stream = cat_file_input_stream_new(cat_fixed_string_new("/home/superman/work/cpp-workspace2/caterpillar/src-ut/test.sht"));
//	CatUtf8InputStreamScanner *in = cat_utf8_input_stream_scanner_new(CAT_IINPUT_STREAM(in_stream));
//
//	CatStdoutStream *out_stream = cat_stdout_stream_new();
//	CatStreamStatus stream_status;
//
//	gboolean keep_running = TRUE;
//	char m[30];
//	while(keep_running) {
//		int read_cnt = cat_utf8_input_stream_scanner_read_length(in, m, sizeof(m), &stream_status);
//		if (stream_status==CAT_STREAM_OK && read_cnt>0) {
////			gchar *out = g_ucs4_to_utf8(m, read_cnt, NULL, NULL, NULL);
//			cat_stdout_stream_write_length(out_stream, m, read_cnt, &stream_status);
////			printf("\n%2x %d:", m[read_cnt-1], read_cnt);
//		} else {
//			keep_running = FALSE;
//		}
//	}
//	fflush(stdout);
//	char *t = "doni!!\n";
//	fwrite(t, 1, strlen(t), stdout);
////	printf("done!!\n");
//	printf("done!!\n");
//	return 0;
//}
//
////static CatStaticString txt_douwe = { "douwe", NULL };
//
////CAT_DEFINE_STRING(txt_douwe, "douwe");
////CAT_DEFINE_STATIC_STRING(txt_hello_world, "Hello world this is me");
//
//
//CatFixedString *_current_work_directory() {
//	char *data = g_malloc(4096);
//	char *txt = getcwd(data, 4096);
//	CatFixedString *result = NULL;
//	if (txt) {
//		result= cat_fixed_string_new(txt);
//	}
//	g_free(data);
//	return result;
//}
//
//CatFixedString *_parent_dir(CatFixedString *in) {
//	int lidx = cat_fixed_string_last_index_of(in, '/');
//	if (lidx>0) {
//		in = cat_fixed_string_substring(in, 0, lidx);
//	}
//	return in;
//}
//
//int main4(int argc, char **argv) {
//
////	CatFixedString *k = CAT_GET_STATIC_STRING(txt_default);
//
////	g_object_ref_sink(k);
////
////	char *txt = cat_fixed_string_to_gchars(k);
////	printf("text=%s %lx\n", txt, (gulong) k);
////
////	cat_unref(k);
//
//
////	CatFixedString *k2 = CAT_GET_STATIC_STRING(txt_hello_world);
//
//
////	char *txt = cat_fixed_string_to_gchars(k2);
////	printf("text=%s %lx\n", txt, (gulong) k2);
//
//
////	CatFileInputStream *in_stream = cat_file_input_stream_new(cat_fixed_string_new("/home/superman/work/cpp-workspace2/caterpillar/src-ut/test.sht"));
////	CatUtf8InputStreamScanner *in = cat_utf8_input_stream_scanner_new((CatIInputStream *) in_stream);
////
////	CatStdoutStream *out_stream = cat_stdout_stream_new();
////	CatUtf8OutputStreamPrinter *utf8_printer = cat_utf8_output_stream_printer_new((CatIOutputStream *) out_stream);
////	CatShtPrinter *sht_printer = cat_sht_printer_new(utf8_printer);
////	CatShtConsumeAndPrint *consume_and_print = cat_sht_consume_and_print_new(sht_printer);
////
////	CatShtParser *parser = cat_sht_parser_new(CAT_IUTF8_SCANNER(in), consume_and_print);
////	cat_sht_parser_parse(parser);
//
////
////	CatFixedString *cwd = _current_work_directory();
////	cat_ref_sink_ptr(cwd);
//////	CatFixedString *project_home = _parent_dir(cwd);
////	CatFixedString *project_home = cat_fixed_string_append_chars(cwd, "/src-ut/etc/textpad-test-grammar.syn");
////	char *ptx = cat_fixed_string_to_gchars(project_home);
////	printf("ptxt=%s\n", ptx);
////
////
////	StaSyntax *syntax = sta_grammar_reader_read(project_home);
////	StaLexer *lexer = sta_lexer_new(syntax);
////
////	CatFixedString *text_path = cat_fixed_string_append_chars(cwd, "/src-ut/etc/textpad-lexer-test.txt");
////
////	CatFileInputStream *in_stream = cat_file_input_stream_new(text_path);
////	CatUtf8InputStreamScanner *text_scanner = cat_utf8_input_stream_scanner_new(CAT_IINPUT_STREAM(in_stream));
////
////	sta_lexer_parse(lexer, CAT_IUTF8_SCANNER(text_scanner));
//
//	return 0;
//}
