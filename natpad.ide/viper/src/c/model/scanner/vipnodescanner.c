/*
   File:    vipnodescanner.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "vipnodescanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipNodeScanner"
#include <logging/catlog.h>

struct _VipNodeScannerPrivate {
	VipMapperRegistry *mapper_registry;
};

G_DEFINE_TYPE (VipNodeScanner, vip_node_scanner, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_node_scanner_class_init(VipNodeScannerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipNodeScannerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_node_scanner_init(VipNodeScanner *instance) {
	VipNodeScannerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_NODE_SCANNER, VipNodeScannerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipNodeScanner *instance = VIP_NODE_SCANNER(object);
	VipNodeScannerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->mapper_registry);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipNodeScanner *vip_node_scanner_new(VipMapperRegistry *mapper_registry) {
	VipNodeScanner *result = g_object_new(VIP_TYPE_NODE_SCANNER, NULL);
	cat_ref_anounce(result);
	VipNodeScannerPrivate *priv = result->priv;
	priv->mapper_registry = cat_ref_ptr(mapper_registry);
	return result;
}



void vip_node_scanner_scan(VipNodeScanner *scanner, CatWritableTreeNode *node, gboolean recursive_from_parent) {
	VipNodeScannerPrivate *priv = VIP_NODE_SCANNER_GET_PRIVATE(scanner);
	cat_log_debug("scan:node=%o", node);
	CatArrayWo *stack = cat_array_wo_new();	// <VipIScanWork>
	VipIScanWork *mainWork = vip_mapper_registry_create_work_for_node(priv->mapper_registry, node, recursive_from_parent, TRUE);
	cat_array_wo_append(stack, (GObject *) mainWork);

	while(cat_array_wo_size(stack)>0) {

		int index = cat_array_wo_size(stack)-1;
		VipIScanWork *workNode = (VipIScanWork *) cat_array_wo_get(stack, index);
		cat_log_trace("workNode=%o", workNode);

		VipIScanWorkInterface *workNode_iface = VIP_ISCAN_WORK_GET_INTERFACE(workNode);
		CatArrayWo *childWork = workNode_iface->initChildWork(workNode, priv->mapper_registry);


		if (childWork!=NULL) {
			cat_log_debug("workNode=%o, child_work=%o", workNode, childWork);
			cat_array_wo_append_all(stack, childWork);
			cat_unref_ptr(childWork);
		} else {
			workNode_iface->runScan(workNode);
			cat_array_wo_remove(stack, index, NULL);
		}
	}
	cat_unref_ptr(stack);
	cat_unref_ptr(mainWork);
}
