/*
   File:    jagjarnameandtype.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 3, 2013
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

#include "jagjarnameandtype.h"
#include "jagvipjarfile.h"
#include "jagvipjarmap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJarNameAndType"
#include <logging/catlog.h>

struct _JagJarNameAndTypePrivate {
	CatStringWo *a_name;
	gboolean is_map;
	CatHashSet *children;	// <JarNameAndType>

	VipIResource *resource;
	int entry_index;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagJarNameAndType, jag_jar_name_and_type, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_jar_name_and_type_class_init(JagJarNameAndTypeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_jar_name_and_type_init(JagJarNameAndType *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagJarNameAndType *instance = JAG_JAR_NAME_AND_TYPE(object);
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(instance);
	cat_unref_ptr(priv->children);
	cat_unref_ptr(priv->a_name);
	cat_unref_ptr(priv->resource);
	G_OBJECT_CLASS(jag_jar_name_and_type_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_jar_name_and_type_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagJarNameAndType *jag_jar_name_and_type_new(CatStringWo *a_name, gboolean is_map) {
	JagJarNameAndType *result = g_object_new(JAG_TYPE_JAR_NAME_AND_TYPE, NULL);
	cat_ref_anounce(result);
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(result);
	priv->a_name = cat_ref_ptr(a_name);
	priv->is_map = is_map;
	priv->children = NULL;
	if (is_map) {
		priv->children = cat_hash_set_new((GHashFunc) jag_jar_name_and_type_hash, (GEqualFunc) jag_jar_name_and_type_equal);
	}
	priv->resource = NULL;
	priv->entry_index = -1;
	return result;
}

int jag_jar_name_and_type_hash(JagJarNameAndType *name_and_type) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	return cat_string_wo_hash(priv->a_name) + (priv->is_map ? 13 : 0);
}

gboolean jag_jar_name_and_type_equal(JagJarNameAndType *name_and_type_a, JagJarNameAndType *name_and_type_b) {
	if (name_and_type_a==name_and_type_b) {
		return TRUE;
	}
	JagJarNameAndTypePrivate *priva = jag_jar_name_and_type_get_instance_private(name_and_type_a);
	JagJarNameAndTypePrivate *privb = jag_jar_name_and_type_get_instance_private(name_and_type_b);
	return priva->is_map==privb->is_map && cat_string_wo_equal(priva->a_name, privb->a_name);
}

JagJarNameAndType *l_add(JagJarNameAndType *name_and_type, CatArrayWo *e_path_entries, int index) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	cat_unref_ptr(priv->resource);
	JagJarNameAndType *result = NULL;
	while(result==NULL) {
		if (index>=cat_array_wo_size(e_path_entries)) {
			return name_and_type;
		}
		CatStringWo *a_entry = (CatStringWo *) cat_array_wo_get(e_path_entries, index++);
		if (cat_string_wo_length(a_entry)==0) {
			continue;
		}
		gboolean isMap = index!=cat_array_wo_size(e_path_entries);
		JagJarNameAndType *nameAndType = jag_jar_name_and_type_new(a_entry, isMap);

		JagJarNameAndType *added = (JagJarNameAndType *) cat_hash_set_add(priv->children, (GObject *) nameAndType);

		if (added==NULL) {
			added = nameAndType;
		}
		cat_unref(nameAndType);

		result = l_add(added, e_path_entries, index);
	}
	return result;
}

JagJarNameAndType *jag_jar_name_and_type_add(JagJarNameAndType *name_and_type, CatStringWo *a_path, gboolean isMap) {
	cat_log_debug("name_and_type=%p, path=%p, isMap=%d", name_and_type, a_path, isMap);
	CatArrayWo *e_split = cat_string_wo_split(a_path, '/', TRUE);
	if (e_split == NULL || cat_array_wo_size(e_split)==0) {
		cat_unref_ptr(e_split);
		return name_and_type;
	}
	if (isMap) {
		cat_array_wo_append(e_split, (GObject *) CAT_S(cat_s_empty_string));
	} else {
		while(cat_string_wo_length((CatStringWo *) cat_array_wo_get_last(e_split))==0) {
			if (cat_array_wo_size(e_split)==1) {
				cat_unref_ptr(e_split);
				return name_and_type;
			}
			cat_array_wo_remove_last(e_split, NULL);
		}
	}
	JagJarNameAndType *result = l_add(name_and_type, e_split, 0);
	cat_unref_ptr(e_split);
	return result;
}

VipIResource *jag_jar_name_and_type_get_vip_resource(JagJarNameAndType *name_and_type, VipIFile *mainFile, CatArrayWo *a_parent_path) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	if (priv->resource==NULL) {
		CatArrayWo *e_vipPath = cat_array_wo_clone(a_parent_path, CAT_CLONE_DEPTH_MAIN);
		cat_array_wo_append(e_vipPath, (GObject *) priv->a_name);
		CatArrayWo *a_vipPath = cat_array_wo_anchor(e_vipPath, 0);
		if (priv->is_map) {
			CatArrayWo *a_new_vip_children = NULL;
			if (priv->children) {
				CatArrayWo *e_vipchildren = cat_array_wo_new();
				CatIIterator *iter = cat_hash_set_iterator(priv->children);
				while(cat_iiterator_has_next(iter)) {
					JagJarNameAndType *childNameAndType = (JagJarNameAndType *) cat_iiterator_next(iter);
					VipIResource *child_resource = jag_jar_name_and_type_get_vip_resource(childNameAndType, mainFile, a_vipPath);
					cat_array_wo_append(e_vipchildren, (GObject *) child_resource);
				}
				cat_unref_ptr(iter);
				a_new_vip_children = cat_array_wo_anchor(e_vipchildren, 0);
			}
			priv->resource = (VipIResource *) jag_vip_jar_map_new(mainFile, priv->a_name, a_new_vip_children);
			cat_unref_ptr(a_new_vip_children);
		} else {
			priv->resource = (VipIResource *) jag_vip_jar_file_new(mainFile, priv->a_name, priv->entry_index);
		}
		cat_unref_ptr(a_vipPath);
	}
	return priv->resource;
}

void jag_jar_name_and_type_set_index(JagJarNameAndType *name_and_type, int entry_index) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	priv->entry_index = entry_index;
}

int jag_jar_name_and_type_get_index(JagJarNameAndType *name_and_type) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	return priv->entry_index;
}

int jag_jar_name_and_type_child_count(JagJarNameAndType *name_and_type) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	return priv->children==NULL ? -1 : cat_hash_set_size(priv->children);
}

CatIIterator *jag_jar_name_and_type_child_iterator(JagJarNameAndType *name_and_type) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	return priv->children==NULL ? NULL : cat_hash_set_iterator(priv->children);
}

gboolean jag_jar_name_and_type_is_map(JagJarNameAndType *name_and_type) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	return priv->is_map;
}

CatStringWo *jag_jar_name_and_type_get_name(JagJarNameAndType *name_and_type) {
	JagJarNameAndTypePrivate *priv = jag_jar_name_and_type_get_instance_private(name_and_type);
	return priv->a_name;
}
