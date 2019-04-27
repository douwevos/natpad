/*
   File:    vipnodeidmatcher.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 20, 2013
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

#include "vipnodeidmatcher.h"
#include "../vipnode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipNodeIdMatcher"
#include <logging/catlog.h>

struct _VipNodeIdMatcherPrivate {
	long long viper_id;
};

static void l_node_matcher_iface_init(VipINodeMatcherInterface *iface);
static void l_matcher_iface_init(CatIMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipNodeIdMatcher, vip_node_id_matcher, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(VipNodeIdMatcher)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_INODE_MATCHER, l_node_matcher_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IMATCHER, l_matcher_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_node_id_matcher_class_init(VipNodeIdMatcherClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_node_id_matcher_init(VipNodeIdMatcher *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(vip_node_id_matcher_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_node_id_matcher_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipNodeIdMatcher *vip_node_id_matcher_new(long long viper_id) {
	VipNodeIdMatcher *result = g_object_new(VIP_TYPE_NODE_ID_MATCHER, NULL);
	cat_ref_anounce(result);
	VipNodeIdMatcherPrivate *priv = vip_node_id_matcher_get_instance_private(result);
	priv->viper_id = viper_id;
	return result;
}

void vip_node_id_matcher_set_id(VipNodeIdMatcher *matcher, long long viper_id) {
	VipNodeIdMatcherPrivate *priv = vip_node_id_matcher_get_instance_private(matcher);
	priv->viper_id = viper_id;
}

/********************* start VipINodeMatcher implementation *********************/

static gboolean l_node_matcher_matches(VipINodeMatcher *self, struct _VipNode *node) {
	VipNodeIdMatcher *instance = VIP_NODE_ID_MATCHER(self);
	VipNodeIdMatcherPrivate *priv = vip_node_id_matcher_get_instance_private(instance);
	long long node_id = vip_node_get_id(node);
	return priv->viper_id == node_id;
}

static void l_node_matcher_iface_init(VipINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

static void l_matcher_iface_init(CatIMatcherInterface *iface) {
	iface->matches = (gboolean (*)(CatIMatcher *, GObject *)) l_node_matcher_matches;
}

/********************* end VipINodeMatcher implementation *********************/
