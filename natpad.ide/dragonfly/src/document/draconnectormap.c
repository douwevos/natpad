/*
   File:    draconnectormap.c
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

#include "draconnectormap.h"
#include "../spell/draspellhelper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraConnectorMap"
#include <logging/catlog.h>

struct _DraConnectorMapPrivate {
	CatLock *lock;
	CatHashMapWo *map;
	WorService *wor_service;
	DraSpellHelper *spell_helper;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraConnectorMap, dra_connector_map, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraConnectorMap)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_connector_map_class_init(DraConnectorMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_connector_map_init(DraConnectorMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraConnectorMap *instance = DRA_CONNECTOR_MAP(object);
	DraConnectorMapPrivate *priv = dra_connector_map_get_instance_private(instance);
	cat_unref_ptr(priv->map);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->spell_helper);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(dra_connector_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_connector_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraConnectorMap *dra_connector_map_new(WorService *wor_service) {
	DraConnectorMap *result = g_object_new(DRA_TYPE_CONNECTOR_MAP, NULL);
	cat_ref_anounce(result);
	DraConnectorMapPrivate *priv = dra_connector_map_get_instance_private(result);
	priv->lock = cat_lock_new();
	priv->map = cat_hash_map_wo_new((GHashFunc) dra_connector_key_hash, (GEqualFunc) dra_connector_key_equal);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->spell_helper = dra_spell_helper_new();
	return result;
}

DraSpellHelper *dra_connector_map_get_spell_helper(DraConnectorMap *connector_map) {
	DraConnectorMapPrivate *priv = dra_connector_map_get_instance_private(connector_map);
	cat_lock_lock(priv->lock);
	DraSpellHelper *result = priv->spell_helper;
	cat_lock_unlock(priv->lock);
	return result;
}

void dra_connector_map_reconfigure(DraConnectorMap *connector_map, DraPreferencesWo *a_prefs) {
	DraConnectorMapPrivate *priv = dra_connector_map_get_instance_private(connector_map);
	DraSpellHelper *new_spell_helper = dra_spell_helper_reconfigure(priv->spell_helper, a_prefs);
	if (priv->spell_helper!=new_spell_helper) {
		cat_lock_lock(priv->lock);
		cat_ref_swap(priv->spell_helper, new_spell_helper);
		CatArrayWo *conn_docs = cat_hash_map_wo_enlist_values(priv->map, NULL);
		CatIIterator *iter = cat_array_wo_iterator(conn_docs);
		cat_unref_ptr(conn_docs);
		while(cat_iiterator_has_next(iter)) {
			DraDocumentConnector *doc_conn = (DraDocumentConnector *) cat_iiterator_next(iter);
			dra_document_connector_set_spell_helper(doc_conn, new_spell_helper);
		}
		cat_unref_ptr(iter);
		cat_lock_unlock(priv->lock);
	}
	cat_unref_ptr(new_spell_helper);
}

void dra_connector_map_connect_document(DraConnectorMap *map, ChaDocument *document, CatStringWo *a_slot_key, DraIConnectorRequestFactory *factory) {
	DraConnectorMapPrivate *priv = dra_connector_map_get_instance_private(map);
	DraConnectorKey *connector_key = dra_connector_key_new(document, a_slot_key);
	cat_lock_lock(priv->lock);

	DraDocumentConnector *connector = (DraDocumentConnector *) cat_hash_map_wo_get(priv->map, connector_key);
	if (connector) {
		dra_document_connector_increase_usage(connector);
	} else {
		connector = dra_document_connector_new(priv->wor_service, priv->spell_helper, document, a_slot_key, factory);
		cat_hash_map_wo_put(priv->map, (GObject *) connector_key, (GObject *) connector);
		cat_unref(connector);
	}
	cat_lock_unlock(priv->lock);
	cat_unref_ptr(connector_key);
}


void dra_connector_map_disconnect_document(DraConnectorMap *map, ChaDocument *document, CatStringWo *a_slot_key) {
	DraConnectorMapPrivate *priv = dra_connector_map_get_instance_private(map);
	DraConnectorKey *connector_key = dra_connector_key_new(document, a_slot_key);
	cat_lock_lock(priv->lock);
	DraDocumentConnector *connector = (DraDocumentConnector *) cat_hash_map_wo_get(priv->map, (GObject *) connector_key);
	if (connector!=NULL) {
		if (dra_document_connector_decrease_usage(connector)) {
			connector = (DraDocumentConnector *) cat_hash_map_wo_remove(priv->map, (GObject *) connector_key);
			cat_unref_ptr(connector);
		}
	}
	cat_lock_unlock(priv->lock);
	cat_unref_ptr(connector_key);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
