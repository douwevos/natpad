/*
   File:    jagmooseservice.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 29, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "jagmooseservice.h"
#include "jagmooapplytransactionchangesrequest.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagMooseService"
#include <logging/catlog.h>

struct _JagMooseServicePrivate {
	WorService *wor_service;
	MooService *moo_service;
	JagIndexerJreMap *jre_map;
	JagIndexerJarMap *jar_map;
};

static void l_moose_tx_listener_iface_init(MooITransactionListenerInterface *iface);
static void l_transaction_commited(MooITransactionListener *self, struct _MooTransaction *transaction, struct _MooNodeWo *new_root);

G_DEFINE_TYPE_WITH_CODE(JagMooseService, jag_moose_service, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ITRANSACTION_LISTENER, l_moose_tx_listener_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_moose_service_class_init(JagMooseServiceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagMooseServicePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

}

static void jag_moose_service_init(JagMooseService *instance) {
	JagMooseServicePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_MOOSE_SERVICE, JagMooseServicePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagMooseService *instance = JAG_MOOSE_SERVICE(object);
	JagMooseServicePrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->jre_map);
	cat_unref_ptr(priv->jar_map);
	cat_unref_ptr(priv->wor_service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagMooseService *jag_moose_service_new(MooService *moo_service, JagIndexerJreMap *jre_map, JagIndexerJarMap *jar_map) {
	JagMooseService *result = g_object_new(JAG_TYPE_MOOSE_SERVICE, NULL);
	cat_ref_anounce(result);
	JagMooseServicePrivate *priv = result->priv;
	priv->wor_service = cat_ref_ptr(moo_service_get_worm_service(moo_service));
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->jar_map = cat_ref_ptr(jar_map);
	priv->jre_map = cat_ref_ptr(jre_map);
	MooTransactionDispatcher *tx_dispatcher = moo_service_get_transaction_dispatcher(priv->moo_service);
	moo_transaction_dispatcher_add_listener(tx_dispatcher, MOO_ITRANSACTION_LISTENER(result));

	MooTransaction *tx = moo_service_create_transaction((GObject *) moo_service, moo_service);
	moo_transaction_begin(tx);
	MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
	l_transaction_commited((MooITransactionListener *) result, tx, root_node);
	cat_unref_ptr(tx);

	return result;
}


WorService *jag_moose_service_get_worm_service(JagMooseService *moose_service) {
	JagMooseServicePrivate *priv = JAG_MOOSE_SERVICE_GET_PRIVATE(moose_service);
	return priv->wor_service;
}



MooService *jag_moose_service_get_moo_service(JagMooseService *moose_service) {
	JagMooseServicePrivate *priv = JAG_MOOSE_SERVICE_GET_PRIVATE(moose_service);
	return priv->moo_service;

}

JagIndexerJreMap *jag_moose_service_get_jre_map(JagMooseService *moose_service) {
	JagMooseServicePrivate *priv = JAG_MOOSE_SERVICE_GET_PRIVATE(moose_service);
	return priv->jre_map;
}


JagIndexerJarMap *jag_moose_service_get_jar_map(JagMooseService *moose_service) {
	JagMooseServicePrivate *priv = JAG_MOOSE_SERVICE_GET_PRIVATE(moose_service);
	return priv->jar_map;
}

/********************* begin MooITransactionListener implementation *********************/

static void l_transaction_commited(MooITransactionListener *self, struct _MooTransaction *transaction, struct _MooNodeWo *new_root) {
	JagMooseServicePrivate *priv = JAG_MOOSE_SERVICE_GET_PRIVATE(self);

	cat_log_debug("Transaction commited: %o", new_root);

	JagMooApplyTransactionChangesRequest *request = jag_moo_apply_transaction_changes_request_new((JagMooseService *) self);
	wor_service_post_request(priv->wor_service, WOR_REQUEST(request));
	cat_unref_ptr(request);
}

static void l_moose_tx_listener_iface_init(MooITransactionListenerInterface *iface) {
	iface->transactionCommited = l_transaction_commited;

}

/********************* end MooITransactionListener implementation *********************/
