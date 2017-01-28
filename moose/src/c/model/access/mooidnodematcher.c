/*
   File:    mooidnodematcher.c
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 6, 2013
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



#include "mooidnodematcher.h"
#include "mooinodematcher.h"
#include "../moonodewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooIdNodeMatcher"
#include <logging/catlog.h>

struct _MooIdNodeMatcherPrivate {
	long long id;
};

static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooIdNodeMatcher, moo_id_node_matcher, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_MATCHER, l_node_matcher_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_id_node_matcher_class_init(MooIdNodeMatcherClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooIdNodeMatcherPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_id_node_matcher_init(MooIdNodeMatcher *instance) {
	MooIdNodeMatcherPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_ID_NODE_MATCHER, MooIdNodeMatcherPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooIdNodeMatcher *moo_id_node_matcher_new(long long id) {
	MooIdNodeMatcher *result = g_object_new(MOO_TYPE_ID_NODE_MATCHER, NULL);
	cat_ref_anounce(result);
	MooIdNodeMatcherPrivate *priv = result->priv;
	priv->id = id;
	return result;
}


void moo_id_node_matcher_set_id(MooIdNodeMatcher *node_matcher, long long id) {
	MooIdNodeMatcherPrivate *priv = MOO_ID_NODE_MATCHER_GET_PRIVATE(node_matcher);
	priv->id = id;
}




/********************* start MooIContentMap implementation *********************/


static gboolean l_node_matcher_matches(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer userdata) {
	MooIdNodeMatcherPrivate *priv = MOO_ID_NODE_MATCHER_GET_PRIVATE(self);
	return moo_node_wo_get_unique_id(node) == priv->id;
}


static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

/********************* end MooIContentMap implementation *********************/
