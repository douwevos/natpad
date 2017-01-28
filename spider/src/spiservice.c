/*
   File:    spiservice.c
   Project: spider
   Author:  Douwe Vos
   Date:    Oct 22, 2015
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

#include "spiservice.h"
#include "editor/spieditorfactory.h"
#include "editor/spieditorconnector.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SpiService"
#include <logging/catlog.h>

struct _SpiServicePrivate {
	WorService *wor_service;
	ElkService *elk_service;
	SpiEditorConnector *connector;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(SpiService, spi_service, G_TYPE_OBJECT,
		G_ADD_PRIVATE(SpiService)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_HANDLER, l_resource_handler_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void spi_service_class_init(SpiServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void spi_service_init(SpiService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	SpiService *instance = SPI_SERVICE(object);
	SpiServicePrivate *priv = spi_service_get_instance_private(instance);
	cat_unref_ptr(priv->elk_service);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->connector);
	G_OBJECT_CLASS(spi_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(spi_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


SpiService *spi_service_new(WorService *wor_service, ElkService *elk_service) {
	SpiService *result = g_object_new(SPI_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	SpiServicePrivate *priv = spi_service_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->elk_service = cat_ref_ptr(elk_service);
	priv->connector = spi_editor_connector_new();
	return result;
}


/********************* start ElkIResourceHandler implementation *********************/

CatS spi_s_ext_js = CAT_S_DEF(".js");


static void l_enlist_editor_factories(ElkIResourceHandler *self, CatArrayWo *e_enlist_to, MooNodeWo *node) {
	SpiServicePrivate *priv = spi_service_get_instance_private(SPI_SERVICE(self));
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
	if (resource_content!=NULL) {
		VipNode *node = moo_resource_content_wo_get_viper_node(resource_content);
		VipIFile *file_to_open = node==NULL ? NULL : (VipIFile *) vip_node_get_content(node);
		if (file_to_open!=NULL) {
			CatStringWo *a_name = vip_iresource_get_name((VipIResource *) file_to_open);

			if (cat_string_wo_endswith(a_name, CAT_S(spi_s_ext_js))) {
				ElkService *elk_service = (ElkService *) priv->elk_service;
				ElkDocumentIO *document_io = priv->elk_service->document_io;
				ElkPreferencesService *prefs_service = elk_service_get_preferences_service(elk_service);
				SpiEditorFactory *factory = spi_editor_factory_new((LeaIPanelOwner *) elk_service->panel_owner, priv->connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			}
		}
	}
}

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface) {
	iface->enlistEditorFactories = l_enlist_editor_factories;
}

/********************* end ElkIResourceHandler implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
