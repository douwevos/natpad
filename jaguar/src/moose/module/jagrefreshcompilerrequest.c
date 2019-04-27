/*
   File:    jagrefreshcompilerrequest.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#include "jagrefreshcompilerrequest.h"
#include "jagmodulecontentwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagRefreshCompilerRequest"
#include <logging/catlog.h>

struct _JagRefreshCompilerRequestPrivate {
	JagMooseService *jag_moose_service;
	MooIdPath *module_id_path;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagRefreshCompilerRequest, jag_refresh_compiler_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(JagRefreshCompilerRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_refresh_compiler_request_class_init(JagRefreshCompilerRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}

static void jag_refresh_compiler_request_init(JagRefreshCompilerRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagRefreshCompilerRequest *instance = JAG_REFRESH_COMPILER_REQUEST(object);
	JagRefreshCompilerRequestPrivate *priv = jag_refresh_compiler_request_get_instance_private(instance);
	cat_unref_ptr(priv->jag_moose_service);
	cat_unref_ptr(priv->module_id_path);
	G_OBJECT_CLASS(jag_refresh_compiler_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_refresh_compiler_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagRefreshCompilerRequest *jag_refresh_compiler_request_new(JagMooseService *jag_moose_service, MooIdPath *module_id_path) {
	JagRefreshCompilerRequest *result = g_object_new(JAG_TYPE_REFRESH_COMPILER_REQUEST, NULL);
	cat_ref_anounce(result);
	JagRefreshCompilerRequestPrivate *priv = jag_refresh_compiler_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->jag_moose_service = cat_ref_ptr(jag_moose_service);
	priv->module_id_path  = cat_ref_ptr(module_id_path);
	return result;
}

CatS l_s_txt_jre = CAT_S_DEF("JagJre");

static void l_run_request(WorRequest *request) {
	JagRefreshCompilerRequest *instance = JAG_REFRESH_COMPILER_REQUEST(request);
	JagRefreshCompilerRequestPrivate *priv = jag_refresh_compiler_request_get_instance_private(instance);
	MooService *moo_service = jag_moose_service_get_moo_service(priv->jag_moose_service);
	MooTransaction *tx = moo_service_create_transaction((GObject *) request, moo_service);
	while(moo_transaction_retry(tx)) {
		moo_transaction_begin(tx);
		MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
		MooNodeWo *e_root_node = moo_node_wo_ensure_editable(root_node, NULL);
		MooNodeWo *e_module_node = moo_id_path_create_editable_node(priv->module_id_path, e_root_node);
		if (e_module_node==NULL) {
			break;
		}

		MooNodeListWo *e_children = moo_node_wo_get_editable_children(e_module_node);

		JagModuleContentWo *jag_module_content = (JagModuleContentWo *) moo_node_wo_get_content(e_module_node, jag_module_content_wo_key());
		JagJreLink *jre_link = jag_module_content_wo_get_jre_link(jag_module_content);
		JgiJreEntry *jre_entry = NULL;
		MooNodeWo *i_node = NULL;
		MooNodeWo *jre_moo_node = NULL;
		if (jre_link) {
			jre_entry = jag_jre_link_get_entry_ref(jre_link);
			if (jre_entry) {
				jre_moo_node = jgi_jre_entry_get_moo_node_ref(jre_entry);
				if (jre_moo_node) {
					i_node = moo_node_wo_child_at(jre_moo_node, 0);
					if (i_node) {
						MooIdNodeMatcher *matcher = moo_id_node_matcher_new(moo_node_wo_get_unique_id(i_node));
						int idx = moo_node_wo_find_index(e_module_node, (MooINodeMatcher *) matcher, NULL, -1);
						if (idx>=0) {
							MooNodeWo *e_node = moo_node_wo_child_at(e_module_node, idx);
							// TODO should do version checking here ???
							if (e_node!=i_node) {
								moo_node_list_wo_set_at(e_children, i_node, idx);
							}
						} else {
							moo_node_wo_append_child(e_module_node, i_node);
						}
						cat_unref_ptr(matcher);
					}
				}
			}
		}

		int idx;
		for(idx=moo_node_list_wo_count(e_children)-1; idx>=0; idx--) {
			MooNodeWo *child = moo_node_list_wo_get_at(e_children, idx);
			if (child==i_node) {
				continue;
			}
			MooContentMapWo *content_map = moo_node_wo_get_content_map(child);
			GObject *xx = moo_content_map_wo_get_raw(content_map, CAT_S(l_s_txt_jre));
			if (xx) {
				moo_node_list_wo_remove(e_children, idx);
			}
		}

		cat_unref_ptr(jre_moo_node);
		cat_unref_ptr(jre_entry);

		if (moo_transaction_commit(tx, e_root_node)) {
			break;
		}
	}
	cat_unref_ptr(tx);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagRefreshCompilerRequest *instance = JAG_REFRESH_COMPILER_REQUEST(self);
	JagRefreshCompilerRequestPrivate *priv = jag_refresh_compiler_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p] path=%o", iname, self, priv->module_id_path);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
