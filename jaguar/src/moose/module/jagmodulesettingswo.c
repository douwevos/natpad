/*
   File:    jagmodulesettingswo.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 6, 2013
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

#include "jagmodulesettingswo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagModuleSettingsWo"
#include <logging/catlog.h>

struct _WoInfo {
	JagModuleSettingsWo *original;
};

typedef struct _WoInfo WoInfo;

struct _JagModuleSettingsWoPrivate {
	int version;
	CatArrayWo *source_folders;
	CatArrayWo *lib_paths;
	CatStringWo *jre_name;

	WoInfo *wo_info;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagModuleSettingsWo, jag_module_settings_wo, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagModuleSettingsWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_module_settings_wo_class_init(JagModuleSettingsWoClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_module_settings_wo_init(JagModuleSettingsWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagModuleSettingsWo *instance = JAG_MODULE_SETTINGS_WO(object);
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(instance);
	cat_unref_ptr(priv->source_folders);
	cat_unref_ptr(priv->lib_paths);
	cat_unref_ptr(priv->jre_name);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
	}
	G_OBJECT_CLASS(jag_module_settings_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_module_settings_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagModuleSettingsWo *jag_module_settings_wo_new() {
	JagModuleSettingsWo *result = g_object_new(JAG_TYPE_MODULE_SETTINGS_WO, NULL);
	cat_ref_anounce(result);
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(result);
	priv->version = 1;
	priv->source_folders = NULL;
	priv->lib_paths = NULL;
	priv->jre_name = NULL;
	priv->wo_info = g_new0(WoInfo, 1);
	priv->wo_info->original = NULL;
	return result;
}


JagModuleSettingsWo *jag_module_settings_wo_ensure_writable(JagModuleSettingsWo *module_settings) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	if (priv->wo_info) {
		return cat_ref_ptr(module_settings);
	}
	JagModuleSettingsWo *result = g_object_new(JAG_TYPE_MODULE_SETTINGS_WO, NULL);
	cat_ref_anounce(result);
	JagModuleSettingsWoPrivate *rpriv = jag_module_settings_wo_get_instance_private(result);
	rpriv->source_folders = cat_ref_ptr(priv->source_folders);
	rpriv->lib_paths = cat_ref_ptr(priv->lib_paths);
	rpriv->jre_name = cat_ref_ptr(priv->jre_name);
	rpriv->wo_info = g_new0(WoInfo, 1);
	rpriv->wo_info->original = cat_ref_ptr(module_settings);
	return result;
}


JagModuleSettingsWo *jag_module_settings_wo_anchor(JagModuleSettingsWo *module_settings, int version) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	if (priv->wo_info==NULL) {
		return module_settings;
	}

	cat_log_debug("anchoring:%p", module_settings);
//
//	if (priv->src_folder_paths!=priv->wo_info->orig_src_paths) {
//		int count = cat_array_wo_size(priv->src_folder_paths);
//		for(count--; count>=0; count--) {
//			JagSrcFolderPathWo *src_folder_path = (JagSrcFolderPathWo *) cat_array_wo_get(priv->src_folder_paths, count);
//			cat_log_debug("src_folder_path[%d]=%o", count, src_folder_path);
//			if (!jag_src_folder_path_wo_is_fixed(src_folder_path)) {
//				cat_ref_ptr(src_folder_path);
//				JagSrcFolderPathWo *replacement = jag_src_folder_path_wo_anchor(src_folder_path, version);
//				cat_array_wo_set(priv->src_folder_paths, (GObject *) replacement, count, NULL);
//			}
//		}
//		if (cat_array_wo_equal(priv->src_folder_paths, priv->wo_info->orig_src_paths, NULL)) {
//			cat_ref_swap(priv->src_folder_paths, priv->wo_info->orig_src_paths);
//		}
//	}

	if (priv->wo_info->original) {
		JagModuleSettingsWoPrivate *opriv = jag_module_settings_wo_get_instance_private(priv->wo_info->original);
		gboolean was_modified = priv->source_folders!=opriv->source_folders
				|| priv->lib_paths!=opriv->lib_paths
				|| !cat_string_wo_equal(priv->jre_name, opriv->jre_name);

		cat_log_debug("was_modified=%d", was_modified);

		if (!was_modified) {
			JagModuleSettingsWo *result = priv->wo_info->original;
			priv->wo_info->original = NULL;
			cat_unref_ptr(module_settings);
			return result;
		}
	}
	priv->version = version;
	cat_unref_ptr(priv->wo_info->original);
	cat_free_ptr(priv->wo_info);
	return module_settings;
}

gboolean jag_module_settings_wo_is_fixed(JagModuleSettingsWo *module_settings) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	return priv->wo_info==NULL;
}

JagModuleSettingsWo *jag_module_settings_wo_get_original(JagModuleSettingsWo *e_module_settings) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(e_module_settings);
	if (priv->wo_info) {
		return priv->wo_info->original;
	}
	return NULL;
}


CatArrayWo /* <CatArrayWo<CatStringWo>> */ *jag_module_settings_wo_get_source_folders(JagModuleSettingsWo *module_settings) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	return priv->source_folders;
}

CatStringWo *jag_module_settings_wo_create_flat_path(CatArrayWo *entries) {
	CatStringWo *result = cat_string_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(entries);
	while(cat_iiterator_has_next(iter)) {
		if (cat_string_wo_length(result)>0) {
			cat_string_wo_append_unichar(result, '/');
		}
		CatStringWo *entry_txt = cat_iiterator_next(iter);
		cat_string_wo_append(result, entry_txt);
	}
	cat_unref_ptr(iter);
	return result;
}

CatStringWo *jag_module_settings_wo_get_jre_name(JagModuleSettingsWo *module_settings) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	return priv->jre_name;
}

#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:"); \
			return rval; \
		} \


void jag_module_settings_wo_set_jre_name(JagModuleSettingsWo *module_settings, CatStringWo *new_jre_name) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	CHECK_IF_WRITABLE()
	cat_ref_sink_swap(priv->jre_name, new_jre_name);
}

void jag_module_settings_wo_set_source_folders(JagModuleSettingsWo *module_settings, CatArrayWo *new_source_folders) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	CHECK_IF_WRITABLE()
	cat_ref_sink_swap(priv->source_folders, new_source_folders);
}

CatArrayWo *jag_module_settings_wo_get_libraries(JagModuleSettingsWo *module_settings) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	return priv->lib_paths;

}

void jag_module_settings_wo_set_libraries(JagModuleSettingsWo *module_settings, CatArrayWo *new_source_folders) {
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(module_settings);
	CHECK_IF_WRITABLE()
	cat_ref_sink_swap(priv->lib_paths, new_source_folders);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagModuleSettingsWo *instance = JAG_MODULE_SETTINGS_WO(self);
	JagModuleSettingsWoPrivate *priv = jag_module_settings_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %s, version=%d, jre_name=%o", iname, self, priv->wo_info ? "editable" : "anchored", priv->version, priv->jre_name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
