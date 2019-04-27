/*
   File:    catreadabletreenode.h
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

#ifndef CATREADABLETREENODE_H_
#define CATREADABLETREENODE_H_

#include "cattreenode.h"

G_BEGIN_DECLS

#define CAT_TYPE_READABLE_TREE_NODE              (cat_readable_tree_node_get_type())
#define CAT_READABLE_TREE_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_readable_tree_node_get_type(), CatReadableTreeNode))
#define CAT_READABLE_TREE_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_READABLE_TREE_NODE, CatReadableTreeNodeClass))
#define CAT_IS_READABLE_TREE_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_READABLE_TREE_NODE))
#define CAT_IS_READABLE_TREE_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_READABLE_TREE_NODE))
#define CAT_READABLE_TREE_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_READABLE_TREE_NODE, CatReadableTreeNodeClass))

typedef struct _CatReadableTreeNode               CatReadableTreeNode;
typedef struct _CatReadableTreeNodeClass          CatReadableTreeNodeClass;
typedef struct _CatReadableTreeNodePrivate        CatReadableTreeNodePrivate;

struct _CatReadableTreeNode {
	CatTreeNode parent;
};

struct _CatReadableTreeNodeClass {
	CatTreeNodeClass parent_class;
};


GType cat_readable_tree_node_get_type();

CatReadableTreeNode *cat_readable_tree_node_new(CatITreeEntryListProvider *list_provider, int location);

CatITreeEntryListProvider *cat_readable_tree_node_get_list_provider(CatReadableTreeNode *tree_node);

G_END_DECLS

#endif /* CATREADABLETREENODE_H_ */
