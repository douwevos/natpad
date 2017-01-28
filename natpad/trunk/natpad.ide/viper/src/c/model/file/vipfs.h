/*
   File:    vipfs.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 16, 2013
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


#ifndef VIPFS_H_
#define VIPFS_H_

#include <caterpillar.h>
#include "../../vippath.h"

G_BEGIN_DECLS

#define VIP_TYPE_FS              (vip_fs_get_type())
#define VIP_FS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_fs_get_type(), VipFS))
#define VIP_FS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_FS, VipFSClass))
#define VIP_IS_FS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_FS))
#define VIP_IS_FS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_FS))
#define VIP_FS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_FS, VipFSClass))
#define VIP_FS_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_FS, VipFSPrivate))


typedef struct _VipFS               VipFS;
typedef struct _VipFSPrivate        VipFSPrivate;
typedef struct _VipFSClass          VipFSClass;

typedef enum _VipFSType          VipFSType;

enum _VipFSType {
	VIP_FS_TYPE_NON_EXISTENT,
	VIP_FS_TYPE_PLAIN,
	VIP_FS_TYPE_DIRECTORY
};

struct _VipFS {
	GObject parent;
	VipFSPrivate *priv;
};

struct _VipFSClass {
	GObjectClass parent_class;
};


GType vip_fs_get_type();

VipFS *vip_fs_new(VipPath *fs_path);

gboolean vip_fs_refresh(VipFS *fs);

VipFSType vip_fs_get_type_of_file(VipFS *fs);


VipPath *vip_fs_get_path(VipFS *fs);


long long vip_fs_get_last_modified(VipFS *fs);

long long vip_fs_get_length(VipFS *fs);


VipFS *vip_fs_create_file(VipFS *fs, CatStringWo *new_file_name);

CatArrayWo *vip_fs_get_dir_content(VipFS *fs);	// <VipFS *>

void vip_fs_clear_dir_content_cache(VipFS *fs);

CatFileInputStream *vip_fs_open_input_stream(CatStringWo *fs_name);

CatFileOutputStream *vip_fs_open_output_stream(CatStringWo *fs_name);

gboolean vip_fs_equal(VipFS *fs_a, VipFS *fs_b);

G_END_DECLS

#endif /* VIPFS_H_ */
