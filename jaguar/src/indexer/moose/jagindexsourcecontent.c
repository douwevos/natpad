/*
   File:    jagindexsourcecontent.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 5, 2014
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

#include "jagindexsourcecontent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagIndexSourceContent"
#include <logging/catlog.h>

struct _JagIndexSourceContentPrivate {
	CatLock *lock;
	int jag_srcfile_content_version;
};


G_DEFINE_TYPE(JagIndexSourceContent, jag_index_source_content, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_index_source_content_class_init(JagIndexSourceContentClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JagIndexSourceContentPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_index_source_content_init(JagIndexSourceContent *instance) {
	JagIndexSourceContentPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_INDEX_SOURCE_CONTENT, JagIndexSourceContentPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagIndexSourceContent *instance = JAG_INDEX_SOURCE_CONTENT(object);
	JagIndexSourceContentPrivate *priv = instance->priv;
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(jag_index_source_content_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_index_source_content_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagIndexSourceContent *jag_index_source_content_new() {
	JagIndexSourceContent *result = g_object_new(JAG_TYPE_INDEX_SOURCE_CONTENT, NULL);
	cat_ref_anounce(result);
	JagIndexSourceContentPrivate *priv = result->priv;
	priv->lock = cat_lock_new();
	priv->jag_srcfile_content_version = -1;
	return result;
}

gboolean jag_index_source_content_need_to_update(JagIndexSourceContent *index_source, int jag_srcfile_content_version) {
	JagIndexSourceContentPrivate *priv = JAG_INDEX_SOURCE_CONTENT_GET_PRIVATE(index_source);
	gboolean result = FALSE;
	cat_lock_lock(priv->lock);
	cat_log_debug("jag_srcfile_content_version=%d, priv->jag_srcfile_content_version=%d", jag_srcfile_content_version, priv->jag_srcfile_content_version);
	if (jag_srcfile_content_version>priv->jag_srcfile_content_version) {
		priv->jag_srcfile_content_version = jag_srcfile_content_version;
		result = TRUE;
	}
	cat_lock_unlock(priv->lock);
	return result;

}
