/*
   File:    draprefsmapper.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 19, 2014
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

#include "draprefsmapper.h"
#include "draprefsspellingwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPrefsMapper"
#include <logging/catlog.h>

struct _DraPrefsMapperPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsMapper, dra_prefs_mapper, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraPrefsMapper)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_prefs_mapper_class_init(DraPrefsMapperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_mapper_init(DraPrefsMapper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraPrefsMapper *instance = DRA_PREFS_MAPPER(object);
//	DraPrefsMapperPrivate *priv = dra_prefs_mapper_get_instance_private(instance);
	G_OBJECT_CLASS(dra_prefs_mapper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_mapper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraPrefsMapper *dra_prefs_mapper_new() {
	DraPrefsMapper *result = g_object_new(DRA_TYPE_PREFS_MAPPER, NULL);
	cat_ref_anounce(result);
//	DraPrefsMapperPrivate *priv = dra_prefs_mapper_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}

static CatS l_s_editor = CAT_S_DEF("editor");
static CatS l_s_blink_cursor = CAT_S_DEF("blink-cursor");
static CatS l_s_block_selection = CAT_S_DEF("block-selection");
static CatS l_s_deprecated_scrolling = CAT_S_DEF("deprecated-scrolling");
static CatS l_s_limit_cursor = CAT_S_DEF("limit-cursor");
static CatS l_s_highlight_line = CAT_S_DEF("highlight-line");
static CatS l_s_show_line_numbers = CAT_S_DEF("show-line-numbers");
static CatS l_s_show_whitespace = CAT_S_DEF("show-whitespace");
static CatS l_s_tab_size = CAT_S_DEF("tab-size");
static CatS l_s_wrap_lines = CAT_S_DEF("wrap-lines");
static CatS l_s_font_name = CAT_S_DEF("font_name");
static CatS l_s_color_map = CAT_S_DEF("color-map");
static CatS l_s_template = CAT_S_DEF("template");
static CatS l_s_description = CAT_S_DEF("description");
static CatS l_s_text = CAT_S_DEF("text");

static CatS l_s_spelling = CAT_S_DEF("spelling");
static CatS l_s_is_enabled = CAT_S_DEF("enabled");
static CatS l_s_dict_name = CAT_S_DEF("dictionary-name");
static CatS l_s_max_nr_props = CAT_S_DEF("max-nr-of-proposals");


void dra_prefs_mapper_model_to_prefs(ShoModel *model, DraPreferencesWo *e_prefs) {

	cat_log_debug("model=%o, e_prefs=%o", model, e_prefs);

	ChaPreferencesWo *e_cprefs = (ChaPreferencesWo *) e_prefs;

	ShoReference *ref_editor = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) model, CAT_S(l_s_editor));
	if (ref_editor) {
		ShoIValue *val_editor = sho_reference_get_value(ref_editor);
		cat_log_debug("val_editor=%o", val_editor);
		if (SHO_IS_REFERENCE_LIST(val_editor)) {

			cat_log_debug("e_prefs=%o", e_prefs);

			gboolean do_blink = cha_preferences_wo_get_blink_cursor(e_cprefs);
			do_blink = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_blink_cursor), do_blink);
			cha_preferences_wo_set_blink_cursor(e_cprefs, do_blink);

			gboolean block_selection = cha_preferences_wo_get_block_selection(e_cprefs);
			block_selection = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_block_selection), block_selection);
			cha_preferences_wo_set_block_selection(e_cprefs, block_selection);

			gboolean deprecated_scrolling = cha_preferences_wo_get_deprecated_scrolling(e_cprefs);
			deprecated_scrolling = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_deprecated_scrolling), deprecated_scrolling);
			cha_preferences_wo_set_deprecated_scrolling(e_cprefs, deprecated_scrolling);

			gboolean limit_cursor = cha_preferences_wo_get_limit_cursor(e_cprefs);
			limit_cursor = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_limit_cursor), limit_cursor);
			cha_preferences_wo_set_limit_cursor(e_cprefs, limit_cursor);

			gboolean highlight_line = cha_preferences_wo_get_highlight_current_line(e_cprefs);
			highlight_line = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_highlight_line), highlight_line);
			cha_preferences_wo_set_highlight_current_line(e_cprefs, highlight_line);

			gboolean show_line_numbers = cha_preferences_wo_get_show_line_numbers(e_cprefs);
			show_line_numbers = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_show_line_numbers), show_line_numbers);
			cha_preferences_wo_set_show_line_numbers(e_cprefs, show_line_numbers);

			gboolean show_whitespace = cha_preferences_wo_get_show_whitespace(e_cprefs);
			show_whitespace = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_show_whitespace), show_whitespace);
			cha_preferences_wo_set_show_whitespace(e_cprefs, show_whitespace);

			int tab_size = cha_preferences_wo_get_tab_size(e_cprefs);
			tab_size = sho_reference_list_get_int((ShoReferenceList *) val_editor, CAT_S(l_s_tab_size), tab_size);
			cha_preferences_wo_set_tab_size(e_cprefs, tab_size);

			gboolean wrap_lines = cha_preferences_wo_get_wrap_lines(e_cprefs);
			wrap_lines = sho_reference_list_get_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_wrap_lines), wrap_lines);
			cha_preferences_wo_set_wrap_lines(e_cprefs, wrap_lines);

			CatStringWo *font_name = cha_preferences_wo_get_font_name(e_cprefs);
			font_name = sho_reference_list_get_string((ShoReferenceList *) val_editor, CAT_S(l_s_font_name), font_name);
			cha_preferences_wo_set_font_name(e_cprefs, font_name);


			ShoReference *cmap_ref = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) val_editor, CAT_S(l_s_color_map));
			if (cmap_ref) {
				ShoIValue *v = sho_reference_get_value(cmap_ref);
				if (SHO_IS_LIST_VALUE(v)) {
					ShoListValue *cmap_lv = SHO_LIST_VALUE(v);
					int count = sho_list_value_count(cmap_lv);
					ChaPrefsColorMapWo *e_cmap = cha_preferences_wo_get_editable_color_map(e_cprefs);
					if (cha_prefs_color_map_wo_count(e_cmap)<count) {
						cha_prefs_color_map_wo_set_entry_count(e_cmap, count);
					}
					int idx;
					for(idx=0; idx<count; idx++) {
						v = sho_list_value_get_at(cmap_lv, idx);
						if (SHO_IS_LIST_VALUE(v)) {
							ShoListValue *cmap_elv = SHO_LIST_VALUE(v);
							if (sho_list_value_count(cmap_elv)!=3) {
								continue;
							}
							ShoIValue *red = sho_list_value_get_at(cmap_elv, 0);
							ShoIValue *green = sho_list_value_get_at(cmap_elv, 1);
							ShoIValue *blue = sho_list_value_get_at(cmap_elv, 2);
							if (SHO_IS_SIMPLE_VALUE(red) && SHO_IS_SIMPLE_VALUE(green) && SHO_IS_SIMPLE_VALUE(blue)) {
								double redv = sho_simple_value_get_double((ShoSimpleValue *) red, 1);
								double greenv = sho_simple_value_get_double((ShoSimpleValue *) green, 1);
								double bluev = sho_simple_value_get_double((ShoSimpleValue *) blue, 1);
								cha_prefs_color_map_wo_set_color_at(e_cmap, redv, greenv, bluev, idx);
							}
						}

					}
				}
			}

			ShoReference *spelling_ref = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) val_editor, CAT_S(l_s_spelling));
			cat_log_debug("spelling_ref=%O", spelling_ref);
			if (spelling_ref) {
				ShoIValue *v = sho_reference_get_value(spelling_ref);
				cat_log_detail("v=%O", v);
				if (SHO_IS_REFERENCE_LIST(v)) {
					DraPrefsSpellingWo *e_prefs_spelling = dra_preferences_wo_get_editable_spelling(e_prefs);

					gboolean is_enabled = dra_prefs_spelling_wo_is_enabled(e_prefs_spelling);
					is_enabled = sho_reference_list_get_boolean((ShoReferenceList *) v, CAT_S(l_s_is_enabled), is_enabled);
					dra_prefs_spelling_wo_set_enabled(e_prefs_spelling, is_enabled);

					CatStringWo *dict_name = dra_prefs_spelling_wo_get_dictionary_name(e_prefs_spelling);
					dict_name = sho_reference_list_get_string((ShoReferenceList *) v, CAT_S(l_s_dict_name), dict_name);
					dra_prefs_spelling_wo_set_dictionary_name(e_prefs_spelling, dict_name);

					int max_suggestions = dra_prefs_spelling_wo_get_max_suggestions(e_prefs_spelling);
					max_suggestions = sho_reference_list_get_int((ShoReferenceList *) v, CAT_S(l_s_max_nr_props), max_suggestions);
					dra_prefs_spelling_wo_set_max_suggestions(e_prefs_spelling, max_suggestions);

				}
			}


			CatHashMapWo *e_template_map = cha_preferences_wo_get_editable_template_map(e_cprefs);

			CatIIterator *iter = sho_reference_list_iterator((ShoReferenceList *) val_editor);
			while(cat_iiterator_has_next(iter)) {
				ShoReference *ref = (ShoReference *) cat_iiterator_next(iter);
				CatStringWo *a_id = sho_reference_get_id(ref);
				cat_log_debug("a_id=%o", a_id);
				if (cat_string_wo_equal(a_id, CAT_S(l_s_template))) {
					ShoIValue *tmp_v = (ShoIValue *) sho_reference_get_value(ref);
					cat_log_debug("tmp_v=%o", tmp_v);
					if (SHO_IS_REFERENCE_LIST(tmp_v)) {
						CatStringWo *name = sho_reference_list_get_string((ShoReferenceList *) tmp_v, CAT_S(sho_s_name), NULL);
						CatStringWo *description = sho_reference_list_get_string((ShoReferenceList *) tmp_v, CAT_S(l_s_description), NULL);
						CatStringWo *text = sho_reference_list_get_string((ShoReferenceList *) tmp_v, CAT_S(l_s_text), NULL);
						cat_log_debug("name=%o, description=%o, text=%o", name, description, text);
						if ((name!=NULL) && (description!=NULL) && (text!=NULL)) {
							ChaPrefsTemplateWo *tt = cha_prefs_template_wo_new();
							cha_prefs_template_wo_set_name(tt, name);
							cha_prefs_template_wo_set_description(tt, description);
							cha_prefs_template_wo_set_template(tt, text);
							cat_hash_map_wo_put(e_template_map, (GObject *) name, (GObject *) tt);
							cat_unref_ptr(tt);
						}
					}
				}

			}
			cat_unref_ptr(iter);

		}
	}
}

void dra_prefs_mapper_prefs_to_model(DraPreferencesWo *prefs, ShoModel *model) {

	ShoReference *ref_editor = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) model, CAT_S(l_s_editor));
	if (ref_editor == NULL) {
		ref_editor = sho_reference_new(CAT_S(l_s_editor));
		sho_reference_list_add_reference((ShoReferenceList *) model, ref_editor);
		cat_unref(ref_editor);
	}

	ShoIValue *val_editor = sho_reference_get_value(ref_editor);
	if (val_editor == NULL) {
		val_editor = (ShoIValue *) sho_block_value_new();
		sho_reference_set_value(ref_editor, val_editor);
		cat_unref(val_editor);
	}


	if (SHO_IS_REFERENCE_LIST(val_editor)) {
		if (prefs) {
			ChaPreferencesWo *cprefs = (ChaPreferencesWo *) prefs;
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_blink_cursor), cha_preferences_wo_get_blink_cursor(cprefs));
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_block_selection), cha_preferences_wo_get_block_selection(cprefs));
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_deprecated_scrolling), cha_preferences_wo_get_deprecated_scrolling(cprefs));
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_limit_cursor), cha_preferences_wo_get_limit_cursor(cprefs));
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_highlight_line), cha_preferences_wo_get_highlight_current_line(cprefs));
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_show_line_numbers), cha_preferences_wo_get_show_line_numbers(cprefs));
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_show_whitespace), cha_preferences_wo_get_show_whitespace(cprefs));
			sho_reference_list_set_int((ShoReferenceList *) val_editor, CAT_S(l_s_tab_size), cha_preferences_wo_get_tab_size(cprefs));
			sho_reference_list_set_boolean((ShoReferenceList *) val_editor, CAT_S(l_s_wrap_lines), cha_preferences_wo_get_wrap_lines(cprefs));
			cat_log_debug("font=%o", cha_preferences_wo_get_font_name(cprefs));
			sho_reference_list_set_string((ShoReferenceList *) val_editor, CAT_S(l_s_font_name), cha_preferences_wo_get_font_name(cprefs));

			/* color map */
			ChaPrefsColorMapWo *cmap =  cha_preferences_wo_get_color_map(cprefs);
			if (cmap) {
				ShoListValue *cmap_lv = sho_list_value_new();
				int idx;
				int count = cha_prefs_color_map_wo_count(cmap);
				for(idx=0; idx<count; idx++) {
					ChaColorEntry ace = cha_prefs_color_map_wo_get_color(cmap, idx);
					ShoListValue *scv = sho_list_value_new();
					ShoSimpleValue *ssv_rgb = sho_simple_value_new_double(ace.red);
					sho_list_value_add_value(scv, (ShoIValue *) ssv_rgb);
					cat_unref_ptr(ssv_rgb);
					ssv_rgb = sho_simple_value_new_double(ace.green);
					sho_list_value_add_value(scv, (ShoIValue *) ssv_rgb);
					cat_unref_ptr(ssv_rgb);
					ssv_rgb = sho_simple_value_new_double(ace.blue);
					sho_list_value_add_value(scv, (ShoIValue *) ssv_rgb);
					cat_unref_ptr(ssv_rgb);

					sho_list_value_add_value(cmap_lv, (ShoIValue *) scv);
					cat_unref_ptr(scv);
				}
				ShoReference *cmap_ref = sho_reference_new(CAT_S(l_s_color_map));
				sho_reference_set_value(cmap_ref, (ShoIValue *) cmap_lv);
				sho_reference_list_set_reference((ShoReferenceList *) val_editor, cmap_ref);
				cat_unref_ptr(cmap_ref);
				cat_unref_ptr(cmap_lv);
			}

			/* templates */
			CatHashMapWo *temp_map = cha_preferences_wo_get_template_map(cprefs);
			cat_log_debug("temp_map=%p, size=%d", temp_map, temp_map==NULL ? -1 : cat_hash_map_wo_size(temp_map));
			if (temp_map!=NULL && cat_hash_map_wo_size(temp_map)>0) {
				CatArrayWo *values =  cat_hash_map_wo_enlist_values(temp_map, NULL);
				cat_log_debug("template-map values=%o", values);
				CatIIterator *values_iter = cat_array_wo_iterator(values);
				while(cat_iiterator_has_next(values_iter)) {
					ChaPrefsTemplateWo *template = (ChaPrefsTemplateWo *) cat_iiterator_next(values_iter);
					ShoReference *temp_ref = sho_reference_new(CAT_S(l_s_template));
					ShoBlockValue *temp_bv = sho_block_value_new();
					sho_reference_set_value(temp_ref, (ShoIValue *) temp_bv);
					sho_reference_list_add_reference((ShoReferenceList *) val_editor, temp_ref);

					sho_reference_list_set_string((ShoReferenceList *) temp_bv, CAT_S(sho_s_name),  cha_prefs_template_wo_get_name(template));
					sho_reference_list_set_string((ShoReferenceList *) temp_bv, CAT_S(l_s_description),  cha_prefs_template_wo_get_description(template));
					sho_reference_list_set_string((ShoReferenceList *) temp_bv, CAT_S(l_s_text),  cha_prefs_template_wo_get_template(template));

					cat_unref_ptr(temp_ref);
					cat_unref_ptr(temp_bv);

				}
				cat_unref_ptr(values_iter);
			}


			/* spelling */
			DraPrefsSpellingWo *spelling = dra_preferences_wo_get_spelling(prefs);
			if (spelling) {
				ShoBlockValue *sp_bv = sho_block_value_new();

				sho_reference_list_set_boolean((ShoReferenceList *) sp_bv, CAT_S(l_s_is_enabled), dra_prefs_spelling_wo_is_enabled(spelling));
				sho_reference_list_set_string((ShoReferenceList *) sp_bv, CAT_S(l_s_dict_name), dra_prefs_spelling_wo_get_dictionary_name(spelling));
				sho_reference_list_set_int((ShoReferenceList *) sp_bv, CAT_S(l_s_max_nr_props), dra_prefs_spelling_wo_get_max_suggestions(spelling));

				ShoReference *sp_ref = sho_reference_new(CAT_S(l_s_spelling));
				sho_reference_set_value(sp_ref, (ShoIValue *) sp_bv);
				cat_unref_ptr(sp_bv);
				sho_reference_list_add_reference((ShoReferenceList *) val_editor, sp_ref);
				cat_unref_ptr(sp_ref);
			}
		}
	} else {
		cat_log_error("val_editor not a reference_list: %o", val_editor);
	}
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

