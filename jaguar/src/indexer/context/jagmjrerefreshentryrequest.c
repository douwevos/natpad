/*
   File:    jagmjrerefreshentryrequest.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#include "jagmjrerefreshentryrequest.h"
#include "jgijreentryprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagmJreRefreshEntryRequest"
#include <logging/catlog.h>

struct _JagmJreRefreshEntryRequestPrivate {
	JgiJreEntry *jre_entry;
	JgiJarEntry *jar_entry;
	int last_jar_entry_index;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagmJreRefreshEntryRequest, jagm_jre_refresh_entry_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(JagmJreRefreshEntryRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jagm_jre_refresh_entry_request_class_init(JagmJreRefreshEntryRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void jagm_jre_refresh_entry_request_init(JagmJreRefreshEntryRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagmJreRefreshEntryRequest *instance = JAGM_JRE_REFRESH_ENTRY_REQUEST(object);
	JagmJreRefreshEntryRequestPrivate *priv = jagm_jre_refresh_entry_request_get_instance_private(instance);
	cat_unref_ptr(priv->jre_entry);
	cat_unref_ptr(priv->jar_entry);
	G_OBJECT_CLASS(jagm_jre_refresh_entry_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagm_jre_refresh_entry_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagmJreRefreshEntryRequest *jagm_jre_refresh_entry_request_new(JgiJreEntry *jre_entry, JgiJarEntry *jar_entry, int last_jar_entry_index) {
	JagmJreRefreshEntryRequest *result = g_object_new(JAGM_TYPE_JRE_REFRESH_ENTRY_REQUEST, NULL);
	cat_ref_anounce(result);
	JagmJreRefreshEntryRequestPrivate *priv = jagm_jre_refresh_entry_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->jre_entry = cat_ref_ptr(jre_entry);
	priv->jar_entry = cat_ref_ptr(jar_entry);
	priv->last_jar_entry_index = last_jar_entry_index;
	return result;
}




static void l_run_request(WorRequest *request) {
	JagmJreRefreshEntryRequestPrivate *priv = jagm_jre_refresh_entry_request_get_instance_private((JagmJreRefreshEntryRequest *) request);
	jgi_jre_entry_refresh_jar_entry(priv->jre_entry, priv->jar_entry, priv->last_jar_entry_index);
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

