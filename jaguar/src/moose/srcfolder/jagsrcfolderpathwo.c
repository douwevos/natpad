/*
   File:    jagsrcfolderpathwo.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 31, 2014
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

#include "jagsrcfolderpathwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagSrcFolderPathWo"
#include <logging/catlog.h>

struct _WoInfo {
	JagSrcFolderPathWo *original;
};

typedef struct _WoInfo WoInfo;


struct _JagSrcFolderPathWoPrivate {
	MooNamePath *name_path;
	int resource_node_version;
	int jaguar_node_offset;
	int version;
	WoInfo *wo_info;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagSrcFolderPathWo, jag_src_folder_path_wo, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_src_folder_path_wo_class_init(JagSrcFolderPathWoClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JagSrcFolderPathWoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_src_folder_path_wo_init(JagSrcFolderPathWo *instance) {
	JagSrcFolderPathWoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_SRC_FOLDER_PATH_WO, JagSrcFolderPathWoPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagSrcFolderPathWo *instance = JAG_SRC_FOLDER_PATH_WO(object);
	JagSrcFolderPathWoPrivate *priv = instance->priv;
	cat_unref_ptr(priv->name_path);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
	}
	G_OBJECT_CLASS(jag_src_folder_path_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_src_folder_path_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagSrcFolderPathWo *jag_src_folder_path_wo_new(MooNamePath *name_path) {
	JagSrcFolderPathWo *result = g_object_new(JAG_TYPE_SRC_FOLDER_PATH_WO, NULL);
	cat_ref_anounce(result);
	JagSrcFolderPathWoPrivate *priv = result->priv;
	priv->name_path = cat_ref_ptr(name_path);
	priv->resource_node_version = -1;
	priv->jaguar_node_offset = -1;
	priv->version = 0;
	priv->wo_info = g_new0(WoInfo, 1);
	priv->wo_info->original = NULL;
	return result;
}


JagSrcFolderPathWo *jag_src_folder_path_wo_ensure_editable(JagSrcFolderPathWo *path) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(path);
	if (priv->wo_info==NULL) {
		JagSrcFolderPathWo *result = g_object_new(JAG_TYPE_SRC_FOLDER_PATH_WO, NULL);
		cat_ref_anounce(result);
		JagSrcFolderPathWoPrivate *opriv = result->priv;
		opriv->name_path = cat_ref_ptr(priv->name_path);
		opriv->resource_node_version = priv->resource_node_version;
		opriv->jaguar_node_offset = priv->jaguar_node_offset;
		opriv->version = priv->version+1;
		opriv->wo_info = g_new0(WoInfo, 1);
		opriv->wo_info->original = cat_ref_ptr(path);
		return result;
	}
	return cat_ref_ptr(path);
}

JagSrcFolderPathWo *jag_src_folder_path_wo_get_original(JagSrcFolderPathWo *e_path) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(e_path);
	if (priv->wo_info) {
		if (priv->wo_info->original) {
			return priv->wo_info->original;
		}
	}
	return e_path;
}

gboolean jag_src_folder_path_wo_is_fixed(JagSrcFolderPathWo *path) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(path);
	return priv->wo_info == NULL;
}

JagSrcFolderPathWo *jag_src_folder_path_wo_anchor(JagSrcFolderPathWo *path, int version) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(path);
	cat_log_debug("anchoring entry:%o", path);
	if (priv->wo_info == NULL) {
		cat_log_debug("wo_info was NULL");
		return path;
	}
	gboolean was_modified = TRUE;
	if (priv->wo_info->original) {
		JagSrcFolderPathWoPrivate *opriv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(priv->wo_info->original);
		was_modified = (priv->name_path!=opriv->name_path)
				|| (priv->resource_node_version!=opriv->resource_node_version)
				|| (priv->jaguar_node_offset!=opriv->jaguar_node_offset);
	}


	if (was_modified) {
		/* return edited version */
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
		priv->version = version;
		cat_log_debug("Returning edited as anchored:%o", path);
		return path;
	}
	/* return original, release edited version */
	JagSrcFolderPathWo *result = priv->wo_info->original;
	priv->wo_info->original = NULL;
	cat_unref_ptr(path);
	cat_log_debug("Returning original as anchored:%o", result);
	return result;
}

#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", e_path); \
			return rval; \
		} \



MooNamePath *jag_src_folder_path_wo_get_name_path(JagSrcFolderPathWo *path) {
	return JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(path)->name_path;
}


gboolean jag_src_folder_path_wo_update_resource_node_version(JagSrcFolderPathWo *e_path, int new_res_version) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(e_path);
	gboolean result = FALSE;
	if (priv->resource_node_version<new_res_version) {
		CHECK_IF_WRITABLE(FALSE)
		priv->resource_node_version = new_res_version;
		result = TRUE;
	}
	return result;
}

gboolean jag_src_folder_path_wo_test_resource_node_version(JagSrcFolderPathWo *e_path, int new_res_version) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(e_path);
	gboolean result = FALSE;
	if (priv->resource_node_version<new_res_version) {
		result = TRUE;
	}
	return result;
}



int jag_src_folder_path_wo_get_jaguar_node_offset(JagSrcFolderPathWo *path) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(path);
	return priv->jaguar_node_offset;
}

void jag_src_folder_path_wo_set_jaguar_node_offset(JagSrcFolderPathWo *e_path, int new_offset) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(e_path);
	CHECK_IF_WRITABLE()
	priv->jaguar_node_offset = new_offset;
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagSrcFolderPathWoPrivate *priv = JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %s, version=%d, jaguar_node_offset=%d, resource_node_version=%d, name_path=%o]", iname, self, priv->wo_info ? "editable" : "anchored", priv->version, priv->jaguar_node_offset, priv->resource_node_version, priv->name_path);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


