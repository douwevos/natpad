/*
   File:    tergrammarreader.c
   Project: tern
   Author:  Douwe Vos
   Date:    Jun 1, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "tergrammarreader.h"
#include "../tercolors.h"

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerGrammarReader"
#include "logging/catlog.h"

/*
 * http://www.wolosoft.com/en/superedi/help/textpad_syntax.html
 */


/* priority-list
 *
 * BracketChars
 * OperatorChars
 * Keywords 5
 * ...
 * Keywords 1
 *
 *
 *
 *
 * If SyntaxStart and/or SyntaxEnd are defined the 'Preprocessor keywords' are ignored.
 *
 * if both 'abstract' and 'abs' are defined as keywords then:
 *
 *   abs         keyword 'abs'
 *   abstr       no keyword
 *   abstract    keyword 'abstract'
 *   abs(tract)  keyword 'abs'
 */

/*
 * Keywords are single set of non white-space characters. A keyword-line containing white-space separated text is completely ignored. White spaces at the start and end of a keyword
 * line are ignored (trimmed). Keywords can be present in multiple sections but the higher keyword sections are prioritized
 */
CatS ter_s_sect_keywords_1 = CAT_S_DEF("Keywords 1");
CatS ter_s_sect_keywords_2 = CAT_S_DEF("Keywords 2");
CatS ter_s_sect_keywords_3 = CAT_S_DEF("Keywords 3");
CatS ter_s_sect_keywords_4 = CAT_S_DEF("Keywords 4");
CatS ter_s_sect_keywords_5 = CAT_S_DEF("Keywords 5");
CatS ter_s_sect_syntax = CAT_S_DEF("Syntax");
CatS ter_s_sect_preprocessor_keywords = CAT_S_DEF("Preprocessor keywords");	// TODO

CatS ter_s_preproc_start = CAT_S_DEF("PreprocStart");	// TODO


CatS ter_s_ignore_case = CAT_S_DEF("IgnoreCase");	// TODO

/*
 * Define the characters to start and to stop the parsing. SyntaxStart and SyntaxEnd can be the same.
 * example1:
 *   SyntaxStart = dow
 *   SyntaxStart = wod
 *
 *   text (no-high) dow (blue) dow (black) keword1 (blue) wod(blue) wod(black) wod(black)
 *
 * The color of the characters is that of 'Keywords 1' (default blue 0xFF0000)
 */
CatS ter_s_syntax_start = CAT_S_DEF("SyntaxStart");	// TODO
CatS ter_s_syntax_end = CAT_S_DEF("SyntaxEnd");	// TODO


CatS ter_s_bracket_chars = CAT_S_DEF("BracketChars");

CatS ter_s_operator_chars = CAT_S_DEF("OperatorChars");


CatS ter_s_namespace1 = CAT_S_DEF("Namespace1");	// TODO

/**
 * Defines the exact length of keywords, keywords shorter or longer are ignored
 */
CatS ter_s_key_word_length = CAT_S_DEF("KeyWordLength");	// TODO

/*
 * 'KeyWordChars' and 'InitKeyWordChars' are ignored
 */
CatS ter_s_key_word_chars = CAT_S_DEF("KeyWordChars");
CatS ter_s_init_key_word_chars = CAT_S_DEF("InitKeyWordChars");


/*
 * Numbers are automatically recognized. Valid characters are 0-9eEfFlL\. start characters are 0-9\.
 * The characters eEfFlL can be used only once in a number part.
 * A new number part start with a \. I.e: 34e.6ef is a valid number
 */
CatS ter_s_hex_prefix = CAT_S_DEF("HexPrefix");


/*
 * A character is not limited by a single character but acts more like a string. CharEsc can only be one character and the character following it
 * is considered to be escaped. CharStart and CharEnd can only be one character as well.
 */
CatS ter_s_char_esc = CAT_S_DEF("CharEsc");	// TODO
CatS ter_s_char_start = CAT_S_DEF("CharStart");	// TODO
CatS ter_s_char_end = CAT_S_DEF("CharEnd");	// TODO

CatS ter_s_comment_start_alt = CAT_S_DEF("CommentStartAlt");	// TODO
CatS ter_s_comment_end_alt = CAT_S_DEF("CommentEndAlt");	// TODO

CatS ter_s_comment_start = CAT_S_DEF("CommentStart");
CatS ter_s_comment_end = CAT_S_DEF("CommentEnd");


/*
 * a string of character
 */
CatS ter_s_single_comment = CAT_S_DEF("SingleComment");
/*
 * the column where a single-line comment must start or 'Leading" to start in the first non-blank
 */
CatS ter_s_single_comment_col = CAT_S_DEF("SingleCommentCol");

/*
 * the escape character for single line comments (both). This appears to do nothing in mode C=1, should test on PERL=1
 */
CatS ter_s_single_comment_esc = CAT_S_DEF("SingleCommentEsc");	// TODO

CatS ter_s_single_comment_alt = CAT_S_DEF("SingleCommentAlt");
CatS ter_s_single_comment_col_alt = CAT_S_DEF("SingleCommentColAlt");

CatS ter_s_string_end = CAT_S_DEF("StringEnd");
CatS ter_s_string_start = CAT_S_DEF("StringStart");
CatS ter_s_strings_span_lines = CAT_S_DEF("StringsSpanLines");	// TODO
CatS ter_s_string_esc = CAT_S_DEF("StringEsc");	// TODO
CatS ter_s_string_alt = CAT_S_DEF("StringAlt");	// TODO


CatS ter_s_leading = CAT_S_DEF("leading");

G_DEFINE_TYPE(TerGrammarReader, ter_grammar_reader, G_TYPE_OBJECT) // @suppress("Unused static function")

static void l_dispose(GObject *object);

static void ter_grammar_reader_class_init(TerGrammarReaderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void ter_grammar_reader_init(TerGrammarReader *reader) {
}

static void l_dispose(GObject *object) {
	G_OBJECT_CLASS(ter_grammar_reader_parent_class)->dispose(object);
}


TerGrammarReader *ter_grammar_reader_new() {
	TerGrammarReader *result = g_object_new(TER_TYPE_GRAMMAR_READER, NULL);
	cat_ref_anounce(result);
	return result;
}

static void _read_line(CatUtf8InputStreamScanner *in, CatUnicharArray *buffer, CatStreamStatus *stream_status) {
	cat_unichar_array_clear(buffer, FALSE);
	gboolean keep_running = TRUE;
	while(keep_running) {
		gunichar uch = cat_utf8_input_stream_scanner_read(in, stream_status);
		if (*stream_status==CAT_STREAM_OK) {
			if (uch==0xA || uch==0xD) {
				keep_running = FALSE;
			} else {
				cat_unichar_array_append_uni_char(buffer, uch);
			}
		} else {
			keep_running = FALSE;
		}
	}
}

static void l_parse_keyword(TerSyntax *syntax, CatUnicharArray *line, int keyword_nr) {
	TerUnicharNode *node = ter_syntax_create_keyword_end_node(syntax, line);
	ter_unichar_node_set_keyword_ns1(node, TER_COLOR_KEYWORDS_1+keyword_nr);
}

static void l_parse_bracket_set(CatHashSet *unichar_set, CatUnicharArray *value) {
	if (value==NULL) {
		return;
	}
	gunichar start_ch = -1;
	gunichar prev_ch = -1;
	int idx;
	for(idx=0; idx<cat_unichar_array_length(value); idx++) {

		gunichar cur_ch = cat_unichar_array_char_at(value, idx);
		if (start_ch!=-1) {
			while(start_ch<=cur_ch) {
				CatUnichar *uchobj = cat_unichar_new(start_ch);
				cat_hash_set_add(unichar_set, (GObject *) uchobj);
				cat_unref_ptr(uchobj);
				start_ch++;
			}
			start_ch = -1;
		} else if (cur_ch=='-' && prev_ch!=-1) {
			start_ch = prev_ch;
			CatUnichar *uchobj = cat_unichar_new(start_ch);
			cat_hash_set_add(unichar_set, (GObject *) uchobj);
			cat_unref_ptr(uchobj);
			start_ch++;
		} else {
			CatUnichar *uchobj = cat_unichar_new(cur_ch);
			cat_hash_set_add(unichar_set, (GObject *) uchobj);
			cat_unref_ptr(uchobj);
		}

		prev_ch = cur_ch;
	}
}

static void l_parse_syntax_line(TerSyntax *syntax, CatUnicharArray *line) {
	int idx = cat_unichar_array_uni_char_index_of(line, (gunichar) '=');
	if (idx<=0) {
		return;
	}

	int line_len = cat_unichar_array_length(line);
	CatUnicharArray *key_arr = cat_unichar_array_substring(line, 0, idx);
	idx++;
	CatUnicharArray *value = cat_unichar_array_substring(line, idx, line_len-idx);

	cat_log_debug("idx=%d", idx);

	cat_unichar_array_trim(key_arr);
	cat_log_debug("trimmed");
	CatStringWo *a_key = cat_unichar_array_to_string(key_arr);
	cat_ref_sink_ptr(key_arr);
	cat_unref_ptr(key_arr);
	cat_ref_sink_ptr(a_key);
	cat_log_debug("key created");


	cat_unichar_array_trim(value);
	if (cat_unichar_array_length(value)==0) {
		cat_unref_ptr(value);
	} else {
		cat_ref_sink_ptr(value);
	}

	if (cat_string_wo_equal(a_key, CAT_S(ter_s_hex_prefix))) {
		ter_syntax_set_hex_prefix(syntax, value);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_bracket_chars))) {
		l_parse_bracket_set(ter_syntax_get_bracket_set(syntax), value);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_operator_chars))) {
		l_parse_bracket_set(ter_syntax_get_operator_char_set(syntax), value);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_single_comment))) {
		ter_syntax_set_single_comment(syntax, value);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_single_comment_col))) {
		int col = 0;
		if (value) {
			CatStringWo *a_valtext = cat_unichar_array_to_string(value);
			CatStringWo *e_valtext = cat_string_wo_clone(a_valtext, CAT_CLONE_DEPTH_MAIN);
			cat_string_wo_to_lowercase(e_valtext);
			if (cat_string_wo_equal(e_valtext, CAT_S(ter_s_leading))) {
				col = -1;
			} else {
				col = atoi(cat_string_wo_getchars(e_valtext));
			}
			cat_unref_ptr(e_valtext);
		}
		ter_syntax_set_single_comment_column(syntax, col);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_single_comment_alt))) {
		ter_syntax_set_single_comment_alt(syntax, value);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_single_comment_col_alt))) {
		int col = 0;
		if (value) {
			CatStringWo *a_valtext = cat_unichar_array_to_string(value);
			CatStringWo *e_valtext = cat_string_wo_clone(a_valtext, CAT_CLONE_DEPTH_MAIN);
			cat_string_wo_to_lowercase(e_valtext);
			if (cat_string_wo_equal(e_valtext, CAT_S(ter_s_leading))) {
				col = -1;
			} else {
				col = atoi(cat_string_wo_getchars(e_valtext));
			}
			cat_unref_ptr(e_valtext);
		}
		ter_syntax_set_single_comment_column_alt(syntax, col);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_comment_start))) {
		ter_syntax_set_multi_line_comment_start(syntax, value);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_comment_end))) {
		ter_syntax_set_multi_line_comment_end(syntax, value);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_string_start))) {
		cat_log_debug("parsing string start");
		gunichar string_start = 0;
		if (value) {
			string_start = cat_unichar_array_char_at(value, 0);
		}
		ter_syntax_set_string_start(syntax, string_start);
	} else if (cat_string_wo_equal(a_key, CAT_S(ter_s_string_end))) {
		gunichar string_end = 0;
		if (value) {
			string_end = cat_unichar_array_char_at(value, 0);
		}
		ter_syntax_set_string_end(syntax, string_end);
	}

	cat_unref_ptr(value);
	cat_unref(a_key);
}

#define _TER_MAIN		0
#define _TER_SYNTAX		1
#define _TER_KEYWORDS1	2
#define _TER_KEYWORDS2	3
#define _TER_KEYWORDS3	4
#define _TER_KEYWORDS4	5
#define _TER_KEYWORDS5	6
#define _TER_KEYWORDS6	7

TerSyntax *ter_grammar_reader_read(CatIInputStream *in_stream) {
	CatUtf8InputStreamScanner *in = cat_utf8_input_stream_scanner_new(CAT_IINPUT_STREAM(in_stream));
	CatStreamStatus stream_status = CAT_STREAM_OK;

	TerSyntax *syntax = ter_syntax_new();

	gboolean keep_running = TRUE;
	CatUnicharArray *buffer = cat_unichar_array_new();
	cat_ref_sink_ptr(buffer);

	int idx = 0;
	int section = _TER_MAIN;

	while(keep_running) {
		_read_line(in, buffer, &stream_status);
		if (stream_status!=CAT_STREAM_OK) {
			break;
		}
		cat_log_on_detail({
			gchar *bu = cat_unichar_array_to_gchars(buffer);
			cat_log_detail("section=%d, line[%d] = %s", section, idx, bu);
			cat_free_ptr(bu);
		});
		cat_unichar_array_trim(buffer);
		idx++;

		gunichar fch = cat_unichar_array_char_at(buffer, 0);
		if (fch==';') {
			cat_log_detail("line is comment");
			continue;
		}
		if (fch=='[') {
			int fidx = cat_unichar_array_uni_char_index_of(buffer, ']');
			cat_log_detail("fidx=%d", fidx);
			if (fidx>=0) {
				CatUnicharArray *blocked_array = cat_unichar_array_substring(buffer, 1, fidx-1);
				CatStringWo *a_blocked = cat_unichar_array_to_string(blocked_array);
				cat_log_trace("blocked=%p, blocked_array=%p", a_blocked, blocked_array);
				if (blocked_array) {
					cat_log_trace("blocked_array->data=%p, size=%d", blocked_array->data, blocked_array->size);
				}
				cat_unref_ptr(blocked_array);
				if (cat_string_wo_equal(a_blocked, CAT_S(ter_s_sect_syntax))) {
					section = _TER_SYNTAX;
					cat_log_detail("syntax section starts here");
					cat_unref_ptr(a_blocked);
					continue;
				} else if (cat_string_wo_equal(a_blocked, CAT_S(ter_s_sect_keywords_1))) {
					section = _TER_KEYWORDS1;
					cat_log_detail("keyword 1 section starts here");
					cat_unref_ptr(a_blocked);
					continue;
				} else if (cat_string_wo_equal(a_blocked, CAT_S(ter_s_sect_keywords_2))) {
					section = _TER_KEYWORDS2;
					cat_log_detail("keyword 2 section starts here");
					cat_unref_ptr(a_blocked);
					continue;
				} else if (cat_string_wo_equal(a_blocked, CAT_S(ter_s_sect_keywords_3))) {
					section = _TER_KEYWORDS3;
					cat_log_detail("keyword 3 section starts here");
					cat_unref_ptr(a_blocked);
					continue;
				} else if (cat_string_wo_equal(a_blocked, CAT_S(ter_s_sect_keywords_4))) {
					section = _TER_KEYWORDS4;
					cat_log_detail("keyword 4 section starts here");
					cat_unref_ptr(a_blocked);
					continue;
				} else if (cat_string_wo_equal(a_blocked, CAT_S(ter_s_sect_keywords_5))) {
					section = _TER_KEYWORDS5;
					cat_log_detail("keyword 5 section starts here");
					cat_unref_ptr(a_blocked);
					continue;
//				} else if (cat_string_wo_equal(a_blocked, CAT_S(ter_s_sect_keywords_6))) {
//					section = _TER_KEYWORDS6;
//					cat_log_detail("keyword 6 section starts here");
//					cat_unref_ptr(a_blocked);
//					continue;
				}
			}
		}

		switch(section) {
			case _TER_MAIN : {

			} break;

			case _TER_SYNTAX : {
				l_parse_syntax_line(syntax, buffer);
				cat_log_debug("end syn");
			} break;

			case _TER_KEYWORDS1 : {
				l_parse_keyword(syntax, buffer, 0);
			} break;

			case _TER_KEYWORDS2 : {
				l_parse_keyword(syntax, buffer, 1);
			} break;
			case _TER_KEYWORDS3 : {
				l_parse_keyword(syntax, buffer, 2);
			} break;
			case _TER_KEYWORDS4 : {
				l_parse_keyword(syntax, buffer, 3);
			} break;
			case _TER_KEYWORDS5 : {
				l_parse_keyword(syntax, buffer, 4);
			} break;
			case _TER_KEYWORDS6  : {
				l_parse_keyword(syntax, buffer, 5);
			} break;
		}

	}

	cat_unref_ptr(buffer);
	cat_unref_ptr(in);
	cat_unref_ptr(in_stream);
	cat_log_detail("done");
	return syntax;
}
