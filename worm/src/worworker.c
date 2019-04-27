/*
   File:    worworker.c
   Project: worm
   Author:  Douwe Vos
   Date:    Dec 12, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#include "worworker.h"
#include "woriprovider.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorWorker"
#include <logging/catlog.h>

struct _WorWorkerPrivate {
	WorIProvider *work_provider;
	GThread *thread;
	WorRequest *running_request;
	CatAtomicReference *current_request_ref;
};

G_DEFINE_TYPE_WITH_PRIVATE(WorWorker, wor_worker, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void *l_worker_run(void *ptr_to_self);

static void wor_worker_class_init(WorWorkerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void wor_worker_init(WorWorker *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorWorker *instance = WOR_WORKER(object);
	WorWorkerPrivate *priv = wor_worker_get_instance_private(instance);
	cat_unref_ptr(priv->current_request_ref);
	G_OBJECT_CLASS(wor_worker_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(wor_worker_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


WorWorker *wor_worker_new(WorIProvider *work_provider) {
	WorWorker *result = g_object_new(WOR_TYPE_WORKER, NULL);
	cat_ref_anounce(result);
	WorWorkerPrivate *priv = wor_worker_get_instance_private(result);
	priv->work_provider = work_provider;
	WOR_IPROVIDER_GET_INTERFACE(work_provider)->increaseUsage(work_provider);
	priv->current_request_ref = cat_atomic_reference_new();
	priv->thread = g_thread_new("worker", l_worker_run, result);
	return result;
}

void wor_worker_dump_state(WorWorker *worker) {
	WorWorkerPrivate *priv = wor_worker_get_instance_private(worker);
	WorRequest *request = (WorRequest *) cat_atomic_reference_get(priv->current_request_ref);
	cat_log_info("### %p request=%o", worker, request);
	cat_unref_ptr(request);
}


static void *l_worker_run(void *ptr_to_self) {
	WorWorker *worker = WOR_WORKER(ptr_to_self);
	WorWorkerPrivate *priv = wor_worker_get_instance_private(worker);
	WorIProviderInterface *provider_iface = WOR_IPROVIDER_GET_INTERFACE(priv->work_provider);
	int64_t start, end;
	int64_t runtime = 0;
	while(TRUE) {
		WorRequest *request = provider_iface->next_request(priv->work_provider, &runtime);
		if (request) {
			start = cat_date_current_time();
			cat_atomic_reference_set(priv->current_request_ref, (GObject *) request);
			cat_log_debug("### %p running  request: %o", worker, request);
			WorRequestClass *wor_class = WOR_REQUEST_GET_CLASS(request);
			wor_class->runRequest(request);
			cat_log_debug("### %p finished request: %o", worker, request);
			cat_atomic_reference_set(priv->current_request_ref, NULL);
			cat_unref_ptr(request);
			end = cat_date_current_time();
			runtime = runtime + (end-start);
		} else {
			if (!provider_iface->isActive(priv->work_provider)) {
				break;
			}
		}
	}
	provider_iface->decreaseUsage(priv->work_provider);
	cat_unref_ptr(worker);
	return NULL;
}
