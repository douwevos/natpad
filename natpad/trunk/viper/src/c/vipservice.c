/*
   File:    vipservice.c
   Project: viper
   Author:  Douwe Vos
   Date:    Jun 6, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "vipservice.h"
#include "vipcdprovider.h"
#include "vipisequence.h"
#include "request/vipscanrequest.h"
#include "request/vipsubmitrequest.h"
#include "request/viprefreshnoderequest.h"
#include "vipcdprovider.h"
#include "model/file/vipsimplefilemapper.h"
#include "model/file/vipfsmap.h"
#include "model/scanner/vipmapperregistry.h"
#include "model/scanner/vipnodescanner.h"
#include <gio/gio.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipService"
#include <logging/catlog.h>

struct _VipServicePrivate {

	/***** new *****/

	VipPath *current_work_directory;
	CatWeakList *listeners;
	CatAtomicLong *id_generator;

	VipMapperRegistry *mapper_registry;

	CatAtomicReference *snapshot_ref;

	WorQueue *queue;
	CatTree *tree;

	uint64_t last_submit;

	VipCdProvider *cd_provider;

};

static void l_sequence_iface_init(VipISequenceInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipService, vip_service, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ISEQUENCE, l_sequence_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_service_class_init(VipServiceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipServicePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_service_init(VipService *instance) {
	VipServicePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_SERVICE, VipServicePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipService *instance = VIP_SERVICE(object);
	VipServicePrivate *priv = instance->priv;
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->id_generator);
	cat_unref_ptr(priv->mapper_registry);
	cat_unref_ptr(priv->snapshot_ref);
	cat_unref_ptr(priv->current_work_directory);
	cat_unref_ptr(priv->queue);
	cat_unref_ptr(priv->tree);
	cat_unref_ptr(priv->cd_provider);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

VipService *vip_service_new(WorService *wor_service, VipCdProvider *cd_provider) {
	VipService *result = g_object_new(VIP_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	VipServicePrivate *priv = result->priv;


	/***** new *****/

	priv->last_submit = 0;
	priv->listeners = cat_weak_list_new();
	priv->id_generator = cat_atomic_long_new();
	priv->snapshot_ref = cat_atomic_reference_new();

	VipSimpleFileMapper *simpleFileMapper = vip_simple_file_mapper_new((VipISequence *) result);
	priv->mapper_registry = vip_mapper_registry_new((VipIMapper*) simpleFileMapper);
	cat_unref_ptr(simpleFileMapper);

	priv->tree = cat_tree_new();
	priv->queue = wor_queue_new(wor_service);

	char *cwd = getcwd(NULL, 0);
	priv->current_work_directory = vip_path_new(cat_string_wo_new_with(cwd));
	cat_free_ptr(cwd);

	priv->cd_provider = cat_ref_ptr(cd_provider);

	return result;
}



//VipResourceMap *vip_service_get_resource_map(VipService *vip_service) {
//	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(vip_service);
//	return priv->resource_map;
//}

//gboolean vip_service_move_resource(VipService *vip_service, VipResource *source, VipResource *destination) {
//	gboolean result = TRUE;
//	if (source==destination) {
//		result = FALSE;
//	} else {
//		VipLockList *lock_list = vip_lock_list_new();
////		vip_lock_list_for_write(lock_list, destination);	// TODO move back in if MonClipboard l_paste_clipboard_received has changed
//		vip_lock_list_for_write(lock_list, vip_resource_get_node(source));
//		VipResourceNode *destination_node = vip_resource_get_node(destination);
//		VipResourceNode *source_node = vip_resource_get_node(source);
//		if (vip_resource_node_is_a_parent_of(source_node, destination_node)) {
//			result = FALSE;
//		} else {
//			if (vip_resource_node_lock_children(source_node, lock_list, TRUE)) {
//				GError *error = NULL;
//				VipPath *source_path = vip_resource_node_create_path(source_node);
//				CatStringWo *a_source_path_txt = vip_path_to_string(source_path);
//				cat_ref_sink_ptr(source_path_txt);
//				cat_log_detail("source_path_txt=%s", cat_string_wo_getchars(source_path_txt));
//				GFile *source_file = g_file_new_for_path(cat_string_wo_getchars(source_path_txt));
//				cat_unref_ptr(source_path_txt);
//
//				CatStringWo *a_source_leaf = vip_path_get_leaf(source_path);
//
//
//				VipPath *destination_path = vip_resource_node_create_path(destination_node);
//				destination_path = vip_path_create_child(destination_path, source_leaf);
//				CatStringWo *a_destination_path_txt = vip_path_to_string(destination_path);
//				cat_log_detail("destination_path_txt=%s", cat_string_wo_getchars(destination_path_txt));
//				GFile *dest_map = g_file_new_for_path(cat_string_wo_getchars(destination_path_txt));
//
//				cat_log_trace("trying to move");
//				g_file_move(source_file, dest_map, G_FILE_COPY_NONE, NULL, NULL, NULL, &error);
//				cat_log_trace("move done: error=%p", error);
//				if (error!=NULL) {
//					result = FALSE;
//					cat_log_error("move done: error-text=%s", error->message);
//				}
//				cat_unref_ptr(source_file);
//				cat_unref_ptr(source_path);
//			} else {
//				result = FALSE;
//				cat_log_error("could not lock children of source:%p", source);
//			}
//		}
//
//		vip_lock_list_release_all(lock_list);	// TODO: this should be the last command, but than monkey blocks on refresh
//		cat_unref_ptr(lock_list);
//
//		if (result) {
//			CatArrayWo *a_res_refs = vip_resource_node_enlist_references(source_node);
//			if (res_refs) {
//				CatIIterator *iter = cat_array_wo_iterator(a_res_refs);
//				while(cat_iiterator_has_next(iter)) {
//					VipIResourceReference *ref = (VipIResourceReference *) cat_iiterator_next(iter);
//					cat_log_trace("ref=%p - %s", ref, g_type_name_from_instance((GTypeInstance *) ref));
//					VipIResourceReferenceInterface *iface = VIP_IRESOURCE_REFERENCE_GET_INTERFACE(ref);
//					if (iface->moved) {
//						iface->moved(ref, NULL, destination);
//					}
//				}
//			}
//		}
//
//
//	}
//	cat_log_debug("done");
//	return result;
//}




//
//gboolean vip_service_rename_resource(VipService *vip_service, VipResource *resource, CatStringWo *a_new_name) {
//	gboolean result = TRUE;
//	VipLockList *lock_list = vip_lock_list_new();
//	VipResourceNode *resource_node = vip_resource_get_node(resource);
//	if (vip_resource_node_lock_children(resource_node, lock_list, TRUE)) {
//		VipPath *resource_path = vip_resource_node_create_path(resource_node);
//		CatStringWo *a_resource_path_txt = vip_path_to_string(resource_path);
//		cat_ref_sink_ptr(resource_path_txt);
//		cat_log_detail("source_path_txt=%s", cat_string_wo_getchars(resource_path_txt));
//		GFile *resource_file = g_file_new_for_path(cat_string_wo_getchars(resource_path_txt));
//		cat_unref_ptr(resource_path_txt);
//
//		GError *error = NULL;
//		GFile *result_file = g_file_set_display_name(resource_file, cat_string_wo_getchars(new_name), NULL, &error);
//		cat_unref_ptr(result_file);
//		if (error!=NULL) {
//			result = FALSE;
//			cat_log_error("rename: error-text=%s", error->message);
//		} else {
//			vip_resource_node_set_name(resource_node, new_name);
//		}
//
//		cat_unref_ptr(resource_file);
//
//	} else {
//		result = FALSE;
//		cat_log_error("could not lock children of source:%p", resource);
//	}
//
//	vip_lock_list_release_all(lock_list);
//	cat_unref_ptr(lock_list);
//
//	if (result) {
//		CatArrayWo *a_res_refs = vip_resource_node_enlist_references(resource_node);
//		if (res_refs) {
//			CatIIterator *iter = cat_array_wo_iterator(a_res_refs);
//			while(cat_iiterator_has_next(iter)) {
//				VipIResourceReference *ref = (VipIResourceReference *) cat_iiterator_next(iter);
//				cat_log_trace("ref=%p - %s", ref, g_type_name_from_instance((GTypeInstance *) ref));
//				VipIResourceReferenceInterface *iface = VIP_IRESOURCE_REFERENCE_GET_INTERFACE(ref);
//				if (iface->renamed) {
//					iface->renamed(ref, NULL, new_name);
//				}
//			}
//		}
//	}
//
//	return result;
//}
//
//
//
//gboolean vip_service_copy_resource(VipService *vip_service, VipResource *source, VipResource *destination) {
//	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(vip_service);
//	gboolean result = TRUE;
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
//	if (source==destination) {
//		result = FALSE;
//	} else {
//		VipLockList *lock_list = vip_lock_list_new();
////		vip_lock_list_for_write(lock_list, destination);	// TODO move back in if MonClipboard l_paste_clipboard_received has changed
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
//	return result;
//}
//
//
//gboolean vip_service_new_folder(VipService *vip_service, VipResource *source, CatStringWo *a_new_folder_name) {
////	VipResourceMap *resource_map = vip_service_get_resource_map(vip_service);
////	VipLockList *lock_list = vip_lock_list_new();
////	VipResource *child_resource = vip_resource_map_create_resource_child(resource_map, lock_list, vip_resource_get_node(source), new_folder_name);
////	gboolean result = FALSE;
////	if (child_resource) {
////		result = vip_service_create_directory(vip_service, lock_list, child_resource);
////	}
////	vip_lock_list_release_all(lock_list);
////	cat_unref_ptr(child_resource);
////	cat_unref_ptr(lock_list);
////	return result;
//}
//
//
//gboolean vip_service_create_directory(VipService *vip_service, VipLockList *lock_list, VipResource *new_directory) {
//	gboolean result = TRUE;
//	VipResourceNode *new_directory_node = vip_resource_get_node(new_directory);
//	vip_lock_list_for_write(lock_list, new_directory_node);
//	VipPath *dest_path = vip_resource_node_create_path(new_directory_node);
//	CatStringWo *a_dest_path_txt = vip_path_to_string(dest_path);
//	GFile *dest_file = g_file_new_for_path(cat_string_wo_getchars(dest_path_txt));
//	cat_log_debug("creating folder:%s", cat_string_wo_getchars(dest_path_txt));
//	cat_unref_ptr(dest_path);
//	cat_unref_ptr(dest_path_txt);
//
//	GError *error = NULL;
//	if (!g_file_make_directory(dest_file, NULL, &error)) {
//		result = FALSE;
//	}
//	cat_unref_ptr(dest_file);
//
//	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(vip_service);
//	vip_resource_map_rescan_info(priv->resource_map, new_directory);
//
//	vip_lock_list_release_last(lock_list);
//	return result;
//}
//
//
//









/***************************************** NEW  *****************************************/

VipICdProvider *vip_service_get_cd_provider(VipService *service) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	return (VipICdProvider *) (priv->cd_provider);
}


VipPath *vip_service_get_current_work_directory(VipService *service) {
	if (service==NULL) {
		char *cwd = getcwd(NULL, 0);
		VipPath *result = vip_path_new(cat_string_wo_new_with(cwd));
		cat_free_ptr(cwd);
		return result;
	}
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	return priv->current_work_directory;
}



void vip_service_add_listener(VipService *service, VipIListener *listener) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
	VipSnapshot *snapshot = (VipSnapshot *) cat_atomic_reference_get(priv->snapshot_ref);
	vip_ilistener_snapshot_set(listener, snapshot);
	cat_unref_ptr(snapshot);
}


void vip_service_post(VipService *service, WorRequest *request) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	wor_queue_post(priv->queue, request);
}


void vip_service_scan(VipService *service) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	VipScanRequest *vip_scan_request = vip_scan_request_new(service);
	wor_queue_post(priv->queue, (WorRequest *) vip_scan_request);
	cat_unref_ptr(vip_scan_request);
}


VipCreatePathRequest *vip_service_create_path(VipService *service, VipPath *full_path) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	VipCreatePathRequest *request = vip_create_path_request_new(service, full_path);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;
}

void vip_service_refresh_node(VipService *service, CatReadableTreeNode *node_to_refresh) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	cat_log_debug("node_to_refresh=%o", node_to_refresh);
	VipRefreshNodeRequest *request = vip_refresh_node_request_new(service, node_to_refresh);
	wor_queue_post(priv->queue, (WorRequest *) request);
	cat_unref_ptr(request);
}

VipRenameNodeRequest *vip_service_rename_node(VipService *service, CatReadableTreeNode *node_to_rename, CatStringWo *new_name) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	VipRenameNodeRequest *request = vip_rename_node_request_new(service, node_to_rename, new_name);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;
}

VipDeleteNodeRequest *vip_service_delete_node(VipService *service, CatReadableTreeNode *node_to_delete) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	VipDeleteNodeRequest *request = vip_delete_node_request_new(service, node_to_delete);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;
}



#define VIP_SUBMIT_FREQUENCY 1000

void vip_service_submit(VipService *service, gboolean now) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	VipSubmitRequest *request = vip_submit_request_new(service);
	if (now) {
		wor_queue_post(priv->queue, (WorRequest *) request);
	} else {
		int64_t current_time = cat_date_current_time();
		if (current_time>=priv->last_submit) {
			wor_queue_post_delayed(priv->queue, (WorRequest *) request, current_time+VIP_SUBMIT_FREQUENCY);
			priv->last_submit = current_time+VIP_SUBMIT_FREQUENCY;
		}
	}
	cat_unref_ptr(request);
}


/**
 * vipcopyormoverequest
 *
 * paths_to_copy     CatArray<VipPath*>
 */
VipCopyOrMoveRequest *vip_service_copy_or_move_to(VipService *service, CatArrayWo *paths_to_copy, gboolean move, CatReadableTreeNode *destination_vip) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	VipCopyOrMoveRequest *request = vip_copy_or_move_request_new(service, paths_to_copy, destination_vip, !move);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;

}



void vip_service_start(VipService *service) {
	cat_log_debug("starting service:%p", service);
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	long long nvipid = cat_atomic_long_increment(priv->id_generator);
	cat_log_debug("nvipid:%d", nvipid);

	VipNode *start_root_node = vip_node_new(nvipid, NULL);
	cat_log_debug("start_root_node:%o", start_root_node);

	CatWritableTreeNode *writable_root_node = cat_tree_get_writable_root_node(priv->tree);
	cat_writable_tree_node_set_content(writable_root_node, (GObject *) start_root_node);
	CatReadableTreeNode *submit = cat_tree_submit(priv->tree, FALSE);
	cat_unref_ptr(submit);
	cat_unref_ptr(writable_root_node);
	CatReadableTreeNode *root_node = cat_tree_get_readable_root_node_ref(priv->tree);
	VipSnapshot *snapshot = vip_snapshot_new(root_node);
	cat_atomic_reference_set(priv->snapshot_ref, (GObject *) snapshot);
	cat_log_debug("snapshot:%o", snapshot);
	cat_unref_ptr(root_node);
	cat_unref_ptr(start_root_node);

	cat_log_debug("snapshot:%o", snapshot);

	VipScanRequest *scan_request = vip_scan_request_new(service);
	wor_queue_post(priv->queue, (WorRequest *) scan_request);
	cat_unref_ptr(scan_request);
	cat_unref_ptr(snapshot);
	cat_unref_ptr(start_root_node);
	cat_log_debug("started service:%p", service);
}


CatTree *vip_service_get_tree(VipService *service) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	return priv->tree;
}


VipSnapshot *vip_service_get_snapshot(VipService *service) {
	cat_log_debug("get snapshot service:%p", service);
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	return (VipSnapshot *) cat_atomic_reference_get(priv->snapshot_ref);
}



gboolean vip_service_set_snapshot(VipService *service, VipSnapshot *new_snapshot) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);

//		IViperListener listeners[] = null;

	gboolean result = FALSE;

	while(TRUE) {
		VipSnapshot *vipSnapshot = (VipSnapshot *) cat_atomic_reference_get(priv->snapshot_ref);
		if (vipSnapshot==new_snapshot) {
			cat_unref_ptr(vipSnapshot);
			return TRUE;
		}

		if (vip_snapshot_get_version(vipSnapshot)<vip_snapshot_get_version(new_snapshot)) {
			if (cat_atomic_reference_compare_set(priv->snapshot_ref, (GObject *) vipSnapshot, (GObject *) new_snapshot)) {
				cat_unref_ptr(vipSnapshot);
				result = TRUE;
				break;
			} else {
				cat_unref_ptr(vipSnapshot);
				// TODO add retry_count
			}
		} else {
			cat_unref_ptr(vipSnapshot);
			break;
		}
	}

	if (result) {

		cat_log_on_debug(
			CatStringWo *e_t = cat_string_wo_new();
			vip_snapshot_print(new_snapshot,e_t);
			cat_log_debug("%s", cat_string_wo_getchars(e_t));
			cat_unref_ptr(e_t);
		)

		CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
		while(cat_iiterator_has_next(iter)) {
			VipIListener *listener = (VipIListener *) cat_iiterator_next(iter);
			cat_log_trace("calling listener:%o for new shanpshot:%o", listener, new_snapshot);
			vip_ilistener_snapshot_set(listener, new_snapshot);
		}
		cat_unref_ptr(iter);
	}
	return result;
}


//VipNodePath *vip_service_open(VipService *service, VipPath *full_path) {
//	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
//	VipNodePath *result = NULL;
//	VipSnapshot *set_snapshot = NULL;
//	VipSnapshot *new_snapshot = NULL;
//	int retryCount = 4;
//	VipNodePathOpener *pathOpener = vip_node_path_opener_new((VipISequence *) service, full_path);
//	while(retryCount-->0) {
//		set_snapshot = (VipSnapshot *) cat_atomic_reference_get(priv->snapshot_ref);
//		VipNodePath *nodePath = vip_node_path_opener_try_open(pathOpener, set_snapshot);
//		new_snapshot = vip_node_path_opener_get_snapshot(pathOpener);
//		if (new_snapshot==NULL) {
//			result = nodePath;
//			break;
//		}
//		if (vip_service_set_snapshot(service, new_snapshot)) {
//			result = nodePath;
//			break;
//		}
//		cat_unref_ptr(nodePath);
//		cat_unref_ptr(set_snapshot);
//	}
//	cat_unref_ptr(pathOpener);
//	cat_unref_ptr(set_snapshot);
//
//	return result;
//}

VipMapperRegistry *vip_service_get_mapper_registry(VipService *service) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	return priv->mapper_registry;
}


void vip_service_dump_tree(VipService *service) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(service);
	cat_tree_print(priv->tree);
}


/********************* start VipISequence implementation *********************/


static long long l_sequence_next(VipISequence *self) {
	VipServicePrivate *priv = VIP_SERVICE_GET_PRIVATE(self);
	return cat_atomic_long_increment(priv->id_generator);
}


static void l_sequence_iface_init(VipISequenceInterface *iface) {
	iface->next = l_sequence_next;
}

/********************* end VipISequence implementation *********************/
