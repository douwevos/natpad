/*
   File:    leaframe.h
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

#ifndef LEAFRAME2_H_
#define LEAFRAME2_H_

#include "event/leaieventlistener.h"
#include "surface/leasurfacehandler.h"
#include "surface/leadraglayer.h"
#include "action/leaactiongroup.h"
#include "statusbar/leastatusbar.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_FRAME              (lea_frame_get_type())
#define LEA_FRAME(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_frame_get_type(), LeaFrame))
#define LEA_FRAME_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_FRAME, LeaFrameClass))
#define LEA_IS_FRAME(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_FRAME))
#define LEA_IS_FRAME_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_FRAME))
#define LEA_FRAME_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_FRAME, LeaFrameClass))


typedef struct _LeaFrame               LeaFrame;
typedef struct _LeaFramePrivate        LeaFramePrivate;
typedef struct _LeaFrameClass          LeaFrameClass;


struct _LeaFrame {
	GtkContainer parent;
};

struct _LeaFrameClass {
	GtkContainerClass parent_class;
};


GType lea_frame_get_type();

LeaFrame *lea_frame_new();

LeaSurfaceHandler *lea_frame_get_surface_handler(LeaFrame *frame);

LeaDragLayer *lea_frame_get_drag_layer(LeaFrame *frame);

LeaStatusBar *lea_frame_get_status_bar(LeaFrame *frame);
LeaSurface *lea_frame_get_focus_surface(LeaFrame *frame);
gboolean lea_frame_get_focus_active(LeaFrame *frame);


void lea_frame_merge_action_group(LeaFrame *frame, LeaActionGroup *action_group);

void lea_frame_unmerge_action_group(LeaFrame *frame, LeaActionGroup *action_group);

void lea_frame_update_menu(LeaFrame *frame);

void lea_frame_update_key_bindings(LeaFrame *frame);

void lea_frame_add_event_listener(LeaFrame *frame, LeaIEventListener *listener);
void lea_frame_remove_event_listener(LeaFrame *frame, LeaIEventListener *listener);

G_END_DECLS

#endif /* LEAFRAME2_H_ */
