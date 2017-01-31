/*
   File:    chaasyncbasic.c
   Project: chameleon
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

#include "chaasyncbasic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaAsyncBasic"
#include <logging/catlog.h>

struct _ChaAsyncBasicPrivate {
	CatLock *lock;
	gboolean success;
	gboolean finished;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaAsyncBasic, cha_async_basic, CHA_TYPE_IO_ASYNC,
		G_ADD_PRIVATE(ChaAsyncBasic)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_finished(ChaIOAsync *async, gboolean success, GError *error);

static void cha_async_basic_class_init(ChaAsyncBasicClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaIOAsyncClass *async_class = CHA_IO_ASYNC_CLASS(clazz);
	async_class->finished = l_finished;
}

static void cha_async_basic_init(ChaAsyncBasic *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaAsyncBasic *instance = CHA_ASYNC_BASIC(object);
	ChaAsyncBasicPrivate *priv = cha_async_basic_get_instance_private(instance);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(cha_async_basic_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_async_basic_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void cha_async_basic_construct(ChaAsyncBasic *async) {
	cha_io_async_construct((ChaIOAsync *) async);
	ChaAsyncBasicPrivate *priv = cha_async_basic_get_instance_private(async);
	priv->lock = cat_lock_new();
	priv->finished = FALSE;
	priv->success = FALSE;

}

ChaAsyncBasic *cha_async_basic_new() {
	ChaAsyncBasic *result = g_object_new(CHA_TYPE_ASYNC_BASIC, NULL);
	cat_ref_anounce(result);
	cha_async_basic_construct(result);
	return result;
}


gboolean cha_async_basic_wait(ChaAsyncBasic *async_basic, long time_to_wait_ms) {
	ChaAsyncBasicPrivate *priv = cha_async_basic_get_instance_private(async_basic);
	gboolean result = FALSE;
	cat_lock_lock(priv->lock);
	int64_t time_out = cat_date_current_time() + time_to_wait_ms;
	while(TRUE) {
		if (priv->finished) {
			result = priv->success;
			break;
		}
		int64_t next = cat_date_current_time();
		if (next>=time_out) {
			result = priv->success;
			break;
		}
		cat_lock_wait_timed(priv->lock, time_out-next);
	}
	cat_lock_unlock(priv->lock);
	return result;
}

static void l_finished(ChaIOAsync *async, gboolean success, GError *error) {
	ChaAsyncBasicPrivate *priv = cha_async_basic_get_instance_private(CHA_ASYNC_BASIC(async));
	cat_lock_lock(priv->lock);
	priv->success = success;
	priv->finished = TRUE;
	cat_lock_notify(priv->lock);
	cat_lock_unlock(priv->lock);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaAsyncBasic *instance = CHA_ASYNC_BASIC(self);
	ChaAsyncBasicPrivate *priv = cha_async_basic_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p finished=%d]", iname, self, priv->finished);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

