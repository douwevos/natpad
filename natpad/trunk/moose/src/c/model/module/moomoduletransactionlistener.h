/*
 * moomoduletransactionlistener.h
 *
 *  Created on: Nov 20, 2013
 *      Author: superman
 */

#ifndef MOOMODULETRANSACTIONLISTENER_H_
#define MOOMODULETRANSACTIONLISTENER_H_

#include <caterpillar.h>
#include "../../mooservice.h"

G_BEGIN_DECLS

#define MOO_TYPE_MODULE_TRANSACTION_LISTENER              (moo_module_transaction_listener_get_type())
#define MOO_MODULE_TRANSACTION_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_module_transaction_listener_get_type(), MooModuleTransactionListener))
#define MOO_MODULE_TRANSACTION_LISTENER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_MODULE_TRANSACTION_LISTENER, MooModuleTransactionListenerClass))
#define MOO_IS_MODULE_TRANSACTION_LISTENER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_MODULE_TRANSACTION_LISTENER))
#define MOO_IS_MODULE_TRANSACTION_LISTENER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_MODULE_TRANSACTION_LISTENER))
#define MOO_MODULE_TRANSACTION_LISTENER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_MODULE_TRANSACTION_LISTENER, MooModuleTransactionListenerClass))
#define MOO_MODULE_TRANSACTION_LISTENER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_MODULE_TRANSACTION_LISTENER, MooModuleTransactionListenerPrivate))


typedef struct _MooModuleTransactionListener               MooModuleTransactionListener;
typedef struct _MooModuleTransactionListenerPrivate        MooModuleTransactionListenerPrivate;
typedef struct _MooModuleTransactionListenerClass          MooModuleTransactionListenerClass;


struct _MooModuleTransactionListener {
	GObject parent;
	MooModuleTransactionListenerPrivate *priv;
};

struct _MooModuleTransactionListenerClass {
	GObjectClass parent_class;
};


GType moo_module_transaction_listener_get_type();

MooModuleTransactionListener *moo_module_transaction_listener_new(MooService *moo_service);

G_END_DECLS

#endif
