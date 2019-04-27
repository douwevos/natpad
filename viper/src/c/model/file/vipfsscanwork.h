/*
   File:    vipfsscanwork.h
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

#ifndef VIPFSSCANWORK_H_
#define VIPFSSCANWORK_H_

#include "../../vipisequence.h"
#include "../vipnode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_FS_SCAN_WORK              (vip_fs_scan_work_get_type())
#define VIP_FS_SCAN_WORK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_fs_scan_work_get_type(), VipFSScanWork))
#define VIP_FS_SCAN_WORK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_FS_SCAN_WORK, VipFSScanWorkClass))
#define VIP_IS_FS_SCAN_WORK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_FS_SCAN_WORK))
#define VIP_IS_FS_SCAN_WORK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_FS_SCAN_WORK))
#define VIP_FS_SCAN_WORK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_FS_SCAN_WORK, VipFSScanWorkClass))

typedef struct _VipFSScanWork               VipFSScanWork;
typedef struct _VipFSScanWorkPrivate        VipFSScanWorkPrivate;
typedef struct _VipFSScanWorkClass          VipFSScanWorkClass;

struct _VipFSScanWork {
	GObject parent;
};

struct _VipFSScanWorkClass {
	GObjectClass parent_class;
};


GType vip_fs_scan_work_get_type();

VipFSScanWork *vip_fs_scan_work_new(VipISequence *sequence, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent);

G_END_DECLS

#endif /* VIPFSSCANWORK_H_ */
