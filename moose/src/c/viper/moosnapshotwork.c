/*
   File:    moosnapshotwork.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 14, 2013
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



#include "moosnapshotwork.h"
#include "mooinodework.h"
#include "moomodulework.h"
#include "../model/module/moomodulecontentwo.h"
#include "../model/mooresourcecontentwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooSnapshotWork"
#include <logging/catlog.h>

struct _MooSnapshotWorkPrivate {
	VipISequence *moose_sequence;
	VipSnapshot *snapshot;
	MooNodeWo *editable_node;
};


static void l_node_work_iface_init(MooINodeWorkInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooSnapshotWork, moo_snapshot_work, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_WORK, l_node_work_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_snapshot_work_class_init(MooSnapshotWorkClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooSnapshotWorkPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_snapshot_work_init(MooSnapshotWork *instance) {
	MooSnapshotWorkPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_SNAPSHOT_WORK, MooSnapshotWorkPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooSnapshotWork *instance = MOO_SNAPSHOT_WORK(object);
	MooSnapshotWorkPrivate *priv = instance->priv;
	cat_unref_ptr(priv->editable_node);
	cat_unref_ptr(priv->moose_sequence);
	cat_unref_ptr(priv->snapshot);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooSnapshotWork *moo_snapshot_work_new(VipISequence *moose_sequence, VipSnapshot *snapshot, MooNodeWo *editable_node) {
	MooSnapshotWork *result = g_object_new(MOO_TYPE_SNAPSHOT_WORK, NULL);
	cat_ref_anounce(result);
	MooSnapshotWorkPrivate *priv = result->priv;
	priv->moose_sequence = cat_ref_ptr(moose_sequence);
	priv->snapshot = cat_ref_ptr(snapshot);
	priv->editable_node = cat_ref_ptr(editable_node);
	return result;
}



/********************* begin MooINodeWork implementation *********************/


//VipNodePath *moo_module_update_and_get_base_viper_path(MooModule *module, VipSnapshot *snapshot) {
//	MooModulePrivate *priv = MOO_MODULE_GET_PRIVATE(module);
//	cat_log_detail("base_path=%o", priv->vip_base_node_path);
//	VipNodePath *new_snapshot = vip_node_path_update(priv->vip_base_node_path, snapshot);
//	cat_ref_swap(priv->vip_base_node_path, new_snapshot);
//	return new_snapshot;
//}


static void l_do_work(MooINodeWork *self, CatArrayWo *e_work_list) {
	MooSnapshotWorkPrivate *priv = MOO_SNAPSHOT_WORK_GET_PRIVATE(self);
	int enode_count = moo_node_wo_child_count(priv->editable_node);
	cat_log_detail("self=%o, enode_count=%d", self, enode_count);
	if (enode_count>0) {
		int childIdx;
		for(childIdx = 0; childIdx < enode_count; childIdx++) {
			MooNodeWo *child = moo_node_wo_child_at(priv->editable_node, childIdx);
			cat_log_detail("child=%o", child);
			MooModuleContentWo *module_content = (MooModuleContentWo *) moo_node_wo_get_content(child, moo_module_content_wo_key());
			MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(child, moo_resource_content_wo_key());
			cat_log_detail("snapshot item[%d] = %o, module-content=%o, resource-content=%o", childIdx, child, module_content, resource_content);
			if ((module_content!=NULL) && (resource_content!=NULL)) {


				CatReadableTreeNode *node = moo_resource_content_wo_get_node(resource_content);
				CatReadableTreeNode *refreshed_node = (CatReadableTreeNode *) vip_snapshot_refresh(priv->snapshot, (CatTreeNode *) node);

				if (node != refreshed_node) {
					MooNodeWo *editableChild = moo_node_wo_get_editable_child_at(priv->editable_node, childIdx);
					MooModuleWork *moduleWork = moo_module_work_new(priv->moose_sequence, refreshed_node, editableChild);
					// work.forceUpdate = forceUpdate;
					cat_array_wo_append(e_work_list, (GObject *) moduleWork);
				}
				cat_unref_ptr(refreshed_node);
			}
		}
	}

}

static void l_node_work_iface_init(MooINodeWorkInterface *iface) {
	iface->doWork = l_do_work;
}

/********************* end MooINodeWork implementation *********************/
