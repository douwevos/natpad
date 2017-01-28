/*
   File:    mooitransactionlistener.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 14, 2013
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


#ifndef MOOITRANSACTIONLISTENER_H_
#define MOOITRANSACTIONLISTENER_H_

#include <caterpillar.h>

#define MOO_TYPE_ITRANSACTION_LISTENER                 (moo_itransaction_listener_get_type())
#define MOO_ITRANSACTION_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_ITRANSACTION_LISTENER, MooITransactionListener))
#define MOO_IS_ITRANSACTION_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_ITRANSACTION_LISTENER))
#define MOO_ITRANSACTION_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_ITRANSACTION_LISTENER, MooITransactionListenerInterface))


typedef struct _MooITransactionListener               MooITransactionListener; /* dummy object */
typedef struct _MooITransactionListenerInterface      MooITransactionListenerInterface;


struct _MooTransaction;
struct _MooNodeWo;

struct _MooITransactionListenerInterface {
	GTypeInterface parent_iface;
	void (*transactionCommited)(MooITransactionListener *self, struct _MooTransaction *transaction, struct _MooNodeWo *new_root);
};

GType moo_itransaction_listener_get_type(void);

void moo_itransaction_listener_transaction_commited(MooITransactionListener *self, struct _MooTransaction *transaction, struct _MooNodeWo *new_root);


#endif /* MOOITRANSACTIONLISTENER_H_ */
