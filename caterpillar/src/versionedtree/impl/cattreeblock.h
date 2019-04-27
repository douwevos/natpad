/*
   File:    cattreeblock.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 2, 2014
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

#ifndef CATTREEBLOCK_H_
#define CATTREEBLOCK_H_

#include "cattreeentry.h"
#include "../../catimatcher.h"

G_BEGIN_DECLS

#define CAT_TREE_BLOCK_SHIFT	12
#define CAT_TREE_BLOCK_SIZE     (1<<CAT_TREE_BLOCK_SHIFT)

#define CAT_TYPE_TREE_BLOCK              (cat_tree_block_get_type())
#define CAT_TREE_BLOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_tree_block_get_type(), CatTreeBlock))
#define CAT_TREE_BLOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_TREE_BLOCK, CatTreeBlockClass))
#define CAT_IS_TREE_BLOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_TREE_BLOCK))
#define CAT_IS_TREE_BLOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_TREE_BLOCK))
#define CAT_TREE_BLOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_TREE_BLOCK, CatTreeBlockClass))


typedef struct _CatTreeBlock               CatTreeBlock;
typedef struct _CatTreeBlockPrivate        CatTreeBlockPrivate;
typedef struct _CatTreeBlockClass          CatTreeBlockClass;


struct _CatTreeBlock {
	GObject parent;
};

struct _CatTreeBlockClass {
	GObjectClass parent_class;
};


GType cat_tree_block_get_type();

CatTreeBlock *cat_tree_block_new();


void cat_tree_block_mark(CatTreeBlock *block);
gboolean cat_tree_block_is_marked_fixed(CatTreeBlock *block);
CatTreeBlock *cat_tree_block_ensure_writable(CatTreeBlock *block);

gboolean cat_tree_block_has_available_spot(CatTreeBlock *block);
gboolean cat_tree_block_is_empty(CatTreeBlock *block);

int cat_tree_block_count(CatTreeBlock *block);
int cat_tree_block_find(CatTreeBlock *block, CatIMatcher *matcher);

int cat_tree_block_create_node(CatTreeBlock *block);


void cat_tree_block_set(CatTreeBlock *block, int index, CatTreeEntry *node);
CatTreeEntry *cat_tree_block_get(CatTreeBlock *block, int index);
CatTreeEntry *cat_tree_block_get_writable(CatTreeBlock *block, int entry_index);

G_END_DECLS

#endif /* CATTREEBLOCK_H_ */
