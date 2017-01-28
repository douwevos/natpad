/*
   File:    worspawnrequest.c
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

#include "worspawnrequest.h"
#include "worspawnedrequest.h"
#include "../worrequest.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorSpawnRequest"
#include <logging/catlog.h>

struct _WorSpawnRequestPrivate {
	WorIRequestPostBox *request_post_box;
	gboolean started;
	CatArrayWo *e_spawned_list;
//	int limit;
};

G_DEFINE_TYPE (WorSpawnRequest, wor_spawn_request, WOR_TYPE_REQUEST)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void l_run_request(WorRequest *request);

static void wor_spawn_request_class_init(WorSpawnRequestClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(WorSpawnRequestPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

	WorRequestClass *req_clazz = WOR_REQUEST_CLASS(clazz);
	req_clazz->runRequest = l_run_request;
}

static void wor_spawn_request_init(WorSpawnRequest *instance) {
	WorSpawnRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, WOR_TYPE_SPAWN_REQUEST, WorSpawnRequestPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorSpawnRequest *instance = WOR_SPAWN_REQUEST(object);
	WorSpawnRequestPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_spawned_list);
	cat_unref_ptr(priv->request_post_box);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void wor_spawn_request_construct(WorSpawnRequest *request, WorIRequestPostBox *request_post_box) {
	WorSpawnRequestPrivate *priv = WOR_SPAWN_REQUEST_GET_PRIVATE(request);
	priv->request_post_box = cat_ref_ptr(request_post_box);
	priv->started = FALSE;
	priv->e_spawned_list = cat_array_wo_new();
//	priv->limit = 4;
	wor_request_construct(WOR_REQUEST(request));
}


#define MAX_ACTIVE   30

static void l_run_request(WorRequest *request) {
	WorSpawnRequest *spawn_request = WOR_SPAWN_REQUEST(request);
	WorSpawnRequestPrivate *priv = spawn_request->priv;

	/* initialize spawning */
	if (!priv->started) {
		cat_log_debug("starting spawn");
		WOR_SPAWN_REQUEST_GET_CLASS(request)->startSpawn(spawn_request);
		priv->started = TRUE;
		wor_irequest_post_box_post_request(priv->request_post_box, request);
		return;
	}

	/* removed finished requests */
	cat_log_debug("removing finished spawned requests: size=%d", cat_array_wo_size(priv->e_spawned_list));
	int idx;
	for(idx=0; idx<cat_array_wo_size(priv->e_spawned_list); idx++) {
		WorSpawnedRequest *spawned_request = (WorSpawnedRequest *) cat_array_wo_get(priv->e_spawned_list, idx);
		if (wor_spawned_request_get_is_finished(spawned_request)) {
			cat_log_detail("spawned requests finished: %p", spawned_request);
			wor_request_remove_condition(request, wor_spawned_request_get_finished_condition(spawned_request));
			cat_array_wo_remove(priv->e_spawned_list, idx, NULL);
			idx--;
			continue;
		}
	}

	/* append new requests */
	cat_log_debug("adding more spawned requests: size=%d", cat_array_wo_size(priv->e_spawned_list));
//	while(cat_array_wo_size(priv->e_spawned_list)<MAX_ACTIVE && priv->limit!=0) {
	while(cat_array_wo_size(priv->e_spawned_list)<MAX_ACTIVE) {
		WorRequest *next_request = WOR_SPAWN_REQUEST_GET_CLASS(request)->nextSpawn(spawn_request);
		cat_log_detail("next_request=%p", next_request);
		if (next_request) {
			WorSpawnedRequest *spawned_request = wor_spawned_request_new(next_request);
			wor_request_add_condition(request, wor_spawned_request_get_finished_condition(spawned_request));
			cat_array_wo_append(priv->e_spawned_list, (GObject *) spawned_request);
			wor_irequest_post_box_post_request(priv->request_post_box, WOR_REQUEST(spawned_request));
			cat_unref_ptr(next_request);
			cat_unref_ptr(spawned_request);
//			priv->limit--;
		} else {
			cat_log_debug("cat_array_wo_size(priv->e_spawned_list)=%d", cat_array_wo_size(priv->e_spawned_list));
			break;
		}
	}

	/* test for continue or break */
	if (cat_array_wo_size(priv->e_spawned_list)>0) {
		cat_log_detail("reposting:%p", request);
		// TODO: isnt't the timeout missing here ???
		wor_irequest_post_box_post_request(priv->request_post_box, request);
	}
}




