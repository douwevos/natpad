/*
   File:    mulservice.c
   Project: mule
   Author:  Douwe Vos
   Date:    Mar 21, 2012
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

#include "mulservice.h"
#include "multype.h"
#include "vhdl/mulvhdleditorconnector.h"
#include "vhdl/mulvhdleditorfactory.h"
#include "python/mulpyteditorconnector.h"
#include "python/mulpyteditorfactory.h"
#include "lua/mulluaeditorconnector.h"
#include "lua/mulluaeditorfactory.h"
#include "eiffel/muleifeditorconnector.h"
#include "eiffel/muleifeditorfactory.h"
#include "php/mulphpeditorconnector.h"
#include "php/mulphpeditorfactory.h"
#include "rust/mulrusteditorconnector.h"
#include "rust/mulrusteditorfactory.h"
#include <caterpillar.h>
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulService"
#include <logging/catlog.h>

struct _MulServicePrivate {
	WorService *wor_service;
	ElkService *elk_service;

	MulLuaEditorConnector *lua_connector;
	MulPytEditorConnector *python_connector;
	MulVhdlEditorConnector *vhdl_connector;
	MulEifEditorConnector *eiffel_connector;
	MulPhpEditorConnector *php_connector;
	MulRustEditorConnector *rust_connector;
};

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulService, mul_service, G_TYPE_OBJECT,{
		G_ADD_PRIVATE(MulService)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_HANDLER, l_resource_handler_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_service_class_init(MulServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

//	GObjectClass *sub_clazz = G_OBJECT_CLASS(clazz);
}

static void mul_service_init(MulService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulService *instance = MUL_SERVICE(object);
	MulServicePrivate *priv = mul_service_get_instance_private(instance);
	cat_unref_ptr(priv->php_connector);
	cat_unref_ptr(priv->eiffel_connector);
	cat_unref_ptr(priv->vhdl_connector);
	cat_unref_ptr(priv->python_connector);
	cat_unref_ptr(priv->lua_connector);
	cat_unref_ptr(priv->rust_connector);
	cat_unref_ptr(priv->elk_service);
	cat_unref_ptr(priv->wor_service);

	G_OBJECT_CLASS(mul_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MulService *mul_service_new(WorService *wor_service, ElkService *elk_service) {
	MulService *result = g_object_new(MUL_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	MulServicePrivate *priv = mul_service_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->elk_service = cat_ref_ptr(elk_service);

	priv->lua_connector = mul_lua_editor_connector_new();
	priv->python_connector = mul_pyt_editor_connector_new();
	priv->vhdl_connector = mul_vhdl_editor_connector_new();
	priv->eiffel_connector = mul_eif_editor_connector_new();
	priv->php_connector = mul_php_editor_connector_new();
	priv->rust_connector = mul_rust_editor_connector_new();
	return result;
}

/********************* start ElkIResourceHandler implementation *********************/

CatS mul_s_ext_lua = CAT_S_DEF(".lua");
CatS mul_s_ext_python = CAT_S_DEF(".py");
CatS mul_s_ext_vhdl = CAT_S_DEF(".vhdl");
CatS mul_s_ext_e = CAT_S_DEF(".e");
CatS mul_s_ext_javascript = CAT_S_DEF(".js");
CatS mul_s_ext_php = CAT_S_DEF(".php");
CatS mul_s_ext_rust = CAT_S_DEF(".rs");

static CatStringWo *l_find_type_of_parser(ElkIResourceHandler *self, CatStringWo *a_filename) {
	CatStringWo *a_result = NULL;
	if (cat_string_wo_endswith(a_filename, CAT_S(mul_s_ext_lua))) {
		a_result = CAT_S(mul_s_type_lua);
	} else 	if (cat_string_wo_endswith(a_filename, CAT_S(mul_s_ext_python))) {
		a_result = CAT_S(mul_s_type_python);
	} else 	if (cat_string_wo_endswith(a_filename, CAT_S(mul_s_ext_vhdl))) {
		a_result = CAT_S(mul_s_type_vhdl);
	} else 	if (cat_string_wo_endswith(a_filename, CAT_S(mul_s_ext_e))) {
		a_result = CAT_S(mul_s_type_eiffel);
	} else 	if (cat_string_wo_endswith(a_filename, CAT_S(mul_s_ext_javascript))) {
		a_result = CAT_S(mul_s_type_javascript);
	} else 	if (cat_string_wo_endswith(a_filename, CAT_S(mul_s_ext_php))) {
		a_result = CAT_S(mul_s_type_php);
	} else 	if (cat_string_wo_endswith(a_filename, CAT_S(mul_s_ext_rust))) {
		a_result = CAT_S(mul_s_type_rust);
	}
	return a_result;
}

static void l_enlist_editor_factories(ElkIResourceHandler *self, CatArrayWo *e_enlist_to, MooNodeWo *node) {
	MulService *instance = MUL_SERVICE(self);
	MulServicePrivate *priv = mul_service_get_instance_private(instance);
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
	if (resource_content!=NULL) {
		VipNode *node = moo_resource_content_wo_get_viper_node(resource_content);
		VipIFile *file_to_open = node==NULL ? NULL : (VipIFile *) vip_node_get_content(node);
		if (file_to_open!=NULL) {
			CatStringWo *a_name = vip_iresource_get_name((VipIResource *) file_to_open);
			CatStringWo *a_mul_top = l_find_type_of_parser(self, a_name);

			ElkService *service = (ElkService *) priv->elk_service;
			ElkDocumentIO *document_io = priv->elk_service->document_io;

			if (a_mul_top == CAT_S(mul_s_type_lua)) {
				MulLuaEditorFactory *factory = mul_lua_editor_factory_new((LeaIPanelOwner *) service->panel_owner, priv->lua_connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			} else 	if (a_mul_top == CAT_S(mul_s_type_python)) {
				MulPytEditorFactory *factory = mul_pyt_editor_factory_new((LeaIPanelOwner *) service->panel_owner, priv->python_connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			} else 	if (a_mul_top == CAT_S(mul_s_type_vhdl)) {
				MulVhdlEditorFactory *factory = mul_vhdl_editor_factory_new((LeaIPanelOwner *) service->panel_owner, priv->vhdl_connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			} else 	if (a_mul_top == CAT_S(mul_s_type_eiffel)) {
				MulEifEditorFactory *factory = mul_eif_editor_factory_new((LeaIPanelOwner *) service->panel_owner, priv->eiffel_connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			} else 	if (a_mul_top == CAT_S(mul_s_type_php)) {
				MulPhpEditorFactory *factory = mul_php_editor_factory_new((LeaIPanelOwner *) service->panel_owner, priv->php_connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			} else 	if (a_mul_top == CAT_S(mul_s_type_rust)) {
				MulRustEditorFactory *factory = mul_rust_editor_factory_new((LeaIPanelOwner *) service->panel_owner, priv->rust_connector, document_io, file_to_open);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			}
		}
	}
}

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface) {
	iface->matchEditorFactories = l_enlist_editor_factories;
}

/********************* end ElkIResourceHandler implementation *********************/
