/*
   File:    mooinoderenderer.c
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




#include "mooinoderenderer.h"

G_DEFINE_INTERFACE(MooINodeRenderer, moo_inode_renderer, G_TYPE_OBJECT);

static void moo_inode_renderer_default_init (MooINodeRendererInterface *klass) {
}



void moo_inode_renderer_update_layout(MooINodeRenderer *self, struct _MooNodeLayout *node_layout) {
	g_return_if_fail(MOO_IS_INODE_RENDERER(self));
	return MOO_INODE_RENDERER_GET_INTERFACE(self)->updateLayout(self, node_layout);
}

void moo_inode_renderer_paint(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout) {
	g_return_if_fail(MOO_IS_INODE_RENDERER(self));
	return MOO_INODE_RENDERER_GET_INTERFACE(self)->paint(self, cairo, node_layout);
}


