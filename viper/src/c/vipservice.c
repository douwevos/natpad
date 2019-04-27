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
		G_ADD_PRIVATE(VipService)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ISEQUENCE, l_sequence_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_service_class_init(VipServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_service_init(VipService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipService *instance = VIP_SERVICE(object);
	VipServicePrivate *priv = vip_service_get_instance_private(instance);
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->id_generator);
	cat_unref_ptr(priv->mapper_registry);
	cat_unref_ptr(priv->snapshot_ref);
	cat_unref_ptr(priv->current_work_directory);
	cat_unref_ptr(priv->queue);
	cat_unref_ptr(priv->tree);
	cat_unref_ptr(priv->cd_provider);
	G_OBJECT_CLASS(vip_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

VipService *vip_service_new(WorService *wor_service, VipCdProvider *cd_provider) {
	VipService *result = g_object_new(VIP_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	VipServicePrivate *priv = vip_service_get_instance_private(result);

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


/***************************************** NEW  *****************************************/

VipICdProvider *vip_service_get_cd_provider(VipService *service) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	return (VipICdProvider *) (priv->cd_provider);
}

VipPath *vip_service_get_current_work_directory(VipService *service) {
	if (service==NULL) {
		char *cwd = getcwd(NULL, 0);
		VipPath *result = vip_path_new(cat_string_wo_new_with(cwd));
		cat_free_ptr(cwd);
		return result;
	}
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	return priv->current_work_directory;
}

void vip_service_add_listener(VipService *service, VipIListener *listener) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
	VipSnapshot *snapshot = (VipSnapshot *) cat_atomic_reference_get(priv->snapshot_ref);
	vip_ilistener_snapshot_set(listener, snapshot);
	cat_unref_ptr(snapshot);
}

void vip_service_post(VipService *service, WorRequest *request) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	wor_queue_post(priv->queue, request);
}

void vip_service_scan(VipService *service) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	VipScanRequest *vip_scan_request = vip_scan_request_new(service);
	wor_queue_post(priv->queue, (WorRequest *) vip_scan_request);
	cat_unref_ptr(vip_scan_request);
}


VipCreatePathRequest *vip_service_create_path(VipService *service, VipPath *full_path) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	VipCreatePathRequest *request = vip_create_path_request_new(service, full_path);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;
}

void vip_service_refresh_node(VipService *service, CatReadableTreeNode *node_to_refresh) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	cat_log_debug("node_to_refresh=%o", node_to_refresh);
	VipRefreshNodeRequest *request = vip_refresh_node_request_new(service, node_to_refresh);
	wor_queue_post(priv->queue, (WorRequest *) request);
	cat_unref_ptr(request);
}

VipRenameNodeRequest *vip_service_rename_node(VipService *service, CatReadableTreeNode *node_to_rename, CatStringWo *new_name) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	VipRenameNodeRequest *request = vip_rename_node_request_new(service, node_to_rename, new_name);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;
}

VipDeleteNodeRequest *vip_service_delete_node(VipService *service, CatReadableTreeNode *node_to_delete) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	VipDeleteNodeRequest *request = vip_delete_node_request_new(service, node_to_delete);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;
}


#define VIP_SUBMIT_FREQUENCY 1000

void vip_service_submit(VipService *service, gboolean now) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
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
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	VipCopyOrMoveRequest *request = vip_copy_or_move_request_new(service, paths_to_copy, destination_vip, !move);
	wor_queue_post(priv->queue, (WorRequest *) request);
	return request;

}

void vip_service_start(VipService *service) {
	cat_log_debug("starting service:%p", service);
	VipServicePrivate *priv = vip_service_get_instance_private(service);
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
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	return priv->tree;
}


VipSnapshot *vip_service_get_snapshot(VipService *service) {
	cat_log_debug("get snapshot service:%p", service);
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	return (VipSnapshot *) cat_atomic_reference_get(priv->snapshot_ref);
}

gboolean vip_service_set_snapshot(VipService *service, VipSnapshot *new_snapshot) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);

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

VipMapperRegistry *vip_service_get_mapper_registry(VipService *service) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	return priv->mapper_registry;
}

void vip_service_dump_tree(VipService *service) {
	VipServicePrivate *priv = vip_service_get_instance_private(service);
	cat_tree_print(priv->tree);
}


/********************* start VipISequence implementation *********************/

static long long l_sequence_next(VipISequence *self) {
	VipService *instance = VIP_SERVICE(self);
	VipServicePrivate *priv = vip_service_get_instance_private(instance);
	return cat_atomic_long_increment(priv->id_generator);
}

static void l_sequence_iface_init(VipISequenceInterface *iface) {
	iface->next = l_sequence_next;
}

/********************* end VipISequence implementation *********************/
