/*
   File:    jagindexrefreshrequest.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 7, 2014
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

#include "jagindexrefreshrequest.h"
#include "jagindexsinglesource.h"
#include "../moose/jagindexmodulecontent.h"
#include "../../moose/module/jagmodulecontentwo.h"
#include "../../moose/srcfolder/jagsrcfoldercontentwo.h"
#include "../../moose/srcfile/jagsrcfilecontentwo.h"
#include "../../moose/package/jagpackagecontent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagIndexRefreshRequest"
#include <logging/catlog.h>

struct _JagIndexRefreshRequestPrivate {
	MooService *moo_service;
	WorQueue *post_queue;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagIndexRefreshRequest, jag_index_refresh_request, WOR_TYPE_REQUEST)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_index_refresh_request_class_init(JagIndexRefreshRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}

static void jag_index_refresh_request_init(JagIndexRefreshRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagIndexRefreshRequest *instance = JAG_INDEX_REFRESH_REQUEST(object);
	JagIndexRefreshRequestPrivate *priv = jag_index_refresh_request_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->post_queue);
	G_OBJECT_CLASS(jag_index_refresh_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_index_refresh_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagIndexRefreshRequest *jag_index_refresh_request_new(MooService *moo_service, WorQueue *post_queue) {
	JagIndexRefreshRequest *result = g_object_new(JAG_TYPE_INDEX_REFRESH_REQUEST, NULL);
	cat_ref_anounce(result);
	JagIndexRefreshRequestPrivate *priv = jag_index_refresh_request_get_instance_private(result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->post_queue = cat_ref_ptr(post_queue);
	wor_request_construct((WorRequest *) result);
	return result;
}


//static void l_create_new_modules(JagIndexRefreshRequestPrivate *priv, CatArrayWo *e_new_modules) {
//	MooTransaction *tx = moo_service_create_transaction("inrefss", priv->moo_service);
//	int retry_cnt = 5;
//	while(retry_cnt-->0) {
//		moo_transaction_begin(tx);
//		MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
//		MooNodeWo *e_root_node = moo_node_wo_ensure_editable(root_node, NULL);
//		CatIIterator *iter = cat_array_wo_iterator(new_modules);
//		MooIdNodeMatcher *id_matcher = moo_id_node_matcher_new(0);
//		while(cat_iiterator_has_next(iter)) {
//			MooNodeWo *module_node = (MooNodeWo *) cat_iiterator_next(iter);
//			moo_id_node_matcher_set_id(id_matcher, moo_node_wo_get_unique_id(module_node));
//			int node_idx = moo_node_wo_find_index(root_node, id_matcher, NULL, -1);
//			if (node_idx>=0) {
//				MooNodeWo *editable_child = moo_node_wo_get_editable_child_at(e_root_node, node_idx);
//				JagIndexModuleContent *jag_index_module_content = jag_index_module_content_new();
//				moo_node_wo_set_content(editable_child, (MooIContent *) jag_index_module_content);
//				cat_unref_ptr(jag_index_module_content);
//			}
//		}
//		if (moo_transaction_commit(tx, e_root_node)) {
//			CatStringWo *e_s = cat_string_wo_new();
//			moo_node_wo_print(e_root_node, s);
//			cat_log_print("dump", "%s", cat_string_wo_getchars(e_s));
//			break;
//		}
//	}
//}

//static void l_test_module_needs_refresh(JagIndexRefreshRequestPrivate *priv, MooNodeWo *module_node,  JagModuleContentWo *jag_module_content, JagIndexModuleContent *jag_index_module_content) {

//}
//

static void l_index_package(JagIndexRefreshRequestPrivate *priv, MooIdPath *jag_package_id_path, MooNodeWo *jag_package_node_wo) {
	cat_ref_sink_ptr(jag_package_id_path);
	int child_count = moo_node_wo_child_count(jag_package_node_wo);
	int child_idx;
	for(child_idx=0; child_idx<child_count; child_idx++) {
		MooNodeWo *child = moo_node_wo_child_at(jag_package_node_wo, child_idx);
		JagSrcfileContentWo *classfile_content = (JagSrcfileContentWo *) moo_node_wo_get_content(child, jag_srcfile_content_wo_key());
		if (classfile_content!=NULL) {
			MooIdPath *jag_source_id_path = moo_id_path_append_id(jag_package_id_path, moo_node_wo_get_unique_id(child), child_idx);
			JagIndexSingleSource *iss_req = jag_index_single_source_new(priv->moo_service, jag_source_id_path);
			wor_queue_post(priv->post_queue, (WorRequest *) iss_req);
			cat_unref_ptr(iss_req);
		}
	}
	cat_unref_ptr(jag_package_id_path);
}


static void l_index_source_folder(JagIndexRefreshRequestPrivate *priv, MooIdPath *jag_src_folder_id_path, MooNodeWo *jag_src_folder_node_wo) {
	cat_ref_sink_ptr(jag_src_folder_id_path);
	int child_count = moo_node_wo_child_count(jag_src_folder_node_wo);
	int child_idx;
	for(child_idx=0; child_idx<child_count; child_idx++) {
		MooNodeWo *child = moo_node_wo_child_at(jag_src_folder_node_wo, child_idx);
		cat_log_debug("check for jag-package or source: %o", child);
		JagPackageContent *package_content = (JagPackageContent *) moo_node_wo_get_content(child, jag_package_content_key());
		if (package_content!=NULL) {
			MooIdPath *jag_package_id_path = moo_id_path_append_id(jag_src_folder_id_path, moo_node_wo_get_unique_id(child), child_idx);
			l_index_package(priv, jag_package_id_path, child);
			continue;
		}

		JagSrcfileContentWo *classfile_content = (JagSrcfileContentWo *) moo_node_wo_get_content(child, jag_srcfile_content_wo_key());
		cat_log_debug("classfile_content: %o", classfile_content);
		if (classfile_content!=NULL) {
			MooIdPath *jag_source_id_path = moo_id_path_append_id(jag_src_folder_id_path, moo_node_wo_get_unique_id(child), child_idx);
			JagIndexSingleSource *iss_req = jag_index_single_source_new(priv->moo_service, jag_source_id_path);
			wor_queue_post(priv->post_queue, (WorRequest *) iss_req);
			cat_unref_ptr(iss_req);
		}
	}
	cat_unref_ptr(jag_src_folder_id_path);
}

static void l_index_single_module(JagIndexRefreshRequestPrivate *priv, int o_child_idx, MooNodeWo *module_node) {
		MooIdPath *empty_id_path = moo_id_path_new();
		cat_ref_sink_ptr(empty_id_path);
		MooIdPath *module_id_path = moo_id_path_append_id(empty_id_path, moo_node_wo_get_unique_id(module_node), o_child_idx);
		cat_ref_sink_ptr(module_id_path);
		cat_unref_ptr(empty_id_path);
		JagModuleContentWo *jag_module_content = (JagModuleContentWo *) moo_node_wo_get_content(module_node, jag_module_content_wo_key());
		if (jag_module_content != NULL) {
			int child_count = moo_node_wo_child_count(module_node);
			int child_idx;
			for(child_idx=0; child_idx<child_count; child_idx++) {
				MooNodeWo *child = moo_node_wo_child_at(module_node, child_idx);
				cat_log_debug("check for jag-src-folder: %o", child);
				JagSrcFolderContentWo *src_folder_content_wo = (JagSrcFolderContentWo *) moo_node_wo_get_content(child, jag_src_folder_content_wo_key());
				if (src_folder_content_wo==NULL) {
					continue;
				}

//				JagIndexSrcFolderContent *index_module_content = jag_src_folder_content_wo_get_index_context(src_folder_content_wo);



				MooIdPath *jag_src_folder_id_path = moo_id_path_append_id(module_id_path, moo_node_wo_get_unique_id(child), child_idx);
				l_index_source_folder(priv, jag_src_folder_id_path, child);

			}
		}
		cat_unref_ptr(module_id_path);

}

static void l_run_request(WorRequest *request) {
	JagIndexRefreshRequest *instance = JAG_INDEX_REFRESH_REQUEST(request);
	JagIndexRefreshRequestPrivate *priv = jag_index_refresh_request_get_instance_private(instance);

//	CatArrayWo *e_new_modules = cat_array_wo_new();

	MooNodeWo *root_node = moo_service_get_root_node_ref(priv->moo_service);
	CatArrayWo *e_children = moo_node_wo_enlist_children(root_node, NULL);
	if (e_children) {
		int child_idx=0;
		CatIIterator *iter = cat_array_wo_iterator(e_children);
		while(cat_iiterator_has_next(iter)) {
			MooNodeWo *child_node = (MooNodeWo *) cat_iiterator_next(iter);
			JagModuleContentWo *jag_module_content = (JagModuleContentWo *) moo_node_wo_get_content(child_node, jag_module_content_wo_key());
			if (jag_module_content == NULL) {
				child_idx++;
				continue;
			}

			JagIndexModuleContent *index_module_content = jag_module_content_wo_get_index_context(jag_module_content);
			if (jag_index_module_content_need_to_update(index_module_content, jag_module_content_wo_get_version(jag_module_content))) {

//			if (jag_index_module_content_need_to_update(index_module_content, moo_node_wo_get_version(child_node))) {
				cat_log_debug("module needs update: %o", child_node);
				l_index_single_module(priv, child_idx, child_node);
			}

//			JagIndexModuleContent *jag_index_module_content = (JagIndexModuleContent *) moo_node_wo_get_content(child_node, jag_index_module_content_key());
//			if (jag_index_module_content==NULL) {
//				cat_array_wo_append(e_new_modules, child_node);
//			} else {
//				l_test_module_needs_refresh(priv, child_node, jag_module_content, jag_index_module_content);
//			}
			child_idx++;
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(e_children);
	}
	cat_unref_ptr(root_node);

//
//	if (cat_array_wo_size(new_modules)>0) {
//		l_create_new_modules(priv, new_modules);
//	} else {
//
//	}
//	cat_unref_ptr(new_modules);
}
