/*
   File:    vipnodescanner.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
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


#ifndef VIPNODESCANNER_H_
#define VIPNODESCANNER_H_

#include "vipmapperregistry.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_NODE_SCANNER              (vip_node_scanner_get_type())
#define VIP_NODE_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_node_scanner_get_type(), VipNodeScanner))
#define VIP_NODE_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_NODE_SCANNER, VipNodeScannerClass))
#define VIP_IS_NODE_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_NODE_SCANNER))
#define VIP_IS_NODE_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_NODE_SCANNER))
#define VIP_NODE_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_NODE_SCANNER, VipNodeScannerClass))
#define VIP_NODE_SCANNER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_NODE_SCANNER, VipNodeScannerPrivate))


typedef struct _VipNodeScanner               VipNodeScanner;
typedef struct _VipNodeScannerPrivate        VipNodeScannerPrivate;
typedef struct _VipNodeScannerClass          VipNodeScannerClass;


struct _VipNodeScanner {
	GObject parent;
	VipNodeScannerPrivate *priv;
};

struct _VipNodeScannerClass {
	GObjectClass parent_class;
};


GType vip_node_scanner_get_type();

VipNodeScanner *vip_node_scanner_new(VipMapperRegistry *mapper_registry);

void vip_node_scanner_scan(VipNodeScanner *scanner, CatWritableTreeNode *node, gboolean recursive_from_parent);


G_END_DECLS



#endif /* VIPNODESCANNER_H_ */
