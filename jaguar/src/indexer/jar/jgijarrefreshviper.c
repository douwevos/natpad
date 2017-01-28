/*
   File:    jgijarrefreshviper.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 17, 2014
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

#include "jgijarrefreshviper.h"
#include "jgijarentryprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JgiJarRefreshViper"
#include <logging/catlog.h>

struct _JgiJarRefreshViperPrivate {
	JgiJarEntry *entry;
	VipSnapshot *snapshot;
};


G_DEFINE_TYPE(JgiJarRefreshViper, jgi_jar_refresh_viper, WOR_TYPE_REQUEST)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jgi_jar_refresh_viper_class_init(JgiJarRefreshViperClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JgiJarRefreshViperPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void jgi_jar_refresh_viper_init(JgiJarRefreshViper *instance) {
	JgiJarRefreshViperPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JGI_TYPE_JAR_REFRESH_VIPER, JgiJarRefreshViperPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JgiJarRefreshViper *instance = JGI_JAR_REFRESH_VIPER(object);
	JgiJarRefreshViperPrivate *priv = instance->priv;
	cat_unref_ptr(priv->entry);
	cat_unref_ptr(priv->snapshot);
	G_OBJECT_CLASS(jgi_jar_refresh_viper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jgi_jar_refresh_viper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JgiJarRefreshViper *jgi_jar_refresh_viper_new(JgiJarEntry *entry, VipSnapshot *snapshot) {
	JgiJarRefreshViper *result = g_object_new(JGI_TYPE_JAR_REFRESH_VIPER, NULL);
	cat_ref_anounce(result);
	JgiJarRefreshViperPrivate *priv = result->priv;
	wor_request_construct((WorRequest *) result);
	priv->entry = cat_ref_ptr(entry);
	priv->snapshot = cat_ref_ptr(snapshot);
	return result;
}




static void l_run_request(WorRequest *request) {
	JgiJarRefreshViperPrivate *priv = JGI_JAR_REFRESH_VIPER_GET_PRIVATE(request);
	jgi_jar_entry_set_vip_snapshot(priv->entry, priv->snapshot);
}

