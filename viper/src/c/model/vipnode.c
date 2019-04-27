/*
   File:    vipnode.c
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 13, 2013
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

#include "vipnode.h"
#include "access/vipnodenamematcher.h"
#include "access/vipnodeidmatcher.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipNode"
#include <logging/catlog.h>

struct _VipNodePrivate {
	long long viper_id;
	VipIResource *resource;

	CatAtomicInteger *hold_counter;
	CatAtomicInteger *recursive_counter;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipNode, vip_node, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(VipNode)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static VipNode *l_clone(VipNode *toclone);

static void vip_node_class_init(VipNodeClass *clazz) {
	clazz->clone = l_clone;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_node_init(VipNode *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipNode *instance = VIP_NODE(object);
	VipNodePrivate *priv = vip_node_get_instance_private(instance);
	cat_unref_ptr(priv->resource);
	cat_unref_ptr(priv->hold_counter);
	cat_unref_ptr(priv->recursive_counter);
	G_OBJECT_CLASS(vip_node_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_node_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipNode *vip_node_new(long long viper_id, VipIResource *resource) {
	cat_log_trace("new:viper_id=%ld, content=%o", viper_id, resource);
	VipNode *result = g_object_new(VIP_TYPE_NODE, NULL);
	cat_ref_anounce(result);
	vip_node_construct(result, viper_id, resource, NULL, NULL);
	return result;
}

VipNode *vip_node_new_full(long long viper_id, VipIResource *resource, CatAtomicInteger *hold_counter, CatAtomicInteger *recursive_counter) {
	cat_log_trace("new:viper_id=%ld, content=%o", viper_id, resource);
	VipNode *result = g_object_new(VIP_TYPE_NODE, NULL);
	cat_ref_anounce(result);
	vip_node_construct(result, viper_id, resource, hold_counter, recursive_counter);
	return result;
}

void vip_node_construct(VipNode *node, long long viperId, VipIResource *resource, CatAtomicInteger *hold_counter, CatAtomicInteger *recursive_counter) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	priv->viper_id = viperId;
	cat_log_detail("node=%p, viperId=%ld, resource=%o", node, viperId, resource);
	priv->resource = cat_ref_ptr(resource);
	priv->hold_counter = hold_counter==NULL ? cat_atomic_integer_new() : cat_ref_ptr(hold_counter);
	priv->recursive_counter = recursive_counter==NULL ? cat_atomic_integer_new() : cat_ref_ptr(recursive_counter);
}


VipNode *vip_node_clone(VipNode *toclone) {
	VipNodeClass *cl = VIP_NODE_GET_CLASS(toclone);
	return cl->clone(toclone);
}

static VipNode *l_clone(VipNode *toclone) {
	VipNode *result = g_object_new(VIP_TYPE_NODE, NULL);
	cat_ref_anounce(result);
	VipNodePrivate *priv = vip_node_get_instance_private(result);
	VipNodePrivate *spriv = vip_node_get_instance_private(toclone);
	priv->viper_id = spriv->viper_id;
	priv->resource = cat_ref_ptr(spriv->resource);
	priv->hold_counter = cat_ref_ptr(spriv->hold_counter);
	priv->recursive_counter = cat_ref_ptr(spriv->recursive_counter);
	return result;
}

long long vip_node_get_id(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	return priv->viper_id;
}

CatStringWo *vip_node_get_name(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	return priv->resource==NULL ? NULL : vip_iresource_get_name(priv->resource);
}

VipIResource *vip_node_get_content(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	return priv->resource;
}

VipNode *vip_node_set_resource(VipNode *node, VipIResource *new_resource) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	if (priv->resource==new_resource) {
		return cat_ref_ptr(node);
	}
	VipNode *result = vip_node_clone(node);
	VipNodePrivate *rpriv = vip_node_get_instance_private(result);
	cat_ref_swap(rpriv->resource, new_resource);
	return result;
}


CatAtomicInteger *vip_node_get_hold_counter(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	return priv->hold_counter;
}

void vip_node_hold_down(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	cat_atomic_integer_decrement(priv->hold_counter);
}

void vip_node_hold_up(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	cat_atomic_integer_increment(priv->hold_counter);
}

gboolean vip_node_should_hold(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	return cat_atomic_integer_get(priv->hold_counter)>0;
}

CatAtomicInteger *vip_node_get_recursive_counter(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	return priv->recursive_counter;
}

void vip_node_recursive_down(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	cat_atomic_integer_decrement(priv->recursive_counter);
}

void vip_node_recursive_up(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	cat_atomic_integer_increment(priv->recursive_counter);
}

gboolean vip_node_should_scan_recursive(VipNode *node) {
	VipNodePrivate *priv = vip_node_get_instance_private(node);
	return cat_atomic_integer_get(priv->recursive_counter)>0;
}

void vip_node_print(VipNode *node, CatStringWo *indent, CatStringWo *e_buf) {
	cat_string_wo_append(e_buf, indent);
	cat_istringable_print((CatIStringable *) node, e_buf);
	cat_string_wo_append_chars(e_buf, "\n");
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipNode *instance = VIP_NODE(self);
	VipNodePrivate *priv = vip_node_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:vip-id=%ld, content=%o, hold=%d, recursive=%d]", iname, self, priv->viper_id,
			priv->resource, cat_atomic_integer_get(priv->hold_counter), cat_atomic_integer_get(priv->recursive_counter));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
