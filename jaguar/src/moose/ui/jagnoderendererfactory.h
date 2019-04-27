/*
   File:    jagnoderendererfactory.h
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

#ifndef JAGNODERENDERERFACTORY_H_
#define JAGNODERENDERERFACTORY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_NODE_RENDERER_FACTORY              (jag_node_renderer_factory_get_type())
#define JAG_NODE_RENDERER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_node_renderer_factory_get_type(), JagNodeRendererFactory))
#define JAG_NODE_RENDERER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_NODE_RENDERER_FACTORY, JagNodeRendererFactoryClass))
#define JAG_IS_NODE_RENDERER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_NODE_RENDERER_FACTORY))
#define JAG_IS_NODE_RENDERER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_NODE_RENDERER_FACTORY))
#define JAG_NODE_RENDERER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_NODE_RENDERER_FACTORY, JagNodeRendererFactoryClass))

typedef struct _JagNodeRendererFactory               JagNodeRendererFactory;
typedef struct _JagNodeRendererFactoryPrivate        JagNodeRendererFactoryPrivate;
typedef struct _JagNodeRendererFactoryClass          JagNodeRendererFactoryClass;

struct _JagNodeRendererFactory {
	GObject parent;
};

struct _JagNodeRendererFactoryClass {
	GObjectClass parent_class;
};

GType jag_node_renderer_factory_get_type();

JagNodeRendererFactory *jag_node_renderer_factory_new();

G_END_DECLS

#endif /* JAGNODERENDERERFACTORY_H_ */
