/*
   File:    jagpackageupdater.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 23, 2013
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

#include "jagpackageupdater.h"
#include "jagpackagematcher.h"
#include "jagpackagecontent.h"
#include "../srcfile/jagsrcfilematcher.h"
#include "../srcfile/jagsrcfilecontentwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPackageUpdater"
#include <logging/catlog.h>

struct _JagPackageUpdaterPrivate {
	VipISequence *moo_id_sequence;
	MooNodeWo *editable_src_folder;
	MooNodeWo *res_node;
	MooNodeWo *editable_package;
	MooIdPath *package_path;
};


G_DEFINE_TYPE(JagPackageUpdater, jag_package_updater, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_package_updater_class_init(JagPackageUpdaterClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagPackageUpdaterPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_package_updater_init(JagPackageUpdater *instance) {
	JagPackageUpdaterPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_PACKAGE_UPDATER, JagPackageUpdaterPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPackageUpdater *instance = JAG_PACKAGE_UPDATER(object);
	JagPackageUpdaterPrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_id_sequence);
	cat_unref_ptr(priv->editable_src_folder);
	cat_unref_ptr(priv->res_node);
	cat_unref_ptr(priv->editable_package);
	cat_unref_ptr(priv->package_path);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPackageUpdater *jag_package_updater_new(VipISequence *moo_id_sequence, MooNodeWo *editable_src_folder, MooNodeWo *res_node, MooNodeWo *editable_package, MooIdPath *package_path) {
	JagPackageUpdater *result = g_object_new(JAG_TYPE_PACKAGE_UPDATER, NULL);
	cat_ref_anounce(result);
	JagPackageUpdaterPrivate *priv = result->priv;
	priv->moo_id_sequence = cat_ref_ptr(moo_id_sequence);
	priv->editable_src_folder = cat_ref_ptr(editable_src_folder);
	priv->res_node = cat_ref_ptr(res_node);
	priv->editable_package = cat_ref_ptr(editable_package);
	priv->package_path = cat_ref_sink_ptr(package_path);
	return result;
}


void jag_package_updater_run(JagPackageUpdater *updater) {
	JagPackageUpdaterPrivate *priv = JAG_PACKAGE_UPDATER_GET_PRIVATE(updater);
	/* scan for new packages and update existing */
	JagPackageMatcher *package_matcher = jag_package_matcher_new();
	JagSrcfileMatcher *classfile_matcher = jag_srcfile_matcher_new();
	int res_child_count = moo_node_wo_child_count(priv->res_node);
	int childIdx;
	for(childIdx = res_child_count-1; childIdx>=0; childIdx--) {
		MooNodeWo *child = moo_node_wo_child_at(priv->res_node, childIdx);
		CatStringWo *a_child_name = moo_node_wo_get_name(child);
		if (cat_string_wo_beginswith_char(a_child_name, '.')) {
			continue;
		}
		MooNodeWo *ochild = moo_node_wo_get_original(child);
		cat_log_debug("ochild=%o", ochild);
		MooResourceContentWo *childResContent = (MooResourceContentWo *) moo_node_wo_get_content(child, moo_resource_content_wo_key());
		cat_log_debug("child[%d] = %o", childIdx, ochild);
		if (childResContent!=NULL) {
			VipNode *child_vip_node = moo_resource_content_wo_get_viper_node(childResContent);
			if (child_vip_node) {
				VipIResource *childVipContent = vip_node_get_content(child_vip_node);
				if (VIP_IS_IMAP(childVipContent)) {
					CatStringWo *e_packageName = cat_string_wo_clone(moo_node_wo_get_name(priv->editable_package), CAT_CLONE_DEPTH_MAIN);
						// TODO ref counting for packageName
	//					String packageName = resNode.name;
					if (cat_string_wo_length(e_packageName)>0) {
						cat_string_wo_append_unichar(e_packageName, '.');
					}
					cat_string_wo_append(e_packageName, moo_node_wo_get_name(child));
					jag_package_matcher_set_match_name(package_matcher, e_packageName);
					int foundIndex = moo_node_wo_find_index(priv->editable_src_folder, (MooINodeMatcher *) package_matcher, NULL, -1);
					cat_log_debug("child[%d] is a map : packageName=%o, foundIndex=%d", childIdx, e_packageName, foundIndex);
					MooNodeWo *e_package_node = NULL;
					if (foundIndex>=0) {
						e_package_node = moo_node_wo_get_editable_child_at(priv->editable_src_folder, foundIndex);
					} else {
						long long uid = vip_isequence_next(priv->moo_id_sequence);
						e_package_node = moo_node_wo_new(uid, cat_ref_ptr(e_packageName), NULL, NULL, 10);
						JagPackageContent *package_content = jag_package_content_new();
						moo_node_wo_set_content(e_package_node, (MooIContent *) package_content);
						cat_unref(package_content);
						moo_node_wo_append_child(priv->editable_src_folder, e_package_node);
						cat_unref(e_package_node);
					}

					MooIdPath *subPkgPath = moo_id_path_append_id(priv->package_path, moo_node_wo_get_unique_id(e_package_node), childIdx);
					JagPackageUpdater *sub_pack_updater = jag_package_updater_new(priv->moo_id_sequence, priv->editable_src_folder, ochild, e_package_node, subPkgPath);
					jag_package_updater_run(sub_pack_updater);
					cat_unref_ptr(sub_pack_updater);
					cat_unref_ptr(e_packageName);

				} else if (VIP_IS_IFILE(childVipContent)) {
					cat_log_debug("child[%d] is a file", childIdx);
					CatStringWo *e_packageName = cat_string_wo_clone(moo_node_wo_get_name(child), CAT_CLONE_DEPTH_MAIN);
					jag_srcfile_matcher_set_match_name(classfile_matcher, e_packageName);
					cat_log_debug("pacakge-name to find:%o", e_packageName);

					int foundIndex = moo_node_wo_find_index(priv->editable_package, (MooINodeMatcher *) classfile_matcher, NULL, -1);
					cat_log_debug("foundIndex:%d", foundIndex);
	//				MooNodeWo *editablePackageNode = NULL;
					if (foundIndex>=0) {
	//					editablePackageNode =
						MooNodeWo *e_source_node = (MooNodeWo *) moo_node_wo_get_editable_child_at(priv->editable_package, foundIndex);
						moo_node_wo_set_content(e_source_node, (MooIContent *) childResContent);
						JagSrcfileContentWo *source_content = (JagSrcfileContentWo *) moo_node_wo_get_content(e_source_node, jag_srcfile_content_wo_key());
						JagSrcfileContentWo *e_src_content = jag_srcfile_content_wo_ensure_editable(source_content);
						jag_srcfile_content_wo_set_file(e_src_content, (VipIFile *) childVipContent);

						moo_node_wo_set_content(e_source_node, (MooIContent *) e_src_content);
						cat_unref_ptr(e_src_content);

						// TODO the uniqueId might have changed
					} else {
						MooIdPath *classfilePath = moo_id_path_append_id(priv->package_path, moo_node_wo_get_unique_id(child), childIdx);
						long long uid = vip_isequence_next(priv->moo_id_sequence);
						MooNodeWo *e_source_node = moo_node_wo_new(uid, cat_ref_ptr(e_packageName), NULL, NULL, 10);
						JagSrcfileContentWo *source_content = jag_srcfile_content_wo_new(classfilePath, (VipIFile *) childVipContent);
						moo_node_wo_set_content(e_source_node, (MooIContent *) source_content);
						moo_node_wo_set_content(e_source_node, (MooIContent *) childResContent);
						cat_unref(source_content);
						moo_node_wo_append_child(priv->editable_package, e_source_node);
						cat_unref_ptr(e_source_node);
					}
	//					MonIdPath subPkgPath = packagePath.addId(editablePackage.getUniqueId(), childIdx);


	//					new JagRequestUpdatePackage(monkeyService, resNode, editablePackage);
					cat_unref_ptr(e_packageName);
				}
			}
		}
	}
	cat_unref_ptr(package_matcher);
	cat_unref_ptr(classfile_matcher);

}

