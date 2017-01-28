/*
   File:    mulluaeditorconnector.c
   Project: mule
   Author:  Douwe Vos
   Date:    Dec 27, 2016
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

#include "mulluaeditorconnector.h"
#include "mulluascannerfactory.h"
#include "mulluaaugmentrequest.h"
#include "parser/gross/mulluaparserconfig.h"
#include <grossruntime.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulLuaEditorConnector"
#include <logging/catlog.h>

struct _MulLuaEditorConnectorPrivate {
	CatStringWo *a_slot_key;
	GroRunModel *model;
	GroRunITokenFactory *token_factory;
	MulLuaScannerFactory *scanner_factory;
};

static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulLuaEditorConnector, mul_lua_editor_connector, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulLuaEditorConnector)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_request_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_lua_editor_connector_class_init(MulLuaEditorConnectorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_lua_editor_connector_init(MulLuaEditorConnector *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulLuaEditorConnector *instance = MUL_LUA_EDITOR_CONNECTOR(object);
	MulLuaEditorConnectorPrivate *priv = mul_lua_editor_connector_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->scanner_factory);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(mul_lua_editor_connector_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_lua_editor_connector_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulLuaEditorConnector *mul_lua_editor_connector_new() {
	MulLuaEditorConnector *result = g_object_new(MUL_TYPE_LUA_EDITOR_CONNECTOR, NULL);
	cat_ref_anounce(result);
	MulLuaEditorConnectorPrivate *priv = mul_lua_editor_connector_get_instance_private(result);
	priv->a_slot_key = (CatStringWo *) cat_string_wo_new_data("Eiffel");
	priv->model = NULL;
	priv->token_factory = (GroRunITokenFactory *) grorun_full_token_factory_new();
	priv->scanner_factory = NULL;
	return result;
}


/********************* start DraIConnectorRequestFactory implementation *********************/

static DraAugmentRequest *l_create_request(DraIConnectorRequestFactory *self, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	MulLuaEditorConnector *instance = MUL_LUA_EDITOR_CONNECTOR(self);
	MulLuaEditorConnectorPrivate *priv = mul_lua_editor_connector_get_instance_private(instance);
	if (priv->model==NULL) {
		priv->model = grorun_model_new(mullua_parser_config_symbols, mullua_parser_config_nr_of_symbols, mullua_parser_config_states_text, mullua_parser_config_nr_of_states);
		priv->scanner_factory = mul_lua_scanner_factory_new(GRORUN_ISYMBOL_PROVIDER(priv->model), priv->token_factory);
	}
	return (DraAugmentRequest *) mul_lua_augment_request_new(document, a_new_revision, priv->a_slot_key, priv->model, priv->token_factory, priv->scanner_factory);
}

static CatStringWo *l_get_slot_key(DraIConnectorRequestFactory *self) {
	MulLuaEditorConnector *instance = MUL_LUA_EDITOR_CONNECTOR(self);
	MulLuaEditorConnectorPrivate *priv = mul_lua_editor_connector_get_instance_private(instance);
	return priv->a_slot_key;
}


static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface) {
	iface->createRequest = l_create_request;
	iface->getSlotKey = l_get_slot_key;
}

/********************* end DraIConnectorRequestFactory implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
