/*
   File:    jagmooapplytransactionchangesrequest.h
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

#ifndef JAGMOOAPPLYTRANSACTIONCHANGESREQUEST_H_
#define JAGMOOAPPLYTRANSACTIONCHANGESREQUEST_H_

#include "jagmooseservice.h"
#include <caterpillar.h>
#include <moose.h>
#include <worm.h>

G_BEGIN_DECLS

#define JAG_TYPE_MOO_APPLY_TRANSACTION_CHANGES_REQUEST              (jag_moo_apply_transaction_changes_request_get_type())
#define JAG_MOO_APPLY_TRANSACTION_CHANGES_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_moo_apply_transaction_changes_request_get_type(), JagMooApplyTransactionChangesRequest))
#define JAG_MOO_APPLY_TRANSACTION_CHANGES_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_MOO_APPLY_TRANSACTION_CHANGES_REQUEST, JagMooApplyTransactionChangesRequestClass))
#define JAG_IS_MOO_APPLY_TRANSACTION_CHANGES_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_MOO_APPLY_TRANSACTION_CHANGES_REQUEST))
#define JAG_IS_MOO_APPLY_TRANSACTION_CHANGES_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_MOO_APPLY_TRANSACTION_CHANGES_REQUEST))
#define JAG_MOO_APPLY_TRANSACTION_CHANGES_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_MOO_APPLY_TRANSACTION_CHANGES_REQUEST, JagMooApplyTransactionChangesRequestClass))

typedef struct _JagMooApplyTransactionChangesRequest               JagMooApplyTransactionChangesRequest;
typedef struct _JagMooApplyTransactionChangesRequestPrivate        JagMooApplyTransactionChangesRequestPrivate;
typedef struct _JagMooApplyTransactionChangesRequestClass          JagMooApplyTransactionChangesRequestClass;


struct _JagMooApplyTransactionChangesRequest {
	WorRequest parent;
};

struct _JagMooApplyTransactionChangesRequestClass {
	WorRequestClass parent_class;
};

GType jag_moo_apply_transaction_changes_request_get_type();

JagMooApplyTransactionChangesRequest *jag_moo_apply_transaction_changes_request_new(JagMooseService *jag_moose_service);

G_END_DECLS

#endif /* JAGMOOAPPLYTRANSACTIONCHANGESREQUEST_H_ */
