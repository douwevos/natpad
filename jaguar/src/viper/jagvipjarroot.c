/*
   File:    jagvipjarroot.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Feb 23, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "jagvipjarroot.h"
#include "jagjarnameandtype.h"
#include "../jar/jagjarreader.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagVipJarRoot"
#include <logging/catlog.h>

struct _JagVipJarRootPrivate {
	VipIFile *jar_file;
};

static void l_resource_iface_init(VipIResourceInterface *iface);
static void l_map_iface_init(VipIMapInterface *iface);
static void l_file_iface_init(VipIFileInterface *iface);


G_DEFINE_TYPE_WITH_CODE(JagVipJarRoot, jag_vip_jar_root, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagVipJarRoot)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IRESOURCE, l_resource_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IMAP, l_map_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IFILE, l_file_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_vip_jar_root_class_init(JagVipJarRootClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_vip_jar_root_init(JagVipJarRoot *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagVipJarRoot *instance = JAG_VIP_JAR_ROOT(object);
	JagVipJarRootPrivate *priv = jag_vip_jar_root_get_instance_private(instance);
	cat_unref_ptr(priv->jar_file);
	G_OBJECT_CLASS(jag_vip_jar_root_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_vip_jar_root_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagVipJarRoot *jag_vip_jar_root_new(VipIFile *jar_file) {
	JagVipJarRoot *result = g_object_new(JAG_TYPE_VIP_JAR_ROOT, NULL);
	cat_ref_anounce(result);
	JagVipJarRootPrivate *priv = jag_vip_jar_root_get_instance_private(result);
	priv->jar_file = cat_ref_ptr(jar_file);
	return result;
}


/********************* begin VipIResource implementation *********************/

static CatStringWo *l_resource_get_name(VipIResource *self) {
	JagVipJarRoot *instance = JAG_VIP_JAR_ROOT(self);
	JagVipJarRootPrivate *priv = jag_vip_jar_root_get_instance_private(instance);
	return vip_iresource_get_name((VipIResource *) priv->jar_file);
}

static long long l_resource_last_modified(VipIResource *self) {
	JagVipJarRoot *instance = JAG_VIP_JAR_ROOT(self);
	JagVipJarRootPrivate *priv = jag_vip_jar_root_get_instance_private(instance);
	return VIP_IRESOURCE_GET_INTERFACE(priv->jar_file)->lastModified((VipIResource *) priv->jar_file);
}

static void l_resource_iface_init(VipIResourceInterface *iface) {
	iface->getName = l_resource_get_name;
	iface->lastModified = l_resource_last_modified;
}

/********************* end VipIResource implementation *********************/

/********************* begin VipIFile implementation *********************/

static long long l_file_length(VipIFile *self) {
	JagVipJarRoot *instance = JAG_VIP_JAR_ROOT(self);
	JagVipJarRootPrivate *priv = jag_vip_jar_root_get_instance_private(instance);
	return VIP_IFILE_GET_INTERFACE(priv->jar_file)->length(priv->jar_file);
}

static CatIInputStream *l_file_open_input_stream(VipIFile *self) {
	JagVipJarRoot *instance = JAG_VIP_JAR_ROOT(self);
	JagVipJarRootPrivate *priv = jag_vip_jar_root_get_instance_private(instance);
	return VIP_IFILE_GET_INTERFACE(priv->jar_file)->openInputStream(priv->jar_file);
}

static void l_file_iface_init(VipIFileInterface *iface) {
	iface->length = l_file_length;
	iface->openInputStream = l_file_open_input_stream;
}

/********************* end VipIFile implementation *********************/

/********************* begin VipIMap implementation *********************/

CatS l_s_jag_root = CAT_S_DEF("<root>");

static CatArrayWo *l_map_enlist(VipIMap *self) {
	cat_log_debug("enlisting:%o", self);
	JagVipJarRoot *instance = JAG_VIP_JAR_ROOT(self);
	JagVipJarRootPrivate *priv = jag_vip_jar_root_get_instance_private(instance);
	CatArrayWo *e_result = cat_array_wo_new();

	JagJarNameAndType *root = jag_jar_name_and_type_new(CAT_S(l_s_jag_root), TRUE);
	CatIInputStream *stream = vip_ifile_open_input_stream(priv->jar_file);
	if (stream) {
		JagJarReader *jarInputStream = jag_jar_reader_new_from_stream(stream);
		if (jag_jar_reader_init_directory(jarInputStream)) {

			CatIIterator *iter = jag_jar_reader_entry_iterator(jarInputStream);

			int entryIndex = 0;
			while(cat_iiterator_has_next(iter)) {
				JagJarCentralDirHeader *header = (JagJarCentralDirHeader *) cat_iiterator_next(iter);
				CatStringWo *a_entry_name = header->a_fileName;
				gboolean is_map = cat_string_wo_endswith_char(a_entry_name, '/');
				JagJarNameAndType *nameAndType = jag_jar_name_and_type_add(root, a_entry_name, is_map);
				jag_jar_name_and_type_set_index(nameAndType, entryIndex);
				entryIndex++;
			}
			cat_unref_ptr(iter);

			int child_count = jag_jar_name_and_type_child_count(root);
			if (child_count>0) {
				int idx = 0;
				CatArrayWo *e_thisVipPath = cat_array_wo_new();
				cat_array_wo_append(e_thisVipPath, (GObject *) CAT_S(cat_s_empty_string));

				CatHashSet *files_added = cat_hash_set_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
				CatHashSet *maps_added = cat_hash_set_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
				CatIIterator *child_iter = jag_jar_name_and_type_child_iterator(root);
				while(cat_iiterator_has_next(child_iter)) {
					JagJarNameAndType *nameAndType = (JagJarNameAndType *) cat_iiterator_next(child_iter);
					CatStringWo *a_child_name = jag_jar_name_and_type_get_name(nameAndType);
					if (jag_jar_name_and_type_is_map(nameAndType)) {
						if (cat_hash_set_add(files_added, (GObject *) a_child_name) == NULL) {
							VipIResource *vip_resource = jag_jar_name_and_type_get_vip_resource(nameAndType, priv->jar_file, e_thisVipPath);
							cat_array_wo_append(e_result, (GObject *) vip_resource);
						}
					} else {
						if (cat_hash_set_add(maps_added, (GObject *) a_child_name)==NULL) {
							VipIResource *vip_resource = jag_jar_name_and_type_get_vip_resource(nameAndType, priv->jar_file, e_thisVipPath);
							cat_array_wo_append(e_result, (GObject *) vip_resource);
						}
					}
					idx++;
				}
				cat_unref_ptr(files_added);
				cat_unref_ptr(maps_added);
				cat_unref_ptr(child_iter);
				cat_unref_ptr(e_thisVipPath);
			}

		} else {
			cat_log_error("jar file seems corrupt:%o", priv->jar_file);
		}
		cat_unref_ptr(jarInputStream);
	} else {
		cat_log_error("can not open stream for file:%o", priv->jar_file);
	}
	cat_unref_ptr(stream);
	cat_unref_ptr(root);
	return e_result;
}

static void l_map_iface_init(VipIMapInterface *iface) {
	iface->enlist = l_map_enlist;
}

/********************* end VipIMap implementation *********************/
