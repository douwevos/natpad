/*
   File:    vipnodenamematcher.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
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



#include "vipnodenamematcher.h"
#include "../vipnode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipNodeNameMatcher"
#include <logging/catlog.h>

struct _VipNodeNameMatcherPrivate {
	CatStringWo *name;
};

static void l_node_matcher_iface_init(VipINodeMatcherInterface *iface);
static void l_matcher_iface_init(CatIMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipNodeNameMatcher, vip_node_name_matcher, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(VIP_TYPE_INODE_MATCHER, l_node_matcher_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IMATCHER, l_matcher_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_node_name_matcher_class_init(VipNodeNameMatcherClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipNodeNameMatcherPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_node_name_matcher_init(VipNodeNameMatcher *instance) {
	VipNodeNameMatcherPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_NODE_NAME_MATCHER, VipNodeNameMatcherPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipNodeNameMatcher *instance = VIP_NODE_NAME_MATCHER(object);
	VipNodeNameMatcherPrivate *priv = instance->priv;
	cat_unref_ptr(priv->name);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

VipNodeNameMatcher *vip_node_name_matcher_new(CatStringWo *name) {
	VipNodeNameMatcher *result = g_object_new(VIP_TYPE_NODE_NAME_MATCHER, NULL);
	cat_ref_anounce(result);
	VipNodeNameMatcherPrivate *priv = result->priv;
	priv->name = cat_ref_ptr(name);
	return result;
}

void vip_node_name_matcher_set_match_name(VipNodeNameMatcher *matcher, CatStringWo *name) {
	VipNodeNameMatcherPrivate *priv = VIP_NODE_NAME_MATCHER_GET_PRIVATE(matcher);
	cat_ref_swap(priv->name, name);
}




/********************* start VipINodeMatcher implementation *********************/

static gboolean l_node_matcher_matches(VipINodeMatcher *self, struct _VipNode *node) {
	VipNodeNameMatcherPrivate *priv = VIP_NODE_NAME_MATCHER_GET_PRIVATE(self);
	CatStringWo *node_name = vip_node_get_name(node);
	return cat_string_wo_equal(priv->name, node_name);
}


static void l_node_matcher_iface_init(VipINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

static void l_matcher_iface_init(CatIMatcherInterface *iface) {
	iface->matches = (gboolean (*)(CatIMatcher *, GObject *)) l_node_matcher_matches;
}

/********************* end VipINodeMatcher implementation *********************/

