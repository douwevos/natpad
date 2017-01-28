/*
   File:    worspawnedrequest.c
   Project: worm
   Author:  Douwe Vos
   Date:    Feb 28, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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



#include "worspawnedrequest.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorSpawnedRequest"
#include <logging/catlog.h>

struct _WorSpawnedRequestPrivate {
	volatile gboolean finished;
	WorCondition *finished_condition;
	WorRequest *delegate_request;
};

G_DEFINE_TYPE (WorSpawnedRequest, wor_spawned_request, WOR_TYPE_REQUEST)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void l_run_request(WorRequest *request);

static void wor_spawned_request_class_init(WorSpawnedRequestClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(WorSpawnedRequestPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

	WorRequestClass *req_clazz = WOR_REQUEST_CLASS(clazz);
	req_clazz->runRequest = l_run_request;
}

static void wor_spawned_request_init(WorSpawnedRequest *instance) {
	WorSpawnedRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, WOR_TYPE_SPAWNED_REQUEST, WorSpawnedRequestPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorSpawnedRequest *instance = WOR_SPAWNED_REQUEST(object);
	WorSpawnedRequestPrivate *priv = instance->priv;
	cat_unref_ptr(priv->delegate_request);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_unref_ptr(priv->finished_condition);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

WorSpawnedRequest *wor_spawned_request_new(WorRequest *delegate_request) {
	WorSpawnedRequest *result = g_object_new(WOR_TYPE_SPAWNED_REQUEST, NULL);
	cat_ref_anounce(result);
	WorSpawnedRequestPrivate *priv = result->priv;
	priv->finished = FALSE;
	priv->delegate_request = cat_ref_ptr(delegate_request);
	priv->finished_condition = wor_condition_new();
	wor_request_construct((WorRequest *) result);
	return result;
}


static void l_run_request(WorRequest *request) {
	WorSpawnedRequest *spawned_request = WOR_SPAWNED_REQUEST(request);
	WorSpawnedRequestPrivate *priv = spawned_request->priv;
	cat_log_debug("request:%p, running delegate:%p", request, priv->delegate_request);
	WOR_REQUEST_GET_CLASS(priv->delegate_request)->runRequest(priv->delegate_request);
	priv->finished = TRUE;
	cat_log_debug("request:%p, setting condition up");
	wor_condition_up(priv->finished_condition);
	cat_log_debug("request:%p, done");
}


WorCondition *wor_spawned_request_get_finished_condition(WorSpawnedRequest *spawned_request) {
	return spawned_request->priv->finished_condition;
}

gboolean wor_spawned_request_get_is_finished(WorSpawnedRequest *spawned_request) {
	return spawned_request->priv->finished;
}


WorRequest *wor_spawned_request_get_delegate(WorSpawnedRequest *spawned_request) {
	return spawned_request->priv->delegate_request;
}
