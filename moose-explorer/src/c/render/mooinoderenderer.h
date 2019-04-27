/*
   File:    mooinoderenderer.h
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

#ifndef MOOINODERENDERER_H_
#define MOOINODERENDERER_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

#define MOO_TYPE_INODE_RENDERER                 (moo_inode_renderer_get_type())
#define MOO_INODE_RENDERER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_INODE_RENDERER, MooINodeRenderer))
#define MOO_IS_INODE_RENDERER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_INODE_RENDERER))
#define MOO_INODE_RENDERER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_INODE_RENDERER, MooINodeRendererInterface))


typedef struct _MooINodeRenderer               MooINodeRenderer; /* dummy object */
typedef struct _MooINodeRendererInterface      MooINodeRendererInterface;

struct _MooNodeLayout;

struct _MooINodeRendererInterface {
	GTypeInterface parent_iface;

	void (*updateLayout)(MooINodeRenderer *self, struct _MooNodeLayout *node_layout);
	void (*paint)(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout);

};

GType moo_inode_renderer_get_type(void);


void moo_inode_renderer_update_layout(MooINodeRenderer *self, struct _MooNodeLayout *node_layout);

void moo_inode_renderer_paint(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout);



#endif /* MOOINODERENDERER_H_ */
