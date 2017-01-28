/*
   File:    jagrefreshlibrariesrequest.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 20, 2014
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

#include "jagrefreshlibrariesrequest.h"
#include "../module/jagmodulecontentwo.h"
#include "jaglibrarypathmatcher.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagRefreshLibrariesRequest"
#include <logging/catlog.h>

struct _JagRefreshLibrariesRequestPrivate {
	MooService *moo_service;
	long long module_node_id;
	int last_known_node_idx;
	CatArrayWo *e_enlisted_requests;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagRefreshLibrariesRequest, jag_refresh_libraries_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(JagRefreshLibrariesRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_refresh_libraries_request_class_init(JagRefreshLibrariesRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}

static void jag_refresh_libraries_request_init(JagRefreshLibrariesRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagRefreshLibrariesRequest *instance = JAG_REFRESH_LIBRARIES_REQUEST(object);
	JagRefreshLibrariesRequestPrivate *priv = jag_refresh_libraries_request_get_instance_private(instance);
	cat_unref_ptr(priv->e_enlisted_requests);
	cat_unref_ptr(priv->moo_service);
	G_OBJECT_CLASS(jag_refresh_libraries_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_refresh_libraries_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagRefreshLibrariesRequest *jag_refresh_libraries_request_new(MooService *moo_service, long long module_node_id, int last_known_node_idx) {
	JagRefreshLibrariesRequest *result = g_object_new(JAG_TYPE_REFRESH_LIBRARIES_REQUEST, NULL);
	cat_ref_anounce(result);
	JagRefreshLibrariesRequestPrivate *priv = jag_refresh_libraries_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->module_node_id = module_node_id;
	priv->last_known_node_idx = last_known_node_idx;
	priv->e_enlisted_requests = cat_array_wo_new();
	return result;
}



static void l_run(JagRefreshLibrariesRequest *refresh_request, MooNodeWo *e_root_node, MooNodeWo *e_module_node, JagModuleContentWo *e_jag_content) {
	JagRefreshLibrariesRequestPrivate *priv = jag_refresh_libraries_request_get_instance_private(refresh_request);
	MooNodeListWo *e_module_children = moo_node_wo_get_editable_children(e_module_node);
	MooIdPath *mainPath = moo_id_path_new();
	MooIdPath *modulePath = moo_id_path_append_id(mainPath, moo_node_wo_get_unique_id(e_module_node), priv->last_known_node_idx);
	cat_ref_sink_ptr(modulePath);

	JagLibraryPathMatcher *library_matcher = jag_library_path_matcher_new();

	CatHashSet /* <Integer> */ *indexesFresh = cat_hash_set_new((GHashFunc) cat_integer_hash, (GEqualFunc) cat_integer_equal);

	cat_log_debug("e_jag_content=%o", e_jag_content);

	int path_count = jag_module_content_wo_library_content_count(e_jag_content);
	cat_log_debug("path_count=%d", path_count);
	cat_array_wo_clear(priv->e_enlisted_requests);
	if (path_count>0) {

		for(path_count--; path_count>=0; path_count--) {
//			JagLibraryContentWo *src_folder_path = jag_module_content_wo_library_content_at(e_jag_content, path_count);
//			cat_log_debug("src_folder_path=%o", src_folder_path);
			JagLibraryContentWo *e_library_content = jag_module_content_wo_editable_library_content_at(e_jag_content, path_count);
			cat_log_debug("e_library_content=%o", e_library_content);
//			MooNamePath *name_path = jag_src_folder_path_wo_get_name_path(e_library_content);
//			cat_log_debug("name_path=%o", name_path);
//			CatStringWo *a_nodeName = moo_name_path_create_flatten(name_path);
//			cat_log_debug("nodeName=%o", nodeName);
//
//
//			gboolean did_folder_change = FALSE;
//			MooNodeWo *src_folder_res_node = moo_name_path_get_tail(name_path, e_module_node);
//			if (src_folder_res_node) {
//				int version = moo_node_wo_get_version(src_folder_res_node);
//				did_folder_change = jag_src_folder_path_wo_update_resource_node_version(e_library_content, version);
//
//			} else {
//				// TODO the src_folder_path does not exists or stopped to exist
//			}
//
//
//
//
			CatTreeNode *libray_vip_tn = jag_library_content_wo_get_vip_node(e_library_content);
			VipNode *vp = (VipNode *) cat_tree_node_get_content(libray_vip_tn);
			int foundIndex = moo_node_wo_find_index(e_module_node, (MooINodeMatcher *) library_matcher, vp, -1);
//			cat_log_debug("src_folder_res_node=%p, foundIndex=%d", src_folder_res_node, foundIndex);
//			if (src_folder_res_node!=NULL) {
				MooNodeWo *library_content_node = NULL;
				if (foundIndex>=0) {
					library_content_node = moo_node_wo_get_editable_child_at(e_module_node, foundIndex);
					CatInteger *it = cat_integer_new(foundIndex);
					cat_hash_set_add(indexesFresh, (GObject *) it);
					cat_unref_ptr(it);
				} else {
					CatInteger *it = cat_integer_new(moo_node_wo_child_count((MooNodeWo *) e_module_node));
					cat_hash_set_add(indexesFresh, (GObject *) it);
					cat_unref_ptr(it);
					long long uid = vip_isequence_next(VIP_ISEQUENCE(priv->moo_service));
					CatStringWo *a_nodeName = vip_node_get_name(vp);
					library_content_node = moo_node_wo_new(uid, cat_ref_ptr(a_nodeName), NULL, NULL, 10500);
					moo_node_wo_append_child(e_module_node, library_content_node);
					cat_unref(library_content_node);
					cat_log_debug("added new child: nodeName=%o", a_nodeName);
					MooIdPath *path = moo_id_path_new();
					path = moo_id_path_append_id(path, priv->module_node_id, priv->last_known_node_idx);
					path = moo_id_path_append_id(path, uid, moo_node_wo_child_count(e_module_node)-1);
					jag_library_content_wo_set_node_id_path(e_library_content, path);
				}

				moo_node_wo_set_content(library_content_node, (MooIContent *) e_library_content);

				cat_array_wo_append(priv->e_enlisted_requests, (GObject *) e_library_content);


//				jag_library_content_wo_apply_jar_request(e_library_content);

//
//				MooIdPath *src_folder_res_id_path = moo_name_path_to_id_path(name_path, e_module_node, modulePath);
//
//				JagSrcFolderUpdater *src_folder_updater = jag_src_folder_updater_new(VIP_ISEQUENCE(priv->moo_service), e_root_node, library_content_node, src_folder_res_id_path);
//				jag_src_folder_updater_run(src_folder_updater);
//				cat_unref_ptr(src_folder_updater);
//
//
//			} else {
//				if (foundIndex>=0) {
//					moo_node_list_wo_remove(e_module_children, foundIndex);
//				}
//			}
//
//			cat_unref_ptr(nodeName);
		}
	}

	cat_log_debug("removing old children");
	/* remove old source folders */
	int idx;
	int ecCount = moo_node_wo_child_count((MooNodeWo *) e_module_node);
	for(idx=ecCount-1; idx>=0; idx--) {
		MooNodeWo *childAt = moo_node_wo_child_at((MooNodeWo *) e_module_node, idx);
		JagLibraryContentWo *library_content = (JagLibraryContentWo *) moo_node_wo_get_content(childAt, jag_library_content_wo_key());
		if (library_content != NULL) {
			CatInteger *it = cat_integer_new(idx);
			if (cat_hash_set_get(indexesFresh, (GObject *) it)==NULL) {
				cat_log_debug("removing at %d", idx);
				moo_node_list_wo_remove(e_module_children, idx);
			}
			cat_unref_ptr(it);
		}
	}

	cat_unref_ptr(library_matcher);
	cat_unref_ptr(indexesFresh);
}







static void l_run_request(WorRequest *request) {
	JagRefreshLibrariesRequestPrivate *priv = jag_refresh_libraries_request_get_instance_private((JagRefreshLibrariesRequest *) request);
	MooIdNodeMatcher *mooIdMatcher = moo_id_node_matcher_new(priv->module_node_id);
	MooTransaction *tx = moo_service_create_transaction((GObject *) request, priv->moo_service);

	while(moo_transaction_retry(tx)) {
		moo_transaction_begin(tx);
		MooNodeWo *rootNode = moo_transaction_get_tx_root_node(tx);
		int nodeIndex = moo_node_wo_find_index(rootNode, (MooINodeMatcher *) mooIdMatcher, NULL, priv->last_known_node_idx);
		cat_log_debug("nodeIndex=%d, rootNode=%d", nodeIndex, rootNode);

		if (nodeIndex>=0) {
			priv->last_known_node_idx = nodeIndex;
			MooNodeWo *e_root_node = moo_node_wo_ensure_editable(rootNode, NULL);
			MooNodeWo *e_module_node = moo_node_wo_get_editable_child_at(e_root_node, nodeIndex);
			JagModuleContentWo *jag_content = (JagModuleContentWo *) moo_node_wo_get_content(e_module_node, jag_module_content_wo_key());
			if (jag_content==NULL) {
				cat_unref_ptr(e_root_node);
				break;
			}
			JagModuleContentWo *e_jag_content = jag_module_content_wo_ensure_editable(jag_content);
			moo_node_wo_set_content(e_module_node, (MooIContent *) e_jag_content);
			cat_unref(e_jag_content);


			l_run((JagRefreshLibrariesRequest *) request, e_root_node, e_module_node, e_jag_content);
			gboolean tx_succes = moo_transaction_commit(tx, e_root_node);

			if (tx_succes) {

				CatIIterator *iter = cat_array_wo_iterator(priv->e_enlisted_requests);
				while(cat_iiterator_has_next(iter)) {
					JagLibraryContentWo *library_content = (JagLibraryContentWo *) cat_iiterator_next(iter);
					jag_library_content_wo_apply_jar_request(library_content);
				}
				cat_unref_ptr(iter);

				cat_log_debug("done");
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



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







