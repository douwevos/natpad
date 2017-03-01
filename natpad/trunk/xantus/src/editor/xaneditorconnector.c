/*
   File:    xaneditorconnector.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "xaneditorconnector.h"
#include "xanaugmentrequest.h"
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "XanEditorConnector"
#include <logging/catlog.h>

struct _XanEditorConnectorPrivate {
	CatStringWo *a_slot_key;
	XanScannerFactory *scanner_factory;
};

static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(XanEditorConnector, xan_editor_connector, G_TYPE_OBJECT,
		G_ADD_PRIVATE(XanEditorConnector)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_request_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void xan_editor_connector_class_init(XanEditorConnectorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void xan_editor_connector_init(XanEditorConnector *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	XanEditorConnector *instance = XAN_EDITOR_CONNECTOR(object);
	XanEditorConnectorPrivate *priv = xan_editor_connector_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	G_OBJECT_CLASS(xan_editor_connector_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_editor_connector_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


XanEditorConnector *xan_editor_connector_new() {
	XanEditorConnector *result = g_object_new(XAN_TYPE_EDITOR_CONNECTOR, NULL);
	cat_ref_anounce(result);
	XanEditorConnectorPrivate *priv = xan_editor_connector_get_instance_private(result);
	priv->a_slot_key = (CatStringWo *) cat_string_wo_new_data("Xantus");
	priv->scanner_factory = xan_scanner_factory_new();
	return result;
}

/********************* start DraIConnectorRequestFactory implementation *********************/

static DraAugmentRequest *l_create_request(DraIConnectorRequestFactory *self, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	XanEditorConnector *instance = XAN_EDITOR_CONNECTOR(self);
	XanEditorConnectorPrivate *priv = xan_editor_connector_get_instance_private(instance);
	return (DraAugmentRequest *) xan_augment_request_new(document, a_new_revision, priv->a_slot_key, priv->scanner_factory);
}

static CatStringWo *l_get_slot_key(DraIConnectorRequestFactory *self) {
	XanEditorConnector *instance = XAN_EDITOR_CONNECTOR(self);
	XanEditorConnectorPrivate *priv = xan_editor_connector_get_instance_private(instance);
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
