/*
   File:    vipfsfile.c
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

#include "vipfsfile.h"
#include "vipfs.h"
#include "../vipifile.h"
#include "../vipiresource.h"
#include <gio/gio.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipFSFile"
#include <logging/catlog.h>

struct _VipFSFilePrivate {
	VipPath *path;
	VipFS *fs;
};

static void l_resource_iface_init(VipIResourceInterface *iface);
static void l_file_iface_init(VipIFileInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipFSFile, vip_fs_file, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(VipFSFile)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IRESOURCE, l_resource_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IFILE, l_file_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_fs_file_class_init(VipFSFileClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_fs_file_init(VipFSFile *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipFSFile *instance = VIP_FS_FILE(object);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	cat_unref_ptr(priv->path);
	cat_unref_ptr(priv->fs);
	G_OBJECT_CLASS(vip_fs_file_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_fs_file_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipFSFile *vip_fs_file_new(VipPath *path) {
	VipFSFile *result = g_object_new(VIP_TYPE_FS_FILE, NULL);
	cat_ref_anounce(result);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(result);
	priv->path = cat_ref_ptr(path);
	priv->fs = vip_fs_new(path);
	return result;
}

VipFSFile *vip_fs_file_new_from_fs(VipFS *fs) {
	VipFSFile *result = g_object_new(VIP_TYPE_FS_FILE, NULL);
	cat_ref_anounce(result);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(result);
	priv->path = cat_ref_ptr(vip_fs_get_path(fs));
	priv->fs = cat_ref_ptr(fs);
	return result;
}

VipPath *vip_fs_file_get_path(VipFSFile *fs_file) {
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(fs_file);
	return priv->path;
}


/********************* start VipIResource implementation *********************/

static CatStringWo *l_resource_get_name(VipIResource *self) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	return vip_path_get_leaf(priv->path);
}

static long long l_resource_last_modified(VipIResource *self) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	return vip_fs_get_last_modified(priv->fs);
}

static VipIResource *l_resource_rename(VipIResource *self, CatStringWo *a_new_name) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	CatStringWo *old_fname = vip_path_to_string(priv->path);
	GFile *resource_file = g_file_new_for_path(cat_string_wo_getchars(old_fname));

	cat_log_error("renaming:%o into", old_fname);

	GError *error = NULL;
	GFile *result_file = g_file_set_display_name(resource_file, cat_string_wo_getchars(a_new_name), NULL, &error);
	cat_unref_ptr(result_file);
	cat_unref_ptr(old_fname);
	if (error==NULL) {
		VipPath *par_path = vip_path_create_parent_path(priv->path);
		return (VipIResource *) vip_fs_file_new(vip_path_create_child(par_path, a_new_name));
	}
	return NULL;
}

static gboolean l_resource_can_rename(VipIResource *self) {
	return TRUE;
}

static gboolean l_resource_delete(VipIResource *self) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	CatStringWo *old_fname = vip_path_to_string(priv->path);
	GFile *resource_file = g_file_new_for_path(cat_string_wo_getchars(old_fname));
	GError *error = NULL;

	if (g_file_delete(resource_file, NULL, &error)) {
		return TRUE;
	}
	return FALSE;
}


static gboolean l_resource_can_delete(VipIResource *self) {
	return TRUE;
}

static VipPath *l_resource_path(VipIResource *self) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	return priv->path;
}

static gboolean l_resource_equal(VipIResource *res_a, VipIResource *res_b) {
	if (!VIP_IS_FS_FILE(res_b)) {
		return FALSE;
	}
	VipFSFilePrivate *priv_a = vip_fs_file_get_instance_private(VIP_FS_FILE(res_a));
	VipFSFilePrivate *priv_b = vip_fs_file_get_instance_private(VIP_FS_FILE(res_b));
	return vip_path_equal(priv_a->path, priv_b->path)
			&& vip_fs_equal(priv_a->fs, priv_b->fs);
}


static void l_resource_iface_init(VipIResourceInterface *iface) {
	iface->getName = l_resource_get_name;
	iface->lastModified = l_resource_last_modified;
	iface->canRename = l_resource_can_rename;
	iface->rename = l_resource_rename;
	iface->canDelete = l_resource_can_delete;
	iface->delete = l_resource_delete;
	iface->path = l_resource_path;
	iface->equal = l_resource_equal;
}

/********************* end VipIResource implementation *********************/

/********************* start VipIFile implementation *********************/

static long long l_file_length(VipIFile *self) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	return vip_fs_get_length(priv->fs);
}

static CatIInputStream *l_file_open_input_stream(VipIFile *self) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	CatStringWo *path_flat = vip_path_to_string(priv->path);
	CatFileInputStream *result = vip_fs_open_input_stream(path_flat);
	cat_unref_ptr(path_flat);
	return (CatIInputStream *) result;
}


static CatIOutputStream *l_file_open_output_stream(VipIFile *self) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	CatStringWo *path_flat = vip_path_to_string(priv->path);
	CatFileOutputStream *result = vip_fs_open_output_stream(path_flat);
	cat_unref_ptr(path_flat);
	return (CatIOutputStream *) result;
}


static void l_file_iface_init(VipIFileInterface *iface) {
	iface->length = l_file_length;
	iface->openInputStream = l_file_open_input_stream;
	iface->openOutputStream = l_file_open_output_stream;
}

/********************* end VipIFile implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipFSFile *instance = VIP_FS_FILE(self);
	VipFSFilePrivate *priv = vip_fs_file_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:path=%o, fs=%o]", iname, self, vip_path_get_leaf(priv->path), priv->fs);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
