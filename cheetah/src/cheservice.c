/*
   File:    cheservice.c
   Project: cheetah
   Author:  Douwe Vos
   Date:    Feb 8, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "cheservice.h"
#include "editor/chechameleoneditorfactory.h"
#include "editor/cheeditorconnector.h"
#include <cow.h>
#include <chameleon.h>
#include <caterpillar.h>
#include <elk.h>
#include <viper.h>
#include <moose.h>
#include <armadillo.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CheService"
#include <logging/catlog.h>

struct _CheServicePrivate {
	WorService *wor_service;

	ElkService *elk_service;

	CheEditorConnector *connector;
	ArmEditorConnector *arm_connector;
};

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CheService, che_service, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(CheService)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_HANDLER, l_resource_handler_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void che_service_class_init(CheServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void che_service_init(CheService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CheService *instance = CHE_SERVICE(object);
	CheServicePrivate *priv = che_service_get_instance_private(instance);
	cat_unref_ptr(priv->elk_service);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->connector);
	cat_unref_ptr(priv->arm_connector);
	G_OBJECT_CLASS(che_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(che_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CheService *che_service_new(WorService *wor_service, ElkService *elk_service) {
	CheService *result = g_object_new(CHE_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	CheServicePrivate *priv = che_service_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->elk_service = cat_ref_ptr(elk_service);
	priv->connector = che_editor_connector_new();
	priv->arm_connector = arm_editor_connector_new();
	return result;
}


CatS che_s_dot_c = CAT_S_DEF(".c");
CatS che_s_dot_h = CAT_S_DEF(".h");
CatS che_s_dot_cpp = CAT_S_DEF(".cpp");
CatS che_s_dot_hpp = CAT_S_DEF(".hpp");

static void l_enlist_editor_factories(ElkIResourceHandler *self, CatArrayWo *e_enlist_to, MooNodeWo *node) {
	CheService *instance = CHE_SERVICE(self);
	CheServicePrivate *priv = che_service_get_instance_private(instance);
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
	if (resource_content!=NULL) {
		VipNode *node = moo_resource_content_wo_get_viper_node(resource_content);
		if (node==NULL) {
			return;
		}
		VipIFile *file_to_open = node==NULL ? NULL : (VipIFile *) vip_node_get_content(node);
		if (file_to_open!=NULL) {
			CatStringWo *a_name = vip_iresource_get_name((VipIResource *) file_to_open);
			cat_log_debug("name:%p", a_name);
			gboolean is_a_c_file = (cat_string_wo_endswith(a_name, CAT_S(che_s_dot_c))) ||
					(cat_string_wo_endswith(a_name, CAT_S(che_s_dot_h)));

/*			if (is_a_c_file) {
				ElkDocumentIO *document_io = priv->elk_service->document_io;
				ElkService *elk_service = (ElkService *) priv->elk_service;
				LeaIPanelOwner *panel_owner = (LeaIPanelOwner *) elk_service->panel_owner;

				ArmEditorFactory *editor_factory = arm_editor_factory_new(panel_owner, priv->arm_connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) editor_factory);
				cat_unref(editor_factory);
			} */

			gboolean is_a_cpp_file = ((cat_string_wo_endswith(a_name, CAT_S(che_s_dot_cpp))) ||
					(cat_string_wo_endswith(a_name, CAT_S(che_s_dot_hpp))));
			if (is_a_cpp_file || is_a_c_file) {
				ElkDocumentIO *document_io = priv->elk_service->document_io;
				ElkService *elk_service = (ElkService *) priv->elk_service;
				LeaIPanelOwner *panel_owner = (LeaIPanelOwner *) elk_service->panel_owner;

				CheChameleonEditorFactory *editor_factory = che_chameleon_editor_factory_new(panel_owner, (DraIConnectorRequestFactory *) priv->connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) editor_factory);
				cat_unref(editor_factory);
			}
		}
	}
}

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface) {
	iface->enlistEditorFactories = l_enlist_editor_factories;
}
