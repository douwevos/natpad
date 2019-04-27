/*
   File:    jagindexer.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 15, 2012
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


#include "jagindexer.h"
#include "jar/jagindexerjarmap.h"
#include "context/jagindexerjremap.h"
#include "worm/jagindexrefreshrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagIndexer"
#include <logging/catlog.h>

struct _JagIndexerPrivate {
	WorService *wor_service;
	MooService *moo_service;
	WorQueue *request_queue;
};

static void l_moose_transaction_listener_iface_init(MooITransactionListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagIndexer, jag_indexer, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagIndexer)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ITRANSACTION_LISTENER, l_moose_transaction_listener_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_indexer_class_init(JagIndexerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_indexer_init(JagIndexer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagIndexer *instance = JAG_INDEXER(object);
	JagIndexerPrivate *priv = jag_indexer_get_instance_private(instance);
//	if (priv->refresh_request) {
//		jag_indexer_refresh_request_stop((JagIndexerRefreshRequest *) priv->refresh_request);
//		cat_unref_ptr(priv->refresh_request);
//	}
//	if (priv->mon_explorer_service) {
//		MonService *mon_service = mon_explorer_service_get_mon_service(priv->mon_explorer_service);
//		MonModel *mon_model = mon_service_get_model(mon_service);
//		mon_model_remove_model_listener(mon_model, MON_IMODEL_LISTENER(instance));
//		cat_unref_ptr(priv->mon_explorer_service);
//	}
//	cat_unref_ptr(priv->pending_requests);
//	cat_unref_ptr(priv->notify_queue_lock);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->request_queue);
	G_OBJECT_CLASS(jag_indexer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_indexer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagIndexer *jag_indexer_new(WorService *wor_service, MooService *moo_service) {
	JagIndexer *result = g_object_new(JAG_TYPE_INDEXER, NULL);
	cat_ref_anounce(result);
	JagIndexerPrivate *priv = jag_indexer_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);

	priv->moo_service = cat_ref_ptr(moo_service);
	priv->request_queue = wor_queue_new(priv->wor_service);
	wor_queue_set_semaphore_size(priv->request_queue, 10);	// TODO calculate the number of semaphores using the number of workers in wor_service
//	priv->notify_queue = cat_array_wo_new();
//	priv->notify_queue_lock = cat_lock_new();
//	priv->refresh_request = WOR_REQUEST(jag_indexer_refresh_request_new(result));
//	priv->pending_requests = cat_array_wo_new();
//	priv->max_pending_requests = 1;
//	MonService *mon_service = mon_explorer_service_get_mon_service(mon_explorer_service);
//	MonModel *mon_model = mon_service_get_model(mon_service);
//	mon_model_add_model_listener(mon_model, MON_IMODEL_LISTENER(result));
//	wor_service_post_request(wor_service, priv->refresh_request);
	MooTransactionDispatcher *tx_dispatcher = moo_service_get_transaction_dispatcher(moo_service);
	moo_transaction_dispatcher_add_listener(tx_dispatcher, MOO_ITRANSACTION_LISTENER(result));
	return result;
}


void jag_indexer_set_preferences(JagIndexer *indexer, JagPreferencesWo *a_jag_prefs) {
}


MooService *jag_indexer_get_moo_service(JagIndexer *indexer) {
	JagIndexerPrivate *priv = jag_indexer_get_instance_private(indexer);
	return priv->moo_service;
}


/********************* begin MooITransactionListener implementation *********************/

static void l_moose_transaction_commited(MooITransactionListener *self, struct _MooTransaction *transaction, struct _MooNodeWo *new_root) {
	JagIndexer *instance = JAG_INDEXER(self);
	JagIndexerPrivate *priv = jag_indexer_get_instance_private(instance);
	JagIndexRefreshRequest *request = jag_index_refresh_request_new(priv->moo_service, priv->request_queue);
	wor_service_post_request(priv->wor_service, (WorRequest *) request);
	cat_unref_ptr(request);
}

static void l_moose_transaction_listener_iface_init(MooITransactionListenerInterface *iface) {
	iface->transactionCommited = l_moose_transaction_commited;
}

/********************* end MooITransactionListener implementation *********************/

//
///********************* start JagIModuleMapListener implementation *********************/
//
//
//static void l_jar_file_mapped(JagIModuleMapListener *self, JagMappedJarFile *mapped_jar_file) {
//
//}
//
//
//static void l_module_map_listener_iface_init(JagIModuleMapListenerInterface *iface) {
//	iface->jarFileMapped = l_jar_file_mapped;
//}
//
//
///********************* end JagIModuleMapListener implementation *********************/
