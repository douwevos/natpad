/*
   File:    mooinoderendererfactory.h
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


#ifndef MOOINODERENDERERFACTORY_H_
#define MOOINODERENDERERFACTORY_H_


#include <caterpillar.h>

#define MOO_TYPE_INODE_RENDERER_FACTORY                 (moo_inode_renderer_factory_get_type())
#define MOO_INODE_RENDERER_FACTORY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_INODE_RENDERER_FACTORY, MooINodeRendererFactory))
#define MOO_IS_INODE_RENDERER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_INODE_RENDERER_FACTORY))
#define MOO_INODE_RENDERER_FACTORY_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_INODE_RENDERER_FACTORY, MooINodeRendererFactoryInterface))


typedef struct _MooINodeRendererFactory               MooINodeRendererFactory; /* dummy object */
typedef struct _MooINodeRendererFactoryInterface      MooINodeRendererFactoryInterface;


struct _MooNodeWo;
struct _MooINodeRenderer;

struct _MooINodeRendererFactoryInterface {
	GTypeInterface parent_iface;

	struct _MooINodeRenderer *(*requestRendererForNode)(MooINodeRendererFactory *self, struct _MooNodeWo *node);

};

GType moo_inode_renderer_factory_get_type(void);

struct _MooINodeRenderer *moo_inode_renderer_factory_request_renderer_for_node(MooINodeRendererFactory *self, struct _MooNodeWo *node);

#endif /* MOOINODERENDERERFACTORY_H_ */
