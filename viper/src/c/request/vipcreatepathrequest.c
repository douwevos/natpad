/*
   File:    vipcreatepathrequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Feb 13, 2014
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

#include "vipcreatepathrequest.h"
#include "../vipisequence.h"
#include "../vipservice.h"
#include "../vippathprivate.h"
#include "../model/access/vipnodenamematcher.h"
#include "../model/file/vipfsmap.h"
#include "../model/file/vipfsfile.h"
#include "../model/vipimap.h"
#include "../model/vipiresource.h"
#include "../model/vipvirtualresource.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipCreatePathRequest"
#include <logging/catlog.h>

struct _VipCreatePathRequestPrivate {
	VipService *vip_service;
	VipPath *full_path;
	VipNodePath *node_path;
	volatile gboolean did_run;
	CatLock *lock;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipCreatePathRequest, vip_create_path_request, WOR_TYPE_REQUEST, {
		G_ADD_PRIVATE(VipCreatePathRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static VipNodePath *l_open(VipCreatePathRequest *request);

static void vip_create_path_request_class_init(VipCreatePathRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void vip_create_path_request_init(VipCreatePathRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipCreatePathRequest *instance = VIP_CREATE_PATH_REQUEST(object);
	VipCreatePathRequestPrivate *priv = vip_create_path_request_get_instance_private(instance);
	cat_unref_ptr(priv->full_path);
	cat_unref_ptr(priv->node_path);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(vip_create_path_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_create_path_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipCreatePathRequest *vip_create_path_request_new(struct _VipService *vip_service, VipPath *full_path) {
	VipCreatePathRequest *result = g_object_new(VIP_TYPE_CREATE_PATH_REQUEST, NULL);
	cat_ref_anounce(result);
	VipCreatePathRequestPrivate *priv = vip_create_path_request_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->full_path = cat_ref_ptr(full_path);
	priv->node_path = NULL;
	priv->did_run = FALSE;
	priv->lock = cat_lock_new();
	wor_request_construct((WorRequest *) result);
	return result;
}

// TODO possible dead lock ... please add timeout
VipNodePath *vip_create_path_request_wait_for_path(VipCreatePathRequest *request) {
	VipCreatePathRequestPrivate *priv = vip_create_path_request_get_instance_private(request);
	cat_lock_lock(priv->lock);
	while(!priv->did_run) {
		cat_lock_wait_timed(priv->lock, 1000);
	}
	cat_lock_unlock(priv->lock);
	return priv->node_path;
}


static CatArrayWo *l_creat_path_entries(VipCreatePathRequest *request, VipPath *full_path) {
	VipCreatePathRequestPrivate *priv = vip_create_path_request_get_instance_private(request);
	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatWritableTreeNode *root_node = cat_tree_get_writable_root_node(tree);
	CatWritableTreeNode *cdNode = root_node;

	CatArrayWo /*<CatTreeNode<?, VipNode>>*/ *e_node_list = cat_array_wo_new();
	cat_log_debug("full_path=%o", full_path);

	if (vip_path_is_canonical(full_path)) {
		full_path = cat_ref_ptr(full_path);
	} else {
		full_path = vip_path_create_canonical(full_path, vip_service_get_cd_provider(priv->vip_service));
	}
	cat_log_debug("full_path=%o", full_path);

	VipNodeNameMatcher *name_matcher = vip_node_name_matcher_new(NULL);


	CatStringWo *a_drive_descr = vip_path_get_drive_descriptor(full_path);
	cat_log_debug("drive_descr=%o", a_drive_descr);
	vip_node_name_matcher_set_match_name(name_matcher, a_drive_descr);
	cat_unref_ptr(a_drive_descr);
	int foundNode = -1;
	foundNode = cat_tree_node_find_child_index((CatTreeNode *) cdNode, (CatIMatcher *) name_matcher, -1);
	CatWritableTreeNode *childNode = NULL;
	if (foundNode>=0) {
		childNode = (CatWritableTreeNode *) cat_tree_node_child_at((CatTreeNode *) cdNode, foundNode);
	} else {
		VipPath *fs_root_path = vip_path_create_drive_path(full_path);
		cat_log_debug("fs_root_path=%o", fs_root_path);
		VipFSMap *fs_root_map = vip_fs_map_new(fs_root_path);
		cat_unref_ptr(fs_root_path);
		long long next_vip_id = vip_isequence_next((VipISequence *) priv->vip_service);
		VipNode *vip_node = vip_node_new(next_vip_id, (VipIResource*) fs_root_map);
		childNode = cat_writable_tree_node_append_child(cdNode);
		cat_writable_tree_node_set_content(childNode, (GObject *) vip_node);
		cat_unref_ptr(fs_root_map);
		cat_unref_ptr(vip_node);
	}
	cat_array_wo_append(e_node_list, (GObject *) childNode);
	cat_unref_ptr(cdNode);
	cdNode = childNode;


	int idx;
	int fp_count = vip_path_count(full_path);

	for(idx=0; idx<fp_count; idx++) {

		CatStringWo *a_entry = vip_path_get_at(full_path, idx);
		cat_log_debug("entry=%o", a_entry);
		if (a_entry==NULL || cat_string_wo_length(a_entry)==0) {
			continue;
		}

		foundNode = -1;
		if (cat_tree_node_child_count((CatTreeNode *) cdNode)>0) {
			vip_node_name_matcher_set_match_name(name_matcher, a_entry);
			foundNode = cat_tree_node_find_child_index((CatTreeNode *) cdNode, (CatIMatcher *) name_matcher, -1);
		}

		CatWritableTreeNode *childNode = NULL;
		if (foundNode>=0) {
			childNode = (CatWritableTreeNode *) cat_tree_node_child_at((CatTreeNode *) cdNode, foundNode);
			cat_log_debug("childNode=%o", childNode);
		} else {
			VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) cdNode);
			VipIResource *content = vip_node_get_content(vip_node);
			cat_log_debug("content=%o", content);
			VipIResource *childContent = NULL;
			if (VIP_IS_IMAP(content)) {
				VipIMap *map = (VipIMap *) content;
				CatArrayWo /*<VipIResource>*/ *e_enlisted = NULL;
				e_enlisted = vip_imap_enlist(map);
				if (e_enlisted!=NULL) {
					CatIIterator *iter = cat_array_wo_iterator(e_enlisted);
					while(cat_iiterator_has_next(iter)) {
						VipIResource *vipIResource = (VipIResource *) cat_iiterator_next(iter);
						CatStringWo *a_lname = vip_iresource_get_name(vipIResource);
						cat_log_debug("lname=%o, vipIResource=%o", a_lname, vipIResource);
						if (cat_string_wo_equal(a_entry, a_lname)) {
							cat_log_debug("is _child_content : vipIResource=%o", vipIResource);
							childContent = cat_ref_ptr(vipIResource);
							break;
						}
					}
					cat_unref_ptr(e_enlisted);
					cat_unref_ptr(iter);
				}
				// TODO invalid path if content is not VipVirtualResource
			}
			if (childContent == NULL) {
				if (idx==fp_count-1) {
					childContent = vip_fs_file_new(full_path);
					cat_log_error("vip_fs_file_new full_path=%o", full_path);
				} else {
					childContent = (VipIResource *) vip_virtual_resource_new(a_entry);
				}
			}

			long long next_vip_id = vip_isequence_next((VipISequence *) priv->vip_service);
			cat_log_error("childContent=%p", childContent);
			cat_log_error("childContent=%o", childContent);
			vip_node = vip_node_new(next_vip_id, childContent);
			childNode = cat_writable_tree_node_append_child(cdNode);
			cat_writable_tree_node_set_content(childNode, (GObject *) vip_node);
			cat_unref_ptr(childContent);
			cat_unref_ptr(vip_node);
		}
		cat_array_wo_append(e_node_list, (GObject *) childNode);
		cat_unref_ptr(cdNode);
		cdNode = childNode;
	}
	cat_unref_ptr(full_path);
	cat_unref_ptr(name_matcher);
	cat_unref_ptr(cdNode);
	return e_node_list;
}


VipNodePath *vip_create_path_request_for_path(VipCreatePathRequest *request, VipPath *full_path) {
	CatArrayWo *e_node_list = l_creat_path_entries(request, full_path);
	e_node_list = cat_array_wo_anchor(e_node_list, 0);
	VipNodePath *result = vip_node_path_new(e_node_list);
	cat_unref_ptr(e_node_list);
	return result;
}


static VipNodePath *l_open(VipCreatePathRequest *request) {
	VipCreatePathRequestPrivate *priv = vip_create_path_request_get_instance_private(request);
	CatTree *tree = vip_service_get_tree(priv->vip_service);

	CatArrayWo *e_node_list = l_creat_path_entries(request, priv->full_path);

	CatReadableTreeNode *readableTreeNode = cat_tree_submit(tree, FALSE);
	if (readableTreeNode==NULL) {
		readableTreeNode = cat_tree_get_readable_root_node_ref(tree);
	}

	CatArrayWo /*<CatReadableTreeNode<VipNode>>*/ *e_sl = cat_array_wo_new_size(cat_array_wo_size(e_node_list));
	CatIIterator *iter = cat_array_wo_iterator(e_node_list);
	while(cat_iiterator_has_next(iter)) {
		CatTreeNode *wn = (CatTreeNode *) cat_iiterator_next(iter);
		CatReadableTreeNode *rn = (CatReadableTreeNode *) cat_tree_node_get_node_for_location((CatTreeNode *) readableTreeNode, cat_tree_node_get_location(wn));
		cat_array_wo_append(e_sl, (GObject *) rn);
		cat_unref_ptr(rn);
	}
	cat_unref_ptr(e_node_list);
	cat_unref_ptr(iter);
	cat_unref_ptr(readableTreeNode);

	e_sl = cat_array_wo_anchor(e_sl, 0);
	VipNodePath *result = vip_node_path_new(e_sl);
	cat_unref_ptr(e_sl);
	return result;
}

static void l_run_request(WorRequest *self) {
	VipCreatePathRequest *instance = VIP_CREATE_PATH_REQUEST(self);
	VipCreatePathRequestPrivate *priv = vip_create_path_request_get_instance_private(instance);
	priv->node_path = l_open((VipCreatePathRequest *) self);
	cat_lock_lock(priv->lock);
	priv->did_run = TRUE;
	cat_lock_notify_all(priv->lock);
	cat_lock_unlock(priv->lock);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipCreatePathRequest *instance = VIP_CREATE_PATH_REQUEST(self);
	VipCreatePathRequestPrivate *priv = vip_create_path_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p, full_path=%o, node-path=%o]", iname, self, priv->full_path, priv->node_path);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
