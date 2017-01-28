/*
   File:    mooinoderendererfactory.c
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

#include "mooinoderendererfactory.h"

G_DEFINE_INTERFACE(MooINodeRendererFactory, moo_inode_renderer_factory, G_TYPE_OBJECT);

static void moo_inode_renderer_factory_default_init (MooINodeRendererFactoryInterface *klass) {
}


struct _MooINodeRenderer *moo_inode_renderer_factory_request_renderer_for_node(MooINodeRendererFactory *self, struct _MooNodeWo *node) {
	g_return_val_if_fail(MOO_IS_INODE_RENDERER_FACTORY(self), NULL);
	return MOO_INODE_RENDERER_FACTORY_GET_INTERFACE(self)->requestRendererForNode(self, node);
}
