/*
   File:    jageditorconnector.c
   Project: jaguar
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

#include "jageditorconnector.h"
#include "jagaugmentrequest.h"
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagEditorConnector"
#include <logging/catlog.h>

struct _JagEditorConnectorPrivate {
	CatStringWo *a_slot_key;
	GroRunModel *model;
	GroRunITokenFactory *token_factory;
	JagPScannerFactory *scanner_factory;
};

static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagEditorConnector, jag_editor_connector, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagEditorConnector)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_request_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_editor_connector_class_init(JagEditorConnectorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_editor_connector_init(JagEditorConnector *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagEditorConnector *instance = JAG_EDITOR_CONNECTOR(object);
	JagEditorConnectorPrivate *priv = jag_editor_connector_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	G_OBJECT_CLASS(jag_editor_connector_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_editor_connector_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagEditorConnector *jag_editor_connector_new() {
	JagEditorConnector *result = g_object_new(JAG_TYPE_EDITOR_CONNECTOR, NULL);
	cat_ref_anounce(result);
	JagEditorConnectorPrivate *priv = jag_editor_connector_get_instance_private(result);
	priv->a_slot_key = (CatStringWo *) cat_string_wo_new_data("Jaguar");
	priv->model = NULL;
	priv->token_factory = (GroRunITokenFactory *) grorun_full_token_factory_new();
	priv->scanner_factory = NULL;
	return result;
}

/********************* start DraIConnectorRequestFactory implementation *********************/

static DraAugmentRequest *l_create_request(DraIConnectorRequestFactory *content_provider, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	JagEditorConnector *instance = JAG_EDITOR_CONNECTOR(content_provider);
	JagEditorConnectorPrivate *priv = jag_editor_connector_get_instance_private(instance);
	if (priv->model == NULL) {
		priv->model = grorun_model_new(jagp_parser_config_symbols, jagp_parser_config_nr_of_symbols, jagp_parser_config_states_text, jagp_parser_config_nr_of_states);
		priv->scanner_factory = jagp_scanner_factory_new(GRORUN_ISYMBOL_PROVIDER(priv->model), priv->token_factory);
	}
	return (DraAugmentRequest *) jag_augment_request_new(document, a_new_revision, priv->a_slot_key, priv->model, priv->token_factory, priv->scanner_factory);
}

static CatStringWo *l_get_slot_key(DraIConnectorRequestFactory *content_provider) {
	JagEditorConnector *instance = JAG_EDITOR_CONNECTOR(content_provider);
	JagEditorConnectorPrivate *priv = jag_editor_connector_get_instance_private(instance);
	return priv->a_slot_key;
}


static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface) {
	iface->createRequest = l_create_request;
	iface->getSlotKey = l_get_slot_key;
}

/********************* end DraIConnectorRequestFactory implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagEditorConnector *instance = JAG_EDITOR_CONNECTOR(self);
	JagEditorConnectorPrivate *priv = jag_editor_connector_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
