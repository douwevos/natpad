/*
   File:    vipsnapshot.h
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

#ifndef VIPSNAPSHOT_H_
#define VIPSNAPSHOT_H_

#include <caterpillar.h>
#include "vipnode.h"

G_BEGIN_DECLS

#define VIP_TYPE_SNAPSHOT              (vip_snapshot_get_type())
#define VIP_SNAPSHOT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_snapshot_get_type(), VipSnapshot))
#define VIP_SNAPSHOT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_SNAPSHOT, VipSnapshotClass))
#define VIP_IS_SNAPSHOT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_SNAPSHOT))
#define VIP_IS_SNAPSHOT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_SNAPSHOT))
#define VIP_SNAPSHOT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_SNAPSHOT, VipSnapshotClass))
#define VIP_SNAPSHOT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_SNAPSHOT, VipSnapshotPrivate))


typedef struct _VipSnapshot               VipSnapshot;
typedef struct _VipSnapshotPrivate        VipSnapshotPrivate;
typedef struct _VipSnapshotClass          VipSnapshotClass;


struct _VipSnapshot {
	GObject parent;
	VipSnapshotPrivate *priv;
};

struct _VipSnapshotClass {
	GObjectClass parent_class;
};


GType vip_snapshot_get_type();

VipSnapshot *vip_snapshot_new(CatReadableTreeNode *root);

VipSnapshot *vip_snapshot_set_root(VipSnapshot *snapshot, CatReadableTreeNode *newRoot);

CatReadableTreeNode *vip_snapshot_get_root(VipSnapshot *snapshot);

long long vip_snapshot_get_version(VipSnapshot *snapshot);

CatTreeNode *vip_snapshot_refresh(VipSnapshot *snapshot, CatTreeNode *node);

void vip_snapshot_print(VipSnapshot *snapshot, CatStringWo *e_buf);

G_END_DECLS


#endif /* VIPSNAPSHOT_H_ */
