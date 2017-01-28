/*
   File:    catwritabletreenode.h
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

#ifndef CATWRITABLETREENODE_H_
#define CATWRITABLETREENODE_H_

#include "cattreenode.h"

G_BEGIN_DECLS

#define CAT_TYPE_WRITABLE_TREE_NODE              (cat_writable_tree_node_get_type())
#define CAT_WRITABLE_TREE_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_writable_tree_node_get_type(), CatWritableTreeNode))
#define CAT_WRITABLE_TREE_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_WRITABLE_TREE_NODE, CatWritableTreeNodeClass))
#define CAT_IS_WRITABLE_TREE_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_WRITABLE_TREE_NODE))
#define CAT_IS_WRITABLE_TREE_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_WRITABLE_TREE_NODE))
#define CAT_WRITABLE_TREE_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_WRITABLE_TREE_NODE, CatWritableTreeNodeClass))


typedef struct _CatWritableTreeNode               CatWritableTreeNode;
typedef struct _CatWritableTreeNodeClass          CatWritableTreeNodeClass;


struct _CatWritableTreeNode {
	CatTreeNode parent;
};

struct _CatWritableTreeNodeClass {
	CatTreeNodeClass parent_class;
};


GType cat_writable_tree_node_get_type();

CatWritableTreeNode *cat_writable_tree_node_new(CatITreeEntryListProvider *list_provider, int location);


void cat_writable_tree_node_set_child_at(CatWritableTreeNode *node, int index, CatWritableTreeNode *child);

void cat_writable_tree_node_remove_child_at(CatWritableTreeNode *node, int index);

void cat_writable_tree_node_remove_child(CatWritableTreeNode *node, CatWritableTreeNode *child_node);

CatWritableTreeNode *cat_writable_tree_node_append_child(CatWritableTreeNode *node);

void cat_writable_tree_node_set_content(CatWritableTreeNode *node, GObject *new_content);

G_END_DECLS

#endif /* CATWRITABLETREENODE_H_ */
