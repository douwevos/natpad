/*
   File:    jagsrcfolderrenderer.h
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

#ifndef JAGSRCFOLDERRENDERER_H_
#define JAGSRCFOLDERRENDERER_H_

#include <caterpillar.h>
#include <moose.h>
#include <mooseexplorer.h>

G_BEGIN_DECLS

#define JAG_TYPE_SRC_FOLDER_RENDERER              (jag_src_folder_renderer_get_type())
#define JAG_SRC_FOLDER_RENDERER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_src_folder_renderer_get_type(), JagSrcFolderRenderer))
#define JAG_SRC_FOLDER_RENDERER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRC_FOLDER_RENDERER, JagSrcFolderRendererClass))
#define JAG_IS_SRC_FOLDER_RENDERER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRC_FOLDER_RENDERER))
#define JAG_IS_SRC_FOLDER_RENDERER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRC_FOLDER_RENDERER))
#define JAG_SRC_FOLDER_RENDERER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRC_FOLDER_RENDERER, JagSrcFolderRendererClass))

typedef struct _JagSrcFolderRenderer               JagSrcFolderRenderer;
typedef struct _JagSrcFolderRendererClass          JagSrcFolderRendererClass;

struct _JagSrcFolderRenderer {
	GObject parent;
};

struct _JagSrcFolderRendererClass {
	GObjectClass parent_class;
};

GType jag_src_folder_renderer_get_type();

JagSrcFolderRenderer *jag_src_folder_renderer_new();

void jag_src_folder_renderer_draw_package(cairo_t *cairo, double xoffset, int yoffset, double size, gboolean non_empty);

G_END_DECLS

#endif /* JAGSRCFOLDERRENDERER_H_ */
