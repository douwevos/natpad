/*
   File:    jagupdatejrelistrequest.c
   Project: jaguar
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

#include "jagupdatejrelistrequest.h"
#include "jagindexerjremapprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagUpdateJreListRequest"
#include <logging/catlog.h>

struct _JagUpdateJreListRequestPrivate {
	JagIndexerJreMap *jre_map;
	CatArrayWo *a_jre_list;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagUpdateJreListRequest, jag_update_jre_list_request, WOR_TYPE_REQUEST)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_update_jre_list_request_class_init(JagUpdateJreListRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void jag_update_jre_list_request_init(JagUpdateJreListRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagUpdateJreListRequest *instance = JAG_UPDATE_JRE_LIST_REQUEST(object);
	JagUpdateJreListRequestPrivate *priv = jag_update_jre_list_request_get_instance_private(instance);
	cat_unref_ptr(priv->a_jre_list);
	cat_unref_ptr(priv->jre_map);
	G_OBJECT_CLASS(jag_update_jre_list_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_update_jre_list_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagUpdateJreListRequest *jag_update_jre_list_request_new(JagIndexerJreMap *jre_map, CatArrayWo *a_jre_list) {
	JagUpdateJreListRequest *result = g_object_new(JAG_TYPE_UPDATE_JRE_LIST_REQUEST, NULL);
	cat_ref_anounce(result);
	JagUpdateJreListRequestPrivate *priv = jag_update_jre_list_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->a_jre_list = cat_ref_ptr(a_jre_list);
	priv->jre_map = cat_ref_ptr(jre_map);
	return result;
}

static void l_run_request(WorRequest *request) {
	JagUpdateJreListRequest *instance= JAG_UPDATE_JRE_LIST_REQUEST(request);
	JagUpdateJreListRequestPrivate *priv = jag_update_jre_list_request_get_instance_private(instance);

	jag_index_jre_map_apply_new_jre_list(priv->jre_map, priv->a_jre_list);
//	CatIIterator *iter = cat_array_wo_iterator(a_priv->jre_list);
//	while(cat_iiterator_has_next(iter)) {
//		JagJdkOrJre *jag_jdk_or_jre = (JagJdkOrJre *) cat_iiterator_next(iter);
//		CatStringWo *name = jag_jdk_or_jre_get_name(jag_jdk_or_jre);
//
//	}
//	cat_unref_ptr(iter);
}
