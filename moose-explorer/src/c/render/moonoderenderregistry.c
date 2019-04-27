/*
   File:    moonoderenderregistry.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 25, 2013
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

#include "moonoderenderregistry.h"
#include "mooinoderenderer.h"
#include "moodefaultnoderenderer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooNodeRenderRegistry"
#include <logging/catlog.h>

struct _MooNodeRenderRegistryPrivate {
	CatArrayWo *e_factories;
	MooINodeRenderer *default_renderer;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooNodeRenderRegistry, moo_node_render_registry, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_node_render_registry_class_init(MooNodeRenderRegistryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_node_render_registry_init(MooNodeRenderRegistry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooNodeRenderRegistry *instance = MOO_NODE_RENDER_REGISTRY(object);
	MooNodeRenderRegistryPrivate *priv = moo_node_render_registry_get_instance_private(instance);
	cat_unref_ptr(priv->e_factories);
	cat_unref_ptr(priv->default_renderer);
	G_OBJECT_CLASS(moo_node_render_registry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_node_render_registry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooNodeRenderRegistry *moo_node_render_registry_new() {
	MooNodeRenderRegistry *result = g_object_new(MOO_TYPE_NODE_RENDER_REGISTRY, NULL);
	cat_ref_anounce(result);
	MooNodeRenderRegistryPrivate *priv = moo_node_render_registry_get_instance_private(result);
	priv->e_factories = cat_array_wo_new();
	priv->default_renderer = (MooINodeRenderer *) moo_default_node_renderer_new();
	return result;
}

void moo_node_render_registry_add_render_factory(MooNodeRenderRegistry *registry, MooINodeRendererFactory *factory) {
	MooNodeRenderRegistryPrivate *priv = moo_node_render_registry_get_instance_private(registry);
	cat_array_wo_append(priv->e_factories, (GObject *) factory);
}

CatArrayWo *moo_node_render_registry_create_renderers_for_node(MooNodeRenderRegistry *registry, MooNodeWo *node) {
	MooNodeRenderRegistryPrivate *priv = moo_node_render_registry_get_instance_private(registry);
	CatArrayWo *result = cat_array_wo_new();

	CatIIterator *iter = cat_array_wo_iterator(priv->e_factories);
	while(cat_iiterator_has_next(iter)) {
		MooINodeRendererFactory *factory = cat_iiterator_next(iter);
		MooINodeRenderer *renderer = moo_inode_renderer_factory_request_renderer_for_node(factory, node);
		if (renderer!=NULL) {
			cat_array_wo_append(result, (GObject *) renderer);
		}
	}
	cat_unref_ptr(iter);

	if (cat_array_wo_size(result) == 0) {
		cat_array_wo_append(result, (GObject *) priv->default_renderer);
	}
	return result;
}
