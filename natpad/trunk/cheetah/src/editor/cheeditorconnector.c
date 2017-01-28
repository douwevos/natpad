/*
   File:    cheeditorconnector.c
   Project: cheetah
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

#include "cheeditorconnector.h"
#include "cheaugmentrequest.h"
#include <gvlcodeparsing/keyWords.h>
#include "highlighting/cheExtraKeyWords.h"
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CheEditorConnector"
#include <logging/catlog.h>

struct _CheEditorConnectorPrivate {
	CatStringWo *a_slot_key;
	CatLock *lock;
};

static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CheEditorConnector, che_editor_connector, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CheEditorConnector)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_request_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void che_editor_connector_class_init(CheEditorConnectorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

    KeyWords.init(); /* To be called at start of application. */
    CheExtraKeyWords.init();

}

static void che_editor_connector_init(CheEditorConnector *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CheEditorConnector *instance = CHE_EDITOR_CONNECTOR(object);
	CheEditorConnectorPrivate *priv = che_editor_connector_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(che_editor_connector_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(che_editor_connector_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CheEditorConnector *che_editor_connector_new() {
	CheEditorConnector *result = g_object_new(CHE_TYPE_EDITOR_CONNECTOR, NULL);
	cat_ref_anounce(result);
	CheEditorConnectorPrivate *priv = che_editor_connector_get_instance_private(result);
	priv->a_slot_key = (CatStringWo *) cat_string_wo_new_data("C");
	priv->lock = cat_lock_new();
	return result;
}


/********************* start DraIConnectorRequestFactory implementation *********************/

static DraAugmentRequest *l_create_request(DraIConnectorRequestFactory *self, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	CheEditorConnector *instance = CHE_EDITOR_CONNECTOR(self);
	CheEditorConnectorPrivate *priv = che_editor_connector_get_instance_private(instance);
	return (DraAugmentRequest *) che_augment_request_new(document, a_new_revision, priv->a_slot_key, priv->lock);
}

static CatStringWo *l_get_slot_key(DraIConnectorRequestFactory *self) {
	CheEditorConnector *instance = CHE_EDITOR_CONNECTOR(self);
	CheEditorConnectorPrivate *priv = che_editor_connector_get_instance_private(instance);
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
