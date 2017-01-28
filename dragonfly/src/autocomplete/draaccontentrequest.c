/*
   File:    draaccontentrequest.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 19, 2015
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

#include "draaccontentrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraAcContentRequest"
#include <logging/catlog.h>

struct _DraAcContentRequestPrivate {
	DraIAcContentProvider *content_provider;
	DraAcContext *ac_context;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraAcContentRequest, dra_ac_content_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(DraAcContentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void dra_ac_content_request_class_init(DraAcContentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_clazz = WOR_REQUEST_CLASS(clazz);
	wor_clazz->runRequest = l_run_request;
}

static void dra_ac_content_request_init(DraAcContentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraAcContentRequest *instance = DRA_AC_CONTENT_REQUEST(object);
	DraAcContentRequestPrivate *priv = dra_ac_content_request_get_instance_private(instance);
	cat_unref_ptr(priv->ac_context);
	cat_unref_ptr(priv->content_provider);
	G_OBJECT_CLASS(dra_ac_content_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_ac_content_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraAcContentRequest *dra_ac_content_request_new(DraIAcContentProvider *content_provider, DraAcContext *ac_context) {
	DraAcContentRequest *result = g_object_new(DRA_TYPE_AC_CONTENT_REQUEST, NULL);
	cat_ref_anounce(result);
	DraAcContentRequestPrivate *priv = dra_ac_content_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->content_provider = cat_ref_ptr(content_provider);
	priv->ac_context = cat_ref_ptr(ac_context);
	dra_ac_context_provider_content_requested(ac_context);
	return result;
}


static void l_run_request(WorRequest *request) {
	DraAcContentRequestPrivate *priv = dra_ac_content_request_get_instance_private(DRA_AC_CONTENT_REQUEST(request));
	DRA_IAC_CONTENT_PROVIDER_GET_INTERFACE(priv->content_provider)->enlistContent(priv->content_provider, priv->ac_context);
	dra_ac_context_provider_finished(priv->ac_context);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraAcContentRequest *instance = DRA_AC_CONTENT_REQUEST(self);
	DraAcContentRequestPrivate *priv = dra_ac_content_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p ac_context=%o, content_provider=%o]", iname, self, priv->ac_context, priv->content_provider);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
