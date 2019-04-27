/*
   File:    mooservice.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 10, 2013
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

#include "mooservice.h"
#include "model/properties/moopropertieshandler.h"
#include "model/moonodewo.h"
#include "model/moocontentmapwo.h"
#include "model/module/moomodulecontentwo.h"
#include "model/module/moomoduletransactionlistener.h"
#include "viper/moosetsnapshotrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooService"
#include <logging/catlog.h>

struct _MooServicePrivate {
	WorService *wor_service;
	VipService *vip_service;

	CatAtomicLong *unique_id_counter;
	CatAtomicInteger *version_counter;

	MooTransactionDispatcher *transaction_dispatcher;
	CatAtomicReference *root_ref;	// <MooNode>
	CatAtomicReference *snapshot_ref;	// <VipSnapshot>

	MooPropertiesHandler *properties_handler;


	MooCowPropertiesContainer *cow_properties_container;
	CowPanelModel *properties_panel_model;

	MooCowPropertiesPanelFactory *cow_properties_panel_factory;

	MooModuleTransactionListener *module_transaction_listener;
};

static void l_vip_listener_iface_init(VipIListenerInterface *iface);
static void l_sequence_iface_init(VipISequenceInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooService, moo_service, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(MooService)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ISEQUENCE, l_sequence_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ILISTENER, l_vip_listener_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_service_class_init(MooServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_service_init(MooService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooService *instance = MOO_SERVICE(object);
	MooServicePrivate *priv = moo_service_get_instance_private(instance);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->unique_id_counter);
	cat_unref_ptr(priv->version_counter);
	cat_unref_ptr(priv->transaction_dispatcher);
	if (priv->root_ref) {
		cat_atomic_reference_set(priv->root_ref, NULL);
	}
	cat_unref_ptr(priv->root_ref);
	if (priv->snapshot_ref) {
		cat_atomic_reference_set(priv->snapshot_ref, NULL);
	}
	cat_unref_ptr(priv->snapshot_ref);
//	if (priv->properties_container && priv->properties_handler) {
//		moo_properties_container_remove_handler(priv->properties_container, priv->properties_handler);
//	}
	cat_unref_ptr(priv->properties_handler);

	cat_unref_ptr(priv->cow_properties_container);
	cat_unref_ptr(priv->properties_panel_model);
	cat_unref_ptr(priv->cow_properties_panel_factory);
	cat_unref_ptr(priv->module_transaction_listener);

	G_OBJECT_CLASS(moo_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooService *moo_service_new(WorService *wor_service, VipService *vip_service) {
	MooService *result = g_object_new(MOO_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	MooServicePrivate *priv = moo_service_get_instance_private(result);
	priv->unique_id_counter = cat_atomic_long_new();
	priv->version_counter = cat_atomic_integer_new();
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->vip_service = cat_ref_ptr(vip_service);

	priv->cow_properties_container = moo_cow_properties_container_new(result);
	priv->cow_properties_panel_factory = moo_cow_properties_panel_factory_new();
	priv->properties_panel_model = cow_panel_model_new();
	cow_panel_model_add_factory(priv->properties_panel_model, (CowIPanelFactory *) priv->cow_properties_panel_factory);

	priv->properties_handler = moo_properties_handler_new(result);
	moo_cow_properties_container_add_handler(priv->cow_properties_container, (MooIPropertiesHandler *) priv->properties_handler);


	priv->transaction_dispatcher = moo_transaction_dispatcher_new();
	MooNodeWo *initial_root = moo_node_wo_new(cat_atomic_long_increment(priv->unique_id_counter),
			cat_string_wo_new_with("<root>"), NULL, NULL, 0);
	priv->root_ref = cat_atomic_reference_new_val((GObject *) initial_root);
	priv->snapshot_ref = cat_atomic_reference_new();

	cat_unref_ptr(initial_root);
	vip_service_add_listener(vip_service, VIP_ILISTENER(result));

	priv->module_transaction_listener = moo_module_transaction_listener_new(result);
	moo_transaction_dispatcher_add_listener(priv->transaction_dispatcher, (MooITransactionListener *) priv->module_transaction_listener);
	return result;
}


MooTransaction *moo_service_create_transaction(GObject *owner, MooService *moo_service) {
	MooServicePrivate *priv = moo_service_get_instance_private(moo_service);
	MooTransaction *result =
			moo_transaction_new(owner, priv->transaction_dispatcher, priv->unique_id_counter, priv->version_counter, priv->root_ref);
	return result;
}


gboolean moo_service_add_node(MooService *moo_service, MooNodeWo *module) {
	MooTransaction *transaction = moo_service_create_transaction((GObject *) moo_service, moo_service);
	int retryCnt = 0;
	gboolean result = FALSE;
	while(retryCnt++<10) {
		moo_transaction_begin(transaction);
		MooNodeWo *tx_root_node = moo_transaction_get_tx_root_node(transaction);
		MooNodeWo *e_root_node = moo_node_wo_ensure_editable(tx_root_node, NULL);
		moo_node_wo_append_child(e_root_node, module);

		gboolean commit_ok = moo_transaction_commit(transaction, e_root_node);
		if (commit_ok) {
			result = TRUE;
			break;
		}
	}
	cat_unref_ptr(transaction);
	return result;
}


MooNodeWo *moo_service_get_root_node_ref(MooService *moo_service) {
	MooServicePrivate *priv = moo_service_get_instance_private(moo_service);
	return (MooNodeWo *) cat_atomic_reference_get(priv->root_ref);
}

MooTransactionDispatcher *moo_service_get_transaction_dispatcher(MooService *moo_service) {
	MooServicePrivate *priv = moo_service_get_instance_private(moo_service);
	return priv->transaction_dispatcher;
}

WorService *moo_service_get_worm_service(MooService *moo_service) {
	MooServicePrivate *priv = moo_service_get_instance_private(moo_service);
	return priv->wor_service;
}

VipService *moo_service_get_viper_service(MooService *moo_service) {
	MooServicePrivate *priv = moo_service_get_instance_private(moo_service);
	return priv->vip_service;
}

MooCowPropertiesContainer *moo_service_get_cow_properties_container(MooService *moo_service) {
	MooServicePrivate *priv = moo_service_get_instance_private(moo_service);
	return priv->cow_properties_container;
}

CowPanelModel *moo_service_get_properties_panel_model(MooService *moo_service) {
	MooServicePrivate *priv = moo_service_get_instance_private(moo_service);
	return priv->properties_panel_model;
}

/********************* begin VipIListener implementation *********************/

static void l_viper_snapshot_set(VipIListener *self, struct _VipSnapshot *newSnapshot) {
	MooService *instance = MOO_SERVICE(self);
	MooServicePrivate *priv = moo_service_get_instance_private(instance);
	MooSetSnapshotRequest *snapshotSetRequest = moo_set_snapshot_request_new(MOO_SERVICE(self), priv->snapshot_ref, newSnapshot);
	wor_service_post_request(priv->wor_service, (WorRequest *) snapshotSetRequest);
	cat_unref_ptr(snapshotSetRequest);
}

static void l_vip_listener_iface_init(VipIListenerInterface *iface) {
	iface->snapshotSet = l_viper_snapshot_set;
}

/********************* end VipIListener implementation *********************/

/********************* begin VipISequence implementation *********************/

static long long l_sequence_next(VipISequence *self) {
	MooService *instance = MOO_SERVICE(self);
	MooServicePrivate *priv = moo_service_get_instance_private(instance);
	return cat_atomic_long_increment(priv->unique_id_counter);
}

static void l_sequence_iface_init(VipISequenceInterface *iface) {
	iface->next = l_sequence_next;
}

/********************* end VipISequence implementation *********************/
