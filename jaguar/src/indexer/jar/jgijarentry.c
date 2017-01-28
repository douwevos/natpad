/*
   File:    jgijarentry.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 17, 2014
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

#include "jgijarentryprivate.h"
#include "../../link/jagatomiclink.h"
#include "../../moose/package/jagpackagematcher.h"
#include "../../moose/package/jagpackagecontent.h"
#include <jaguarbytecode.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JgiJarEntry"
#include <logging/catlog.h>

struct _JgiJarEntryPrivate {
	VipISequence *moo_sequence;
	CatAtomicInteger *usage;
	VipNodePath *vip_node_path;
	VipSnapshot *last_vip_snaphhot;

	JagLink *link_moo_node;
	CatLock *lock;
};


G_DEFINE_TYPE(JgiJarEntry, jgi_jar_entry, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jgi_jar_entry_class_init(JgiJarEntryClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JgiJarEntryPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jgi_jar_entry_init(JgiJarEntry *instance) {
	JgiJarEntryPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JGI_TYPE_JAR_ENTRY, JgiJarEntryPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JgiJarEntry *instance = JGI_JAR_ENTRY(object);
	JgiJarEntryPrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_sequence);
	cat_unref_ptr(priv->vip_node_path);
	cat_unref_ptr(priv->usage);
	cat_unref_ptr(priv->last_vip_snaphhot);
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->link_moo_node);
	G_OBJECT_CLASS(jgi_jar_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jgi_jar_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JgiJarEntry *jgi_jar_entry_new(VipISequence *moo_sequence, VipNodePath *vip_node_path) {
	JgiJarEntry *result = g_object_new(JGI_TYPE_JAR_ENTRY, NULL);
	cat_ref_anounce(result);
	JgiJarEntryPrivate *priv = result->priv;
	priv->moo_sequence = cat_ref_ptr(moo_sequence);
	priv->vip_node_path = cat_ref_ptr(vip_node_path);
	if (vip_node_path==NULL) {
		cat_log_fatal("vip_node_path is NULL");
	}

	priv->usage = cat_atomic_integer_new();
	priv->last_vip_snaphhot = NULL;
	priv->lock = cat_lock_new();
	priv->link_moo_node = (JagLink *) jag_atomic_link_new();

	MooNodeWo *moo_node = moo_node_wo_new(0, NULL, NULL, NULL, 0);
	VipNode *tail_node = vip_node_path_get_tail(vip_node_path);
	CatStringWo *tail_name = vip_node_get_name(tail_node);
	cat_log_debug("vip_node_path=%o, tail_node=%o, name=%o", vip_node_path, tail_node, tail_name);
	MooNodeWo *e_jar_node = moo_node_wo_new(vip_isequence_next(moo_sequence), cat_ref_ptr(tail_name), NULL, NULL, 0);

	CatTreeNode *tree_node = vip_node_path_get_tail_node(vip_node_path);
	MooResourceContentWo *resource_content = moo_resource_content_wo_new((CatReadableTreeNode *) tree_node, NULL);
	moo_node_wo_set_content(e_jar_node, (MooIContent *) resource_content);
	cat_unref_ptr(resource_content);
	moo_node_wo_append_child(moo_node, e_jar_node);
	moo_node = moo_node_wo_anchor(moo_node, 1);
	jag_link_set_value(priv->link_moo_node, (GObject *) moo_node);
	cat_unref_ptr(moo_node);
	cat_unref_ptr(e_jar_node);
	return result;
}


void jgi_jar_entry_usage_up(JgiJarEntry *entry) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	cat_atomic_integer_increment(priv->usage);
}

gboolean jgi_jar_entry_usage_down(JgiJarEntry *entry) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	int nv = cat_atomic_integer_decrement(priv->usage);
	return nv == 0;
}

VipNodePath *jgi_jar_entry_get_vip_node_path(JgiJarEntry *entry) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	return priv->vip_node_path;
}

VipSnapshot *jgi_jar_entry_get_last_vip_snapshot(JgiJarEntry *entry) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	return priv->last_vip_snaphhot;
}

MooNodeWo *jgi_jar_entry_get_moo_node_ref(JgiJarEntry *entry) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	return (MooNodeWo *) jag_link_get_value_ref(priv->link_moo_node);
}
//
//
//static MooNodeWo *l_index_classfile(JgiJarEntryPrivate *priv, VipIFile *classfile) {
//	MooNodeWo *result = NULL;
//	CatIInputStream *classfile_stream = vip_ifile_open_input_stream(classfile);
//	if (classfile_stream) {
//		JagBytecodeReader *reader = jag_bytecode_reader_new(classfile_stream);
//		JagBytClassfile *byte_classfile = jag_bytecode_reader_parse_bytecode(reader);
//
//		if (byte_classfile) {
//			CatStringWo *class_name = jag_byt_classfile_get_fq_classname(byte_classfile);
//			if (class_name) {
//				result = moo_node_wo_new(vip_isequence_next(priv->moo_sequence), class_name, NULL, NULL, 1);
//
//				JagBytMethodList *method_list = jag_byt_classfile_get_method_list(byte_classfile);
//				if (method_list) {
//					CatIIterator *iter = jag_byt_method_list_iterator(method_list);
//					while(cat_iiterator_has_next(iter)) {
//						JagBytMethod *method = (JagBytMethod *) cat_iiterator_next(iter);
//						JagBytMethodHeader *method_header = jag_byt_method_get_method_header(method);
//						CatStringWo *method_name = jag_byt_method_header_get_method_name(method_header);
//						MooNodeWo *e_method_node = moo_node_wo_new(vip_isequence_next(priv->moo_sequence), method_name, NULL, NULL, 1);
//						moo_node_wo_append_child(result, e_method_node);
//						cat_unref_ptr(e_method_node);
//					}
//				}
//			}
//			cat_unref_ptr(byte_classfile);
//		}
//		cat_unref_ptr(reader);
//		cat_unref_ptr(classfile_stream);
//	}
//	return result;
//}
//
//static CatS l_s_txt_dot_class = { NULL, ".class"};

static void l_sync_viper_with_moose_nodes(JgiJarEntryPrivate *priv, CatTreeNode *tree_node, CatStringWo *a_package_name, MooNodeWo *e_jar_node, MooNodeWo *e_package_node) {
	int count = cat_tree_node_child_count(tree_node);
	int idx;

	JagPackageMatcher *package_matcher = jag_package_matcher_new();
	MooResourceNameMatcher *resource_matcher = moo_resource_name_matcher_new();

	for(idx=0; idx<count; idx++) {
		CatTreeNode *child = cat_tree_node_child_at(tree_node, idx);
//		cat_log_trace("[%d] %o", idx, child);
		VipNode *child_vip_node = (VipNode *) cat_tree_node_get_content(child);
		VipIResource *content = vip_node_get_content(child_vip_node);
		if (VIP_IS_IMAP(content)) {
			CatStringWo *a_child_name = vip_node_get_name(child_vip_node);
			CatStringWo *e_child_package_name = cat_string_wo_clone(a_package_name, CAT_CLONE_DEPTH_MAIN);
			if (cat_string_wo_length(e_child_package_name)>0) {
				cat_string_wo_append_unichar(e_child_package_name, '.');
			}
			cat_string_wo_append(e_child_package_name, a_child_name);
			e_child_package_name = cat_string_wo_anchor(e_child_package_name, 0);


			MooNodeWo *e_child_moo_node = NULL;
			jag_package_matcher_set_match_name(package_matcher, e_child_package_name);
			int fidx = moo_node_wo_find_index(e_jar_node, (MooINodeMatcher *) package_matcher, e_child_package_name, idx);
			if (fidx>=0) {
				e_child_moo_node = moo_node_wo_get_editable_child_at(e_jar_node, fidx);
			} else {
				e_child_moo_node = moo_node_wo_new(vip_isequence_next(priv->moo_sequence), cat_ref_ptr(e_child_package_name), NULL, NULL, -1);
				JagPackageContent *package_content = jag_package_content_new();
				moo_node_wo_set_content(e_child_moo_node, (MooIContent *) package_content);
				cat_unref_ptr(package_content);
				moo_node_wo_insert_child_at(e_jar_node, idx, e_child_moo_node);
				cat_unref(e_child_moo_node);
			}
			l_sync_viper_with_moose_nodes(priv, child, e_child_package_name, e_jar_node, e_child_moo_node);
			cat_unref_ptr(e_child_package_name);
		} else {
			CatStringWo *a_child_name = vip_iresource_get_name(content);
			MooNodeWo *e_child_moo_node = NULL;
			int fidx = moo_node_wo_find_index(e_package_node, (MooINodeMatcher *) resource_matcher, a_child_name, idx);
			if (fidx>=0) {
				e_child_moo_node = moo_node_wo_get_editable_child_at(e_package_node, fidx);
			} else {
				e_child_moo_node = moo_node_wo_new(vip_isequence_next(priv->moo_sequence), cat_ref_ptr(a_child_name), NULL, NULL, -1);
				MooResourceContentWo *resource_content = moo_resource_content_wo_new((CatReadableTreeNode *) child, NULL);
//				if (cat_string_wo_endswith(child_name,CAT_S(l_s_txt_dot_class))) {
//					VipIFile *res_file = (VipIFile *) vip_node_get_content(child_vip_node);
//					MooNodeWo *e_classfile_indexed = l_index_classfile(priv, res_file);
//					if (e_classfile_indexed) {
//						moo_node_wo_append_child(e_child_moo_node, e_classfile_indexed);
//						cat_unref_ptr(e_classfile_indexed);
//					}
//				}

				moo_node_wo_set_content(e_child_moo_node, (MooIContent *) resource_content);
				cat_unref_ptr(resource_content);
				moo_node_wo_insert_child_at(e_package_node, idx, e_child_moo_node);
				cat_unref(e_child_moo_node);
			}
		}
		cat_unref_ptr(child);
	}
	cat_unref_ptr(resource_matcher);
	cat_unref_ptr(package_matcher);
}

void jgi_jar_entry_set_vip_snapshot(JgiJarEntry *entry, VipSnapshot *snapshot) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	gboolean should_run = FALSE;
	cat_lock_lock(priv->lock);
	if ((snapshot==NULL) || (priv->last_vip_snaphhot==NULL) ||
			(vip_snapshot_get_version(snapshot)>vip_snapshot_get_version(priv->last_vip_snaphhot))) {
		cat_ref_swap(priv->last_vip_snaphhot, snapshot);
		should_run = TRUE;
	}

//	cat_lock_wait_timed(priv->lock, 7000L);

	cat_lock_unlock(priv->lock);

	cat_log_debug("should_run=%d", should_run);

	if (should_run) {
		CatTreeNode *tree_node = vip_node_path_get_tail_node(priv->vip_node_path);
		CatTreeNode *refreshed_tree_node = vip_snapshot_refresh(snapshot, tree_node);

		MooNodeWo *moo_node = (MooNodeWo *) jag_link_get_value_ref(priv->link_moo_node);

		MooNodeWo *e_root_node = moo_node_wo_ensure_editable(moo_node, NULL);
		MooNodeWo *e_jar_node = moo_node_wo_get_editable_child_at(e_root_node, 0);

		CatStringWo *a_package_name = cat_string_wo_new();

		l_sync_viper_with_moose_nodes(priv, refreshed_tree_node, a_package_name, e_jar_node, e_jar_node);
		cat_unref_ptr(a_package_name);

		MooNodeWo *a_root_node = moo_node_wo_anchor(e_root_node, (int) vip_snapshot_get_version(snapshot));

		cat_log_debug("done=%o", a_root_node);
		cat_unref_ptr(refreshed_tree_node);


		cat_lock_lock(priv->lock);
		if (snapshot==priv->last_vip_snaphhot) {
			jag_link_set_value(priv->link_moo_node, (GObject *) a_root_node);
			jag_link_notify(priv->link_moo_node, (GObject *) entry);
		}
		cat_lock_unlock(priv->lock);
		cat_unref_ptr(moo_node);
		cat_unref_ptr(a_root_node);
	}
}


void jgi_jar_entry_add_link_listener(JgiJarEntry *entry, JagILinkListener *link_listener) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	jag_link_add_referred_by(priv->link_moo_node, (GObject *) link_listener);
}

void jgi_jar_entry_remove_link_listener(JgiJarEntry *entry, JagILinkListener *link_listener) {
	JgiJarEntryPrivate *priv = JGI_JAR_ENTRY_GET_PRIVATE(entry);
	jag_link_remove_referred_by(priv->link_moo_node, (GObject *) link_listener);
}
