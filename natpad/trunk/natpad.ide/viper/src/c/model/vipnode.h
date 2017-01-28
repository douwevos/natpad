/*
   File:    vipnode.h
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 13, 2013
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


#ifndef VIPNODE_H_
#define VIPNODE_H_

#include "access/vipinodematcher.h"
#include "vipiresource.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_NODE              (vip_node_get_type())
#define VIP_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_node_get_type(), VipNode))
#define VIP_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_NODE, VipNodeClass))
#define VIP_IS_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_NODE))
#define VIP_IS_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_NODE))
#define VIP_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_NODE, VipNodeClass))
#define VIP_NODE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_NODE, VipNodePrivate))


typedef struct _VipNode               VipNode;
typedef struct _VipNodePrivate        VipNodePrivate;
typedef struct _VipNodeClass          VipNodeClass;


struct _VipNode {
	GObject parent;
	VipNodePrivate *priv;
};

struct _VipNodeClass {
	GObjectClass parent_class;
	VipNode *(*clone)(VipNode *to_clone);
};


GType vip_node_get_type();

VipNode *vip_node_new(long long viper_id, VipIResource *resource);

VipNode *vip_node_new_full(long long viper_id, VipIResource *resource, CatAtomicInteger *hold_counter, CatAtomicInteger *recursive_counter);


void vip_node_construct(VipNode *node, long long viperId, VipIResource *resource, CatAtomicInteger *hold_counter, CatAtomicInteger *recursive_counter);

VipNode *vip_node_clone(VipNode *toclone);

long long vip_node_get_id(VipNode *node);

CatStringWo *vip_node_get_name(VipNode *node);

VipIResource *vip_node_get_content(VipNode *node);
VipNode *vip_node_set_resource(VipNode *node, VipIResource *new_resource);

CatAtomicInteger *vip_node_get_hold_counter(VipNode *node);
void vip_node_hold_down(VipNode *node);
void vip_node_hold_up(VipNode *node);
gboolean vip_node_should_hold(VipNode *node);

CatAtomicInteger *vip_node_get_recursive_counter(VipNode *node);
void vip_node_recursive_down(VipNode *node);
void vip_node_recursive_up(VipNode *node);
gboolean vip_node_should_scan_recursive(VipNode *node);

void vip_node_print(VipNode *node, CatStringWo *indent, CatStringWo *e_buf);

G_END_DECLS

#endif /* VIPNODE_H_ */
