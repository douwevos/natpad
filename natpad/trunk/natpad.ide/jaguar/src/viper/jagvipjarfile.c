/*
   File:    jagvipjarfile.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 27, 2013
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

#include "jagvipjarfile.h"
#include "../jar/jagjarreader.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagVipJarFile"
#include <logging/catlog.h>

struct _JagVipJarFilePrivate {
	VipIFile *main_file;
	CatStringWo *a_entry_name;
	int entry_index;
};

static void l_resource_iface_init(VipIResourceInterface *iface);
static void l_file_iface_init(VipIFileInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagVipJarFile, jag_vip_jar_file, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IRESOURCE, l_resource_iface_init);
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IFILE, l_file_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_vip_jar_file_class_init(JagVipJarFileClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagVipJarFilePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_vip_jar_file_init(JagVipJarFile *instance) {
	JagVipJarFilePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_VIP_JAR_FILE, JagVipJarFilePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagVipJarFile *instance = JAG_VIP_JAR_FILE(object);
	JagVipJarFilePrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_entry_name);
	cat_unref_ptr(priv->main_file);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagVipJarFile *jag_vip_jar_file_new(VipIFile *main_file, CatStringWo *a_entry_name, int entry_index) {
	JagVipJarFile *result = g_object_new(JAG_TYPE_VIP_JAR_FILE, NULL);
	cat_ref_anounce(result);
	JagVipJarFilePrivate *priv = result->priv;
	priv->main_file = cat_ref_ptr(main_file);
	priv->a_entry_name = cat_ref_ptr(a_entry_name);
	priv->entry_index = entry_index;
	return result;
}



/********************* start VipIResource implementation *********************/

static CatStringWo *l_resource_get_name(VipIResource *self) {
	JagVipJarFilePrivate *priv = JAG_VIP_JAR_FILE_GET_PRIVATE(self);
	return priv->a_entry_name;

}

static long long l_resource_last_modified(VipIResource *self) {
	return 0;
}

VipPath *l_resource_path(VipIResource *self) {
	JagVipJarFilePrivate *priv = JAG_VIP_JAR_FILE_GET_PRIVATE(self);
	VipPath *main_path = vip_iresource_path((VipIResource *) priv->main_file);

	cat_log_error("%o", main_path);

	const CatStringWo *seperator = cat_string_wo_new_data("!");
	VipPath *child = vip_path_create_child(main_path, seperator);

	VipPath *result = vip_path_create_child(child, priv->a_entry_name);
	cat_unref_ptr(child);
	cat_log_error("%o", result);
	return result;
}

static void l_resource_iface_init(VipIResourceInterface *iface) {
	iface->getName = l_resource_get_name;
	iface->lastModified = l_resource_last_modified;
	iface->path = l_resource_path;
}

/********************* end VipIResource implementation *********************/

/********************* start VipIFile implementation *********************/

static long long l_file_length(VipIFile *self) {
	return 0;
}

static CatIInputStream *l_file_open_input_stream(VipIFile *self) {
	JagVipJarFilePrivate *priv = JAG_VIP_JAR_FILE_GET_PRIVATE(self);

	CatIInputStream *stream = vip_ifile_open_input_stream(priv->main_file);
	if (stream) {
		cat_log_debug("priv->entry_name=%o, priv->entry_index=%d", priv->a_entry_name, priv->entry_index);
		JagJarReader *jarInputStream = jag_jar_reader_new_from_stream(stream);
		if (jag_jar_reader_init_directory(jarInputStream)) {

			CatIIterator *iter = jag_jar_reader_entry_iterator(jarInputStream);
			int idx;
			for(idx=priv->entry_index; idx>=0; idx--) {
				if (cat_iiterator_has_next(iter)) {
					cat_iiterator_next(iter);
				} else {
					idx = -10;
					break;
				}
			}
			cat_unref_ptr(iter);
			cat_unref_ptr(stream);
			if (idx!=-10) {
	//			LimitInputStream limitInputStream = new LimitInputStream(jarInputStream, nextJarEntry.getSize());
				return (CatIInputStream *) jarInputStream;
			}
		} else {
			cat_log_error("jarfile seems corrupt:%0", priv->main_file);
		}
		cat_unref_ptr(jarInputStream);
		cat_unref_ptr(stream);
	}
	return NULL;
}


static void l_file_iface_init(VipIFileInterface *iface) {
	iface->length = l_file_length;
	iface->openInputStream = l_file_open_input_stream;
}

/********************* end VipIFile implementation *********************/

void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagVipJarFilePrivate *priv = JAG_VIP_JAR_FILE_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:%o, main_file=%o]", iname, self, priv->a_entry_name, priv->main_file);
}


static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}
