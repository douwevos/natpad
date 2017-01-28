/*
   File:    worservice.c
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

#include "worservice.h"
#include "woriprovider.h"
#include "worirequestpostbox.h"
#include "worimergeablerequest.h"
#include "worworker.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorService"
#include <logging/catlog.h>

struct _WorServicePrivate {
	GThread *thread;
	volatile int state;
	volatile gboolean stop_request;
	int usage;
	int load;
	volatile gboolean stop_response;
	CatLock *consumer_lock;
	CatArrayWo *e_default_queue;		/* default queue */
	CatArrayWo *e_waiting_queue;		/* waiting queue */

	CatLock *dispatch_lock;
	CatArrayWo *e_dispatch_queue;		/* dispatch queue */

	CatArrayWo *e_workers;
};

static void l_provider_iface_init(WorIProviderInterface *iface);
static void l_post_box_iface_init(WorIRequestPostBoxInterface *iface);

G_DEFINE_TYPE_WITH_CODE(WorService, wor_service, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(WOR_TYPE_IPROVIDER, l_provider_iface_init);
		G_IMPLEMENT_INTERFACE(WOR_TYPE_IREQUEST_POST_BOX, l_post_box_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);
static void *l_service_run(void *ptr_to_self);


static void wor_service_class_init(WorServiceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(WorServicePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void wor_service_init(WorService *instance) {
	WorServicePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, WOR_TYPE_SERVICE, WorServicePrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorService *instance = WOR_SERVICE(object);
	WorServicePrivate *priv = instance->priv;
	if (priv->dispatch_lock && priv->consumer_lock) {
		wor_service_request_shutdown(instance);

		int64_t end = cat_date_current_time()+30000;
		gboolean keep_waiting = TRUE;
		while(keep_waiting) {
			cat_log_debug("priv->usage=%d, priv->stop_response=%d", priv->usage, priv->stop_response);
			cat_lock_lock(priv->consumer_lock);
			if (priv->usage==0 && priv->stop_response) {
				keep_waiting = FALSE;
			} else {
				cat_lock_wait_timed(priv->consumer_lock, 1000);
				if (cat_date_current_time()>=end) {
					keep_waiting = FALSE;
				} else {
					CatIIterator *iter = cat_array_wo_iterator(priv->e_workers);
					while(cat_iiterator_has_next(iter)) {
						WorWorker *worker = (WorWorker *) cat_iiterator_next(iter);
						wor_worker_dump_state(worker);
					}
					cat_unref_ptr(iter);
				}
			}
			cat_lock_unlock(priv->consumer_lock);
		}
	}
	cat_log_debug("releasing consumer-lock");
	cat_unref_ptr(priv->consumer_lock);
	cat_log_debug("releasing dispatch-lock");
	cat_unref_ptr(priv->dispatch_lock);
	cat_unref_ptr(priv->e_default_queue);
	cat_unref_ptr(priv->e_dispatch_queue);
	cat_unref_ptr(priv->e_waiting_queue);
	cat_unref_ptr(priv->e_workers);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


WorService *wor_service_new_ext(int nr_of_workers, int load) {
	WorService *result = g_object_new(WOR_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	WorServicePrivate *priv = result->priv;
	priv->consumer_lock = cat_lock_new();
	priv->e_default_queue = cat_array_wo_new();
	priv->e_waiting_queue = cat_array_wo_new();
	priv->dispatch_lock = cat_lock_new();
	priv->e_dispatch_queue = cat_array_wo_new();
	priv->stop_request = FALSE;
	priv->stop_response = FALSE;
	priv->load = load;
	priv->thread = g_thread_new("Worm service", l_service_run, result);

	priv->e_workers = cat_array_wo_new();

	int idx;
	for(idx=nr_of_workers; idx>0; idx--) {
		WorWorker *worker = wor_worker_new(WOR_IPROVIDER(result));
		cat_array_wo_append(priv->e_workers, (GObject *) worker);
	}
	return result;
}

WorService *wor_service_new() {
	return wor_service_new_ext(10, 256);
}




void wor_service_request_shutdown(WorService *service) {
	WorServicePrivate *priv = service->priv;
	priv->stop_request = TRUE;
	cat_lock_lock(priv->dispatch_lock);
	cat_lock_notify_all(priv->dispatch_lock);
	cat_lock_unlock(priv->dispatch_lock);
	cat_lock_lock(priv->consumer_lock);
	cat_lock_notify_all(priv->consumer_lock);
	cat_lock_unlock(priv->consumer_lock);
}

static gboolean l_merge(WorIMergeableRequest *request, CatArrayWo *e_queue) {
	int idx;
	cat_log_trace(" :: %o ", request);
	for(idx=0; idx<cat_array_wo_size(e_queue); idx++) {
		WorRequest *in_queue = WOR_REQUEST(cat_array_wo_get(e_queue, idx));
		cat_log_trace("%d :: %o -%p", idx, in_queue, request);
		if (WOR_IS_IMERGEABLE_REQUEST(in_queue)) {
			WorMergeResult merge_result = wor_imergeable_request_try_merge((WorIMergeableRequest *) request, (WorIMergeableRequest *) in_queue);
			switch(merge_result) {
				case WOR_MERGE_FAILED :
					break;
				case WOR_MERGE_KEEP_IN_QUEUE : {
					request = NULL;
					idx = cat_array_wo_size(e_queue);
				} break;
				case WOR_MERGE_KEEP_NEW : {
					cat_array_wo_remove(e_queue, idx, NULL);
					idx--;
				} break;
			}
		}
	}
	return request==NULL;
}

void wor_service_post_request(WorService *service, WorRequest *request) {
	WorServicePrivate *priv = WOR_SERVICE_GET_PRIVATE(service);
	if (priv->stop_request) {
		return;
	}

	wor_request_add_notify(request, priv->consumer_lock);

	cat_lock_lock(priv->consumer_lock);
	if (wor_request_is_delayed(request)) {
		gboolean was_merged = FALSE;
		if (WOR_IS_IMERGEABLE_REQUEST(request)) {
			was_merged = l_merge((WorIMergeableRequest *) request, priv->e_waiting_queue);
		}

		if (!was_merged) {
			uint64_t req_time_out = wor_request_get_time_out(request);
			gboolean did_add = FALSE;
			int idx;
			for(idx=0; idx<cat_array_wo_size(priv->e_waiting_queue); idx++) {
				WorRequest *other = WOR_REQUEST(cat_array_wo_get(priv->e_waiting_queue, idx));
				uint64_t other_time_out = wor_request_get_time_out(other);
				if (req_time_out<other_time_out) {
					cat_array_wo_insert(priv->e_waiting_queue, G_OBJECT(request), idx);
					did_add = TRUE;
					idx = cat_array_wo_size(priv->e_waiting_queue);
				}
			}
			if (!did_add) {
				cat_array_wo_append(priv->e_waiting_queue, G_OBJECT(request));
			}
		}
	} else {
		gboolean was_merged = FALSE;
		if (WOR_IS_IMERGEABLE_REQUEST(request)) {
			was_merged = l_merge((WorIMergeableRequest *) request, priv->e_default_queue);
		}

		if (!was_merged) {
			cat_array_wo_append(priv->e_default_queue, G_OBJECT(request));
		}
	}
	cat_log_debug("queue-size=%d, request=%o", cat_array_wo_size(priv->e_default_queue), request);

	cat_lock_notify_all(priv->consumer_lock);
	cat_lock_unlock(priv->consumer_lock);
}




#define WOR_STATE_WAIT  0

static void *l_service_run(void *ptr_to_self) {
	WorService *service = WOR_SERVICE(ptr_to_self);
	WorServicePrivate *priv = WOR_SERVICE_GET_PRIVATE(service);
//	WorRequest *request = NULL;
	priv->state = WOR_STATE_WAIT;
//	int64_t end_report = 0l;

	CatArrayWo *e_transfer_list = cat_array_wo_new();

	while(!priv->stop_request) {

		/* transfer requests form waiting-queue and default-queue to the transfer list */
		cat_lock_lock(priv->consumer_lock);
		uint64_t next_wait_time = 30000;

		if (cat_array_wo_size(priv->e_default_queue)>0) {
			cat_log_debug("moving default-queue:cnt:%d", cat_array_wo_size(priv->e_default_queue));
			int def_idx;
			for(def_idx=0; def_idx<cat_array_wo_size(priv->e_default_queue); def_idx++) {
				WorRequest *def_req = WOR_REQUEST(cat_array_wo_get(priv->e_default_queue, def_idx));
				gboolean did_merge = FALSE;
				if (WOR_IS_IMERGEABLE_REQUEST(def_req)) {
					did_merge = l_merge((WorIMergeableRequest *) def_req, e_transfer_list);
				}
				if (!did_merge) {
					cat_array_wo_append(e_transfer_list, (GObject *) def_req);
				}
			}

			cat_array_wo_clear(priv->e_default_queue);
		}

		if (cat_array_wo_size(priv->e_waiting_queue)>0) {
			uint64_t time_now = cat_date_current_time();
			cat_log_debug("testing waiting-queue:cnt:%d, time_now=%lld", cat_array_wo_size(priv->e_waiting_queue), time_now);
			int wait_idx;
			int pre_insert_index = 0;

			cat_log_on_debug({
				for(wait_idx=0; wait_idx<cat_array_wo_size(priv->e_waiting_queue); wait_idx++) {
					WorRequest *wait_req = WOR_REQUEST(cat_array_wo_get(priv->e_waiting_queue, wait_idx));
					cat_log_debug("wait[%d] : %o", wait_idx, wait_req);
				}
			});

			for(wait_idx=0; wait_idx<cat_array_wo_size(priv->e_waiting_queue); wait_idx++) {
				WorRequest *wait_req = WOR_REQUEST(cat_array_wo_get(priv->e_waiting_queue, wait_idx));

				gboolean did_merge = FALSE;
				if (WOR_IS_IMERGEABLE_REQUEST(wait_req)) {
					did_merge = l_merge((WorIMergeableRequest *) wait_req, e_transfer_list);
				}

				if (!did_merge) {
					uint64_t req_time_out = wor_request_get_time_out(wait_req);
					gboolean is_notified = wor_request_get_and_reset_notified(wait_req);
					gboolean is_timed_out = req_time_out<=time_now;
					if (is_timed_out || is_notified) {
						if (is_timed_out) {
							cat_array_wo_insert(e_transfer_list, G_OBJECT(wait_req), pre_insert_index++);
						} else {
							cat_array_wo_append(e_transfer_list, G_OBJECT(wait_req));
						}
						cat_array_wo_remove(priv->e_waiting_queue, wait_idx, NULL);
						wait_idx--;
					} else {
						uint64_t req_wait_time = req_time_out-time_now;
						if (req_wait_time<next_wait_time) {
							next_wait_time = req_wait_time;
						}
					}
				}
			}
//			while(cat_array_wo_size(priv->e_waiting_queue)>0) {
//				WorRequest *wait_req = WOR_REQUEST(cat_array_wo_get(priv->e_waiting_queue, 0));
//				uint64_t req_time_out = wor_request_get_time_out(wait_req);
//				cat_log_trace("req_time_out:%lld, time_now=%lld", req_time_out, time_now);
//				if (req_time_out<=time_now) {
//					cat_array_wo_append(e_transfer_list, G_OBJECT(wait_req));
//					cat_array_wo_remove(priv->e_waiting_queue, 0, NULL);
//				} else {
//					next_wait_time = req_time_out-time_now;
//					if (next_wait_time>30000) {
//						next_wait_time = 30000;
//					}
//					break;
//				}
//			}
		}

		/* transfer list is empty then lets wait */
		if (cat_array_wo_size(e_transfer_list)==0) {
			cat_log_debug("waiting:time - %ld", next_wait_time);
			/* next_wait_time is the time to wait for the next request which times out or just 30000 */
			cat_lock_wait_timed(priv->consumer_lock, next_wait_time);
		}
		cat_lock_unlock(priv->consumer_lock);


		/* move all request in the transfer list to the end of the dispatcher_queue */
		if (cat_array_wo_size(e_transfer_list)>0) {
			cat_log_debug("transfering updating states:%d", cat_array_wo_size(e_transfer_list));
			CatIIterator *iter = cat_array_wo_iterator(e_transfer_list);
			while(cat_iiterator_has_next(iter)) {
				WorRequest *wait_req = WOR_REQUEST(cat_iiterator_next(iter));
				cat_log_detail("wait_req=%p", wait_req);
				wor_request_update_states(wait_req);
			}
			cat_unref_ptr(iter);

			cat_log_debug("transfering-cnt:%d", cat_array_wo_size(e_transfer_list));
			cat_lock_lock(priv->dispatch_lock);
			cat_array_wo_append_all(priv->e_dispatch_queue, e_transfer_list);
			cat_array_wo_clear(e_transfer_list);
			cat_lock_notify_all(priv->dispatch_lock);
			cat_lock_unlock(priv->dispatch_lock);
		}
	}
	cat_unref_ptr(e_transfer_list);
	cat_lock_lock(priv->consumer_lock);
	priv->stop_response = TRUE;
	cat_lock_notify_all(priv->consumer_lock);
	cat_lock_unlock(priv->consumer_lock);
	return NULL;
}



static WorRequest *l_provider_next_request(WorIProvider *self, int64_t *runtime) {
	WorRequest *result = NULL;
	WorService *service = WOR_SERVICE(self);
	WorServicePrivate *priv = WOR_SERVICE_GET_PRIVATE(service);
	if (priv->load<256) {
		if (*runtime>250) {
			int64_t sp = *runtime - ((priv->load * *runtime)/256);
			if (sp>500) {
				sp = 500;
			}
			sp += cat_date_current_time();
			cat_lock_lock(priv->dispatch_lock);
			while(!priv->stop_request) {
				int64_t st = sp - cat_date_current_time();
				if (st<3) {
					break;
				}
				cat_lock_wait_timed(priv->dispatch_lock, st);
			}
			cat_lock_unlock(priv->dispatch_lock);
			*runtime = 0;
		}
	}
	if (!priv->stop_request) {
		cat_lock_lock(priv->dispatch_lock);
		if (cat_array_wo_size(priv->e_dispatch_queue)>0) {
			cat_array_wo_remove(priv->e_dispatch_queue, 0, (GObject **) (&result));
		} else {
			cat_lock_wait_timed(priv->dispatch_lock, 30000l);
		}
		cat_lock_unlock(priv->dispatch_lock);
	}
//	if (result) {
//		cat_log_print("INC", "return request:%p - %s", result, g_type_name_from_instance((GTypeInstance *) result))
//	}
	return result;
}



static void l_provider_increase_usage(WorIProvider *self) {
	WorService *service = WOR_SERVICE(self);
	WorServicePrivate *priv = WOR_SERVICE_GET_PRIVATE(service);
	cat_lock_lock(priv->consumer_lock);
	priv->usage++;
	cat_lock_unlock(priv->consumer_lock);
}

static void l_provider_decrease_usage(WorIProvider *self) {
	WorService *service = WOR_SERVICE(self);
	WorServicePrivate *priv = WOR_SERVICE_GET_PRIVATE(service);
	cat_lock_lock(priv->consumer_lock);
	priv->usage--;
	cat_lock_notify(priv->consumer_lock);
	cat_lock_unlock(priv->consumer_lock);
}

static gboolean l_provider_is_active(WorIProvider *self) {
	WorService *service = WOR_SERVICE(self);
	WorServicePrivate *priv = WOR_SERVICE_GET_PRIVATE(service);
	return !priv->stop_request;
}


static void l_provider_iface_init(WorIProviderInterface *iface) {
	iface->next_request = l_provider_next_request;
	iface->increaseUsage = l_provider_increase_usage;
	iface->decreaseUsage = l_provider_decrease_usage;
	iface->isActive = l_provider_is_active;

}


void l_post_box_post_request(WorIRequestPostBox *self, WorRequest *post) {
	WorService *service = WOR_SERVICE(self);
	wor_service_post_request(service, post);
}


static void l_post_box_iface_init(WorIRequestPostBoxInterface *iface) {
	iface->postRequest = l_post_box_post_request;
}
