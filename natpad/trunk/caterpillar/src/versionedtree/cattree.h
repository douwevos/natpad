/*
   File:    cattree.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 1, 2014
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

#ifndef CATTREE_H_
#define CATTREE_H_

#include "catreadabletreenode.h"
#include "catwritabletreenode.h"

G_BEGIN_DECLS

#define CAT_TYPE_TREE              (cat_tree_get_type())
#define CAT_TREE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_tree_get_type(), CatTree))
#define CAT_TREE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_TREE, CatTreeClass))
#define CAT_IS_TREE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_TREE))
#define CAT_IS_TREE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_TREE))
#define CAT_TREE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_TREE, CatTreeClass))
#define CAT_TREE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_TREE, CatTreePrivate))


typedef struct _CatTree               CatTree;
typedef struct _CatTreePrivate        CatTreePrivate;
typedef struct _CatTreeClass          CatTreeClass;


struct _CatTree {
	GObject parent;
	CatTreePrivate *priv;
};

struct _CatTreeClass {
	GObjectClass parent_class;
};


GType cat_tree_get_type();

CatTree *cat_tree_new();

CatWritableTreeNode *cat_tree_get_writable_root_node(CatTree *tree);

CatReadableTreeNode *cat_tree_get_readable_root_node_ref(CatTree *tree);

CatReadableTreeNode *cat_tree_submit(CatTree *tree, gboolean pack);

void cat_tree_print(CatTree *tree);


G_END_DECLS

#endif /* CATTREE_H_ */
