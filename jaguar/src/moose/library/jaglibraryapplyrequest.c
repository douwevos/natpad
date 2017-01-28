/*
   File:    jaglibraryapplyrequest.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 22, 2014
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

#include "jaglibraryapplyrequest.h"
#include "jaglibrarycontentwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagLibraryApplyRequest"
#include <logging/catlog.h>

struct _JagLibraryApplyRequestPrivate {
	JagMooseService *moose_service;
	MooIdPath *id_path;
	MooNodeWo *node_wo_jar;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagLibraryApplyRequest, jag_library_apply_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(JagLibraryApplyRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_library_apply_request_class_init(JagLibraryApplyRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}

static void jag_library_apply_request_init(JagLibraryApplyRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagLibraryApplyRequest *instance = JAG_LIBRARY_APPLY_REQUEST(object);
	JagLibraryApplyRequestPrivate *priv = jag_library_apply_request_get_instance_private(instance);
	cat_unref_ptr(priv->id_path);
	cat_unref_ptr(priv->moose_service);
	cat_unref_ptr(priv->node_wo_jar);
	G_OBJECT_CLASS(jag_library_apply_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_library_apply_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagLibraryApplyRequest *jag_library_apply_request_new(JagMooseService *moose_service, MooIdPath *id_path, MooNodeWo *node_wo_jar) {
	JagLibraryApplyRequest *result = g_object_new(JAG_TYPE_LIBRARY_APPLY_REQUEST, NULL);
	cat_ref_anounce(result);
	JagLibraryApplyRequestPrivate *priv = jag_library_apply_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->moose_service = cat_ref_ptr(moose_service);
	priv->id_path = cat_ref_ptr(id_path);
	priv->node_wo_jar = cat_ref_ptr(node_wo_jar);
	return result;
}


static void l_run_request(WorRequest *request) {
	JagLibraryApplyRequestPrivate *priv = jag_library_apply_request_get_instance_private((JagLibraryApplyRequest *) request);

	MooService *moo_service = jag_moose_service_get_moo_service(priv->moose_service);
	MooTransaction *tx = moo_service_create_transaction((GObject *) request, moo_service);

	while(moo_transaction_retry(tx)) {
		moo_transaction_begin(tx);
		MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
		MooNodeWo *e_root_node = moo_node_wo_ensure_editable(root_node, NULL);

		MooNodeWo *e_jar_node = moo_id_path_create_editable_node(priv->id_path, e_root_node);
		if (e_jar_node== NULL) {
			cat_unref_ptr(e_root_node);
			break;
		}

		JagLibraryContentWo *library_content = (JagLibraryContentWo *) moo_node_wo_get_content(e_jar_node, jag_library_content_wo_key());
		JagLibraryContentWo *e_library_content = jag_library_content_wo_ensure_editable(library_content);
		if (!jag_library_content_wo_set_new_jar_node_version(e_library_content, priv->node_wo_jar)) {
			cat_unref_ptr(e_library_content);
			cat_unref_ptr(e_root_node);
			break;
		}

		moo_node_wo_set_content(e_jar_node, (MooIContent *) e_library_content);
		cat_unref_ptr(e_library_content);

		MooNodeListWo *e_children = moo_node_wo_get_editable_children(e_jar_node);
		moo_node_list_wo_clear(e_children);


		if (priv->node_wo_jar) {
			MooNodeWo *jar_content_node = moo_node_wo_child_at(priv->node_wo_jar, 0);
			int jar_child_count = moo_node_wo_child_count(jar_content_node);
			int jar_child_idx;
			for(jar_child_idx=0; jar_child_idx<jar_child_count; jar_child_idx++) {
				MooNodeWo *jar_child_node = moo_node_wo_child_at(jar_content_node, jar_child_idx);
				moo_node_list_wo_append(e_children, jar_child_node);
			}
		}

		gboolean tx_succes = moo_transaction_commit(tx, e_root_node);

		if (tx_succes) {
			cat_log_debug("done");

//			cat_log_on_trace({
//				MooNodeWo *c_root_node = moo_transaction_get_commited_root_node(tx);
//				CatStringWo *e_buf = cat_string_wo_new();
//				moo_node_wo_print(c_root_node, buf);
//				cat_log_print("DUMP", "%s", cat_string_wo_getchars(e_buf));
//			});
			break;
		}
	}
	cat_unref_ptr(tx);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/





