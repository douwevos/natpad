/*
   File:    jagmodulepanelfactory.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 31, 2014
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

#include "jagmodulepanelfactory.h"
#include "jagsrcfolderpanel.h"
#include "jaglibrariespanel.h"
#include "jagcompilerpanel.h"
#include "../../moose/module/jagmodulecontentwo.h"
#include <cow.h>
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagModulePanelFactory"
#include <logging/catlog.h>

struct _JagModulePanelFactoryPrivate {
	CowPanelDescription *pd_java;
	CowPanelDescription *pd_src_folders;
	CowPanelDescription *pd_libs;
	CowPanelDescription *pd_jdk;
	JagPreferencesWo *a_jag_prefs;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagModulePanelFactory, jag_module_panel_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagModulePanelFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_IPANEL_FACTORY, l_panel_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_module_panel_factory_class_init(JagModulePanelFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_module_panel_factory_init(JagModulePanelFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagModulePanelFactory *instance = JAG_MODULE_PANEL_FACTORY(object);
	JagModulePanelFactoryPrivate *priv = jag_module_panel_factory_get_instance_private(instance);
	cat_unref_ptr(priv->a_jag_prefs);
	cat_unref_ptr(priv->pd_java);
	cat_unref_ptr(priv->pd_jdk);
	cat_unref_ptr(priv->pd_libs);
	cat_unref_ptr(priv->pd_src_folders);
	G_OBJECT_CLASS(jag_module_panel_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_module_panel_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagModulePanelFactory *jag_module_panel_factory_new() {
	JagModulePanelFactory *result = g_object_new(JAG_TYPE_MODULE_PANEL_FACTORY, NULL);
	cat_ref_anounce(result);
	JagModulePanelFactoryPrivate *priv = jag_module_panel_factory_get_instance_private(result);
	priv->pd_java = cow_panel_description_new(cat_string_wo_new_with("Java"));
	priv->pd_src_folders = cow_panel_description_new(cat_string_wo_new_with("Source folders"));
	priv->pd_libs = cow_panel_description_new(cat_string_wo_new_with("Libraries"));
	priv->pd_jdk = cow_panel_description_new(cat_string_wo_new_with("Jdk/Jre"));
	priv->a_jag_prefs = NULL;
	return result;
}

void jag_module_panel_factory_set_preferences(JagModulePanelFactory *factory, JagPreferencesWo *a_jag_prefs) {
	JagModulePanelFactoryPrivate *priv = jag_module_panel_factory_get_instance_private(factory);
	cat_ref_swap(priv->a_jag_prefs, a_jag_prefs);
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


/********************* start CowIPanelFactory implementation *********************/

static void l_factory_attach(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowContainer *e_container) {
	JagModulePanelFactory *instance = JAG_MODULE_PANEL_FACTORY(self);
	JagModulePanelFactoryPrivate *priv = jag_module_panel_factory_get_instance_private(instance);

	cat_log_debug("e_container=%o", e_container);
	MooNodeWo *moo_node = moo_cow_properties_container_get_node((MooCowPropertiesContainer *) e_container);
	cat_log_debug("moo_node=%o", moo_node);
	if (moo_node == NULL) {
		return;
	}
	MooIContent *jag_mod_content = moo_node_wo_get_content(moo_node, jag_module_content_wo_key());
	if (jag_mod_content) {
		GtkTreeIter java_iter = cow_dialog_add_entry(cow_dialog, NULL, self, priv->pd_java);
		cow_dialog_add_entry(cow_dialog, &java_iter, self, priv->pd_src_folders);
		cow_dialog_add_entry(cow_dialog, &java_iter, self, priv->pd_libs);
		cow_dialog_add_entry(cow_dialog, &java_iter, self, priv->pd_jdk);
	}

}

static void l_factory_detach(CowIPanelFactory *self, struct _CowDialog *cow_dialog) {

}

static CowPanel *l_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr) {
	JagModulePanelFactory *instance = JAG_MODULE_PANEL_FACTORY(self);
	JagModulePanelFactoryPrivate *priv = jag_module_panel_factory_get_instance_private(instance);

	CowPanel *result = NULL;
	if (panel_descr==priv->pd_src_folders) {
		result = (CowPanel *) jag_src_folder_panel_new();
	} else if (panel_descr==priv->pd_libs) {
		result = (CowPanel *) jag_libraries_panel_new();
	} else if (panel_descr==priv->pd_jdk) {
		result = (CowPanel *) jag_compiler_panel_new(priv->a_jag_prefs);
	}
	return result;
}


static void l_panel_factory_iface_init(CowIPanelFactoryInterface *iface) {
	iface->attach = l_factory_attach;
	iface->detach = l_factory_detach;
	iface->create = l_factory_create;
}


/********************* end CowIPanelFactory implementation *********************/

