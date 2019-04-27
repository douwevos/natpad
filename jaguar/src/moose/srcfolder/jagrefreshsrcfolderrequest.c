/*
 File:    jagrefreshsrcfolderrequest.c
 Project: jaguar
 Author:  Douwe Vos
 Date:    May 26, 2014
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

#include "jagrefreshsrcfolderrequest.h"
#include "jagsrcfoldercontentwo.h"
#include "jagsrcfoldermatcher.h"
#include "jagsrcfolderupdater.h"
#include "../module/jagmodulecontentwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagRefreshSrcFolderRequest"
#include <logging/catlog.h>

struct _JagRefreshSrcFolderRequestPrivate {
	MooService *moo_service;
	long long module_node_id;
	int last_known_node_idx;
};

static void l_mergeable_request_iface_init(WorIMergeableRequestInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagRefreshSrcFolderRequest, jag_refresh_src_folder_request, WOR_TYPE_REQUEST, {
		G_ADD_PRIVATE(JagRefreshSrcFolderRequest)
		G_IMPLEMENT_INTERFACE(WOR_TYPE_IMERGEABLE_REQUEST, l_mergeable_request_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_refresh_src_folder_request_class_init(JagRefreshSrcFolderRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}

static void jag_refresh_src_folder_request_init(JagRefreshSrcFolderRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagRefreshSrcFolderRequest *instance = JAG_REFRESH_SRC_FOLDER_REQUEST(object);
	JagRefreshSrcFolderRequestPrivate *priv = jag_refresh_src_folder_request_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	G_OBJECT_CLASS(jag_refresh_src_folder_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_refresh_src_folder_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagRefreshSrcFolderRequest *jag_refresh_src_folder_request_new(MooService *moo_service, long long module_node_id, int last_known_node_idx) {
	JagRefreshSrcFolderRequest *result = g_object_new(JAG_TYPE_REFRESH_SRC_FOLDER_REQUEST, NULL);
	cat_ref_anounce(result);
	JagRefreshSrcFolderRequestPrivate *priv = jag_refresh_src_folder_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	wor_request_set_time_out((WorRequest *) result, 400);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->module_node_id = module_node_id;
	priv->last_known_node_idx = last_known_node_idx;
	return result;
}

static void l_run(JagRefreshSrcFolderRequest *src_folder_request, MooNodeWo *e_root_node, MooNodeWo *e_module_node,
        JagModuleContentWo *e_jag_content) {
	JagRefreshSrcFolderRequestPrivate *priv = jag_refresh_src_folder_request_get_instance_private(src_folder_request);
	MooNodeListWo *e_module_children = moo_node_wo_get_editable_children(e_module_node);
	MooIdPath *mainPath = moo_id_path_new();
	MooIdPath *modulePath = moo_id_path_append_id(mainPath, moo_node_wo_get_unique_id(e_module_node), priv->last_known_node_idx);
	cat_unref_ptr(mainPath);
	cat_ref_sink_ptr(modulePath);

	JagSrcFolderMatcher *src_folder_matcher = jag_src_folder_matcher_new();

	CatHashSet /* <Integer> */*indexesFresh = cat_hash_set_new((GHashFunc) cat_integer_hash, (GEqualFunc) cat_integer_equal);

	int path_count = jag_module_content_wo_source_path_count(e_jag_content);

	if (path_count > 0) {
		cat_log_debug("path_count=%d", path_count);

		for (path_count--; path_count >= 0; path_count--) {
			JagSrcFolderPathWo *e_src_folder_path = jag_module_content_wo_editable_source_path_at(e_jag_content, path_count);
			cat_log_debug("e_src_folder_path=%o", e_src_folder_path);
			MooNamePath *name_path = jag_src_folder_path_wo_get_name_path(e_src_folder_path);
			cat_log_debug("name_path=%o", name_path);
			CatStringWo *a_nodeName = moo_name_path_create_flatten(name_path);
			cat_log_debug("nodeName=%o", a_nodeName);

			MooNodeWo *src_folder_res_node = moo_name_path_get_tail(name_path, e_module_node);
			if (src_folder_res_node) {
				int version = moo_node_wo_get_version(src_folder_res_node);
				jag_src_folder_path_wo_update_resource_node_version(e_src_folder_path, version);

				jag_module_content_wo_mark(e_jag_content);
			} else {
				// TODO the src_folder_path does not exists or stopped to exist
			}

			int foundIndex = moo_node_wo_find_index(e_module_node, (MooINodeMatcher *) src_folder_matcher, a_nodeName, -1);
			cat_log_debug("src_folder_res_node=%p, foundIndex=%d", src_folder_res_node, foundIndex);
			if (src_folder_res_node != NULL) {
				MooNodeWo *e_src_folder_node = NULL;
				if (foundIndex >= 0) {
					e_src_folder_node = moo_node_wo_get_editable_child_at(e_module_node, foundIndex);
					moo_node_wo_set_z_order(e_src_folder_node, 20000 + path_count);
					CatInteger *it = cat_integer_new(foundIndex);
					cat_hash_set_add(indexesFresh, (GObject *) it);
					cat_unref_ptr(it);
				} else {
					CatInteger *it = cat_integer_new(moo_node_wo_child_count((MooNodeWo *) e_module_node));
					cat_hash_set_add(indexesFresh, (GObject *) it);
					cat_unref_ptr(it);
					long long uid = vip_isequence_next(VIP_ISEQUENCE(priv->moo_service));
					e_src_folder_node = moo_node_wo_new(uid, cat_ref_ptr(a_nodeName), NULL, NULL, 20000 + path_count);
					JagSrcFolderContentWo *srcFolderContent = jag_src_folder_content_wo_new();
					moo_node_wo_set_content(e_src_folder_node, (MooIContent *) srcFolderContent);
					cat_unref(srcFolderContent);
					moo_node_wo_append_child(e_module_node, e_src_folder_node);
					cat_unref(e_src_folder_node); cat_log_debug("added new child");
				}

				MooIdPath *src_folder_res_id_path = moo_name_path_to_id_path(name_path, e_module_node, modulePath);

				JagSrcFolderUpdater *src_folder_updater = jag_src_folder_updater_new(VIP_ISEQUENCE(priv->moo_service), e_root_node,
				        e_src_folder_node, src_folder_res_id_path);
				jag_src_folder_updater_run(src_folder_updater);
				cat_unref_ptr(src_folder_updater);

			} else {
				if (foundIndex >= 0) {
					moo_node_list_wo_remove(e_module_children, foundIndex);
				}
			}

			cat_unref_ptr(a_nodeName);
		}
	}
	cat_unref_ptr(modulePath);

	cat_log_debug("removing old children");
	/* remove old source folders */
	int idx;
	int ecCount = moo_node_wo_child_count((MooNodeWo *) e_module_node);
	for (idx = 0; idx < ecCount; idx++) {

		MooNodeWo *childAt = moo_node_wo_child_at((MooNodeWo *) e_module_node, idx);
		JagSrcFolderContentWo *srcFolderContent = (JagSrcFolderContentWo *) moo_node_wo_get_content(childAt,
		        jag_src_folder_content_wo_key());
		if (srcFolderContent != NULL) {
			CatInteger *it = cat_integer_new(idx);
			if (cat_hash_set_get(indexesFresh, (GObject *) it) == NULL) {
				moo_node_list_wo_remove(e_module_children, idx);
				idx--;
				ecCount--;
			}
			cat_unref_ptr(it);
		}
	}

	cat_unref_ptr(src_folder_matcher);
	cat_unref_ptr(indexesFresh);
}

static void l_run_request(WorRequest *request) {
	JagRefreshSrcFolderRequest *instance = JAG_REFRESH_SRC_FOLDER_REQUEST(request);
	JagRefreshSrcFolderRequestPrivate *priv = jag_refresh_src_folder_request_get_instance_private(instance);
	MooIdNodeMatcher *mooIdMatcher = moo_id_node_matcher_new(priv->module_node_id);

	MooTransaction *tx = moo_service_create_transaction((GObject *) request, priv->moo_service);
	while(moo_transaction_retry(tx)) {
		moo_transaction_begin(tx);
		MooNodeWo *rootNode = moo_transaction_get_tx_root_node(tx);
		int nodeIndex = moo_node_wo_find_index(rootNode, (MooINodeMatcher *) mooIdMatcher, NULL, priv->last_known_node_idx);
		cat_log_debug("nodeIndex=%d, rootNode=%d", nodeIndex, rootNode);

		if (nodeIndex >= 0) {
			priv->last_known_node_idx = nodeIndex;
			MooNodeWo *e_root_node = moo_node_wo_ensure_editable(rootNode, NULL);
			MooNodeWo *e_module_node = moo_node_wo_get_editable_child_at(e_root_node, nodeIndex);
			JagModuleContentWo *jag_content = (JagModuleContentWo *) moo_node_wo_get_content(e_module_node, jag_module_content_wo_key());
			if (jag_content == NULL) {
				cat_unref_ptr(e_root_node);
				break;
			}
			JagModuleContentWo *e_jag_content = jag_module_content_wo_ensure_editable(jag_content);
			moo_node_wo_set_content(e_module_node, (MooIContent *) e_jag_content);

			l_run((JagRefreshSrcFolderRequest *) request, e_root_node, e_module_node, e_jag_content);
			cat_unref_ptr(e_jag_content);
			gboolean tx_succes = moo_transaction_commit(tx, e_root_node);

			if (tx_succes) {
				cat_log_debug("done");

				cat_log_on_trace( {
							MooNodeWo *c_root_node = moo_transaction_get_commited_root_node(tx);
							CatStringWo *e_buf = cat_string_wo_new();
							moo_node_wo_print(c_root_node, e_buf);
							cat_log_print("DUMP", "%s", cat_string_wo_getchars(e_buf));
						});
				break;
			} else {
				cat_log_debug("failed commit");
			}
		} else {
			break;
		}
	}

	cat_unref_ptr(tx);
	cat_unref_ptr(mooIdMatcher);
}

static 	WorMergeResult l_try_merge(WorIMergeableRequest *new_request, WorIMergeableRequest *in_queue) {
	if (JAG_IS_REFRESH_SRC_FOLDER_REQUEST(in_queue)) {
		cat_log_debug("Merged !!!!!!!!!!!");
		return WOR_MERGE_KEEP_NEW;
	}
	cat_log_debug("Not Merged !!!!!!!!!!!");
	return WOR_MERGE_FAILED;
}

static void l_mergeable_request_iface_init(WorIMergeableRequestInterface *iface) {
	iface->tryMerge = l_try_merge;
}
