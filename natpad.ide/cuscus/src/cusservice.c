/*
   File:    cusservice.c
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 6, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "cusservice.h"
#include "editor/cuseditorfactory.h"
#include "editor/cushighlightermap.h"
#include "editor/cuseditorconnector.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CusService"
#include <logging/catlog.h>

struct _CusServicePrivate {
	WorService *wor_service;
	ElkService *elk_service;
	DraHighlighterMap *highlighter_map;
	CusEditorConnector *connector;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CusService, cus_service, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CusService)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_HANDLER, l_resource_handler_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cus_service_class_init(CusServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cus_service_init(CusService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CusService *instance = CUS_SERVICE(object);
	CusServicePrivate *priv = cus_service_get_instance_private(instance);
	cat_unref_ptr(priv->elk_service);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->highlighter_map);
	cat_unref_ptr(priv->connector);
	G_OBJECT_CLASS(cus_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cus_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CusService *cus_service_new(WorService *wor_service, ElkService *elk_service) {
	CusService *result = g_object_new(CUS_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	CusServicePrivate *priv = cus_service_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->elk_service = cat_ref_ptr(elk_service);
	priv->highlighter_map = (DraHighlighterMap *) cus_highlighter_map_new(wor_service);
	priv->connector = cus_editor_connector_new();
	return result;
}


/********************* start ElkIResourceHandler implementation *********************/

CatS cus_s_ext_cup = CAT_S_DEF(".cup");


static void l_enlist_editor_factories(ElkIResourceHandler *self, CatArrayWo *e_enlist_to, MooNodeWo *node) {
	CusServicePrivate *priv = cus_service_get_instance_private(CUS_SERVICE(self));
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
	if (resource_content!=NULL) {
		VipNode *node = moo_resource_content_wo_get_viper_node(resource_content);
		VipIFile *file_to_open = node==NULL ? NULL : (VipIFile *) vip_node_get_content(node);
		if (file_to_open!=NULL) {
			CatStringWo *a_name = vip_iresource_get_name((VipIResource *) file_to_open);

			if (cat_string_wo_endswith(a_name, CAT_S(cus_s_ext_cup))) {
				ElkService *elk_service = (ElkService *) priv->elk_service;
				ElkDocumentIO *document_io = priv->elk_service->document_io;
				ElkPreferencesService *prefs_service = elk_service_get_preferences_service(elk_service);
				CowIEntryAccessor *prefs_accessor = elk_preferences_services_get_chameleon_prefs_accessor(prefs_service);
				CusEditorFactory *factory = cus_editor_factory_new((LeaIPanelOwner *) elk_service->panel_owner, (DraHighlighterMap *) priv->highlighter_map, document_io, file_to_open);
				cus_editor_factory_set_chameleon_prefs_accessor(factory, prefs_accessor);
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
	CusService *instance = CUS_SERVICE(self);
	CusServicePrivate *priv = cus_service_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
