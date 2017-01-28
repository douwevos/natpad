/*
   File:    jagprefspanelfactory.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 28, 2014
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

#include "jagprefspanelfactory.h"
#include "jagprefscompilerpanel.h"
#include <cow.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPrefsPanelFactory"
#include <logging/catlog.h>

struct _JagPrefsPanelFactoryPrivate {
	VipService *vip_service;
	CowPanelDescription *pd_java;
	CowPanelDescription *pd_compiler;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPrefsPanelFactory, jag_prefs_panel_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPrefsPanelFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_IPANEL_FACTORY, l_panel_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_prefs_panel_factory_class_init(JagPrefsPanelFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_prefs_panel_factory_init(JagPrefsPanelFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPrefsPanelFactory *instance = JAG_PREFS_PANEL_FACTORY(object);
	JagPrefsPanelFactoryPrivate *priv = jag_prefs_panel_factory_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->pd_compiler);
	cat_unref_ptr(priv->pd_java);
	G_OBJECT_CLASS(jag_prefs_panel_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_prefs_panel_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPrefsPanelFactory *jag_prefs_panel_factory_new(VipService *vip_service) {
	JagPrefsPanelFactory *result = g_object_new(JAG_TYPE_PREFS_PANEL_FACTORY, NULL);
	cat_ref_anounce(result);
	JagPrefsPanelFactoryPrivate *priv = jag_prefs_panel_factory_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->pd_java = cow_panel_description_new(cat_string_wo_new_with("Java"));
	priv->pd_compiler = cow_panel_description_new(cat_string_wo_new_with("Compiler"));
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPrefsPanelFactory *instance = JAG_PREFS_PANEL_FACTORY(self);
	JagPrefsPanelFactoryPrivate *priv = jag_prefs_panel_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p pd_java=%o]", iname, self, priv->pd_java);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



/********************* start CowIPanelFactory implementation *********************/

static void l_factory_attach(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowContainer *e_container) {
	JagPrefsPanelFactory *instance = JAG_PREFS_PANEL_FACTORY(self);
	JagPrefsPanelFactoryPrivate *priv = jag_prefs_panel_factory_get_instance_private(instance);

	GtkTreeIter iter_java = cow_dialog_add_entry(cow_dialog, NULL, self, priv->pd_java);
	cow_dialog_add_entry(cow_dialog, &iter_java, self, priv->pd_compiler);
}

static void l_factory_detach(CowIPanelFactory *self, struct _CowDialog *cow_dialog) {

}

static CowPanel *l_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr) {
	JagPrefsPanelFactory *instance = JAG_PREFS_PANEL_FACTORY(self);
	JagPrefsPanelFactoryPrivate *priv = jag_prefs_panel_factory_get_instance_private(instance);

	CowPanel *result = NULL;
	if (panel_descr==priv->pd_compiler) {
		result = (CowPanel *) jag_prefs_compiler_panel_new(priv->vip_service);
	}
	return result;
}


static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface) {
	iface->attach = l_factory_attach;
	iface->detach = l_factory_detach;
	iface->create = l_factory_create;
}

/********************* end CowIPanelFactory implementation *********************/
