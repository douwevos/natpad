/*
   File:    elkdefaultmapper.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 3, 2014
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

#include "elkdefaultmapper.h"
#include "elkipreferencesmapper.h"
#include <chameleon.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkDefaultMapper"
#include <logging/catlog.h>

static CatS l_s_one_instance = CAT_S_DEF("one-instance");
static CatS l_s_full_screen = CAT_S_DEF("full-screen");
static CatS l_s_multiple_resource_editors = CAT_S_DEF("multiple-editors-per-resource");



struct _ElkDefaultMapperPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_preferences_mapper_iface_init(ElkIPreferencesMapperInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkDefaultMapper, elk_default_mapper, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ElkDefaultMapper)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IPREFERENCES_MAPPER, l_preferences_mapper_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_default_mapper_class_init(ElkDefaultMapperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_default_mapper_init(ElkDefaultMapper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ElkDefaultMapper *instance = ELK_DEFAULT_MAPPER(object);
//	ElkDefaultMapperPrivate *priv = elk_default_mapper_get_instance_private(instance);
	G_OBJECT_CLASS(elk_default_mapper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_default_mapper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkDefaultMapper *elk_default_mapper_new() {
	ElkDefaultMapper *result = g_object_new(ELK_TYPE_DEFAULT_MAPPER, NULL);
	cat_ref_anounce(result);
//	ElkDefaultMapperPrivate *priv = elk_default_mapper_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
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

static void l_models_to_prefs(ElkIPreferencesMapper *self, ShoModel *model, ElkPreferencesWo *e_prefs) {
	cat_log_indent_level++;
	gboolean one_instance = sho_reference_list_get_boolean((ShoReferenceList *) model, CAT_S(l_s_one_instance), TRUE);
	gboolean full_screen = sho_reference_list_get_boolean((ShoReferenceList *) model, CAT_S(l_s_full_screen), TRUE);
	gboolean multiple_res_editors = sho_reference_list_get_boolean((ShoReferenceList *) model, CAT_S(l_s_multiple_resource_editors), FALSE);

	cat_log_debug("one_instance=%d, full_screen=%d", one_instance, full_screen);

	elk_preferences_wo_set_one_instance(e_prefs, one_instance);
	elk_preferences_wo_set_full_screen(e_prefs, full_screen);
	elk_preferences_wo_set_multiple_resource_editors(e_prefs, multiple_res_editors);

	DraPreferencesWo *e_dra_prefs = (DraPreferencesWo *) elk_preferences_wo_get_sub(e_prefs, CAT_S(elk_s_editor_prefs));
	if (e_dra_prefs==NULL) {
		e_dra_prefs = dra_preferences_wo_new();
		elk_preferences_wo_set_sub(e_prefs, CAT_S(elk_s_editor_prefs), (GObject *) e_dra_prefs);
		cat_unref(e_dra_prefs);
	} else if (dra_preferences_wo_is_anchored(e_dra_prefs)) {
		e_dra_prefs = dra_preferences_wo_create_editable(e_dra_prefs);
		elk_preferences_wo_set_sub(e_prefs, CAT_S(elk_s_editor_prefs), (GObject *) e_dra_prefs);
		cat_unref(e_dra_prefs);
	}

	dra_prefs_mapper_model_to_prefs(model, e_dra_prefs);


	cat_log_trace("done");
	cat_log_indent_level--;

}

static void l_prefs_to_model(ElkIPreferencesMapper *self, ElkPreferencesWo *prefs, ShoModel *model) {
//	ElkDefaultMapper *instance = ELK_DEFAULT_MAPPER(self);
//	ElkDefaultMapperPrivate *priv = elk_default_mapper_get_instance_private(instance);
	sho_reference_list_set_boolean((ShoReferenceList *) model, CAT_S(l_s_one_instance), elk_preferences_wo_one_instance(prefs));
	sho_reference_list_set_boolean((ShoReferenceList *) model, CAT_S(l_s_full_screen), elk_preferences_wo_full_screen(prefs));
	sho_reference_list_set_boolean((ShoReferenceList *) model, CAT_S(l_s_multiple_resource_editors), elk_preferences_wo_multiple_resource_editors(prefs));

	DraPreferencesWo *dra_prefs = (DraPreferencesWo *) elk_preferences_wo_get_sub(prefs, CAT_S(elk_s_editor_prefs));
	if (dra_prefs) {
		dra_prefs_mapper_prefs_to_model(dra_prefs, model);
	}

}

static void l_preferences_mapper_iface_init(ElkIPreferencesMapperInterface *iface) {
	iface->modelToPrefs = l_models_to_prefs;
	iface->prefsToModel = l_prefs_to_model;
}

/********************* end ElkIPreferencesMapper implementation *********************/
