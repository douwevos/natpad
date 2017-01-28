/*
   File:    moocowpropertiespanelfactory.c
   Project: moose
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#include "moocowpropertiespanelfactory.h"
#include "moocowpropertiescontainer.h"
#include "moocowpanelresource.h"
#include "../mooresourcecontentwo.h"
#include <cow.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooCowPropertiesPanelFactory"
#include <logging/catlog.h>

struct _MooCowPropertiesPanelFactoryPrivate {
	CowPanelDescription *pd_resource;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_factory_iface_init(CowIPanelFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooCowPropertiesPanelFactory, moo_cow_properties_panel_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MooCowPropertiesPanelFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_IPANEL_FACTORY, l_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_cow_properties_panel_factory_class_init(MooCowPropertiesPanelFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_cow_properties_panel_factory_init(MooCowPropertiesPanelFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooCowPropertiesPanelFactory *instance = MOO_COW_PROPERTIES_PANEL_FACTORY(object);
	MooCowPropertiesPanelFactoryPrivate *priv = moo_cow_properties_panel_factory_get_instance_private(instance);
	cat_unref_ptr(priv->pd_resource);
	G_OBJECT_CLASS(moo_cow_properties_panel_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_cow_properties_panel_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooCowPropertiesPanelFactory *moo_cow_properties_panel_factory_new() {
	MooCowPropertiesPanelFactory *result = g_object_new(MOO_TYPE_COW_PROPERTIES_PANEL_FACTORY, NULL);
	cat_ref_anounce(result);
	MooCowPropertiesPanelFactoryPrivate *priv = moo_cow_properties_panel_factory_get_instance_private(result);
	priv->pd_resource = cow_panel_description_new(cat_string_wo_new_with("resource"));
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooCowPropertiesPanelFactory *instance = MOO_COW_PROPERTIES_PANEL_FACTORY(self);
	MooCowPropertiesPanelFactoryPrivate *priv = moo_cow_properties_panel_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p] panel-descr=%o", iname, self, priv->pd_resource);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* start CowIPanelFactory implementation *********************/

static void l_factory_attach(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowContainer *e_container) {
	MooCowPropertiesPanelFactory *instance = MOO_COW_PROPERTIES_PANEL_FACTORY(self);
	MooCowPropertiesPanelFactoryPrivate *priv = moo_cow_properties_panel_factory_get_instance_private(instance);

	cat_log_debug("e_container=%o", e_container);
	MooNodeWo *moo_node = moo_cow_properties_container_get_node((MooCowPropertiesContainer *) e_container);
	cat_log_debug("moo_node=%o", moo_node);
	if (moo_node == NULL) {
		return;
	}

	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(moo_node, moo_resource_content_wo_key());
	cat_log_debug("resource_content=%o", resource_content);
	if (resource_content) {
		cow_dialog_add_entry(cow_dialog, NULL, self, priv->pd_resource);
	}
}

static void l_factory_detach(CowIPanelFactory *self, struct _CowDialog *cow_dialog) {
//	MooCowPropertiesPanelFactory *instance = MOO_COW_PROPERTIES_PANEL_FACTORY(self);
//	MooCowPropertiesPanelFactoryPrivate *priv = moo_cow_properties_panel_factory_get_instance_private(instance);
}

static CowPanel *l_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr) {
//	MooCowPropertiesPanelFactory *instance = MOO_COW_PROPERTIES_PANEL_FACTORY(self);
//	MooCowPropertiesPanelFactoryPrivate *priv = moo_cow_properties_panel_factory_get_instance_private(instance);
	MooCowPanelResource *panel = moo_cow_panel_resource_new();
	return (CowPanel *) panel;

}

static void l_factory_iface_init(CowIPanelFactoryInterface *iface) {
	iface->attach = l_factory_attach;
	iface->detach = l_factory_detach;
	iface->create = l_factory_create;
}


/********************* end CowIPanelFactory implementation *********************/
