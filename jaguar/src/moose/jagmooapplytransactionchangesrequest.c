/*
   File:    jagmooapplytransactionchangesrequest.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 29, 2013
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

#include "jagmooapplytransactionchangesrequest.h"
#include "jagrefreshmodulerequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagMooApplyTransactionChangesRequest"
#include <logging/catlog.h>

struct _JagMooApplyTransactionChangesRequestPrivate {
	JagMooseService *jag_moose_service;
};

static void l_mergeable_request_iface_init(WorIMergeableRequestInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagMooApplyTransactionChangesRequest, jag_moo_apply_transaction_changes_request, WOR_TYPE_REQUEST, {
		G_ADD_PRIVATE(JagMooApplyTransactionChangesRequest)
		G_IMPLEMENT_INTERFACE(WOR_TYPE_IMERGEABLE_REQUEST, l_mergeable_request_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);


static void jag_moo_apply_transaction_changes_request_class_init(JagMooApplyTransactionChangesRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}

static void jag_moo_apply_transaction_changes_request_init(JagMooApplyTransactionChangesRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagMooApplyTransactionChangesRequest *instance = JAG_MOO_APPLY_TRANSACTION_CHANGES_REQUEST(object);
	JagMooApplyTransactionChangesRequestPrivate *priv = jag_moo_apply_transaction_changes_request_get_instance_private(instance);
	cat_unref_ptr(priv->jag_moose_service);
	G_OBJECT_CLASS(jag_moo_apply_transaction_changes_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_moo_apply_transaction_changes_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagMooApplyTransactionChangesRequest *jag_moo_apply_transaction_changes_request_new(JagMooseService *jag_moose_service) {
	JagMooApplyTransactionChangesRequest *result = g_object_new(JAG_TYPE_MOO_APPLY_TRANSACTION_CHANGES_REQUEST, NULL);
	cat_ref_anounce(result);
	JagMooApplyTransactionChangesRequestPrivate *priv = jag_moo_apply_transaction_changes_request_get_instance_private(result);
	priv->jag_moose_service = cat_ref_ptr(jag_moose_service);
	wor_request_construct((WorRequest *) result);
	wor_request_set_time_out((WorRequest *) result, cat_date_current_time()+25);
	return result;
}

static void l_run_request(WorRequest *request) {
	JagMooApplyTransactionChangesRequest *instance = JAG_MOO_APPLY_TRANSACTION_CHANGES_REQUEST(request);
	JagMooApplyTransactionChangesRequestPrivate *priv = jag_moo_apply_transaction_changes_request_get_instance_private(instance);
	cat_log_debug("RUNNING !!!");

	MooService *moo_service= jag_moose_service_get_moo_service(priv->jag_moose_service);
	MooTransaction *transaction = moo_service_create_transaction((GObject *) request, moo_service);
	moo_transaction_begin(transaction);


	MooNodeWo *rootNode = moo_transaction_get_tx_root_node(transaction);
	cat_log_debug("rootNode=%o", rootNode);

	int idx;
	for(idx=0; idx<moo_node_wo_child_count((MooNodeWo *) rootNode); idx++) {
		MooNodeWo *childNode = moo_node_wo_child_at((MooNodeWo *) rootNode, idx);
		MooModuleContentWo *moduleContent = (MooModuleContentWo *) moo_node_wo_get_content(childNode, moo_module_content_wo_key());

		if (moduleContent!=NULL) {

			WorService *worm_service = jag_moose_service_get_worm_service(priv->jag_moose_service);

			cat_log_debug("refreshing module:%o", moduleContent);
			JagRefreshModuleRequest *refreshModule = jag_refresh_module_request_new(priv->jag_moose_service, moo_node_wo_get_unique_id(childNode), idx);
			wor_service_post_request(worm_service, (WorRequest *) refreshModule);
			cat_unref_ptr(refreshModule);
		}
	}

	cat_unref_ptr(transaction);

//	MooEditableNode *editableRootNode = moo_node_wo_ensure_editable(moo_transaction_get_tx_root_node(transaction));
//
//	int count = moo_node_wo_child_count(editableRootNode);
//
//	MooNamePath mainPath = moo_node_wo_get_name()MonIdPath(new long[0]);
//	int idx;
//	for(idx=0; idx<count; idx++) {
//
//		MooNodeWo *childAt = moo_node_wo_child_at(editableRootNode, idx);
//
//		if (MOO_IS_MODULE(childAt)) {
//			MooModule *module = (MooModule *) childAt;
//			MooEditableModule *editableModule = (MooEditableModule *) moo_node_wo_get_editable_child_at(editableRootNode, idx);
//
//
//			MonIdPath modulePath = mainPath.addId(module.getUniqueId(), idx);
//
//
////			MooModuleContext context = module.getModuleContext();
////			JagModuleContext jagModuleContext = (JagModuleContext) context.getSubContext("Jaguar");
////			if (jagModuleContext==null) {
////				jagModuleContext = new JagModuleContext();
////			}
////
////			jagModuleContext.reconfigure(context.getModel());
////
////
////			JagModuleSettingsWo moduleSettings = jagModuleContext.getModuleSettings();
////			moduleSettings.getSourceFolders();
//
//
//
//			/* source folders */
//			long srcMainJavaPathIds[] = new long[3];
//			int[] srcMainJavaPath = findNodePath(module, new String[] { "src", "main", "java" }, srcMainJavaPathIds);
//			int foundIndex = module.findIndex(new JagSrcFolderMatcher("src/main/java"));
//			cat_log_debug("srcMainJavaPath="+srcMainJavaPath+", foundIndex="+foundIndex);
//			if (srcMainJavaPath!=null) {
//				MonIdPath srcFolderPath = modulePath.addMultipleIds(srcMainJavaPathIds);
//				JagNodeSrcFolder nodeSrcFolder = null;
//				MooEditableNode editableSrcFolder = null;
//				if (foundIndex>=0) {
//					nodeSrcFolder = (JagNodeSrcFolder) module.childAt(foundIndex);
//					editableSrcFolder = editableModule.getEditableChild(foundIndex);
//				} else {
//					nodeSrcFolder = new JagNodeSrcFolder(transaction.uniqueIdCounter.incrementAndGet(), "src/main/java");
//					editableSrcFolder = editableModule.addNewChild(nodeSrcFolder);
//					cat_log_debug("added new child");
//				}
//
//
//				JagRequestUpdateSrcFolder srcFolderReq = jag_request_update_src_folder(priv->moo_service, editableRootNode, editableSrcFolder, srcFolderPath);
//				srcFolderReq.runRequest();
//
//
//			} else {
//				if (foundIndex>=0) {
//					MooEditableNode toDelete = editableModule.getEditableChild(foundIndex);
//					toDelete.setResultNode(null);
//				}
//			}
//
//
//
//			/* compiler */
//			foundIndex = module.findIndex(new JagCompilerMatcher());
//			JagNodeCompiler nodeCompiler = null;
//			MooEditableNode editableCompiler = null;
//			if (foundIndex>=0) {
//				nodeCompiler = (JagNodeCompiler) module.childAt(foundIndex);
//				editableCompiler = editableModule.getEditableChild(foundIndex);
//			} else {
//				nodeCompiler = compilerMap.getCompilerNode("java");
//				if (nodeCompiler!=null) {
////					nodeCompiler = new JagNodeCompiler(transaction.uniqueIdCounter.incrementAndGet());
//					editableCompiler = editableModule.addNewChild(nodeCompiler);
//					cat_log_debug("added new child");
//				}
//			}
//
//		}
//	}
//
//
////		int findIndex = editableRootNode.findIndex(new JagCompilerMatcher());
////		if (findIndex<0) {
////			editableRootNode.addNewChild(compilerMap.getCompilerNode("java"));
////		}
//
//	MooNode finalizedRootNode = editableRootNode.finalizeResult();
//
//	cat_log_debug("finalizedRootNode="+finalizedRootNode+", editableRootNode="+transaction.root);
//
//	transaction.commitEditable(editableRootNode);
}

static WorMergeResult l_try_merge(WorIMergeableRequest *new_request, WorIMergeableRequest *in_queue) {
//	JagRefreshSrcFolderRequestPrivate *priv = JAG_MOO_APPLY_TRANSACTION_CHANGES_REQUEST_GET_PRIVATE(new_request);
	if (JAG_IS_MOO_APPLY_TRANSACTION_CHANGES_REQUEST(in_queue)) {
		cat_log_debug("Merged !!!!!!!!!!!");
		return WOR_MERGE_KEEP_NEW;
	}
	cat_log_debug("Not Merged !!!!!!!!!!!");
	return WOR_MERGE_FAILED;
}

static void l_mergeable_request_iface_init(WorIMergeableRequestInterface *iface) {
	iface->tryMerge = l_try_merge;
}
