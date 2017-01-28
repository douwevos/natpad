/*
   File:    catreadabletreenode.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 6, 2014
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

#include "catreadabletreenode.h"
#include "cattreenodeprivate.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatReadableTreeNode"
#include "../logging/catlog.h"

struct _CatReadableTreeNodePrivate {
	CatTreeEntry *node_entry;
};

G_DEFINE_TYPE(CatReadableTreeNode, cat_readable_tree_node, CAT_TYPE_TREE_NODE)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatTreeNode *l_create_new(CatITreeEntryListProvider *list_provider, int location);
static CatTreeEntry *l_get_entry(CatTreeNode *node);


static void cat_readable_tree_node_class_init(CatReadableTreeNodeClass *clazz) {
	g_type_class_add_private(clazz, sizeof(CatReadableTreeNodePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	CatTreeNodeClass *node_class = CAT_TREE_NODE_CLASS(clazz);
	node_class->createNew = l_create_new;
	node_class->getEntry = l_get_entry;
}

static void cat_readable_tree_node_init(CatReadableTreeNode *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatReadableTreeNodePrivate *priv = CAT_READABLE_TREE_NODE_GET_PRIVATE(object);
	cat_unref_ptr(priv->node_entry);
	G_OBJECT_CLASS(cat_readable_tree_node_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_readable_tree_node_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatReadableTreeNode *cat_readable_tree_node_new(CatITreeEntryListProvider *list_provider, int location) {
	CatReadableTreeNode *result = g_object_new(CAT_TYPE_READABLE_TREE_NODE, NULL);
	cat_ref_anounce(result);
	CatReadableTreeNodePrivate *priv = CAT_READABLE_TREE_NODE_GET_PRIVATE(result);
	CatTreeEntryList *entry_list = cat_itree_entry_list_provider_get_entry_list(list_provider);
	priv->node_entry = cat_tree_entry_list_get_entry(entry_list, location);
	cat_ref_ptr(priv->node_entry);
	cat_tree_node_construct((CatTreeNode *) result, list_provider, location);
	return result;
}

static CatTreeEntry *l_get_entry(CatTreeNode *node) {
	CatReadableTreeNodePrivate *priv = CAT_READABLE_TREE_NODE_GET_PRIVATE(node);
	return priv->node_entry;
}



static CatTreeNode *l_create_new(CatITreeEntryListProvider *list_provider, int location) {
	return (CatTreeNode *) cat_readable_tree_node_new(list_provider, location);
}

CatITreeEntryListProvider *cat_readable_tree_node_get_list_provider(CatReadableTreeNode *tree_node) {
	return cat_tree_node_get_list_provider((CatTreeNode *) tree_node);
}

