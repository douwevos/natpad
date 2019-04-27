/*
   File:    jagjarnoderenderer.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 6, 2013
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

#ifndef JAGJARNODERENDERER_H_
#define JAGJARNODERENDERER_H_

#include <caterpillar.h>
#include <moose.h>
#include <mooseexplorer.h>

G_BEGIN_DECLS

#define JAG_TYPE_JAR_NODE_RENDERER              (jag_jar_node_renderer_get_type())
#define JAG_JAR_NODE_RENDERER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jar_node_renderer_get_type(), JagJarNodeRenderer))
#define JAG_JAR_NODE_RENDERER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JAR_NODE_RENDERER, JagJarNodeRendererClass))
#define JAG_IS_JAR_NODE_RENDERER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JAR_NODE_RENDERER))
#define JAG_IS_JAR_NODE_RENDERER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JAR_NODE_RENDERER))
#define JAG_JAR_NODE_RENDERER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JAR_NODE_RENDERER, JagJarNodeRendererClass))

typedef struct _JagJarNodeRenderer               JagJarNodeRenderer;
typedef struct _JagJarNodeRendererClass          JagJarNodeRendererClass;

struct _JagJarNodeRenderer {
	GObject parent;
};

struct _JagJarNodeRendererClass {
	GObjectClass parent_class;
};

GType jag_jar_node_renderer_get_type();

JagJarNodeRenderer *jag_jar_node_renderer_new();

void jag_jar_renderer_draw_jar(cairo_t *cairo, double xoffset, int yoffset, double size, gboolean with_blue);

G_END_DECLS

#endif /* JAGJARNODERENDERER_H_ */
