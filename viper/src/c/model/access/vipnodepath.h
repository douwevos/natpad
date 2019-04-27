/*
   File:    vipnodepath.h
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

#ifndef VIPNODEPATH_H_
#define VIPNODEPATH_H_

#include "../vipnode.h"
#include "../../vippath.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_NODE_PATH              (vip_node_path_get_type())
#define VIP_NODE_PATH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_node_path_get_type(), VipNodePath))
#define VIP_NODE_PATH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_NODE_PATH, VipNodePathClass))
#define VIP_IS_NODE_PATH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_NODE_PATH))
#define VIP_IS_NODE_PATH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_NODE_PATH))
#define VIP_NODE_PATH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_NODE_PATH, VipNodePathClass))

typedef struct _VipNodePath               VipNodePath;
typedef struct _VipNodePathPrivate        VipNodePathPrivate;
typedef struct _VipNodePathClass          VipNodePathClass;

struct _VipNodePath {
	GObject parent;
};

struct _VipNodePathClass {
	GObjectClass parent_class;
};


GType vip_node_path_get_type();

VipNodePath *vip_node_path_new(CatArrayWo /*<CatTreeNode*>*/ *tree_nodes);

VipNodePath *vip_node_path_create_parent_path(VipNodePath *node_path);

VipNodePath *vip_node_path_from_tree_node(CatTreeNode *tree_node);

VipNode *vip_node_path_get_tail(VipNodePath *node_path);

CatTreeNode *vip_node_path_get_tail_node(VipNodePath *node_path);

VipPath *vip_node_path_create_path(VipNodePath *node_path);

//int vip_node_path_first_virtual_index(VipNodePath *node_path);

G_END_DECLS

#endif /* VIPNODEPATH_H_ */
