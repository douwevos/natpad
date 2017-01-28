/*
   File:    draspellhelper.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 27, 2016
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


// TODO aspell objects are not cleaned

#include "draspellhelper.h"
#include "draspellchecker.h"
#include <aspell.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraSpellHelper"
#include <logging/catlog.h>

struct _DraSpellHelperPrivate {
	DraPreferencesWo *a_prefs;
	struct AspellSpeller *speller;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraSpellHelper, dra_spell_helper, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraSpellHelper)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_spell_helper_class_init(DraSpellHelperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_spell_helper_init(DraSpellHelper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraSpellHelper *instance = DRA_SPELL_HELPER(object);
	DraSpellHelperPrivate *priv = dra_spell_helper_get_instance_private(instance);
	cat_unref_ptr(priv->a_prefs);
	G_OBJECT_CLASS(dra_spell_helper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_spell_helper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraSpellHelper *dra_spell_helper_create(DraPreferencesWo *a_prefs) {
	DraSpellHelper *result = g_object_new(DRA_TYPE_SPELL_HELPER, NULL);
	cat_ref_anounce(result);
	DraSpellHelperPrivate *priv = dra_spell_helper_get_instance_private(result);
	priv->a_prefs = cat_ref_ptr(a_prefs);

	gboolean is_enabled = FALSE;
	CatStringWo *lang = NULL;

	if (a_prefs) {
		DraPrefsSpellingWo *prefs_spelling = dra_preferences_wo_get_spelling(a_prefs);
		is_enabled = dra_prefs_spelling_wo_is_enabled(prefs_spelling);
		lang = dra_prefs_spelling_wo_get_dictionary_name(prefs_spelling);
	}
	if (!is_enabled) {
		priv->speller = NULL;
		return result;
	}


	struct AspellConfig *aspell_config = new_aspell_config();

	AspellDictInfoList *adil = get_aspell_dict_info_list(aspell_config);
	AspellDictInfoEnumeration *adile = aspell_dict_info_list_elements(adil);
	while(TRUE) {
		AspellDictInfo *adi = aspell_dict_info_enumeration_next(adile);
		if (adi==NULL) {
			break;
		}
		cat_log_detail("nam=%s, jargo=%s, code=%s", adi->name, adi->jargon, adi->code);
	}

	if (lang) {
		aspell_config_replace(aspell_config, "lang", cat_string_wo_getchars(lang));
	}

	struct AspellCanHaveError *aspell_che = new_aspell_speller(aspell_config);

	priv->speller = to_aspell_speller(aspell_che);

	return result;
}


DraSpellHelper *dra_spell_helper_new() {
	return dra_spell_helper_create(NULL);
}

DraSpellHelper *dra_spell_helper_reconfigure(DraSpellHelper *spell_helper, DraPreferencesWo *a_prefs) {
	DraSpellHelperPrivate *priv = dra_spell_helper_get_instance_private(spell_helper);
	if (priv->a_prefs==a_prefs) {
		return cat_ref_ptr(spell_helper);
	}
	return dra_spell_helper_create(a_prefs);
}


void dra_spell_helper_scan(DraSpellHelper *spell_helper, DraKeywordPrinter *line_tag_printer, CatStringWo *text, int column_start, long long row_start) {
	DraSpellHelperPrivate *priv = dra_spell_helper_get_instance_private(spell_helper);
	if (priv->speller==NULL) {
		return;
	}
	CatStringInputStream *csis = cat_string_input_stream_new(text);
	CatUtf8InputStreamScanner *scanner = cat_utf8_input_stream_scanner_new((CatIInputStream *) csis);
//	cat_unref_ptr(csis);
	DraSpellChecker *checker = dra_spell_checker_new_at(scanner, column_start, row_start);

	while(TRUE) {
		const DraSpellWord spell_word = dra_spell_checker_next_word(checker);
		if (spell_word.word==NULL) {
			break;
		}
		CatStringWo *buf = (CatStringWo *) spell_word.word;
		int res = aspell_speller_check(priv->speller, cat_string_wo_getchars(buf), cat_string_wo_length(buf));
//		cat_log_debug("checked:%O --- %d", buf, res);
		if (res==0) {
			DraLineTagWo *tag = dra_line_tag_wo_new(spell_word.row, DRA_TAG_TYPE_SPELL_ERROR);
			dra_line_tag_wo_set_color(tag, 0.6, 0.6, 0.8);
			dra_line_tag_wo_set_start_and_end_index(tag, spell_word.column_start, spell_word.column_end);
			CatStringWo *word = cat_string_wo_clone(buf, CAT_CLONE_DEPTH_FULL);
			dra_line_tag_wo_set_extra_data(tag, word);
			cat_unref_ptr(word);
			dra_keyword_printer_print_line_tag(line_tag_printer, tag);
		}
	}
}


CatArrayWo *dra_spell_helper_enlist_corrections(DraSpellHelper *spell_helper, CatStringWo *word) {
	CatArrayWo *result = NULL;
	DraSpellHelperPrivate *priv = dra_spell_helper_get_instance_private(spell_helper);
	if (priv->speller) {
		int max = 5;
		if (priv->a_prefs) {
			DraPrefsSpellingWo *prefs_spelling = dra_preferences_wo_get_spelling(priv->a_prefs);
			max = dra_prefs_spelling_wo_get_max_suggestions(prefs_spelling);
		}
		const struct AspellWordList *suggestions = aspell_speller_suggest(priv->speller, cat_string_wo_getchars(word), cat_string_wo_length(word));
		if (suggestions) {

			result = cat_array_wo_new();
			struct AspellStringEnumeration *asenum = aspell_word_list_elements(suggestions);
			while(max>0) {
				const char *sug_text = aspell_string_enumeration_next(asenum);

				if (sug_text==NULL) {
					break;
				}
				CatStringWo *sg = cat_string_wo_new_with(sug_text);
				cat_array_wo_append(result, sg);
				cat_unref_ptr(sg);
				max--;
			}
		}
	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraSpellHelper *instance = DRA_SPELL_HELPER(self);
	DraSpellHelperPrivate *priv = dra_spell_helper_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
