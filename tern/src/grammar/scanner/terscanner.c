/*
   File:    terscanner.c
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 19, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "terscanner.h"
#include "../tercolors.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerScanner"
#include <logging/catlog.h>

struct _TerScannerPrivate {
	TerSyntax *syntax;
	CatIUtf8Scanner *scanner;
	DraKeywordPrinter *token_printer;
	CatUnicharArray *scan_buffer;
	int scan_buffer_offset;
	CatStreamStatus stream_status;
	gunichar unichar;
	int row, column;
	gboolean test_for_keyword;
	gboolean next_test_for_keyword;
	gboolean leading_only_whitespaces;
};

G_DEFINE_TYPE_WITH_PRIVATE(TerScanner, ter_scanner, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_scanner_class_init(TerScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_scanner_init(TerScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerScanner *instance = TER_SCANNER(object);
	TerScannerPrivate *priv = ter_scanner_get_instance_private(instance);
	cat_unref_ptr(priv->syntax);
	cat_unref_ptr(priv->scan_buffer);
	cat_unref_ptr(priv->scanner);
	cat_unref_ptr(priv->token_printer);
	G_OBJECT_CLASS(ter_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

TerScanner *ter_scanner_new(TerSyntax *syntax, CatIUtf8Scanner *scanner, DraKeywordPrinter *token_printer) {
	TerScanner *result = g_object_new(TER_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	TerScannerPrivate *priv = ter_scanner_get_instance_private(result);
	priv->syntax = cat_ref_ptr(syntax);
	priv->scan_buffer = cat_unichar_array_new();
	priv->scanner = cat_ref_ptr(scanner);
	priv->token_printer = cat_ref_ptr(token_printer);
	cat_ref_sink_ptr(priv->scan_buffer);

	cat_log_on_trace({
		ter_unichar_node_dump(ter_syntax_get_keyword_node(priv->syntax), NULL);
	});

	return result;
}

static void l_look_ahead(TerScannerPrivate *priv) {
	cat_log_trace("priv->scan_buffer_offset=%d, cat_unichar_array_length(priv->scan_buffer)=%d", priv->scan_buffer_offset, cat_unichar_array_length(priv->scan_buffer));
	if (priv->scan_buffer_offset < cat_unichar_array_length(priv->scan_buffer)) {
		priv->unichar = cat_unichar_array_char_at(priv->scan_buffer, priv->scan_buffer_offset++);
	} else {
		priv->unichar = cat_iutf8_scanner_next_char(priv->scanner, &priv->stream_status);
		if (priv->stream_status!=CAT_STREAM_OK) {
			priv->unichar = -1;
		} else {
			cat_unichar_array_append_uni_char(priv->scan_buffer, priv->unichar);
			priv->scan_buffer_offset++;
		}
	}

	cat_log_trace("priv->scan_buffer_offset=%d, priv->unichar=%d", priv->scan_buffer_offset, priv->unichar);
}

static void l_look_ahead_and_shift(TerScannerPrivate *priv) {
	l_look_ahead(priv);
	cat_unichar_array_remove(priv->scan_buffer,0, 1);
	priv->scan_buffer_offset--;
}


static void l_look_ahead_reset(TerScannerPrivate *priv) {
	priv->scan_buffer_offset = 0;
}


static gboolean l_print_node(TerScannerPrivate *priv, TerUnicharNode *unichar_node) {
	gboolean result = FALSE;
	int ns1_code = ter_unichar_node_get_keyword_ns1(unichar_node);
	cat_log_trace("unichar_node=%p, ns1_code=%d", unichar_node, ns1_code);
	if (ns1_code>0) {
		gushort level = ter_unichar_node_get_level(unichar_node);
		dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+level, ns1_code);
		cat_unichar_array_remove(priv->scan_buffer, 0, level);
		priv->column += level;
		priv->scan_buffer_offset = 0;
		result = TRUE;
	}
	return result;
}

static gboolean l_attempt_keyword(TerScannerPrivate *priv) {
	if (!priv->test_for_keyword) {
		return FALSE;
	}

	priv->next_test_for_keyword = FALSE;

	TerUnicharNode *root_node = ter_syntax_get_keyword_node(priv->syntax);
	TerUnicharNode *unichar_node = root_node;
	cat_log_trace("unichar_node=%p", unichar_node);


	while(TRUE) {
		l_look_ahead(priv);
		cat_log_trace("%d : %c", priv->unichar, (char) priv->unichar);
		if (priv->unichar==-1) {
			return root_node==unichar_node ? FALSE : l_print_node(priv, unichar_node);
		}


		TerUnicharNode *next_node = ter_unichar_node_get(unichar_node, priv->unichar);
		cat_log_trace("unichar_node=%p, next_node=%p", unichar_node, next_node);
		if (next_node) {
			unichar_node = next_node;
		} else {
			CatUnichar *uchobj = cat_unichar_new(priv->unichar);
			cat_log_trace("uchobj=%p, is_keyword_char=%d", uchobj, ter_syntax_is_keyword_char(priv->syntax, uchobj));

			if (ter_syntax_is_keyword_char(priv->syntax, uchobj)) {
				cat_unref_ptr(uchobj);
				return FALSE;
			}
			cat_unref_ptr(uchobj);
			return root_node==unichar_node ? FALSE : l_print_node(priv, unichar_node);
		}
	}

	return FALSE;
}

static gboolean l_attempt_unichar_set(TerScannerPrivate *priv, CatHashSet *unichar_set, int code) {
	gboolean result = FALSE;
	int cnt = 0;
	while(TRUE) {
		l_look_ahead(priv);
		if (priv->unichar==-1) {
			break;
		}
		CatUnichar *uchobj = cat_unichar_new(priv->unichar);
		if (cat_hash_set_get(unichar_set, (GObject *) uchobj)!=NULL) {
			cnt++;
		} else {
			cat_unref_ptr(uchobj);
			break;
		}
		cat_unref_ptr(uchobj);
	}

	if (cnt>0) {
		dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+cnt, code);

		cat_unichar_array_remove(priv->scan_buffer, 0, cnt);
		priv->column += cnt;
		priv->scan_buffer_offset = 0;
		result = TRUE;
	}

	return result;
}


static gboolean l_attempt_hex_number(TerScannerPrivate *priv) {
	CatUnicharArray *hexprefix = ter_syntax_get_hex_prefix(priv->syntax);
	cat_log_detail("hexprefix=%p, priv->test_for_keyword=%d", hexprefix, priv->test_for_keyword);
	if ((hexprefix==NULL) || (!priv->test_for_keyword)) {
		return FALSE;
	}
	cat_log_detail("attempting hex");


	gboolean result = FALSE;
	int cnt = -1;
	while(TRUE) {
		cnt++;
		l_look_ahead(priv);
		if (priv->unichar==-1) {
			break;
		}
		if (cnt<cat_unichar_array_length(hexprefix)) {
			if (cat_unichar_array_char_at(hexprefix, cnt)!=priv->unichar) {
				cat_log_trace("%c not part of prefix", priv->unichar);
				cnt = 0;
				break;
			}
		} else {
			if ((priv->unichar>='0' && priv->unichar<='9')
					|| (priv->unichar>='A' && priv->unichar<='F')
					|| (priv->unichar>='a' && priv->unichar<='f')) {
				continue;
			}
			if (priv->unichar==' ' || priv->unichar=='\t' || priv->unichar<32) {
				cat_log_trace("whitespace detected");
				break;
			}
			CatUnichar *uchobj = cat_unichar_new(priv->unichar);
			if (ter_syntax_is_bracket_char(priv->syntax, uchobj)) {
				cat_unref_ptr(uchobj);
				cat_log_trace("part of bracket : %c", priv->unichar);
				break;
			} else if (ter_syntax_is_operator_char(priv->syntax, uchobj)) {
				cat_log_trace("part of operator-charset : %c", priv->unichar);
				cat_unref_ptr(uchobj);
				break;
			}
			cat_log_trace("bad-char : %c", priv->unichar);
			cat_unref_ptr(uchobj);
			break;
		}
	}

	cat_log_trace("cnt = %d", cnt);
	if (cnt>cat_unichar_array_length(hexprefix)) {
		dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+cnt, TER_COLOR_NUMBERS);
		cat_unichar_array_remove(priv->scan_buffer, 0, cnt);
		priv->column += cnt;
		priv->scan_buffer_offset = 0;
		result = TRUE;
	}

	return result;
}

static gboolean l_attempt_single_line_comment(TerScannerPrivate *priv, CatUnicharArray *comment_start, int comment_col, gunichar comment_escape, int code) {
	if (comment_start==NULL) {
		return FALSE;
	}
	gboolean result = FALSE;

	if (comment_col>0) {
		if (priv->column+1!=comment_col) {
			return FALSE;
		}
	} else if (comment_col<0) {
		if (!priv->leading_only_whitespaces) {
			return FALSE;
		}
	}

	int cnt = -1;
	while(TRUE) {
		cnt++;
		l_look_ahead(priv);
		if (priv->unichar==-1) {
			break;
		}
		if (cnt<cat_unichar_array_length(comment_start)) {
			if (cat_unichar_array_char_at(comment_start, cnt)!=priv->unichar) {
				cat_log_trace("%c not part of comment_start", priv->unichar);
				cnt = 0;
				break;
			}
		} else {
			if (priv->unichar==0 || priv->unichar==0xa || priv->unichar==0xd) {
				cat_log_trace("line-break detected");
				break;
			}
		}
	}

	cat_log_trace("cnt = %d", cnt);
	if (cnt>0) {
		dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+cnt, code);
		cat_unichar_array_remove(priv->scan_buffer, 0, cnt);
		priv->column += cnt;
		priv->scan_buffer_offset = 0;
		result = TRUE;
	}

	return result;
}

static gboolean l_attempt_multi_line_comment(TerScannerPrivate *priv, CatUnicharArray *comment_start, CatUnicharArray *comment_end, int code) {
	if (comment_start==NULL || comment_end==NULL) {
		return FALSE;
	}

	gboolean result = TRUE;
	int cnt = 0;
	while(cnt<cat_unichar_array_length(comment_start)) {
		l_look_ahead(priv);
		if (priv->unichar==-1) {
			result = FALSE;
			break;
		}
		if (cnt<cat_unichar_array_length(comment_start)) {
			if (cat_unichar_array_char_at(comment_start, cnt)!=priv->unichar) {
				cat_log_trace("%c not part of comment_start", priv->unichar);
				result = FALSE;
				break;
			}
		}
		cnt++;
	}

	if (result) {

		cat_unichar_array_set_length(priv->scan_buffer,0);
		priv->scan_buffer_offset = 0;

		while(TRUE) {
			l_look_ahead(priv);
			if (priv->unichar==-1) {
				if (cnt>0) {
					dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+cnt, code);
					cat_unichar_array_set_length(priv->scan_buffer,0);
					priv->scan_buffer_offset = 0;
					priv->column += cnt;
				}
				break;
			} else if (priv->unichar==0xa || priv->unichar==0xd || priv->unichar==0x0) {
				if (cnt>0) {
					dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+cnt, code);
					cat_unichar_array_set_length(priv->scan_buffer,0);
					priv->scan_buffer_offset = 0;
					priv->column = 0;
					priv->row++;
					cnt = 0;
				}
			} else {
				if (cat_unichar_array_length(priv->scan_buffer)>cat_unichar_array_length(comment_end)) {
					cat_unichar_array_remove(priv->scan_buffer,0, 1);
					priv->scan_buffer_offset--;
				}

				if (cat_unichar_array_equal(priv->scan_buffer, comment_end)) {
					if (cnt>0) {
						dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+cnt, code);
						cat_unichar_array_set_length(priv->scan_buffer,0);
						priv->scan_buffer_offset = 0;
						priv->column += cnt;
					}
					break;
				}
			}
			cnt++;
		}
	}
	return result;
}

gboolean l_attempt_string(TerScannerPrivate *priv, gunichar string_start, gunichar string_end, gunichar string_escape, int code) {
	if (string_start==0 || string_end==0) {
		return FALSE;
	}

	l_look_ahead(priv);
	if (priv->unichar!=string_start) {
		return FALSE;
	}

	int cnt = 1;
	while(TRUE) {
		l_look_ahead(priv);
		cnt++;
		gboolean is_return = priv->unichar==0xa || priv->unichar==0xD;
		gboolean is_end = priv->unichar==string_end || priv->unichar==-1;
		if (is_return || is_end) {
			if (is_return) {
				cnt--;
			}
			dra_keyword_printer_print_fg_color(priv->token_printer, priv->row, priv->column, priv->row, priv->column+cnt, code);
			cat_unichar_array_remove(priv->scan_buffer, 0, cnt);
			priv->column += cnt;
			priv->scan_buffer_offset = 0;
			break;
		}
	}

	return TRUE;
}

void ter_scanner_run(TerScanner *lexer) {
	TerScannerPrivate *priv = ter_scanner_get_instance_private(lexer);
	priv->next_test_for_keyword = TRUE;
	priv->test_for_keyword = TRUE;
	priv->leading_only_whitespaces = TRUE;
	gboolean keep_running = TRUE;

	while(keep_running) {

		priv->test_for_keyword = priv->next_test_for_keyword;
		priv->next_test_for_keyword = FALSE;
		cat_log_trace("test_for_keyword=%d", priv->test_for_keyword);
		if (l_attempt_keyword(priv)) {
			continue;
		} else {
			l_look_ahead_reset(priv);
		}

		if (l_attempt_single_line_comment(priv, ter_syntax_get_single_comment(priv->syntax), ter_syntax_get_single_comment_column(priv->syntax), 0, TER_COLOR_COMMENTS)) {
			continue;
		} else {
			l_look_ahead_reset(priv);
		}

		if (l_attempt_single_line_comment(priv, ter_syntax_get_single_comment_alt(priv->syntax), ter_syntax_get_single_comment_column_alt(priv->syntax), 0, TER_COLOR_COMMENTS)) {
			continue;
		} else {
			l_look_ahead_reset(priv);
		}

		if (l_attempt_multi_line_comment(priv, ter_syntax_get_multi_line_comment_start(priv->syntax), ter_syntax_get_multi_line_comment_end(priv->syntax), TER_COLOR_COMMENTS)) {
			continue;
		} else {
			l_look_ahead_reset(priv);
		}


		if (l_attempt_string(priv, ter_syntax_get_string_start(priv->syntax), ter_syntax_get_string_end(priv->syntax), 0, TER_COLOR_STRINGS)) {
			continue;
		} else {
			l_look_ahead_reset(priv);
		}


		if (l_attempt_unichar_set(priv, ter_syntax_get_bracket_set(priv->syntax), TER_COLOR_BRACKET_CHARS)) {
			priv->next_test_for_keyword = TRUE;
			continue;
		} else {
			l_look_ahead_reset(priv);
		}

		if (l_attempt_unichar_set(priv, ter_syntax_get_operator_char_set(priv->syntax), TER_COLOR_OPERATOR_CHARS)) {
			priv->next_test_for_keyword = TRUE;
			continue;
		} else {
			l_look_ahead_reset(priv);
		}


		if (l_attempt_hex_number(priv)) {
			continue;
		} else {
			l_look_ahead_reset(priv);
		}

		l_look_ahead_and_shift(priv);
		priv->column++;

		switch(priv->unichar) {
			case -1 : {
				keep_running = FALSE;
			} break;
			case 0xa :
			case 0xd :
			case 0 : {
				priv->row++;
				priv->column = 0;
				priv->next_test_for_keyword = TRUE;
				priv->leading_only_whitespaces = TRUE;
			} break;
			case ' ' :
			case '\t' : {
				priv->next_test_for_keyword = TRUE;
			} break;
			default :  {
				CatUnichar *uchobj = cat_unichar_new(priv->unichar);
				priv->next_test_for_keyword = !ter_syntax_is_keyword_char(priv->syntax, uchobj);
				cat_unref_ptr(uchobj);
				priv->leading_only_whitespaces = FALSE;
			} break;
		}
	}
}
