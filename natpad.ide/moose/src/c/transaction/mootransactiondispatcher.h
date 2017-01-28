/*
   File:    mootransactiondispatcher.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 11, 2013
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

#ifndef MOOTRANSACTIONDISPATCHER_H_
#define MOOTRANSACTIONDISPATCHER_H_

#include "mooitransactionlistener.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_TRANSACTION_DISPATCHER              (moo_transaction_dispatcher_get_type())
#define MOO_TRANSACTION_DISPATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_transaction_dispatcher_get_type(), MooTransactionDispatcher))
#define MOO_TRANSACTION_DISPATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_TRANSACTION_DISPATCHER, MooTransactionDispatcherClass))
#define MOO_IS_TRANSACTION_DISPATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_TRANSACTION_DISPATCHER))
#define MOO_IS_TRANSACTION_DISPATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_TRANSACTION_DISPATCHER))
#define MOO_TRANSACTION_DISPATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_TRANSACTION_DISPATCHER, MooTransactionDispatcherClass))
#define MOO_TRANSACTION_DISPATCHER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_TRANSACTION_DISPATCHER, MooTransactionDispatcherPrivate))


typedef struct _MooTransactionDispatcher               MooTransactionDispatcher;
typedef struct _MooTransactionDispatcherPrivate        MooTransactionDispatcherPrivate;
typedef struct _MooTransactionDispatcherClass          MooTransactionDispatcherClass;


struct _MooTransactionDispatcher {
	GObject parent;
	MooTransactionDispatcherPrivate *priv;
};

struct _MooTransactionDispatcherClass {
	GObjectClass parent_class;
};


GType moo_transaction_dispatcher_get_type();

MooTransactionDispatcher *moo_transaction_dispatcher_new();


CatIIterator *moo_transaction_dispatcher_get_listeners_iter(MooTransactionDispatcher *dispatcher);

void moo_transaction_dispatcher_add_listener(MooTransactionDispatcher *dispatcher, MooITransactionListener *listener);

void moo_transaction_dispatcher_increase_collision(MooTransactionDispatcher *dispatcher);

void moo_transaction_dispatcher_increase_unchanged(MooTransactionDispatcher *dispatcher);

void moo_transaction_dispatcher_increase_succes(MooTransactionDispatcher *dispatcher);

G_END_DECLS

#endif /* MOOTRANSACTIONDISPATCHER_H_ */
