/*
   File:    jagprefsmapper.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 17, 2014
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

#include "jagprefsmapper.h"
#include "jagpreferenceswo.h"
#include "jagprefsjrewo.h"
#include "jagprefsaccessor.h"
#include <elk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPrefsMapper"
#include <logging/catlog.h>

static CatS l_s_java = CAT_S_DEF("java");
static CatS l_s_jre = CAT_S_DEF("jre");
static CatS l_s_libs = CAT_S_DEF("libs");


struct _JagPrefsMapperPrivate {
	CowIEntryAccessor *entry_accessor;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_preferences_mapper_iface_init(ElkIPreferencesMapperInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPrefsMapper, jag_prefs_mapper, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPrefsMapper)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IPREFERENCES_MAPPER, l_preferences_mapper_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_prefs_mapper_class_init(JagPrefsMapperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_prefs_mapper_init(JagPrefsMapper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPrefsMapper *instance = JAG_PREFS_MAPPER(object);
	JagPrefsMapperPrivate *priv = jag_prefs_mapper_get_instance_private(instance);
	cat_unref_ptr(priv->entry_accessor);
	G_OBJECT_CLASS(jag_prefs_mapper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_prefs_mapper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPrefsMapper *jag_prefs_mapper_new() {
	JagPrefsMapper *result = g_object_new(JAG_TYPE_PREFS_MAPPER, NULL);
	cat_ref_anounce(result);
	JagPrefsMapperPrivate *priv = jag_prefs_mapper_get_instance_private(result);
	priv->entry_accessor = (CowIEntryAccessor *) jag_prefs_accessor_new();
	return result;
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


/********************* start ElkIPreferencesMapper implementation *********************/

static void l_model_to_prefs(ElkIPreferencesMapper *self, ShoModel *model, ElkPreferencesWo *e_prefs) {
	JagPrefsMapper *instance = JAG_PREFS_MAPPER(self);
	JagPrefsMapperPrivate *priv = jag_prefs_mapper_get_instance_private(instance);

	cat_log_debug("model=%o, e_prefs=%o", model, e_prefs);

	ShoReference *ref_java = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) model, CAT_S(l_s_java));
	if (ref_java) {
		ShoIValue *val_java = sho_reference_get_value(ref_java);
		cat_log_debug("val_java=%o", val_java);
		if (SHO_IS_REFERENCE_LIST(val_java)) {

			JagPreferencesWo *e_jag_prefs = (JagPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, e_prefs);
			if (e_jag_prefs==NULL) {
				e_jag_prefs = jag_preferences_wo_new();
			} else if (jag_preferences_wo_is_anchored(e_jag_prefs)) {
				e_jag_prefs = jag_preferences_wo_create_editable(e_jag_prefs);
			} else {
				cat_ref_ptr(e_jag_prefs);
			}
			cat_log_debug("e_jag_prefs=%o", e_jag_prefs);

			cow_ientry_accessor_set(priv->entry_accessor, e_prefs, e_jag_prefs);

			CatArrayWo *e_jre_list = jag_preferences_wo_get_editable_jre_list(e_jag_prefs);
			CatIIterator *java_iter = sho_reference_list_iterator((ShoReferenceList *) val_java);
			while(cat_iiterator_has_next(java_iter)) {
				ShoReference *jre_ref = cat_iiterator_next(java_iter);
				cat_log_debug("jre_ref=%o", jre_ref);
				CatStringWo *a_ref_id = sho_reference_get_id(jre_ref);
				if (!cat_string_wo_equal(a_ref_id, CAT_S(l_s_jre))) {
					continue;
				}
				ShoIValue *jre_val = sho_reference_get_value(jre_ref);
				if (SHO_IS_BLOCK_VALUE(jre_val)) {
					CatStringWo *jre_name = sho_reference_list_get_string((ShoReferenceList *) jre_val, CAT_S(sho_s_name), NULL);
					CatStringWo *jre_path = sho_reference_list_get_string((ShoReferenceList *) jre_val, CAT_S(sho_s_path), NULL);

					cat_log_debug("jre_name=%o, jre_path=%o", jre_name, jre_path);


					JagPrefsJreWo *e_jre_wo = jag_prefs_jre_wo_new();
					jag_prefs_jre_wo_set_name(e_jre_wo, jre_name);
					jag_prefs_jre_wo_set_path(e_jre_wo, jre_path);

					ShoReference *libs_ref = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) jre_val, CAT_S(l_s_libs));
					cat_log_debug("libs_ref=%o", libs_ref);
					if (libs_ref) {
						ShoIValue *libs_val = sho_reference_get_value(libs_ref);
						cat_log_debug("libs_val=%o", libs_val);
						if (SHO_IS_LIST_VALUE(libs_val)) {
							CatArrayWo *new_lib_paths = cat_array_wo_new();
							ShoListValue *libs_lv = (ShoListValue *) libs_val;

							cat_log_debug("libs_lv=%o", libs_lv);

							CatIIterator *iter = sho_list_value_iterator(libs_lv);
							while(cat_iiterator_has_next(iter)) {

								ShoIValue *lib_entry_val = (ShoIValue *) cat_iiterator_next(iter);
								cat_log_debug("lib_entry_val=%o", lib_entry_val);
								if (SHO_IS_SIMPLE_VALUE(lib_entry_val)) {
									CatStringWo *lb = sho_simple_value_get_string((ShoSimpleValue *) lib_entry_val);
									cat_log_debug("lb=%o", lb);
									cat_array_wo_append(new_lib_paths, (GObject *) lb);
								}

							}
							cat_unref_ptr(iter);
							jag_prefs_jre_wo_set_library_paths(e_jre_wo, new_lib_paths);
							cat_unref_ptr(new_lib_paths);
						}
					}

					cat_array_wo_append(e_jre_list, (GObject *) e_jre_wo);
					cat_unref_ptr(e_jre_wo);
				}
			}
			cat_unref_ptr(e_jag_prefs);
			cat_unref_ptr(java_iter);
		}
	}

//	cat_log_debug("one_instance=%d, full_screen=%d", one_instance, full_screen);
//
//	elk_preferences_wo_set_one_instance(e_prefs, one_instance);
//	elk_preferences_wo_set_full_screen(e_prefs, full_screen);
//	elk_preferences_wo_set_multiple_resource_editors(e_prefs, multiple_res_editors);
//	elk_preferences_wo_set_own_process(e_prefs, own_process);
}

static void l_prefs_to_model(ElkIPreferencesMapper *self, ElkPreferencesWo *prefs, ShoModel *model) {
	JagPrefsMapper *instance = JAG_PREFS_MAPPER(self);
	JagPrefsMapperPrivate *priv = jag_prefs_mapper_get_instance_private(instance);

	ShoReference *ref_java = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) model, CAT_S(l_s_java));
	if (ref_java == NULL) {
		ref_java = sho_reference_new(CAT_S(l_s_java));
		sho_reference_list_add_reference((ShoReferenceList *) model, ref_java);
		cat_unref(ref_java);
	}

	ShoIValue *val_java = sho_reference_get_value(ref_java);
	if (val_java == NULL) {
		val_java = (ShoIValue *) sho_block_value_new();
		sho_reference_set_value(ref_java, val_java);
		cat_unref(val_java);
	}


	if (SHO_IS_REFERENCE_LIST(val_java)) {
		JagPreferencesWo *jag_prefs = (JagPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, prefs);
		if (jag_prefs) {
			CatArrayWo *jre_list = jag_preferences_wo_get_jre_list(jag_prefs);
			CatIIterator *jre_iter = cat_array_wo_iterator(jre_list);
			while(cat_iiterator_has_next(jre_iter)) {
				JagPrefsJreWo *jre_wo = (JagPrefsJreWo *) cat_iiterator_next(jre_iter);


				ShoReference *ref_jre = sho_reference_new(CAT_S(l_s_jre));
				sho_reference_list_add_reference((ShoReferenceList *) val_java, ref_jre);
				ShoBlockValue *bv_jre = sho_block_value_new();
				sho_reference_set_value(ref_jre, (ShoIValue *) bv_jre);
				sho_reference_list_set_string((ShoReferenceList *) bv_jre, CAT_S(sho_s_name), jag_prefs_jre_wo_get_name(jre_wo));
				sho_reference_list_set_string((ShoReferenceList *) bv_jre, CAT_S(sho_s_path), jag_prefs_jre_wo_get_path(jre_wo));
				CatArrayWo *lib_paths = jag_prefs_jre_wo_get_library_paths(jre_wo);
				if (lib_paths) {
					ShoReference *ref_paths = sho_reference_new(CAT_S(l_s_libs));
					sho_reference_list_set_reference((ShoReferenceList *) bv_jre, ref_paths);
					ShoListValue *lv_paths = sho_list_value_new();
					sho_reference_set_value(ref_paths, (ShoIValue *) lv_paths);
					CatIIterator *libs_iter = cat_array_wo_iterator(lib_paths);
					while(cat_iiterator_has_next(libs_iter)) {
						CatStringWo *path = (CatStringWo *) cat_iiterator_next(libs_iter);
						sho_list_value_add_string(lv_paths, path);
					}
					cat_unref_ptr(libs_iter);
					cat_unref_ptr(ref_paths);
					cat_unref_ptr(lv_paths);
				}

				cat_unref_ptr(bv_jre)
				cat_unref_ptr(ref_jre)
			}
			cat_unref_ptr(jre_iter);
		}

	} else {
		cat_log_error("val_java not a reference_list: %o", val_java);
	}

//	sho_reference_list_set_boolean((ShoReferenceList *) model, CAT_S(l_s_one_instance), elk_preferences_wo_one_instance(prefs));
//	sho_reference_list_set_boolean((ShoReferenceList *) model, CAT_S(l_s_full_screen), elk_preferences_wo_full_screen(prefs));
//	sho_reference_list_set_boolean((ShoReferenceList *) model, CAT_S(l_s_multiple_resource_editors), elk_preferences_wo_multiple_resource_editors(prefs));
//	sho_reference_list_set_boolean((ShoReferenceList *) model, CAT_S(l_s_own_process), elk_preferences_wo_own_process(prefs));
//	sho_reference_list_get_first_reference_by_id((ShoReferenceList *) model)
}

static void l_preferences_mapper_iface_init(ElkIPreferencesMapperInterface *iface) {
	iface->modelToPrefs = l_model_to_prefs;
	iface->prefsToModel = l_prefs_to_model;
}

/********************* end ElkIPreferencesMapper implementation *********************/
