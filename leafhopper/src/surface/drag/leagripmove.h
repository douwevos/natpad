/*
   File:    leagripmove.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 22, 2014
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

#ifndef LEAGRIPMOVE_H_
#define LEAGRIPMOVE_H_

#include "../leasurface.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_GRIP_MOVE              (lea_grip_move_get_type())
#define LEA_GRIP_MOVE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_grip_move_get_type(), LeaGripMove))
#define LEA_GRIP_MOVE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_GRIP_MOVE, LeaGripMoveClass))
#define LEA_IS_GRIP_MOVE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_GRIP_MOVE))
#define LEA_IS_GRIP_MOVE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_GRIP_MOVE))
#define LEA_GRIP_MOVE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_GRIP_MOVE, LeaGripMoveClass))


typedef struct _LeaGripMove               LeaGripMove;
typedef struct _LeaGripMovePrivate        LeaGripMovePrivate;
typedef struct _LeaGripMoveClass          LeaGripMoveClass;


struct _LeaGripMove {
	GObject parent;
};

struct _LeaGripMoveClass {
	GObjectClass parent_class;
};


GType lea_grip_move_get_type();

LeaGripMove *lea_grip_move_new(LeaSurface *surface, int panel_idx);

G_END_DECLS

#endif /* LEAGRIPMOVE_H_ */
