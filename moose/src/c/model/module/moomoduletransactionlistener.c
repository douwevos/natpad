/*
 * moomoduletransactionlistener.c
 *
 *  Created on: Nov 20, 2013
 *      Author: superman
 */

#include "moomoduletransactionlistener.h"
#include "../../transaction/mooitransactionlistener.h"
#include "../../viper/moosetsnapshotrequest.h"
#include "moomodulecontentwo.h"
#include "../moonodewo.h"
#include "moomodulerefreshviperrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooModuleTransactionListener"
#include <logging/catlog.h>

struct _MooModuleTransactionListenerPrivate {
	MooService *moo_service;
};

static void l_transaction_listener_iface_init(MooITransactionListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooModuleTransactionListener, moo_module_transaction_listener, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ITRANSACTION_LISTENER, l_transaction_listener_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_module_transaction_listener_class_init(MooModuleTransactionListenerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooModuleTransactionListenerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_module_transaction_listener_init(MooModuleTransactionListener *instance) {
	MooModuleTransactionListenerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_MODULE_TRANSACTION_LISTENER, MooModuleTransactionListenerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooModuleTransactionListener *instance = MOO_MODULE_TRANSACTION_LISTENER(object);
	MooModuleTransactionListenerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooModuleTransactionListener *moo_module_transaction_listener_new(MooService *moo_service) {
	MooModuleTransactionListener *result = g_object_new(MOO_TYPE_MODULE_TRANSACTION_LISTENER, NULL);
	cat_ref_anounce(result);
	MooModuleTransactionListenerPrivate *priv = result->priv;
	priv->moo_service = cat_ref_ptr(moo_service);
	return result;
}





/********************* begin MooITransactionListener implementation *********************/



static void l_transaction_commited(MooITransactionListener *self, struct _MooTransaction *transaction, struct _MooNodeWo *new_root) {
	MooModuleTransactionListenerPrivate *priv = MOO_MODULE_TRANSACTION_LISTENER_GET_PRIVATE(self);


	WorService *wor_service = moo_service_get_worm_service(priv->moo_service);

	int child_count = moo_node_wo_child_count((MooNodeWo *) new_root);
	cat_log_debug("commited: child_count=%d, new_root=%p", child_count, new_root);
	if (child_count > 0) {
		int child_idx;
		for(child_idx=0; child_idx<child_count; child_idx++) {
			MooNodeWo *child = moo_node_wo_child_at((MooNodeWo *) new_root, child_idx);
			cat_log_debug("child[%d]=%o", child_idx, child);
			MooModuleContentWo *module_content = (MooModuleContentWo *) moo_node_wo_get_content(child, moo_module_content_wo_key());
			cat_log_trace("child=%o, module_content=%o", child, module_content);
			if (module_content != NULL) {
				if (moo_module_content_wo_should_refresh_viper_info(module_content)) {
					cat_log_detail("posting MooModuleRefreshViperRequest");
					MooModuleRefreshViperRequest *request = moo_module_refresh_viper_request_new(priv->moo_service, moo_node_wo_get_unique_id(child), FALSE);
					wor_service_post_request(wor_service, (WorRequest *) request);
					cat_unref_ptr(request);
				} else {
					cat_log_detail("no need to refresh module: content=%o", module_content);
				}
			}
		}
	}

}


static void l_transaction_listener_iface_init(MooITransactionListenerInterface *iface) {
	iface->transactionCommited = l_transaction_commited;
}


/********************* end MooITransactionListener implementation *********************/





