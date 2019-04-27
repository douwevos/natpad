/*
   File:    vipwritefilerequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 13, 2014
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

#include "vipwritefilerequest.h"
#include "../vipservice.h"
#include "../model/vipnode.h"
#include "../model/vipifile.h"
#include "../model/vipimap.h"
#include "../model/file/vipfsmap.h"
#include "../model/vipvirtualresource.h"
#include "../model/scanner/vipnodescanner.h"
#include "../model/access/vipnodeidmatcher.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipWriteFileRequest"
#include <logging/catlog.h>

struct _VipWriteFileRequestPrivate {
	VipService *vip_service;
	CatTreeNode *tree_node;
	gboolean did_run;
	CatLock *lock;

	CatStringWo *e_data;
};

G_DEFINE_TYPE_WITH_PRIVATE(VipWriteFileRequest, vip_write_file_request, WOR_TYPE_REQUEST);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);
static void l_write(VipWriteFileRequest *request, CatIOutputStream *output_stream);


static void vip_write_file_request_class_init(VipWriteFileRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;

	clazz->write = l_write;
}

static void vip_write_file_request_init(VipWriteFileRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipWriteFileRequest *instance = VIP_WRITE_FILE_REQUEST(object);
	VipWriteFileRequestPrivate *priv = vip_write_file_request_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->tree_node);
	G_OBJECT_CLASS(vip_write_file_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_write_file_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipWriteFileRequest *vip_write_file_request_new_simple(struct _VipService *vip_service, CatTreeNode *tree_node, CatStringWo *e_data) {
	VipWriteFileRequest *result = g_object_new(VIP_TYPE_WRITE_FILE_REQUEST, NULL);
	cat_ref_anounce(result);
	vip_write_file_request_construct(result, vip_service, tree_node);
	VipWriteFileRequestPrivate *priv = vip_write_file_request_get_instance_private(result);
	priv->e_data = cat_ref_ptr(e_data);
	return result;
}

void vip_write_file_request_construct(VipWriteFileRequest *request, struct _VipService *vip_service, CatTreeNode *tree_node) {
	VipWriteFileRequestPrivate *priv = vip_write_file_request_get_instance_private(request);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->tree_node = cat_ref_ptr(tree_node);
	priv->lock = cat_lock_new();
	priv->did_run = FALSE;
	priv->e_data = NULL;
	wor_request_construct((WorRequest *) request);
}


// TODO possible dead lock ... please add timeout
void vip_write_file_request_wait_to_finish(VipWriteFileRequest *request) {
	VipWriteFileRequestPrivate *priv = vip_write_file_request_get_instance_private(request);
	cat_lock_lock(priv->lock);
	while(!priv->did_run) {
		cat_lock_wait(priv->lock);
	}
	cat_lock_unlock(priv->lock);
}


static void l_run_request(WorRequest *self) {
	VipWriteFileRequest *instance = VIP_WRITE_FILE_REQUEST(self);
	VipWriteFileRequestPrivate *priv = vip_write_file_request_get_instance_private(instance);

	CatTreeNode *parent = cat_tree_node_get_parent_node(priv->tree_node);
	// TODO what if parent is null
	VipNode *parent_node = (VipNode *) cat_tree_node_get_content(parent);
	VipIResource *parent_resource = vip_node_get_content(parent_node);
	if (VIP_IS_VIRTUAL_RESOURCE(parent_resource)) {
		cat_log_error("Trying to create a file in a non existing map");
		return;
	}

	VipNode *main_node = (VipNode *) cat_tree_node_get_content(priv->tree_node);
	VipIResource *main_resource = vip_node_get_content(main_node);
	if (VIP_IS_VIRTUAL_RESOURCE(main_resource)) {
		VipIMap *map = (VipIMap *) parent_resource;
		VipIFile *new_file = vip_imap_create_file(map, vip_iresource_get_name(main_resource));
		if (new_file) {
			main_resource = VIP_IRESOURCE(new_file);
			// TODO main_node should get this new file
		} else {
			cat_log_error("Cannot create file:%o in map %o", vip_iresource_get_name(main_resource), map);
		}
	}


	if (VIP_IS_IFILE(main_resource)) {
		VipIFile *vip_file = (VipIFile *) main_resource;
		CatIOutputStream *out = vip_ifile_open_output_stream(vip_file);
		if (out) {

			VipWriteFileRequestClass *wf_class = VIP_WRITE_FILE_REQUEST_GET_CLASS(self);
			wf_class->write(VIP_WRITE_FILE_REQUEST(self), out);
			cat_ioutput_stream_close(out);
		}
	} else {
		cat_log_error("Cannot write to a non-file:%o", main_resource);
	}



	cat_log_debug("main_node=%o",main_node);

	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(tree);
	CatITreeEntryListProvider *new_list_prov = cat_tree_node_get_list_provider((CatTreeNode *) writable_root_node);
	cat_log_debug("writable_root_node=%o, new_list_prov=%o",writable_root_node, new_list_prov);


	VipNodeIdMatcher *node_id_matcher = vip_node_id_matcher_new(vip_node_get_id(main_node));
	CatTreeNode *refreshed_node = cat_tree_node_refresh((CatTreeNode *) priv->tree_node, new_list_prov, (CatIMatcher *) node_id_matcher);
	cat_log_debug("refreshed_node=%o",refreshed_node);
	cat_unref_ptr(node_id_matcher);
	if (refreshed_node) {
		CatWritableTreeNode *wnode = cat_writable_tree_node_new(new_list_prov, cat_tree_node_get_location(refreshed_node));
		CatWritableTreeNode *pnode = (CatWritableTreeNode *) cat_tree_node_get_parent_node((CatTreeNode *) wnode);
		VipNode *vp = (VipNode *) cat_tree_node_get_content((CatTreeNode *) pnode);
		if (vp) {
			VipIResource *ppp = vip_node_get_content(vp);
			if (VIP_IS_FS_MAP(ppp)) {
				vip_fs_map_clear_cache((VipFSMap *) ppp);
			}
		}
		cat_log_debug("wnode=%o",wnode);
		VipNodeScanner *nodeScanner = vip_node_scanner_new(vip_service_get_mapper_registry(priv->vip_service));
		vip_node_scanner_scan(nodeScanner, pnode, TRUE);
		cat_unref_ptr(nodeScanner);
		cat_unref_ptr(pnode);
	}

	vip_service_submit(priv->vip_service, FALSE);

//	if (VIP_IS_VIRTUAL_RESOURCE(resource)) {
//
//		vip_node_path_get_tail(priv->node_path);
//
//	}
//
//	CatTree *tree = vip_service_get_tree(priv->vip_serivce);
//	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(tree);
//	CatITreeEntryListProvider *new_list_prov = cat_tree_node_get_list_provider((CatTreeNode *) writable_root_node);
//
//	VipNode *old_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node_to_rename);
//	VipNodeIdMatcher *node_id_matcher = vip_node_id_matcher_new(vip_node_get_id(old_vip_node));
//	CatTreeNode *refreshed_node = cat_tree_node_refresh((CatTreeNode *) priv->node_to_rename, new_list_prov, (CatIMatcher *) node_id_matcher);
//	cat_log_debug("refreshed_node=%p, priv->node_to_rename=%p", refreshed_node, priv->node_to_rename);
//	cat_unref_ptr(node_id_matcher);
//	if (refreshed_node) {
//		VipIResource *resource = vip_node_get_content(old_vip_node);
//		VipIResource *renamed_resource = vip_iresource_rename(resource, priv->new_name);
//		cat_log_debug("renamed_resource=%o", renamed_resource);
//		if (renamed_resource) {
//			CatWritableTreeNode *wnode = cat_writable_tree_node_new(new_list_prov, cat_tree_node_get_location(refreshed_node));
//			VipNode *new_vip_node = vip_node_set_resource(old_vip_node, renamed_resource);
//			cat_unref(renamed_resource);
//			cat_writable_tree_node_set_content(wnode, (GObject *) new_vip_node);
//			if (cat_tree_node_child_count((CatTreeNode *) wnode)>0) {
//				VipNodeScanner *nodeScanner = vip_node_scanner_new(vip_service_get_mapper_registry(priv->vip_service));
//				vip_node_scanner_scan(nodeScanner, wnode, TRUE);
//				cat_unref_ptr(nodeScanner);
//			}
//			cat_unref_ptr(new_vip_node);
//			priv->did_rename = TRUE;
//		}
//	}
//	cat_unref_ptr(refreshed_node);
//	cat_unref_ptr(writable_root_node);
//
//	/* schedule a submit */
//	vip_service_submit(priv->vip_service, FALSE);
//
//

	cat_lock_lock(priv->lock);
	priv->did_run = TRUE;
	cat_lock_notify_all(priv->lock);
	cat_lock_unlock(priv->lock);
}

static void l_write(VipWriteFileRequest *request, CatIOutputStream *output_stream) {
	VipWriteFileRequestPrivate *priv = vip_write_file_request_get_instance_private(request);
	if (priv->e_data) {
		cat_ioutput_stream_write_length(output_stream, cat_string_wo_getchars(priv->e_data), cat_string_wo_length(priv->e_data));
	}

}
