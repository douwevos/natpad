/*
   File:    dradocumentconnector.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
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

#include "dradocumentconnector.h"
#include "draaugmentrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraDocumentConnector"
#include <logging/catlog.h>

struct _DraDocumentConnectorPrivate {
	WorService *wor_service;
	DraSpellHelper *spell_helper;
	ChaDocument *document;
	CatStringWo *a_slot_key;
	DraIConnectorRequestFactory *factory;
	int usage_count;
	int last_content_version;
};

static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraDocumentConnector, dra_document_connector, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraDocumentConnector)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, l_document_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_document_connector_class_init(DraDocumentConnectorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_document_connector_init(DraDocumentConnector *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraDocumentConnector *instance = DRA_DOCUMENT_CONNECTOR(object);
//	DraDocumentConnectorPrivate *priv = dra_document_connector_get_instance_private(instance);
	G_OBJECT_CLASS(dra_document_connector_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_document_connector_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraDocumentConnector *dra_document_connector_new(WorService *wor_service, DraSpellHelper *spell_helper, ChaDocument *document, CatStringWo *a_slot_key, DraIConnectorRequestFactory *factory) {
	DraDocumentConnector *result = g_object_new(DRA_TYPE_DOCUMENT_CONNECTOR, NULL);
	cat_ref_anounce(result);
	DraDocumentConnectorPrivate *priv = dra_document_connector_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->spell_helper = cat_ref_ptr(spell_helper);
	priv->document = cat_ref_ptr(document);
	priv->a_slot_key = cat_ref_ptr(a_slot_key);
	priv->factory = cat_ref_ptr(factory);
	priv->usage_count = 1;
	priv->last_content_version = 0;
	cha_document_add_listener(document, CHA_IDOCUMENT_LISTENER(result));
	cha_document_register_enrichment_slot(document, (GObject *) priv->a_slot_key);
	return result;
}

void dra_document_connector_set_spell_helper(DraDocumentConnector *connector, DraSpellHelper *new_spell_helper) {
	DraDocumentConnectorPrivate *priv = dra_document_connector_get_instance_private(connector);
	cat_ref_swap(priv->spell_helper, new_spell_helper);
}


void dra_document_connector_increase_usage(DraDocumentConnector *connector) {
	DraDocumentConnectorPrivate *priv = dra_document_connector_get_instance_private(connector);
	priv->usage_count++;
}

gboolean dra_document_connector_decrease_usage(DraDocumentConnector *connector) {
	DraDocumentConnectorPrivate *priv = dra_document_connector_get_instance_private(connector);
	priv->usage_count--;
	return priv->usage_count==0;
}


/********************* start ChaIDocumentListener implementation *********************/

static void l_document_new_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	DraDocumentConnector *instance = DRA_DOCUMENT_CONNECTOR(self);
	DraDocumentConnectorPrivate *priv = dra_document_connector_get_instance_private(instance);
	if (!cha_document_is_big_file_mode(priv->document)) {
		int content_version = cha_revision_wo_get_page_list_version(a_new_revision);
		if (priv->last_content_version<content_version) {
			priv->last_content_version = content_version;
			DraIConnectorRequestFactoryInterface *iface = DRA_ICONNECTOR_REQUEST_FACTORY_GET_INTERFACE(priv->factory);
			DraAugmentRequest *request = iface->createRequest(priv->factory, priv->document, a_new_revision);
			dra_augment_request_set_spell_helper(request, priv->spell_helper);
			wor_service_post_request(priv->wor_service, WOR_REQUEST(request));
			cat_unref_ptr(request);
		}
	}
}

static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	iface->onNewRevision = l_document_new_revision;
}

/********************* end ChaIDocumentListener implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
