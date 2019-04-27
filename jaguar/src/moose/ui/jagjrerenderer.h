/*
   File:    jagjrerenderer.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef JAGJRERENDERER_H_
#define JAGJRERENDERER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_JRE_RENDERER              (jag_jre_renderer_get_type())
#define JAG_JRE_RENDERER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jre_renderer_get_type(), JagJreRenderer))
#define JAG_JRE_RENDERER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JRE_RENDERER, JagJreRendererClass))
#define JAG_IS_JRE_RENDERER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JRE_RENDERER))
#define JAG_IS_JRE_RENDERER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JRE_RENDERER))
#define JAG_JRE_RENDERER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JRE_RENDERER, JagJreRendererClass))

typedef struct _JagJreRenderer               JagJreRenderer;
typedef struct _JagJreRendererClass          JagJreRendererClass;

struct _JagJreRenderer {
	GObject parent;
};

struct _JagJreRendererClass {
	GObjectClass parent_class;
};

GType jag_jre_renderer_get_type();

JagJreRenderer *jag_jre_renderer_new();

G_END_DECLS

#endif /* JAGJRERENDERER_H_ */
