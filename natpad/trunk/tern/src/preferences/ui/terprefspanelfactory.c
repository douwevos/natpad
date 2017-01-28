/*
   File:    terprefspanelfactory.c
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

#include "terprefspanelfactory.h"
#include "terprefsclazzdescription.h"
#include "terprefsmainpanel.h"
#include "terprefsclazzpanel.h"
#include "../terprefsclazzwo.h"
#include "../terpreferenceswo.h"
#include "../terprefsmapper.h"
#include <cow.h>
#include <elk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerPrefsPanelFactory"
#include <logging/catlog.h>

struct _TerPrefsPanelFactoryPrivate {
	CowPanelDescription *pd_textpad;
	CowDialog *cow_dialog;

	GtkTreeIter iter_textpad;
	ElkPreferencesContainer *elk_prefs_container;

	CatHashMapWo *document_classes;
	CatArrayWo *e_pd_clazzes;

	VipPath *shared_config_path;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface);
static void l_change_listener_iface_init(CowIChangeListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerPrefsPanelFactory, ter_prefs_panel_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TerPrefsPanelFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_IPANEL_FACTORY, l_panel_factory_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_ICHANGE_LISTENER, l_change_listener_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_prefs_panel_factory_class_init(TerPrefsPanelFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

}

static void ter_prefs_panel_factory_init(TerPrefsPanelFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerPrefsPanelFactory *instance = TER_PREFS_PANEL_FACTORY(object);
	TerPrefsPanelFactoryPrivate *priv = ter_prefs_panel_factory_get_instance_private(instance);
	cat_unref_ptr(priv->document_classes);
	cat_unref_ptr(priv->pd_textpad);
	cat_unref_ptr(priv->shared_config_path);
	cat_unref_ptr(priv->e_pd_clazzes);
	priv->elk_prefs_container = NULL;
	G_OBJECT_CLASS(ter_prefs_panel_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_prefs_panel_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerPrefsPanelFactory *ter_prefs_panel_factory_new(VipPath *shared_config_path) {
	TerPrefsPanelFactory *result = g_object_new(TER_TYPE_PREFS_PANEL_FACTORY, NULL);
	cat_ref_anounce(result);
	TerPrefsPanelFactoryPrivate *priv = ter_prefs_panel_factory_get_instance_private(result);
	priv->pd_textpad = cow_panel_description_new(cat_string_wo_new_with("Textpad"));
	priv->elk_prefs_container = NULL;
	priv->document_classes = NULL;
	priv->cow_dialog = NULL;
	priv->e_pd_clazzes = cat_array_wo_new();
	priv->shared_config_path = cat_ref_ptr(shared_config_path);
	return result;
}


static int l_find_pd_clazz(CatArrayWo *pd_clazzes, TerPrefsClazzWo *what, int expected_index) {
	int found_index = -1;
	CatStringWo *swhat_name = ter_prefs_clazz_wo_get_name(what);
	for(; expected_index<cat_array_wo_size(pd_clazzes); expected_index++) {
		TerPrefsClazzDescription *clazz_descr = (TerPrefsClazzDescription *) cat_array_wo_get(pd_clazzes, expected_index);
		CatStringWo *a_entry_name = cow_panel_description_get_entry_name((CowPanelDescription *) clazz_descr);
		if (cat_string_wo_equal(swhat_name, a_entry_name)) {
			found_index = expected_index;
			break;
		}
	}
	return found_index;
}

void ter_prefs_panel_factory_set_configuration(TerPrefsPanelFactory *factory) {
	TerPrefsPanelFactoryPrivate *priv = ter_prefs_panel_factory_get_instance_private(factory);

	ElkPreferencesWo *elk_preferences = elk_preferences_container_get(priv->elk_prefs_container);
	TerPreferencesWo *ter_preferences = (TerPreferencesWo *) elk_preferences_wo_get_sub(elk_preferences, CAT_S(ter_s_textpad));
	CatHashMapWo *new_classes = NULL;
	if (ter_preferences!=NULL) {
		new_classes = ter_preferences_wo_get_document_clazzes(ter_preferences);
	}

	cat_log_detail("updating document_classes: new_classes=%p", new_classes);
//	if (priv->document_classes!=new_classes) {
		cat_ref_swap(priv->document_classes, new_classes);
		if (new_classes) {
			CatArrayWo *e_keys = cat_hash_map_wo_enlist_keys(new_classes, NULL);
			cat_log_detail("e_keys=%o", e_keys);
			cat_array_wo_sort(e_keys, (GCompareFunc) cat_string_wo_compare);
			int idx = 0;
			for(idx=0; idx<cat_array_wo_size(e_keys); idx++) {
				CatStringWo *name = (CatStringWo *) cat_array_wo_get(e_keys, idx);
				TerPrefsClazzWo *clazz_wo = (TerPrefsClazzWo *) cat_hash_map_wo_get(new_classes, name);
				int existing_idx = l_find_pd_clazz(priv->e_pd_clazzes, clazz_wo, idx);
				cat_log_debug("priv->e_pd_clazzes=%o, idx=%d ,existing_idx=%d",priv->e_pd_clazzes, idx, existing_idx);
				if (existing_idx<0) {
					TerPrefsClazzDescription *rem = ter_prefs_clazz_description_new(clazz_wo);
					cat_array_wo_insert(priv->e_pd_clazzes, (GObject *) rem, idx);
					cat_unref_ptr(rem);
				} else if (existing_idx!=idx) {
					TerPrefsClazzDescription *rem;
					cat_array_wo_remove(priv->e_pd_clazzes, existing_idx, (GObject **) (&rem));
					cat_array_wo_insert(priv->e_pd_clazzes, (GObject *) rem, idx);
					ter_prefs_clazz_description_set(rem, clazz_wo);
					cat_unref_ptr(rem);
				}
			}
			cat_unref_ptr(e_keys);
		} else {
			cat_array_wo_clear(priv->e_pd_clazzes);
		}

		cat_log_debug("priv->e_pd_clazzes=%o",priv->e_pd_clazzes);
		cow_dialog_update_entries(priv->cow_dialog, &priv->iter_textpad, COW_IPANEL_FACTORY(factory), priv->e_pd_clazzes);
//	}

}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	TerPrefsPanelFactory *instance = TER_PREFS_PANEL_FACTORY(self);
	TerPrefsPanelFactoryPrivate *priv = ter_prefs_panel_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p shared_config_path=%o]", iname, self, priv->shared_config_path);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* start CowIPanelFactory implementation *********************/

static void l_factory_attach(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowContainer *e_container) {
	TerPrefsPanelFactory *instance = TER_PREFS_PANEL_FACTORY(self);
	TerPrefsPanelFactoryPrivate *priv = ter_prefs_panel_factory_get_instance_private(instance);
	priv->cow_dialog = cat_ref_ptr(cow_dialog);

	priv->iter_textpad = cow_dialog_add_entry(cow_dialog, NULL, self, priv->pd_textpad);
//	cow_dialog_add_entry(cow_dialog, &iter_java, self, priv->pd_compiler);

	priv->elk_prefs_container = ELK_PREFERENCES_CONTAINER(e_container);
	cow_container_add_change_listener(e_container, (CowIChangeListener *) self);
	ter_prefs_panel_factory_set_configuration(instance);
}

static void l_factory_detach(CowIPanelFactory *self, struct _CowDialog *cow_dialog) {
	TerPrefsPanelFactory *instance = TER_PREFS_PANEL_FACTORY(self);
	TerPrefsPanelFactoryPrivate *priv = ter_prefs_panel_factory_get_instance_private(instance);
	cat_unref_ptr(priv->cow_dialog)
}

static CowPanel *l_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr) {
	TerPrefsPanelFactory *instance = TER_PREFS_PANEL_FACTORY(self);
	TerPrefsPanelFactoryPrivate *priv = ter_prefs_panel_factory_get_instance_private(instance);

	cat_log_debug("panel_descr=%o", panel_descr);

	CowPanel *result = NULL;
	if (panel_descr==priv->pd_textpad) {
		result = (CowPanel *) ter_prefs_main_panel_new();
	} else if (TER_IS_PREFS_CLAZZ_DESCRIPTION(panel_descr)) {
		TerPrefsClazzDescription *clazz_descr = TER_PREFS_CLAZZ_DESCRIPTION(panel_descr);
		result = (CowPanel *) ter_prefs_clazz_panel_new(ter_prefs_clazz_description_get(clazz_descr), priv->shared_config_path);
	}
	return result;
}


static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface) {
	iface->attach = l_factory_attach;
	iface->detach = l_factory_detach;
	iface->create = l_factory_create;
}

/********************* end CowIPanelFactory implementation *********************/

static void l_config_changed(CowIChangeListener *self, GObject *new_config) {
	TerPrefsPanelFactory *instance = TER_PREFS_PANEL_FACTORY(self);

	ter_prefs_panel_factory_set_configuration(instance);
}


static void l_change_listener_iface_init(CowIChangeListenerInterface *iface) {
	iface->configChanged = l_config_changed;
}
