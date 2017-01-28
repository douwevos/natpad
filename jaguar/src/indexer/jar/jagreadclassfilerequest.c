/*
   File:    jagreadclassfilerequest.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 28, 2014
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

#include "jagreadclassfilerequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagReadClassfileRequest"
#include <logging/catlog.h>

struct _JagReadClassfileRequestPrivate {
	JgiJarEntry *jar_entry;
	MooNodeWo *node;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagReadClassfileRequest, jag_read_classfile_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(JagReadClassfileRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_read_classfile_request_class_init(JagReadClassfileRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void jag_read_classfile_request_init(JagReadClassfileRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagReadClassfileRequest *instance = JAG_READ_CLASSFILE_REQUEST(object);
	JagReadClassfileRequestPrivate *priv = jag_read_classfile_request_get_instance_private(instance);
	cat_unref_ptr(priv->jar_entry);
	cat_unref_ptr(priv->node);
	G_OBJECT_CLASS(jag_read_classfile_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_read_classfile_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagReadClassfileRequest *jag_read_classfile_request_new(JgiJarEntry *jar_entry, MooNodeWo *node) {
	JagReadClassfileRequest *result = g_object_new(JAG_TYPE_READ_CLASSFILE_REQUEST, NULL);
	cat_ref_anounce(result);
	JagReadClassfileRequestPrivate *priv = jag_read_classfile_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->jar_entry = cat_ref_ptr(jar_entry);
	priv->node = cat_ref_ptr(node);
	return result;
}


static void l_run_request(WorRequest *request) {
//	JagReadClassfileRequestPrivate *priv = jag_read_classfile_request_get_instance_private(request);
//
//	if (moo_node_wo_child_count(priv->node)>0) {
//		return;
//	}


//	MooTransaction *tx = moo_service_create_transaction(CAT_LOG_CLAZZ, priv->moo_service);
//	moo_transaction_begin(tx);
//	MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
//	moo_node_wo_find_path()

//	moo_node_wo_get_content(priv->node, moo_node_wo

}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagReadClassfileRequest *instance = JAG_READ_CLASSFILE_REQUEST(self);
	JagReadClassfileRequestPrivate *priv = jag_read_classfile_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p] node=%o", iname, self, priv->node);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
