/*
   File:    leakeynodekey.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 30, 2013
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

#include "leakeynodekey.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaKeyNodeKey"
#include <logging/catlog.h>

struct _LeaKeyNodeKeyPrivate {
	LeaKey *key;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaKeyNodeKey, lea_key_node_key, LEA_TYPE_KEY_NODE, {
		G_ADD_PRIVATE(LeaKeyNodeKey)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static int l_node_hash(LeaKeyNode *self);
static gboolean l_node_equal(LeaKeyNode *node_a, LeaKeyNode *node_b);


static void lea_key_node_key_class_init(LeaKeyNodeKeyClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaKeyNodeClass *node_clazz = LEA_KEY_NODE_CLASS(clazz);
	node_clazz->hash = l_node_hash;
	node_clazz->equal = l_node_equal;
}

static void lea_key_node_key_init(LeaKeyNodeKey *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaKeyNodeKey *instance = LEA_KEY_NODE_KEY(object);
	LeaKeyNodeKeyPrivate *priv = lea_key_node_key_get_instance_private(instance);
	cat_unref_ptr(priv->key);
	G_OBJECT_CLASS(lea_key_node_key_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_key_node_key_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

LeaKeyNodeKey *lea_key_node_key_new(LeaKey *key) {
	LeaKeyNodeKey *result = g_object_new(LEA_TYPE_KEY_NODE_KEY, NULL);
	cat_ref_anounce(result);
	LeaKeyNodeKeyPrivate *priv = lea_key_node_key_get_instance_private(result);
	priv->key = cat_ref_ptr(key);
	lea_key_node_construct((LeaKeyNode *) result);
	return result;
}

static int l_node_hash(LeaKeyNode *self) {
	LeaKeyNodeKey *instance = LEA_KEY_NODE_KEY(self);
	LeaKeyNodeKeyPrivate *priv = lea_key_node_key_get_instance_private(instance);
	return lea_key_hashcode(priv->key);
}

static gboolean l_node_equal(LeaKeyNode *node_a, LeaKeyNode *node_b) {
	if (node_a==node_b) {
		return TRUE;
	}
	if (node_a==NULL || node_b==NULL) {
		return FALSE;
	}
	LeaKeyNodeKeyPrivate *priv_a = lea_key_node_key_get_instance_private(LEA_KEY_NODE_KEY(node_a));
	LeaKeyNodeKeyPrivate *priv_b = lea_key_node_key_get_instance_private(LEA_KEY_NODE_KEY(node_b));
	return lea_key_equal(priv_a->key, priv_b->key);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaKeyNodeKey *instance = LEA_KEY_NODE_KEY(self);
	LeaKeyNodeKeyPrivate *priv = lea_key_node_key_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%o]", iname, priv->key);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
