/*
   File:    elkpreferencespanelfactory.c
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 26, 2014
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

#include "elkpreferencespanelfactory.h"
#include "elkmainprefspanel.h"
#include <cow.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkPreferencesPanelFactory"
#include <logging/catlog.h>

struct _ElkPreferencesPanelFactoryPrivate {
	CowPanelDescription *pd_general;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkPreferencesPanelFactory, elk_preferences_panel_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ElkPreferencesPanelFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_IPANEL_FACTORY, l_panel_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_preferences_panel_factory_class_init(ElkPreferencesPanelFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_preferences_panel_factory_init(ElkPreferencesPanelFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkPreferencesPanelFactory *instance = ELK_PREFERENCES_PANEL_FACTORY(object);
	ElkPreferencesPanelFactoryPrivate *priv = elk_preferences_panel_factory_get_instance_private(instance);
	cat_unref_ptr(priv->pd_general);
	G_OBJECT_CLASS(elk_preferences_panel_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_preferences_panel_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkPreferencesPanelFactory *elk_preferences_panel_factory_new() {
	ElkPreferencesPanelFactory *result = g_object_new(ELK_TYPE_PREFERENCES_PANEL_FACTORY, NULL);
	cat_ref_anounce(result);
	ElkPreferencesPanelFactoryPrivate *priv = elk_preferences_panel_factory_get_instance_private(result);
	priv->pd_general = cow_panel_description_new(cat_string_wo_new_with("General"));
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkPreferencesPanelFactory *instance = ELK_PREFERENCES_PANEL_FACTORY(self);
	ElkPreferencesPanelFactoryPrivate *priv = elk_preferences_panel_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p %o]", iname, self, priv->pd_general);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



/********************* start CowIPanelFactory implementation *********************/

static void l_factory_attach(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowContainer *e_container) {
	ElkPreferencesPanelFactory *instance = ELK_PREFERENCES_PANEL_FACTORY(self);
	ElkPreferencesPanelFactoryPrivate *priv = elk_preferences_panel_factory_get_instance_private(instance);

	cow_dialog_add_entry(cow_dialog, NULL, self, priv->pd_general);

}

static void l_factory_detach(CowIPanelFactory *self, struct _CowDialog *cow_dialog) {

}

static CowPanel *l_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr) {
	ElkPreferencesPanelFactory *instance = ELK_PREFERENCES_PANEL_FACTORY(self);
	ElkPreferencesPanelFactoryPrivate *priv = elk_preferences_panel_factory_get_instance_private(instance);

	CowPanel *result = NULL;
	if (panel_descr==priv->pd_general) {
		result = (CowPanel *) elk_main_prefs_panel_new();
	}
	return result;
}


static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface) {
	iface->attach = l_factory_attach;
	iface->detach = l_factory_detach;
	iface->create = l_factory_create;
}

/********************* end CowIPanelFactory implementation *********************/

