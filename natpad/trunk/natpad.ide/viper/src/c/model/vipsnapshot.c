/*
   File:    vipsnapshot.c
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 13, 2013
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

#include "vipsnapshot.h"
#include "vipnode.h"
#include "access/vipnodeidmatcher.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipSnapshot"
#include <logging/catlog.h>

struct _VipSnapshotPrivate {
	CatAtomicLong *version_generator;
	CatReadableTreeNode *root;
	long long version;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipSnapshot, vip_snapshot, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_snapshot_class_init(VipSnapshotClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipSnapshotPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_snapshot_init(VipSnapshot *instance) {
	VipSnapshotPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_SNAPSHOT, VipSnapshotPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipSnapshot *instance = VIP_SNAPSHOT(object);
	VipSnapshotPrivate *priv = instance->priv;
	cat_unref_ptr(priv->root);
	cat_unref_ptr(priv->version_generator);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipSnapshot *vip_snapshot_new(CatReadableTreeNode *root) {
	VipSnapshot *result = g_object_new(VIP_TYPE_SNAPSHOT, NULL);
	cat_ref_anounce(result);
	VipSnapshotPrivate *priv = result->priv;
	priv->root = cat_ref_ptr(root);
	priv->version_generator = cat_atomic_long_new();
	priv->version = cat_atomic_long_increment(priv->version_generator);
	return result;
}



VipSnapshot *l_snapshot_new(CatReadableTreeNode *root, CatAtomicLong *versionGenerator) {
	VipSnapshot *result = g_object_new(VIP_TYPE_SNAPSHOT, NULL);
	cat_ref_anounce(result);
	VipSnapshotPrivate *priv = result->priv;
	priv->root = cat_ref_ptr(root);
	priv->version_generator = cat_ref_ptr(versionGenerator);
	priv->version = cat_atomic_long_increment(priv->version_generator);
	return result;
}


VipSnapshot *vip_snapshot_set_root(VipSnapshot *snapshot, CatReadableTreeNode *newRoot) {
	VipSnapshotPrivate *priv = VIP_SNAPSHOT_GET_PRIVATE(snapshot);
	if (newRoot==priv->root) {
		return cat_ref_ptr(snapshot);
	}
	return l_snapshot_new(newRoot, priv->version_generator);
}


VipSnapshot *vip_snapshot_copy(VipSnapshot *snapshot, CatArrayWo /*VipNode*/ *from, VipNode *to) {

	return NULL;
}



CatReadableTreeNode *vip_snapshot_get_root(VipSnapshot *snapshot) {
	VipSnapshotPrivate *priv = VIP_SNAPSHOT_GET_PRIVATE(snapshot);
	return priv->root;
}


long long vip_snapshot_get_version(VipSnapshot *snapshot) {
	VipSnapshotPrivate *priv = VIP_SNAPSHOT_GET_PRIVATE(snapshot);
	return priv->version;
}


CatTreeNode *vip_snapshot_refresh(VipSnapshot *snapshot, CatTreeNode *node) {
	if (node==NULL) {
		return NULL;
	}
	VipNode *vip_node = (VipNode *) cat_tree_node_get_content(node);
	if (vip_node==NULL) {
		return cat_ref_ptr(node);
	}
	CatReadableTreeNode *orig_root_node = vip_snapshot_get_root(snapshot);
	VipNodeIdMatcher *id_matcher = vip_node_id_matcher_new(vip_node_get_id(vip_node));
	CatITreeEntryListProvider *list_provider = cat_tree_node_get_list_provider((CatTreeNode *) orig_root_node);
	CatTreeNode *result = cat_tree_node_refresh(node, list_provider, (CatIMatcher *) id_matcher);
	cat_unref_ptr(id_matcher);
	return result;
}



void vip_snapshot_print(VipSnapshot *snapshot, CatStringWo *e_buf) {
	VipSnapshotPrivate *priv = VIP_SNAPSHOT_GET_PRIVATE(snapshot);
	cat_string_wo_append_chars(e_buf, "snapshot:");
	cat_string_wo_append_hexadecimal(e_buf, (unsigned long long int) snapshot, -1);
	cat_string_wo_append_chars(e_buf, "\n");
	VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->root);
	CatStringWo *indent = cat_string_wo_new();
	vip_node_print(vip_node, indent, e_buf);
	cat_unref_ptr(indent);
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipSnapshotPrivate *priv = VIP_SNAPSHOT_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:version=%ld, root-node=%p]", iname, self, priv->version, priv->root);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


