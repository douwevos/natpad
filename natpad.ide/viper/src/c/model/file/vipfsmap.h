/*
   File:    vipfsmap.h
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


#ifndef VIPFSMAP_H_
#define VIPFSMAP_H_

#include <caterpillar.h>
#include "vipfs.h"
#include "../../vippath.h"

G_BEGIN_DECLS

#define VIP_TYPE_FS_MAP              (vip_fs_map_get_type())
#define VIP_FS_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_fs_map_get_type(), VipFSMap))
#define VIP_FS_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_FS_MAP, VipFSMapClass))
#define VIP_IS_FS_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_FS_MAP))
#define VIP_IS_FS_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_FS_MAP))
#define VIP_FS_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_FS_MAP, VipFSMapClass))
#define VIP_FS_MAP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_FS_MAP, VipFSMapPrivate))


typedef struct _VipFSMap               VipFSMap;
typedef struct _VipFSMapPrivate        VipFSMapPrivate;
typedef struct _VipFSMapClass          VipFSMapClass;


struct _VipFSMap {
	GObject parent;
	VipFSMapPrivate *priv;
};

struct _VipFSMapClass {
	GObjectClass parent_class;
};


GType vip_fs_map_get_type();

VipFSMap *vip_fs_map_new(VipPath *path);

VipFSMap *vip_fs_map_new_from_fs(VipFS *fs);


VipPath *vip_fs_map_get_path(VipFSMap *fs_map);

void vip_fs_map_clear_cache(VipFSMap *fs_map);


G_END_DECLS

#endif /* VIPFSMAP_H_ */
