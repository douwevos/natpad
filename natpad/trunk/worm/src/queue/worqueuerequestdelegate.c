/*
   File:    worqueuerequestdelegate.c
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


#include "worqueuerequestdelegate.h"
#include "worqueueprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorQueueRequestDelegate"
#include <logging/catlog.h>

struct _WorQueueRequestDelegatePrivate {
	WorRequest *request;
	WorQueue *queue;
	gboolean did_timeout;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(WorQueueRequestDelegate, wor_queue_request_delegate, WOR_TYPE_REQUEST, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void wor_queue_request_delegate_class_init(WorQueueRequestDelegateClass *clazz) {
	g_type_class_add_private(clazz, sizeof(WorQueueRequestDelegatePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_clazz = WOR_REQUEST_CLASS(clazz);
	wor_clazz->runRequest = l_run_request;
}

static void wor_queue_request_delegate_init(WorQueueRequestDelegate *instance) {
	WorQueueRequestDelegatePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, WOR_TYPE_QUEUE_REQUEST_DELEGATE, WorQueueRequestDelegatePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorQueueRequestDelegate *instance = WOR_QUEUE_REQUEST_DELEGATE(object);
	WorQueueRequestDelegatePrivate *priv = instance->priv;
	cat_unref_ptr(priv->request);
	cat_unref_ptr(priv->queue);
	G_OBJECT_CLASS(wor_queue_request_delegate_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(wor_queue_request_delegate_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


WorQueueRequestDelegate *wor_queue_request_delegate_new(struct _WorQueue *queue, WorRequest *request, gboolean with_timeout) {
	WorQueueRequestDelegate *result = g_object_new(WOR_TYPE_QUEUE_REQUEST_DELEGATE, NULL);
	cat_ref_anounce(result);
	WorQueueRequestDelegatePrivate *priv = result->priv;
	wor_request_construct((WorRequest *) result);
	priv->queue = cat_ref_ptr(queue);
	priv->request = cat_ref_ptr(request);
	priv->did_timeout = with_timeout;
	return result;
}



static void l_run_request(WorRequest *request) {
	WorQueueRequestDelegatePrivate *priv = WOR_QUEUE_REQUEST_DELEGATE_GET_PRIVATE(request);
	cat_log_debug("request=%o, did_timeout=%d", priv->request, priv->did_timeout);
	if (priv->did_timeout) {
		priv->did_timeout = FALSE;
		wor_queue_repost(priv->queue, (WorQueueRequestDelegate *) request);
	} else {
		WOR_REQUEST_GET_CLASS(priv->request)->runRequest(priv->request);
		wor_queue_forward_next(priv->queue);
	}
	cat_log_debug("request=%o, done", priv->request);
}






/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	WorQueueRequestDelegatePrivate *priv = WOR_QUEUE_REQUEST_DELEGATE_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:timedout=%d, %o]", iname, self, priv->did_timeout, priv->request);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

