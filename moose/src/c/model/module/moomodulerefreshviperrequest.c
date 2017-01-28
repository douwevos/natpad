/*
   File:    moomodulerefreshviperrequest.c
   Project: moose
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

#include "moomodulerefreshviperrequest.h"
#include "../module/moomodulecontentwo.h"
#include "../moonodewo.h"
#include "../mooresourcecontentwo.h"
#include "../access/mooinodematcher.h"
#include "../access/mooidnodematcher.h"
#include "../access/mooresourcenamematcher.h"
#include "../../viper/moomodulework.h"
#include "../../viper/mooinodework.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooModuleRefreshViperRequest"
#include <logging/catlog.h>

struct _MooModuleRefreshViperRequestPrivate {
	MooService *moo_service;
	long long module_id;
	gboolean force_refresh;
};


G_DEFINE_TYPE(MooModuleRefreshViperRequest, moo_module_refresh_viper_request, WOR_TYPE_REQUEST)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void moo_module_refresh_viper_request_class_init(MooModuleRefreshViperRequestClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooModuleRefreshViperRequestPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *req_class = WOR_REQUEST_CLASS(clazz);
	req_class->runRequest = l_run_request;

}

static void moo_module_refresh_viper_request_init(MooModuleRefreshViperRequest *instance) {
	MooModuleRefreshViperRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_MODULE_REFRESH_VIPER_REQUEST, MooModuleRefreshViperRequestPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooModuleRefreshViperRequest *instance = MOO_MODULE_REFRESH_VIPER_REQUEST(object);
	MooModuleRefreshViperRequestPrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooModuleRefreshViperRequest *moo_module_refresh_viper_request_new(MooService *moo_service, long long module_id, gboolean force_refresh) {
	MooModuleRefreshViperRequest *result = g_object_new(MOO_TYPE_MODULE_REFRESH_VIPER_REQUEST, NULL);
	cat_ref_anounce(result);
	MooModuleRefreshViperRequestPrivate *priv = result->priv;
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->module_id = module_id;
	priv->force_refresh = force_refresh;
	wor_request_construct((WorRequest *) result);
	return result;
}



static void l_refresh_module_settings(MooModuleContentWo *e_module_content, MooNodeWo *e_module_node);




static void l_run_request(WorRequest *request) {
	MooModuleRefreshViperRequestPrivate *priv = MOO_MODULE_REFRESH_VIPER_REQUEST_GET_PRIVATE(request);
	cat_log_debug("starting:%o, forced=%d", request, priv->force_refresh);

	MooTransaction *transaction = moo_service_create_transaction((GObject *) request, priv->moo_service);

	MooIdNodeMatcher *node_id_matcher = moo_id_node_matcher_new(priv->module_id);

	int  retryCnt = 0;
	while(retryCnt++<10) {	// TODO: report error when we didn't succeed after 10 retries
		moo_transaction_begin(transaction);
		MooNodeWo *tx_root_node = moo_transaction_get_tx_root_node(transaction);
		int module_node_index = moo_node_wo_find_index(tx_root_node, (MooINodeMatcher *) node_id_matcher, NULL, -1);
		if (module_node_index<0) {
			cat_log_debug("module was removed:moduleId=%d", priv->module_id);
			/* the module does not exist anymore ... so were done */
			break;
		}

		MooNodeWo *module_node = moo_node_wo_child_at(tx_root_node, module_node_index);
		MooModuleContentWo *module_content = (MooModuleContentWo *) moo_node_wo_get_content(module_node, moo_module_content_wo_key());
		cat_log_debug("module_content=%o", module_content);
		if (module_content==NULL) {
			/* this module is erronous ... were done */
			break;
		}


		if (!priv->force_refresh && !moo_module_content_wo_should_refresh_viper_info(module_content)) {
			/* apparently it was already refreshed */
			break;
		}


		MooNodeWo *editable_root_node = moo_node_wo_ensure_editable(tx_root_node, NULL);
		MooNodeWo *e_module = moo_node_wo_get_editable_child_at(editable_root_node, module_node_index);
		cat_log_debug("e_module=%o", e_module);
		MooModuleContentWo *e_module_content = moo_module_content_wo_ensure_editable(module_content);
		if (module_content!=e_module_content) {
			moo_node_wo_set_content(e_module, (MooIContent *) e_module_content);
		}
		cat_unref(e_module_content);


		CatReadableTreeNode *base_node = moo_module_content_wo_get_base_node(e_module_content);
		cat_log_debug("base_node=%o", base_node);
		MooModuleWork *moduleWork = moo_module_work_new(VIP_ISEQUENCE(priv->moo_service), base_node, e_module);
		CatArrayWo /* <MooINodeWork> */ *e_workList = cat_array_wo_new();
		cat_array_wo_append(e_workList, (GObject *) moduleWork);
		cat_unref_ptr(moduleWork);
		while(cat_array_wo_size(e_workList)>0) {
			MooINodeWork *work = NULL;
			cat_array_wo_remove_last(e_workList, (GObject **) &work);
//			cat_log_trace("doing work : %o is-work:%d", work, MOO_IS_INODE_WORK(work));
			moo_inode_work_do_work(work, e_workList);
			cat_unref_ptr(work);
		}
		cat_unref_ptr(e_workList);


		l_refresh_module_settings(e_module_content, e_module);
		moo_module_content_wo_set_refresh_viper_info(e_module_content, FALSE);

//		CatStringWo *e_s = cat_string_wo_new();
//		moo_node_wo_print(new_root, s);
//		cat_log_print("DUMP","%o", s);
		gboolean commit_ok = moo_transaction_commit(transaction, editable_root_node);
		if (commit_ok) {
			cat_log_debug("commit succeeded");
			break;
		} else {
			cat_log_debug("failed commit");
		}
	}
	cat_unref_ptr(node_id_matcher);
	cat_unref_ptr(transaction);
}


CatS l_s_txt_settings_fname = CAT_S_DEF("natpad-project.sht");

static void l_refresh_module_settings(MooModuleContentWo *e_module_content, MooNodeWo *e_module_node) {
	MooResourceNameMatcher *name_matcher = moo_resource_name_matcher_new();
	int settingsFileIndex = moo_node_wo_find_index(e_module_node, (MooINodeMatcher *) name_matcher, CAT_S(l_s_txt_settings_fname), -1);
	cat_unref_ptr(name_matcher);
	VipNode *settingsVipNode = NULL;
	if (settingsFileIndex>=0) {
		MooNodeWo *childAt = moo_node_wo_child_at(e_module_node, settingsFileIndex);
		MooResourceContentWo *settingsResContent = (MooResourceContentWo *) moo_node_wo_get_content(childAt, moo_resource_content_wo_key());
		settingsVipNode = moo_resource_content_wo_get_viper_node(settingsResContent);
	}

	moo_module_content_wo_set_settings_info(e_module_content, settingsFileIndex, settingsVipNode);
}
