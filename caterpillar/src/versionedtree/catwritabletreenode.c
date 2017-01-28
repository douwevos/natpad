/*
   File:    catwritabletreenode.c
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

#include "catwritabletreenode.h"
#include "cattreenodeprivate.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatWritableTreeNode"
#include "../logging/catlog.h"


G_DEFINE_TYPE(CatWritableTreeNode, cat_writable_tree_node, CAT_TYPE_TREE_NODE)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatTreeNode *l_create_new(CatITreeEntryListProvider *list_provider, int location);
static CatTreeEntry *l_get_entry(CatTreeNode *tree_node);

static void cat_writable_tree_node_class_init(CatWritableTreeNodeClass *clazz) {

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	CatTreeNodeClass *node_class = CAT_TREE_NODE_CLASS(clazz);
	node_class->createNew = l_create_new;
	node_class->getEntry = l_get_entry;
}

static void cat_writable_tree_node_init(CatWritableTreeNode *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cat_writable_tree_node_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_writable_tree_node_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatWritableTreeNode *cat_writable_tree_node_new(CatITreeEntryListProvider *list_provider, int location) {
	CatWritableTreeNode *result = g_object_new(CAT_TYPE_WRITABLE_TREE_NODE, NULL);
	cat_ref_anounce(result);
	cat_tree_node_construct((CatTreeNode *) result, list_provider, location);
	return result;
}



static CatTreeNode *l_create_new(CatITreeEntryListProvider *list_provider, int location) {
	return (CatTreeNode *) cat_writable_tree_node_new(list_provider, location);
}

static CatTreeEntry *l_get_entry(CatTreeNode *tree_node) {
	CatTreeNodePrivate *priv = CAT_TREE_NODE_GET_PRIVATE(tree_node);
	CatTreeEntryList *entry_list = cat_itree_entry_list_provider_get_entry_list(priv->list_provider);
	return cat_tree_entry_list_get_entry(entry_list, priv->location);
}


static CatTreeEntry *l_get_writable_entry(CatTreeNodePrivate *priv) {
	CatTreeEntryList *entry_list = cat_itree_entry_list_provider_get_writable_entry_list(priv->list_provider);
	CatTreeEntry *new_entry = cat_tree_entry_list_get_writable_entry(entry_list, priv->location);
	return new_entry;
}


void cat_writable_tree_node_set_child_at(CatWritableTreeNode *node, int index, CatWritableTreeNode *child) {
	CatTreeNodePrivate *priv = CAT_TREE_NODE_GET_PRIVATE(node);
	CatTreeEntry *node_entry = l_get_entry((CatTreeNode *) node);
	int child_location = cat_tree_entry_get_child(node_entry, index);
	if (child_location!= -1) {
		node_entry = l_get_writable_entry(priv);
		cat_tree_entry_set_child_at(node_entry, index, CAT_TREE_NODE_GET_PRIVATE(child)->location);
	} else {
		// TODO error
	}
}

void cat_writable_tree_node_remove_child_at(CatWritableTreeNode *node, int index) {
	CatTreeNodePrivate *priv = CAT_TREE_NODE_GET_PRIVATE(node);
	CatTreeEntry *node_entry = l_get_entry((CatTreeNode *) node);
	int child_location = cat_tree_entry_get_child(node_entry, index);
	if (child_location!= -1) {
		node_entry = l_get_writable_entry(priv);
		int location = cat_tree_entry_removed_child_at(node_entry, index);
		if (location!=-1) {
			CatTreeEntryList *entry_list = cat_itree_entry_list_provider_get_writable_entry_list(priv->list_provider);
			cat_tree_entry_list_remove_entry(entry_list, location, TRUE);
		}
	} else {
			// TODO error
	}
}

void cat_writable_tree_node_remove_child(CatWritableTreeNode *node, CatWritableTreeNode *child_node) {
	CatTreeEntry *node_entry = l_get_entry((CatTreeNode *) node);
	int child_index = cat_tree_entry_find_child_index(node_entry, CAT_TREE_NODE_GET_PRIVATE(child_node)->location);
	if (child_index!=-1) {
		cat_writable_tree_node_remove_child_at(node, child_index);
	}
}



CatWritableTreeNode *cat_writable_tree_node_append_child(CatWritableTreeNode *node) {
	CatTreeNodePrivate *priv = CAT_TREE_NODE_GET_PRIVATE(node);
	CatTreeEntry *node_entry = l_get_writable_entry(priv);
	CatTreeEntryList *entry_list = cat_itree_entry_list_provider_get_writable_entry_list(priv->list_provider);
	int created_location = cat_tree_entry_list_create_node(entry_list);
	CatTreeEntry *created_entry = cat_tree_entry_list_get_entry(entry_list, created_location);
	cat_log_trace("created_location=%d, created_entry=%p", created_location, created_entry);
	cat_tree_entry_set_parent_location(created_entry, priv->location);
	cat_tree_entry_add_child(node_entry, created_location);
	return cat_writable_tree_node_new(priv->list_provider, created_location);
}


void cat_writable_tree_node_set_content(CatWritableTreeNode *node, GObject *new_content) {
	CatTreeNodePrivate *priv = CAT_TREE_NODE_GET_PRIVATE(node);
	CatTreeEntry *node_entry = l_get_writable_entry(priv);
	cat_tree_entry_set_payload(node_entry, new_content);
}

