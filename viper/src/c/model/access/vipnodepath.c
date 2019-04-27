/*
   File:    vipnodepath.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 18, 2013
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

#include "vipnodepath.h"
#include "vipnodeidmatcher.h"
#include "../../vippathprivate.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipNodePath"
#include <logging/catlog.h>

struct _VipNodePathPrivate {
	CatArrayWo /*<CatTreeNode*>*/ *tree_nodes;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipNodePath, vip_node_path, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(VipNodePath)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_node_path_class_init(VipNodePathClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_node_path_init(VipNodePath *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipNodePath *instance = VIP_NODE_PATH(object);
	VipNodePathPrivate *priv = vip_node_path_get_instance_private(instance);
	cat_unref_ptr(priv->tree_nodes);
	G_OBJECT_CLASS(vip_node_path_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_node_path_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipNodePath *vip_node_path_new(CatArrayWo /*<CatTreeNode*>*/ *tree_nodes) {
	VipNodePath *result = g_object_new(VIP_TYPE_NODE_PATH, NULL);
	cat_ref_anounce(result);
	VipNodePathPrivate *priv = vip_node_path_get_instance_private(result);
	priv->tree_nodes = cat_ref_ptr(tree_nodes);
	return result;
}

VipNodePath *vip_node_path_create_parent_path(VipNodePath *node_path) {
	VipNodePathPrivate *priv = vip_node_path_get_instance_private(node_path);
	if ((priv->tree_nodes == NULL) || (cat_array_wo_size(priv->tree_nodes)==0)) {
		return NULL;
	}

	VipNodePath *result = g_object_new(VIP_TYPE_NODE_PATH, NULL);
	cat_ref_anounce(result);
	VipNodePathPrivate *rpriv = vip_node_path_get_instance_private(result);
	CatArrayWo *tree_nodes = cat_array_wo_clone(priv->tree_nodes, CAT_CLONE_DEPTH_MAIN);
	cat_array_wo_remove_last(tree_nodes, NULL);
	rpriv->tree_nodes = tree_nodes;
	return result;
}


VipNodePath *vip_node_path_from_tree_node(CatTreeNode *tree_node) {
	if (tree_node==NULL) {
		return NULL;
	}
	CatArrayWo *list = cat_array_wo_new();
	cat_ref_ptr(tree_node);
	while(tree_node!=NULL) {
		cat_array_wo_append(list, (GObject *) tree_node);
		cat_unref(tree_node);
		tree_node = cat_tree_node_get_parent_node((CatTreeNode *) tree_node);
	}
	cat_array_wo_reverse(list);
	VipNodePath *result = vip_node_path_new(list);
	cat_unref_ptr(list);
	return result;
}

VipNode *vip_node_path_get_tail(VipNodePath *node_path) {
	CatTreeNode *rtn = vip_node_path_get_tail_node(node_path);
	return rtn==NULL ? NULL : (VipNode *) cat_tree_node_get_content((CatTreeNode *) rtn);
}


CatTreeNode *vip_node_path_get_tail_node(VipNodePath *node_path) {
	VipNodePathPrivate *priv = vip_node_path_get_instance_private(node_path);
	return priv->tree_nodes ? (CatTreeNode *) cat_array_wo_get_last(priv->tree_nodes) : NULL;
}

VipPath *vip_node_path_create_path(VipNodePath *node_path) {
	VipNodePathPrivate *priv = vip_node_path_get_instance_private(node_path);
	if (priv->tree_nodes==NULL) {
		return NULL;
	}
	CatArrayWo *entries = cat_array_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(priv->tree_nodes);
	while(cat_iiterator_has_next(iter)) {
		CatTreeNode *tree_node = (CatTreeNode *) cat_iiterator_next(iter);
		VipNode *vip_node = (VipNode*) cat_tree_node_get_content(tree_node);
		CatStringWo *name = vip_node_get_name(vip_node);
		if (name==NULL) {
			continue;
		}
		cat_log_debug("name=%p", name);
		cat_array_wo_append(entries, (GObject *) vip_node_get_name(vip_node));
	}
	cat_unref_ptr(iter);

	VipPath *result = vip_path_new_from_entries(entries);
	cat_unref_ptr(entries);
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipNodePath *instance = VIP_NODE_PATH(self);
	VipNodePathPrivate *priv = vip_node_path_get_instance_private(instance);

	CatStringWo *child_text = cat_string_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(priv->tree_nodes);
	while(cat_iiterator_has_next(iter)) {
		CatTreeNode *child_tn = (CatTreeNode *) cat_iiterator_next(iter);
		VipNode *child = (VipNode *) cat_tree_node_get_content(child_tn);
		cat_string_wo_append_hexadecimal(child_text, (long long) child, -1);
		cat_string_wo_append_chars(child_text, ":");
		CatStringWo *node_name = vip_node_get_name(child);
		cat_string_wo_append_chars(child_text, node_name==NULL ? "null" : cat_string_wo_getchars(node_name));

		if (!cat_iiterator_is_last(iter)) {
			cat_string_wo_append_chars(child_text, ">>");
		}
	}
	cat_unref_ptr(iter);

	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%s]", iname, cat_string_wo_getchars(child_text));
	cat_unref_ptr(child_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
