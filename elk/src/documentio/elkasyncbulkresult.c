/*
   File:    elkasyncbulkresult.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 28, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "elkasyncbulkresult.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkAsyncBulkResult"
#include <logging/catlog.h>

struct _ElkAsyncBulkResultPrivate {
	int expect_count;
	CatLock *lock;
	int error_count;
	int success_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkAsyncBulkResult, elk_async_bulk_result, CHA_TYPE_IO_ASYNC,
		G_ADD_PRIVATE(ElkAsyncBulkResult)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_async_finished(ChaIOAsync *async, gboolean success, GError *error);


static void elk_async_bulk_result_class_init(ElkAsyncBulkResultClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaIOAsyncClass *async_class = CHA_IO_ASYNC_CLASS(clazz);
	async_class->finished = l_async_finished;
}

static void elk_async_bulk_result_init(ElkAsyncBulkResult *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkAsyncBulkResult *instance = ELK_ASYNC_BULK_RESULT(object);
	ElkAsyncBulkResultPrivate *priv = elk_async_bulk_result_get_instance_private(instance);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(elk_async_bulk_result_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_async_bulk_result_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkAsyncBulkResult *elk_async_bulk_result_new(int expect_count) {
	ElkAsyncBulkResult *result = g_object_new(ELK_TYPE_ASYNC_BULK_RESULT, NULL);
	cat_ref_anounce(result);
	ElkAsyncBulkResultPrivate *priv = elk_async_bulk_result_get_instance_private(result);
//	cha_io_async_construct((ChaIOAsync *) result);
	priv->expect_count = expect_count;
	priv->lock = cat_lock_new();
	priv->success_count = 0;
	priv->error_count = 0;
	return result;
}

gboolean elk_async_bulk_result_wait(ElkAsyncBulkResult *bulk_result) {
	ElkAsyncBulkResultPrivate *priv = elk_async_bulk_result_get_instance_private(bulk_result);
	gboolean result = FALSE;
	cat_lock_lock(priv->lock);
	int64_t end = cat_date_current_time()+60000;
	while(TRUE) {
		if (priv->success_count+priv->error_count==priv->expect_count) {
			result = TRUE;
			break;
		}
		int64_t next = cat_date_current_time();
		if (next>=end) {
			break;
		}
		cat_lock_wait_timed(priv->lock, end-next);
	}
	cat_lock_unlock(priv->lock);
	return result;
}

static void l_async_finished(ChaIOAsync *async, gboolean success, GError *error) {
	ElkAsyncBulkResultPrivate *priv = elk_async_bulk_result_get_instance_private(ELK_ASYNC_BULK_RESULT(async));
	cat_lock_lock(priv->lock);
	if (success) {
		priv->success_count++;
	} else {
		priv->error_count++;
	}
	if (priv->success_count+priv->error_count==priv->expect_count) {
		cat_lock_notify_all(priv->lock);
	}
	cat_lock_unlock(priv->lock);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
