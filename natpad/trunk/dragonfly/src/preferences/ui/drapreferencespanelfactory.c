/*
   File:    drapreferencespanelfactory.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 25, 2015
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

#include "drapreferencespanelfactory.h"
#include "draprefseditorpanel.h"
#include "draprefsbehaviourpanel.h"
#include "draprefscolormappanel.h"
#include "draprefstemplatespanel.h"
#include "draprefsspellingpanel.h"
#include <logging/catlogdefs.h>

#include "draprefsspellingpanel.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPreferencesPanelFactory"
#include <logging/catlog.h>

struct _DraPreferencesPanelFactoryPrivate {
	CowIEntryAccessor *entry_extractor;
	DraSpellHelper *spell_helper;
	CowPanelDescription *pd_editor;
	CowPanelDescription *pd_behaviour;
	CowPanelDescription *pd_colors;
	CowPanelDescription *pd_templates;
	CowPanelDescription *pd_spelling;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPreferencesPanelFactory, dra_preferences_panel_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraPreferencesPanelFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_IPANEL_FACTORY, l_panel_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_preferences_panel_factory_class_init(DraPreferencesPanelFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_preferences_panel_factory_init(DraPreferencesPanelFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPreferencesPanelFactory *instance = DRA_PREFERENCES_PANEL_FACTORY(object);
	DraPreferencesPanelFactoryPrivate *priv = dra_preferences_panel_factory_get_instance_private(instance);
	cat_unref_ptr(priv->entry_extractor);
	cat_unref_ptr(priv->spell_helper);
	cat_unref_ptr(priv->pd_editor);
	cat_unref_ptr(priv->pd_behaviour);
	cat_unref_ptr(priv->pd_colors);
	cat_unref_ptr(priv->pd_templates);
	cat_unref_ptr(priv->pd_spelling);
	G_OBJECT_CLASS(dra_preferences_panel_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_preferences_panel_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraPreferencesPanelFactory *dra_preferences_panel_factory_new(CowIEntryAccessor *entry_extractor) {
	DraPreferencesPanelFactory *result = g_object_new(DRA_TYPE_PREFERENCES_PANEL_FACTORY, NULL);
	cat_ref_anounce(result);
	DraPreferencesPanelFactoryPrivate *priv = dra_preferences_panel_factory_get_instance_private(result);
	priv->spell_helper = NULL;
	priv->entry_extractor = cat_ref_ptr(entry_extractor);
	priv->pd_editor = cow_panel_description_new(cat_string_wo_new_with("Editor"));
	priv->pd_behaviour = cow_panel_description_new(cat_string_wo_new_with("Behaviour"));
	priv->pd_colors = cow_panel_description_new(cat_string_wo_new_with("Colors"));
	priv->pd_templates = cow_panel_description_new(cat_string_wo_new_with("Templates"));
	priv->pd_spelling = cow_panel_description_new(cat_string_wo_new_with("Spelling"));
	return result;
}


void dra_preferences_panel_factory_set_spell_helper(DraPreferencesPanelFactory *factory, DraSpellHelper *spell_helper) {
	DraPreferencesPanelFactoryPrivate *priv = dra_preferences_panel_factory_get_instance_private(factory);
	cat_ref_swap(priv->spell_helper, spell_helper);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPreferencesPanelFactory *instance = DRA_PREFERENCES_PANEL_FACTORY(self);
	DraPreferencesPanelFactoryPrivate *priv = dra_preferences_panel_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p %o]", iname, self, priv->pd_editor);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



/********************* start CowIPanelFactory implementation *********************/

static void l_factory_attach(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowContainer *e_container) {
	DraPreferencesPanelFactory *instance = DRA_PREFERENCES_PANEL_FACTORY(self);
	DraPreferencesPanelFactoryPrivate *priv = dra_preferences_panel_factory_get_instance_private(instance);

	GtkTreeIter editor_iter = cow_dialog_add_entry(cow_dialog, NULL, self, priv->pd_editor);
	cow_dialog_add_entry(cow_dialog, &editor_iter, self, priv->pd_behaviour);
	cow_dialog_add_entry(cow_dialog, &editor_iter, self, priv->pd_spelling);
	cow_dialog_add_entry(cow_dialog, &editor_iter, self, priv->pd_colors);
	cow_dialog_add_entry(cow_dialog, &editor_iter, self, priv->pd_templates);

}

static void l_factory_detach(CowIPanelFactory *self, struct _CowDialog *cow_dialog) {

}

static CowPanel *l_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr) {
	DraPreferencesPanelFactory *instance = DRA_PREFERENCES_PANEL_FACTORY(self);
	DraPreferencesPanelFactoryPrivate *priv = dra_preferences_panel_factory_get_instance_private(instance);

	CowPanel *result = NULL;
	if (panel_descr==priv->pd_editor) {
		result = (CowPanel *) dra_prefs_editor_panel_new(priv->entry_extractor);
	} else if (panel_descr==priv->pd_behaviour) {
		result = (CowPanel *) dra_prefs_behaviour_panel_new(priv->entry_extractor);
	} else if (panel_descr==priv->pd_colors) {
		result = (CowPanel *) dra_prefs_color_map_panel_new(priv->entry_extractor);
	} else if (panel_descr==priv->pd_templates) {
		result = (CowPanel *) dra_prefs_templates_panel_new(priv->entry_extractor);
	} else if (panel_descr==priv->pd_spelling) {
		result = (CowPanel *) dra_prefs_spelling_panel_new(priv->entry_extractor, priv->spell_helper);
	}
	return result;
}


static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface) {
	iface->attach = l_factory_attach;
	iface->detach = l_factory_detach;
	iface->create = l_factory_create;
}

/********************* end CowIPanelFactory implementation *********************/

