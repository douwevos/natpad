/*
   File:    jagindexmodulecontent.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 7, 2014
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

#include "jagindexmodulecontent.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagIndexModuleContent"
#include <logging/catlog.h>

struct _JagIndexModuleContentPrivate {
	CatLock *lock;
	int jag_module_content_version;
};


static CatS jag_s_index_module_content_key = CAT_S_DEF("JagIndexModuleContent");

static void l_content_iface_init(MooIContentInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagIndexModuleContent, jag_index_module_content, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_index_module_content_class_init(JagIndexModuleContentClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JagIndexModuleContentPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_index_module_content_init(JagIndexModuleContent *instance) {
	JagIndexModuleContentPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_INDEX_MODULE_CONTENT_WO, JagIndexModuleContentPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagIndexModuleContent *instance = JAG_INDEX_MODULE_CONTENT_WO(object);
	JagIndexModuleContentPrivate *priv = instance->priv;
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(jag_index_module_content_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_index_module_content_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagIndexModuleContent *jag_index_module_content_new() {
	JagIndexModuleContent *result = g_object_new(JAG_TYPE_INDEX_MODULE_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagIndexModuleContentPrivate *priv = result->priv;
	priv->lock = cat_lock_new();
	return result;
}



gboolean jag_index_module_content_need_to_update(JagIndexModuleContent *index_module, int jag_module_content_version) {
	JagIndexModuleContentPrivate *priv = JAG_INDEX_MODULE_CONTENT_WO_GET_PRIVATE(index_module);
	gboolean result = FALSE;
	cat_lock_lock(priv->lock);
	if (jag_module_content_version>priv->jag_module_content_version) {
		priv->jag_module_content_version = jag_module_content_version;
		result = TRUE;
	}
	cat_lock_unlock(priv->lock);
	return result;
}






CatStringWo *jag_index_module_content_key() {
	return CAT_S(jag_s_index_module_content_key);
}


/********************* start MooIContent implementation *********************/


static CatStringWo *l_content_get_key(MooIContent *self) {
	return jag_index_module_content_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return self;
}



static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/
