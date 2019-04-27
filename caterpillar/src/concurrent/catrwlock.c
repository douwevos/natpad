/*
   File:    catrwlock.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 7, 2012
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

#include "catrwlock.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatRWLock"
#include "../logging/catlog.h"

struct _CatRWLockPrivate {
	GRWLock rwLock;
	GCond condition;
};

G_DEFINE_TYPE_WITH_PRIVATE(CatRWLock, cat_rw_lock, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_rw_lock_class_init(CatRWLockClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_rw_lock_init(CatRWLock *instance) {
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(instance);
	g_rw_lock_init(&(priv->rwLock));
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cat_rw_lock_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	CatRWLock *instance = (CatRWLock *) object;
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(instance);
	g_cond_clear(&(priv->condition));
	G_OBJECT_CLASS(cat_rw_lock_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatRWLock *cat_rw_lock_new() {
	CatRWLock *result = g_object_new(CAT_TYPE_RW_LOCK, NULL);
	cat_ref_anounce(result);
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(result);
	g_cond_init(&(priv->condition));
	return result;
}


void cat_rw_lock_read_lock(CatRWLock *lock) {
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(lock);
//	int64_t start = cat_date_current_time();
//	while(TRUE) {
//		if (g_rw_lock_reader_trylock(&(priv->rwLock))) {
//			return;
//		}
//		int64_t end = cat_date_current_time();
//		if ((start-end)>10000) {
//			cat_stacktrace_print();
//			sleep(3);
////			l_wait_timed(priv, 2500);
//		} else {
//			sleep(1);
////			l_wait_timed(priv, 250);
//		}
//	}

	g_rw_lock_reader_lock(&(priv->rwLock));
}

void cat_rw_lock_write_lock(CatRWLock *lock) {
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(lock);
//	int64_t start = cat_date_current_time();
//	while(TRUE) {
//		if (g_rw_lock_writer_trylock(&(priv->rwLock))) {
//			return;
//		}
//		int64_t end = cat_date_current_time();
//		if ((start-end)>10000) {
//			cat_stacktrace_print();
//			sleep(3);
////			l_wait_timed(priv, 2500);
//		} else {
//			sleep(1);
////			l_wait_timed(priv, 250);
//		}
//	}


	g_rw_lock_writer_lock(&(priv->rwLock));
}

gboolean cat_rw_lock_read_trylock(CatRWLock *lock) {
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(lock);
	return g_rw_lock_reader_trylock(&(priv->rwLock));
}

gboolean cat_rw_lock_write_trylock(CatRWLock *lock) {
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(lock);
	return g_rw_lock_writer_trylock(&(priv->rwLock));
}

void cat_rw_lock_read_unlock(CatRWLock *lock) {
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(lock);
	g_rw_lock_reader_unlock(&(priv->rwLock));
}

void cat_rw_lock_write_unlock(CatRWLock *lock) {
	CatRWLockPrivate *priv = cat_rw_lock_get_instance_private(lock);
	g_rw_lock_writer_unlock(&(priv->rwLock));
}
