/*
   File:    moodefaultnoderenderer.h
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

#ifndef MOODEFAULTNODERENDERER_H_
#define MOODEFAULTNODERENDERER_H_

#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MOO_TYPE_DEFAULT_NODE_RENDERER              (moo_default_node_renderer_get_type())
#define MOO_DEFAULT_NODE_RENDERER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_default_node_renderer_get_type(), MooDefaultNodeRenderer))
#define MOO_DEFAULT_NODE_RENDERER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_DEFAULT_NODE_RENDERER, MooDefaultNodeRendererClass))
#define MOO_IS_DEFAULT_NODE_RENDERER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_DEFAULT_NODE_RENDERER))
#define MOO_IS_DEFAULT_NODE_RENDERER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_DEFAULT_NODE_RENDERER))
#define MOO_DEFAULT_NODE_RENDERER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_DEFAULT_NODE_RENDERER, MooDefaultNodeRendererClass))

typedef struct _MooDefaultNodeRenderer               MooDefaultNodeRenderer;
typedef struct _MooDefaultNodeRendererClass          MooDefaultNodeRendererClass;

struct _MooDefaultNodeRenderer {
	GObject parent;
};

struct _MooDefaultNodeRendererClass {
	GObjectClass parent_class;
};


GType moo_default_node_renderer_get_type();

MooDefaultNodeRenderer *moo_default_node_renderer_new();

void moo_default_node_renderer_draw_map(cairo_t *cairo, double xoffset, int yoffset, double size, gboolean as_opened, gboolean with_blue);

void moo_default_renderer_draw_file(cairo_t *cairo, double xoffset, int yoffset, double size);

G_END_DECLS

#endif /* MOODEFAULTNODERENDERER_H_ */
