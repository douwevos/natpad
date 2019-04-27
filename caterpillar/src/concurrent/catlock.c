/*
   File:    catlock.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jul 1, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "catlock.h"
#include <sys/time.h>
#include "../catlib.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatLock"
#include "../logging/catlog.h"

struct _CatLockPrivate {
	GMutex mutex;
	GCond condition;
};

G_DEFINE_TYPE_WITH_PRIVATE(CatLock, cat_lock, G_TYPE_OBJECT)

static void l_dispose(GObject *object);

static void cat_lock_class_init(CatLockClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void cat_lock_init(CatLock *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatLock *instance = CAT_LOCK(object);
	CatLockPrivate *priv = cat_lock_get_instance_private(instance);
//	if (instance->mutex) {
//#ifdef CAT_REF_MONITORING
//		if (g_mutex_trylock(instance->mutex)) {
//			cat_stacktrace_print();
//		}
//#endif
		g_mutex_clear(&(priv->mutex));
//		instance->mutex = NULL;
//	}

//	if (instance->condition) {
		g_cond_clear(&(priv->condition));
//	}
	cat_log_detail("end-dispose:%p", object);
}


CatLock *cat_lock_new() {
	CatLock *result = g_object_new(CAT_TYPE_LOCK, NULL);
	cat_ref_anounce(result);
	CatLockPrivate *priv = cat_lock_get_instance_private(result);
	g_mutex_init(&(priv->mutex));
	g_cond_init(&(priv->condition));
	return result;
}


void cat_lock_lock(CatLock *lock) {
	CatLockPrivate *priv = cat_lock_get_instance_private(lock);
//	int64_t start = cat_date_current_time();
//	while(TRUE) {
//		gboolean could_lock = g_mutex_trylock(lock->mutex);
//		if (could_lock) {
//			return;
//		}
//		int64_t end = cat_date_current_time();
//		if ((start-end)>10000) {
//			cat_stacktrace_print();
//			sleep(3);
////			cat_lock_wait_timed(lock, 2500);
//		} else {
//			sleep(1);
////			cat_lock_wait_timed(lock, 250);
//		}
//	}

	g_mutex_lock(&(priv->mutex));
}

void cat_lock_unlock(CatLock *lock) {
	CatLockPrivate *priv = cat_lock_get_instance_private(lock);
	g_mutex_unlock(&(priv->mutex));
}

void cat_lock_wait(CatLock *lock) {
	CatLockPrivate *priv = cat_lock_get_instance_private(lock);
	g_cond_wait(&(priv->condition), &(priv->mutex));
}

gboolean cat_lock_wait_timed(CatLock *lock, int64_t ms) {
	CatLockPrivate *priv = cat_lock_get_instance_private(lock);
	gint64 end_time = g_get_monotonic_time () + (ms * G_TIME_SPAN_SECOND)/1000l;
	return g_cond_wait_until(&(priv->condition), &(priv->mutex), end_time);
}



void cat_lock_notify(CatLock *lock) {
	CatLockPrivate *priv = cat_lock_get_instance_private(lock);
	g_cond_signal(&(priv->condition));
}

void cat_lock_notify_all(CatLock *lock) {
	CatLockPrivate *priv = cat_lock_get_instance_private(lock);
	g_cond_broadcast(&(priv->condition));
}
