/*
   File:    worqueue.c
   Project: worm
   Author:  Douwe Vos
   Date:    May 2, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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


#include "worqueueprivate.h"
#include "worqueuerequestdelegate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorQueue"
#include <logging/catlog.h>

struct _WorQueuePrivate {
	WorService *wor_service;
	volatile int semaphore_count;
	volatile int semaphore_size;
	CatArrayWo *e_queue;
	CatLock *lock;
};


G_DEFINE_TYPE(WorQueue, wor_queue, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void wor_queue_class_init(WorQueueClass *clazz) {
	g_type_class_add_private(clazz, sizeof(WorQueuePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void wor_queue_init(WorQueue *instance) {
	WorQueuePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, WOR_TYPE_QUEUE, WorQueuePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorQueue *instance = WOR_QUEUE(object);
	WorQueuePrivate *priv = instance->priv;
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->e_queue);
	cat_unref_ptr(priv->wor_service);
	G_OBJECT_CLASS(wor_queue_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(wor_queue_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


WorQueue *wor_queue_new(WorService *wor_service) {
	WorQueue *result = g_object_new(WOR_TYPE_QUEUE, NULL);
	cat_ref_anounce(result);
	WorQueuePrivate *priv = result->priv;
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->semaphore_count = 0;
	priv->semaphore_size = 1;
	priv->e_queue = cat_array_wo_new();
	priv->lock = cat_lock_new();
	return result;
}

void wor_queue_set_semaphore_size(WorQueue *queue, int size) {
	WorQueuePrivate *priv = WOR_QUEUE_GET_PRIVATE(queue);
	cat_lock_lock(priv->lock);
	priv->semaphore_size = size;

	// if the semaphore size increased and we still have requests queued then post them until the semaphore_count reaches the new size.
	while(priv->semaphore_count<priv->semaphore_size) {
		WorRequest *first_entry = NULL;
		if (cat_array_wo_remove_first(priv->e_queue, (GObject **) &first_entry)) {
			priv->semaphore_count++;
			wor_service_post_request(priv->wor_service, first_entry);
			cat_unref_ptr(first_entry);
		} else {
			break;
		}
	}
	cat_lock_unlock(priv->lock);
}



void wor_queue_post(WorQueue *queue, WorRequest *request) {
	WorQueueRequestDelegate *request_delegate = wor_queue_request_delegate_new(queue, request, FALSE);
	cat_log_debug("[%p] post:%o", queue, request_delegate);
	wor_queue_repost(queue, request_delegate);
	cat_unref_ptr(request_delegate);
}

void wor_queue_post_delayed(WorQueue *queue, WorRequest *request, uint64_t timeout) {
	WorQueuePrivate *priv = WOR_QUEUE_GET_PRIVATE(queue);
	WorQueueRequestDelegate *request_delegate = wor_queue_request_delegate_new(queue, request, TRUE);
	wor_request_set_time_out((WorRequest *) request_delegate, timeout);
	wor_service_post_request(priv->wor_service, (WorRequest *) request_delegate);
	cat_unref_ptr(request_delegate);
}


void wor_queue_repost(WorQueue *queue, WorQueueRequestDelegate *request_delegate) {
	WorQueuePrivate *priv = WOR_QUEUE_GET_PRIVATE(queue);
	cat_lock_lock(priv->lock);
	cat_log_debug("[%p] reposting:%o, sema-count=%d, queue-size=%d", queue, request_delegate, priv->semaphore_count, cat_array_wo_size(priv->e_queue));
	if (priv->semaphore_count<priv->semaphore_size) {
		priv->semaphore_count++;
		wor_service_post_request(priv->wor_service, (WorRequest *) request_delegate);
	} else {
		cat_array_wo_append(priv->e_queue, (GObject *) request_delegate);
	}
//	cat_lock_notify_all(priv->lock);
	cat_lock_unlock(priv->lock);
}

void wor_queue_forward_next(WorQueue *queue) {
	WorQueuePrivate *priv = WOR_QUEUE_GET_PRIVATE(queue);
	cat_log_debug("[%p] forward_next, sema-count=%d", queue, priv->semaphore_count);
	cat_lock_lock(priv->lock);
	WorRequest *first_entry = NULL;
	if (cat_array_wo_remove_first(priv->e_queue, (GObject **) &first_entry)) {
		cat_log_debug("[%p] posting:%o", queue, first_entry);
		wor_service_post_request(priv->wor_service, first_entry);
		cat_log_debug("[%p] posted:%o", queue, first_entry);
		cat_unref_ptr(first_entry);
	} else {
		priv->semaphore_count--;
	}
	cat_lock_unlock(priv->lock);
}






