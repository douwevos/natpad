/*
   File:    vippath.h
   Project: viper
   Author:  Douwe Vos
   Date:    Jun 6, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef VIPPATH_H_
#define VIPPATH_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_PATH              (vip_path_get_type())
#define VIP_PATH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_path_get_type(), VipPath))
#define VIP_PATH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_PATH, VipPathClass))
#define VIP_IS_PATH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_PATH))
#define VIP_IS_PATH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_PATH))
#define VIP_PATH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_PATH, VipPathClass))
#define VIP_PATH_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_PATH, VipPathPrivate))

typedef struct _VipPath               VipPath;
typedef struct _VipPathPrivate        VipPathPrivate;
typedef struct _VipPathClass          VipPathClass;

typedef enum _VipPathDriveType VipPathDriveType;


enum _VipPathDriveType {
	VIP_PATH_DRIVE_CD,				/*  example:   foo/bar  (linux)  foo\bar (windows)      */
	VIP_PATH_DRIVE_ROOT				/*  example:   /foo/bar (linux)  C:\foo\bar (windows)   */
#ifdef G_OS_WIN32
	,VIP_PATH_DRIVE_ROOT_CD,		/*  example:   C:foo\bar              */
	VIP_PATH_DRIVE_UNC				/*  example:   \\netwokfs\foo\bar     */
#endif
};


struct _VipICdProvider;

struct _VipPath {
	GObject parent;
	VipPathPrivate *priv;
};

struct _VipPathClass {
	GObjectClass parent_class;
};


GType vip_path_get_type();


VipPath *vip_path_new(const CatStringWo *c_full_path);
VipPath *vip_path_new_for_uri(CatStringWo *full_uri);

gboolean vip_path_is_canonical(VipPath *path);

//gboolean vip_path_get_level(VipPath *path);


VipPath *vip_path_create_drive_path(VipPath *path);

CatStringWo *vip_path_get_drive(VipPath *path);

CatStringWo *vip_path_get_drive_descriptor(VipPath *path);


CatStringWo *vip_path_get_leaf(VipPath *path);

int vip_path_count(VipPath *path);
CatStringWo *vip_path_get_at(VipPath *path, int index);
CatIIterator *vip_path_iterator(VipPath *path);

VipPath *vip_path_create_parent_path(VipPath *path);
VipPath *vip_path_create_child(VipPath *path, const CatStringWo *new_leaf);

VipPath *vip_path_create_canonical(VipPath *path, struct _VipICdProvider *cd_provider);

int vip_path_hash(VipPath *path);

gboolean vip_path_equal(VipPath *path_a, VipPath *path_b);

CatStringWo *vip_path_to_string(VipPath *path);

CatStringWo *vip_path_as_uri(VipPath *path);


gboolean vip_path_is_a_child(VipPath *path, VipPath *potential_child);

VipPathDriveType vip_path_get_directory_type(VipPath *path);

G_END_DECLS


#endif /* VIPPATH_H_ */
