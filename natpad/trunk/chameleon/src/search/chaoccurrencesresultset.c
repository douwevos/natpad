/*
   File:    chaoccurrencesresultset.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Nov 5, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chaoccurrencesresultset.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaOccurrencesResultSet"
#include <logging/catlog.h>

struct _ChaOccurrencesResultSetPrivate {
	CatArrayWo *pages;
	GRegex *regexp;
	ChaSearchQueryWo *query;
	ChaDocument *document;
	ChaRevisionWo *a_revision;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaOccurrencesResultSet, cha_occurrences_result_set, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaOccurrencesResultSet)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_occurrences_result_set_class_init(ChaOccurrencesResultSetClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_occurrences_result_set_init(ChaOccurrencesResultSet *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaOccurrencesResultSet *instance = CHA_OCCURRENCES_RESULT_SET(object);
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(instance);
	cat_unref_ptr(priv->a_revision);
	cat_unref_ptr(priv->document);
	cat_unref_ptr(priv->pages);
	cat_unref_ptr(priv->query);
	cat_unref_ptr(priv->regexp);
	G_OBJECT_CLASS(cha_occurrences_result_set_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_occurrences_result_set_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaOccurrencesResultSet *cha_occurrences_result_set_new(ChaSearchQueryWo *query, ChaDocument *document) {
	ChaOccurrencesResultSet *result = g_object_new(CHA_TYPE_OCCURRENCES_RESULT_SET, NULL);
	cat_ref_anounce(result);
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(result);
	CatStringWo *search_text = cha_search_query_wo_get_text(query);
	priv->pages = cat_array_wo_new();
	priv->document = cat_ref_ptr(document);
	if (document) {
		priv->a_revision = cha_document_get_current_revision_ref(priv->document);
	} else {
		priv->a_revision = NULL;
	}
	priv->query = cha_search_query_wo_clone(query, CAT_CLONE_DEPTH_NONE);
	priv->regexp = NULL;
	gboolean match_case = cha_search_query_wo_get_match_case(query);
	if (cha_search_query_wo_get_regexp(query)) {
		GRegexCompileFlags cflgs = G_REGEX_OPTIMIZE;

		if (!match_case) {
			cflgs |= G_REGEX_CASELESS|G_REGEX_UNGREEDY;
		}

		GError *error = NULL;

		priv->regexp = g_regex_new(cat_string_wo_getchars(search_text), cflgs, G_REGEX_MATCH_NOTEMPTY, &error);

	}
	return result;
}

void cha_occurences_result_set_set_revision(ChaOccurrencesResultSet *result_set, ChaRevisionWo *revision) {
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(result_set);
	cat_ref_swap(priv->a_revision, revision);
}



static ChaSearchPageOccurrences *l_get_page_occurrences(ChaOccurrencesResultSet *result_set, int page_index) {
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(result_set);
	ChaSearchPageOccurrences *page_occurrences = (ChaSearchPageOccurrences *) cat_array_wo_get(priv->pages, page_index);

	ChaPageWo *a_page = cha_revision_wo_page_at(priv->a_revision, page_index);
	if (page_occurrences!=NULL) {
		ChaPageWo *p = cha_search_page_occurrences_get_page(page_occurrences);
		if (a_page != p) {
			page_occurrences = NULL;
		}
	}

	if (page_occurrences==NULL) {
		page_occurrences = cha_search_query_wo_run_for_page(result_set, a_page, page_index);
		cat_array_wo_pad(priv->pages, page_index+1, NULL);
		cat_array_wo_set(priv->pages, (GObject *) page_occurrences, page_index, NULL);
		cat_unref(page_occurrences);
	}
	return page_occurrences;
}

ChaSearchPageOccurrences *cha_occurences_result_set_get_page_occurrences(ChaOccurrencesResultSet *result_set, int page_index) {
	return l_get_page_occurrences(result_set, page_index);
}


ChaSearchOccurrence *cha_occurrences_result_set_find_backward(ChaOccurrencesResultSet *result_set, ChaCursorWo *start_cursor, ChaCursorWo *stop_cursor, int *opg_idx) {
	ChaSearchOccurrence *result = NULL;
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(start_cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	int column = cha_cursor_wo_get_x_cursor_bytes(start_cursor);

	int stop_column = -1;
	int stop_page_line_index = -1;

	cat_log_debug("page_index:%d, page_line_index=%d, column=%d", page_index, page_line_index, column);
	while(result == NULL) {
		ChaSearchPageOccurrences *page_occurrences = l_get_page_occurrences(result_set, page_index);
		int occ_count = cha_search_page_occurrences_count(page_occurrences);
		cat_log_debug("occ_count:%d", occ_count);
		if (occ_count!=0) {

			if (stop_cursor) {
				ChaLineLocationWo *stop_line_location = cha_cursor_wo_get_line_location(stop_cursor);
				int epi = cha_line_location_wo_get_page_index(stop_line_location);
				if (page_index<epi) {
					return NULL;
				} else if (page_index==epi) {
					stop_column = cha_cursor_wo_get_x_cursor_bytes(stop_cursor);
					stop_page_line_index = cha_line_location_wo_get_page_line_index(stop_line_location);
				}
			}

			int occ_line_idx;
			if (page_line_index==-1) {
				page_line_index = occ_count;
			}
			for(occ_line_idx=page_line_index; occ_line_idx>=0; occ_line_idx--) {
				if (occ_line_idx<stop_page_line_index) {
					return NULL;
				}
				CatArrayWo *a_occurrences = cha_search_page_occurrences_enlist_for_line(page_occurrences, occ_line_idx);
				if (a_occurrences) {
					int count = cat_array_wo_size(a_occurrences);
					int occ_idx;
					for(occ_idx=count-1; occ_idx>=0; occ_idx--) {
						ChaSearchOccurrence *occurence = (ChaSearchOccurrence *) cat_array_wo_get(a_occurrences, occ_idx);
						if (stop_page_line_index==occ_line_idx && cha_search_occurrence_is_before(occurence, stop_page_line_index, stop_column)) {
							return NULL;
						}
						cat_log_debug("testing occurence:%o", occurence);
						if (cha_search_occurrence_is_before(occurence, page_line_index, column)) {
							cat_log_debug("found occurence:%o", occurence);
							result = occurence;
							*opg_idx = page_index;
							return result;
						}
					}
				}
			}
		}
		page_line_index = -1;
		page_index--;
		if (page_index<0) {
			break;
		}
	}
	*opg_idx = page_index;
	return result;
}

ChaSearchOccurrence *cha_occurrences_result_set_find_forward(ChaOccurrencesResultSet *result_set, ChaCursorWo *cursor, ChaCursorWo *stop_cursor, int *opg_idx) {
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(result_set);
	ChaSearchOccurrence *result = NULL;
	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(cursor);
	int page_index = cha_line_location_wo_get_page_index(line_location);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	int column = cha_cursor_wo_get_x_cursor_bytes(cursor);

	int stop_column = -1;
	int stop_page_line_index = -1;

	cat_log_debug("page_index:%d, page_line_index=%d, column=%d", page_index, page_line_index, column);
	while(result == NULL) {
		ChaSearchPageOccurrences *page_occurrences = l_get_page_occurrences(result_set, page_index);
		int occ_count = cha_search_page_occurrences_count(page_occurrences);
		cat_log_debug("page_occurrences=%p, occ_count:%d", page_occurrences, occ_count);
		if (occ_count!=0) {

			if (stop_cursor) {
				ChaLineLocationWo *stop_line_location = cha_cursor_wo_get_line_location(stop_cursor);
				int epi = cha_line_location_wo_get_page_index(stop_line_location);
				if (page_index>epi) {
					return NULL;
				} else if (page_index==epi) {
					stop_column = cha_cursor_wo_get_x_cursor_bytes(stop_cursor);
					stop_page_line_index = cha_line_location_wo_get_page_line_index(stop_line_location);
				}
			}

			int occ_line_idx;
			for(occ_line_idx=page_line_index; occ_line_idx<occ_count; occ_line_idx++) {
				if (stop_page_line_index!=-1 && occ_line_idx>stop_page_line_index) {
					return NULL;
				}
				CatArrayWo *a_occurrences = cha_search_page_occurrences_enlist_for_line(page_occurrences, occ_line_idx);
				if (a_occurrences) {
					int count = cat_array_wo_size(a_occurrences);
					int occ_idx;
					for(occ_idx=0; occ_idx<count; occ_idx++) {
						ChaSearchOccurrence *occurence = (ChaSearchOccurrence *) cat_array_wo_get(a_occurrences, occ_idx);
						if (stop_page_line_index==occ_line_idx && cha_search_occurrence_is_after(occurence, stop_page_line_index, stop_column)) {
							return NULL;
						}
						cat_log_debug("testing occurence:%o", occurence);
						if (cha_search_occurrence_is_after(occurence, page_line_index, column)) {
							cat_log_debug("found occurence:%o", occurence);
							result = occurence;
							*opg_idx = page_index;
							return result;
						}
					}
				}
			}
		}
		page_line_index = 0;
		column = 0;
		page_index++;
		cat_log_debug("page_index:%d, pages.size=%d, result=%p", page_index, cat_array_wo_size(priv->pages), result);
		if (page_index>=cha_revision_wo_page_count(priv->a_revision)) {
			break;
		}
	}
	*opg_idx = page_index;
	return result;
}




static ChaSearchPageOccurrences *l_search_regexp_on_page(ChaPageWo *page, int page_index, GRegex *regex, gboolean match_case) {

	ChaSearchPageOccurrences *occurences = cha_search_page_occurrences_new(page, page_index);

	ChaPageWoClass *pg_class = CHA_PAGE_WO_GET_CLASS(page);
	cha_page_wo_hold_lines(page);
	int line_count = pg_class->lineCount(page);
	int line_idx;
	for(line_idx=0; line_idx<line_count; line_idx++) {
		const ChaUtf8Text utf8_txt = pg_class->utf8At(page, line_idx, FALSE);
		cat_log_detail("txt_length=%d", txt_length);
		if (utf8_txt.text_len<=0) {
			cha_utf8_text_cleanup(&utf8_txt);
			continue;
		}

		int col = 0;
		while(TRUE) {
			GMatchInfo *match_info = NULL;
			if (g_regex_match_full(regex, utf8_txt.text,utf8_txt.text_len, col, 0, &match_info, NULL)) {
				int start_pos, end_pos;
				g_match_info_fetch_pos(match_info, 0, &start_pos, &end_pos);
				cat_log_debug("start_pos=%d,end_pos=%d", start_pos, end_pos);
				ChaSearchOccurrence *occurrence = cha_search_occurrence_new(line_idx, start_pos, end_pos, 0);
				cha_search_page_occurrences_add(occurences, occurrence);
				cat_log_debug("occurences=%p, occurrence=%o",occurences, occurrence);
				cat_unref_ptr(occurrence);
				if (end_pos<utf8_txt.text_len) {
					col = end_pos;
				} else {
					break;
				}
			} else {
				break;
			}
		}
		cha_utf8_text_cleanup(&utf8_txt);
	}
	cha_page_wo_release_lines(page);
	return occurences;

}


static ChaSearchPageOccurrences *l_search_on_page(ChaPageWo *page, int page_index, const CatStringWo *search_text, gboolean match_case, gboolean match_words) {

	ChaSearchPageOccurrences *occurences = cha_search_page_occurrences_new(page, page_index);

	int st_len = cat_string_wo_length(search_text);

	char first_ch = cat_string_wo_char_at(search_text, 0);
	char second_ch = 0;
	char *search_cf = NULL;

	if (!match_case) {
		search_cf = g_utf8_casefold(cat_string_wo_getchars(search_text), st_len);
		const char *st = cat_string_wo_getchars(search_text);
		const char *en = st + cat_string_wo_length(search_text);
		char *nc = g_utf8_find_next_char(st, en);
		if (nc) {
			glong wcnt = 0;
			gunichar *p = g_utf8_to_ucs4_fast(st, nc-st, &wcnt);
			if (p) {
				gunichar lch = g_unichar_tolower(*p);
				gunichar uch = g_unichar_toupper(*p);
				g_free(p);
				if (lch!=uch) {
					char u8[10];
					g_unichar_to_utf8(lch, u8);
					first_ch = u8[0];
					g_unichar_to_utf8(uch, u8);
					second_ch = u8[0];
					if (first_ch==second_ch) {
						second_ch = 0;
					}
				}
			} else {
				cat_log_warn("No conversion from utf8 to ucs4 possible");
			}
		} else {
			cat_log_warn("No next char found: search_text=%O", search_text);
		}
	}



	ChaPageWoClass *pg_class = CHA_PAGE_WO_GET_CLASS(page);
	cha_page_wo_hold_lines(page);
	int line_count = pg_class->lineCount(page);
	int line_idx;
	for(line_idx=0; line_idx<line_count; line_idx++) {
		const ChaUtf8Text utf8_txt = pg_class->utf8At(page, line_idx, FALSE);
		int txt_length = utf8_txt.text_len;
		const char *txt = utf8_txt.text;
		cat_log_detail("txt_length=%d, st_len=%d", txt_length, st_len);
		if (txt_length<st_len) {
			cha_utf8_text_cleanup(&utf8_txt);
			continue;
		}

		int last_chr_idx = txt_length+1-st_len;

		int col = 0;
		char *f_fpos = NULL;
		char *s_fpos = NULL;
		if (second_ch!=0) {
			f_fpos = (char *) memchr(txt, first_ch, last_chr_idx);
			s_fpos = (char *) memchr(txt, second_ch, last_chr_idx);
			cat_log_detail("f_fpos=%p, s_fpos=%p, fch=%d, sch=%d", f_fpos, s_fpos, first_ch, second_ch);
		}
		while(col<(last_chr_idx)) {
			char *np = NULL;
			if (second_ch==0) {
				np = (char *) memchr(txt+col, first_ch, last_chr_idx-col);
			} else {

				if (f_fpos!=NULL) {
					if (s_fpos!=NULL && s_fpos<f_fpos) {
						np = s_fpos;
						col=(int) (np-txt)+1;
						s_fpos = (char *) memchr(txt+col, second_ch, last_chr_idx-col);
					} else {
						np = f_fpos;
						col=(int) (np-txt)+1;
						f_fpos = (char *) memchr(txt+col, first_ch, last_chr_idx-col);
					}
				} else if (s_fpos!=NULL) {
					np = s_fpos;
					col=(int) (np-txt)+1;
					s_fpos = (char *) memchr(txt+col, second_ch, last_chr_idx-col);
				} else {
					break;
				}

			}
			cat_log_detail("np=%p", np);
			if (np==NULL) {
				break;
			}
			col=(int) (np-txt);
			gboolean did_match = FALSE;
			if (match_case) {
				/* simple memory comparison */
				if (memcmp(np, cat_string_wo_getchars(search_text), st_len)==0) {
					did_match = TRUE;
				}
			} else {
				/* case sensitive UTF8 comparison */
				char *test_cf = g_utf8_casefold(np, st_len);
				cat_log_debug("test_cf=%s, search_cf=%s", test_cf, search_cf);
				if (g_utf8_collate(test_cf, search_cf)==0) {
					did_match = TRUE;
				}
				cat_free_ptr(test_cf);
			}
			if (did_match) {
				if (match_words) {
					const char *n = txt+(col+st_len);
					gunichar fuch = g_utf8_get_char_validated(n, txt+txt_length-n);
					if (g_unichar_isalpha(fuch) || g_unichar_isdigit(fuch) || fuch=='_') {
						did_match = FALSE;
					} else {
						if (np>txt) {
							n = g_utf8_find_prev_char(txt, np);
							if (n) {
								gunichar fuch = g_utf8_get_char_validated(n, txt+txt_length-n);
								if (g_unichar_isalpha(fuch) || g_unichar_isdigit(fuch) || fuch=='_') {
									did_match = FALSE;
								}
							}
						}
					}
				}
				if (did_match) {
					ChaSearchOccurrence *occurrence = cha_search_occurrence_new(line_idx, col, col+st_len, 0);
					cha_search_page_occurrences_add(occurences, occurrence);
					cat_log_debug("occurrence=%o",occurrence);
					cat_unref_ptr(occurrence);
				}

			}

			col++;
		}
		cha_utf8_text_cleanup(&utf8_txt);
	}
	cat_free_ptr(search_cf);

	cha_page_wo_release_lines(page);
	return occurences;
}

ChaSearchPageOccurrences *cha_search_query_wo_run_for_page(ChaOccurrencesResultSet *result_set, ChaPageWo *page, int page_index) {
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(result_set);
	ChaSearchPageOccurrences *result = NULL;
	gboolean match_case = cha_search_query_wo_get_match_case(priv->query);
	gboolean match_words = cha_search_query_wo_get_match_words(priv->query);
	if (priv->regexp) {
		result = l_search_regexp_on_page(page, page_index, priv->regexp, match_case);
	} else {
		CatStringWo *search_text = cha_search_query_wo_get_text(priv->query);
		result = l_search_on_page(page, page_index, search_text, match_case, match_words);
	}
	return result;
}


CatArrayWo *cha_occurrences_result_set_enlist_for_line(ChaOccurrencesResultSet *result_set, int page_index, int page_line_index) {
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(result_set);
	if (priv->a_revision==NULL) {
		return NULL;
	}
	ChaSearchPageOccurrences *page_occurrences = l_get_page_occurrences(result_set, page_index);
	CatArrayWo *result = NULL;

	if (page_occurrences) {
		result = cha_search_page_occurrences_enlist_for_line(page_occurrences, page_line_index);
	}
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaOccurrencesResultSet *instance = CHA_OCCURRENCES_RESULT_SET(self);
	ChaOccurrencesResultSetPrivate *priv = cha_occurrences_result_set_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p pages=%p]", iname, self, priv->pages);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
