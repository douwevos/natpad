/*
   File:    leakeynode.c
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

#include "leakeynode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaKeyNode"
#include <logging/catlog.h>

struct _LeaKeyNodePrivate {
	CatHashSet *children;
	LeaIKeyBinding *key_binding;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaKeyNode, lea_key_node, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(LeaKeyNode)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_key_node_class_init(LeaKeyNodeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_key_node_init(LeaKeyNode *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaKeyNode *instance = LEA_KEY_NODE(object);
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(instance);
	cat_unref_ptr(priv->children);
	cat_unref_ptr(priv->key_binding);
	G_OBJECT_CLASS(lea_key_node_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_key_node_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void lea_key_node_construct(LeaKeyNode *key_node) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(key_node);
	priv->children = cat_hash_set_new((GHashFunc) lea_key_node_hash, (GEqualFunc) lea_key_node_equal);
	priv->key_binding = NULL;
}


LeaKeyNode *lea_key_node_new() {
	LeaKeyNode *result = g_object_new(LEA_TYPE_KEY_NODE, NULL);
	cat_ref_anounce(result);
	lea_key_node_construct(result);
	return result;
}

LeaKeyNode *lea_key_node_add(LeaKeyNode *node, LeaKeyNode *child_node) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	cat_log_debug("add:child_node=%o", child_node);
	LeaKeyNode *existing_node = (LeaKeyNode *) cat_hash_set_add(priv->children, (GObject *) child_node);
	return existing_node ? existing_node : child_node;
}

void lea_key_node_remove(LeaKeyNode *node, LeaKeyNode *child_node) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	GObject *result = cat_hash_set_remove(priv->children, (GObject *) child_node);
	cat_unref_ptr(result);
}


LeaKeyNode *lea_key_node_get(LeaKeyNode *node, LeaKeyNode *child_node) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	return (LeaKeyNode *) cat_hash_set_get(priv->children, (GObject *) child_node);
}

gboolean lea_key_node_is_shallow(LeaKeyNode *node) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	return (cat_hash_set_size(priv->children)==0)
			&& priv->key_binding==NULL;
}


gboolean lea_key_node_has_children(LeaKeyNode *node) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	return cat_hash_set_size(priv->children)>0;
}

void lea_key_node_enlist_children(LeaKeyNode *node, CatArrayWo *e_enlistto) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	CatArrayWo *e_result = cat_hash_set_enlist_all(priv->children);
	cat_array_wo_append_all(e_enlistto, e_result);
	cat_unref_ptr(e_result);
}

void lea_key_node_set_key_binding(LeaKeyNode *node, LeaIKeyBinding *key_binding) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	cat_ref_swap(priv->key_binding, key_binding);
}

LeaIKeyBinding *lea_key_node_get_key_binding(LeaKeyNode *node) {
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	return priv->key_binding;
}

int lea_key_node_hash(LeaKeyNode *node) {
	LeaKeyNodeClass *clazz = LEA_KEY_NODE_GET_CLASS(node);
	int result = clazz->hash(node);
	cat_log_debug("hash:%s - %p, val=%d", g_type_name_from_instance((GTypeInstance *) node), node, result);
	return result;
}

gboolean lea_key_node_equal(LeaKeyNode *node_a, LeaKeyNode *node_b) {
	if (node_a==node_b) {
		return TRUE;
	}
	if (node_a==NULL || node_b==NULL) {
		return FALSE;
	}
	LeaKeyNodeClass *clazz_a = LEA_KEY_NODE_GET_CLASS(node_a);
	LeaKeyNodeClass *clazz_b = LEA_KEY_NODE_GET_CLASS(node_b);

	cat_log_debug("clza=%p, clzb=%p", clazz_a, clazz_b);

	return clazz_a==clazz_b && clazz_a->equal(node_a, node_b);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaKeyNode *node = LEA_KEY_NODE(self);
	LeaKeyNodePrivate *priv = lea_key_node_get_instance_private(node);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%o]", iname, priv->children);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
