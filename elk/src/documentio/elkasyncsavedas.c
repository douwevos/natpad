/*
   File:    elkasyncsavedas.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 4, 2015
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

#include "elkasyncsavedas.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkAsyncSavedAs"
#include <logging/catlog.h>

struct _ElkAsyncSavedAsPrivate {
	ElkDocumentBin *document_bin;
	CatStringWo *path;
	ChaIOAsync *async;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkAsyncSavedAs, elk_async_saved_as, CHA_TYPE_IO_ASYNC,
		G_ADD_PRIVATE(ElkAsyncSavedAs)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_async_finished(ChaIOAsync *async, gboolean success, GError **error);

static void elk_async_saved_as_class_init(ElkAsyncSavedAsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaIOAsyncClass *ioas_class = CHA_IO_ASYNC_CLASS(clazz);
	ioas_class->finished = l_async_finished;
}

static void elk_async_saved_as_init(ElkAsyncSavedAs *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkAsyncSavedAs *instance = ELK_ASYNC_SAVED_AS(object);
	ElkAsyncSavedAsPrivate *priv = elk_async_saved_as_get_instance_private(instance);
	cat_unref_ptr(priv->document_bin);
	cat_unref_ptr(priv->path);
	cat_unref_ptr(priv->async);
	G_OBJECT_CLASS(elk_async_saved_as_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_async_saved_as_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkAsyncSavedAs *elk_async_saved_as_new(ElkDocumentBin *document_bin, CatStringWo *path, ChaIOAsync *async) {
	ElkAsyncSavedAs *result = g_object_new(ELK_TYPE_ASYNC_SAVED_AS, NULL);
	cat_ref_anounce(result);
	ElkAsyncSavedAsPrivate *priv = elk_async_saved_as_get_instance_private(result);
	priv->document_bin = cat_ref_ptr(document_bin);
	priv->path = cat_ref_ptr(path);
	priv->async = cat_ref_ptr(async);
	return result;
}

static void l_async_finished(ChaIOAsync *async, gboolean success, GError **error) {
	ElkAsyncSavedAsPrivate *priv = elk_async_saved_as_get_instance_private((ElkAsyncSavedAs *) async);
	if (success) {
		elk_document_bin_set_path(priv->document_bin, priv->path);
	}

	if (priv->async) {
		cha_io_async_finished(priv->async, success, error);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkAsyncSavedAs *instance = ELK_ASYNC_SAVED_AS(self);
	ElkAsyncSavedAsPrivate *priv = elk_async_saved_as_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p document_bin=%p]", iname, self, priv->document_bin);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
