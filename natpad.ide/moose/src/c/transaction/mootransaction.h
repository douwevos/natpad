/*
   File:    mootransaction.h
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


#ifndef MOOTRANSACTION_H_
#define MOOTRANSACTION_H_

#include "mootransactiondispatcher.h"
#include "../model/moonodewo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_TRANSACTION              (moo_transaction_get_type())
#define MOO_TRANSACTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_transaction_get_type(), MooTransaction))
#define MOO_TRANSACTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_TRANSACTION, MooTransactionClass))
#define MOO_IS_TRANSACTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_TRANSACTION))
#define MOO_IS_TRANSACTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_TRANSACTION))
#define MOO_TRANSACTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_TRANSACTION, MooTransactionClass))
#define MOO_TRANSACTION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_TRANSACTION, MooTransactionPrivate))


typedef struct _MooTransaction               MooTransaction;
typedef struct _MooTransactionPrivate        MooTransactionPrivate;
typedef struct _MooTransactionClass          MooTransactionClass;


struct _MooTransaction {
	GObject parent;
	MooTransactionPrivate *priv;
};

struct _MooTransactionClass {
	GObjectClass parent_class;
};


GType moo_transaction_get_type();

MooTransaction *moo_transaction_new(GObject *owner, MooTransactionDispatcher *transaction_dispatcher, CatAtomicLong *unique_id_counter, CatAtomicInteger *version_counter, CatAtomicReference *root_ref);

void moo_transaction_begin(MooTransaction *transaction);

gboolean moo_transaction_commit(MooTransaction *transaction, MooNodeWo *new_root);

MooNodeWo *moo_transaction_get_tx_root_node(MooTransaction *transaction);

MooNodeWo *moo_transaction_get_commited_root_node(MooTransaction *transaction);

gboolean moo_transaction_retry(MooTransaction *transaction);

G_END_DECLS

#endif /* MOOTRANSACTION_H_ */
