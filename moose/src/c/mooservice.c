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
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ISEQUENCE, l_sequence_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ILISTENER, l_vip_listener_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_service_class_init(MooServiceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooServicePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_service_init(MooService *instance) {
	MooServicePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_SERVICE, MooServicePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooService *instance = MOO_SERVICE(object);
	MooServicePrivate *priv = instance->priv;
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


	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooService *moo_service_new(WorService *wor_service, VipService *vip_service) {
	MooService *result = g_object_new(MOO_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	MooServicePrivate *priv = result->priv;
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



//	cat_ref_ptr(priv->root_ref);
//	cat_ref_ptr(priv->root_ref);
//	cat_ref_ptr(priv->root_ref);

	cat_unref_ptr(initial_root);
	vip_service_add_listener(vip_service, VIP_ILISTENER(result));

	priv->module_transaction_listener = moo_module_transaction_listener_new(result);
	moo_transaction_dispatcher_add_listener(priv->transaction_dispatcher, (MooITransactionListener *) priv->module_transaction_listener);
	return result;
}


MooTransaction *moo_service_create_transaction(GObject *owner, MooService *moo_service) {
	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(moo_service);
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
		} else {
//			try {
//				Thread.sleep(100+(retryCnt*200));
//			} catch (InterruptedException e) {
//				e.printStackTrace();
//			}
		}
	}
	cat_unref_ptr(transaction);
	return result;
}


//gboolean moo_service_add_module(MooService *moo_service, VipPath *module_base_path, CatStringWo *a_module_name) {
//	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(moo_service);
//
//	/* scan Viper contents */
//	VipNodePath *base_node_path = vip_service_open(priv->vip_service, module_base_path);
//
//	VipNode *tail = vip_node_path_get_tail(base_node_path);
//	vip_node_set_scan_recursive(tail, TRUE);
//
//	vip_service_scan(priv->vip_service);
//
//	/* create module */
//	MooContentMapWo *edit_con_map = moo_content_map_wo_new();
//	VipNode *base_node = vip_node_path_get_tail(base_node_path);
//	MooModuleContentWo *module_content = moo_module_content_wo_new(base_node);
//	moo_content_map_wo_set(edit_con_map, (MooIContent *) module_content);
//	cat_unref(module_content);
//	MooContentMapWo *content_map = moo_content_map_wo_finalize_result(edit_con_map);
//	long long moose_id = vip_isequence_next((VipISequence *) moo_service);
//	cat_log_debug("content_map=%o", content_map);
//	MooNodeWo *module = moo_node_new(moose_id, module_name, NULL, content_map, 0);
//	//	gboolean didAddModule =
//	gboolean result =  l_add_node(moo_service, module);
//	cat_unref(content_map);
//	cat_unref_ptr(edit_con_map);
//	cat_unref_ptr(module);
//	cat_unref_ptr(base_node_path);
//	return result;
//}

MooNodeWo *moo_service_get_root_node_ref(MooService *moo_service) {
	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(moo_service);
	return (MooNodeWo *) cat_atomic_reference_get(priv->root_ref);
}

MooTransactionDispatcher *moo_service_get_transaction_dispatcher(MooService *moo_service) {
	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(moo_service);
	return priv->transaction_dispatcher;
}

WorService *moo_service_get_worm_service(MooService *moo_service) {
	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(moo_service);
	return priv->wor_service;
}

VipService *moo_service_get_viper_service(MooService *moo_service) {
	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(moo_service);
	return priv->vip_service;
}

MooCowPropertiesContainer *moo_service_get_cow_properties_container(MooService *moo_service) {
	return MOO_SERVICE_GET_PRIVATE(moo_service)->cow_properties_container;
}

CowPanelModel *moo_service_get_properties_panel_model(MooService *moo_service) {
	return MOO_SERVICE_GET_PRIVATE(moo_service)->properties_panel_model;
}



/********************* begin VipIListener implementation *********************/

static void l_viper_snapshot_set(VipIListener *self, struct _VipSnapshot *newSnapshot) {
	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(self);
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
	MooServicePrivate *priv = MOO_SERVICE_GET_PRIVATE(self);
	return cat_atomic_long_increment(priv->unique_id_counter);
}


static void l_sequence_iface_init(VipISequenceInterface *iface) {
	iface->next = l_sequence_next;
}

/********************* end VipISequence implementation *********************/
