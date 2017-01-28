/*
   File:    chammap.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Apr 17, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chammap.h"
#include <sys/types.h>
#include <gio/gio.h>

#ifdef G_OS_UNIX
#include <sys/mman.h>
#else
#include <windows.h>
#endif

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaMMap"
#include <logging/catlog.h>

struct _ChaMMapPrivate {
	GFile *file;
	goffset file_length;
#ifdef G_OS_UNIX
	int filedes;
#else
	HANDLE filedes;
#endif
	void *data;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaMMap, cha_mmap, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaMMap)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_mmap_class_init(ChaMMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_mmap_init(ChaMMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaMMap *instance = CHA_MMAP(object);
//	ChaMMapPrivate *priv = cha_mmap_get_instance_private(instance);
	G_OBJECT_CLASS(cha_mmap_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_mmap_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaMMap *cha_mmap_new(GFile *file) {
	GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	cat_log_debug("info=%o", info);
	if (info==NULL) {
		// TODO handle error
		return NULL;
	}
	goffset file_length = g_file_info_get_attribute_uint64(info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
	cat_unref_ptr(info);


	ChaMMap *result = g_object_new(CHA_TYPE_MMAP, NULL);
	cat_ref_anounce(result);
	ChaMMapPrivate *priv = cha_mmap_get_instance_private(result);
	priv->file = cat_ref_ptr(file);

	char *path = g_file_get_path(file);
#ifdef G_OS_UNIX
	priv->filedes = open(path, O_RDONLY);
	priv->data = mmap(NULL, file_length, PROT_READ, MAP_SHARED, priv->filedes, 0);
#else
	priv->filedes = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE fm = CreateFileMapping(priv->filedes, NULL,PAGE_READONLY,0,0,NULL);
	priv->data = MapViewOfFile(fm, FILE_MAP_READ, 0, 0, file_length);
#endif
	priv->file_length = file_length;

//	madvise(priv->data + 1024*1024, priv->file_length-1024*1024, MADV_DONTNEED);


	return result;
}


void cha_mmap_call_dontneed(ChaMMap *map) {
	ChaMMapPrivate *priv = cha_mmap_get_instance_private(map);
#ifdef G_OS_UNIX
	if (priv->file_length > 1024*1024) {
		VARIABLE_IS_NOT_USED int s = madvise(priv->data , priv->file_length, MADV_DONTNEED);
		cat_log_debug("calling madvise %d", s);
	}
#endif
}

void *cha_mmap_get_data(ChaMMap *map) {
	ChaMMapPrivate *priv = cha_mmap_get_instance_private(map);
	return priv->data;
}


uint64_t cha_mmap_get_length(ChaMMap *map) {
	ChaMMapPrivate *priv = cha_mmap_get_instance_private(map);
	return priv->file_length;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaMMap *instance = CHA_MMAP(self);
	ChaMMapPrivate *priv = cha_mmap_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p data=%p]", iname, self, priv->data);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
