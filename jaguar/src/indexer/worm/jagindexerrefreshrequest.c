///*
//   File:    jagindexerrefreshrequest.c
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Aug 16, 2012
//   Web:     http://www.natpad.net/
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2012 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//
//
//#include "jagindexerrefreshrequest.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "JagIndexerRefreshRequest"
//#include <logging/catlog.h>
//
//struct _JagIndexerRefreshRequestPrivate {
//	JagIndexer *indexer;
//	WorCondition *condition;
//
//	CatLock *stop_lock;
//	volatile gboolean running;
//	volatile gboolean has_stopped;
//};
//
//G_DEFINE_TYPE (JagIndexerRefreshRequest, jag_indexer_refresh_request, WOR_TYPE_REQUEST)
//
//static gpointer parent_class = NULL;
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void l_run_request(WorRequest *request);
//
//
//static void jag_indexer_refresh_request_class_init(JagIndexerRefreshRequestClass *clazz) {
//	parent_class = g_type_class_peek_parent(clazz);
//	g_type_class_add_private(clazz, sizeof(JagIndexerRefreshRequestPrivate));
//
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//
//	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
//	worm_clazz->runRequest = l_run_request;
//}
//
//static void jag_indexer_refresh_request_init(JagIndexerRefreshRequest *instance) {
//	JagIndexerRefreshRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_INDEXER_REFRESH_REQUEST, JagIndexerRefreshRequestPrivate);
//	instance->priv = priv;
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	JagIndexerRefreshRequest *instance = JAG_INDEXER_REFRESH_REQUEST(object);
//	JagIndexerRefreshRequestPrivate *priv = instance->priv;
//	if (priv->condition) {
//		WorCondition *condition = priv->condition;
//		priv->condition = NULL;
//		wor_request_remove_condition(WOR_REQUEST(instance), condition);
//	}
//	cat_unref_ptr(priv->stop_lock);
//	priv->condition = NULL;
//	G_OBJECT_CLASS(parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//JagIndexerRefreshRequest *jag_indexer_refresh_request_new(JagIndexer *indexer) {
//	JagIndexerRefreshRequest *result = g_object_new(JAG_TYPE_INDEXER_REFRESH_REQUEST, NULL);
//	cat_ref_anounce(result);
//	JagIndexerRefreshRequestPrivate *priv = result->priv;
//	wor_request_construct((WorRequest *) result);
//	priv->indexer = indexer;
//	priv->condition = wor_condition_new();
//
//	priv->stop_lock = cat_lock_new();
//	priv->running = TRUE;
//	priv->has_stopped = FALSE;
//	wor_request_add_condition(WOR_REQUEST(result), priv->condition);
//	cat_unref(priv->condition);
//	return result;
//}
//
//
//WorCondition *jag_indexer_refresh_request_get_condition(JagIndexerRefreshRequest *request) {
//	return JAG_INDEXER_REFRESH_REQUEST_GET_PRIVATE(request)->condition;
//}
//
//void jag_indexer_refresh_request_stop(JagIndexerRefreshRequest *request) {
//	JagIndexerRefreshRequestPrivate *priv = JAG_INDEXER_REFRESH_REQUEST_GET_PRIVATE(request);
//	cat_log_debug("stop requested");
//	priv->running = FALSE;
//	wor_condition_up(priv->condition);
//
//	// we wait for 3 seconds for the reuest handler to report back to us that it has finished
//	int64_t end = cat_date_current_time()+3000;
//	gboolean keep_waiting = TRUE;
//	while(keep_waiting) {
//		cat_lock_lock(priv->stop_lock);
//		if (priv->has_stopped) {
//			keep_waiting = FALSE;
//		} else {
//			cat_lock_wait_timed(priv->stop_lock, 1000);
//			if (cat_date_current_time()>=end) {
//				keep_waiting = FALSE;
//			}
//		}
//		cat_lock_unlock(priv->stop_lock);
//	}
//	cat_log_debug("has-stopped=%d", priv->has_stopped);
//}
//
//gboolean jag_indexer_refresh_request_is_running(JagIndexerRefreshRequest *request) {
//	return JAG_INDEXER_REFRESH_REQUEST_GET_PRIVATE(request)->running;
//}
//
//
//static void l_run_request(WorRequest *request) {
//	JagIndexerRefreshRequestPrivate *priv = JAG_INDEXER_REFRESH_REQUEST_GET_PRIVATE(request);
//	jag_indexer_refresh(priv->indexer);
//}
//
//
//void jag_indexer_refresh_request_report_has_stopped(JagIndexerRefreshRequest *request) {
//	JagIndexerRefreshRequestPrivate *priv = JAG_INDEXER_REFRESH_REQUEST_GET_PRIVATE(request);
//	cat_lock_lock(priv->stop_lock);
//	priv->has_stopped = TRUE;
//	cat_lock_notify(priv->stop_lock);
//	cat_lock_unlock(priv->stop_lock);
//}
//
//
//
