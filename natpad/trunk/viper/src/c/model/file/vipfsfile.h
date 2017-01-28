/*
   File:    vipfsfile.h
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


#ifndef VIPFSFILE_H_
#define VIPFSFILE_H_

#include <caterpillar.h>
#include "vipfs.h"
#include "../../vippath.h"

G_BEGIN_DECLS

#define VIP_TYPE_FS_FILE              (vip_fs_file_get_type())
#define VIP_FS_FILE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_fs_file_get_type(), VipFSFile))
#define VIP_FS_FILE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_FS_FILE, VipFSFileClass))
#define VIP_IS_FS_FILE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_FS_FILE))
#define VIP_IS_FS_FILE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_FS_FILE))
#define VIP_FS_FILE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_FS_FILE, VipFSFileClass))
#define VIP_FS_FILE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_FS_FILE, VipFSFilePrivate))


typedef struct _VipFSFile               VipFSFile;
typedef struct _VipFSFilePrivate        VipFSFilePrivate;
typedef struct _VipFSFileClass          VipFSFileClass;


struct _VipFSFile {
	GObject parent;
	VipFSFilePrivate *priv;
};

struct _VipFSFileClass {
	GObjectClass parent_class;
};


GType vip_fs_file_get_type();

VipFSFile *vip_fs_file_new(VipPath *path);

VipFSFile *vip_fs_file_new_from_fs(VipFS *fs);


VipPath *vip_fs_file_get_path(VipFSFile *fs_file);

G_END_DECLS

#endif /* VIPFSFILE_H_ */
