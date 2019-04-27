/*
   File:    viprefreshnoderequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Mar 14, 2014
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

#include "viprefreshnoderequest.h"
#include "../vipservice.h"
#include "../model/scanner/vipnodescanner.h"
#include "../model/access/vipnodeidmatcher.h"
#include "../model/vipnode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipRefreshNodeRequest"
#include <logging/catlog.h>

struct _VipRefreshNodeRequestPrivate {
	VipService *vip_service;
	CatReadableTreeNode *node_to_refresh;
};

G_DEFINE_TYPE_WITH_PRIVATE(VipRefreshNodeRequest, vip_refresh_node_request, WOR_TYPE_REQUEST); // @suppress("Unused static function")

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void vip_refresh_node_request_class_init(VipRefreshNodeRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void vip_refresh_node_request_init(VipRefreshNodeRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipRefreshNodeRequest *instance = VIP_REFRESH_NODE_REQUEST(object);
	VipRefreshNodeRequestPrivate *priv = vip_refresh_node_request_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->node_to_refresh);
	G_OBJECT_CLASS(vip_refresh_node_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_refresh_node_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipRefreshNodeRequest *vip_refresh_node_request_new(struct _VipService *vip_service, CatReadableTreeNode *node_to_refresh) {
	VipRefreshNodeRequest *result = g_object_new(VIP_TYPE_REFRESH_NODE_REQUEST, NULL);
	cat_ref_anounce(result);
	VipRefreshNodeRequestPrivate *priv = vip_refresh_node_request_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->node_to_refresh = cat_ref_ptr(node_to_refresh);
	wor_request_construct((WorRequest *) result);
	return result;
}

static void l_run_request(WorRequest *self) {
	VipRefreshNodeRequest *instance = VIP_REFRESH_NODE_REQUEST(self);
	VipRefreshNodeRequestPrivate *priv = vip_refresh_node_request_get_instance_private(instance);

	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(tree);
	CatITreeEntryListProvider *new_list_prov = cat_tree_node_get_list_provider((CatTreeNode *) writable_root_node);

	VipNode *old_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node_to_refresh);
	VipNodeIdMatcher *node_id_matcher = vip_node_id_matcher_new(vip_node_get_id(old_vip_node));
	CatTreeNode *refreshed_node = cat_tree_node_refresh((CatTreeNode *) priv->node_to_refresh, new_list_prov, (CatIMatcher *) node_id_matcher);
	cat_log_debug("refreshed_node=%p, priv->node_to_refresh=%p", refreshed_node, priv->node_to_refresh);
	cat_unref_ptr(node_id_matcher);
	if (refreshed_node) {
		CatWritableTreeNode *wnode = cat_writable_tree_node_new(new_list_prov, cat_tree_node_get_location(refreshed_node));
		VipNodeScanner *nodeScanner = vip_node_scanner_new(vip_service_get_mapper_registry(priv->vip_service));
		vip_node_scanner_scan(nodeScanner, wnode, TRUE);
		cat_unref_ptr(nodeScanner);
		cat_unref_ptr(wnode);
	}
	cat_unref_ptr(refreshed_node);
	cat_unref_ptr(writable_root_node);

	/* schedule a submit */
	vip_service_submit(priv->vip_service, FALSE);
}

