/*
   File:    mooaddmodule.c
   Project: moose
   Author:  Douwe Vos
   Date:    Feb 14, 2014
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


#include "mooaddmodule.h"
#include "../model/moocontentmapwo.h"
#include "../model/mooresourcecontentwo.h"
#include "../model/mooiservicescontent.h"
#include "../model/module/moomodulecontentwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooAddModule"
#include <logging/catlog.h>

struct _MooAddModulePrivate {
	MooService *moo_service;
	VipNodePath *node_path;
	MooNodeWo *module;
	CatLock *lock;
	gboolean is_done;
};

G_DEFINE_TYPE(MooAddModule, moo_add_module, WOR_TYPE_REQUEST);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);


static void moo_add_module_class_init(MooAddModuleClass *clazz) {
	g_type_class_add_private(clazz, sizeof(MooAddModulePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void moo_add_module_init(MooAddModule *instance) {
	MooAddModulePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_ADD_MODULE, MooAddModulePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooAddModule *instance = MOO_ADD_MODULE(object);
	MooAddModulePrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->node_path);
	cat_unref_ptr(priv->module);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(moo_add_module_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_add_module_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooAddModule *moo_add_module_new(MooService *moo_service, VipNodePath *node_path) {
	MooAddModule *result = g_object_new(MOO_TYPE_ADD_MODULE, NULL);
	cat_ref_anounce(result);
	wor_request_construct((WorRequest *) result);
	MooAddModulePrivate *priv = result->priv;
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->node_path = cat_ref_ptr(node_path);
	priv->lock = cat_lock_new();
	priv->is_done = FALSE;
	priv->module = NULL;
	return result;
}

void moo_add_module_wait_for_done(MooAddModule *add_module) {
	MooAddModulePrivate *priv = MOO_ADD_MODULE_GET_PRIVATE(add_module);
	cat_lock_lock(priv->lock);
	while(!priv->is_done) {
		cat_lock_wait(priv->lock);
	}
	cat_lock_unlock(priv->lock);
}

void moo_add_module_from_path(MooService *moo_service, VipPath *path) {
	VipService *vip_service = moo_service_get_viper_service(moo_service);
	VipNodePath *result = NULL;
	VipCreatePathRequest *create_path_request = vip_service_create_path(vip_service, path);
	result = vip_create_path_request_wait_for_path(create_path_request);
	if (result!=NULL) {
		VipNode *tail = vip_node_path_get_tail(result);

		cat_log_debug("request=%o", create_path_request);

		vip_node_recursive_up(tail);
		vip_service_scan(vip_service);

		MooAddModule *add_module = moo_add_module_new(moo_service, result);
		VipService *vip_service = moo_service_get_viper_service(moo_service);
		vip_service_post(vip_service, (WorRequest *) add_module);
		cat_unref_ptr(add_module);

	}
	cat_unref_ptr(create_path_request);

}


static void l_run_request(WorRequest *request) {
	MooAddModulePrivate *priv = MOO_ADD_MODULE_GET_PRIVATE(request);
	if (priv->module==NULL) {

		VipNode *tail = vip_node_path_get_tail(priv->node_path);
		CatTreeNode *tail_node = vip_node_path_get_tail_node(priv->node_path);

		cat_log_debug("pre-refresh:tailnode=%o, list-provider=%o", tail_node, cat_itree_entry_list_provider_get_entry_list(cat_readable_tree_node_get_list_provider((CatReadableTreeNode *) tail_node)));

		VipService *vip_service = moo_service_get_viper_service(priv->moo_service);
		VipSnapshot *snapshot = vip_service_get_snapshot(vip_service);

		tail_node = vip_snapshot_refresh(snapshot, tail_node);
		cat_unref_ptr(snapshot);

		cat_log_debug("post-refresh:tailnode=%o, list-provider=%o", tail_node, cat_itree_entry_list_provider_get_entry_list(cat_readable_tree_node_get_list_provider((CatReadableTreeNode *) tail_node)));

		MooContentMapWo *editable_map = moo_content_map_wo_new();
		MooResourceContentWo *resourceContent = moo_resource_content_wo_new((CatReadableTreeNode *) tail_node, NULL);
		moo_content_map_wo_set(editable_map, (MooIContent *) resourceContent);
		moo_content_map_wo_create_link(editable_map, moo_iservices_content_key(), moo_resource_content_wo_key());
		cat_unref_ptr(resourceContent);
		MooModuleContentWo *moduleContent = moo_module_content_wo_new((CatReadableTreeNode *) tail_node);
		moo_content_map_wo_set(editable_map, (MooIContent *) moduleContent);
		cat_unref_ptr(moduleContent);

		long long next_moo_id = vip_isequence_next((VipISequence *) priv->moo_service);

		CatStringWo *tail_name = vip_node_get_name(tail);
		priv->module = moo_node_wo_new(next_moo_id, cat_ref_ptr(tail_name), NULL, editable_map, 50);
		WorService *worm_service = moo_service_get_worm_service(priv->moo_service);
		wor_service_post_request(worm_service, (WorRequest *) request);

		cat_unref_ptr(editable_map);
		cat_unref_ptr(tail_node);

	} else {
		moo_service_add_node(priv->moo_service, priv->module);
		cat_lock_lock(priv->lock);
		cat_lock_notify_all(priv->lock);
		cat_lock_unlock(priv->lock);
	}
}


