/*
   File:    vipcopyormoverequest.c
   Project: viper
   Author:  Douwe Vos
   Date:    Feb 26, 2014
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

#include "vipcopyormoverequest.h"
#include "vipcreatepathrequestprivate.h"
#include "../vipisequence.h"
#include "../model/vipvirtualresource.h"
#include "../model/vipifile.h"
#include "../model/vipimap.h"
#include "../model/access/vipnodeidmatcher.h"
#include "../model/file/vipfsfile.h"
#include "../model/file/vipfsmap.h"
#include "../model/scanner/vipnodescanner.h"
#include "../vipservice.h"
#include <gio/gio.h>


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipCopyOrMoveRequest"
#include <logging/catlog.h>

struct _VipCopyOrMoveRequestPrivate {
	VipService *vip_service;
	CatArrayWo *e_source_path_list;
	CatReadableTreeNode *destination;
	gboolean is_copy;
	char *buf;
};

G_DEFINE_TYPE_WITH_PRIVATE(VipCopyOrMoveRequest, vip_copy_or_move_request, WOR_TYPE_REQUEST);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void vip_copy_or_move_request_class_init(VipCopyOrMoveRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void vip_copy_or_move_request_init(VipCopyOrMoveRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipCopyOrMoveRequest *instance = VIP_COPY_OR_MOVE_REQUEST(object);
	VipCopyOrMoveRequestPrivate *priv = vip_copy_or_move_request_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->e_source_path_list);
	cat_unref_ptr(priv->destination);
	cat_free_ptr(priv->buf);

	G_OBJECT_CLASS(vip_copy_or_move_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_copy_or_move_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

VipCopyOrMoveRequest *vip_copy_or_move_request_new(struct _VipService *vip_service, CatArrayWo *source_path_list, CatReadableTreeNode *destination, gboolean is_copy) {
	VipCopyOrMoveRequest *result = g_object_new(VIP_TYPE_COPY_OR_MOVE_REQUEST, NULL);
	cat_ref_anounce(result);
	VipCopyOrMoveRequestPrivate *priv = vip_copy_or_move_request_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->e_source_path_list = cat_ref_ptr(source_path_list);
	priv->destination = cat_ref_ptr(destination);
	priv->is_copy = is_copy;
	priv->buf = NULL;
	wor_request_construct((WorRequest *) result);
	return result;
}


gboolean l_plain_old_file_copy(CatWritableTreeNode *source, CatWritableTreeNode *dest_node) {
	gboolean result = TRUE;

	VipNodePath *source_node_path = vip_node_path_from_tree_node((CatTreeNode *) source);
	VipNode *source_vip_node = vip_node_path_get_tail(source_node_path);

	VipNodePath *dest_node_path = vip_node_path_from_tree_node((CatTreeNode *) dest_node);
	VipPath *dest_path = vip_node_path_create_path(dest_node_path);
	VipPath *dest_entry_path = vip_path_create_child(dest_path, vip_node_get_name(source_vip_node));
	CatStringWo *dest_path_txt = vip_path_to_string(dest_entry_path);
	GFile *dest_gfile = g_file_new_for_path(cat_string_wo_getchars(dest_path_txt));
	cat_unref_ptr(dest_path_txt);
	cat_unref_ptr(dest_path);


	VipPath *source_path = vip_node_path_create_path(source_node_path);
	CatStringWo *source_path_txt = vip_path_to_string(source_path);
	GFile *source_gfile = g_file_new_for_path(cat_string_wo_getchars(source_path_txt));
	cat_unref_ptr(source_path_txt);
	cat_unref_ptr(source_path);

	GError *error = NULL;

	cat_log_debug("copy from %s to %s", g_file_get_uri(source_gfile), g_file_get_uri(dest_gfile));

	if (g_file_copy(source_gfile, dest_gfile, G_FILE_COPY_NONE, NULL, NULL, NULL, &error)) {
		cat_log_info("copied file");
	} else {
		cat_log_error("could not copy file:%s", error->message);
		result = FALSE;
	}

	cat_unref_ptr(source_gfile);
	cat_unref_ptr(dest_gfile);

	return result;
}


#define BUF_SIZE  0x10000

gboolean l_copy_using_stream(VipCopyOrMoveRequest *request, CatWritableTreeNode *source, CatWritableTreeNode *dest_node) {
	VipCopyOrMoveRequestPrivate *priv = vip_copy_or_move_request_get_instance_private(request);
	if (priv->buf==NULL) {
		priv->buf = g_malloc(BUF_SIZE);
	}

	gboolean result = TRUE;

	VipNode *source_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) source);

	VipNodePath *dest_node_path = vip_node_path_from_tree_node((CatTreeNode *) dest_node);
	VipPath *dest_path = vip_node_path_create_path(dest_node_path);
	VipPath *dest_entry_path = vip_path_create_child(dest_path, vip_node_get_name(source_vip_node));
	CatStringWo *dest_path_txt = vip_path_to_string(dest_entry_path);
	GFile *dest_gfile = g_file_new_for_path(cat_string_wo_getchars(dest_path_txt));
	cat_unref_ptr(dest_path_txt);
	cat_unref_ptr(dest_path);

	GError *gtk_error = NULL;

	GFileOutputStream *out_stream = g_file_create(dest_gfile, G_FILE_CREATE_REPLACE_DESTINATION, NULL, &gtk_error);
	if (gtk_error) {
		return FALSE;
	}

	VipIFile *source_resource = (VipIFile *) vip_node_get_content(source_vip_node);

	CatIInputStream *in_stream = vip_ifile_open_input_stream(source_resource);
	while(TRUE) {
		int count = cat_iinput_stream_read_length(in_stream, priv->buf, BUF_SIZE);
		if (count>0) {
			g_output_stream_write_all((GOutputStream *) out_stream, priv->buf, count, NULL, NULL, &gtk_error);
			if (gtk_error) {
				result = FALSE;
				break;
			}
		} else {
			break;
		}
	}

	cat_unref_ptr(in_stream);

	g_output_stream_close((GOutputStream *) out_stream, NULL, &gtk_error);

//	if (g_file_copy(source_gfile, dest_gfile, G_FILE_COPY_NONE, NULL, NULL, NULL, &error)) {
//		cat_log_info("copied file");
//	} else {
//		cat_log_error("could not copy file:%s", error->message);
//		result = FALSE;
//	}

	cat_unref_ptr(dest_gfile);
	return result;
}

gboolean l_copy_stack(VipCopyOrMoveRequest *request, CatArrayWo *stack) {
	VipCopyOrMoveRequestPrivate *priv = vip_copy_or_move_request_get_instance_private(request);
	gboolean result = TRUE;

	CatWritableTreeNode *dest = NULL;
	CatWritableTreeNode *source = NULL;

	while(cat_array_wo_size(stack)>=2) {
		cat_array_wo_remove_last(stack, (GObject **) &dest);
		cat_array_wo_remove_last(stack, (GObject **) &source);
		cat_log_debug("source=%p, dest=%p", source, dest);

		VipNode *source_vip_node = (VipNode*) cat_tree_node_get_content((CatTreeNode *) source);
		VipIResource *source_resource = vip_node_get_content(source_vip_node);
		if (VIP_IS_IFILE(source_resource)) {
			if (VIP_IS_FS_FILE(source_resource)) {
				l_plain_old_file_copy(source, dest);
			} else {
				l_copy_using_stream(request, source, dest);
			}
		} else if (VIP_IS_IMAP(source_resource)) {

			CatStringWo *new_map_name = vip_node_get_name(source_vip_node);
			VipNodePath *dest_node_path = vip_node_path_from_tree_node((CatTreeNode *) dest);
			VipPath *dest_path = vip_node_path_create_path(dest_node_path);
			VipPath *dest_entry_path = vip_path_create_child(dest_path, new_map_name);

			CatWritableTreeNode *new_dest = cat_writable_tree_node_append_child(dest);
			long long vip_id = vip_isequence_next((VipISequence *) priv->vip_service);
			VipFSMap *new_dest_resource = vip_fs_map_new(dest_entry_path);
			VipNode *new_dest_vip_node = vip_node_new(vip_id, (VipIResource *) new_dest_resource);
			cat_writable_tree_node_set_content(new_dest, (GObject *) new_dest_vip_node);


			CatStringWo *dest_path_txt = vip_path_to_string(dest_entry_path);
			GFile *dest_file = g_file_new_for_path(cat_string_wo_getchars(dest_path_txt));
			GError *error = NULL;
			if (!g_file_make_directory(dest_file, NULL, &error)) {
				result = FALSE;
				break;
			}

			int child_count = cat_tree_node_child_count((CatTreeNode *) source);
			int child_idx;
			for(child_idx=0; child_idx<child_count; child_idx++) {
				CatTreeNode *source_child = cat_tree_node_child_at((CatTreeNode *) source, child_idx);
				cat_array_wo_append(stack, (GObject *) new_dest);
				cat_array_wo_append(stack, (GObject *) source_child);
				cat_unref_ptr(source_child);
			}
		}
	}
	return result;
}



static gboolean l_copy_resources(VipCopyOrMoveRequest *request) {
	VipCopyOrMoveRequestPrivate *priv = vip_copy_or_move_request_get_instance_private(request);
	gboolean result = TRUE;

	VipCreatePathRequest *create_path_request = vip_create_path_request_new(priv->vip_service, NULL);
//	VipNodePath *dest_path = vip_node_path_from_tree_node((CatTreeNode *) priv->destination);

	VipNodeScanner *nodeScanner = vip_node_scanner_new(vip_service_get_mapper_registry(priv->vip_service));

	CatTree *tree = vip_service_get_tree(priv->vip_service);
	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(tree);
	CatITreeEntryListProvider *list_provider = cat_tree_node_get_list_provider((CatTreeNode *) writable_root_node);
	VipNodeIdMatcher *matcher = vip_node_id_matcher_new(0l);

	/* refresh destination node*/
	VipNode *dest_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->destination);
	vip_node_id_matcher_set_id(matcher, vip_node_get_id(dest_vip_node));
	CatTreeNode *fresh_dest_node = cat_tree_node_refresh((CatTreeNode *) priv->destination, list_provider, (CatIMatcher *) matcher);
	CatWritableTreeNode *writable_dest = cat_writable_tree_node_new(list_provider, cat_tree_node_get_location(fresh_dest_node));
	cat_unref_ptr(fresh_dest_node);

	CatArrayWo *stack = cat_array_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(priv->e_source_path_list);
	while(cat_iiterator_has_next(iter)) {
		VipPath *path = (VipPath *) cat_iiterator_next(iter);
		cat_log_debug("path=%p", path);

		/* create node-path */
		VipNodePath *source_path = vip_create_path_request_for_path(create_path_request, path);
		CatTreeNode *node = vip_node_path_get_tail_node(source_path);

		/* refresh tail node*/
		VipNode *source_vip_node = (VipNode *) cat_tree_node_get_content(node);
		vip_node_id_matcher_set_id(matcher, vip_node_get_id(source_vip_node));
		CatTreeNode *fresh_source_node = cat_tree_node_refresh(node, list_provider, (CatIMatcher *) matcher);
		CatWritableTreeNode *sn = cat_writable_tree_node_new(list_provider, cat_tree_node_get_location(fresh_source_node));
		cat_unref_ptr(fresh_source_node);
		source_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) sn);

// TODO : the code below is disabled for now since the node-scanner does not guarantee that a replaced virtual resource
//		  has the same vip-node-id
//
//		/* see if tail node is a virtual node */
//		VipIResource *source_resource = vip_node_get_content(source_vip_node);
//		if (VIP_IS_VIRTUAL_RESOURCE(source_resource)) {
//
//
//			/* node is a virtual resource ... lets see if we can resolve it using the node-scanner */
//			CatWritableTreeNode *scan_test = cat_tree_node_get_parent_node(sn);
//			CatWritableTreeNode *scan_wn = cat_ref_ptr(sn);
//			while(scan_test!=NULL) {
//				VipNode test_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) sn);
//				VipIResource *test_resource = vip_node_get_content(test_vip_node);
//				if (VIP_IS_VIRTUAL_RESOURCE(source_resource)) {
//					cat_unref_ptr(scan_wn);
//					scan_wn = scan_test;
//					cat_unref_ptr(scan_test);
//					break;
//				} else {
//					scan_wn = scan_test;
//					scan_test = cat_tree_node_get_parent_node(scan_test);
//				}
//			}
//
//			/* scan_wn now contains the first non virtual resource in the parent-list of our source node-path */
//			vip_node_scanner_scan(nodeScanner, scan_wn);
//		}

		cat_log_debug("sn=%p, writable_dest=%p", sn, writable_dest);
		cat_array_wo_append(stack, (GObject *) sn);
		cat_array_wo_append(stack, (GObject *) writable_dest);
		cat_unref_ptr(sn);
	}
	cat_unref_ptr(iter);

	l_copy_stack(request, stack);

	vip_node_scanner_scan(nodeScanner, writable_dest, TRUE);

	CatReadableTreeNode *submit = cat_tree_submit(tree, FALSE);
	cat_log_debug("submit=%o", submit);
	if (submit==NULL) {
		submit = cat_tree_get_readable_root_node_ref(tree);
	}
	VipSnapshot *snapshot = vip_service_get_snapshot(priv->vip_service);

	VipSnapshot *new_snapshot = vip_snapshot_set_root(snapshot, submit);
	cat_unref_ptr(snapshot);
	cat_unref_ptr(submit);
	vip_service_set_snapshot(priv->vip_service, new_snapshot);
	cat_unref_ptr(new_snapshot);
	cat_unref_ptr(nodeScanner);


//	vip_node_scanner_scan(nodeScanner, writable_root_node);


//
//
//	cat_log_on_debug({
//		VipPath *source_path = vip_resource_node_create_path(vip_resource_get_node(source));
//		CatStringWo *a_source_txt = vip_path_to_string(source_path);
//		cat_ref_sink_ptr(source_txt);
//		VipPath *dest_path = vip_resource_node_create_path(vip_resource_get_node(destination));
//		CatStringWo *a_dest_txt = vip_path_to_string(dest_path);
//		cat_ref_sink_ptr(dest_txt);
//		cat_log_debug("Copying:source=%s to dest:%s", cat_string_wo_getchars(source_txt), cat_string_wo_getchars(dest_txt));
//	});
//
//	CatTree *tree = vip_service_get_tree(priv->vip_service);
//	CatWritableTreeNode *writable_root = cat_tree_get_writable_root_node(tree);
//
//
//
//
//
//
////	if (source==destination) {
////		result = FALSE;
////	} else {
//		VipLockList *lock_list = vip_lock_list_new();
//		VipResourceNode *destination_node = vip_resource_get_node(destination);
//		VipResourceNode *source_node = vip_resource_get_node(source);
//
//		vip_lock_list_for_write(lock_list, source_node);
//		if (vip_resource_node_is_a_parent_of(source_node, destination_node)) {
//			result = FALSE;
//		} else {
//			if (vip_resource_node_lock_children(source_node, lock_list, FALSE)) {
//
//				GError *error = NULL;
//
//				CatArrayWo *e_stack = cat_array_wo_new();
//				cat_array_wo_append(e_stack, (GObject *) source);
//				cat_array_wo_append(e_stack, (GObject *) destination);
//				VipResource *vip_sub_dest = NULL;
//				VipResource *vip_sub_source = NULL;
//				while(cat_array_wo_size(stack)>0) {
//					cat_unref_ptr(vip_sub_dest);
//					cat_unref_ptr(vip_sub_source);
//					cat_array_wo_remove_last(e_stack, (GObject **) (&vip_sub_dest));
//					cat_array_wo_remove_last(e_stack, (GObject **) (&vip_sub_source));
//					// TODO unref sub_dest and sub_srouce
//					source_node = vip_resource_get_node(vip_sub_source);
//					destination_node = vip_resource_get_node(vip_sub_dest);
//					VipResourceInfo *source_info = vip_resource_node_get_info(source_node);
//					if (source_info==NULL) {
//						cat_log_error("no info for source_node:source_node=%p", source_node);
//						cat_array_wo_clear(a_stack);
//					} else {
//						CatStringWo *a_source_node_name = vip_resource_node_get_name(source_node);
//						VipResource *new_dest = vip_resource_map_create_resource_child(priv->resource_map, lock_list, destination_node, source_node_name);
//						VipResourceNode *new_dest_node = vip_resource_get_node(new_dest);
//						if (vip_resource_info_is_directory(source_info)) {
//							/* copy directory */
//							// should we lock new_dest here ??
//							VipResourceInfo *new_dest_info = vip_resource_node_get_info(new_dest_node);
//							if (new_dest_info==NULL) {
//								/* non existing directory, create a new directory */
//								if (!vip_service_create_directory(vip_service, lock_list, new_dest)) {
//									cat_log_error("could not create directory");
//									cat_unref_ptr(new_dest);
//									result = FALSE;
//									break;
//								}
//							} else if (!vip_resource_info_is_directory(new_dest_info)) {
//								cat_log_error("Destination is an existing file, expected a directory");
//								cat_unref_ptr(new_dest);
//								result = FALSE;
//								break;
//							}
//
//							CatArrayWo *a_directory_content = vip_resource_info_get_directory_content(source_info);
//							if (directory_content) {
//								CatIIterator *iter = cat_array_wo_iterator(a_directory_content);
//								while(cat_iiterator_has_next(iter)) {
//									CatStringWo *a_child_name = (CatStringWo *) cat_iiterator_next(iter);
//									VipResource *child_source = vip_resource_map_create_resource_child(priv->resource_map, lock_list, source_node, child_name);
//									cat_array_wo_append(e_stack, (GObject *) new_dest);
//									cat_array_wo_append(e_stack, (GObject *) child_source);
//									cat_unref_ptr(child_source);
//								}
//								cat_unref_ptr(iter);
//							}
//							cat_unref_ptr(new_dest);
//						} else {
//							/* copy file */
//							VipPath *dest_path = vip_resource_node_create_path(new_dest_node);
//							CatStringWo *a_dest_path_txt = vip_path_to_string(dest_path);
//							cat_ref_sink_ptr(dest_path_txt);
//							GFile *dest_gfile = g_file_new_for_path(cat_string_wo_getchars(dest_path_txt));
//							cat_unref_ptr(dest_path_txt);
//							cat_unref_ptr(dest_path);
//
//							VipPath *source_path = vip_resource_node_create_path(source_node);
//							CatStringWo *a_source_path_txt = vip_path_to_string(source_path);
//							cat_ref_sink_ptr(source_path_txt);
//							GFile *source_gfile = g_file_new_for_path(cat_string_wo_getchars(source_path_txt));
//							cat_unref_ptr(source_path_txt);
//							cat_unref_ptr(source_path);
//
//							if (g_file_copy(source_gfile, dest_gfile, G_FILE_COPY_NONE, NULL, NULL, NULL, &error)) {
//								cat_log_info("copied file");
//							} else {
//								cat_log_error("could not copy file:%s", error->message);
//								cat_array_wo_clear(a_stack);	/* we break the copy operation */
//								result = FALSE;
//							}
//
//							cat_unref_ptr(source_gfile);
//							cat_unref_ptr(dest_gfile);
//						}
//					}
//				}
//				cat_unref_ptr(vip_sub_dest);
//				cat_unref_ptr(vip_sub_source);
//
//				cat_unref_ptr(stack);
//
//			} else {
//				result = FALSE;
//				cat_log_error("could not lock children of source:%p", source);
//			}
//		}
//
//		vip_lock_list_release_all(lock_list);
//		cat_unref_ptr(lock_list);
//	}
//	cat_log_debug("done");
	return result;
}

static void l_run_request(WorRequest *self) {
	l_copy_resources(VIP_COPY_OR_MOVE_REQUEST(self));
}
