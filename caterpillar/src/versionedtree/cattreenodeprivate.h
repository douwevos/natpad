/*
   File:    cattreenodeprivate.h
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

#ifndef CATTREENODEPRIVATE_H_
#define CATTREENODEPRIVATE_H_

#include "cattreenode.h"

G_BEGIN_DECLS


struct _CatTreeNodePrivate {
	CatITreeEntryListProvider *list_provider;
	int location;
};

CatITreeEntryListProvider *cat_tree_node_get_list_provider(CatTreeNode *tree_node);

CatTreeNodePrivate *cat_tree_node_get_private(CatTreeNode *tree_node);

G_END_DECLS

#endif /* CATTREENODEPRIVATE_H_ */



