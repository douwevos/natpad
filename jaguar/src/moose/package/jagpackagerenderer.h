/*
   File:    jagpackagerenderer.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 23, 2013
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

#ifndef JAGPACKAGERENDERER_H_
#define JAGPACKAGERENDERER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_PACKAGE_RENDERER              (jag_package_renderer_get_type())
#define JAG_PACKAGE_RENDERER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_package_renderer_get_type(), JagPackageRenderer))
#define JAG_PACKAGE_RENDERER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PACKAGE_RENDERER, JagPackageRendererClass))
#define JAG_IS_PACKAGE_RENDERER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PACKAGE_RENDERER))
#define JAG_IS_PACKAGE_RENDERER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PACKAGE_RENDERER))
#define JAG_PACKAGE_RENDERER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PACKAGE_RENDERER, JagPackageRendererClass))

typedef struct _JagPackageRenderer               JagPackageRenderer;
typedef struct _JagPackageRendererClass          JagPackageRendererClass;

struct _JagPackageRenderer {
	GObject parent;
};

struct _JagPackageRendererClass {
	GObjectClass parent_class;
};

GType jag_package_renderer_get_type();

JagPackageRenderer *jag_package_renderer_new();

G_END_DECLS

#endif /* JAGPACKAGERENDERER_H_ */
