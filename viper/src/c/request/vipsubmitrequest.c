/*
   File:    vipsubmitrequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Mar 15, 2014
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


#include "vipsubmitrequest.h"
#include "../vipservice.h"
#include "../model/vipsnapshot.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipSubmitRequest"
#include <logging/catlog.h>

struct _VipSubmitRequestPrivate {
	VipService *vip_service;
};

G_DEFINE_TYPE(VipSubmitRequest, vip_submit_request, WOR_TYPE_REQUEST);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void vip_submit_request_class_init(VipSubmitRequestClass *clazz) {
	g_type_class_add_private(clazz, sizeof(VipSubmitRequestPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void vip_submit_request_init(VipSubmitRequest *instance) {
	VipSubmitRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_SUBMIT_REQUEST, VipSubmitRequestPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipSubmitRequest *instance = VIP_SUBMIT_REQUEST(object);
	VipSubmitRequestPrivate *priv = instance->priv;
	cat_unref_ptr(priv->vip_service);
	G_OBJECT_CLASS(vip_submit_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_submit_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipSubmitRequest *vip_submit_request_new(struct _VipService *vip_service) {
	VipSubmitRequest *result = g_object_new(VIP_TYPE_SUBMIT_REQUEST, NULL);
	cat_ref_anounce(result);
	VipSubmitRequestPrivate *priv = result->priv;
	priv->vip_service = cat_ref_ptr(vip_service);
	wor_request_construct((WorRequest *) result);
	return result;
}


static void l_run_request(WorRequest *self) {
	VipSubmitRequestPrivate *priv = VIP_SUBMIT_REQUEST_GET_PRIVATE(self);

	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatReadableTreeNode *submit = cat_tree_submit(tree, FALSE);
	cat_log_debug("submit=%p", submit);
	if (submit!=NULL) {
		VipSnapshot *snapshot = vip_service_get_snapshot(priv->vip_service);
		VipSnapshot *new_snapshot = vip_snapshot_set_root(snapshot, submit);
		cat_unref_ptr(snapshot);
		vip_service_set_snapshot(priv->vip_service, new_snapshot);
		cat_unref_ptr(new_snapshot);
		cat_unref_ptr(submit);
	}
}

