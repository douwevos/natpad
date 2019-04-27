/*
   File:    jagscanjarmapwork.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 4, 2013
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

#include "jagscanjarmapwork.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagScanJarMapWork"
#include <logging/catlog.h>

struct _JagScanJarMapWorkPrivate {
	VipISequence *id_provider;
	CatWritableTreeNode *node;
	CatArrayWo /* <VipIScanWork*> */ *e_child_work;
	gboolean recursive_from_parent;
};

static void l_scan_work_iface_init(VipIScanWorkInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagScanJarMapWork, jag_scan_jar_map_work, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagScanJarMapWork)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ISCAN_WORK, l_scan_work_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_scan_jar_map_work_class_init(JagScanJarMapWorkClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_scan_jar_map_work_init(JagScanJarMapWork *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagScanJarMapWork *instance = JAG_SCAN_JAR_MAP_WORK(object);
	JagScanJarMapWorkPrivate *priv = jag_scan_jar_map_work_get_instance_private(instance);
	cat_unref_ptr(priv->e_child_work);
	cat_unref_ptr(priv->id_provider);
	cat_unref_ptr(priv->node);
	G_OBJECT_CLASS(jag_scan_jar_map_work_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_scan_jar_map_work_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagScanJarMapWork *jag_scan_jar_map_work_new(VipISequence *id_provider, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent) {
	JagScanJarMapWork *result = g_object_new(JAG_TYPE_SCAN_JAR_MAP_WORK, NULL);
	cat_ref_anounce(result);
	JagScanJarMapWorkPrivate *priv = jag_scan_jar_map_work_get_instance_private(result);
	priv->id_provider = cat_ref_ptr(id_provider);
	priv->node = cat_ref_ptr(node);
	priv->e_child_work = NULL;
	priv->recursive_from_parent = recursive_from_parent;
	return result;
}


/********************* start VipIScanWork implementation *********************/

static void l_initialize_work_for_map(JagScanJarMapWork *scan_work, VipMapperRegistry *mapper_registry) {
	JagScanJarMapWorkPrivate *priv = jag_scan_jar_map_work_get_instance_private(scan_work);
	cat_unref_ptr(priv->e_child_work);
	priv->e_child_work = cat_array_wo_new();
	cat_log_debug("scan_work=%p, child_work=%p", scan_work, priv->e_child_work);

	VipNode *main_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);
	gboolean parentRecursive = priv->recursive_from_parent || vip_node_should_scan_recursive(main_vip_node);

	VipIResource *main_resource = vip_node_get_content(main_vip_node);
	if (parentRecursive && VIP_IS_IMAP(main_resource)) {
		VipIMap *map = (VipIMap *) main_resource;
		CatArrayWo *e_enlisted = NULL;	// <VipIResource>
		e_enlisted = vip_imap_enlist(map);


		if (e_enlisted) {
			int guessIndex = -1;
			CatIIterator *iter = cat_array_wo_iterator(e_enlisted);
			VipNodeNameMatcher *matcher = vip_node_name_matcher_new(NULL);
			while(cat_iiterator_has_next(iter)) {
				VipIResource *childFile = (VipIResource *) cat_iiterator_next(iter);
				CatStringWo *a_resourceName = vip_iresource_get_name(childFile);

				vip_node_name_matcher_set_match_name(matcher, a_resourceName);
				int foundIndex = cat_tree_node_find_child_index((CatTreeNode *) priv->node,(CatIMatcher *) matcher, guessIndex);

				CatWritableTreeNode *child_node = NULL;
				if (foundIndex>=0) {
					child_node = (CatWritableTreeNode *) cat_tree_node_child_at((CatTreeNode *) priv->node, foundIndex);
					guessIndex = foundIndex+1;
				} else {
					long long vip_id = vip_isequence_next(priv->id_provider);
					VipNode *child_vip_node = vip_node_new(vip_id, childFile);
					child_node = cat_writable_tree_node_append_child(priv->node);
					cat_writable_tree_node_set_content(child_node, (GObject *) child_vip_node);
					cat_unref_ptr(child_vip_node);
				}

				VipIScanWork *work = vip_mapper_registry_create_work_for_node(mapper_registry, child_node, TRUE, TRUE);
				cat_array_wo_append(priv->e_child_work, (GObject *) work);
				cat_unref_ptr(work);
				cat_unref_ptr(child_node);
			}
			cat_unref_ptr(iter);
			cat_unref_ptr(matcher);
		}
		cat_unref_ptr(e_enlisted);

	} else {
		cat_log_error("not a map:%o", main_resource);
	}
}

static CatArrayWo *l_scan_work_init_child_work(VipIScanWork *self, struct _VipMapperRegistry *mapper_registry) {
	JagScanJarMapWork *instance = JAG_SCAN_JAR_MAP_WORK(self);
	JagScanJarMapWorkPrivate *priv = jag_scan_jar_map_work_get_instance_private(instance);
	if (priv->e_child_work==NULL) {
//		VipIResource *content = vip_node_get_content(priv->main_node);
//		if (VIP_IS_IMAP(content)) {
			l_initialize_work_for_map((JagScanJarMapWork *) self, mapper_registry);
			return cat_ref_ptr(priv->e_child_work);
//		}
	}
	return NULL;

}

static void l_scan_work_run_scan(VipIScanWork *self) {
//	JagScanJarMapWorkPrivate *priv = JAG_SCAN_JAR_MAP_WORK_GET_PRIVATE(self);
}


static void l_scan_work_iface_init(VipIScanWorkInterface *iface) {
	iface->initChildWork = l_scan_work_init_child_work;
	iface->runScan = l_scan_work_run_scan;
}

/********************* end VipIScanWork implementation *********************/
