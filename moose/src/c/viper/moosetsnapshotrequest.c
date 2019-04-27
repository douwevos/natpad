/*
   File:    moosetsnapshotrequest.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 11, 2013
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

#include "moosetsnapshotrequest.h"
#include "moosnapshotwork.h"
#include "mooinodework.h"
#include "../transaction/mootransaction.h"
#include "../model/moonodewo.h"
#include "../model/module/moomodulecontentwo.h"
#include "../model/module/moomodulerefreshviperrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooSetSnapshotRequest"
#include <logging/catlog.h>

struct _MooSetSnapshotRequestPrivate {
	MooService *moo_service;
	CatAtomicReference *vip_snapshot_ref;
	VipSnapshot *snapshot;
	long long infoCount;
};

static CatAtomicLong *l_info_count_gen;

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooSetSnapshotRequest, moo_set_snapshot_request, WOR_TYPE_REQUEST, {
		G_ADD_PRIVATE(MooSetSnapshotRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void moo_set_snapshot_request_class_init(MooSetSnapshotRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_clazz = WOR_REQUEST_CLASS(clazz);
	wor_clazz->runRequest = l_run_request;
	l_info_count_gen = cat_atomic_long_new_val(1000);
	cat_ref_ignore(l_info_count_gen);
}

static void moo_set_snapshot_request_init(MooSetSnapshotRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooSetSnapshotRequest *instance = MOO_SET_SNAPSHOT_REQUEST(object);
	MooSetSnapshotRequestPrivate *priv = moo_set_snapshot_request_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->vip_snapshot_ref);
	cat_unref_ptr(priv->snapshot);
	G_OBJECT_CLASS(moo_set_snapshot_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_set_snapshot_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooSetSnapshotRequest *moo_set_snapshot_request_new(MooService *moo_service, CatAtomicReference *vip_snapshot_ref, VipSnapshot *snapshot) {
	MooSetSnapshotRequest *result = g_object_new(MOO_TYPE_SET_SNAPSHOT_REQUEST, NULL);
	cat_ref_anounce(result);
	MooSetSnapshotRequestPrivate *priv = moo_set_snapshot_request_get_instance_private(result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->vip_snapshot_ref = cat_ref_ptr(vip_snapshot_ref);
	priv->snapshot = cat_ref_ptr(snapshot);
	priv->infoCount = cat_atomic_long_increment(l_info_count_gen);
	wor_request_construct((WorRequest *) result);
	wor_request_set_time_out((WorRequest *) result, cat_date_current_time()+250);
	return result;
}

static void l_dispatch_refresh_requests(MooSetSnapshotRequestPrivate *priv, CatHashSet *nodesWhichNeedRefresh);

static void l_run_request(WorRequest *request) {
	cat_log_debug("starting:%o", request);
	MooSetSnapshotRequest *instance = MOO_SET_SNAPSHOT_REQUEST(request);
	MooSetSnapshotRequestPrivate *priv = moo_set_snapshot_request_get_instance_private(instance);
	MooTransaction *transaction = moo_service_create_transaction((GObject *) request, priv->moo_service);
	int  retryCnt = 0;
	CatHashSet *nodes_which_need_refresh = cat_hash_set_new((GHashFunc) moo_node_wo_hash_code, (GEqualFunc) moo_node_wo_equals);

	while(retryCnt++<10) {	// TODO: report error when we didn't succeed after 10 retries
		cat_hash_set_clear(nodes_which_need_refresh);
		moo_transaction_begin(transaction);
		VipSnapshot *currentSnapshot = (VipSnapshot *) cat_atomic_reference_get(priv->vip_snapshot_ref);
		cat_log_debug("%ld >> currentSnapshot=%o, snapshot=%o", priv->infoCount, currentSnapshot, priv->snapshot);
		if (currentSnapshot!=priv->snapshot) {
			if (currentSnapshot!=NULL && priv->snapshot!=NULL) {
				if (vip_snapshot_get_version(currentSnapshot)>vip_snapshot_get_version(priv->snapshot)) {
					cat_log_debug("current snaphot has higher or equal version number");
					cat_unref_ptr(currentSnapshot);
					break;
				}
			}

			gboolean compareAndSet = cat_atomic_reference_compare_set(priv->vip_snapshot_ref, (GObject *) currentSnapshot, (GObject *) priv->snapshot);
			cat_unref_ptr(currentSnapshot);
			cat_log_debug("%ld >> compareAndSet=%d", priv->infoCount, compareAndSet);
			if (!compareAndSet) {
				continue;
			}

			MooNodeWo *tx_root_node = moo_transaction_get_tx_root_node(transaction);
			MooNodeWo *e_root_node = moo_node_wo_ensure_editable(tx_root_node, NULL);


			int root_node_idx;
			for(root_node_idx = moo_node_wo_child_count(e_root_node)-1; root_node_idx>=0; root_node_idx--) {
				MooNodeWo *editableChild = moo_node_wo_get_editable_child_at(e_root_node, root_node_idx);
				MooModuleContentWo *moduleContent = (MooModuleContentWo *) moo_node_wo_get_content(editableChild, moo_module_content_wo_key());
				cat_log_detail("root_node_idx=%d, moduleContent=%o", root_node_idx, moduleContent);
				if (moduleContent != NULL) {

					CatReadableTreeNode *old_module_content = moo_module_content_wo_get_base_node(moduleContent);
					CatReadableTreeNode *new_module_content = (CatReadableTreeNode *) vip_snapshot_refresh(priv->snapshot, (CatTreeNode *) old_module_content);

					cat_log_detail("root_node_idx=%d, new-moduleContent=%o", root_node_idx, new_module_content);
					if (cat_tree_node_get_location((CatTreeNode *) new_module_content)!=-1) {
						if (old_module_content!=new_module_content) {
							MooModuleContentWo *e_module_content = moo_module_content_wo_ensure_editable(moduleContent);
							if (e_module_content!=moduleContent) {
								moo_node_wo_set_content(editableChild, (MooIContent *) e_module_content);
							}
							cat_unref(e_module_content);
							moo_module_content_wo_set_base_node(e_module_content, new_module_content);
						}
						cat_hash_set_add(nodes_which_need_refresh, (GObject *) editableChild);
					}
					cat_unref_ptr(new_module_content);
				}
			}

			gboolean commit_ok = moo_transaction_commit(transaction, e_root_node);
			if (commit_ok) {
				l_dispatch_refresh_requests(priv, nodes_which_need_refresh);
				break;
			} else {
				cat_log_error("%ld >> failed set", priv->infoCount);
			}
		} else {
			cat_unref_ptr(currentSnapshot);
			break;
		}
	}
	cat_unref_ptr(transaction);
	cat_unref_ptr(nodes_which_need_refresh);
}

static void l_dispatch_refresh_requests(MooSetSnapshotRequestPrivate *priv, CatHashSet *nodesWhichNeedRefresh) {
	if (cat_hash_set_size(nodesWhichNeedRefresh)==0) {
		return;
	}
	WorService *wor_service = moo_service_get_worm_service(priv->moo_service);
	CatIIterator *iter = cat_hash_set_iterator(nodesWhichNeedRefresh);
	while(cat_iiterator_has_next(iter)) {
		MooNodeWo *node = (MooNodeWo *) cat_iiterator_next(iter);
		cat_log_detail("sending refresh request : node=%o", node);
		MooModuleRefreshViperRequest *req = moo_module_refresh_viper_request_new(priv->moo_service, moo_node_wo_get_unique_id(node), TRUE);
		wor_service_post_request(wor_service, (WorRequest *) req);
		cat_unref_ptr(req);
	}
	cat_unref_ptr(iter);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooSetSnapshotRequest *instance = MOO_SET_SNAPSHOT_REQUEST(self);
	MooSetSnapshotRequestPrivate *priv = moo_set_snapshot_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: snapshot=%o]", iname, self, priv->snapshot);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
