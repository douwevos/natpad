/*
   File:    leadraglayer.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 18, 2014
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

#ifndef LEADRAGLAYER_H_
#define LEADRAGLAYER_H_

#include "lealayer.h"
#include "drag/leaigrip.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_DRAG_LAYER              (lea_drag_layer_get_type())
#define LEA_DRAG_LAYER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_drag_layer_get_type(), LeaDragLayer))
#define LEA_DRAG_LAYER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_DRAG_LAYER, LeaDragLayerClass))
#define LEA_IS_DRAG_LAYER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_DRAG_LAYER))
#define LEA_IS_DRAG_LAYER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_DRAG_LAYER))
#define LEA_DRAG_LAYER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_DRAG_LAYER, LeaDragLayerClass))


typedef struct _LeaDragLayer               LeaDragLayer;
typedef struct _LeaDragLayerPrivate        LeaDragLayerPrivate;
typedef struct _LeaDragLayerClass          LeaDragLayerClass;


struct _LeaDragLayer {
	LeaLayer parent;
};

struct _LeaDragLayerClass {
	LeaLayerClass parent_class;
};


GType lea_drag_layer_get_type();

LeaDragLayer *lea_drag_layer_new();

void lea_drag_layer_set_active_grip(LeaDragLayer *drag_layer, LeaIGrip *active_grip);
gboolean lea_drag_layer_is_active(LeaDragLayer *drag_layer);
void lea_drag_layer_rewrite_event(LeaDragLayer *drag_layer, GdkEvent *event);

G_END_DECLS

#endif /* LEADRAGLAYER_H_ */
