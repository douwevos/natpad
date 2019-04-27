/*
   File:    mootransactiondispatcher.c
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

#include "mootransactiondispatcher.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooTransactionDispatcher"
#include <logging/catlog.h>

struct _MooTransactionDispatcherPrivate {
	CatWeakList *listeners;
	CatAtomicInteger *collision_count;
	CatAtomicInteger *unchanged_count;
	CatAtomicInteger *success_count;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooTransactionDispatcher, moo_transaction_dispatcher, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_transaction_dispatcher_class_init(MooTransactionDispatcherClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_transaction_dispatcher_init(MooTransactionDispatcher *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooTransactionDispatcher *instance = MOO_TRANSACTION_DISPATCHER(object);
	MooTransactionDispatcherPrivate *priv = moo_transaction_dispatcher_get_instance_private(instance);
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->collision_count);
	cat_unref_ptr(priv->success_count);
	cat_unref_ptr(priv->unchanged_count);
	G_OBJECT_CLASS(moo_transaction_dispatcher_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_transaction_dispatcher_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooTransactionDispatcher *moo_transaction_dispatcher_new() {
	MooTransactionDispatcher *result = g_object_new(MOO_TYPE_TRANSACTION_DISPATCHER, NULL);
	cat_ref_anounce(result);
	MooTransactionDispatcherPrivate *priv = moo_transaction_dispatcher_get_instance_private(result);
	priv->listeners = cat_weak_list_new();
	priv->collision_count = cat_atomic_integer_new();
	priv->unchanged_count = cat_atomic_integer_new();
	priv->success_count = cat_atomic_integer_new();
	return result;
}


CatIIterator *moo_transaction_dispatcher_get_listeners_iter(MooTransactionDispatcher *dispatcher) {
	MooTransactionDispatcherPrivate *priv = moo_transaction_dispatcher_get_instance_private(dispatcher);
	return cat_weak_list_iterator(priv->listeners);
}


void moo_transaction_dispatcher_add_listener(MooTransactionDispatcher *dispatcher, MooITransactionListener *listener) {
	MooTransactionDispatcherPrivate *priv = moo_transaction_dispatcher_get_instance_private(dispatcher);
	cat_weak_list_append_once(priv->listeners, (GObject *)  listener);
}

void moo_transaction_dispatcher_increase_unchanged(MooTransactionDispatcher *dispatcher) {
	MooTransactionDispatcherPrivate *priv = moo_transaction_dispatcher_get_instance_private(dispatcher);
	cat_atomic_integer_increment(priv->unchanged_count);
	cat_log_debug("collision/unchanged/success = %d/%d/%d", cat_atomic_integer_get(priv->collision_count), cat_atomic_integer_get(priv->unchanged_count), cat_atomic_integer_get(priv->success_count));
}


void moo_transaction_dispatcher_increase_collision(MooTransactionDispatcher *dispatcher) {
	MooTransactionDispatcherPrivate *priv = moo_transaction_dispatcher_get_instance_private(dispatcher);
	cat_atomic_integer_increment(priv->collision_count);
	cat_log_debug("collision/unchanged/success = %d/%d/%d", cat_atomic_integer_get(priv->collision_count), cat_atomic_integer_get(priv->unchanged_count), cat_atomic_integer_get(priv->success_count));
}

void moo_transaction_dispatcher_increase_succes(MooTransactionDispatcher *dispatcher) {
	MooTransactionDispatcherPrivate *priv = moo_transaction_dispatcher_get_instance_private(dispatcher);
	cat_atomic_integer_increment(priv->success_count);
	cat_log_debug("collision/unchanged/success = %d/%d/%d", cat_atomic_integer_get(priv->collision_count), cat_atomic_integer_get(priv->unchanged_count), cat_atomic_integer_get(priv->success_count));
}
