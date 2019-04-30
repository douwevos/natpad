/*
   File:    drabasicaugmentor.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 12, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#include "drabasicaugmentor.h"
#include "draiconnectorrequestfactory.h"
#include "drabasicaugmentrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraBasicAugmentor"
#include <logging/catlog.h>

struct _DraBasicAugmentorPrivate {
	CatStringWo *a_slot_key;
};

static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraBasicAugmentor, dra_basic_augmentor, G_TYPE_OBJECT,
	G_ADD_PRIVATE(DraBasicAugmentor)
	G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_request_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_basic_augmentor_class_init(DraBasicAugmentorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_basic_augmentor_init(DraBasicAugmentor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraBasicAugmentor *instance = DRA_BASIC_AUGMENTOR(object);
	DraBasicAugmentorPrivate *priv = dra_basic_augmentor_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	G_OBJECT_CLASS(dra_basic_augmentor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_basic_augmentor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraBasicAugmentor *dra_basic_augmentor_new() {
	DraBasicAugmentor *result = g_object_new(DRA_TYPE_BASIC_AUGMENTOR, NULL);
	cat_ref_anounce(result);
	DraBasicAugmentorPrivate *priv = dra_basic_augmentor_get_instance_private(result);
	priv->a_slot_key = (CatStringWo *) cat_string_wo_new_data("Dragonfly");
	return result;
}


/********************* start DraIConnectorRequestFactory implementation *********************/

static DraAugmentRequest *l_create_request(DraIConnectorRequestFactory *content_provider, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	DraBasicAugmentor *instance = DRA_BASIC_AUGMENTOR(content_provider);
	DraBasicAugmentorPrivate *priv = dra_basic_augmentor_get_instance_private(instance);
	return (DraAugmentRequest *) dra_basic_augment_request_new(document, a_new_revision, priv->a_slot_key);
}

static CatStringWo *l_get_slot_key(DraIConnectorRequestFactory *content_provider) {
	DraBasicAugmentor *instance = DRA_BASIC_AUGMENTOR(content_provider);
	DraBasicAugmentorPrivate *priv = dra_basic_augmentor_get_instance_private(instance);
	return priv->a_slot_key;
}


static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface) {
	iface->createRequest = l_create_request;
	iface->getSlotKey = l_get_slot_key;
}

/********************* end DraIConnectorRequestFactory implementation *********************/
