/*
   File:    vipdeletenoderequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 19, 2014
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

#include "vipdeletenoderequest.h"
#include "../vipservice.h"
//#include "../model/scanner/vipnodescanner.h"
#include "../model/access/vipnodeidmatcher.h"
#include "../model/vipnode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipDeleteNodeRequest"
#include <logging/catlog.h>

struct _VipDeleteNodeRequestPrivate {
	VipService *vip_service;
	CatReadableTreeNode *node_to_delete;
	gboolean did_delete;
	gboolean did_run;
	CatLock *lock;
};

G_DEFINE_TYPE(VipDeleteNodeRequest, vip_delete_node_request, WOR_TYPE_REQUEST);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void vip_delete_node_request_class_init(VipDeleteNodeRequestClass *clazz) {
	g_type_class_add_private(clazz, sizeof(VipDeleteNodeRequestPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void vip_delete_node_request_init(VipDeleteNodeRequest *instance) {
	VipDeleteNodeRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_DELETE_NODE_REQUEST, VipDeleteNodeRequestPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipDeleteNodeRequest *instance = VIP_DELETE_NODE_REQUEST(object);
	VipDeleteNodeRequestPrivate *priv = instance->priv;
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->node_to_delete);
	cat_unref_ptr(priv->vip_service);
	G_OBJECT_CLASS(vip_delete_node_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_delete_node_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipDeleteNodeRequest *vip_delete_node_request_new(struct _VipService *vip_service, CatReadableTreeNode *node_to_delete) {
	VipDeleteNodeRequest *result = g_object_new(VIP_TYPE_DELETE_NODE_REQUEST, NULL);
	cat_ref_anounce(result);
	VipDeleteNodeRequestPrivate *priv = result->priv;
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->node_to_delete = cat_ref_ptr(node_to_delete);
	priv->did_delete = FALSE;
	priv->did_run = FALSE;
	priv->lock = cat_lock_new();
	wor_request_construct((WorRequest *) result);
	return result;
}


// TODO possible dead lock ... please add timeout
gboolean vip_delete_node_request_wait_for_result(VipDeleteNodeRequest *request) {
	VipDeleteNodeRequestPrivate *priv = VIP_DELETE_NODE_REQUEST_GET_PRIVATE(request);
	cat_lock_lock(priv->lock);
	while(!priv->did_run) {
		cat_lock_wait(priv->lock);
	}
	cat_lock_unlock(priv->lock);
	return priv->did_delete;
}


static void l_run_request(WorRequest *self) {
	VipDeleteNodeRequestPrivate *priv = VIP_DELETE_NODE_REQUEST_GET_PRIVATE(self);

	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(tree);
	CatITreeEntryListProvider *new_list_prov = cat_tree_node_get_list_provider((CatTreeNode *) writable_root_node);

	VipNode *old_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node_to_delete);
	VipNodeIdMatcher *node_id_matcher = vip_node_id_matcher_new(vip_node_get_id(old_vip_node));
	CatTreeNode *refreshed_node = cat_tree_node_refresh((CatTreeNode *) priv->node_to_delete, new_list_prov, (CatIMatcher *) node_id_matcher);
	cat_log_debug("refreshed_node=%p, priv->node_to_delete=%p", refreshed_node, priv->node_to_delete);
	cat_unref_ptr(node_id_matcher);
	if (refreshed_node) {
		VipIResource *resource = vip_node_get_content(old_vip_node);
		gboolean was_deleted = vip_iresource_delete(resource);
		cat_log_debug("was_deleted=%d", was_deleted);
		if (was_deleted) {
			CatWritableTreeNode *wnode = cat_writable_tree_node_new(new_list_prov, cat_tree_node_get_location(refreshed_node));
			CatWritableTreeNode *pnode = (CatWritableTreeNode *) cat_tree_node_get_parent_node((CatTreeNode *) wnode);
			cat_writable_tree_node_remove_child(pnode, wnode);
			cat_unref_ptr(wnode);
			cat_unref_ptr(pnode);

			priv->did_delete = TRUE;
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

