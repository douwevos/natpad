/*
   File:    moomodulework.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 21, 2013
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

#include "moomodulework.h"
#include "mooinodework.h"
#include "../model/mooresourcecontentwo.h"
#include "../model/mooiservicescontent.h"
#include "../model/moocontentmapwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooModuleWork"
#include <logging/catlog.h>

struct _MooModuleWorkPrivate {
	VipISequence *moose_sequence;
	CatReadableTreeNode *new_node;
	MooNodeWo *editableNode;
};

static void l_node_work_iface_init(MooINodeWorkInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooModuleWork, moo_module_work, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_WORK, l_node_work_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_module_work_class_init(MooModuleWorkClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooModuleWorkPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_module_work_init(MooModuleWork *instance) {
	MooModuleWorkPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_MODULE_WORK, MooModuleWorkPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooModuleWork *instance = MOO_MODULE_WORK(object);
	MooModuleWorkPrivate *priv = instance->priv;
	cat_unref_ptr(priv->moose_sequence);
	cat_unref_ptr(priv->new_node);
	cat_unref_ptr(priv->editableNode);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooModuleWork *moo_module_work_new(VipISequence *moose_sequence, CatReadableTreeNode *new_node, MooNodeWo *editableNode) {
	MooModuleWork *result = g_object_new(MOO_TYPE_MODULE_WORK, NULL);
	cat_ref_anounce(result);
	MooModuleWorkPrivate *priv = result->priv;
	priv->moose_sequence = cat_ref_ptr(moose_sequence);
	priv->new_node = cat_ref_ptr(new_node);
	priv->editableNode = cat_ref_ptr(editableNode);
	if (moo_node_wo_is_fixed(editableNode)) {
		cat_log_fatal("***********:editableNode=%o", editableNode);
	}

	MooResourceContentWo *e_resource_content = (MooResourceContentWo *)  moo_node_wo_get_content(editableNode, moo_resource_content_wo_key());
	if (e_resource_content) {
		if (moo_resource_content_wo_is_fixed(e_resource_content)) {

			MooResourceContentWo *parent_resource_content = NULL;
			MooNodeWo *parent_moo_node = moo_node_wo_get_parent_node_ref(editableNode);
			if (parent_moo_node) {
				parent_resource_content = (MooResourceContentWo *)  moo_node_wo_get_content(parent_moo_node, moo_resource_content_wo_key());
				cat_unref_ptr(parent_moo_node);
			}

			e_resource_content = moo_resource_content_wo_ensure_editable(e_resource_content, parent_resource_content);
			moo_node_wo_set_content(editableNode, (MooIContent *) e_resource_content);
			cat_unref(e_resource_content);
		}
		moo_resource_content_wo_set_node(e_resource_content, new_node);
	}
	return result;
}


/********************* begin MooINodeWork implementation *********************/

static void l_do_work(MooINodeWork *self, CatArrayWo /*<MooINodeWork>*/ *e_work_list) {
	MooModuleWorkPrivate *priv = MOO_MODULE_WORK_GET_PRIVATE(self);


	MooResourceContentWo *parent_resource_content = (MooResourceContentWo *)  moo_node_wo_get_content(priv->editableNode, moo_resource_content_wo_key());


	CatHashSet/*<Long>*/ *vipNodesAdded = cat_hash_set_new((GHashFunc) cat_long_hash, (GEqualFunc) cat_long_equal);

	MooNodeListWo *eChildren = moo_node_wo_get_editable_children(priv->editableNode);

	int en_count = moo_node_list_wo_count(eChildren);
	cat_log_trace("en_count=%d", en_count);
	if (en_count>0) {
		gboolean forceUpdate = FALSE;
		int childIdx;
		for(childIdx=0; childIdx<en_count; childIdx++) {
			MooNodeWo *child = moo_node_list_wo_get_at(eChildren, childIdx);
			MooResourceContentWo *resource_content = (MooResourceContentWo *)  moo_node_wo_get_content(child, moo_resource_content_wo_key());

			if (resource_content!=NULL) {
//				cat_log_debug("[%d] testing resource: child=%o", childIdx, child);
				CatReadableTreeNode *old_node = moo_resource_content_wo_get_node(resource_content);
				VipNode *old_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) old_node);

				VipNodeIdMatcher *id_matcher = vip_node_id_matcher_new(vip_node_get_id(old_vip_node));
				int foundIndex = cat_tree_node_find_child_index((CatTreeNode *) priv->new_node, (CatIMatcher *) id_matcher, -1);
				cat_unref_ptr(id_matcher);
				if (foundIndex>=0) {
					CatReadableTreeNode *child_node = (CatReadableTreeNode *) cat_tree_node_child_at((CatTreeNode *)priv->new_node, foundIndex);
					VipNode *newVipChildNode = (VipNode *) cat_tree_node_get_content((CatTreeNode *) child_node);
					if (child_node != old_node || forceUpdate) {
						MooNodeWo *editableChild = moo_node_wo_get_editable_child_at(priv->editableNode, childIdx);
						moo_node_wo_set_name(editableChild, vip_node_get_name(newVipChildNode));
						MooModuleWork *moduleWork = moo_module_work_new(priv->moose_sequence, child_node, editableChild);
						cat_array_wo_append(e_work_list, (GObject *) moduleWork);
						cat_unref_ptr(moduleWork);
					}
					CatLong *vip_id_added = cat_long_new(vip_node_get_id(newVipChildNode));
					cat_hash_set_add(vipNodesAdded, (GObject *) vip_id_added);
					cat_unref_ptr(vip_id_added);
					cat_unref_ptr(child_node);

				} else {
					cat_log_debug("deleting: old_vip_node=%o, child=%o", old_vip_node, child);
					moo_node_list_wo_remove(eChildren, childIdx);
					childIdx--;
					en_count--;
				}
			}
		}
	}

	cat_log_trace("new_node=%o", priv->new_node);
	int newChildCount = cat_tree_node_child_count((CatTreeNode *) priv->new_node);
	// add new entries
	int childIdx;
	for(childIdx=0; childIdx<newChildCount; childIdx++) {
		CatReadableTreeNode *child = (CatReadableTreeNode *) cat_tree_node_child_at((CatTreeNode *) priv->new_node, childIdx);
		VipNode *child_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) child);
		cat_log_trace("test if  new child[%d]=%o, vip-node=%o", childIdx, child, child_vip_node);
		CatLong *vip_id = cat_long_new(vip_node_get_id(child_vip_node));
		if (cat_hash_set_get(vipNodesAdded, (GObject *) vip_id)==NULL) {
			VipIResource *vipRes = vip_node_get_content(child_vip_node);
			cat_log_debug("vipRes=%o", vipRes);
			int z = 10000;
			if (VIP_IS_IFILE(vipRes)) {
				z = 5000;
			}
			MooContentMapWo *content_map = moo_content_map_wo_new();
			MooResourceContentWo *resource_content = moo_resource_content_wo_new(child, parent_resource_content);
			moo_content_map_wo_set(content_map, (MooIContent *) resource_content);
			moo_content_map_wo_create_link(content_map, moo_iservices_content_key(), moo_resource_content_wo_key());
			cat_unref_ptr(resource_content);
			long long moose_id = vip_isequence_next(priv->moose_sequence);

			CatStringWo *child_name = vip_node_get_name(child_vip_node);
			MooNodeWo *sub_node = moo_node_wo_new(moose_id, cat_ref_ptr(child_name), NULL, content_map, z);
			cat_log_debug("sub_node=%o", sub_node);
			moo_node_wo_append_child(priv->editableNode, sub_node);

			MooModuleWork *moduleWork = moo_module_work_new(priv->moose_sequence, child, sub_node);
			cat_array_wo_append(e_work_list, (GObject *) moduleWork);
			cat_unref_ptr(moduleWork);
			cat_unref_ptr(sub_node);
			cat_unref_ptr(content_map);
		}
		cat_unref_ptr(vip_id);
		cat_unref_ptr(child);
	}
	cat_unref_ptr(vipNodesAdded);
}



static void l_node_work_iface_init(MooINodeWorkInterface *iface) {
	iface->doWork = l_do_work;
}

/********************* end MooINodeWork implementation *********************/
