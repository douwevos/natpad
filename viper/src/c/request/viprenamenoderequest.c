/*
   File:    viprenamenoderequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 5, 2014
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

#include "viprenamenoderequest.h"
#include "../vipservice.h"
#include "../model/scanner/vipnodescanner.h"
#include "../model/access/vipnodeidmatcher.h"
#include "../model/vipnode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipRenameNodeRequest"
#include <logging/catlog.h>

struct _VipRenameNodeRequestPrivate {
	VipService *vip_service;
	CatReadableTreeNode *node_to_rename;
	CatStringWo *a_new_name;
	gboolean did_rename;
	gboolean did_run;
	CatLock *lock;
};

G_DEFINE_TYPE_WITH_PRIVATE(VipRenameNodeRequest, vip_rename_node_request, WOR_TYPE_REQUEST);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void vip_rename_node_request_class_init(VipRenameNodeRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void vip_rename_node_request_init(VipRenameNodeRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipRenameNodeRequest *instance = VIP_RENAME_NODE_REQUEST(object);
	VipRenameNodeRequestPrivate *priv = vip_rename_node_request_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->node_to_rename);
	cat_unref_ptr(priv->a_new_name);
	G_OBJECT_CLASS(vip_rename_node_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_rename_node_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipRenameNodeRequest *vip_rename_node_request_new(struct _VipService *vip_service, CatReadableTreeNode *node_to_rename, CatStringWo *a_new_name) {
	VipRenameNodeRequest *result = g_object_new(VIP_TYPE_RENAME_NODE_REQUEST, NULL);
	cat_ref_anounce(result);
	VipRenameNodeRequestPrivate *priv = vip_rename_node_request_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->node_to_rename = cat_ref_ptr(node_to_rename);
	priv->a_new_name = cat_ref_ptr(a_new_name);
	priv->did_rename = FALSE;
	priv->did_run = FALSE;
	priv->lock = cat_lock_new();
	wor_request_construct((WorRequest *) result);
	return result;
}

// TODO possible dead lock ... please add timeout
gboolean vip_rename_node_request_wait_for_result(VipRenameNodeRequest *request) {
	VipRenameNodeRequestPrivate *priv = vip_rename_node_request_get_instance_private(request);
	cat_lock_lock(priv->lock);
	while(!priv->did_run) {
		cat_lock_wait(priv->lock);
	}
	cat_lock_unlock(priv->lock);
	return priv->did_rename;
}

static void l_run_request(WorRequest *self) {
	VipRenameNodeRequest *instance = VIP_RENAME_NODE_REQUEST(self);
	VipRenameNodeRequestPrivate *priv = vip_rename_node_request_get_instance_private(instance);

	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(tree);
	CatITreeEntryListProvider *new_list_prov = cat_tree_node_get_list_provider((CatTreeNode *) writable_root_node);

	VipNode *old_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node_to_rename);
	VipNodeIdMatcher *node_id_matcher = vip_node_id_matcher_new(vip_node_get_id(old_vip_node));
	CatTreeNode *refreshed_node = cat_tree_node_refresh((CatTreeNode *) priv->node_to_rename, new_list_prov, (CatIMatcher *) node_id_matcher);
	cat_log_debug("refreshed_node=%p, priv->node_to_rename=%p", refreshed_node, priv->node_to_rename);
	cat_unref_ptr(node_id_matcher);
	if (refreshed_node) {
		VipIResource *resource = vip_node_get_content(old_vip_node);
		VipIResource *renamed_resource = vip_iresource_rename(resource, priv->a_new_name);
		cat_log_debug("renamed_resource=%o", renamed_resource);
		if (renamed_resource) {
			CatWritableTreeNode *wnode = cat_writable_tree_node_new(new_list_prov, cat_tree_node_get_location(refreshed_node));
			VipNode *new_vip_node = vip_node_set_resource(old_vip_node, renamed_resource);
			cat_unref(renamed_resource);
			cat_writable_tree_node_set_content(wnode, (GObject *) new_vip_node);
			if (cat_tree_node_child_count((CatTreeNode *) wnode)>0) {
				VipNodeScanner *nodeScanner = vip_node_scanner_new(vip_service_get_mapper_registry(priv->vip_service));
				vip_node_scanner_scan(nodeScanner, wnode, TRUE);
				cat_unref_ptr(nodeScanner);
			}
			cat_unref_ptr(new_vip_node);
			priv->did_rename = TRUE;
		}
	}
	cat_unref_ptr(refreshed_node);
	cat_unref_ptr(writable_root_node);

	/* schedule a submit */
	vip_service_submit(priv->vip_service, FALSE);


	cat_lock_lock(priv->lock);
	priv->did_run = TRUE;
	cat_lock_notify_all(priv->lock);
	cat_lock_unlock(priv->lock);
}
