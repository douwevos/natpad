/*
   File:    cuseditorconnector.c
   Project: cuscus
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

#include "cuseditorconnector.h"
#include "cusaugmentrequest.h"
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CusEditorConnector"
#include <logging/catlog.h>

struct _CusEditorConnectorPrivate {
	CatStringWo *a_slot_key;
};

static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CusEditorConnector, cus_editor_connector, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CusEditorConnector)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_request_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cus_editor_connector_class_init(CusEditorConnectorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cus_editor_connector_init(CusEditorConnector *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CusEditorConnector *instance = CUS_EDITOR_CONNECTOR(object);
	CusEditorConnectorPrivate *priv = cus_editor_connector_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	G_OBJECT_CLASS(cus_editor_connector_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cus_editor_connector_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CusEditorConnector *cus_editor_connector_new() {
	CusEditorConnector *result = g_object_new(CUS_TYPE_EDITOR_CONNECTOR, NULL);
	cat_ref_anounce(result);
	CusEditorConnectorPrivate *priv = cus_editor_connector_get_instance_private(result);
	priv->a_slot_key = (CatStringWo *) cat_string_wo_new_data("Cup");
	return result;
}



/********************* start DraIConnectorRequestFactory implementation *********************/

static WorRequest *l_create_request(DraIConnectorRequestFactory *self, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	CusEditorConnector *instance = CUS_EDITOR_CONNECTOR(self);
	CusEditorConnectorPrivate *priv = cus_editor_connector_get_instance_private(instance);
	return cus_augment_request_new(document, a_new_revision, priv->a_slot_key);
}

static CatStringWo *l_get_slot_key(DraIConnectorRequestFactory *self) {
	CusEditorConnector *instance = CUS_EDITOR_CONNECTOR(self);
	CusEditorConnectorPrivate *priv = cus_editor_connector_get_instance_private(instance);
	return priv->a_slot_key;
}


static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface) {
	iface->createRequest = l_create_request;
	iface->getSlotKey = l_get_slot_key;
}

/********************* end DraIConnectorRequestFactory implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CusEditorConnector *instance = CUS_EDITOR_CONNECTOR(self);
	CusEditorConnectorPrivate *priv = cus_editor_connector_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
