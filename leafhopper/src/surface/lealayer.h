/*
   File:    lealayer.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 17, 2014
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

#ifndef LEALAYER_H_
#define LEALAYER_H_

#include "leaidrawable.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_LAYER              (lea_layer_get_type())
#define LEA_LAYER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_layer_get_type(), LeaLayer))
#define LEA_LAYER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_LAYER, LeaLayerClass))
#define LEA_IS_LAYER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_LAYER))
#define LEA_IS_LAYER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_LAYER))
#define LEA_LAYER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_LAYER, LeaLayerClass))


typedef struct _LeaLayer               LeaLayer;
typedef struct _LeaLayerPrivate        LeaLayerPrivate;
typedef struct _LeaLayerClass          LeaLayerClass;


struct _LeaLayer {
	GtkWidget parent;
};

struct _LeaLayerClass {
	GtkWidgetClass parent_class;
};


GType lea_layer_get_type();

void lea_layer_construct(LeaLayer *layer);
LeaLayer *lea_layer_new();

void lea_layer_add_drawable(LeaLayer *layer, LeaIDrawable *drawable);
void lea_layer_remove_drawable(LeaLayer *layer, LeaIDrawable *drawable);

gboolean lea_layer_has_drawables(LeaLayer *layer);

G_END_DECLS

#endif /* LEALAYER_H_ */
