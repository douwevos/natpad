/*
   File:    vipfsmap.c
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

#include "vipfsmap.h"
#include "vipfsfile.h"
#include "../vipimap.h"
#include "../vipiresource.h"
#include <gio/gio.h>
#include <glib/gstdio.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipFSMap"
#include <logging/catlog.h>

struct _VipFSMapPrivate {
	VipPath *path;
	VipFS *fs;
};

static void l_resource_iface_init(VipIResourceInterface *iface);
static void l_map_iface_init(VipIMapInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipFSMap, vip_fs_map, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IRESOURCE, l_resource_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IMAP, l_map_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_fs_map_class_init(VipFSMapClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipFSMapPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_fs_map_init(VipFSMap *instance) {
	VipFSMapPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_FS_MAP, VipFSMapPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipFSMap *instance = VIP_FS_MAP(object);
	VipFSMapPrivate *priv = instance->priv;
	cat_unref_ptr(priv->path);
	cat_unref_ptr(priv->fs);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipFSMap *vip_fs_map_new(VipPath *path) {
	VipFSMap *result = g_object_new(VIP_TYPE_FS_MAP, NULL);
	cat_ref_anounce(result);
	VipFSMapPrivate *priv = result->priv;
	priv->path = cat_ref_ptr(path);
	priv->fs = vip_fs_new(path);
//	cat_ref_ptr(priv->fs);
	return result;
}

VipFSMap *vip_fs_map_new_from_fs(VipFS *fs) {
	VipFSMap *result = g_object_new(VIP_TYPE_FS_MAP, NULL);
	cat_ref_anounce(result);
	VipFSMapPrivate *priv = result->priv;
	priv->path = cat_ref_ptr(vip_fs_get_path(fs));
	priv->fs = cat_ref_ptr(fs);
//	cat_ref_ptr(priv->fs);
	return result;
}

VipPath *vip_fs_map_get_path(VipFSMap *fs_map) {
	return VIP_FS_MAP_GET_PRIVATE(fs_map)->path;
}


void vip_fs_map_clear_cache(VipFSMap *fs_map) {
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(fs_map);
	vip_fs_clear_dir_content_cache(priv->fs);
}


/********************* start VipIResource implementation *********************/

static CatStringWo *l_resource_get_name(VipIResource *self) {
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	CatStringWo *result = vip_path_get_leaf(priv->path);
	if (result == NULL) {
		result = vip_path_get_drive(priv->path);
	}
	return result;

}

static long long l_resource_last_modified(VipIResource *self) {
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	return vip_fs_get_last_modified(priv->fs);
}

static VipIResource *l_resource_rename(VipIResource *self, CatStringWo *new_name) {
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	CatStringWo *old_fname = vip_path_to_string(priv->path);
	GFile *resource_file = g_file_new_for_path(cat_string_wo_getchars(old_fname));

	cat_log_error("renaming:%o into", old_fname);


	GError *error = NULL;
	GFile *result_file = g_file_set_display_name(resource_file, cat_string_wo_getchars(new_name), NULL, &error);
	cat_unref_ptr(result_file);
	cat_unref_ptr(old_fname);
	if (error==NULL) {
		VipPath *par_path = vip_path_create_parent_path(priv->path);
		return (VipIResource *) vip_fs_map_new(vip_path_create_child(par_path, new_name));
	}
	return NULL;
}

static gboolean l_resource_can_rename(VipIResource *self) {
	return TRUE;
}


static CatArrayWo *l_map_enlist(VipIMap *self);

static gboolean l_resource_delete(VipIResource *self) {
	gboolean result = TRUE;

	CatArrayWo *e_content = l_map_enlist((VipIMap *) self);
	if ((e_content!=NULL) && (cat_array_wo_size(e_content)!=0)) {
		CatIIterator *iter = cat_array_wo_iterator(e_content);
		while(result && cat_iiterator_has_next(iter)) {
			VipIResource *child = (VipIResource *) cat_iiterator_next(iter);
			result = vip_iresource_delete(child);
		}
		cat_unref_ptr(iter);
	}
	cat_unref_ptr(e_content);

	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	if (result) {
		CatStringWo *old_fname = vip_path_to_string(priv->path);
		result = g_rmdir(cat_string_wo_getchars(old_fname))==0;
	}
	return result;
}


static gboolean l_resource_can_delete(VipIResource *self) {
	return TRUE;
}

static VipPath *l_resource_path(VipIResource *self) {
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	return priv->path;
}

static gboolean l_resource_equal(VipIResource *res_a, VipIResource *res_b) {
	if (!VIP_IS_FS_MAP(res_b)) {
		return FALSE;
	}
	VipFSMapPrivate *priv_a = VIP_FS_MAP_GET_PRIVATE(res_a);
	VipFSMapPrivate *priv_b = VIP_FS_MAP_GET_PRIVATE(res_b);
	return vip_path_equal(priv_a->path, priv_b->path)
			&& vip_fs_equal(priv_a->fs, priv_b->fs);
}


static void l_resource_iface_init(VipIResourceInterface *iface) {
	iface->getName = l_resource_get_name;
	iface->lastModified = l_resource_last_modified;
	iface->rename = l_resource_rename;
	iface->canRename = l_resource_can_rename;
	iface->delete = l_resource_delete;
	iface->canDelete = l_resource_can_delete;
	iface->path = l_resource_path;
	iface->equal = l_resource_equal;
}

/********************* end VipIResource implementation *********************/

/********************* start VipIMap implementation *********************/


static CatArrayWo *l_map_enlist(VipIMap *self) {
	cat_log_debug("enlisting:%o", self);
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	CatArrayWo *result = cat_array_wo_new();
	// TODO next line could cause a performance penalty ... decide whether we always need to refresh or if it should be configurable (through a parameter ?)
	vip_fs_refresh(priv->fs);
	CatArrayWo *dir_fses = vip_fs_get_dir_content(priv->fs);
	if (dir_fses!=NULL) {
		CatIIterator *iter = cat_array_wo_iterator(dir_fses);
		while(cat_iiterator_has_next(iter)) {
			VipFS *child_fs = (VipFS *) cat_iiterator_next(iter);
			VipFSType tof = vip_fs_get_type_of_file(child_fs);
			switch(tof) {
				case VIP_FS_TYPE_DIRECTORY : {
					VipFSMap *child_map = vip_fs_map_new_from_fs(child_fs);
					cat_array_wo_append(result, (GObject *) child_map);
					cat_unref_ptr(child_map);
				} break;
				case VIP_FS_TYPE_PLAIN : {
					VipFSFile *child_file = vip_fs_file_new_from_fs(child_fs);
					cat_array_wo_append(result, (GObject *) child_file);
					cat_unref_ptr(child_file);
				} break;
				case VIP_FS_TYPE_NON_EXISTENT : {
				} break;
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(dir_fses);
	}
	return result;
}

VipIFile *l_map_create_file(VipIMap *self, CatStringWo *a_new_file_name) {
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	VipFS *child_fs = vip_fs_create_file(priv->fs, a_new_file_name);
	VipIFile *result = NULL;
	if (child_fs) {
		result = (VipIFile *) vip_fs_file_new_from_fs(child_fs);
	}
	return result;

}



static void l_map_iface_init(VipIMapInterface *iface) {
	iface->enlist = l_map_enlist;
	iface->createFile = l_map_create_file;
}

/********************* end VipIMap implementation *********************/



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipFSMapPrivate *priv = VIP_FS_MAP_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p, path=%o]", iname, self, priv->path);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

