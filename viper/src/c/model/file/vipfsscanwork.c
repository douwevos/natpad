/*
   File:    vipfsscanwork.c
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

#include "vipfsscanwork.h"
#include "../scanner/vipmapperregistry.h"
#include "../scanner/vipiscanwork.h"
#include "../access/vipnodenamematcher.h"
#include "../vipimap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipFSScanWork"
#include <logging/catlog.h>

struct _VipFSScanWorkPrivate {
	VipISequence *sequence;
	CatWritableTreeNode *node;
	gboolean recursive_from_parent;
	gboolean validated_by_parent;
	VipNode *main_node;
	CatArrayWo *child_work;	// <VipIScanWork>
};

static void l_scan_work_iface_init(VipIScanWorkInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipFSScanWork, vip_fs_scan_work, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(VipFSScanWork)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ISCAN_WORK, l_scan_work_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_fs_scan_work_class_init(VipFSScanWorkClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_fs_scan_work_init(VipFSScanWork *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipFSScanWork *instance = VIP_FS_SCAN_WORK(object);
	VipFSScanWorkPrivate *priv = vip_fs_scan_work_get_instance_private(instance);
	cat_unref_ptr(priv->sequence);
	cat_log_detail("dispose:%p main_node", object);
	cat_unref_ptr(priv->main_node);
	cat_log_detail("dispose:%p child_work", object);
	cat_unref_ptr(priv->child_work);
	cat_log_detail("dispose:%p node", object);
	cat_unref_ptr(priv->node);

	cat_log_detail("dispose:%p parent", object);
	G_OBJECT_CLASS(vip_fs_scan_work_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_fs_scan_work_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipFSScanWork *vip_fs_scan_work_new(VipISequence *sequence, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent) {
	VipFSScanWork *result = g_object_new(VIP_TYPE_FS_SCAN_WORK, NULL);
	cat_ref_anounce(result);
	VipFSScanWorkPrivate *priv = vip_fs_scan_work_get_instance_private(result);

	priv->sequence = cat_ref_ptr(sequence);
	priv->node = cat_ref_ptr(node);
	priv->main_node = cat_ref_ptr(cat_tree_node_get_content((CatTreeNode *) node));
	priv->child_work =NULL;
	priv->recursive_from_parent = recursive_from_parent;
	priv->validated_by_parent = validated_by_parent;
	return result;
}


/********************* start VipIScanWork implementation *********************/

static void l_initialize_work_for_map(VipFSScanWork *scan_work, VipMapperRegistry *mapper_registry, VipIMap *map) {
	VipFSScanWork *instance = VIP_FS_SCAN_WORK(scan_work);
	VipFSScanWorkPrivate *priv = vip_fs_scan_work_get_instance_private(instance);
	CatArrayWo *workList = cat_array_wo_new();	// <VipIScanWork>
	CatHashSet *namesAdded = cat_hash_set_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);	// <CatStringWo>
	gboolean parent_recursive = priv->recursive_from_parent || vip_node_should_scan_recursive(priv->main_node);
	if (parent_recursive && map!=NULL) {
		CatArrayWo *enlisted = NULL;	// <VipIResource>
		enlisted = vip_imap_enlist(map);

		if (enlisted) {
			VipNodeNameMatcher *matcher = vip_node_name_matcher_new(NULL);
			int guessIndex = -1;
			CatIIterator *iter = cat_array_wo_iterator(enlisted);
			while(cat_iiterator_has_next(iter)) {
				VipIResource *childFile = (VipIResource *) cat_iiterator_next(iter);
				CatStringWo *resourceName = vip_iresource_get_name(childFile);
				cat_hash_set_add(namesAdded, (GObject *) resourceName);

				vip_node_name_matcher_set_match_name(matcher, resourceName);
				int foundIndex = cat_tree_node_find_child_index((CatTreeNode *) priv->node, (CatIMatcher *) matcher,  guessIndex);

				cat_log_detail("resourceName=%o, foundIndex=%d", resourceName, foundIndex);

				if (foundIndex>=0) {
					CatWritableTreeNode *child_at = (CatWritableTreeNode *) cat_tree_node_child_at((CatTreeNode *) priv->node, foundIndex);
					VipNode *oldChild = (VipNode *) cat_tree_node_get_content((CatTreeNode *) child_at);
					guessIndex = foundIndex+1;
					VipIResource *old_res = vip_node_get_content(oldChild);

					GType old_type = ((GTypeInstance *) old_res)->g_class->g_type;
					GType new_type = ((GTypeInstance *) childFile)->g_class->g_type;
					cat_log_detail("oldChild=%o", oldChild);
					VipIScanWork *new_work = vip_mapper_registry_create_work_for_node(mapper_registry, child_at, TRUE, TRUE);
					cat_array_wo_append(workList, (GObject *) new_work);
					cat_unref_ptr(new_work);
					long long use_id = 0L;
					if (old_type==new_type) {
						if (vip_iresource_equal(old_res, childFile)) {
							cat_unref_ptr(child_at);
							cat_log_detail("use old-child");
							continue;
						}
						use_id = vip_node_get_id(oldChild);
					} else {
						use_id = vip_isequence_next(priv->sequence);
					}
					VipNode *vipNode = vip_node_new_full(use_id, childFile, vip_node_get_hold_counter(oldChild), vip_node_get_recursive_counter(oldChild));
					cat_writable_tree_node_set_content(child_at, (GObject *) vipNode);
					cat_unref_ptr(child_at);
					cat_unref_ptr(vipNode);
					continue;
				}

				CatWritableTreeNode *appended_node = cat_writable_tree_node_append_child(priv->node);
				long long next_id = vip_isequence_next(priv->sequence);
				VipNode *vipNode = vip_node_new(next_id, childFile);
				cat_log_detail("create new-child:%o", vipNode);

				cat_writable_tree_node_set_content(appended_node, (GObject *) vipNode);
				VipIScanWork *new_work = vip_mapper_registry_create_work_for_node(mapper_registry, appended_node, TRUE, TRUE);
				cat_array_wo_append(workList, (GObject *) new_work);
				cat_unref_ptr(new_work);
				cat_unref_ptr(vipNode);
				cat_unref_ptr(appended_node);
			}

			cat_unref_ptr(matcher);
			cat_unref_ptr(iter);
		}
		cat_unref_ptr(enlisted);
	}

	int idx;
	int main_child_count = cat_tree_node_child_count((CatTreeNode *) priv->node);
	for(idx=0; idx<main_child_count; idx++) {
		CatTreeNode *child_at = cat_tree_node_child_at((CatTreeNode *) priv->node, idx);
		VipNode *childAt = (VipNode *) cat_tree_node_get_content(child_at);
		CatStringWo *child_name = vip_node_get_name(childAt);
		if (cat_hash_set_get(namesAdded, (GObject *) child_name)==NULL) {
			VipIScanWork *new_work = vip_mapper_registry_create_work_for_node(mapper_registry, (CatWritableTreeNode *) child_at, parent_recursive, FALSE);
			cat_array_wo_append(workList, (GObject *) new_work);
			cat_unref_ptr(new_work);
		}
		cat_unref_ptr(child_at);
	}
	cat_ref_swap(priv->child_work, workList);
	cat_unref_ptr(namesAdded);
}

static CatArrayWo *l_scan_work_init_child_work(VipIScanWork *self, struct _VipMapperRegistry *mapper_registry) {
	VipFSScanWork *instance = VIP_FS_SCAN_WORK(self);
	VipFSScanWorkPrivate *priv = vip_fs_scan_work_get_instance_private(instance);
	if (priv->child_work==NULL) {
		VipIResource *content = vip_node_get_content(priv->main_node);
		if (VIP_IS_IMAP(content)) {
			l_initialize_work_for_map((VipFSScanWork *) self, mapper_registry, (VipIMap *) content);
			return priv->child_work;
		} else if (content == NULL) {
			CatArrayWo *workList = cat_array_wo_new();	// <VipIScanWork>
			int count = cat_tree_node_child_count((CatTreeNode *) priv->node);
			for(count--; count>=0; count--) {
				CatTreeNode *child_node = cat_tree_node_child_at((CatTreeNode *) priv->node, count);
				cat_log_debug("child_node == %o", child_node);
				VipFSScanWork *work = vip_fs_scan_work_new(priv->sequence, (CatWritableTreeNode *) child_node, FALSE, FALSE);
				cat_unref_ptr(child_node);
				cat_array_wo_append(workList, (GObject *) work);
				cat_unref_ptr(work);
			}
			priv->child_work = cat_ref_ptr(workList);
			return priv->child_work;
		}
	}
	return NULL;

}

static void l_scan_work_run_scan(VipIScanWork *self) {
	VipFSScanWork *instance = VIP_FS_SCAN_WORK(self);
	VipFSScanWorkPrivate *priv = vip_fs_scan_work_get_instance_private(instance);
	VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);

	gboolean hold = (priv->recursive_from_parent && priv->validated_by_parent)
			|| vip_node_should_hold(vip_node)
			|| vip_node_should_scan_recursive(vip_node)
			|| cat_tree_node_child_count((CatTreeNode *) priv->node)>0;
	if (!hold) {
		CatWritableTreeNode *parent_node = (CatWritableTreeNode *) cat_tree_node_get_parent_node((CatTreeNode *) priv->node);
		if (parent_node!=NULL) {
			cat_writable_tree_node_remove_child(parent_node, priv->node);
			cat_unref_ptr(parent_node);
		}
	}
}


static void l_scan_work_iface_init(VipIScanWorkInterface *iface) {
	iface->initChildWork = l_scan_work_init_child_work;
	iface->runScan = l_scan_work_run_scan;
}

/********************* end VipIScanWork implementation *********************/

static void l_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipFSScanWork *instance = VIP_FS_SCAN_WORK(self);
	VipFSScanWorkPrivate *priv = vip_fs_scan_work_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:main-node=%o, rec-from-par=%d, val-by-par=%d]", iname, self, priv->main_node,
			priv->recursive_from_parent, priv->validated_by_parent);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_print;
}
