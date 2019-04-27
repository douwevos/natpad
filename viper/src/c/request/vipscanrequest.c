/*
   File:    vipscanrequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Feb 8, 2014
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

#include "vipscanrequest.h"
#include "../model/scanner/vipnodescanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipScanRequest"
#include <logging/catlog.h>

struct _VipScanRequestPrivate {
	VipService *vip_service;
};

G_DEFINE_TYPE_WITH_PRIVATE(VipScanRequest, vip_scan_request, WOR_TYPE_REQUEST);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void vip_scan_request_class_init(VipScanRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void vip_scan_request_init(VipScanRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipScanRequest *instance = VIP_SCAN_REQUEST(object);
	VipScanRequestPrivate *priv = vip_scan_request_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	G_OBJECT_CLASS(vip_scan_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_scan_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

VipScanRequest *vip_scan_request_new(VipService *vip_service) {
	VipScanRequest *result = g_object_new(VIP_TYPE_SCAN_REQUEST, NULL);
	cat_ref_anounce(result);
	VipScanRequestPrivate *priv = vip_scan_request_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	wor_request_construct((WorRequest *) result);
	return result;
}


static void l_run_request(WorRequest *self) {
	VipScanRequest *instance = VIP_SCAN_REQUEST(self);
	VipScanRequestPrivate *priv = vip_scan_request_get_instance_private(instance);
	VipSnapshot *snapshot = vip_service_get_snapshot(priv->vip_service);
	VipNodeScanner *nodeScanner = vip_node_scanner_new(vip_service_get_mapper_registry(priv->vip_service));

	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(tree);
	vip_node_scanner_scan(nodeScanner, writable_root_node, FALSE);
	CatReadableTreeNode *submit = cat_tree_submit(tree, FALSE);
	if (submit==NULL) {
		submit = cat_tree_get_readable_root_node_ref(tree);
	}
	VipSnapshot *new_snapshot = vip_snapshot_set_root(snapshot, submit);
	cat_unref_ptr(submit);
	cat_unref_ptr(snapshot);
	vip_service_set_snapshot(priv->vip_service, new_snapshot);
	cat_unref_ptr(writable_root_node);
	cat_unref_ptr(new_snapshot);
	cat_unref_ptr(nodeScanner);
//	cat_unref_ptr(submit);
}
