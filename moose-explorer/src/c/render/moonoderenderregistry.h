/*
   File:    moonoderenderregistry.h
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

#ifndef MOONODERENDERREGISTRY_H_
#define MOONODERENDERREGISTRY_H_

#include "mooinoderendererfactory.h"
#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_NODE_RENDER_REGISTRY              (moo_node_render_registry_get_type())
#define MOO_NODE_RENDER_REGISTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_node_render_registry_get_type(), MooNodeRenderRegistry))
#define MOO_NODE_RENDER_REGISTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_NODE_RENDER_REGISTRY, MooNodeRenderRegistryClass))
#define MOO_IS_NODE_RENDER_REGISTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_NODE_RENDER_REGISTRY))
#define MOO_IS_NODE_RENDER_REGISTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_NODE_RENDER_REGISTRY))
#define MOO_NODE_RENDER_REGISTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_NODE_RENDER_REGISTRY, MooNodeRenderRegistryClass))

typedef struct _MooNodeRenderRegistry               MooNodeRenderRegistry;
typedef struct _MooNodeRenderRegistryPrivate        MooNodeRenderRegistryPrivate;
typedef struct _MooNodeRenderRegistryClass          MooNodeRenderRegistryClass;

struct _MooNodeRenderRegistry {
	GObject parent;
};

struct _MooNodeRenderRegistryClass {
	GObjectClass parent_class;
};

GType moo_node_render_registry_get_type();

MooNodeRenderRegistry *moo_node_render_registry_new();

void moo_node_render_registry_add_render_factory(MooNodeRenderRegistry *registry, MooINodeRendererFactory *factory);

CatArrayWo *moo_node_render_registry_create_renderers_for_node(MooNodeRenderRegistry *registry, MooNodeWo *node);

G_END_DECLS

#endif /* MOONODERENDERREGISTRY_H_ */
