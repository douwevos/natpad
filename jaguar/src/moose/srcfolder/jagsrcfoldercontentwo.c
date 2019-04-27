/*
   File:    jagsrcfoldercontent.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 6, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "jagsrcfoldercontentwo.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagSrcFolderContentWo"
#include <logging/catlog.h>

struct _WoInfo {
	JagSrcFolderContentWo *original;
//	MooNodeWo *moo_src_folder_node;
	gboolean marked;
};

typedef struct _WoInfo WoInfo;

struct _JagSrcFolderContentWoPrivate {
	int version;
	WoInfo *wo_info;
};

static CatS jag_s_src_folder_content_key = CAT_S_DEF("JagSrcFolderContentWo");

static void l_content_iface_init(MooIContentInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagSrcFolderContentWo, jag_src_folder_content_wo, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagSrcFolderContentWo)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_src_folder_content_wo_class_init(JagSrcFolderContentWoClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_src_folder_content_wo_init(JagSrcFolderContentWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagSrcFolderContentWo *instance = JAG_SRC_FOLDER_CONTENT_WO(object);
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(instance);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
	}
	G_OBJECT_CLASS(jag_src_folder_content_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_src_folder_content_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagSrcFolderContentWo *jag_src_folder_content_wo_new() {
	JagSrcFolderContentWo *result = g_object_new(JAG_TYPE_SRC_FOLDER_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(result);
	priv->version = 1;
	priv->wo_info = g_new0(WoInfo, 1);
//	priv->wo_info->moo_src_folder_node = NULL;
	priv->wo_info->original = NULL;
	return result;
}

JagSrcFolderContentWo *jag_src_folder_content_wo_ensure_editable(JagSrcFolderContentWo *content, MooNodeWo *moo_src_folder_node) {
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(content);
	if (priv->wo_info) {
//		cat_ref_swap(priv->wo_info->moo_src_folder_node, moo_src_folder_node);
		return cat_ref_ptr(content);
	}

	JagSrcFolderContentWo *result = g_object_new(JAG_TYPE_SRC_FOLDER_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagSrcFolderContentWoPrivate *dpriv = jag_src_folder_content_wo_get_instance_private(result);
	dpriv->version = priv->version;
	dpriv->wo_info = g_new0(WoInfo, 1);
	dpriv->wo_info->original = cat_ref_ptr(content);
//	dpriv->wo_info->moo_src_folder_node = cat_ref_ptr(moo_src_folder_node);
	return result;
}

gboolean jag_src_folder_content_wo_is_fixed(JagSrcFolderContentWo *content) {
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(content);
	return priv->wo_info==NULL;
}

int jag_src_folder_content_wo_get_version(JagSrcFolderContentWo *content) {
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(content);
	return priv->version;
}

JagSrcFolderContentWo *jag_src_folder_content_wo_anchor(JagSrcFolderContentWo *content, int version) {
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(content);
	if (priv->wo_info) {
		gboolean was_modified = (priv->wo_info->original==NULL) || priv->wo_info->marked;


//			if (priv->wo_info->moo_src_folder_node) {
//				was_modified = FALSE;
//				int count = moo_node_wo_child_count(priv->wo_info->moo_src_folder_node);
//				int idx=0;
//				for(idx=0; idx<count; idx++) {
//					MooNodeWo *child = moo_node_wo_child_at(priv->wo_info->moo_src_folder_node, idx);
//					if (moo_node_wo_get_version(child)>=version) {
//						was_modified = TRUE;
//					}
//				}
//			}
		if (was_modified) {
			cat_unref_ptr(priv->wo_info->original);
//			cat_unref_ptr(priv->wo_info->moo_src_folder_node);
			cat_free_ptr(priv->wo_info);
			priv->version = version;
			return content;
		}

		JagSrcFolderContentWo *result = priv->wo_info->original;
		priv->wo_info->original = NULL;
		cat_unref_ptr(content);
		return result;
	}
	return content;
}

void jag_src_folder_content_wo_mark(JagSrcFolderContentWo *e_content) {
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(e_content);
	priv->wo_info->marked = TRUE;
}

CatStringWo *jag_src_folder_content_wo_key() {
	return CAT_S(jag_s_src_folder_content_key);
}


/********************* start MooIContent implementation *********************/


static CatStringWo *l_content_get_key(MooIContent *self) {
	return jag_src_folder_content_wo_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return (MooIContent *) jag_src_folder_content_wo_anchor(JAG_SRC_FOLDER_CONTENT_WO(self), version);
}



static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagSrcFolderContentWo *instance = JAG_SRC_FOLDER_CONTENT_WO(self);
	JagSrcFolderContentWoPrivate *priv = jag_src_folder_content_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: version=%d]", iname, self, priv->version);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
