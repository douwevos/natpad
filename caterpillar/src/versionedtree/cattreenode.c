/*
   File:    cattreenode.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 5, 2014
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

#include "cattreenodeprivate.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatTreeNode"
#include "../logging/catlog.h"

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatTreeNode, cat_tree_node, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(CatTreeNode)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_tree_node_class_init(CatTreeNodeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_tree_node_init(CatTreeNode *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatTreeNode *instance = CAT_TREE_NODE(object);
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(instance);
	cat_unref_ptr(priv->list_provider);
	G_OBJECT_CLASS(cat_tree_node_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_tree_node_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void cat_tree_node_construct(CatTreeNode *node, CatITreeEntryListProvider *list_provider, int location) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	priv->list_provider = cat_ref_ptr(list_provider);
	priv->location = location;
}

CatTreeNodePrivate *cat_tree_node_get_private(CatTreeNode *node) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	return priv;
}


CatTreeNode* cat_tree_node_refresh(CatTreeNode *node, CatITreeEntryListProvider *new_list_provider, CatIMatcher *matcher) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	if (new_list_provider==priv->list_provider) {
		return cat_ref_ptr(node);
	}
	CatTreeEntryList *new_entry_list = cat_itree_entry_list_provider_get_entry_list(new_list_provider);
	CatTreeEntryList *old_entry_list = cat_itree_entry_list_provider_get_entry_list(priv->list_provider);
	if (old_entry_list==new_entry_list) {
		return cat_ref_ptr(node);
	}

	int new_location = cat_tree_entry_list_find_location(new_entry_list, matcher, priv->location);
	cat_log_debug("node=%o, new_location=%d", node, new_location);
	return CAT_TREE_NODE_GET_CLASS(node)->createNew(new_list_provider, new_location);
}

CatTreeNode *cat_tree_node_get_parent_node(CatTreeNode *node) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	CatTreeNode *result = NULL;
	CatTreeEntry *node_entry = CAT_TREE_NODE_GET_CLASS(node)->getEntry(node);
	int parent_location = cat_tree_entry_get_parent_location(node_entry);
	if (parent_location!= -1) {
		cat_log_debug("node=%o, parent_location=%d", node, parent_location);
		result = CAT_TREE_NODE_GET_CLASS(node)->createNew(priv->list_provider, parent_location);
	}
	return result;
}


CatTreeNode * cat_tree_node_child_at(CatTreeNode *node, int index) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	CatTreeNode *result = NULL;
	CatTreeEntry *node_entry = CAT_TREE_NODE_GET_CLASS(node)->getEntry(node);
	int child_location = cat_tree_entry_get_child(node_entry, index);
	if (child_location!= -1) {
		cat_log_debug("node=%o, child_location=%d", node, child_location);
		result =  CAT_TREE_NODE_GET_CLASS(node)->createNew(priv->list_provider, child_location);
	}
	return result;
}

int cat_tree_node_child_count(CatTreeNode *node) {
	CatTreeEntry *node_entry = CAT_TREE_NODE_GET_CLASS(node)->getEntry(node);
	return cat_tree_entry_count(node_entry);
}


int cat_tree_node_find_child_index(CatTreeNode *node, CatIMatcher *matcher, int guess_index) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	CatTreeEntry *node_entry = CAT_TREE_NODE_GET_CLASS(node)->getEntry(node);
	int entry_count = cat_tree_entry_count(node_entry);
	if (entry_count<=0) {
		return -1;
	}
	int result = -1;
	CatTreeEntryList *entry_list = cat_itree_entry_list_provider_get_entry_list(priv->list_provider);
	if (guess_index>=0 && guess_index<entry_count) {
		int child_location = cat_tree_entry_get_child(node_entry, guess_index);
		CatTreeEntry *child_entry = cat_tree_entry_list_get_entry(entry_list, child_location);
		if (cat_imatcher_matches(matcher, cat_tree_entry_get_payload(child_entry))) {
			return guess_index;
		}
	}
	int idx;
	for(idx=entry_count-1; idx>=0; idx--) {
		int child_location = cat_tree_entry_get_child(node_entry, idx);
		CatTreeEntry *child_entry = cat_tree_entry_list_get_entry(entry_list, child_location);
		cat_log_debug("child_location[%d]=%d, child_entry=%o", idx, child_location, child_entry);
		if (cat_imatcher_matches(matcher, cat_tree_entry_get_payload(child_entry))) {
			result = idx;
			break;
		}
	}
	return result;
}





GObject *cat_tree_node_get_content(CatTreeNode *node) {
	CatTreeEntry *node_entry = CAT_TREE_NODE_GET_CLASS(node)->getEntry(node);
	return cat_tree_entry_get_payload(node_entry);
}


int cat_tree_node_get_location(CatTreeNode *node) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	return priv->location;
}

CatITreeEntryListProvider *cat_tree_node_get_list_provider(CatTreeNode *tree_node) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(tree_node);
	return priv->list_provider;
}



/****** entry-list based methods ******/

int cat_tree_node_find_location(CatTreeNode *node, CatIMatcher *matcher, int guess_location) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	CatTreeEntryList *entry_list = cat_itree_entry_list_provider_get_entry_list(priv->list_provider);
	return cat_tree_entry_list_find_location(entry_list, matcher, guess_location);
}

CatTreeNode *cat_tree_node_get_node_for_location(CatTreeNode *node, int location) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(node);
	CatTreeNode *result = NULL;
	if (location!= -1) {
		result = CAT_TREE_NODE_GET_CLASS(node)->createNew(priv->list_provider, location);
	}
	return result;
}





/********************* begin CatIStringable implementation *********************/

static void l_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatTreeNodePrivate *priv = cat_tree_node_get_instance_private(CAT_TREE_NODE(self));
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	CatTreeEntry *node_entry = CAT_TREE_NODE_GET_CLASS(self)->getEntry((CatTreeNode *) self);
	cat_string_wo_format(append_to, "%s[%p:location=%d, entry=%o]", iname, self, priv->location, node_entry);
}


static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_print;
}

/********************* end CatIStringable implementation *********************/
