/*
   File:    terprefsmapper.c
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 26, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "terprefsmapper.h"
#include "terpreferenceswo.h"
#include "terprefsclazzwo.h"
#include <elk.h>
#include <shoveler.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerPrefsMapper"
#include <logging/catlog.h>


CatS ter_s_textpad = CAT_S_DEF("textpad");


static CatS ter_s_clazz = CAT_S_DEF("clazz");

static CatS ter_s_grammar_file = CAT_S_DEF("grammer-file");
static CatS ter_s_extra_characters = CAT_S_DEF("extra-characters");
static CatS ter_s_patterns = CAT_S_DEF("patterns");

struct _TerPrefsMapperPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_preferences_mapper_iface_init(ElkIPreferencesMapperInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerPrefsMapper, ter_prefs_mapper, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TerPrefsMapper)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IPREFERENCES_MAPPER, l_preferences_mapper_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_prefs_mapper_class_init(TerPrefsMapperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_prefs_mapper_init(TerPrefsMapper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	TerPrefsMapper *instance = TER_PREFS_MAPPER(object);
//	TerPrefsMapperPrivate *priv = ter_prefs_mapper_get_instance_private(instance);
	G_OBJECT_CLASS(ter_prefs_mapper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_prefs_mapper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerPrefsMapper *ter_prefs_mapper_new() {
	TerPrefsMapper *result = g_object_new(TER_TYPE_PREFS_MAPPER, NULL);
	cat_ref_anounce(result);
//	TerPrefsMapperPrivate *priv = ter_prefs_mapper_get_instance_private(result);
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
//	TerPrefsMapper *instance = TER_PREFS_MAPPER(self);
//	TerPrefsMapperPrivate *priv = ter_prefs_mapper_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



/********************* start ElkIPreferencesMapper implementation *********************/

static void l_model_to_prefs(ElkIPreferencesMapper *self, ShoModel *model, ElkPreferencesWo *e_prefs) {
	TerPreferencesWo *e_ter_prefs = (TerPreferencesWo *) elk_preferences_wo_get_sub(e_prefs, CAT_S(ter_s_textpad));
	if (e_ter_prefs==NULL) {
		e_ter_prefs = ter_preferences_wo_new();
		elk_preferences_wo_set_sub(e_prefs, CAT_S(ter_s_textpad), (GObject *) e_ter_prefs);
		cat_unref(e_ter_prefs);
		cat_log_detail("e_ter_prefs=%o", e_ter_prefs);
	} else if (ter_preferences_wo_is_anchored(e_ter_prefs)) {
		e_ter_prefs = ter_preferences_wo_create_editable(e_ter_prefs);
		elk_preferences_wo_set_sub(e_prefs, CAT_S(ter_s_textpad), (GObject *) e_ter_prefs);
		cat_unref(e_ter_prefs);
	}

	ShoReference *ref_textpad = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) model, CAT_S(ter_s_textpad));
	if (ref_textpad) {
		ShoIValue *val_textpad = sho_reference_get_value(ref_textpad);
		cat_log_debug("val_textpad=%o", val_textpad);
		if (SHO_IS_REFERENCE_LIST(val_textpad)) {

			CatIIterator *t_iter = sho_reference_list_iterator((ShoReferenceList *) val_textpad);
			while(cat_iiterator_has_next(t_iter)) {
				GObject *t_entry = cat_iiterator_next(t_iter);
				if (SHO_IS_REFERENCE(t_entry)) {
					CatStringWo *name_of_entry = sho_reference_get_id((ShoReference *) t_entry);
					if (!cat_string_wo_equal(name_of_entry, CAT_S(ter_s_clazz))) {
						continue;
					}

					ShoIValue *val_entry = sho_reference_get_value((ShoReference *) t_entry);
					if (!SHO_IS_BLOCK_VALUE(val_entry)) {
						continue;
					}

					CatStringWo *name = sho_reference_list_get_string((ShoReferenceList *) val_entry, CAT_S(sho_s_name), NULL);
					if (name == NULL) {
						continue;
					}
					TerPrefsClazzWo *e_prefs_clazz = ter_prefs_clazz_wo_new(name);
					ter_preferences_wo_set_clazz(e_ter_prefs, e_prefs_clazz);
					cat_unref(e_prefs_clazz);

					CatStringWo *grammar_file = sho_reference_list_get_string((ShoReferenceList *) val_entry, CAT_S(ter_s_grammar_file), NULL);
					if (grammar_file != NULL) {
						ter_prefs_clazz_wo_set_grammar_file(e_prefs_clazz, grammar_file);
					}
					CatStringWo *extra_characters = sho_reference_list_get_string((ShoReferenceList *) val_entry, CAT_S(ter_s_extra_characters), NULL);
					if (extra_characters != NULL) {
						ter_prefs_clazz_wo_set_extra_characters(e_prefs_clazz, extra_characters);
					}

					ShoReference *pattern_ref = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) val_entry, CAT_S(ter_s_patterns));
					if (pattern_ref) {
						ShoIValue *pattern_val = sho_reference_get_value(pattern_ref);
						if (SHO_IS_LIST_VALUE(pattern_val)) {

							CatIIterator *pattern_iter = sho_list_value_iterator((ShoListValue *) pattern_val);
							while(cat_iiterator_has_next(pattern_iter)) {
								ShoIValue *p_e_val = (ShoIValue *) cat_iiterator_next(pattern_iter);
								if (SHO_IS_SIMPLE_VALUE(p_e_val)) {
									CatStringWo *text_ref = sho_simple_value_get_string((ShoSimpleValue *) p_e_val);
									ter_prefs_clazz_wo_add_pattern(e_prefs_clazz, text_ref);
								} else {
									cat_log_error("Incorrect pattern value, expected String: val=%o", p_e_val);
								}
							}
							cat_unref_ptr(pattern_iter);
						}
					}

				}
			}
			cat_unref_ptr(t_iter);
		}
	}
}

static void l_prefs_to_model(ElkIPreferencesMapper *self, ElkPreferencesWo *prefs, ShoModel *model) {
	TerPreferencesWo *ter_prefs = (TerPreferencesWo *) elk_preferences_wo_get_sub(prefs, CAT_S(ter_s_textpad));
	if (ter_prefs) {
		ShoReference *ref_textpad = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) model, CAT_S(ter_s_textpad));
		if (ref_textpad == NULL) {
			ref_textpad = sho_reference_new(CAT_S(ter_s_textpad));
			sho_reference_list_add_reference((ShoReferenceList *) model, ref_textpad);
			cat_unref(ref_textpad);
		}

		ShoIValue *val_textpad = sho_reference_get_value(ref_textpad);
		if (val_textpad == NULL) {
			val_textpad = (ShoIValue *) sho_block_value_new();
			sho_reference_set_value(ref_textpad, val_textpad);
			cat_unref(val_textpad);
		}


		if (SHO_IS_REFERENCE_LIST(val_textpad)) {
			CatHashMapWo *clazz_map = ter_preferences_wo_get_document_clazzes(ter_prefs);
			CatArrayWo *values = cat_hash_map_wo_enlist_values(clazz_map, NULL);
			CatIIterator *clazz_iter = cat_array_wo_iterator(values);
			while(cat_iiterator_has_next(clazz_iter)) {
				TerPrefsClazzWo *clazz_wo = (TerPrefsClazzWo *) cat_iiterator_next(clazz_iter);

				ShoReference *clazz_ref = sho_reference_new(CAT_S(ter_s_clazz));
				sho_reference_list_add_reference((ShoReferenceList *) val_textpad, clazz_ref);
				ShoBlockValue *clazz_val = sho_block_value_new();
				sho_reference_set_value(clazz_ref, (ShoIValue *) clazz_val);
				cat_unref(clazz_ref);

				CatStringWo *clazz_name = ter_prefs_clazz_wo_get_name(clazz_wo);
				sho_reference_list_set_string((ShoReferenceList *) clazz_val, CAT_S(sho_s_name), clazz_name);

				CatStringWo *clazz_grammar_file = ter_prefs_clazz_wo_get_grammar_file(clazz_wo);
				sho_reference_list_set_string((ShoReferenceList *) clazz_val, CAT_S(ter_s_grammar_file), clazz_grammar_file);

				CatStringWo *clazz_extra_characters = ter_prefs_clazz_wo_get_extra_characters(clazz_wo);
				sho_reference_list_set_string((ShoReferenceList *) clazz_val, CAT_S(ter_s_extra_characters), clazz_extra_characters);

				CatArrayWo *pattern_list = ter_prefs_clazz_wo_get_pattern_list(clazz_wo);
				if (pattern_list) {
					ShoReference *ref_patterns = sho_reference_new(CAT_S(ter_s_patterns));
					sho_reference_list_add_reference((ShoReferenceList *) clazz_val, ref_patterns);

					ShoListValue *list_patterns = sho_list_value_new();
					sho_reference_set_value(ref_patterns, (ShoIValue *) list_patterns);

					CatIIterator *pattern_iter = cat_array_wo_iterator(pattern_list);
					while(cat_iiterator_has_next(pattern_iter)) {
						CatStringWo *pattern = (CatStringWo *) cat_iiterator_next(pattern_iter);
						sho_list_value_add_string(list_patterns, pattern);
					}
					cat_unref_ptr(pattern_iter);
					cat_unref_ptr(ref_patterns);
					cat_unref_ptr(list_patterns);
				}

				cat_unref_ptr(clazz_val);
			}
			cat_unref_ptr(clazz_iter);
			cat_unref_ptr(values);
		}
	}
}

static void l_preferences_mapper_iface_init(ElkIPreferencesMapperInterface *iface) {
	iface->modelToPrefs = l_model_to_prefs;
	iface->prefsToModel = l_prefs_to_model;
}

/********************* end ElkIPreferencesMapper implementation *********************/
