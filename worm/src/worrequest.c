/*
   File:    worrequest.c
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

#include "worrequest.h"
#include "woriconditionobserver.h"

#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorRequest"
#include <logging/catlog.h>

struct _WorRequestPrivate {
	CatLock *consumerLock;
	gboolean delaySet;
	uint64_t timeOut;
	gboolean isNotified;
	CatHashMapWo *conditionStateMap;
	CatLock *mapLock;
};


static void l_condition_observer_iface_init(WorIConditionObserverInterface *iface);

G_DEFINE_TYPE_WITH_CODE(WorRequest, wor_request, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(WorRequest)
		G_IMPLEMENT_INTERFACE(WOR_TYPE_ICONDITION_OBSERVER, l_condition_observer_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void wor_request_class_init(WorRequestClass *clazz) {
	clazz->tryCombine = NULL;
	clazz->addNotify = wor_request_add_notify_real;
	clazz->setNotified = wor_request_set_notified_real;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void wor_request_init(WorRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorRequest *instance = WOR_REQUEST(object);
	WorRequestPrivate *priv = wor_request_get_instance_private(instance);
	cat_unref_ptr(priv->consumerLock);
	cat_unref_ptr(priv->conditionStateMap);
	G_OBJECT_CLASS(wor_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	WorRequest *instance = WOR_REQUEST(object);
	WorRequestPrivate *priv = wor_request_get_instance_private(instance);
	cat_unref_ptr(priv->mapLock);
	G_OBJECT_CLASS(wor_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void wor_request_construct(WorRequest *request) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	cat_log_debug("request=%s, %p, priv=%p", g_type_name_from_instance((GTypeInstance *) request), request, priv);
	priv->consumerLock = NULL;
	priv->delaySet = FALSE;
	priv->timeOut = 0l;
	priv->isNotified = FALSE;
	priv->conditionStateMap = NULL;
	priv->mapLock = cat_lock_new();
}


gboolean wor_request_is_delayed(WorRequest *request) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	return priv->delaySet;
}

void wor_request_add_notify_real(WorRequest *request, CatLock *serviceConsumerLock) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	cat_ref_swap(priv->consumerLock, serviceConsumerLock);
}

void wor_request_add_notify(WorRequest *request, CatLock *serviceConsumerLock) {
	WOR_REQUEST_GET_CLASS(request)->addNotify(request, serviceConsumerLock);

	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	cat_log_debug("request:%s, %p, priv=%p", g_type_name_from_instance((GTypeInstance *) request), request, priv);
	cat_lock_lock(priv->mapLock);
	CatHashMapWo *localMap = priv->conditionStateMap==NULL ? NULL : cat_ref_ptr(priv->conditionStateMap);
	cat_lock_unlock(priv->mapLock);

	if (localMap) {
		CatArrayWo *map_values = cat_hash_map_wo_enlist_values(localMap, NULL);
		CatIIterator *iter = cat_array_wo_iterator(map_values);
		gboolean at_least_one_was_selected = FALSE;
		while(cat_iiterator_has_next(iter)) {
			WorConditionState *state = WOR_CONDITION_STATE(cat_iiterator_next(iter));
			if (wor_condition_state_peek(state)) {
				at_least_one_was_selected = TRUE;
				break;
			}
		}
		cat_unref_ptr(iter);
		cat_unref(localMap);
		cat_unref_ptr(map_values);

		if (at_least_one_was_selected) {
			priv->isNotified = TRUE;
		}
	}
}


void wor_request_set_notified_real(WorRequest *request) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	cat_log_debug("getting notified:priv->consumerLock=%p", priv->consumerLock);
	if (priv->consumerLock) {
		cat_lock_lock(priv->consumerLock);
		priv->isNotified = TRUE;
		cat_lock_notify_all(priv->consumerLock);
		cat_lock_unlock(priv->consumerLock);
	} else {
		cat_log_fatal("reporting but no consumerLock set");
	}
}

void wor_request_set_notified(WorRequest *request) {
	WOR_REQUEST_GET_CLASS(request)->setNotified(request);
}


void wor_request_set_time_out(WorRequest *request, uint64_t time_out) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	if (time_out<1000000) {
		cat_log_warn("possible incorrect time-out configured:%o", request);
	}
	priv->timeOut = time_out;
	priv->delaySet = TRUE;
}

uint64_t wor_request_get_time_out(WorRequest *request) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	return priv->timeOut;
}

gboolean wor_request_get_and_reset_notified(WorRequest *request) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	gboolean result = priv->isNotified;
	priv->isNotified = FALSE;
	return result;
}

void wor_request_add_condition(WorRequest *request, WorCondition *condition2add) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	cat_lock_lock(priv->mapLock);
	CatHashMapWo *newMap = priv->conditionStateMap;
	WorConditionState *condition_state = wor_condition_state_new(condition2add, WOR_ICONDITION_OBSERVER(request));
	if (newMap == NULL) {
		newMap = cat_hash_map_wo_new((GHashFunc) wor_condition_hash_code, (GEqualFunc) wor_condition_equals);
	} else {
		newMap = cat_hash_map_wo_clone(newMap, CAT_CLONE_DEPTH_MAIN);
	}

	cat_hash_map_wo_put(newMap, (GObject *) condition2add, (GObject *) condition_state);
	cat_ref_swap(priv->conditionStateMap, newMap);
	cat_unref_ptr(condition_state);
	cat_unref_ptr(newMap);
	cat_lock_unlock(priv->mapLock);
}

gboolean wor_request_remove_condition(WorRequest *request, WorCondition *condition2remove) {
	gboolean result = FALSE;
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	cat_lock_lock(priv->mapLock);
	if (priv->conditionStateMap!=NULL) {
		if (cat_hash_map_wo_get(priv->conditionStateMap, (GObject *) condition2remove)!=NULL) {
			result = TRUE;
			CatHashMapWo *newMap = cat_hash_map_wo_clone(priv->conditionStateMap, CAT_CLONE_DEPTH_MAIN);
			GObject *obj = cat_hash_map_wo_remove(priv->conditionStateMap, (GObject *) condition2remove);
			cat_unref_ptr(obj);
			cat_ref_sink_swap(priv->conditionStateMap, newMap);
		}
	} else {
		cat_log_warn("condition not in state-map:%p", condition2remove);
	}
	cat_lock_unlock(priv->mapLock);
	return result;
}

void wor_request_update_states(WorRequest *request) {
	WorRequestPrivate *priv = wor_request_get_instance_private(request);
	cat_log_debug("request:%s, %p, priv=%p", g_type_name_from_instance((GTypeInstance *) request), request, priv);
	cat_lock_lock(priv->mapLock);
	CatHashMapWo *localMap = priv->conditionStateMap==NULL ? NULL : cat_ref_ptr(priv->conditionStateMap);
	cat_lock_unlock(priv->mapLock);

	if (localMap) {
		CatArrayWo *map_values = cat_hash_map_wo_enlist_values(localMap, NULL);
		CatIIterator *iter = cat_array_wo_iterator(map_values);
		while(cat_iiterator_has_next(iter)) {
			WorConditionState *state = WOR_CONDITION_STATE(cat_iiterator_next(iter));
			wor_condition_state_hold(state);
		}
		cat_unref_ptr(iter);
		cat_unref(localMap);
		cat_unref_ptr(map_values);
	}
}


static void l_condition_observer_condition_reporting(WorIConditionObserver *self) {
	WorRequest *request = WOR_REQUEST(self);
	wor_request_set_notified(request);
}

static void l_condition_observer_iface_init(WorIConditionObserverInterface *iface) {
	iface->condition_reporting = l_condition_observer_condition_reporting;
}
