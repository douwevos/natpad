/*
   File:    cattreenode.h
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

#ifndef CATTREENODE_H_
#define CATTREENODE_H_

#include "catitreeentrylistprovider.h"

G_BEGIN_DECLS

#define CAT_TYPE_TREE_NODE              (cat_tree_node_get_type())
#define CAT_TREE_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_tree_node_get_type(), CatTreeNode))
#define CAT_TREE_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_TREE_NODE, CatTreeNodeClass))
#define CAT_IS_TREE_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_TREE_NODE))
#define CAT_IS_TREE_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_TREE_NODE))
#define CAT_TREE_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_TREE_NODE, CatTreeNodeClass))
#define CAT_TREE_NODE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_TREE_NODE, CatTreeNodePrivate))


typedef struct _CatTreeNode               CatTreeNode;
typedef struct _CatTreeNodePrivate        CatTreeNodePrivate;
typedef struct _CatTreeNodeClass          CatTreeNodeClass;


struct _CatTreeNode {
	GObject parent;
	CatTreeNodePrivate *priv;
};

struct _CatTreeNodeClass {
	GObjectClass parent_class;
	CatTreeNode *(*createNew)(CatITreeEntryListProvider *list_provider, int location);
	CatTreeEntry *(*getEntry)(CatTreeNode *node);
};


GType cat_tree_node_get_type();

void cat_tree_node_construct(CatTreeNode *node, CatITreeEntryListProvider *list_provider, int location);

CatTreeNode* cat_tree_node_refresh(CatTreeNode *node, CatITreeEntryListProvider *new_list_provider, CatIMatcher *matcher);

CatTreeNode *cat_tree_node_get_parent_node(CatTreeNode *node);
CatTreeNode * cat_tree_node_child_at(CatTreeNode *node, int index);
int cat_tree_node_child_count(CatTreeNode *node);

int cat_tree_node_find_child_index(CatTreeNode *node, CatIMatcher *matcher, int guess_index);

GObject *cat_tree_node_get_content(CatTreeNode *node);
int cat_tree_node_get_location(CatTreeNode *node);
CatITreeEntryListProvider *cat_tree_node_get_list_provider(CatTreeNode *tree_node);

int cat_tree_node_find_location(CatTreeNode *node, CatIMatcher *matcher, int guess_location);
CatTreeNode *cat_tree_node_get_node_for_location(CatTreeNode *node, int location);


G_END_DECLS

#endif /* CATTREENODE_H_ */
