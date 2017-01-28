/*
   File:    jagmodulecontentwo.c
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

#include "jagmodulecontentwo.h"
#include "jagrefreshcompilerrequest.h"
#include "../library/jaglibrarypathmatcher.h"
#include "../../link/jagilinklistener.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagModuleContentWo"
#include <logging/catlog.h>


struct _WoInfo {
	JagModuleContentWo *original;
	CatArrayWo *orig_src_paths;
	CatArrayWo *orig_library_content_list;
	gboolean marked;
};

typedef struct _WoInfo WoInfo;

struct _JagModuleContentWoPrivate {
	CatWeakReference *jag_moose_service_ref;
	MooIdPath *module_id_path;
	ShoModel *model_cached;
	JagModuleSettingsWo *module_settings;
	int resource_content_version;
	int version;
	CatArrayWo *a_source_folders;
	CatArrayWo *e_src_folder_paths;
	CatArrayWo *a_libraries;
	CatArrayWo *e_library_content_list;
	JagJreLink *jre_link;

	JagIndexModuleContent *index_content;
	WoInfo *wo_info;
};

static CatS jag_s_module_content_key = CAT_S_DEF("JagModuleContentWo");

static void l_content_iface_init(MooIContentInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_link_listener_iface_init(JagILinkListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagModuleContentWo, jag_module_content_wo, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagModuleContentWo)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_TYPE_ILINK_LISTENER, l_link_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_module_content_wo_class_init(JagModuleContentWoClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
//	g_type_class_add_private(clazz, sizeof(JagModuleContentWoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->res_name_matcher = moo_resource_name_matcher_new();
	cat_ref_ignore(clazz->res_name_matcher);
}

static void jag_module_content_wo_init(JagModuleContentWo *instance) {
//	JagModuleContentWoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_MODULE_CONTENT_WO, JagModuleContentWoPrivate);
//	instance->priv = priv;
}


static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagModuleContentWo *instance = JAG_MODULE_CONTENT_WO(object);
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(instance);
	cat_unref_ptr(priv->jag_moose_service_ref);
	cat_unref_ptr(priv->index_content);
	cat_unref_ptr(priv->model_cached);
	cat_unref_ptr(priv->module_settings);
	cat_unref_ptr(priv->a_source_folders);
	cat_unref_ptr(priv->e_src_folder_paths);
	cat_unref_ptr(priv->a_libraries);
	cat_unref_ptr(priv->e_library_content_list);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_unref_ptr(priv->wo_info->orig_src_paths);
		cat_unref_ptr(priv->wo_info->orig_library_content_list);
		cat_free_ptr(priv->wo_info);
	}
	if (priv->jre_link) {
		jag_link_remove_referred_by((JagLink *) priv->jre_link, (GObject *) instance);
	}
	cat_unref_ptr(priv->jre_link);
	cat_unref_ptr(priv->module_id_path);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagModuleContentWo *jag_module_content_wo_new(JagMooseService *jag_moose_service, MooIdPath *module_id_path) {
	JagModuleContentWo *result = g_object_new(JAG_TYPE_MODULE_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(result);
//	JagModuleContentWoPrivate *priv = result->priv;
	priv->jag_moose_service_ref = cat_weak_reference_new((GObject *) jag_moose_service);
	priv->module_id_path = cat_ref_ptr(module_id_path);
	priv->model_cached = NULL;
	priv->module_settings = jag_module_settings_wo_new();
	priv->resource_content_version = 0;
	priv->a_source_folders = NULL;
	priv->e_src_folder_paths = cat_array_wo_new();
	priv->a_libraries = NULL;
	priv->e_library_content_list = cat_array_wo_new();
	priv->index_content = jag_index_module_content_new();
	priv->jre_link = NULL;
	priv->wo_info = g_new0(WoInfo, 1);
	priv->wo_info->original = NULL;
	priv->wo_info->orig_src_paths = NULL;
	priv->wo_info->orig_library_content_list = NULL;
	priv->wo_info->marked = FALSE;
	return result;
}


JagModuleContentWo *jag_module_content_wo_ensure_editable(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	if (priv->wo_info) {
		return cat_ref_ptr(module_content);
	}

	JagModuleContentWo *result = g_object_new(JAG_TYPE_MODULE_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagModuleContentWoPrivate *opriv = jag_module_content_wo_get_instance_private(result);
//	JagModuleContentWoPrivate *opriv = result->priv;
	opriv->jag_moose_service_ref = cat_ref_ptr(priv->jag_moose_service_ref);
	opriv->module_id_path = cat_ref_ptr(priv->module_id_path);
	opriv->model_cached = cat_ref_ptr(priv->model_cached);
	opriv->module_settings = cat_ref_ptr(priv->module_settings);
	opriv->e_src_folder_paths = cat_ref_ptr(priv->e_src_folder_paths);
	opriv->a_source_folders = cat_ref_ptr(priv->a_source_folders);
	opriv->a_libraries = cat_ref_ptr(priv->a_libraries);
	opriv->e_library_content_list = cat_ref_ptr(priv->e_library_content_list);
	opriv->index_content = cat_ref_ptr(priv->index_content);
	opriv->jre_link = cat_ref_ptr(priv->jre_link);
	opriv->wo_info = g_new0(WoInfo, 1);
	opriv->wo_info->original = cat_ref_ptr(module_content);
	opriv->wo_info->orig_src_paths = cat_ref_ptr(priv->e_src_folder_paths);
	opriv->wo_info->orig_library_content_list = cat_ref_ptr(priv->e_library_content_list);
	opriv->wo_info->marked = FALSE;
	if (opriv->jre_link) {
		jag_link_add_referred_by((JagLink *) opriv->jre_link, (GObject *) result);
	}
	return result;
}

JagModuleContentWo *jag_module_content_wo_anchor(JagModuleContentWo *module_content, int version) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	if (priv->wo_info==NULL) {
		return module_content;
	}

	cat_log_debug("anchoring:%p", module_content);

	if (priv->e_src_folder_paths!=priv->wo_info->orig_src_paths) {
		int count = cat_array_wo_size(priv->e_src_folder_paths);
		for(count--; count>=0; count--) {
			JagSrcFolderPathWo *src_folder_path = (JagSrcFolderPathWo *) cat_array_wo_get(priv->e_src_folder_paths, count);
			cat_log_debug("src_folder_path[%d]=%o", count, src_folder_path);
			if (!jag_src_folder_path_wo_is_fixed(src_folder_path)) {
				cat_ref_ptr(src_folder_path);
				JagSrcFolderPathWo *replacement = jag_src_folder_path_wo_anchor(src_folder_path, version);
				cat_array_wo_set(priv->e_src_folder_paths, (GObject *) replacement, count, NULL);
				cat_unref_ptr(replacement);
			}
		}
		if (cat_array_wo_equal(priv->e_src_folder_paths, priv->wo_info->orig_src_paths, NULL)) {
			cat_ref_swap(priv->e_src_folder_paths, priv->wo_info->orig_src_paths);
		}
	}

	if (priv->e_library_content_list!=priv->wo_info->orig_library_content_list) {
		int count = cat_array_wo_size(priv->e_library_content_list);
		for(count--; count>=0; count--) {
			JagLibraryContentWo *library_content = (JagLibraryContentWo *) cat_array_wo_get(priv->e_library_content_list, count);
			cat_log_debug("library_content[%d]=%o", count, library_content);
			if (!jag_library_content_wo_is_fixed(library_content)) {
				cat_ref_ptr(library_content);
				JagLibraryContentWo *replacement = (JagLibraryContentWo *) moo_icontent_anchor((MooIContent *) library_content, version);
				cat_array_wo_set(priv->e_library_content_list, (GObject *) replacement, count, NULL);
				cat_unref_ptr(replacement);
			}
		}
		if (cat_array_wo_equal(priv->e_library_content_list, priv->wo_info->orig_library_content_list, NULL)) {
			cat_ref_swap(priv->e_library_content_list, priv->wo_info->orig_library_content_list);
		}
	}


	if (!jag_module_settings_wo_is_fixed(priv->module_settings)) {
		priv->module_settings = jag_module_settings_wo_anchor(priv->module_settings, version);
	}


	if (priv->wo_info->original) {
		JagModuleContentWoPrivate *opriv = jag_module_content_wo_get_instance_private(priv->wo_info->original);
		gboolean was_modified = priv->wo_info->marked
				|| priv->model_cached!=opriv->model_cached
				|| priv->module_settings!=opriv->module_settings
				|| priv->a_source_folders!=opriv->a_source_folders
				|| priv->e_src_folder_paths!=priv->wo_info->orig_src_paths
				|| priv->e_library_content_list!=priv->wo_info->orig_library_content_list
				|| priv->a_libraries!=opriv->a_libraries
				|| priv->jre_link!=opriv->jre_link
				|| priv->resource_content_version!=opriv->resource_content_version;

		cat_log_debug("was_modified=%d", was_modified);

		if (!was_modified) {
			JagModuleContentWo *result = priv->wo_info->original;
			priv->wo_info->original = NULL;
			cat_unref_ptr(module_content);
			return result;
		}
	}
	priv->version = version;
	cat_unref_ptr(priv->wo_info->original);
	cat_unref_ptr(priv->wo_info->orig_src_paths);
	cat_unref_ptr(priv->wo_info->orig_library_content_list);
	cat_free_ptr(priv->wo_info);
	return module_content;
}

gboolean jag_module_content_wo_is_fixed(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return priv->wo_info==NULL;
}

int jag_module_content_wo_get_version(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return priv->version;
}


JagIndexModuleContent *jag_module_content_wo_get_index_context(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return priv->index_content;
}


gboolean jag_module_content_wo_test_resource_content_version(JagModuleContentWo *module_content, int next_version) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	int cur_version = priv->resource_content_version;
	return next_version>cur_version;
}


#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:"); \
			return rval; \
		} \


void jag_module_content_wo_set_resource_content_version(JagModuleContentWo *e_module_content, int next_version) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	CHECK_IF_WRITABLE()
	priv->resource_content_version = next_version;
}

void jag_module_content_wo_mark(JagModuleContentWo *e_module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	CHECK_IF_WRITABLE()
	priv->wo_info->marked = TRUE;
}


static CatS l_s_txt_java = CAT_S_DEF("java");
static CatS l_s_txt_java_src_folders = CAT_S_DEF("java_source_folders");
static CatS l_s_txt_libraries = CAT_S_DEF("libraries");
static CatS l_s_txt_jre_name = CAT_S_DEF("jre_name");


static void l_update_module_settings(JagModuleContentWo *e_module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	CatArrayWo *e_sourceFolderList = cat_array_wo_new(); //<CatArray<CatStringWo>>
	CatArrayWo *e_libraryList = cat_array_wo_new(); //CatStringWo>
	CatStringWo *new_jre_name = NULL;
	cat_log_debug("priv->model_cached=%o", priv->model_cached);
	if (priv->model_cached!=NULL) {
		ShoReference *javaRef = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) priv->model_cached, CAT_S(l_s_txt_java));
		cat_log_debug("javaRef=%o", javaRef);
		if (javaRef!=NULL) {

			ShoIValue *value = sho_reference_get_value(javaRef);
			cat_log_debug("javaRef.value=%o", value);
			if (SHO_IS_BLOCK_VALUE(value)) {
				ShoBlockValue *block = (ShoBlockValue *) value;

				/* source folders */
				ShoReference *srcFoldersRef = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) block, CAT_S(l_s_txt_java_src_folders));
				cat_log_debug("javaRef.value.srcFoldersRef=%o", srcFoldersRef);
				value = srcFoldersRef==NULL ? NULL : sho_reference_get_value(srcFoldersRef);
				if (SHO_IS_LIST_VALUE(value)) {
					ShoListValue *srcFoldersList = (ShoListValue *) value;
					cat_log_debug("javaRef.value.srcFoldersRef.list=%o", srcFoldersList);
					CatIIterator *iter = sho_list_value_iterator(srcFoldersList);
					while(cat_iiterator_has_next(iter)) {
						ShoIValue *lv = (ShoIValue *) cat_iiterator_next(iter);
						cat_log_debug("lv=%o", lv);
						if (SHO_IS_SIMPLE_VALUE(lv)) {
							CatStringWo *folder_name = sho_simple_value_get_string((ShoSimpleValue *) lv);
							CatArrayWo *e_split = cat_string_wo_split(folder_name, '/', FALSE);
							cat_array_wo_append(e_sourceFolderList, (GObject *) e_split);
							cat_unref_ptr(e_split);
						}
					}
					cat_unref_ptr(iter);
				}

				/* libraries */
				ShoReference *librariesRef = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) block, CAT_S(l_s_txt_libraries));
				cat_log_debug("javaRef.value.librariesRef=%o", librariesRef);

				value = librariesRef==NULL ? NULL : sho_reference_get_value(librariesRef);
				if (SHO_IS_LIST_VALUE(value)) {
					ShoListValue *librariesList = (ShoListValue *) value;
					cat_log_debug("javaRef.value.librariesRef.list=%o", librariesList);
					CatIIterator *iter = sho_list_value_iterator(librariesList);
					while(cat_iiterator_has_next(iter)) {
						ShoIValue *lv = (ShoIValue *) cat_iiterator_next(iter);
						cat_log_debug("lv=%o", lv);
						if (SHO_IS_SIMPLE_VALUE(lv)) {
							CatStringWo *full_path = sho_simple_value_get_string((ShoSimpleValue *) lv);
							cat_array_wo_append(e_libraryList, (GObject *) full_path);
						}
					}
					cat_unref_ptr(iter);
				}


				/* compiler */
				ShoSimpleValue *jre_name_sv = sho_reference_list_get_simple_value((ShoReferenceList *) block, CAT_S(l_s_txt_jre_name));
				if (jre_name_sv) {
					new_jre_name = sho_simple_value_get_string(jre_name_sv);
				}
			}
		}
	} else {
		// TODO remove config
	}


	if (jag_module_settings_wo_is_fixed(priv->module_settings)) {
		JagModuleSettingsWo *settings = priv->module_settings;
		priv->module_settings = jag_module_settings_wo_ensure_writable(settings);
		cat_unref_ptr(settings);
	}

	CatArrayWo *a_fsrclist = cat_array_wo_anchor(e_sourceFolderList, 0);
	jag_module_settings_wo_set_source_folders(priv->module_settings, a_fsrclist);
	cat_unref_ptr(a_fsrclist);

	CatArrayWo *a_fliblist = cat_array_wo_anchor(e_libraryList, 0);
	jag_module_settings_wo_set_libraries(priv->module_settings, a_fliblist);
	cat_unref_ptr(a_fliblist);

	jag_module_settings_wo_set_jre_name(priv->module_settings, new_jre_name);
}


void jag_module_content_wo_reconfigure(JagModuleContentWo *e_module_content, ShoModel *model) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	if (model==priv->model_cached) {
		return;
	}

	cat_log_on_debug({
		if (model!=NULL) {
			ShoSerializer *sho_serialiazer = sho_serializer_new(model);
			CatStringWo *ser = sho_serialize_to_char_array(sho_serialiazer);
			cat_log_debug("ser=%o", ser);
		}
	});

	CHECK_IF_WRITABLE()

	cat_ref_swap(priv->model_cached, model);
	l_update_module_settings(e_module_content);
}

static CatStringWo *l_create_flat_path(CatArrayWo *e_entries) {
	CatStringWo *e_result = cat_string_wo_new_with("");
	CatIIterator *iter = cat_array_wo_iterator(e_entries);
	while(cat_iiterator_has_next(iter)) {
		if (cat_string_wo_length(e_result)>0) {
			cat_string_wo_append_unichar(e_result, '/');
		}
		CatStringWo *a_entry_txt = cat_iiterator_next(iter);
		cat_string_wo_append(e_result, a_entry_txt);
	}
	cat_unref_ptr(iter);
	return e_result;
}

void jag_module_content_write_settings_2_model(JagModuleContentWo *module_content, ShoModel *model) {
//	ShoReference *javaRef = sho_reference_list_get_first_reference_by_id((ShoReferenceList *) priv->model_cached, CAT_S(l_s_txt_java));

	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	JagModuleSettingsWo *jag_settings = priv->module_settings;

	cat_log_debug("jag_settings=%o", jag_settings);



	ShoBlockValue *block_value = sho_block_value_new();

//		sho_block_value_add_simple_string(block_value, CAT_S(jag_s_java_compiler), jag_project_settings_get_selected_compiler(jag_settings));

		/* source folders */
		CatArrayWo *a_source_folders = jag_module_settings_wo_get_source_folders(jag_settings);
		cat_log_debug("source_folders=%p", a_source_folders);
		if (a_source_folders) {
			ShoListValue *source_folders_list = sho_list_value_new();

			CatIIterator *iter = cat_array_wo_iterator(a_source_folders);
			while(cat_iiterator_has_next(iter)) {
				CatArrayWo *e_path_entries = (CatArrayWo *) cat_iiterator_next(iter);
				CatStringWo *a_entry = l_create_flat_path(e_path_entries);
				ShoSimpleValue *entry_val = sho_simple_value_new_string(a_entry);
				sho_list_value_add_value(source_folders_list, (ShoIValue *) entry_val);
				cat_unref_ptr(entry_val);
			}
			cat_unref_ptr(iter);

			ShoReference *ref = sho_reference_new(CAT_S(l_s_txt_java_src_folders));
			sho_reference_set_value(ref, SHO_IVALUE(source_folders_list));
			cat_unref_ptr(source_folders_list);
			sho_reference_list_add_reference((ShoReferenceList *) block_value, ref);
			cat_unref_ptr(ref);

		}

		/* libraries */
		CatArrayWo *a_libraries = jag_module_settings_wo_get_libraries(jag_settings);
		cat_log_debug("libraries=%p", a_libraries);
		if (a_libraries) {
			ShoListValue *libraries_list = sho_list_value_new();
			CatIIterator *iter = cat_array_wo_iterator(a_libraries);
			while(cat_iiterator_has_next(iter)) {
				CatStringWo *entry = (CatStringWo *) cat_iiterator_next(iter);
				ShoSimpleValue *entry_val = sho_simple_value_new_string(entry);
				sho_list_value_add_value(libraries_list, (ShoIValue *) entry_val);
				cat_unref_ptr(entry_val);
			}
			cat_unref_ptr(iter);
			ShoReference *ref = sho_reference_new(CAT_S(l_s_txt_libraries));
			sho_reference_set_value(ref, SHO_IVALUE(libraries_list));
			cat_unref_ptr(libraries_list);
			sho_reference_list_add_reference((ShoReferenceList *) block_value, ref);
			cat_unref_ptr(ref);
		}

		/* compiler */
		CatStringWo *a_jre_name = jag_module_settings_wo_get_jre_name(jag_settings);
		if (a_jre_name) {
			sho_reference_list_set_string((ShoReferenceList *) block_value, CAT_S(l_s_txt_jre_name), a_jre_name);
		}
		cat_log_debug("jre_name=%o", a_jre_name);

//			con_unmap_context_set_simple_string(unmap_context, block_value, CAT_S(pro_s_workspace), pro_configuration_get_workspace_path(pro_config));
//
	ShoReference *main_config_ref = sho_reference_new(CAT_S(l_s_txt_java));
	sho_reference_set_value(main_config_ref, SHO_IVALUE(block_value));
	sho_reference_list_set_reference((ShoReferenceList *) model, main_config_ref);
	cat_unref_ptr(main_config_ref);
	cat_unref_ptr(block_value);
}


JagModuleSettingsWo *jag_module_content_wo_get_module_settings(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return priv->module_settings;
}


gboolean jag_module_content_wo_set_module_settings(JagModuleContentWo *e_module_content, JagModuleSettingsWo *new_settings) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	if (priv->module_settings == new_settings) {
		return FALSE;
	}
	CHECK_IF_WRITABLE(FALSE)

	cat_unref_ptr(priv->model_cached);
	cat_ref_swap(priv->module_settings, new_settings);
	return TRUE;
}


CatStringWo *jag_module_content_wo_key() {
	return CAT_S(jag_s_module_content_key);
}

static int l_find_library_content(CatArrayWo *e_library_content_list, int in_index, CatTreeNode *vip_tn) {
	VipNode *vip_node = (VipNode *) cat_tree_node_get_content(vip_tn);
	long long vip_node_id = vip_node_get_id(vip_node);

	JagLibraryContentWo *library_content = (JagLibraryContentWo *) cat_array_wo_get(e_library_content_list, in_index);
	if (library_content) {
		CatTreeNode *t_vip_tn = jag_library_content_wo_get_vip_node(library_content);
		VipNode * t_vip_node = (VipNode *) cat_tree_node_get_content(t_vip_tn);
		if (vip_node_get_id(t_vip_node)==vip_node_id) {
			return in_index;
		}
	}

	int idx;
	for(idx = cat_array_wo_size(e_library_content_list)-1; idx>=0; idx--) {
		JagLibraryContentWo *library_content = (JagLibraryContentWo *) cat_array_wo_get(e_library_content_list, idx);
		CatTreeNode *t_vip_tn = jag_library_content_wo_get_vip_node(library_content);
		VipNode * t_vip_node = (VipNode *) cat_tree_node_get_content(t_vip_tn);
		if (vip_node_get_id(t_vip_node)==vip_node_id) {
			return idx;
		}
	}
	return -1;
}

gboolean jag_module_content_wo_set_libraries(JagModuleContentWo *e_module_content, CatArrayWo *a_libraries) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	if (priv->a_libraries==a_libraries) {
		return FALSE;
	}
	CHECK_IF_WRITABLE(FALSE)

	JagMooseService *jag_moose_service = (JagMooseService *) cat_weak_reference_get(priv->jag_moose_service_ref);
	if (jag_moose_service==NULL) {
		return FALSE;
	}

	cat_ref_swap(priv->a_libraries, a_libraries);


	gboolean changed = FALSE;

	JagIndexerJarMap *jar_map = jag_moose_service_get_jar_map(jag_moose_service);

	MooService *moo_service = jag_moose_service_get_moo_service(jag_moose_service);
	VipService *vip_service = moo_service_get_viper_service(moo_service);

//	JagLibraryPathMatcher *matcher = jag_library_path_matcher_new();


	if (a_libraries) {
		int in_index = 0;
		CatIIterator *iter = cat_array_wo_iterator(a_libraries);
		CatArrayWo *e_new_library_content_list = NULL;
		if (cat_array_wo_size(a_libraries)!=cat_array_wo_size(priv->e_library_content_list)) {
			e_new_library_content_list = cat_array_wo_new();
		}
//		MooINodeMatcher *matcher = (MooINodeMatcher *) JAG_MODULE_CONTENT_WO_GET_CLASS(e_module_content)->res_name_matcher;
//
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *jar_path = (CatStringWo *) cat_iiterator_next(iter);
			cat_log_debug("jar_path=%o", jar_path);
			VipPath *vip_jar_path = vip_path_new(cat_ref_ptr(jar_path));
			VipCreatePathRequest *path_request = vip_service_create_path(vip_service, vip_jar_path);
			cat_unref_ptr(vip_jar_path);
			VipNodePath *jar_node_path = vip_create_path_request_wait_for_path(path_request);
			CatTreeNode *path_tn = vip_node_path_get_tail_node(jar_node_path);

			cat_log_debug("path_tn=%o", path_tn);

//			MooNamePath *name_path = moo_name_path_new(single_path, matcher);
//
			int new_in_index = l_find_library_content(priv->e_library_content_list, in_index, path_tn);

			if ((e_new_library_content_list!=NULL) || (new_in_index!=in_index)) {
				if (e_new_library_content_list==NULL) {
					e_new_library_content_list = cat_array_wo_new();
					int pidx;
					for(pidx=0; pidx<in_index; pidx++) {
						GObject *ow = cat_array_wo_get(priv->e_library_content_list, pidx);
						cat_log_trace("ow[%d]=%o", pidx, ow);
						cat_array_wo_append(e_new_library_content_list, ow);
					}
				}
				if (new_in_index>=0) {
					GObject *ow = cat_array_wo_get(priv->e_library_content_list, new_in_index);
					cat_log_trace("new_in_index[%d]=%o", new_in_index, ow);
					cat_array_wo_append(e_new_library_content_list, ow);
					in_index = new_in_index;
				} else {
					JgiJarEntry *jar_entry = jag_indexer_jar_map_attach(jar_map, jar_node_path);
					JagLibraryContentWo *library_content = jag_library_content_wo_new(jag_moose_service, priv->module_id_path, jar_entry);
					jag_library_content_wo_set_vip_node(library_content, path_tn);
					cat_log_trace("new_content=%o", library_content);
					cat_array_wo_append(e_new_library_content_list, (GObject *) library_content);
					cat_unref_ptr(library_content);
				}
			}
			in_index++;
			cat_unref_ptr(path_request);
		}
		cat_unref_ptr(iter);

		if (e_new_library_content_list!=NULL) {
			cat_unref_ptr(priv->e_library_content_list);
			priv->e_library_content_list = e_new_library_content_list;
			changed = TRUE;
		}
	} else {
		if (cat_array_wo_size(priv->e_library_content_list)!=0) {
			changed = TRUE;
			cat_unref_ptr(priv->e_library_content_list);
			priv->e_library_content_list = cat_array_wo_new();
		}
	}

	cat_unref_ptr(jag_moose_service);
	return changed;
}

CatArrayWo *jag_module_content_wo_get_libraries(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return priv->a_libraries;
}

int jag_module_content_wo_library_content_count(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(JAG_MODULE_CONTENT_WO(module_content));
	return cat_array_wo_size(priv->e_library_content_list);

}

JagLibraryContentWo *jag_module_content_wo_editable_library_content_at(JagModuleContentWo *e_module_content, int index) {
	JagModuleContentWoPrivate *priv = (JagModuleContentWoPrivate *) jag_module_content_wo_get_instance_private(e_module_content);
	CHECK_IF_WRITABLE(NULL)
	JagLibraryContentWo *result = (JagLibraryContentWo *) cat_array_wo_get(priv->e_library_content_list, index);
	if (jag_library_content_wo_is_fixed(result)) {
		if (priv->e_library_content_list==priv->wo_info->orig_library_content_list) {
			cat_unref_ptr(priv->e_library_content_list);
			priv->e_library_content_list = cat_array_wo_new();
			cat_array_wo_append_all(priv->e_library_content_list, priv->wo_info->orig_library_content_list);
		}

		result = jag_library_content_wo_ensure_editable(result);
		cat_array_wo_set(priv->e_library_content_list, (GObject *) result, index, NULL);
		cat_unref(result);
	}
	return result;
}


static int l_find_name_path(CatArrayWo *e_src_folder_paths, int in_index, MooNamePath *name_path) {
	JagSrcFolderPathWo *src_folder_path = (JagSrcFolderPathWo *) cat_array_wo_get(e_src_folder_paths, in_index);
	if (src_folder_path) {
		MooNamePath *t_name_path = (MooNamePath *) jag_src_folder_path_wo_get_name_path(src_folder_path);
		if (moo_name_path_equal(name_path, t_name_path)) {
			return in_index;
		}
	}

	int idx;
	for(idx = cat_array_wo_size(e_src_folder_paths)-1; idx>=0; idx--) {
		MooNamePath *t_name_path = (MooNamePath *) cat_array_wo_get(e_src_folder_paths, idx);
		if (moo_name_path_equal(name_path, t_name_path)) {
			return idx;
		}
	}
	return -1;
}

gboolean jag_module_content_wo_set_source_folders(JagModuleContentWo *e_module_content, CatArrayWo *a_source_folders) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	if (priv->a_source_folders==a_source_folders) {
		return FALSE;
	}
	CHECK_IF_WRITABLE(FALSE)
	cat_ref_swap(priv->a_source_folders, a_source_folders);


	gboolean changed = FALSE;

	if (a_source_folders) {
		int in_index = 0;
		CatIIterator *iter = cat_array_wo_iterator(a_source_folders);
		CatArrayWo *e_new_source_paths = NULL;
		if (cat_array_wo_size(a_source_folders)!=cat_array_wo_size(priv->e_src_folder_paths)) {
			e_new_source_paths = cat_array_wo_new();
		}
		MooINodeMatcher *matcher = (MooINodeMatcher *) JAG_MODULE_CONTENT_WO_GET_CLASS(e_module_content)->res_name_matcher;

		while(cat_iiterator_has_next(iter)) {
			CatArrayWo *e_single_path = (CatArrayWo *) cat_iiterator_next(iter);
			MooNamePath *name_path = moo_name_path_new(e_single_path, matcher);

			int new_in_index = l_find_name_path(priv->e_src_folder_paths, in_index, name_path);

			if ((e_new_source_paths!=NULL) || (new_in_index!=in_index)) {
				if (e_new_source_paths==NULL) {
					e_new_source_paths = cat_array_wo_new();
					int pidx;
					for(pidx=0; pidx<in_index; pidx++) {
						cat_array_wo_append(e_new_source_paths, cat_array_wo_get(priv->e_src_folder_paths, pidx));
					}
				}
				JagSrcFolderPathWo *src_folder_path = NULL;
				if (new_in_index>=0) {
					src_folder_path = (JagSrcFolderPathWo *) cat_array_wo_get(priv->e_src_folder_paths, new_in_index);
					cat_ref_ptr(src_folder_path);
					cat_array_wo_append(e_new_source_paths, (GObject *) src_folder_path);
					in_index = new_in_index;
				} else {
					src_folder_path = jag_src_folder_path_wo_new(name_path);
					cat_array_wo_append(e_new_source_paths, (GObject *) src_folder_path);
				}

				cat_unref_ptr(src_folder_path);
			}
			in_index++;
			cat_unref_ptr(name_path);
		}
		cat_unref_ptr(iter);

		if (e_new_source_paths!=NULL) {
			cat_unref_ptr(priv->e_src_folder_paths);
			priv->e_src_folder_paths = e_new_source_paths;
			changed = TRUE;
		}
	} else {
		if (cat_array_wo_size(priv->e_src_folder_paths)!=0) {
			changed = TRUE;
			cat_unref_ptr(priv->e_src_folder_paths);
			priv->e_src_folder_paths = cat_array_wo_new();
		}
	}

	return changed;
}

CatArrayWo *jag_module_content_wo_get_source_folders(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return priv->a_source_folders;
}

int jag_module_content_wo_source_path_count(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return cat_array_wo_size(priv->e_src_folder_paths);
}

JagSrcFolderPathWo *jag_module_content_wo_source_path_at(JagModuleContentWo *module_content, int index) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return (JagSrcFolderPathWo *) cat_array_wo_get(priv->e_src_folder_paths, index);
}

#define CHECK_IF_WRITABLE_LIST(nlist, rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", nlist); \
			return rval; \
		} \
		if (priv->wo_info->original_list == priv->list) { \
			cat_unref_ptr(priv->list); \
			priv->list = cat_array_wo_new(); \
			if (priv->wo_info->original_list) { \
				cat_array_wo_append_all(priv->list, priv->wo_info->original_list); \
			} \
		} \


JagSrcFolderPathWo *jag_module_content_wo_editable_source_path_at(JagModuleContentWo *e_module_content, int index) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	CHECK_IF_WRITABLE(NULL)
	JagSrcFolderPathWo *result = (JagSrcFolderPathWo *) cat_array_wo_get(priv->e_src_folder_paths, index);
	if (jag_src_folder_path_wo_is_fixed(result)) {
		if (priv->e_src_folder_paths==priv->wo_info->orig_src_paths) {
			cat_unref_ptr(priv->e_src_folder_paths);
			priv->e_src_folder_paths = cat_array_wo_new();
			cat_array_wo_append_all(priv->e_src_folder_paths, priv->wo_info->orig_src_paths);
		}

		result = jag_src_folder_path_wo_ensure_editable(result);
		cat_array_wo_set(priv->e_src_folder_paths, (GObject *) result, index, NULL);
		cat_unref(result);
	}
	return result;
}



gboolean jag_module_content_wo_set_jre_link(JagModuleContentWo *e_module_content, JagJreLink *jre_link) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(e_module_content);
	CHECK_IF_WRITABLE(FALSE)
	if (priv->jre_link==jre_link) {
		return FALSE;
	}

	if (jre_link) {
		jag_link_add_referred_by((JagLink *) jre_link, (GObject *) e_module_content);
	}
	if (priv->jre_link) {
		jag_link_remove_referred_by((JagLink *) priv->jre_link, (GObject *) e_module_content);
	}
	cat_ref_swap(priv->jre_link, jre_link);
	return TRUE;
}

JagJreLink *jag_module_content_wo_get_jre_link(JagModuleContentWo *module_content) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private(module_content);
	return priv->jre_link;
}



/********************* start MooIContent implementation *********************/


static CatStringWo *l_content_get_key(MooIContent *self) {
	return jag_module_content_wo_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return (MooIContent *) jag_module_content_wo_anchor((JagModuleContentWo *) self, version);
}



static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/




/********************* start JagILinkListener implementation *********************/

static void l_link_notify(JagILinkListener *self, struct _JagLink *link, GObject *notify_data) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private((JagModuleContentWo *) self);
	cat_log_debug("self=%p", self)
	cat_log_debug("self=%o", self)
//	priv->module_id_path;
	JagMooseService *jag_moose_service = (JagMooseService *) cat_weak_reference_get(priv->jag_moose_service_ref);
	if (jag_moose_service==NULL) {
		return;
	}

	WorRequest *refresh_compiler_request = (WorRequest *) jag_refresh_compiler_request_new(jag_moose_service, priv->module_id_path);
	WorService *wor_service = jag_moose_service_get_worm_service(jag_moose_service);
	cat_log_debug("wor_service=%o", wor_service)
	wor_service_post_request(wor_service, refresh_compiler_request);
	cat_unref_ptr(refresh_compiler_request);
	cat_unref_ptr(jag_moose_service);
}

static void l_link_listener_iface_init(JagILinkListenerInterface *iface) {
	iface->notify = l_link_notify;
}

/********************* end JagILinkListener implementation *********************/




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagModuleContentWoPrivate *priv = jag_module_content_wo_get_instance_private((JagModuleContentWo *) self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s, version=%d, resource_content_version=%d, jag_moose_service=%p, settings=%o, libraries=%o", iname, self, priv->wo_info ? "editable" : "anchored", priv->version, priv->resource_content_version, priv->jag_moose_service_ref, priv->module_settings, priv->a_libraries);

	if (priv->e_src_folder_paths) {
		cat_string_wo_append_chars(append_to, ", src_folder_paths[");
		gboolean is_first = TRUE;
		CatIIterator *iter = cat_array_wo_iterator(priv->e_src_folder_paths);
		while(cat_iiterator_has_next(iter)) {
			GObject *obj = cat_iiterator_next(iter);
			if (is_first) {
				is_first = FALSE;
			} else {
				cat_string_wo_append_chars(append_to, ", ");
			}
			cat_string_wo_format(append_to, "%o", obj);
		}
		cat_unref_ptr(iter);
		cat_string_wo_append_char(append_to, ']');
	}

	if (priv->e_library_content_list) {
		cat_string_wo_append_chars(append_to, ", lib-cont-lst[");
		gboolean is_first = TRUE;
		CatIIterator *iter = cat_array_wo_iterator(priv->e_library_content_list);
		while(cat_iiterator_has_next(iter)) {
			GObject *obj = cat_iiterator_next(iter);
			if (is_first) {
				is_first = FALSE;
			} else {
				cat_string_wo_append_chars(append_to, ", ");
			}
			cat_string_wo_format(append_to, "%o", obj);
		}
		cat_unref_ptr(iter);
		cat_string_wo_append_char(append_to, ']');
	}

	cat_string_wo_append_char(append_to, ']');
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


