/*
   File:    mootransaction.c
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

#include "mootransaction.h"
#include <unistd.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooTransaction"
#include <logging/catlog.h>

struct _MooTransactionPrivate {
	MooTransactionDispatcher *transaction_dispatcher;
	CatAtomicLong *unique_id_counter;
	CatAtomicInteger *version_counter;
	CatAtomicReference *root_ref;

	MooNodeWo *root;
	MooNodeWo *commited_root;

	GObject *owner;
	int retry_count;
	int64_t start_ts;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooTransaction, moo_transaction, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_transaction_class_init(MooTransactionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_transaction_init(MooTransaction *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooTransaction *instance = MOO_TRANSACTION(object);
	MooTransactionPrivate *priv = moo_transaction_get_instance_private(instance);
	cat_unref_ptr(priv->root);
	cat_unref_ptr(priv->commited_root);
	cat_unref_ptr(priv->root_ref);
	cat_unref_ptr(priv->transaction_dispatcher);
	cat_unref_ptr(priv->unique_id_counter);
	cat_unref_ptr(priv->version_counter);
	cat_unref_ptr(priv->owner);
	G_OBJECT_CLASS(moo_transaction_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_transaction_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooTransaction *moo_transaction_new(GObject *owner, MooTransactionDispatcher *transaction_dispatcher, CatAtomicLong *unique_id_counter, CatAtomicInteger *version_counter, CatAtomicReference *root_ref) {
	MooTransaction *result = g_object_new(MOO_TYPE_TRANSACTION, NULL);
	cat_ref_anounce(result);
	MooTransactionPrivate *priv = moo_transaction_get_instance_private(result);
	priv->transaction_dispatcher = cat_ref_ptr(transaction_dispatcher);
	priv->unique_id_counter = cat_ref_ptr(unique_id_counter);
	priv->version_counter = cat_ref_ptr(version_counter);
	priv->root_ref = cat_ref_ptr(root_ref);
	priv->root = NULL;
	priv->commited_root = NULL;
	priv->owner = cat_ref_ptr(owner);
	priv->retry_count = 0;
	return result;
}


void moo_transaction_begin(MooTransaction *transaction) {
	MooTransactionPrivate *priv = moo_transaction_get_instance_private(transaction);
	cat_unref_ptr(priv->root);
	priv->root = (MooNodeWo *) cat_atomic_reference_get(priv->root_ref);
	cat_log_debug("%o >> start: root=%o", priv->owner, priv->root);
	priv->start_ts = cat_date_current_time();
}


//static MooNodeWo *l_merge(MooNodeWo *old_root, MooNodeWo *current_root, MooNodeWo *goal_root) {
//	MooNodeWo *e_goal_root = moo_node_wo_ensure_editable(goal_root, NULL);
//
//	moo_node_wo_
//
//}

gboolean moo_transaction_commit(MooTransaction *transaction, MooNodeWo *e_new_root) {
	MooTransactionPrivate *priv = moo_transaction_get_instance_private(transaction);
	cat_unref_ptr(priv->commited_root);
	priv->commited_root = moo_node_wo_anchor(e_new_root, cat_atomic_integer_increment(priv->version_counter));
	if (priv->root == priv->commited_root) {
		cat_log_debug("%o >> commit no changes: root=%o", priv->owner, priv->commited_root);
		moo_transaction_dispatcher_increase_unchanged(priv->transaction_dispatcher);
		return TRUE;
	}

	MooNodeWo *current_root = (MooNodeWo *) cat_atomic_reference_get(priv->root_ref);
	if (current_root!=priv->root) {
		cat_log_on_debug({
			uint64_t end_ts = cat_date_current_time();
			int period = (int) (end_ts - priv->start_ts);
			cat_log_debug("collision:period=%d - %o", period, priv->owner);
			cat_log_debug("%o >> current_root=%o not equal priv->root=%o", priv->owner, current_root, priv->root);
		});
		// TODO implement merge
//		l_merge(priv->root, current_root, priv->commited_root);
		moo_transaction_dispatcher_increase_collision(priv->transaction_dispatcher);

		cat_unref_ptr(current_root);

		return FALSE;
	}

	gboolean result = cat_atomic_reference_compare_set(priv->root_ref, (GObject *) current_root, (GObject *) priv->commited_root);
	cat_unref_ptr(current_root);
	cat_log_debug("%o >> reference set: new_root=%o, priv->root=%o, success=%d", priv->owner, e_new_root, priv->root, result);
	if (!result) {

		cat_log_debug("collision: %o", priv->owner);
		moo_transaction_dispatcher_increase_collision(priv->transaction_dispatcher);
//		cat_atomic_reference_get(priv->root_ref);
	} else if (result && (priv->root!=priv->commited_root)) {
		cat_log_debug("success: %o", priv->owner);
		moo_transaction_dispatcher_increase_succes(priv->transaction_dispatcher);
		cat_log_on_trace({
			CatStringWo *e_buf = cat_string_wo_new();
			moo_node_wo_print(priv->commited_root, e_buf);
			printf("%s", cat_string_wo_getchars(e_buf));
			cat_unref_ptr(e_buf);
		});
		CatIIterator *list_iter = moo_transaction_dispatcher_get_listeners_iter(priv->transaction_dispatcher);
		while(cat_iiterator_has_next(list_iter)) {
			MooITransactionListener *listener = (MooITransactionListener *) cat_iiterator_next(list_iter);
			cat_log_debug("%o >> dispatch to listener:%s - %o", priv->owner, g_type_name_from_instance((GTypeInstance *) listener), listener);
			moo_itransaction_listener_transaction_commited(listener, transaction, priv->commited_root);
		}
		cat_unref_ptr(list_iter);
	}
	return result;
}


MooNodeWo *moo_transaction_get_tx_root_node(MooTransaction *transaction) {
	MooTransactionPrivate *priv = moo_transaction_get_instance_private(transaction);
	return priv->root;
}

MooNodeWo *moo_transaction_get_commited_root_node(MooTransaction *transaction) {
	MooTransactionPrivate *priv = moo_transaction_get_instance_private(transaction);
	return priv->commited_root;
}

gboolean moo_transaction_retry(MooTransaction *transaction) {
	MooTransactionPrivate *priv = moo_transaction_get_instance_private(transaction);
	priv->retry_count++;
	if (priv->retry_count>50) {
		cat_log_error("transaction could not be committed:%o", priv->owner);
		return FALSE;
	}
	if (priv->retry_count>3) {
		usleep(priv->retry_count*priv->retry_count*10000);
	}
	return TRUE;
}
