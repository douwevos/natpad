/*
   File:    jagrefreshmodulerequest.c
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

#include "jagrefreshmodulerequest.h"
#include "srcfolder/jagsrcfoldercontentwo.h"
#include "srcfolder/jagsrcfoldermatcher.h"
#include "srcfolder/jagsrcfolderupdater.h"
#include "srcfolder/jagrefreshsrcfolderrequest.h"
#include "library/jagrefreshlibrariesrequest.h"
#include "module/jagrefreshcompilerrequest.h"
#include "module/jagmodulecontentwo.h"
#include "module/jagmodulesettingswo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagRefreshModuleRequest"
#include <logging/catlog.h>

struct _JagRefreshModuleRequestPrivate {
	JagMooseService *jag_moose_service;
	WorRequest *refresh_compiler_request;
	long long unique_id;
	int last_known_node_idx;
	gboolean was_modified, libraries_modified;
};

G_DEFINE_TYPE (JagRefreshModuleRequest, jag_refresh_module_request, WOR_TYPE_REQUEST)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_refresh_module_request_class_init(JagRefreshModuleRequestClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagRefreshModuleRequestPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}


static void jag_refresh_module_request_init(JagRefreshModuleRequest *instance) {
	JagRefreshModuleRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_REFRESH_MODULE_REQUEST, JagRefreshModuleRequestPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagRefreshModuleRequest *instance = JAG_REFRESH_MODULE_REQUEST(object);
	JagRefreshModuleRequestPrivate *priv = instance->priv;
	cat_unref_ptr(priv->jag_moose_service);
	cat_unref_ptr(priv->refresh_compiler_request);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagRefreshModuleRequest *jag_refresh_module_request_new(JagMooseService *jag_mooose_service, long long unique_id, int last_known_node_idx) {
	JagRefreshModuleRequest *result = g_object_new(JAG_TYPE_REFRESH_MODULE_REQUEST, NULL);
	cat_ref_anounce(result);
	JagRefreshModuleRequestPrivate *priv = result->priv;
	wor_request_construct((WorRequest *) result);
	priv->jag_moose_service = cat_ref_ptr(jag_mooose_service);
	priv->last_known_node_idx = last_known_node_idx;
	priv->unique_id = unique_id;
	priv->was_modified = FALSE;
	priv->refresh_compiler_request = NULL;
	priv->libraries_modified = FALSE;
	return result;
}


static MooNodeWo *l_refresh_module(JagRefreshModuleRequestPrivate *priv, MooTransaction *transaction, int nodeIndex);


static void l_run_request(WorRequest *request) {

	JagRefreshModuleRequestPrivate *priv = JAG_REFRESH_MODULE_REQUEST_GET_PRIVATE(request);
	MooService *moo_service = jag_moose_service_get_moo_service(priv->jag_moose_service);


	MooTransaction *tx = moo_service_create_transaction((GObject *) request, moo_service);
	while(moo_transaction_retry(tx)) {
		moo_transaction_begin(tx);
		MooNodeWo *rootNode = moo_transaction_get_tx_root_node(tx);
		MooIdNodeMatcher *mooIdMatcher = moo_id_node_matcher_new(priv->unique_id);
		int nodeIndex = moo_node_wo_find_index(rootNode, (MooINodeMatcher *) mooIdMatcher, NULL, priv->last_known_node_idx);
		cat_unref_ptr(mooIdMatcher);
		cat_log_debug("nodeIndex=%d, rootNode=%o", nodeIndex, rootNode);

		if (nodeIndex>=0) {

			MooNodeWo *e_root_node = l_refresh_module(priv, tx, nodeIndex);
			cat_log_debug("refreshModule=%o", e_root_node);
			if (e_root_node==NULL) {
				cat_unref_ptr(tx);
				return;
			}

			gboolean tx_succes = moo_transaction_commit(tx, e_root_node);

			if (tx_succes) {
				cat_log_debug("done");
				break;
			} else {
				priv->was_modified = FALSE;
				priv->libraries_modified = FALSE;
				cat_log_debug("failed commit");
			}
		} else {
			cat_log_debug("uniqueId=%d not found", priv->unique_id);
 		}
	}


	cat_log_debug("was_modified=%d, libraries_modified=%d", priv->was_modified, priv->libraries_modified);


	WorService *wor_service = moo_service_get_worm_service(moo_service);
	if (priv->was_modified) {
		JagRefreshSrcFolderRequest *req = jag_refresh_src_folder_request_new(moo_service, priv->unique_id, priv->last_known_node_idx);
		wor_service_post_request(wor_service, (WorRequest *) req);
		cat_unref_ptr(req);
	}
	if (priv->libraries_modified) {
		JagRefreshLibrariesRequest *req = jag_refresh_libraries_request_new(moo_service, priv->unique_id, priv->last_known_node_idx);
		wor_service_post_request(wor_service, (WorRequest *) req);
		cat_unref_ptr(req);

	}

	if (priv->refresh_compiler_request) {
		wor_service_post_request(wor_service, priv->refresh_compiler_request);
	}
	cat_unref_ptr(tx);
}


static MooNodeWo *l_refresh_module(JagRefreshModuleRequestPrivate *priv, MooTransaction *transaction, int nodeIndex) {

	cat_unref_ptr(priv->refresh_compiler_request);
	priv->was_modified = FALSE;
	priv->libraries_modified = FALSE;
	MooNodeWo *tx_node = moo_transaction_get_tx_root_node(transaction);

	MooNodeWo *moduleNode = moo_node_wo_child_at(tx_node, nodeIndex);
	cat_log_debug("moduleNode=%o", moduleNode);
	JagModuleContentWo *jagContent = (JagModuleContentWo *) moo_node_wo_get_content(moduleNode, jag_module_content_wo_key());
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(moduleNode, moo_resource_content_wo_key());

	int resource_content_change_count = moo_resource_content_wo_get_version(resource_content);



	gboolean shouldRefresh = jagContent==NULL;
	shouldRefresh = TRUE;
	if (!shouldRefresh) {
		if (jag_module_content_wo_test_resource_content_version(jagContent, resource_content_change_count)) {
			shouldRefresh = TRUE;
		}
	}

//	cat_log_debug("shouldRefresh="+shouldRefresh);


	MooIdPath *mainPath = moo_id_path_new();
	cat_ref_sink_ptr(mainPath);


	if (shouldRefresh) {
		MooNodeWo *e_root_node = moo_node_wo_ensure_editable((MooNodeWo *) tx_node, NULL);
		MooNodeWo *e_module_node = moo_node_wo_get_editable_child_at(e_root_node, nodeIndex);


		MooIdPath *modulePath = moo_id_path_append_id(mainPath, moo_node_wo_get_unique_id(e_module_node), nodeIndex);
		cat_ref_sink_ptr(modulePath);

		MooModuleContentWo *moduleContent = (MooModuleContentWo *) moo_node_wo_get_content(e_module_node, moo_module_content_wo_key());

		cat_log_debug("moduleContent=%o, jagContent=%o", moduleContent, jagContent);
		// TODO test moduleContent if java nature is enabled

		JagModuleContentWo *e_jag_module_content = NULL;
		if (jagContent==NULL) {
			e_jag_module_content = jag_module_content_wo_new(priv->jag_moose_service, modulePath);
		} else {
			e_jag_module_content = jag_module_content_wo_ensure_editable(jagContent);
		}
		jag_module_content_wo_set_resource_content_version(e_jag_module_content, resource_content_change_count);
		moo_node_wo_set_content(e_module_node, (MooIContent *) e_jag_module_content);
		cat_unref(e_jag_module_content);

		ShoModel *sho_model = moo_module_content_wo_get_model(moduleContent);
		cat_log_debug("new sho_model=%o", sho_model);
//		sho_model_dump(sho_model);
		jag_module_content_wo_reconfigure(e_jag_module_content, sho_model);


		JagModuleSettingsWo *moduleSettings = jag_module_content_wo_get_module_settings(e_jag_module_content);
		cat_log_debug("new moduleSettings=%o", moduleSettings);
		CatArrayWo *a_sourceFolders = jag_module_settings_wo_get_source_folders(moduleSettings);
		cat_log_debug("new sourceFolders=%o", a_sourceFolders);


		priv->was_modified = jag_module_content_wo_set_source_folders(e_jag_module_content, a_sourceFolders);

		if (!priv->was_modified) {
			int src_path_count = jag_module_content_wo_source_path_count(e_jag_module_content);
			for(src_path_count--; src_path_count>=0; src_path_count--) {
				JagSrcFolderPathWo *src_folder_path = jag_module_content_wo_source_path_at(e_jag_module_content, src_path_count);
				MooNamePath *name_path = jag_src_folder_path_wo_get_name_path(src_folder_path);
				MooNodeWo *src_folder_res_node = moo_name_path_get_tail(name_path, e_module_node);
				if (src_folder_res_node) {
					int new_res_version = moo_node_wo_get_version(src_folder_res_node);
					if (jag_src_folder_path_wo_test_resource_node_version(src_folder_path, new_res_version)) {
						priv->was_modified = TRUE;
						break;
					}
				}
			}
		}



		CatArrayWo *a_libraries = jag_module_settings_wo_get_libraries(moduleSettings);

		priv->libraries_modified = jag_module_content_wo_set_libraries(e_jag_module_content, a_libraries);


		CatStringWo *a_jre_name = jag_module_settings_wo_get_jre_name(moduleSettings);
		cat_log_debug("new jre_name=%o", a_jre_name);
		if (a_jre_name!=NULL) {
			JagIndexerJreMap *jre_map = jag_moose_service_get_jre_map(priv->jag_moose_service);
			JagJreLink *jre_link = jag_indexer_jre_map_get_jre_link_ref(jre_map, a_jre_name);

			if (jag_module_content_wo_set_jre_link(e_jag_module_content, jre_link)) {

				priv->refresh_compiler_request = (WorRequest *) jag_refresh_compiler_request_new(priv->jag_moose_service, modulePath);

				priv->was_modified = TRUE;
			}
			cat_unref_ptr(jre_link);
		} else {

			// TODO: I don't know .... no jre_name and then?

		}






//		/* compiler */
//		int foundIndex = editableChild.findIndex(new JagCompilerMatcher(), -1);
//		MooNode nodeCompiler = NULL;
//		MooNodeWo editableCompiler = NULL;
//		if (foundIndex>=0) {
////				nodeCompiler = (JagNodeCompiler) module.childAt(foundIndex);
////				editableCompiler = editableChild.getEditableChild(foundIndex);
//		} else {
//			nodeCompiler = mooseContext.getCompilerNode("java");
//			if (nodeCompiler!=NULL) {
//////					nodeCompiler = new JagNodeCompiler(transaction.uniqueIdCounter.incrementAndGet());
//				editableCompiler = editableChild.addNewChild(nodeCompiler);
////					cat_log_debug("added new child");
//			} else {
//				// TODO remove compiler from module
//			}
//		}

//		return NULL;
		cat_log_debug("unreffing path");
		cat_unref_ptr(modulePath);
		cat_unref_ptr(mainPath);
//		cat_unref_ptr(indexesFresh);
		cat_log_debug("done");
		return e_root_node;

	}
	return NULL;
}

