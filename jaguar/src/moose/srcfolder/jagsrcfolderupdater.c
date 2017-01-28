/*
   File:    jagsrcfolderupdater.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 20, 2013
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

#include "jagsrcfolderupdater.h"
#include "jagsrcfoldercontentwo.h"
#include "../package/jagpackageupdater.h"
#include "../package/jagpackagecontent.h"
#include "../package/jagpackagematcher.h"
#include "../srcfile/jagsrcfilematcher.h"
#include "../srcfile/jagsrcfilecontentwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagSrcFolderUpdater"
#include <logging/catlog.h>

struct _JagSrcFolderUpdaterPrivate {
	VipISequence *moo_id_sequence;
	MooNodeWo *editable_root;
	MooNodeWo *editable_src_folder;
	MooIdPath *src_folder_path;
};


G_DEFINE_TYPE(JagSrcFolderUpdater, jag_src_folder_updater, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_src_folder_updater_class_init(JagSrcFolderUpdaterClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagSrcFolderUpdaterPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_src_folder_updater_init(JagSrcFolderUpdater *instance) {
	JagSrcFolderUpdaterPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_SRC_FOLDER_UPDATER, JagSrcFolderUpdaterPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagSrcFolderUpdater *instance = JAG_SRC_FOLDER_UPDATER(object);
	JagSrcFolderUpdaterPrivate *priv = instance->priv;
	cat_unref_ptr(priv->editable_root);
	cat_unref_ptr(priv->editable_src_folder);
	cat_unref_ptr(priv->moo_id_sequence);
	cat_unref_ptr(priv->src_folder_path);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagSrcFolderUpdater *jag_src_folder_updater_new(VipISequence *moo_id_sequence, MooNodeWo *editable_root, MooNodeWo *editable_src_folder,
		MooIdPath *src_folder_path) {
	JagSrcFolderUpdater *result = g_object_new(JAG_TYPE_SRC_FOLDER_UPDATER, NULL);
	cat_ref_anounce(result);
	JagSrcFolderUpdaterPrivate *priv = result->priv;
	priv->moo_id_sequence = cat_ref_ptr(moo_id_sequence);
	priv->editable_root = cat_ref_ptr(editable_root);
	priv->editable_src_folder = cat_ref_ptr(editable_src_folder);
	priv->src_folder_path = cat_ref_sink_ptr(src_folder_path);
	return result;
}

void jag_src_folder_updater_run(JagSrcFolderUpdater *updater) {
	JagSrcFolderUpdaterPrivate *priv = JAG_SRC_FOLDER_UPDATER_GET_PRIVATE(updater);
	MooNodeWo *srcFolderNode = moo_id_path_create_editable_node(priv->src_folder_path, priv->editable_root);

	cat_log_debug("path=%o, node=%o", priv->src_folder_path, priv->editable_root);

	if (srcFolderNode == NULL) {
		cat_log_error("ERROR :::: path invalid");
		// TODO all old packages need to be removed
		return;
	}

	JagSrcFolderContentWo *src_folder_content_wo = (JagSrcFolderContentWo *) moo_node_wo_get_content(priv->editable_src_folder, jag_src_folder_content_wo_key());
	JagSrcFolderContentWo *e_src_folder_content_wo = jag_src_folder_content_wo_ensure_editable(src_folder_content_wo, priv->editable_src_folder);
	moo_node_wo_set_content(priv->editable_src_folder, (MooIContent *) e_src_folder_content_wo);
	cat_unref_ptr(e_src_folder_content_wo);

	cat_log_debug("updating packages: srcFolderNode.count()=%d", moo_node_wo_child_count(srcFolderNode));

	/* scan for new packages and update existing */
	JagPackageMatcher *package_matcher = jag_package_matcher_new();
	JagSrcfileMatcher *classfile_matcher = jag_srcfile_matcher_new();

	int child_idx;
	for(child_idx = moo_node_wo_child_count(srcFolderNode)-1; child_idx>=0; child_idx--) {
		MooNodeWo *child = moo_node_wo_child_at(srcFolderNode, child_idx);
		if (cat_string_wo_beginswith_char(moo_node_wo_get_name(child), '.')) {
			continue;
		}

		MooNodeWo *ochild = moo_node_wo_get_original(child);
		MooResourceContentWo *childResContent = (MooResourceContentWo *) moo_node_wo_get_content(child, moo_resource_content_wo_key());
		cat_log_debug("child[%d] = %o", child_idx, ochild);
		if (childResContent!=NULL) {
			VipNode *res_node = moo_resource_content_wo_get_viper_node(childResContent);
			if (res_node) {
				VipIResource *viperNode = vip_node_get_content(res_node);
				cat_log_debug("child[%d] vip-node=%o", child_idx, viperNode);
				if (VIP_IS_IMAP(viperNode)) {
					CatStringWo *a_packageName = moo_node_wo_get_name(child);
					cat_log_debug("packagename=%o", a_packageName);
					jag_package_matcher_set_match_name(package_matcher, a_packageName);
					cat_log_debug("set matcher");
					int foundIndex = moo_node_wo_find_index(priv->editable_src_folder, (MooINodeMatcher *) package_matcher, NULL,  -1);
					cat_log_debug("child[%d] is a map : packageName=%o, foundIndex=%d", child_idx, a_packageName, foundIndex);
					JagPackageContent *jagNodePackage = NULL;
					MooNodeWo *e_package_node = NULL;
					if (foundIndex>=0) {
						e_package_node = moo_node_wo_get_editable_child_at(priv->editable_src_folder, foundIndex);
					} else {
						long long uid = vip_isequence_next(priv->moo_id_sequence);
						e_package_node = moo_node_wo_new(uid, cat_ref_ptr(a_packageName), NULL, NULL, 200);
						jagNodePackage = jag_package_content_new();
						moo_node_wo_set_content(e_package_node, (MooIContent *) jagNodePackage);
						moo_node_wo_append_child(priv->editable_src_folder, e_package_node);
						cat_unref(e_package_node);
						cat_unref(jagNodePackage);
					}

					MooIdPath *subPkgPath = moo_id_path_append_id(priv->src_folder_path, moo_node_wo_get_unique_id(e_package_node), child_idx);
					JagPackageUpdater *package_updater = jag_package_updater_new(priv->moo_id_sequence, priv->editable_src_folder, ochild, e_package_node, subPkgPath);
					jag_package_updater_run(package_updater);
					cat_unref_ptr(package_updater);
				} else if (VIP_IS_IFILE(viperNode)) {
					/* TODO what about classes in the default package */
					CatStringWo *a_file_name = moo_node_wo_get_name(child);
					jag_srcfile_matcher_set_match_name(classfile_matcher, a_file_name);
					cat_log_debug("pacakge-name to find:%o", a_file_name);

					int foundIndex = moo_node_wo_find_index(priv->editable_src_folder, (MooINodeMatcher *) classfile_matcher, NULL, -1);
					cat_log_debug("foundIndex:%d", foundIndex);
					JagSrcfileContentWo *jagNodeclassfile = NULL;
	//				MooNodeWo *editablePackageNode = NULL;
					if (foundIndex>=0) {
	//					editablePackageNode =
								moo_node_wo_get_editable_child_at(priv->editable_src_folder, foundIndex);
						// TODO the uniqueId might have changed
					} else {
						MooIdPath *classfilePath = moo_id_path_new();
						moo_id_path_append_id(classfilePath, moo_node_wo_get_unique_id(child), child_idx);
						MooContentMapWo *econtentMap = moo_content_map_wo_new();
						jagNodeclassfile = jag_srcfile_content_wo_new(classfilePath, (VipIFile *) viperNode);
						moo_content_map_wo_set(econtentMap, (MooIContent *) jagNodeclassfile);
						moo_content_map_wo_set(econtentMap, (MooIContent *) childResContent);
						long long uid = vip_isequence_next(priv->moo_id_sequence);
						MooNodeWo *classNode = moo_node_wo_new(uid, cat_ref_ptr(a_file_name), NULL, econtentMap, 10);

						moo_node_wo_append_child(priv->editable_src_folder, classNode);
						cat_unref_ptr(classNode);
						cat_unref_ptr(econtentMap);
						cat_unref(jagNodeclassfile);
					}
				}
			}
		}
	}
	cat_unref_ptr(package_matcher);
	cat_unref_ptr(classfile_matcher);

}








